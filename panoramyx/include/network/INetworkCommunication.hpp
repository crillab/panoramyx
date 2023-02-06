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
 * @file INetworkCommunication.hpp
 * @brief Defines a strategy for network communication.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_INETWORKCOMMUNICATION_HPP
#define PANORAMYX_INETWORKCOMMUNICATION_HPP

#include <cstring>

#include "Message.hpp"

namespace Panoramyx {

    /**
     * The INetworkCommunication defines en interface for a strategy allowing to
     * exchange information through the network.
     * This interface is designed to allow communication using various approaches,
     * such as, e.g., MPI, UNIX pipes, etc.
     */
    class INetworkCommunication {

    public:

        /**
         * Destroys this INetworkCommunication.
         */
        virtual ~INetworkCommunication() = default;

        /**
         * Gives the identifier of the current communicator.
         *
         * @return The identifier of the current communicator.
         */
        virtual int getId() = 0;

        /**
         * Gives the number of processes that are currently communicating.
         *
         * @return The number of processes.
         */
        virtual int nbProcesses() = 0;

        /**
         * Receives a message.
         *
         * @param tag The tag identifying the kind of the message to read.
         * @param src The identifier of the source of the message (i.e., the communicator that sent it).
         * @param size The maximum size of the message to read.
         *
         * @return The received message.
         */
        virtual Message *receive(int tag, int src, unsigned long size) = 0;

        /**
         * Sends a message.
         *
         * @param message The message to send.
         * @param dest The identifier of the destination of the message (i.e., the communicator that will receive it).
         */
        virtual void send(Message *message, int dest) = 0;


        virtual void finalize()=0;
    };

}

#endif
