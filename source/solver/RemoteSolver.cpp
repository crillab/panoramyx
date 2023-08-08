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
 * @file RemoteSolver.cpp
 * @brief Provides a solver implementation encapsulating the communication with a remote solver.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <iostream>

#include <crillab-except/except.hpp>

#include <crillab-panoramyx/network/Message.hpp>
#include <crillab-panoramyx/network/MessageBuilder.hpp>
#include <crillab-panoramyx/solver/RemoteSolver.hpp>
#include "crillab-panoramyx/problem/RemoteConstraint.hpp"

using namespace Panoramyx;
using namespace Universe;

RemoteSolver::RemoteSolver(int rank) :
        rank(rank) {
    // Nothing to do: everything is already initialized.
}

void RemoteSolver::setIndex(unsigned i) {
    this->index = i;
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_INDEX);
    Message *m = mb.withTag(PANO_TAG_SOLVE).withParameter(i).build();
    communicator->send(m, rank);
    free(m);
}

bool RemoteSolver::isOptimization() {
    if (!optimization) {
        mutex.lock();
        MessageBuilder mb;
        mb.named(PANO_MESSAGE_IS_OPTIMIZATION);
        Message *m = mb.withTag(PANO_TAG_RESPONSE).build();
        communicator->send(m, rank);
        free(m);
        LOG_F(INFO, "Wait answer");
        m = communicator->receive(PANO_TAG_RESPONSE, rank, PANO_DEFAULT_MESSAGE_SIZE);
        LOG_F(INFO, "after answer");
        mutex.unlock();
        optimization = m->read<bool>();
        LOG_F(INFO, "Remote Solver: readMessage - %d", optimization);
        free(m);
    }
    return *optimization;
}

UniverseSolverResult RemoteSolver::solve() {
    nVariables();
    nConstraints();
    MessageBuilder mb;
    Message *m = mb.named(PANO_MESSAGE_SOLVE).withTag(PANO_TAG_SOLVE).build();
    communicator->send(m, rank);
    free(m);
    return UniverseSolverResult::UNKNOWN;
}

UniverseSolverResult RemoteSolver::solve(
        const std::string &filename) {
    //todo nVariables ? nConstraints ? in this case
    MessageBuilder mb;
    Message *m = mb.named(PANO_MESSAGE_SOLVE_FILENAME)
            .withParameter(filename)
            .withTag(PANO_TAG_SOLVE)
            .build();
    communicator->send(m, rank);
    free(m);
    return UniverseSolverResult::UNKNOWN;
}

UniverseSolverResult RemoteSolver::solve(
        const std::vector<UniverseAssumption<BigInteger>>
        &assumpts) {
    nVariables();
    nConstraints();
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_SOLVE_ASSUMPTIONS);
    for (auto &assumpt: assumpts) {
        mb.withParameter(assumpt.getVariableId());
        mb.withParameter(assumpt.isEqual());
        mb.withParameter(toString(assumpt.getValue()));

        LOG_F(INFO, "add assumption: %s %s '%s'", assumpt.getVariableId().c_str(),
              assumpt.isEqual() ? "=" : "!=",
              toString(assumpt.getValue()).c_str());
    }
    Message *m = mb.withTag(PANO_TAG_SOLVE).build();
    communicator->send(m, rank);
    free(m);
    return UniverseSolverResult::UNKNOWN;
}

void RemoteSolver::interrupt() {
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_INTERRUPT);
    Message *m = mb.withTag(PANO_TAG_SOLVE).build();
    communicator->send(m, rank);
    free(m);
}

void RemoteSolver::setVerbosity(int level) {
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_SET_VERBOSITY);
    mb.withParameter(level);
    Message *m = mb.withTag(PANO_TAG_CONFIG).build();
    communicator->send(m, rank);
    free(m);
}

void RemoteSolver::setTimeout(long seconds) {
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_SET_TIMEOUT);
    mb.withParameter(seconds);
    Message *m = mb.withTag(PANO_TAG_CONFIG).build();
    communicator->send(m, rank);
    free(m);
}

