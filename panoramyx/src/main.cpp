#include <mpi.h>

#include <fstream>
#include <iostream>

#include "../../libs/autis/autis/include/core/Scanner.hpp"
#include "../../libs/autis/autis/include/xcsp/AutisXcspParserAdapter.hpp"
#include "../../libs/autis/libs/universe/libs/easy-jni/easyjni/JavaVirtualMachineBuilder.h"
#include "../../libs/autis/libs/universe/universe/include/utils/UniverseJavaSolverFactory.hpp"
#include "../include/core/FinalConsistencyChecker.hpp"
#include "../include/core/PartialConsistencyChecker.hpp"
#include "../include/decomposition/LexicographicCubeGenerator.hpp"
#include "../include/network/INetworkCommunication.hpp"
#include "../include/network/MPINetworkCommunication.hpp"
#include "../include/optim/decomposition/LinearRangeIterator.hpp"
#include "../include/optim/decomposition/RangeBaseAllocationStrategy.hpp"
#include "../include/solver/AbstractParallelSolver.hpp"
#include "../include/solver/EPSSolver.hpp"
#include "../include/solver/GauloisSolver.hpp"
#include "../include/solver/PortfolioSolver.hpp"
#include "../include/utils/VectorStream.hpp"
#include "loguru.hpp"
#include "argparse/argparse.hpp"
#include "../include/core/panoramyx.hpp"

using namespace Panoramyx;
using namespace easyjni;

/**
 * @date 28/09/22
 * @file ${HEADER_FILENAME}
 * @brief
 * @author Thibault Falque
 * @author Romain Wallon
 * @license This project is released under the GNU LGPL3 License.
 */

argparse::ArgumentParser createPortfolioParser(){
    argparse::ArgumentParser portfolio("portfolio");
    portfolio.add_argument("-a","--allocation-strategy")
        .default_value(std::string{"Linear"})
        .action([](const std::string& value) {
            static const std::vector<std::string> choices = { "Linear" };
            if (std::find(choices.begin(), choices.end(), value) != choices.end()) {
            return value;
            }
            throw runtime_error("Unknown allocation strategy "+value);
        });
    return portfolio;
}

argparse::ArgumentParser createEPSParser(){
    argparse::ArgumentParser eps("eps");
    eps.add_argument("-g","--cube-generator")
        .default_value(std::string{"Lexicographic"})
        .action([](const std::string& value) {
            static const std::vector<std::string> choices = { "Lexicographic" };
            if (std::find(choices.begin(), choices.end(), value) != choices.end()) {
            return value;
            }
            throw runtime_error("Unknown cube generator "+value);
        });
    return eps;
}

void addCommonArguments(argparse::ArgumentParser& parser){
    parser.add_argument("-i","--instance").required().help("specify the instance file.");
    parser.add_argument("-j","--jars").nargs(argparse::nargs_pattern::any).help("specify the list of jars.");
    parser.add_argument("--java-options").help("specify jvm options");
    parser.add_argument("-c","--network-communicator")
        .default_value(std::string{"MPI"})
        .action([](const std::string& value) {
            static const std::vector<std::string> choices = { "MPI" };
            if (std::find(choices.begin(), choices.end(), value) != choices.end()) {
            return value;
            }
            throw runtime_error("Unknown communicator value "+value);
        });
    parser.add_argument("--factory")
        .default_value<std::vector<std::string>>({})
        .append()
        .help("specify factory of the solver");
    parser.add_argument("--verbosity")
        .default_value<std::vector<int>>({})
        .append()
        .help("specify the verbosity of the solver");

}


argparse::ArgumentParser parse(int argc,char**argv){
    argparse::ArgumentParser program("Panoramyx",PANO_VERSION);
    auto portfolio = createPortfolioParser();
    auto eps = createEPSParser();
    program.add_subparser(portfolio);
    program.add_subparser(eps);
    addCommonArguments(program);
    program.parse_args(argc,argv);
    return program;
}



int main(int argc, char** argv) {
    try {
        parse(argc, argv);   // Example: ./main --input_files config.yml System.xml
    }catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::exit(1);
    }

    // int toto;
    // MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &toto);
    // loguru::init(argc, argv);
    // INetworkCommunication* comm = MPINetworkCommunication::getInstance();
    // JavaVirtualMachineBuilder builder = JavaVirtualMachineBuilder();
    // builder.addToClasspath("/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/jars/juniverse.jar");
    // builder.addToClasspath("/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/jars/ACE.jar");
    // builder.addToClasspath("/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/jars/aceurancetourix.jar");
    // builder.addToClasspath("/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/jars/xcsp3-tools.jar");
    // builder.addToClasspath("/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/jars/javax.json-1.1.2.jar");
    // builder.addToClasspath("/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/jars/javax.json-api-1.1.2.jar");
    // builder.addOption("-ea");
    // builder.setVersion(JNI_VERSION_10);
    // auto jvm = builder.buildJavaVirtualMachine();
    // JavaVirtualMachineRegistry::set(jvm);
    // if (comm->getId() == 0) {
    //     std::cout << "mon pid" << getpid() << std::endl;
    //     std::string instance = "/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/CoinsGrid-28-12_c22.xml";
    //     Universe::UniverseJavaSolverFactory factory("fr/univartois/cril/aceurancetourix/PreprocAceSolverFactory");
    //     // auto ace = factory.createCspSolver();
    //     // ace->setVerbosity(-1);
    //     // std::ifstream input(instance);
    //     // Autis::Scanner scanner(input);
    //     // auto parser = make_unique<Autis::AutisXCSPParserAdapter>(scanner, dynamic_cast<Universe::IUniverseCspSolver*>(ace));
    //     // parser->parse();
    //     // auto cubeGenerator = new LexicographicCubeGenerator(100);
    //     // cubeGenerator->setSolver(ace);
    //     // cubeGenerator->setConsistencyChecker(new PartialConsistencyChecker(ace));
    //     Abraracourcix* chief = new PortfolioSolver(comm, new RangeBaseAllocationStrategy([](auto min, auto max, auto nb) { return new LinearRangeIterator(min, max, nb); }));
    //     for (int i = 1; i < comm->nbProcesses(); i++) {
    //         chief->addSolver(new RemoteSolver(i));
    //     }
    //     chief->loadInstance(instance);
    //     auto r = chief->solve();
    //     std::cout << (int)r << std::endl;
    // } else {
    //     std::cout << "mon pid" << getpid() << std::endl;
    //     Universe::UniverseJavaSolverFactory factory("fr/univartois/cril/aceurancetourix/AceSolverFactory");
    //     auto ace = factory.createCspSolver();
    //     ace->setVerbosity(1);
    //     GauloisSolver* gaulois = new GauloisSolver(ace, comm);
    //     gaulois->start();
    // }
    // getchar();
    // MPI_Finalize();
    return 0;
}