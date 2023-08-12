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
 * @file AbstractParallelSolver.cpp
 * @brief The parent class of all the solvers that run in parallel.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <thread>

#include <loguru.hpp>
#include <mpi.h>

#include <crillab-panoramyx/solver/AbstractParallelSolver.hpp>
#include <crillab-panoramyx/network/Message.hpp>

using namespace std;

using namespace Except;
using namespace Panoramyx;
using namespace Universe;

AbstractParallelSolver::AbstractParallelSolver(
        INetworkCommunication *comm, IBoundAllocationStrategy *allocationStrategy) :
        communicator(comm),
        allocationStrategy(allocationStrategy),
        solvers(),
        currentRunningSolvers(),
        runningSolvers(0),
        minimization(true),
        lowerBound(0),
        upperBound(0),
        currentBounds(),
        winner(-1),
        result(Universe::UniverseSolverResult::UNKNOWN),
        bestSolution(),
        solutionMutex(),
        solved(0),
        interrupted(false),
        end(0) {
    currentBounds.emplace_back(0);
}

void AbstractParallelSolver::addSolver(PanoramyxSolver *solver) {
    unsigned index = solvers.size();

    solvers.emplace_back(solver);
    currentRunningSolvers.emplace_back(false);
    currentBounds.emplace_back(LLONG_MAX);
    runningSolvers++;

    solver->setCommunicator(communicator);
    solver->setIndex(index);
}

void AbstractParallelSolver::loadInstance(const string &filename) {
    for (auto &solver: solvers) {
        solver->loadInstance(filename);
    }
}

void AbstractParallelSolver::reset() {
    // FIXME: Make sure that the semaphores, etc. are also reset.
    for (auto &solver: solvers) {
        solver->reset();
    }
}

int AbstractParallelSolver::nVariables() {
    return solvers[0]->nVariables();
}

const map<string, IUniverseVariable *> &AbstractParallelSolver::getVariablesMapping() {
    throw UnsupportedOperationException("parallel solver has too many mappings!");
}

const vector<string> &AbstractParallelSolver::getAuxiliaryVariables() {
    throw UnsupportedOperationException("parallel solver has too many auxiliary variables!");
}

int AbstractParallelSolver::nConstraints() {
    return solvers[0]->nConstraints();
}

const vector<IUniverseConstraint *> &AbstractParallelSolver::getConstraints() {
    throw UnsupportedOperationException("parallel solver has too many constraints!");
}

void AbstractParallelSolver::decisionVariables(const vector<string> &variables) {
    for (auto &solver: solvers) {
        solver->decisionVariables(variables);
    }
}

void AbstractParallelSolver::valueHeuristicStatic(const vector<string> &variables,
                                                  const vector<BigInteger> &orderedValues) {
    for (auto &solver: solvers) {
        solver->valueHeuristicStatic(variables, orderedValues);
    }
}

bool AbstractParallelSolver::isOptimization() {
    return solvers[0]->isOptimization();
}

void AbstractParallelSolver::setTimeout(long seconds) {
    for (auto &solver: solvers) {
        solver->setTimeout(seconds);
    }
}

void AbstractParallelSolver::setTimeoutMs(long mseconds) {
    for (auto &solver: solvers) {
        solver->setTimeoutMs(mseconds);
    }
}

void AbstractParallelSolver::setVerbosity(int level) {
    for (auto &solver: solvers) {
        solver->setVerbosity(level);
    }
}

void AbstractParallelSolver::addSearchListener(IUniverseSearchListener *listener) {
    for (auto &solver: solvers) {
        solver->addSearchListener(listener);
    }
}

void AbstractParallelSolver::setLogFile(const string &filename) {
    // TODO: Make sure that the solvers do not write their logs in the same file.
}

void AbstractParallelSolver::setLogStream(ostream &stream) {
    // TODO: Make sure that the solvers do not write their logs in the same stream.
}

