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
 * @file GauloisSolver.cpp
 * @brief Provides a solver implementation encapsulating the communication with Abraracourcix.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <fstream>
#include <thread>

#include <mpi.h>

#include <loguru.hpp>

#include <crillab-easyjni/JavaVirtualMachineRegistry.h>

#include <crillab-panoramyx/network/Message.hpp>
#include <crillab-panoramyx/network/MessageBuilder.hpp>
#include <crillab-panoramyx/solver/GauloisSolver.hpp>

using namespace Panoramyx;
using namespace std;

GauloisSolver::GauloisSolver(Universe::IUniverseSolver *solver, INetworkCommunication *comm) : solver(solver),
                                                                                               comm(comm) {
}

Universe::UniverseSolverResult GauloisSolver::solve() {
    nbSolved++;
    auto r = solver->solve();
    LOG_F(INFO, "result after solve(): %s",
          r == Universe::UniverseSolverResult::SATISFIABLE ? "satisfiable" : "unsatisfiable");
    return r;
}

Universe::UniverseSolverResult GauloisSolver::solve(const std::string &filename) {
    nbSolved++;
    loadInstance(filename);
    auto result = solver->solve();
    return result;
}

Universe::UniverseSolverResult
GauloisSolver::solve(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &asumpts) {

    nbSolved++;
    auto r = solver->solve(asumpts);
    LOG_F(INFO, "result after solve(assumpts): %s",
          r == Universe::UniverseSolverResult::SATISFIABLE ? "satisfiable" : "unsatisfiable");
    return r;
}

void GauloisSolver::interrupt() {
    DLOG_F(INFO, "interrupt !");
    interrupted = true;
    solver->interrupt();
    DLOG_F(INFO, "after interrupt !");
}

void GauloisSolver::setVerbosity(int level) {
    solver->setVerbosity(level);
}

void GauloisSolver::setTimeout(long seconds) {
    solver->setTimeout(seconds);
}

void GauloisSolver::setTimeoutMs(long mseconds) {
    solver->setTimeoutMs(mseconds);
}

void GauloisSolver::reset() {
    loadMutex.lock();
    DLOG_F(INFO, "call reset");
    solver->reset();
    loadMutex.unlock();
}

std::vector<Universe::BigInteger> GauloisSolver::solution() {
    return sol;
}

int GauloisSolver::nVariables() {
    return solver->nVariables();
}

int GauloisSolver::nConstraints() {
    return solver->nConstraints();
}

void GauloisSolver::setLogFile(const std::string &filename) {
    solver->setLogFile(filename);
    loguru::add_file(filename.c_str(), loguru::Append, loguru::Verbosity_INFO);
}

void GauloisSolver::start() {
    while (!finishedB) {
        auto message = comm->receive(PANO_ANY_TAG, MPI_ANY_SOURCE, PANO_DEFAULT_MESSAGE_SIZE);
        readMessage(message);
        if (NAME_OF(message, PANO_MESSAGE_END_SEARCH)) {
            break;
        }
        free(message);
    }
    for (int i = 0; i < nbSolved; i++) {
        finished.acquire();
    }
}

