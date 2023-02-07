#include <mpi.h>

#include <fstream>
#include <iostream>
#include <filesystem>

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
#include "../include/network/NetworkCommunicationFactory.hpp"
#include "../include/solver/AbstractSolverBuilder.hpp"
#include "../include/solver/EpsSolverBuilder.hpp"
#include "../include/core/NullConsistencyChecker.hpp"
#include "../include/solver/PortfolioSolverBuilder.hpp"


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


bool isJava(const std::string &s) {
    return std::find(s.begin(), s.end(), '/') != s.end();
}


argparse::ArgumentParser createPortfolioParser() {
    argparse::ArgumentParser portfolio("portfolio");
    portfolio.add_argument("-a", "--allocation-strategy")
            .default_value(std::string{"Linear"})
            .action([](const std::string &value) {
                static const std::vector<std::string> choices = {"Linear"};
                if (std::find(choices.begin(), choices.end(), value) != choices.end()) {
                    return value;
                }
                throw runtime_error("Unknown allocation strategy " + value);
            });
    return portfolio;
}

argparse::ArgumentParser createEPSParser() {
    argparse::ArgumentParser eps("eps");
    eps.add_argument("-g", "--cube-generator")
            .default_value<string>(std::string{"Lexicographic"})
            .action([](const std::string &value) {
                static const std::vector<std::string> choices = {"Lexicographic"};
                if (std::find(choices.begin(), choices.end(), value) != choices.end()) {
                    return value;
                }
                throw runtime_error("Unknown cube generator " + value);
            });
    eps.add_argument("-f", "--factor-cube-generator").default_value(30);
    eps.add_argument("--consistency-checker-strategy").default_value(std::string{"Null"})
            .action([](const std::string &value) {
                static const std::vector<std::string> choices = {"Null", "Partial", "Final"};
                if (std::find(choices.begin(), choices.end(), value) != choices.end()) {
                    return value;
                }
                throw runtime_error("Unknown consistency checker " + value);
            });
    eps.add_argument("--consistency-checker-solver");
    return eps;
}

void addCommonArguments(argparse::ArgumentParser &parser) {
    parser.add_argument("-i", "--instance").required().help("specify the instance file.");
    parser.add_argument("-l", "--log-directory").required().help("specify the directory of log files.");
    parser.add_argument("-j", "--jars").nargs(argparse::nargs_pattern::any).help("specify the list of jars.");
    parser.add_argument("--java-options").default_value<string>("").help("specify jvm options");
    parser.add_argument("-c", "--network-communicator")
            .default_value(std::string{"MPI"})
            .action([](const std::string &value) {
                static const std::vector<std::string> choices = {"MPI"};
                if (std::find(choices.begin(), choices.end(), value) != choices.end()) {
                    return value;
                }
                throw runtime_error("Unknown communicator value " + value);
            });
    parser.add_argument("--factory")
            .default_value<std::vector<std::string>>({})
            .append()
            .help("specify factory of the solver");
    parser.add_argument("--verbosity")
            .default_value<std::vector<int>>({})
            .scan<'i', int>()
            .append()
            .help("specify the verbosity of the solver");

}


argparse::ArgumentParser parse(int argc, char **argv) {
    argparse::ArgumentParser program("Panoramyx", PANO_VERSION);
    auto portfolio = createPortfolioParser();
    auto eps = createEPSParser();
    program.add_subparser(portfolio);
    program.add_subparser(eps);
    addCommonArguments(program);
    program.parse_args(argc, argv);
    return program;
}


INetworkCommunication *parseNetworkCommunication(argparse::ArgumentParser &program, int *argc, char ***argv) {
    NetworkCommunicationFactory networkCommunicationFactory(argc, argv);
    if (program.get<string>("network-communicator") == "MPI") {
        return networkCommunicationFactory.createMPINetworkCommunication();
    }
    throw runtime_error("invalid network communicator");
}

void parseConsistencyChecker(argparse::ArgumentParser &program, ICubeGenerator *cg) {
    const std::string &consistency = program.get<string>("consistency-checker-solver");
    Universe::IUniverseSolver *solver;
    if (isJava(consistency)) {
        Universe::UniverseJavaSolverFactory factory(consistency);
        solver = factory.createCspSolver();
        solver->setVerbosity(-1);
        cg->setSolver(solver);
    } else {
        //todo
    }
    const std::string &consistency_strategy = program.get<string>("consistency-checker-strategy");
    if (consistency_strategy == "Final") {
        cg->setConsistencyChecker(new FinalConsistencyChecker(solver));
    } else if (consistency_strategy == "Partial") {
        cg->setConsistencyChecker(new PartialConsistencyChecker(solver));
    } else {
        cg->setConsistencyChecker(new NullConsistencyChecker());
    }

}


