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
 * @file PortfolioSolver.cpp
 * @brief Defines a parallel solver that runs different solvers on the same instance.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include "../../include/solver/PortfolioSolver.hpp"
#include <iostream>

using namespace std;

using namespace Except;
using namespace Panoramyx;
using namespace Universe;

PortfolioSolver::PortfolioSolver(INetworkCommunication *comm, IAllocationStrategy *allocationStrategy) :
        AbstractParallelSolver(comm, allocationStrategy) {
    // Nothing to do: everything is already initialized.
}

void PortfolioSolver::beforeSearch() {
    if (isOptimization()) {
        // FIXME: Maybe use a state design pattern here?
        minimization = solvers[0]->isMinimization();
        allocationStrategy->setMinimization(minimization);
        lowerBound = solvers[0]->getLowerBound();
        upperBound = solvers[0]->getUpperBound();
        currentBounds = allocationStrategy->computeBoundAllocation(currentBounds, lowerBound, upperBound);
    }
}

void PortfolioSolver::startSearch() {
    for (unsigned i = 0; i < solvers.size(); i++) {
        solve(i);
        currentRunningSolvers[i] = true;
        solvers[i]->nVariables();
        solvers[i]->nConstraints();
    }
}

void PortfolioSolver::startSearch(const vector<UniverseAssumption<BigInteger>> &assumpts) {
    for (unsigned i = 0; i < solvers.size(); i++) {
        solve(i, assumpts);
        currentRunningSolvers[i] = true;
    }
}

void PortfolioSolver::solve(unsigned i) {
    if (solvers[i]->isOptimization()) {
        LOG_F(INFO, "range of bounds for solver %d: %s ... %s", i, Universe::toString(currentBounds[i]).c_str(),
              Universe::toString(currentBounds[i + 1]).c_str());
        // FIXME: Maybe use a state design pattern here?
        if (minimization) {
            solvers[i]->setUpperBound(currentBounds[i + 1]);
        } else {
            solvers[i]->setLowerBound(currentBounds[i]);
        }
    }
    solvers[i]->solve();
}

void PortfolioSolver::solve(unsigned i, const vector<UniverseAssumption<BigInteger>> &assumpts) {
    if (solvers[i]->isOptimization()) {
        LOG_F(INFO, "range of bounds for solver %d: %s ... %s", i, Universe::toString(currentBounds[i]).c_str(),
              Universe::toString(currentBounds[i + 1]).c_str());
        // FIXME: Maybe use a state design pattern here?
        if (minimization) {
            solvers[i]->setUpperBound(currentBounds[i + 1]);
        } else {
            solvers[i]->setLowerBound(currentBounds[i]);
        }

    }
    solvers[i]->solve(assumpts);
}

void PortfolioSolver::onNewBoundFound(const Universe::BigInteger &bound, unsigned int src) {
    // FIXME: Maybe use a state design pattern here?
    // TODO GMP case
    if (minimization && bound < upperBound) {
        upperBound = bound;
        winner = src;
        bestSolution = solvers[winner]->mapSolution();
        LOG_F(INFO, "New upper bound %lld by solver %d", bound, winner);
    } else if (!minimization && lowerBound < bound) {
        lowerBound = bound;
        winner = src;
        bestSolution = solvers[winner]->mapSolution();
        LOG_F(INFO, "New lower bound %lld by solver %d", bound, winner);
    } else {
        LOG_F(INFO, "else %lld %lld %lld", lowerBound, bound, upperBound);
    }
    updateBounds();
}

void PortfolioSolver::onUnsatisfiableFound(unsigned solverIndex) {
    // FIXME: Maybe use a state design pattern here?
    // TODO GMP case
    bool trueUnsat = !isOptimization();
    if (!trueUnsat) {
        auto bound = currentBounds[solverIndex];
        if (!minimization && bound < upperBound) {
            upperBound = bound - 1;
            LOG_F(INFO, "New upper bound %lld by solver %d", upperBound, solverIndex);
        } else if (minimization && lowerBound < bound) {
            lowerBound = bound + 1;
            LOG_F(INFO, "New lower bound %lld by solver %d", lowerBound, solverIndex);
        }

        if (lowerBound >= upperBound) {
            trueUnsat = true;
        } else {
            updateBounds();
        }

    }

    if (trueUnsat) {
        winner = solverIndex;
        result = (result == Universe::UniverseSolverResult::SATISFIABLE) ?
                Universe::UniverseSolverResult::OPTIMUM_FOUND :
                Universe::UniverseSolverResult::UNSATISFIABLE;
        solved.release();
    }

}

void PortfolioSolver::updateBounds() {
    auto newBounds = allocationStrategy->computeBoundAllocation(currentBounds, lowerBound, upperBound);
    for (unsigned long i = 0; i < solvers.size(); i++) {
        auto solver = solvers[i];
        if (currentBounds[i] == currentBounds[i+1]) {
            // No more bounds.
            solver->interrupt();
            LOG_F(INFO, "Solver %ld is definitively interrupted", i);
            continue;
        }

        LOG_F(INFO, "range of bounds for solver %ld: %s ... %s", i, toString(currentBounds[i]).c_str(),
              toString(currentBounds[i + 1]).c_str());
        if (minimization && newBounds[i + 1] != currentBounds[i + 1]) {
            currentBounds[i + 1] = newBounds[i + 1];
            solver->setUpperBound(currentBounds[i + 1]);

        } else if (!minimization && currentBounds[i] != newBounds[i]) {
            currentBounds[i] = newBounds[i];
            solver->setLowerBound(currentBounds[i]);
        }
        LOG_F(INFO, "Is solver %lu running? %d", i, (int) currentRunningSolvers[i]);
        if (!currentRunningSolvers[i]) {
            solver->reset();
            solver->solve();
            currentRunningSolvers[i] = solver;
        }
    }
}