UniverseSolverResult AbstractParallelSolver::solve() {
    return solve(vector<UniverseAssumption<BigInteger>>());
}

UniverseSolverResult AbstractParallelSolver::solve(const string &filename) {
    loadInstance(filename);
    return solve();
}

UniverseSolverResult AbstractParallelSolver::solve(const vector<UniverseAssumption<BigInteger>> &assumpts) {
    // Reading the messages in a dedicated thread.
    readMessages();

    // Preparing the solving process.
    for (unsigned i = 0; i < solvers.size(); i++) {
        ready(i);
    }

    // Actually solving the problem.
    return internalSolve(assumpts);
}

void AbstractParallelSolver::interrupt() {
    for (auto &solver : solvers) {
        solver->interrupt();
    }
    interrupted = true;
}

UniverseSolverResult AbstractParallelSolver::getResult() {
    return result;
}

vector<BigInteger> AbstractParallelSolver::solution() {
    if ((result == UniverseSolverResult::SATISFIABLE) || (result == UniverseSolverResult::OPTIMUM_FOUND)) {
        solutionMutex.lock();
        auto local = bestSolutionVector;
        solutionMutex.unlock();
        return local;
    }
    throw IllegalStateException("problem has no solution yet");
}

map<string, BigInteger> AbstractParallelSolver::mapSolution() {
    if ((result == UniverseSolverResult::SATISFIABLE) || (result == UniverseSolverResult::OPTIMUM_FOUND)) {
        solutionMutex.lock();
        auto local = bestSolution;
        solutionMutex.unlock();
        return local;
    }
    throw IllegalStateException("problem has no solution yet");
}

map<string, BigInteger> AbstractParallelSolver::mapSolution(bool excludeAux) {
    if ((result == UniverseSolverResult::SATISFIABLE) || (result == UniverseSolverResult::OPTIMUM_FOUND)) {
        solutionMutex.lock();
        auto local = bestSolution;
        solutionMutex.unlock();
        return local;
    }
    throw IllegalStateException("problem has no solution yet");
}

bool AbstractParallelSolver::checkSolution() {
    return solvers[winner]->checkSolution();
}

bool AbstractParallelSolver::checkSolution(const map<string, BigInteger> &assignment) {
    return solvers[0]->checkSolution(assignment);
}

IOptimizationSolver *AbstractParallelSolver::toOptimizationSolver() {
    return this;
}

bool AbstractParallelSolver::isMinimization() {
    return minimization;
}

void AbstractParallelSolver::setBounds(const BigInteger &lb, const BigInteger &ub) {
    this->lowerBound = lb;
    this->upperBound = ub;
    updateBounds();
}

void AbstractParallelSolver::setLowerBound(const BigInteger &lb) {
    this->lowerBound = lb;
    updateBounds();
}

BigInteger AbstractParallelSolver::getLowerBound() {
    return lowerBound;
}

void AbstractParallelSolver::setUpperBound(const BigInteger &ub) {
    this->upperBound = ub;
    updateBounds();
}

BigInteger AbstractParallelSolver::getUpperBound() {
    return upperBound;
}

BigInteger AbstractParallelSolver::getCurrentBound() {
    if (minimization) {
        return upperBound;
    }
    return lowerBound;
}

void AbstractParallelSolver::readMessages() {
    thread receiver([this]() {
        while (runningSolvers > 0) {
            auto message = communicator->receive(PANO_TAG_SOLVE, PANO_ANY_SOURCE, PANO_DEFAULT_MESSAGE_SIZE);
            readMessage(message);
            free(message);
        }
    });
    receiver.detach();
}