void GauloisSolver::readMessage(Message *m) {
    DLOG_F(INFO, "Gaulois Solver: readMessage - %s", m->name);
    if (NAME_OF(m, IS(PANO_MESSAGE_SOLVE_FILENAME))) {
        std::string filename(m->parameters);
        this->solve(filename, m);
    } else if (NAME_OF(m, IS(PANO_MESSAGE_SOLVE))) {
        this->solve(m);
    } else if (NAME_OF(m, IS(PANO_MESSAGE_INDEX))) {
        this->index = m->read<unsigned>();
        DLOG_F(INFO, "Setting index to %d", index);
    } else if (NAME_OF(m, IS(PANO_MESSAGE_SOLVE_ASSUMPTIONS))) {
        std::vector<Universe::UniverseAssumption<Universe::BigInteger>> assumpts;
        for (int i = 0, n = 0; n < m->nbParameters; n += 3) {
            int s = (int) strlen(m->parameters + i);
            char *ptr = m->parameters + i;
            std::string varId(ptr, s + 1);
            i += s + 1;
            bool equal = m->read<bool>(i);
            i += sizeof(bool);
            ptr = m->parameters + i;
            std::string param(ptr, strlen(ptr) + 1);
            LOG_F(INFO, "%s %s '%s'", varId.c_str(), equal ? "=" : "!=", param.c_str());
            LOG_F(INFO, "%ld %ld %d", param.size(), strlen(ptr), s);
            Universe::BigInteger tmp = Universe::bigIntegerValueOf(param);
            assumpts.emplace_back(varId, equal, tmp);
            i += param.size();
        }
        this->solve(assumpts, m);
    } else if (strncmp(m->name, PANO_MESSAGE_RESET, sizeof(m->name)) == 0) {
        this->reset();
    } else if (strncmp(m->name, PANO_MESSAGE_LOAD_INSTANCE, sizeof(m->name)) == 0) {
        std::string filename(m->parameters);
        this->loadInstance(filename);
    } else if (strncmp(m->name, PANO_MESSAGE_INTERRUPT, sizeof(m->name)) == 0) {
        this->interrupt();
    } else if (strncmp(m->name, PANO_MESSAGE_SET_TIMEOUT, sizeof(m->name)) == 0) {
        auto timeout = m->read<long>();
        this->setTimeout(timeout);
    } else if (strncmp(m->name, PANO_MESSAGE_SET_TIMEOUT_MS, sizeof(m->name)) == 0) {
        auto timeout = m->read<long>();
        this->setTimeoutMs(timeout);
    } else if (strncmp(m->name, PANO_MESSAGE_SET_VERBOSITY, sizeof(m->name)) == 0) {
        int level = m->read<int>();
        this->setVerbosity(level);
    } else if (strncmp(m->name, PANO_MESSAGE_SET_LOG_FILE, sizeof(m->name)) == 0) {
        std::string filename(m->parameters);
        this->setLogFile(filename);
    } else if (strncmp(m->name, PANO_MESSAGE_END_SEARCH, sizeof(m->name)) == 0) {
        boundMutex.lock();
        interrupt();
        MessageBuilder mb;
        Message *r = mb.named(PANO_MESSAGE_END_SEARCH).withTag(PANO_TAG_SOLVE).build();
        comm->send(r, m->src);
        free(r);
        finishedB = true;
        boundMutex.unlock();
    } else if (strncmp(m->name, PANO_MESSAGE_LOWER_BOUND, sizeof(m->name)) == 0) {
        std::string param(m->parameters, strlen(m->parameters) + 1);
        Universe::BigInteger newBound = Universe::bigIntegerValueOf(param);
        this->setLowerBound(newBound);
    } else if (strncmp(m->name, PANO_MESSAGE_UPPER_BOUND, sizeof(m->name)) == 0) {
        std::string param(m->parameters, strlen(m->parameters) + 1);
        Universe::BigInteger newBound = Universe::bigIntegerValueOf(param);
        this->setUpperBound(newBound);
    } else if (strncmp(m->name, PANO_MESSAGE_LOWER_UPPER_BOUND, sizeof(m->name)) == 0) {
        std::string param(m->parameters, strlen(m->parameters) + 1);
        Universe::BigInteger lowerBound = Universe::bigIntegerValueOf(param);

        std::string param2(m->parameters + param.size(), strlen(m->parameters + param.size()) + 1);
        Universe::BigInteger upperBound = Universe::bigIntegerValueOf(param2);
        this->setBounds(lowerBound, upperBound);
    } else if (strncmp(m->name, PANO_MESSAGE_GET_CURRENT_BOUND, sizeof(m->name)) == 0) {
        this->getCurrentBound(m);
    } else if (strncmp(m->name, PANO_MESSAGE_GET_LOWER_BOUND, sizeof(m->name)) == 0) {
        this->getLowerBound(m);
    } else if (strncmp(m->name, PANO_MESSAGE_GET_UPPER_BOUND, sizeof(m->name)) == 0) {
        this->getUpperBound(m);
    } else if (strncmp(m->name, PANO_MESSAGE_IS_MINIMIZATION, sizeof(m->name)) == 0) {
        this->isMinimization(m);
    } else if (strncmp(m->name, PANO_MESSAGE_IS_OPTIMIZATION, sizeof(m->name)) == 0) {
        this->isOptimization(m);
    } else if (NAME_OF(m, IS(PANO_MESSAGE_MAP_SOLUTION))) {
        this->mapSolution(m);
    } else if (NAME_OF(m, IS(PANO_MESSAGE_N_VARIABLES))) {
        this->nVariables(m);
    } else if (NAME_OF(m, IS(PANO_MESSAGE_N_CONSTRAINTS))) {
        this->nConstraints(m);
    } else if (NAME_OF(m, IS(PANO_MESSAGE_DECISION_VARIABLES))) {
        this->decisionVariables(m);
    } else if (NAME_OF(m, IS(PANO_MESSAGE_GET_AUXILIARY_VARIABLES))) {
        this->getAuxiliaryVariables(m);
    } else if (NAME_OF(m, IS(PANO_MESSAGE_CHECK_SOLUTION))) {
        this->checkSolution(m);
    } else if (NAME_OF(m, IS(PANO_MESSAGE_CHECK_SOLUTION_ASSIGNMENT))) {
        this->checkSolutionAssignment(m);
    } else if (NAME_OF(m, IS(PANO_MESSAGE_VALUE_HEURISTIC_STATIC))) {
        this->valueHeuristicStatic(m);
    }
}

