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
 * @file RemoteConstraint.cpp
 * @brief Provides the representation of a Universe constraint that is defined in a remote solver.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <crillab-except/except.hpp>

#include <crillab-panoramyx/network/MessageBuilder.hpp>
#include <crillab-panoramyx/problem/RemoteConstraint.hpp>

using namespace std;

using namespace Except;
using namespace Panoramyx;
using namespace Universe;

RemoteConstraint::RemoteConstraint(
        INetworkCommunication *communicator, std::mutex &mutex, int solverRank, int constraintIndex) :
        communicator(communicator),
        mutex(mutex),
        solverRank(solverRank),
        constraintIndex(constraintIndex) {
    // Nothing to do: everything is already initialized.
}

const vector<IUniverseVariable *> &RemoteConstraint::scope() {
    throw UnsupportedOperationException("Variables are too far (far) away!");
}

void RemoteConstraint::setIgnored(bool ignored) {
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_CONSTRAINT_SET_IGNORED);
    Message *m = mb.withTag(PANO_TAG_SOLVE).withParameter(constraintIndex).withParameter(ignored).build();
    communicator->send(m, solverRank);
    free(m);
}

const bool RemoteConstraint::isIgnored() const {
    mutex.lock();
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_CONSTRAINT_IS_IGNORED)
            .withParameter(constraintIndex);
    Message *m = mb.withTag(PANO_TAG_RESPONSE).build();
    communicator->send(m, solverRank);
    free(m);
    m = communicator->receive(PANO_TAG_RESPONSE, solverRank, PANO_DEFAULT_MESSAGE_SIZE);
    mutex.unlock();
    bool ignored = m->read<bool>();
    free(m);
    return ignored;
}

const double RemoteConstraint::getScore() const {
    mutex.lock();
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_CONSTRAINT_SCORE)
            .withParameter(constraintIndex);
    Message *m = mb.withTag(PANO_TAG_RESPONSE).build();
    communicator->send(m, solverRank);
    free(m);
    m = communicator->receive(PANO_TAG_RESPONSE, solverRank, PANO_DEFAULT_MESSAGE_SIZE);
    mutex.unlock();
    double score = m->read<double>();
    free(m);
    return score;
}