void RemoteSolver::setTimeoutMs(long mseconds) {
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_SET_TIMEOUT_MS);
    mb.withParameter(mseconds);
    Message *m = mb.withTag(PANO_TAG_CONFIG).build();
    communicator->send(m, rank);
    free(m);
}

void RemoteSolver::reset() {
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_RESET);
    Message *m = mb.withTag(PANO_TAG_SOLVE).build();
    communicator->send(m, rank);
    free(m);
}

std::vector<BigInteger> RemoteSolver::solution() {
    unsigned long size =
            nVariables() * (PANO_NUMBER_MAX_CHAR + 1) + sizeof(Message);
    mutex.lock();
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_SOLUTION);
    Message *m = mb.withTag(PANO_TAG_RESPONSE).build();
    communicator->send(m, rank);
    free(m);
    m = communicator->receive(PANO_TAG_RESPONSE, rank, size);
    mutex.unlock();

    std::vector<BigInteger> bigbig;
    char *ptr = m->parameters;
    for (int i = 0, n = 0; n < m->nbParameters; i++) {
        if (ptr[i] == '\0') {
            std::string param(ptr, i);
            bigbig.push_back(bigIntegerValueOf(param));
            ptr += i + 1;
            i = -1;
            n++;
        }
    }
    free(m);
    return bigbig;
}

int RemoteSolver::nVariables() {
    if (nbVariables < 0) {
        mutex.lock();
        MessageBuilder mb;
        mb.named(PANO_MESSAGE_N_VARIABLES);
        Message *m = mb.withTag(PANO_TAG_RESPONSE).build();
        communicator->send(m, rank);
        free(m);

        m = communicator->receive(PANO_TAG_RESPONSE, rank, PANO_DEFAULT_MESSAGE_SIZE);
        mutex.unlock();
        nbVariables = *((const int *) m->parameters);
        free(m);
    }
    return nbVariables;
}

int RemoteSolver::nConstraints() {
    if (nbConstraints < 0) {
        mutex.lock();
        MessageBuilder mb;
        mb.named(PANO_MESSAGE_N_CONSTRAINTS);
        Message *m = mb.withTag(PANO_TAG_RESPONSE).build();
        communicator->send(m, rank);
        free(m);

        m = communicator->receive(PANO_TAG_RESPONSE, rank);
        mutex.unlock();
        nbConstraints = *((const int *) m->parameters);

        for (int i = 0; i < nbConstraints; i++) {
            remoteConstraints.push_back(new RemoteConstraint(communicator, mutex, rank, i));
        }
        free(m);
    }
    return nbConstraints;
}

void RemoteSolver::setLogFile(const std::string &filename) {}


void RemoteSolver::setCommunicator(INetworkCommunication *communicator) { this->communicator = communicator; }

void RemoteSolver::endSearch() {
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_END_SEARCH);
    Message *m = mb.withTag(PANO_TAG_SOLVE).build();
    communicator->send(m, rank);
    free(m);
}

UniverseSolverResult RemoteSolver::getResult() {
    // TODO Really implement this method.
    return UniverseSolverResult::UNKNOWN;
}

void RemoteSolver::loadInstance(const std::string &filename) {
    MessageBuilder mb;
    Message *m = mb.named(PANO_MESSAGE_LOAD_INSTANCE)
            .withParameter(filename)
            .withTag(PANO_TAG_SOLVE)
            .build();
    communicator->send(m, rank);
    free(m);
}

[[nodiscard]] const std::map<std::string, IUniverseVariable *>
&RemoteSolver::getVariablesMapping() {
    throw Except::UnsupportedOperationException("variables are too far far away.");
}

std::map<std::string, BigInteger> RemoteSolver::mapSolution() {
    return this->mapSolution(false);
}

void RemoteSolver::setLowerBound(const BigInteger &lb) {
    MessageBuilder mb;
    Message *m = mb.named(PANO_MESSAGE_LOWER_BOUND)
            .withParameter(lb)
            .withTag(PANO_TAG_SOLVE)
            .build();
    communicator->send(m, rank);

    free(m);
}

