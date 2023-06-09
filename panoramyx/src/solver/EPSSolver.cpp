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
 * @file EPSSolver.cpp
 * @brief Defines a parallel solver that applies a cube-and-conquer approach to solve an instance.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <thread>

#include "../../../libs/loguru/loguru.hpp"

#include "../../include/network/Message.hpp"
#include "../../include/solver/EPSSolver.hpp"

using namespace std;

using namespace Except;
using namespace Panoramyx;
using namespace Universe;

EPSSolver::EPSSolver(INetworkCommunication *comm, ICubeGenerator *generator) :
        AbstractParallelSolver(comm),
        generator(generator),
        cubes(0) {
    // Nothing to do: everything is already initialized.
}

void EPSSolver::ready(unsigned solverIndex) {
    availableSolvers.add(solvers[solverIndex]);
}

void EPSSolver::startSearch() {
    std::thread t([this]() {
        int nbCubes = 0;

        // Generating the cubes, and assigning them to the different solvers.
        for (auto cube : *this->generator->generateCubes()) {
            if (cube.empty()) {
                // There is no more consistent cubes.
                break;
            }

            // Solving the cube using one of the available solvers.
            // FIXME: We must indeed release the semaphore when clearing, because we must stop waiting for a new one...
            try {
                nbCubes++;
                DLOG_F(INFO, "generate cubes %d", nbCubes);
                auto s = availableSolvers.get();
                currentRunningSolvers[((RemoteSolver*)s)->getIndex()]=true;
                s->solve(cube);

            } catch (Except::NoSuchElementException &e) {
                return;
            }
        }

        // All cubes have been generated.
        // We must wait for the solvers to solve them.
        waitForAllCubes(nbCubes);
    });

    t.detach();
}

void EPSSolver::startSearch(const vector<UniverseAssumption<BigInteger>> &assumpts) {
    throw UnsupportedOperationException("cannot use assumptions in EPS mode");
}

void EPSSolver::onSatisfiableFound(unsigned solverIndex) {
    AbstractParallelSolver::onSatisfiableFound(solverIndex);
    availableSolvers.clear();
    cubes.release();
}

void EPSSolver::onUnsatisfiableFound(unsigned solverIndex) {
    solvers[solverIndex]->reset();
    availableSolvers.add(solvers[solverIndex]);
    cubes.release();
}

void EPSSolver::waitForAllCubes(int nbCubes) {
    for (int i = 0; i < nbCubes; i++) {
        cubes.acquire();
        if (result == Universe::UniverseSolverResult::SATISFIABLE) {
            // One of the cube has a solution, so the search is finished.
            return;
        }
    }

    // None of the cubes has a solution: the problem is unsatisfiable.
    result = Universe::UniverseSolverResult::UNSATISFIABLE;
    solved.release();
}

void EPSSolver::loadInstance(const string &filename) {
    AbstractParallelSolver::loadInstance(filename);
    this->generator->loadInstance(filename);
}
