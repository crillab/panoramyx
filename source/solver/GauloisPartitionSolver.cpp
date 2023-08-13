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
 * @file GauloisSolver.hpp
 * @brief Provides a solver implementation encapsulating the communication with Abraracourcix.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <loguru.hpp>

#include <crillab-panoramyx/solver/GauloisPartitionSolver.hpp>
#include "mpi.h"
#include "crillab-panoramyx/network/MessageBuilder.hpp"

using namespace Panoramyx;

GauloisPartitionSolver::GauloisPartitionSolver(Panoramyx::PartitionSolver *solver,
                                               Panoramyx::INetworkCommunication *comm) :
        Panoramyx::GauloisSolver(solver, comm),
        partitionSolver(solver) {

}
void GauloisPartitionSolver::start() {
    while (partitionSolver->getRunningSolvers() > 0) {
        auto message = comm->receive(PANO_ANY_TAG, MPI_ANY_SOURCE, PANO_DEFAULT_MESSAGE_SIZE);
        LOG_F(INFO, "GauloisPartitionSolver: readMessage - %s", message->name);
        if (NAME_OF(message, IS(PANO_MESSAGE_END_SEARCH))) {
            partitionSolver->endSearch();
        } else if (NAME_OF(message, IS(PANO_MESSAGE_END_SEARCH_ACK))) {
            partitionSolver->readEnd(message);
        } else {
            readMessage(message);
        }
        free(message);
    }
    LOG_F(INFO, "After loop message");
}