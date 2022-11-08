#include <iostream>
#include <mpi.h>
#include "../include/network/INetworkCommunication.hpp"
#include "../include/network/MpiNetworkCommunication.hpp"
#include "../include/network/MessageBuilder.hpp"

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
    Panoramyx::INetworkCommunication* comm = Panoramyx::MPINetworkCommunication::getInstance();
    if(comm->getId()==0){
        Panoramyx::MessageBuilder mb = Panoramyx::MessageBuilder();
        Panoramyx::Message* m = mb.forMethod("coucou").withParameter(42).withTag(7).build();
        std::cout<<m->size<<std::endl;
        std::cout<<m->nbParameters<<std::endl;
        std::cout<<m->methodName<<std::endl;
        std::cout<<*((int*)m->parameters)<<std::endl;
        comm->send(m,1);
    }else{
        Panoramyx::Message* m=comm->receive(7,0,1024);
        std::cout<<m->size<<std::endl;
        std::cout<<m->nbParameters<<std::endl;
        std::cout<<m->methodName<<std::endl;
        std::cout<<*((int*)m->parameters)<<std::endl;
    }
    MPI_Finalize();
    return 0;
}