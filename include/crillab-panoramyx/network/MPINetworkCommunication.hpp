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
 * @file MPINetworkCommunication.hpp
 * @brief An MPI implementation of INetworkCommunication.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_MPINETWORKCOMMUNICATION_HPP
#define PANORAMYX_MPINETWORKCOMMUNICATION_HPP

#include "INetworkCommunication.hpp"

namespace Panoramyx {

    /**
     * The MPINetworkCommunication is an implementation of INetworkCommunication
     * that relies on MPI (Message Passing Interface) to communicate between
     * different processes.
     */
    class MPINetworkCommunication : public Panoramyx::INetworkCommunication {

    private:

        /**
         * The unique instance of this class.
         */
        static Panoramyx::INetworkCommunication *instance;

        /**
         * The rank of the current communicator, as assigned by MPI.
         */
        int rank = -1;

        /**
         * The size of MPI's world, i.e., the number of processes.
         */
        int worldSize = -1;

    public:

        /**
         * Destroys this MPINetworkCommunication.
         */
        ~MPINetworkCommunication() override = default;

        /**
         * Gives the unique instance of MPINetworkCommunication.
         *
         * @return The unique instance of MPINetworkCommunication.
         */
        static INetworkCommunication *getInstance();

        /**
         * Gives the identifier of the current communicator.
         *
         * @return The identifier of the current communicator.
         */
        int getId() override;

        /**
         * Gives the number of processes that are currently communicating.
         *
         * @return The number of processes.
         */
        int nbProcesses() override;

        /**
         * Executes the given runnable as many times as needed by this strategy.
         *
         * @param runnable The runnable to execute.
         */
        void start(std::function<void()> runnable) override;

        /**
         * Receives a message.
         *
         * @param tag The tag identifying the kind of the message to read.
         * @param src The identifier of the source of the message (i.e., the communicator that sent it).
         * @param size The maximum size of the message to read.
         *
         * @return The received message.
         */
        Message *receive(int tag, int src, unsigned long size) override;

        /**
         * Sends a message.
         *
         * @param message The message to send.
         * @param dest The identifier of the destination of the message (i.e., the communicator that will receive it).
         */
        void send(Message *message, int dest) override;

        /**
         * Finalizes the communication between the different communicators.
         */
        void finalize() override;

    };

}

#endif
