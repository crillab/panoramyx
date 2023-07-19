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
 * @file ThreadCommunication.hpp
 * @brief A thread-based implementation of INetworkCommunication.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_THREADCOMMUNICATION_HPP
#define PANORAMYX_THREADCOMMUNICATION_HPP

#include <map>
#include <thread>
#include <vector>
#include <functional>

#include "INetworkCommunication.hpp"
#include "../utils/BlockingDeque.hpp"

namespace Panoramyx {

    /**
     * The ThreadCommunication is an implementation of INetworkCommunication
     * that relies on threads to run different operations in parallel and uses
     * blocking queues to communicate between threads.
     */
    class ThreadCommunication : public INetworkCommunication {

    private:

        /**
         * The number of threads to run.
         */
        int nbThreads;

        /**
         * The running threads.
         */
        std::vector<std::thread> threads;

        /**
         * The map associating to each thread identifier their communicator identifier.
         */
        std::map<std::thread::id, int> ids;

        /**
         * The queue that are used to exchange information between threads.
         */
        std::vector<std::map<std::pair<int, int>, Panoramyx::BlockingDeque<Message *> *>> queues;

        /**
         * The mutex used to add new communication queues.
         */
        std::mutex newqmutex;

    public:

        /**
         * Creates a new ThreadCommunication.
         *
         * @param nbThreads The number of threads to run.
         */
        explicit ThreadCommunication(int nbThreads);

        /**
         * Destroys this ThreadCommunication.
         */
        ~ThreadCommunication() override = default;

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
