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
 * @file MPINetworkCommunication.cpp
 * @brief An MPI implementation of INetworkCommunication.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <cstdlib>

#include <mpi.h>

#include "../../include/network/MPINetworkCommunication.hpp"

using namespace Panoramyx;

INetworkCommunication *MPINetworkCommunication::instance = nullptr;

INetworkCommunication *MPINetworkCommunication::getInstance() {
    if (!instance) {
        instance = new MPINetworkCommunication();
        MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI::ERRORS_THROW_EXCEPTIONS);
    }
    return instance;
}

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
    auto *m = (Message *) (malloc(size));
    MPI_Recv(m, (int) size, MPI_BYTE, src, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return m;
}

void MPINetworkCommunication::send(Message *m, int dest) {
    m->src = getId();
    MPI_Send(m, (int) (sizeof(Message) + m->size), MPI_BYTE, dest, m->tag, MPI_COMM_WORLD);
}

void MPINetworkCommunication::finalize() {
    MPI_Finalize();
}