void RemoteSolver::setUpperBound(const BigInteger &ub) {
    MessageBuilder mb;
    Message *m = mb.named(PANO_MESSAGE_UPPER_BOUND)
            .withParameter(ub)
            .withTag(PANO_TAG_SOLVE)
            .build();
    communicator->send(m, rank);
    free(m);
}

void RemoteSolver::setBounds(const BigInteger &lb,
                             const BigInteger &ub) {
    MessageBuilder mb;
    Message *m = mb.named(PANO_MESSAGE_LOWER_UPPER_BOUND)
            .withParameter(lb)
            .withParameter(ub)
            .withTag(PANO_TAG_SOLVE)
            .build();
    communicator->send(m, rank);
    free(m);
}

BigInteger RemoteSolver::getCurrentBound() {
    mutex.lock();
    MessageBuilder mb;
    Message *m = mb.named(PANO_MESSAGE_GET_CURRENT_BOUND)
            .withTag(PANO_TAG_RESPONSE)
            .build();
    communicator->send(m, rank);
    free(m);

    m = communicator->receive(PANO_TAG_RESPONSE, rank);
    std::string param(m->parameters, strlen(m->parameters) + 1);
    BigInteger newBound = bigIntegerValueOf(param);
    mutex.unlock();
    free(m);
    return newBound;
}

bool RemoteSolver::isMinimization() {
    mutex.lock();
    MessageBuilder mb;
    Message *m =
            mb.named(PANO_MESSAGE_IS_MINIMIZATION).withTag(PANO_TAG_RESPONSE).build();
    communicator->send(m, rank);
    free(m);

    m = communicator->receive(PANO_TAG_RESPONSE, rank);
    bool r = m->read<bool>();
    mutex.unlock();
    free(m);
    return r;
}

BigInteger RemoteSolver::getLowerBound() {
    mutex.lock();
    MessageBuilder mb;
    Message *m =
            mb.named(PANO_MESSAGE_GET_LOWER_BOUND).withTag(PANO_TAG_RESPONSE).build();
    communicator->send(m, rank);
    free(m);

    m = communicator->receive(PANO_TAG_RESPONSE, rank);
    std::string param(m->parameters, strlen(m->parameters) + 1);
    BigInteger newBound = bigIntegerValueOf(param);
    mutex.unlock();
    free(m);
    return newBound;
}

BigInteger RemoteSolver::getUpperBound() {
    mutex.lock();
    MessageBuilder mb;
    Message *m =
            mb.named(PANO_MESSAGE_GET_UPPER_BOUND).withTag(PANO_TAG_RESPONSE).build();
    communicator->send(m, rank);
    free(m);

    m = communicator->receive(PANO_TAG_RESPONSE, rank);
    std::string param(m->parameters, strlen(m->parameters) + 1);
    BigInteger newBound = bigIntegerValueOf(param);
    mutex.unlock();
    free(m);
    return newBound;
}

unsigned int RemoteSolver::getIndex() const {
    return index;
}

void RemoteSolver::decisionVariables(const std::vector<std::string> &variables) {
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_DECISION_VARIABLES);
    for (auto &v: variables) {
        mb.withParameter(v);
    }
    Message *m = mb.withTag(PANO_TAG_SOLVE).build();
    communicator->send(m, rank);
    free(m);
}

void RemoteSolver::addSearchListener(IUniverseSearchListener *listener) {
    throw Except::UnsupportedOperationException("addSearchListener on RemoteSolver is not supported.");
}

void RemoteSolver::setLogStream(std::ostream &stream) {
    throw Except::UnsupportedOperationException("setLogStream on RemoteSolver is not supported.");
}

