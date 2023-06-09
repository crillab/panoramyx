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

#include "../../include/solver/AbstractParallelSolver.hpp"

#include <mpi.h>

#include <thread>

#include "../../include/network/Message.hpp"

using namespace std;

using namespace Except;
using namespace Panoramyx;
using namespace Universe;

AbstractParallelSolver::AbstractParallelSolver(INetworkCommunication *comm, IAllocationStrategy *allocationStrategy) : communicator(comm),
                                                                                                                       solvers(),
                                                                                                                       runningSolvers(0),
                                                                                                                       allocationStrategy(allocationStrategy),
                                                                                                                       currentBounds(),
                                                                                                                       minimization(true),
                                                                                                                       lowerBound(0),
                                                                                                                       upperBound(0),
                                                                                                                       interrupted(false),
                                                                                                                       solved(0),
                                                                                                                       result(Universe::UniverseSolverResult::UNKNOWN),
                                                                                                                       winner(-1),
                                                                                                                       end(0) {
    currentBounds.push_back(0);
}

void AbstractParallelSolver::addSolver(RemoteSolver *solver) {
    unsigned index = solvers.size();
    solvers.push_back(solver);
    currentBounds.push_back(LLONG_MAX);
    currentRunningSolvers.push_back(false);
    solver->setComm(communicator);
    solver->setIndex(index);
    runningSolvers++;
}

void AbstractParallelSolver::loadInstance(const string &filename) {
    for (auto &solver : solvers) {
        solver->loadInstance(filename);
    }
}

void AbstractParallelSolver::reset() {
    // FIXME: make sure that the semaphores, etc. are also reset.
    for (auto &solver : solvers) {
        solver->reset();
    }
}

int AbstractParallelSolver::nVariables() {
    return solvers[0]->nVariables();
}

const map<string, IUniverseVariable *> &AbstractParallelSolver::getVariablesMapping() {
    throw UnsupportedOperationException("Parallel solver has too many mappings!");
}

int AbstractParallelSolver::nConstraints() {
    return solvers[0]->nConstraints();
}

void AbstractParallelSolver::setTimeout(long seconds) {
    for (auto &solver : solvers) {
        solver->setTimeout(seconds);
    }
}

void AbstractParallelSolver::setTimeoutMs(long mseconds) {
    for (auto &solver : solvers) {
        solver->setTimeoutMs(mseconds);
    }
}

void AbstractParallelSolver::setVerbosity(int level) {
    for (auto &solver : solvers) {
        solver->setVerbosity(level);
    }
}

void AbstractParallelSolver::setLogFile(const string &filename) {
    // TODO Make sure that the solvers to not write their logs in the same file.
}

UniverseSolverResult AbstractParallelSolver::solve() {
    return solve(vector<UniverseAssumption<BigInteger>>());
}

UniverseSolverResult AbstractParallelSolver::solve(const string &filename) {
    loadInstance(filename);
    return solve();
}

UniverseSolverResult AbstractParallelSolver::solve(const vector<UniverseAssumption<BigInteger>> &assumpts) {
    readMessages();
    for (unsigned i = 0; i < solvers.size(); i++) {
        ready(i);
    }
    return internalSolve(assumpts);
}

UniverseSolverResult AbstractParallelSolver::internalSolve(const vector<UniverseAssumption<BigInteger>> &assumpts) {
    beforeSearch();

    if (assumpts.empty()) {
        startSearch();
    } else {
        startSearch(assumpts);
    }

    solved.acquire();
    endSearch();
    end.acquire();
    return result;
}

void AbstractParallelSolver::interrupt() {
    for (auto &solver : solvers) {
        solver->interrupt();
    }
    interrupted = true;
}

vector<BigInteger> AbstractParallelSolver::solution() {
    if (result == UniverseSolverResult::SATISFIABLE || result == UniverseSolverResult::OPTIMUM_FOUND) {
        return solvers[winner]->solution();
    }
    throw IllegalStateException("problem has no solution yet");
}

map<string, BigInteger> AbstractParallelSolver::mapSolution() {
    if (result == UniverseSolverResult::SATISFIABLE || result == UniverseSolverResult::OPTIMUM_FOUND) {
        return bestSolution;
    }
    throw IllegalStateException("problem has no solution yet");
}

