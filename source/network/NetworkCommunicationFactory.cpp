/**
 * PANORAMYX - Programming pArallel coNstraint sOlveRs mAde aMazingly easY.
 * Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 * If not, see {@link http://www.gnu.org/licenses}.
 */

/**
 * @file NetworkCommunicationFactory.cpp
 * @brief Provides a factory for implementations of INetworkCommunication.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <mpi.h>

#include <crillab-panoramyx/network/NetworkCommunicationFactory.hpp>
#include <crillab-panoramyx/network/MPINetworkCommunication.hpp>
#include <crillab-panoramyx/network/ThreadCommunication.hpp>

using namespace Panoramyx;

NetworkCommunicationFactory::NetworkCommunicationFactory(int *argc, char ***argv) :
        argc(argc),
        argv(argv) {
    // Nothing to do: everything is already initialized.
}

INetworkCommunication *NetworkCommunicationFactory::createMPINetworkCommunication() {
    int provided = 0;
    MPI_Init_thread(argc, argv, MPI_THREAD_MULTIPLE, &provided);
    return MPINetworkCommunication::getInstance();
}

INetworkCommunication *NetworkCommunicationFactory::createThreadCommunication(int nbThreads) {
    return new ThreadCommunication(nbThreads);
}
