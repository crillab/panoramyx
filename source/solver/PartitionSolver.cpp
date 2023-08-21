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
 * @file PartitionSolver.cpp
 * @brief Defines a parallel solver that solves in parallel independent sub-problems of the original problem.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <loguru.hpp>

#include <limits>
#include <thread>

#include <crillab-except/except.hpp>
#include <crillab-panoramyx/decomposition/HypergraphDecompositionCubeGenerator.hpp>
#include <crillab-panoramyx/solver/PartitionSolver.hpp>
#include <crillab-panoramyx/network/MessageBuilder.hpp>

using namespace std;

using namespace Except;
using namespace Panoramyx;
using namespace Universe;

PartitionSolver::PartitionSolver(INetworkCommunication *comm, IHypergraphDecompositionSolver *decompositionSolver) :
        AbstractParallelSolver(comm),
        decompositionSolver(decompositionSolver),
        generator(new HypergraphDecompositionCubeGenerator(INT_MAX, decompositionSolver)),
        constraintsInPartitions(),
        partitions(0),
        numberOfSolutions(0) {
    // Nothing to do: everything is already initialized.
}

PartitionSolver::~PartitionSolver() {
    delete generator;
}

void PartitionSolver::loadInstance(const string &filename) {
    AbstractParallelSolver::loadInstance(filename);
    this->decompositionSolver->loadInstance(filename);
    decompositionSolver->solve();
}

void PartitionSolver::beforeSearch() {
    constraintsInPartitions = decompositionSolver->getPartition();
}

void PartitionSolver::beforeSearch(unsigned int solverIndex) {
    // Ignoring constraints that are not in the partition assigned to the solver.
    auto *solver = solvers[solverIndex];
    auto &constraints = solver->getConstraints();
    auto &solverPartition = constraintsInPartitions[solverIndex];

    if (solverPartition.empty()) {
        // All constraints are ignored as the partition is empty.
        for (auto * constraint : constraints) {
            constraint->setIgnored(true);
        }

    } else {
        for (int current = 0, i = 0; i < constraints.size(); i++) {
            if ((current < solverPartition.size()) && (i == solverPartition[current])) {
                // This constraint is in the partition, so it must be kept.
                constraints[i]->setIgnored(false);
                current++;

            } else {
                // The constraint is not in the partition, so it is ignored.
                constraints[i]->setIgnored(true);
            }
        }
    }

    // TODO Should we restrict variables in the solver? Maybe with decisionVariables()?
}

void PartitionSolver::startSearch() {
    std::thread solvingThread([this]() {
        // Generating the cubes, and assigning them to the different solvers.
        for (auto cube : *this->generator->generateCubes()) {
            if (cube.empty()) {
                // There is no more consistent cubes: the whole search space has been explored.
                result = UniverseSolverResult::UNSATISFIABLE;
                solved.release();
                break;
            }

            // Solving the cubes for the different sub-problems.
            numberOfSolutions = 0;
            bestSolution.clear();
            for (auto *solver : solvers) {
                solver->solve(cube);
            }

            // Waiting for all sub-problems to be solved.
            for (int i = 0; i < solvers.size(); i++) {
                partitions.acquire();
            }

            // Checking whether a solution has been found.
            if (result == UniverseSolverResult::SATISFIABLE) {
                solved.release();
                break;
            }
        }
    });

    solvingThread.detach();
}

void PartitionSolver::startSearch(const vector<UniverseAssumption<BigInteger>> &assumpts) {
    // Solving the cubes for the different sub-problems.
    numberOfSolutions = 0;
    bestSolution.clear();
    for (const auto &a : assumpts) {
        bestSolution[a.getVariableId()] = a.getValue();
    }

    LOG_F(INFO, "working with %d solvers", solvers.size());
    for (auto *solver : solvers) {
        LOG_F(INFO, "sending one more cube");
        solver->solve(assumpts);
    }

    // Waiting for all sub-problems to be solved.
    for (int i = 0; i < solvers.size(); i++) {
        partitions.acquire();
        LOG_F(INFO, "%d partitions solved", i + 1);
    }

    solved.release();
}

void PartitionSolver::interrupt() {
    AbstractParallelSolver::interrupt();
    //solved.release();
}

void PartitionSolver::onSatisfiableFound(unsigned solverIndex) {
    solutionMutex.lock();

    // A new (partial) solution has been found.
    numberOfSolutions++;
    if (numberOfSolutions == solvers.size()) {
        result = UniverseSolverResult::SATISFIABLE;
    }

    auto sol = solvers[solverIndex]->mapSolution();
    auto vec = decompositionSolver->getVariablePartition();
    for (const auto &var : vec[solverIndex]) {
        if (sol.contains(var)) {
            bestSolution[var] = sol[var];
        }
    }
    solutionMutex.unlock();

    // The solver has finished its sub-problem.
    partitions.release();
}

void PartitionSolver::onUnsatisfiableFound(unsigned) {
    // The current cube is unsatisfiable, so all remaining solvers can be interrupted.
    for (auto *solver : solvers) {
        solver->interrupt();
    }

    // The solver has finished its sub-problem.
    partitions.release();
}

void PartitionSolver::onUnknown(unsigned) {
    // The solver has finished its sub-problem.
    partitions.release();
}

int PartitionSolver::getRunningSolvers() const {
    return runningSolvers;
}

Universe::UniverseSolverResult
PartitionSolver::internalSolve(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) {
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
    LOG_F(INFO, "partition before solved.acquire()");
    solved.acquire();
    LOG_F(INFO, "partition after solved.acquire()");

    // Returning the result.
    return result;
}

void PartitionSolver::readEnd(const Panoramyx::Message *message) {
    AbstractParallelSolver::readEnd(message);
    if (runningSolvers <= 0) {
        MessageBuilder mb;
        Message *r = mb.named(PANO_MESSAGE_END_SEARCH_ACK).withTag(PANO_TAG_SOLVE).build();
        communicator->send(r, 0);
        free(r);
    }
}

void PartitionSolver::endSearch() {
    interrupt();
    AbstractParallelSolver::endSearch();
}