std::vector<Universe::BigInteger> GauloisSolver::solution(Message *m) {
    boundMutex.lock();
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_SOLUTION);
    for (auto &big: sol) {
        mb.withParameter(Universe::toString(big));
    }
    Message *r = mb.withTag(PANO_TAG_RESPONSE).build();
    comm->send(r, m->src);
    free(r);
    boundMutex.unlock();
    return sol;
}

int GauloisSolver::nVariables(Message *m) {
    int n = solver->nVariables();
    MessageBuilder mb;
    Message *r = mb.named(PANO_MESSAGE_N_VARIABLES).withTag(PANO_TAG_RESPONSE).withParameter(n).build();
    comm->send(r, m->src);
    free(r);
    return n;
}

int GauloisSolver::nConstraints(Message *m) {
    int n = solver->nConstraints();
    MessageBuilder mb;
    Message *r = mb.named(PANO_MESSAGE_N_CONSTRAINTS).withTag(PANO_TAG_RESPONSE).withParameter(n).build();
    comm->send(r, m->src);
    free(r);
    return n;
}

Universe::UniverseSolverResult GauloisSolver::solve(Message *m) {
    try {
        int src = m->src;
        std::thread t([this, src]() {
            try {
                DLOG_F(INFO, "before load mutex");
                loadMutex.lock();
                DLOG_F(INFO, "after load mutex");
                auto result = this->solve();
                DLOG_F(INFO, "after solve");
                sendResult(src, result);
                DLOG_F(INFO, "after send");
                loadMutex.unlock();
                finished.release();
                easyjni::JavaVirtualMachineRegistry::detachCurrentThread();
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        });
        t.detach();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return Universe::UniverseSolverResult::UNKNOWN;
}

void GauloisSolver::sendResult(int src, Universe::UniverseSolverResult result) {
    MessageBuilder mb;
    mb.withParameter(index);
    DLOG_F(INFO, "avant boundMutex.lock()");
    boundMutex.lock();
    DLOG_F(INFO, "après boundMutex.lock()");
    if (interrupted) {
        boundMutex.unlock();
        return;
    }
    switch (result) {
        case Universe::UniverseSolverResult::SATISFIABLE:
            if (optimization) {
                currentBound = getOptimSolver()->getCurrentBound();
                mb.named(PANO_MESSAGE_NEW_BOUND_FOUND).withParameter(currentBound);
            } else {
                mb.named(PANO_MESSAGE_SATISFIABLE);
            }
            currentSolution = solver->mapSolution();
            sol = solver->solution();
            break;
        case Universe::UniverseSolverResult::UNSATISFIABLE:
            mb.named(PANO_MESSAGE_UNSATISFIABLE);
            break;
        case Universe::UniverseSolverResult::UNKNOWN:
            mb.named(PANO_MESSAGE_UNKNOWN);
            break;
        case Universe::UniverseSolverResult::UNSUPPORTED:
            mb.named(PANO_MESSAGE_UNSUPPORTED);
            break;
        case Universe::UniverseSolverResult::OPTIMUM_FOUND:
            mb.named(PANO_MESSAGE_OPTIMUM_FOUND);
            break;
    }
    boundMutex.unlock();
    Message *r = mb.withTag(PANO_TAG_SOLVE).build();
    comm->send(r, src);
    free(r);
    LOG_F(INFO, "sending result: %s",
          result == Universe::UniverseSolverResult::SATISFIABLE ? "satisfiable" : "unsatisfiable");
}

Universe::UniverseSolverResult GauloisSolver::solve(std::string filename, Message *m) {
    int src = m->src;
    std::thread t([this, src, filename]() {
        loadMutex.lock();
        auto result = this->solve(filename);
        sendResult(src, result);
        loadMutex.unlock();
        finished.release();
    });
    t.detach();
    return Universe::UniverseSolverResult::UNKNOWN;
}

Universe::UniverseSolverResult
GauloisSolver::solve(std::vector<Universe::UniverseAssumption<Universe::BigInteger>> asumpts, Message *m) {
    int src = m->src;
    std::thread t([this, src, asumpts]() {
        std::vector<Universe::UniverseAssumption<Universe::BigInteger>> realAssumpts;
        std::map<std::string, Universe::IUniverseVariable*> mapping = solver->getVariablesMapping();
        for (int i = 0; i < asumpts.size(); i++) {
            auto &a = asumpts[i];
            if (a.isEqual()) {
                realAssumpts.emplace_back(a);
            }  else {
                auto &b = asumpts[i + 1];
                mapping[a.getVariableId()]->getDomain()->keepValues(a.getValue(), b.getValue());
                i++;
            }
        }

        DLOG_F(INFO, "Run solve(assumpts,m) in a new thread.");
        loadMutex.lock();
        DLOG_F(INFO, "après loadmutex.lock()");
        auto result = this->solve(asumpts);
        sendResult(src, result);
        loadMutex.unlock();
        easyjni::JavaVirtualMachineRegistry::detachCurrentThread();
        finished.release();
    });
    t.detach();
    return Universe::UniverseSolverResult::UNKNOWN;
}

void GauloisSolver::loadInstance(const std::string &filename) {
    loadMutex.lock();
    solver->loadInstance(filename);
    optimization = solver->isOptimization();
    loadMutex.unlock();
}

const map<std::string, Universe::IUniverseVariable *> &GauloisSolver::getVariablesMapping() {
    return solver->getVariablesMapping();
}

map<std::string, Universe::BigInteger> GauloisSolver::mapSolution() {

    return currentSolution;
}

bool GauloisSolver::isOptimization() {
    return solver->isOptimization();
}

void GauloisSolver::setLowerBound(const Universe::BigInteger &lb) {
    //TODO GMP case
    boundMutex.lock();
    LOG_F(INFO, "New lower bound %lld", lb);
    this->getOptimSolver()->setLowerBound(lb);
    boundMutex.unlock();
}

void GauloisSolver::setUpperBound(const Universe::BigInteger &ub) {
    //TODO GMP case
    boundMutex.lock();
    LOG_F(INFO, "New upper bound %lld", ub);
    this->getOptimSolver()->setUpperBound(ub);
    boundMutex.unlock();
}

void GauloisSolver::setBounds(const Universe::BigInteger &lb, const Universe::BigInteger &ub) {
    boundMutex.lock();
    this->getOptimSolver()->setBounds(lb, ub);
    boundMutex.unlock();
}

Universe::BigInteger GauloisSolver::getCurrentBound() {
    return this->getOptimSolver()->getCurrentBound();
}

bool GauloisSolver::isMinimization() {
    return this->getOptimSolver()->isMinimization();
}

Universe::IOptimizationSolver *GauloisSolver::getOptimSolver() {
    return solver->toOptimizationSolver();
}

Universe::BigInteger GauloisSolver::getLowerBound() {
    assert(getOptimSolver() != nullptr);
    return getOptimSolver()->getLowerBound();
}

Universe::BigInteger GauloisSolver::getUpperBound() {
    return getOptimSolver()->getUpperBound();
}

Universe::BigInteger GauloisSolver::getLowerBound(Message *m) {
    auto result = this->getLowerBound();
    MessageBuilder mb;
    Message *r = mb.named(PANO_MESSAGE_GET_LOWER_BOUND).withTag(PANO_TAG_RESPONSE).withParameter(result).build();
    comm->send(r, m->src);
    free(r);
    return result;
}

Universe::BigInteger GauloisSolver::getUpperBound(Message *m) {
    auto result = this->getUpperBound();
    MessageBuilder mb;
    Message *r = mb.named(PANO_MESSAGE_GET_UPPER_BOUND).withTag(PANO_TAG_RESPONSE).withParameter(result).build();
    comm->send(r, m->src);
    free(r);
    return result;
}

Universe::BigInteger GauloisSolver::getCurrentBound(Message *m) {
    auto result = this->getCurrentBound();
    MessageBuilder mb;
    Message *r = mb.named(PANO_MESSAGE_GET_CURRENT_BOUND).withTag(PANO_TAG_RESPONSE).withParameter(result).build();
    comm->send(r, m->src);
    free(r);
    return result;
}

bool GauloisSolver::isMinimization(Message *m) {
    auto result = this->isMinimization();
    MessageBuilder mb;
    Message *r = mb.named(PANO_MESSAGE_IS_MINIMIZATION).withTag(PANO_TAG_RESPONSE).withParameter(result).build();
    comm->send(r, m->src);
    free(r);
    return result;
}

bool GauloisSolver::isOptimization(Message *m) {
    MessageBuilder mb;
    Message *r = mb.named(PANO_MESSAGE_IS_OPTIMIZATION).withTag(PANO_TAG_RESPONSE).withParameter(
            isOptimization()).build();

    DLOG_F(INFO, "send message to %d", m->src);
    comm->send(r, m->src);
    free(r);
    return optimization;
}

std::map<std::string, Universe::BigInteger> GauloisSolver::mapSolution(Message *m) {
    DLOG_F(INFO, "log avant %d", m->src);
    boundMutex.lock();
    DLOG_F(INFO, "log après");
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_MAP_SOLUTION);
    for (auto &kv: currentSolution) {
        mb.withParameter(kv.first);
        mb.withParameter(kv.second);
    }
    Message *r = mb.withTag(PANO_TAG_RESPONSE).build();
    comm->send(r, m->src);
    free(r);
    boundMutex.unlock();

    return currentSolution;
}

void GauloisSolver::decisionVariables(const vector<std::string> &variables) {
    solver->decisionVariables(variables);
}

void GauloisSolver::addSearchListener(Universe::IUniverseSearchListener *listener) {
    solver->addSearchListener(listener);
}

void GauloisSolver::setLogStream(ostream &stream) {
    solver->setLogStream(stream);
}

map<std::string, Universe::BigInteger> GauloisSolver::mapSolution(bool excludeAux) {
    return solver->mapSolution(excludeAux);
}

Universe::IOptimizationSolver *GauloisSolver::toOptimizationSolver() {
    return this;
}

void GauloisSolver::decisionVariables(Message *m) {
    std::vector<std::string> decisions;
    for (int i = 0, n = 0; n < m->nbParameters; n += 1) {
        int s = (int) strlen(m->parameters + i);
        char *ptr = m->parameters + i;
        std::string varId(ptr, s + 1);
        i += s;
        decisions.push_back(varId);
    }
    this->decisionVariables(decisions);
}

const vector<std::string> &GauloisSolver::getAuxiliaryVariables() {
    return solver->getAuxiliaryVariables();
}

void GauloisSolver::valueHeuristicStatic(const vector<std::string> &variables,
                                         const vector<Universe::BigInteger> &orderedValues) {
    return solver->valueHeuristicStatic(variables, orderedValues);
}

void GauloisSolver::removeSearchListener(Universe::IUniverseSearchListener *listener) {
    solver->removeSearchListener(listener);
}

bool GauloisSolver::checkSolution() {
    return solver->checkSolution();
}

bool GauloisSolver::checkSolution(const map<std::string, Universe::BigInteger> &assignment) {
    return solver->checkSolution(assignment);
}

void GauloisSolver::getAuxiliaryVariables(Message *pMessage) {
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_MAP_SOLUTION);
    for (auto &kv: solver->getAuxiliaryVariables()) {
        mb.withParameter(kv);
    }
    Message *r = mb.withTag(PANO_TAG_RESPONSE).build();
    comm->send(r, pMessage->src);
    free(r);
}

