#include <iostream>
#include <mpi.h>
#include <fstream>
#include "../include/network/INetworkCommunication.hpp"
#include "../include/network/MPINetworkCommunication.hpp"
#include "../include/solver/AbstractParallelSolver.hpp"
#include "../include/solver/PortfolioSolver.hpp"
#include "../include/solver/GauloisSolver.hpp"
#include "loguru.hpp"
#include "../include/solver/EPSSolver.hpp"
#include "../include/utils/VectorStream.hpp"
#include "../include/decomposition/LexicographicCubeGenerator.hpp"
#include "../include/core/FinalConsistencyChecker.hpp"
#include "../../libs/autis/autis/include/core/Scanner.hpp"
#include "../../libs/autis/autis/include/xcsp/AutisXcspParserAdapter.hpp"

#include "../../libs/autis/libs/universe/universe/include/utils/UniverseJavaSolverFactory.hpp"
#include "../../libs/autis/libs/universe/libs/easy-jni/easyjni/JavaVirtualMachineBuilder.h"
#include "../include/core/PartialConsistencyChecker.hpp"

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
int main(int argc, char** argv){
    MPI_Init(&argc,&argv);
    loguru::init(argc, argv);
    INetworkCommunication* comm = MPINetworkCommunication::getInstance();
    JavaVirtualMachineBuilder builder = JavaVirtualMachineBuilder();
    builder.addToClasspath("/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/jars/juniverse.jar");
    builder.addToClasspath("/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/jars/ACE.jar");
    builder.addToClasspath("/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/jars/aceurancetourix.jar");
    builder.addToClasspath("/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/jars/xcsp3-tools.jar");
    builder.addToClasspath("/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/jars/javax.json-1.1.2.jar");
    builder.addToClasspath("/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/jars/javax.json-api-1.1.2.jar");
    builder.setVersion(JNI_VERSION_10);
    auto jvm = builder.buildJavaVirtualMachine();
    JavaVirtualMachineRegistry::set(jvm);
    if(comm->getId()==0){
        std::string instance = "/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/AllInterval.xml";
        Universe::UniverseJavaSolverFactory factory("fr/univartois/cril/aceurancetourix/PreprocAceSolverFactory");
        auto ace = factory.createCspSolver();
        ace->setVerbosity(-1);
        std::ifstream input(instance);
        Autis::Scanner scanner(input);
        auto parser = make_unique<Autis::AutisXCSPParserAdapter>(scanner, dynamic_cast<Universe::IUniverseCspSolver *>(ace));
        parser->parse();
        auto cubeGenerator = new LexicographicCubeGenerator(100);
        cubeGenerator->setSolver(ace);
        cubeGenerator->setConsistencyChecker(new PartialConsistencyChecker(ace));
        Abraracourcix* chief = new EPSSolver(comm,cubeGenerator);
        for(int i=1;i<comm->nbProcesses();i++){
            chief->addSolver(new RemoteSolver(i));
        }
        chief->loadInstance(instance);
        auto r=chief->solve();
        std::cout<<(int)r<<std::endl;
    }else{
        Universe::UniverseJavaSolverFactory factory("fr/univartois/cril/aceurancetourix/AceSolverFactory");
        auto ace = factory.createCspSolver();
        ace->setVerbosity(-1);
        GauloisSolver* gaulois = new GauloisSolver(ace,comm);
        gaulois->start();
    }
    getchar();
    MPI_Finalize();
    return 0;
}