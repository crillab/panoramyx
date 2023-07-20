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
 * @brief Defines a parallel solver that runs different solvers on the same input instance.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <iostream>

#include <crillab-panoramyx/solver/PortfolioSolver.hpp>

using namespace std;

using namespace Except;
using namespace Panoramyx;
using namespace Universe;

PortfolioSolver::PortfolioSolver(INetworkCommunication *comm, IBoundAllocationStrategy *allocationStrategy) :
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
        // FIXME: Avoid invoking nVariables() and nConstraints() like this...
        solvers[i]->nVariables();
        solvers[i]->nConstraints();
        solve(i);
        currentRunningSolvers[i] = true;
    }
}

void PortfolioSolver::startSearch(const vector<UniverseAssumption<BigInteger>> &assumpts) {
    for (unsigned i = 0; i < solvers.size(); i++) {
        // FIXME: Avoid invoking nVariables() and nConstraints() like this...
        solvers[i]->nVariables();
        solvers[i]->nConstraints();
        solve(i, assumpts);
        currentRunningSolvers[i] = true;
    }
}

void PortfolioSolver::solve(unsigned index) {
    if (solvers[index]->isOptimization()) {
        // FIXME: Maybe use a state design pattern here?
        assignBounds(index);
    }

    solvers[index]->solve();
}

void PortfolioSolver::solve(unsigned index, const vector<UniverseAssumption<BigInteger>> &assumpts) {
    if (solvers[index]->isOptimization()) {
        // FIXME: Maybe use a state design pattern here?
        assignBounds(index);
    }

    solvers[index]->solve(assumpts);
}

void PortfolioSolver::onNewBoundFound(const Universe::BigInteger &bound, unsigned int src) {
    // FIXME: Maybe use a state design pattern here?
    if (minimization && (bound < upperBound)) {
        // The upper bound has been improved.
        upperBound = bound;
        winner = src;
        bestSolution = solvers[winner]->mapSolution();
        DLOG_F(INFO, "new upper bound %s by solver #%d", Universe::toString(bound).c_str(), winner);

    } else if (!minimization && (lowerBound < bound)) {
        // The lower bound has been improved.
        lowerBound = bound;
        winner = src;
        bestSolution = solvers[winner]->mapSolution();
        DLOG_F(INFO, "new lower bound %s by solver #%d", Universe::toString(bound).c_str(), winner);

    } else {
        // The bound does not improve the current objective range.
        DLOG_F(INFO, "ignored bound %s (current range is %s .. %s)", Universe::toString(bound).c_str(),
               Universe::toString(lowerBound).c_str(), Universe::toString(upperBound).c_str());
    }

    // The bounds might need to be recomputed.
    updateBounds();
}

void PortfolioSolver::onUnsatisfiableFound(unsigned solverIndex) {
    // FIXME: Maybe use a state design pattern here?
    bool trueUnsat = !isOptimization();

    if (!trueUnsat) {
        // A new bound may have been obtained.
        auto bound = currentBounds[solverIndex];

        if (!minimization && (bound < upperBound)) {
            // The upper bound has been improved.
            upperBound = bound - 1;
            DLOG_F(INFO, "new upper bound %s by solver #%d", Universe::toString(upperBound).c_str(), solverIndex);

        } else if (minimization && (lowerBound < bound)) {
            // The lower bound has been improved.
            lowerBound = bound + 1;
            DLOG_F(INFO, "new lower bound %s by solver #%d", Universe::toString(lowerBound).c_str(), solverIndex);

        } else {
            // The bound does not improve the current objective range.
            DLOG_F(INFO, "ignored bound %s (current range is %s .. %s)", Universe::toString(bound).c_str(),
                   Universe::toString(lowerBound).c_str(), Universe::toString(upperBound).c_str());
        }

        // The problem may now be unsatisfiable, otherwise the bounds might need to be recomputed.
        if (lowerBound >= upperBound) {
            trueUnsat = true;
        } else {
            updateBounds();
        }
    }

    // Checking whether the problem is definitively solved now.
    if (trueUnsat) {
        winner = solverIndex;
        result = (result == Universe::UniverseSolverResult::SATISFIABLE) ?
                 Universe::UniverseSolverResult::OPTIMUM_FOUND :
                 Universe::UniverseSolverResult::UNSATISFIABLE;
        solved.release();
    }
}

void PortfolioSolver::updateBounds() {
    // Computing the new bound allocation.
    auto newBounds = allocationStrategy->computeBoundAllocation(currentBounds, lowerBound, upperBound);

    // Allocating the computed bounds.
    for (unsigned long i = 0; i < solvers.size(); i++) {
        auto solver = solvers[i];

        if (currentBounds[i] == currentBounds[i + 1]) {
            // When consecutive bounds are equal, there is no more bounds to assign.
            solver->interrupt();
            DLOG_F(INFO, "solver #%ld is definitively interrupted", i);
            continue;
        }

        // Actually assigning the bounds to the solver.
        DLOG_F(INFO, "assigning bounds for solver #%lu: %s .. %s", i,
               Universe::toString(currentBounds[i]).c_str(),
               Universe::toString(currentBounds[i + 1]).c_str());

        if (minimization && (newBounds[i + 1] != currentBounds[i + 1])) {
            // A new upper bound is assigned, so the solver must be updated.
            currentBounds[i + 1] = newBounds[i + 1];
            solver->setUpperBound(currentBounds[i + 1]);

        } else if (!minimization && (currentBounds[i] != newBounds[i])) {
            // A new lower bound is assigned, so the solver must be updated.
            currentBounds[i] = newBounds[i];
            solver->setLowerBound(currentBounds[i]);
        }

        // Checking whether the solver is running.
        DLOG_F(INFO, "solver #%lu %s running", i, (currentRunningSolvers[i] ? "is": "is not"));
        if (!currentRunningSolvers[i]) {
            // The solver must be restarted with its newly allocated bound.
            solver->reset();
            solver->solve();
            currentRunningSolvers[i] = true;
        }
    }
}

void PortfolioSolver::assignBounds(unsigned index) {
    DLOG_F(INFO, "assigning bounds for solver #%d: %s .. %s", index,
           Universe::toString(currentBounds[index]).c_str(),
           Universe::toString(currentBounds[index + 1]).c_str());

    if (minimization) {
        solvers[index]->setUpperBound(currentBounds[index + 1]);

    } else {
        solvers[index]->setLowerBound(currentBounds[index]);
    }
}