ICubeGenerator *parseCubeGenerator(argparse::ArgumentParser &program, INetworkCommunication *networkCommunication) {
    if (program.get<string>("cube-generator") == "Lexicographic") {
        auto cg = new LexicographicCubeGenerator(
                networkCommunication->nbProcesses() * program.get<int>("factor-cube-generator"));
        parseConsistencyChecker(program, cg);
        return cg;
    }
    throw runtime_error("invalid network communicator");
}


std::vector<Configuration> parseSolverConfiguration(argparse::ArgumentParser &program) {
    std::vector<Configuration> configs;
    auto factories = program.get<std::vector<string>>("factory");
    auto verbosity = program.get<std::vector<int>>("verbosity");
    for (unsigned long i = 0; i < factories.size(); i++) {
        Configuration configuration;
        configuration.set("factory", factories[i]);
        if (verbosity.size() > i) {
            configuration.set("verbosity", verbosity[i]);
        } else {
            configuration.set("verbosity", -1);
        }
        configs.emplace_back(configuration);
    }
    return configs;
}

std::vector<std::string> splitJavaOptions(std::string opts) {
    std::vector<std::string> tokens;
    if (!opts.empty()) {
        std::string token;
        std::stringstream s(opts);
        while (getline(s, token, ',')) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

void buildJVM(argparse::ArgumentParser &parser) {
    auto jars = parser.get<std::vector<string>>("jars");
    auto opts = parser.get<string>("java-options");
    std::vector<std::string> javaOptions = splitJavaOptions(opts);
    if (!jars.empty()) {
        JavaVirtualMachineBuilder builder = JavaVirtualMachineBuilder();

        for (auto &j: jars) {
            builder.addToClasspath(j);
        }

        for (auto &j: javaOptions) {
            builder.addOption(j);
        }

        builder.setVersion(JNI_VERSION_10);

        auto jvm = builder.buildJavaVirtualMachine();
        JavaVirtualMachineRegistry::set(jvm);
    }
}

IAllocationStrategy *parseAllocationStrategy(argparse::ArgumentParser &program, INetworkCommunication *pCommunication) {
    if (program.get<string>("allocation-strategy") == "Linear") {
        return new RangeBaseAllocationStrategy([](auto min, auto max, auto nb) { return new LinearRangeIterator(min, max, nb); });
    }
    throw runtime_error("invalid network communicator");
}


int main(int argc, char **argv) {
    loguru::init(argc, argv);
    argparse::ArgumentParser program("Panoramyx", PANO_VERSION);
    auto portfolio = createPortfolioParser();
    auto eps = createEPSParser();
    program.add_subparser(portfolio);
    program.add_subparser(eps);
    addCommonArguments(program);
    program.parse_args(argc, argv);
    auto networkCommunication = parseNetworkCommunication(program, &argc, &argv);
    buildJVM(program);
    if (networkCommunication->getId() == 0) {
        AbstractParallelSolver *chief;
        AbstractSolverBuilder *asb;
        if (program.is_subcommand_used("eps")) {
            asb = (new EPSSolverBuilder())->withCubeGenerator(
                    parseCubeGenerator(program.at<argparse::ArgumentParser>("eps"),
                                       networkCommunication))->withNetworkCommunicator(
                    networkCommunication)->withJavaOption(
                    splitJavaOptions(program.get<string>("java-options")))->withJars(
                    program.get<std::vector<string>>("jars"));

        }else{
            asb = (new PortfolioSolverBuilder())->withAllocationStrategy(
                    parseAllocationStrategy(program.at<argparse::ArgumentParser>("portfolio"),
                                       networkCommunication))->withNetworkCommunicator(
                    networkCommunication)->withJavaOption(
                    splitJavaOptions(program.get<string>("java-options")))->withJars(
                    program.get<std::vector<string>>("jars"));

        }
        chief = asb->build();

        for (int i = 1; i < networkCommunication->nbProcesses(); i++) {
            chief->addSolver(new RemoteSolver(i));
        }
        chief->loadInstance(program.get<string>("instance"));
        auto r = chief->solve();
        std::cout << (int) r << std::endl;
    } else {
        auto configs = parseSolverConfiguration(program);
        auto logdir = program.get<string>("log-directory");
        if (!std::filesystem::is_directory(logdir)) {
            std::filesystem::create_directory(logdir);
        }

        auto localConfig = configs[networkCommunication->getId() % configs.size()];
        auto factoryString = localConfig.get<string>("factory");
        Universe::IUniverseSolver *solver;
        if (isJava(factoryString)) {
            Universe::UniverseJavaSolverFactory factory(factoryString);
            solver = factory.createCspSolver();

        } else {
            //todo
        }
        solver->setVerbosity(localConfig.get<int>("verbosity"));
        auto *gaulois = new GauloisSolver(solver, networkCommunication);
        gaulois->setLogFile(logdir + std::filesystem::path::preferred_separator + "log_gaulois_" +
                            std::to_string(networkCommunication->getId())+"_"+std::to_string(getpid())+".log");
        gaulois->start();

    }

    networkCommunication->finalize();



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


