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
#include <functional>

#include <mpi.h>

#include <crillab-panoramyx/network/MPINetworkCommunication.hpp>

using namespace std;

using namespace Panoramyx;

INetworkCommunication *MPINetworkCommunication::instance = nullptr;

INetworkCommunication *MPINetworkCommunication::getInstance() {
    if (instance == nullptr) {
        instance = new MPINetworkCommunication();
        MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_ARE_FATAL);
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

void MPINetworkCommunication::start(function<void()> runnable) {
    runnable();
}

Message *MPINetworkCommunication::receive(int tag, int src, unsigned long size) {
    auto *message = static_cast<Message *>(malloc(size));
    MPI_Recv(message, (int) size, MPI_BYTE, src, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return message;
}

void MPINetworkCommunication::send(Message *message, int dest) {
    message->src = getId();
    MPI_Send(message, (int) (sizeof(Message) + message->size), MPI_BYTE, dest, message->tag, MPI_COMM_WORLD);
}

void MPINetworkCommunication::finalize() {
    MPI_Finalize();
}