const vector<Universe::IUniverseConstraint *> &GauloisSolver::getConstraints() {
    return solver->getConstraints();
}


void GauloisSolver::checkSolutionAssignment(Message *pMessage) {
    std::map<std::string, Universe::BigInteger> bigbig;
    char *ptrName = pMessage->parameters;
    char *ptrValue = nullptr;
    char *ptr = pMessage->parameters;
    for (int i = 0, n = 0; n < pMessage->nbParameters; i++) {
        if (ptr[i] == '\0') {
            if (ptrValue == nullptr) {
                ptrValue = ptr + i + 1;
            } else {
                std::string name(ptrName, ptrValue - ptrName - 1);
                std::string value(ptrValue, ptr + i - ptrValue);
                bigbig[name] = Universe::bigIntegerValueOf(value);
                ptrValue = nullptr;
                ptrName = ptr + i + 1;
            }
            n++;
        }
    }
    bool b = solver->checkSolution(bigbig);
    MessageBuilder mb;
    Message *r = mb.named(PANO_MESSAGE_CHECK_SOLUTION_ASSIGNMENT).withTag(PANO_TAG_RESPONSE).withParameter(b).build();
    comm->send(r, pMessage->src);
    free(r);

}

void GauloisSolver::checkSolution(Message *pMessage) {
    bool b = solver->checkSolution();
    MessageBuilder mb;
    Message *r = mb.named(PANO_MESSAGE_CHECK_SOLUTION).withTag(PANO_TAG_RESPONSE).withParameter(b).build();
    comm->send(r, pMessage->src);
    free(r);
}

void GauloisSolver::valueHeuristicStatic(Message *pMessage) {
    std::vector<std::string> names;
    std::vector<Universe::BigInteger> ordered;
    int n = pMessage->read<int>();
    char *ptrName = pMessage->parameters + sizeof(int);
    for (int i = 0; i < n; i++) {
        std::string name(ptrName, strlen(ptrName) + 1);
        names.push_back(name);
        ptrName += name.size() + 1;
    }
    n = *((int *) ptrName);
    ptrName = ptrName + sizeof(int);
    for (int i = 0; i < n; i++) {
        std::string big(ptrName, strlen(ptrName) + 1);
        ordered.push_back(Universe::bigIntegerValueOf(big));
        ptrName += big.size() + 1;
    }
    this->valueHeuristicStatic(names, ordered);
}