void AbstractParallelSolver::readMessages() {
    thread receiver([this]() {
        while (!interrupted) {
            auto message = communicator->receive(PANO_TAG_SOLVE, PANO_ANY_SOURCE, PANO_DEFAULT_MESSAGE_SIZE);
            readMessage(message);
            free(message);
        }
    });
    receiver.detach();
}

void AbstractParallelSolver::readMessage(const Message *message) {
    DLOG_F(INFO, "abstract readMessage %s", message->name);
    if (NAME_OF(message, IS(PANO_MESSAGE_SATISFIABLE))) {
        winner = message->read<unsigned>();
        currentRunningSolvers[winner]= false;
        result = UniverseSolverResult::SATISFIABLE;
        onSatisfiableFound(winner);
        solved.release();

    } else if (NAME_OF(message, IS(PANO_MESSAGE_UNSATISFIABLE))) {
        auto src = message->read<unsigned>();
        onUnsatisfiableFound(src);

    } else if (NAME_OF(message, IS(PANO_MESSAGE_END_SEARCH))) {
        runningSolvers--;
        if (runningSolvers <= 0) {
            interrupted = true;
            end.release();
        }

    } else if (NAME_OF(message, IS(PANO_MESSAGE_NEW_BOUND_FOUND))) {
        auto src = message->read<unsigned>();
        string param(message->parameters + sizeof(unsigned), strlen(message->parameters + sizeof(unsigned)) + 1);
        BigInteger newBound = bigIntegerValueOf(param);
        result = Universe::UniverseSolverResult::SATISFIABLE;
        LOG_F(INFO,"solver %d send current bound: %lld",src,newBound);
        currentRunningSolvers[src]= false;
        onNewBoundFound(newBound, src);
    }
}

void AbstractParallelSolver::ready(unsigned solverIndex) {
    // Nothing to do by default.
}

void AbstractParallelSolver::beforeSearch() {
    // Nothing to do by default.
}

void AbstractParallelSolver::onSatisfiableFound(unsigned solverIndex) {
    bestSolution=solvers[solverIndex]->mapSolution();
}

void AbstractParallelSolver::onNewBoundFound(const Universe::BigInteger &bound, unsigned int i) {
    throw UnsupportedOperationException("Optimization problems not supported");
}

void AbstractParallelSolver::onUnsatisfiableFound(unsigned solverIndex) {
    // Nothing to do by default.
}

void AbstractParallelSolver::endSearch() {
    for (auto &solver : solvers) {
        solver->endSearch();
    }
}

UniverseSolverResult AbstractParallelSolver::getResult() {
    return result;
}

BigInteger AbstractParallelSolver::getCurrentBound() {
    if(minimization){
        return upperBound;
    }else{
        return lowerBound;
    }
}

BigInteger AbstractParallelSolver::getLowerBound() {
    return lowerBound;
}

void AbstractParallelSolver::setLowerBound(const BigInteger &lb) {

}

BigInteger AbstractParallelSolver::getUpperBound() {
    return upperBound;
}

void AbstractParallelSolver::setUpperBound(const BigInteger &ub) {

}

void AbstractParallelSolver::setBounds(const BigInteger &lb, const BigInteger &ub) {

}

bool AbstractParallelSolver::isMinimization() {
    return minimization;
}

bool AbstractParallelSolver::isOptimization() {
    return solvers[0]->isOptimization();
}

void AbstractParallelSolver::decisionVariables(const vector<std::string> &variables) {
    for(auto& solver:solvers){
        solver->decisionVariables(variables);
    }
}

void AbstractParallelSolver::addSearchListener(Universe::IUniverseSearchListener *listener) {
    for(auto& solver:solvers){
        solver->addSearchListener(listener);
    }
}

void AbstractParallelSolver::setLogStream(ostream &stream) {
    //TODO
}

map<std::string, Universe::BigInteger> AbstractParallelSolver::mapSolution(bool excludeAux) {
    if (result == UniverseSolverResult::SATISFIABLE || result == UniverseSolverResult::OPTIMUM_FOUND) {
        return bestSolution;
    }
    throw IllegalStateException("problem has no solution yet");
}

IOptimizationSolver *AbstractParallelSolver::toOptimizationSolver() {
    return this;
}
