#include <iostream>
#include <mpi.h>
#include "../include/network/INetworkCommunication.hpp"
#include "../include/network/MpiNetworkCommunication.hpp"
#include "../include/solver/AbstractParallelSolver.hpp"
#include "../include/solver/PortfolioSolver.hpp"
#include "../include/solver/GauloisSolver.hpp"
#include "../../libs/autis/libs/universe/universe/include/utils/UniverseJavaSolverFactory.hpp"
#include "../../libs/autis/libs/universe/libs/easy-jni/easyjni/JavaVirtualMachineBuilder.h"
#include "../../libs/autis/libs/universe/libs/easy-jni/easyjni/JavaVirtualMachineRegistry.h"
#include "loguru.hpp"

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
    LOG_F(INFO, "I'm hungry for some %.3f!", 3.14159);
    INetworkCommunication* comm = MPINetworkCommunication::getInstance();
    if(comm->getId()==0){
        Abraracourcix* chief = new PortfolioSolver(comm);
        for(int i=1;i<comm->nbProcesses();i++){
            chief->addSolver(new RemoteSolver(i));
        }
        auto r=chief->solve("/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/panoramyx/AllInterval-006.xml");
        std::cout<<(int)r<<std::endl;
    }else{
        JavaVirtualMachineBuilder builder = JavaVirtualMachineBuilder();
        builder.addToClasspath("/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/jars/juniverse.jar");
        builder.addToClasspath("/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/jars/ACE.jar");
        builder.addToClasspath("/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/jars/assurancetourix.jar");
        builder.addToClasspath("/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/jars/xcsp3-tools.jar");
        builder.addToClasspath("/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/jars/javax.json-1.1.2.jar");
        builder.addToClasspath("/home/falque/Travail/Doctorat/CP/Workspace/panoramyx/jars/javax.json-api-1.1.2.jar");
        builder.setVersion(JNI_VERSION_10);
        auto jvm = builder.buildJavaVirtualMachine();
        JavaVirtualMachineRegistry::set(jvm);
        Universe::UniverseJavaSolverFactory factory("fr/univartois/cril/aceurancetourix/AceSolverFactory");
        auto ace = factory.createCspSolver();
        ace->setVerbosity(-1);
        GauloisSolver* gaulois = new GauloisSolver(ace,comm);
        gaulois->start();
    }
    MPI_Finalize();
    return 0;
}