std::map<std::string, BigInteger> RemoteSolver::mapSolution(bool excludeAux) {
    mutex.lock();
    MessageBuilder mb;
    Message *m = mb.named(PANO_MESSAGE_MAP_SOLUTION)
            .withTag(PANO_TAG_RESPONSE).withParameter(excludeAux)
            .build();
    LOG_F(INFO, "avant send");
    communicator->send(m, rank);
    LOG_F(INFO, "après send");
    free(m);
    unsigned long size =
            100 * nVariables() * (PANO_VARIABLE_NAME_MAX_CHAR + PANO_NUMBER_MAX_CHAR + 2) + sizeof(Message);
    LOG_F(INFO, "avant receive");
    m = communicator->receive(PANO_TAG_RESPONSE, rank, size);
    LOG_F(INFO, "après receive", m->src);
    mutex.unlock();
    std::map<std::string, BigInteger> bigbig;
    char *ptrName = m->parameters;
    char *ptrValue = nullptr;
    char *ptr = m->parameters;
    for (int i = 0, n = 0; n < m->nbParameters; i++) {
        if (ptr[i] == '\0') {
            if (ptrValue == nullptr) {
                ptrValue = ptr + i + 1;
            } else {
                std::string name(ptrName, ptrValue - ptrName - 1);
                std::string value(ptrValue, ptr + i - ptrValue);
                bigbig[name] = bigIntegerValueOf(value);
                ptrValue = nullptr;
                ptrName = ptr + i + 1;
            }
            n++;
        }
    }
    free(m);
    return bigbig;
}

IOptimizationSolver *RemoteSolver::toOptimizationSolver() {
    return this;
}

const std::vector<std::string> &RemoteSolver::getAuxiliaryVariables() {
    if (!auxiliaryVariables.empty()) {
        return auxiliaryVariables;
    }
    mutex.lock();
    MessageBuilder mb;
    Message *m = mb.named(PANO_MESSAGE_GET_AUXILIARY_VARIABLES)
            .withTag(PANO_TAG_RESPONSE)
            .build();
    communicator->send(m, rank);
    free(m);
    unsigned long size = 100 * nVariables() * (PANO_VARIABLE_NAME_MAX_CHAR + 1) + sizeof(Message);

    m = communicator->receive(PANO_TAG_RESPONSE, rank, size);
    mutex.unlock();

    int n = m->nbParameters;
    char *ptrName = m->parameters;
    for (int i = 0; i < n; i++) {
        std::string name(ptrName, strlen(ptrName) + 1);
        auxiliaryVariables.push_back(name);
        ptrName += name.size() + 1;
    }
    return auxiliaryVariables;
}

void RemoteSolver::valueHeuristicStatic(const std::vector<std::string> &variables,
                                        const std::vector<BigInteger> &orderedValues) {
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_VALUE_HEURISTIC_STATIC);
    mb.withParameter((int) variables.size());
    for (auto &v: variables) {
        mb.withParameter(v);
    }
    mb.withParameter((int) orderedValues.size());
    for (auto &v: orderedValues) {
        mb.withParameter(toString(v));
    }
    Message *m = mb.withTag(PANO_TAG_SOLVE).build();
    communicator->send(m, rank);
    free(m);
}

bool RemoteSolver::checkSolution() {
    mutex.lock();
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_CHECK_SOLUTION);
    Message *m = mb.withTag(PANO_TAG_RESPONSE).build();
    communicator->send(m, rank);
    free(m);

    m = communicator->receive(PANO_TAG_RESPONSE, rank, PANO_DEFAULT_MESSAGE_SIZE);
    bool b = m->read<bool>();
    mutex.unlock();
    free(m);
    return b;
}

bool RemoteSolver::checkSolution(const std::map<std::string, BigInteger> &assignment) {
    mutex.lock();
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_CHECK_SOLUTION_ASSIGNMENT);
    for (auto &kv: assignment) {
        mb.withParameter(kv.first);
        mb.withParameter(kv.second);
    }
    Message *r = mb.withTag(PANO_TAG_RESPONSE).build();
    communicator->send(r, rank);
    free(r);
    r = communicator->receive(PANO_TAG_RESPONSE, rank, PANO_DEFAULT_MESSAGE_SIZE);
    bool b = r->read<bool>();
    mutex.unlock();
    free(r);
    return b;
}

const std::vector<IUniverseConstraint *> &RemoteSolver::getConstraints() {
    throw remoteConstraints;
}
