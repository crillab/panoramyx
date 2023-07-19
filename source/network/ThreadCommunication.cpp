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
 * @file ThreadCommunication.cpp
 * @brief A thread-based implementation of INetworkCommunication.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <crillab-panoramyx/network/ThreadCommunication.hpp>

using namespace std;

using namespace Panoramyx;

ThreadCommunication::ThreadCommunication(int nbThreads) :
        nbThreads(nbThreads) {
    // Nothing to do: everything is already initialized.
}

int ThreadCommunication::getId() {
    return ids[this_thread::get_id()];
}

int ThreadCommunication::nbProcesses() {
    return (int) ids.size();
}

void ThreadCommunication::start(function<void()> runnable) {
    for (int i = 0; i < nbThreads; i++) {
        threads.emplace_back(runnable);
        ids[threads.back().get_id()] = i;
        queues.emplace_back();
    }
}

Message *ThreadCommunication::receive(int tag, int src, unsigned long size) {
    // FIXME PANO_ANY_TAG and PANO_ANY_SOURCE are not supported with this approach.
    pair<int, int> k = make_pair(tag, src);
    newqmutex.lock();
    if (queues[this->getId()].find(k) == queues[this->getId()].end()) {
        queues[this->getId()][k] = new BlockingDeque<Message *>();
    }
    newqmutex.unlock();

    return queues[this->getId()][{tag, src}]->get();
}

void ThreadCommunication::send(Message *message, int dest) {
    // FIXME PANO_ANY_TAG and PANO_ANY_SOURCE are not supported with this approach.
    Message *copiedMessage = static_cast<Message *>(malloc(sizeof(Message) + message->size));
    memcpy(copiedMessage, message, sizeof(Message) + message->size);
    copiedMessage->src = this->getId();

    pair<int, int> k = make_pair(message->tag, message->src);
    newqmutex.lock();
    if (queues[dest].find(k) == queues[dest].end()) {
        queues[dest][k] = new BlockingDeque<Message *>();
    }
    newqmutex.unlock();

    queues[dest][{message->tag, message->src}]->add(copiedMessage);
}

void ThreadCommunication::finalize() {
    for (int i = 0; i < nbThreads; i++) {
        threads[i].join();
    }
}
