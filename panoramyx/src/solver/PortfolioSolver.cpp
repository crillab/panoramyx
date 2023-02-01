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

using namespace std;

using namespace Except;
using namespace Panoramyx;
using namespace Universe;

PortfolioSolver::PortfolioSolver(INetworkCommunication *comm, IAllocationStrategy *allocationStrategy) :
        AbstractParallelSolver(comm, allocationStrategy) {
    // Nothing to do: everything is already initialized.
}

void PortfolioSolver::beforeSearch() {
    if (solvers[0]->isOptimization()) {
        // FIXME: Maybe use a state design pattern here?
        auto lb = solvers[0]->getLowerBound();
        auto ub = solvers[0]->getUpperBound();
        currentBounds = allocationStrategy->computeBoundAllocation(currentBounds, lb, ub);
    }
}

void PortfolioSolver::startSearch() {
    for (unsigned i = 0; i < solvers.size(); i++) {
        solve(i);
    }
}

void PortfolioSolver::startSearch(const vector<UniverseAssumption<BigInteger>> &assumpts) {
    for (unsigned i = 0; i < solvers.size(); i++) {
        solve(i, assumpts);
    }
}

void PortfolioSolver::solve(unsigned i) {
    if (solvers[i]->isOptimization()) {
        // FIXME: Maybe use a state design pattern here?
        solvers[i]->setBounds(currentBounds[i], currentBounds[i + 1]);
    }
    solvers[i]->solve();
}

void PortfolioSolver::solve(unsigned i, const vector<UniverseAssumption<BigInteger>> &assumpts) {
    if (solvers[i]->isOptimization()) {
        // FIXME: Maybe use a state design pattern here?
        solvers[i]->setBounds(currentBounds[i], currentBounds[i + 1]);
    }
    solvers[i]->solve(assumpts);
}

void PortfolioSolver::onNewBoundFound(const BigInteger &bound) {
    // FIXME: Maybe use a state design pattern here?
    // TODO: Interrupts solvers that are out of bounds.
    if (isMinimization && bound < upperBound) {
        upperBound = bound;
        for (auto solver: solvers) {
            solver->setUpperBound(upperBound);
        }
    } else if (!isMinimization && lowerBound < bound) {
        lowerBound = bound;
        for (auto solver: solvers) {
            solver->setLowerBound(lowerBound);
        }
    }
}

void PortfolioSolver::onUnsatisfiableFound(unsigned solverIndex) {
    // FIXME: If OPTIM, update bounds
    winner = solverIndex;
    result = Universe::UniverseSolverResult::UNSATISFIABLE;
    solved.release();
}