void AbstractParallelSolver::readMessage(const Message *message) {
    LOG_F(INFO, "main solver received a message '%s'", message->name);

    if (NAME_OF(message, IS(PANO_MESSAGE_SATISFIABLE))) {
        readSatisfiable(message);

    } else if (NAME_OF(message, IS(PANO_MESSAGE_NEW_BOUND_FOUND))) {
        readBound(message);

    } else if (NAME_OF(message, IS(PANO_MESSAGE_UNSATISFIABLE))) {
        readUnsatisfiable(message);

    } else if (NAME_OF(message, IS(PANO_MESSAGE_UNKNOWN))) {
        readUnknown(message);

    } else if (NAME_OF(message, IS(PANO_MESSAGE_END_SEARCH_ACK))) {
        readEnd(message);
    }
}

void AbstractParallelSolver::readSatisfiable(const Panoramyx::Message *message) {
    winner = message->read<unsigned>();
    currentRunningSolvers[winner] = false;
    result = UniverseSolverResult::SATISFIABLE;
    onSatisfiableFound(winner);
    solved.release();
}

void AbstractParallelSolver::readBound(const Panoramyx::Message *message) {
    auto src = message->read<unsigned>();
    string param(message->parameters + sizeof(unsigned), strlen(message->parameters + sizeof(unsigned)) + 1);
    BigInteger newBound = Universe::bigIntegerValueOf(param);
    result = UniverseSolverResult::SATISFIABLE;
    LOG_F(INFO, "solver #%d sent its current bound: %s", src, Universe::toString(newBound).c_str());
    currentRunningSolvers[src] = false;
    onNewBoundFound(newBound, src);
}

void AbstractParallelSolver::readUnsatisfiable(const Panoramyx::Message *message) {
    auto src = message->read<unsigned>();
    onUnsatisfiableFound(src);
}

void AbstractParallelSolver::readUnknown(const Panoramyx::Message *message) {
    auto src = message->read<unsigned>();
    onUnknown(src);
}

void AbstractParallelSolver::readEnd(const Panoramyx::Message *message) {
    runningSolvers--;
    if (runningSolvers <= 0) {
        interrupted = true;
        end.release();
    }
}

void AbstractParallelSolver::ready(unsigned solverIndex) {
    // Nothing to do by default.
}

void AbstractParallelSolver::beforeSearch() {
    // Nothing to do by default.
}

void AbstractParallelSolver::beforeSearch(unsigned int solverIndex) {
    // Nothing to do by default.
}

void AbstractParallelSolver::onSatisfiableFound(unsigned int solverIndex) {
    if (!interrupted) {
        solutionMutex.lock();
        bestSolution = solvers[solverIndex]->mapSolution();
        bestSolutionVector = solvers[solverIndex]->solution();
        solutionMutex.unlock();
    }
}

void AbstractParallelSolver::onNewBoundFound(const Universe::BigInteger &bound, unsigned int i) {
    throw UnsupportedOperationException("optimization problems are not supported yet by this solver");
}

void AbstractParallelSolver::onUnsatisfiableFound(unsigned solverIndex) {
    // Nothing to do by default.
}

void AbstractParallelSolver::onUnknown(unsigned int solverIndex) {
    // Nothing to do by default.
}

void AbstractParallelSolver::updateBounds() {
    throw UnsupportedOperationException("optimization problems are not supported yet by this solver");
}

void AbstractParallelSolver::endSearch() {
    for (auto &solver: solvers) {
        solver->endSearch();
    }
}

UniverseSolverResult AbstractParallelSolver::internalSolve(const vector<UniverseAssumption<BigInteger>> &assumpts) {
    // Preparing the solvers.
    beforeSearch();
    for (int i = 0; i < solvers.size(); i++) {
        beforeSearch(i);
    }

    // Solving the problem (with or without assumptions).
    if (assumpts.empty()) {
        startSearch();
    } else {
        startSearch(assumpts);
    }

    // Waiting for the solvers to answer.
    LOG_F(INFO, "before solved.acquire()");
    solved.acquire();
    LOG_F(INFO, "after solved.acquire()");
    endSearch();
    LOG_F(INFO, "after endSearch()");
    end.acquire();
    LOG_F(INFO, "after and.acquire()");

    // Returning the result.
    return result;
}
