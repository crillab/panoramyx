/**
* @date 12/10/22
* @file MpiNetworkCommunication.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/

#include <cstdlib>
#include "../../include/network/MpiNetworkCommunication.hpp"
#include <mpi.h>

namespace Panoramyx {

/**
@class MPINetworkCommunication
@brief Definition of the class MPINetworkCommunication. 
@file MpiNetworkCommunication.cpp
*/

    INetworkCommunication *MPINetworkCommunication::instance = nullptr;

    int MPINetworkCommunication::getId() {
        if (rank < 0) {
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        }
        return rank;
    }

    int MPINetworkCommunication::nbProcesses() {
        if (worldSize < 0) {
            MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
        }
        return worldSize;
    }

    Message *MPINetworkCommunication::receive(int tag, int src, unsigned long size) {
        auto* m = (Message*)(malloc(size));
        MPI_Recv(m,size,MPI_BYTE,src,tag,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        return m;
    }

    void MPINetworkCommunication::send(Message *m, int dest) {
        m->src=getId();
        MPI_Send(m,sizeof(Message)+m->size,MPI_BYTE,dest,m->tag,MPI_COMM_WORLD);
    }

    INetworkCommunication *MPINetworkCommunication::getInstance() {
        if (!instance) {
            instance=new MPINetworkCommunication();
            MPI_Comm_set_errhandler(MPI_COMM_WORLD,MPI::ERRORS_THROW_EXCEPTIONS);

        }
        return instance;
    }
} // Panoramyx