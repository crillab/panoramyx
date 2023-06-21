/**
* @date 06/02/23
* @file NetworkCommunicationFactory.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/

#include <mpi.h>
#include "../../include/network/NetworkCommunicationFactory.hpp"
#include "../../include/network/MPINetworkCommunication.hpp"
#include "../../include/network/ThreadCommunication.hpp"

namespace Panoramyx {

/**
@class NetworkCommunicationFactory
@brief Definition of the class NetworkCommunicationFactory. 
@file NetworkCommunicationFactory.cpp
*/

    NetworkCommunicationFactory::NetworkCommunicationFactory(int *argc, char ***argv):argc(argc),argv(argv) {

    }

    INetworkCommunication *NetworkCommunicationFactory::createMPINetworkCommunication() {
        int provided;
        MPI_Init_thread(argc, argv, MPI_THREAD_MULTIPLE, &provided);
        return MPINetworkCommunication::getInstance();
    }

    INetworkCommunication *NetworkCommunicationFactory::createThreadCommunication(int nb) {
        return new ThreadCommunication(nb);
    }

} // Panoramyx