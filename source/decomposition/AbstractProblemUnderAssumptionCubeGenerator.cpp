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
 * @file AbstractProblemUnderAssumptionCubeGenerator.hpp
 * @brief Defines the common behavior for cube generators based on ProblemUnderAssumption instances.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <loguru.hpp>

#include <cassert>
#include <crillab-panoramyx/decomposition/AbstractProblemUnderAssumptionCubeGenerator.hpp>
#include <crillab-panoramyx/decomposition/StreamPriorityQueueAdapter.hpp>

using namespace std;

using namespace Panoramyx;
using namespace Universe;

AbstractProblemUnderAssumptionCubeGenerator::AbstractProblemUnderAssumptionCubeGenerator(int nbCubesMax) :
        AbstractCubeGenerator(nbCubesMax) {
    // Nothing to do: everything is already initialized.
}

Stream<vector<UniverseAssumption<BigInteger>>> *AbstractProblemUnderAssumptionCubeGenerator::generateCubes() {
    // We first put in the queue the whole problem, without any assumption.
    priorityQueue.emplace(vector<UniverseAssumption<BigInteger>>(), 0);

    while ((!priorityQueue.empty()) && (priorityQueue.size() < nbCubesMax)) {
        // Trying to solve the next problem in the queue.
        auto problem = priorityQueue.top();
        priorityQueue.pop();
        solver->reset();
        const UniverseSolverResult result = solver->solve(problem.getAssumptions());

        if (result == Universe::UniverseSolverResult::UNSATISFIABLE) {
            // There is no solution to this problem, so it is discarded.
            continue;
        }

        // Looking for the next variable to assign.
        IUniverseVariable *nextVariable = nullptr;
        for (const auto &variable: solver->getVariablesMapping()) {
            LOG_F(INFO, "trying variable %s with size %d", variable.second->getName().c_str(), variable.second->getDomain()->currentSize());
            if (variable.second->getDomain()->currentSize() > 1) {
                nextVariable = variable.second;
                break;
            }
        }

        if (nextVariable == nullptr) {
            // Re-adding the previous problem with a 0-score.
            priorityQueue.emplace(problem.getAssumptions(), 0);

        } else {
            // Creating assumptions with the found variable.
            for (const auto &value : nextVariable->getDomain()->getCurrentValues()) {
                vector<UniverseAssumption<BigInteger>> newAssumpts(problem.getAssumptions());
                newAssumpts.emplace_back(nextVariable->getName(), true, value);
                const long score = computeScore(newAssumpts);
                if (score >= 0) {
                    priorityQueue.emplace(newAssumpts, score);
                }
            }
        }

        // If the first problem has 0 as score, then all variables are assigned for all problems.
        if ((!priorityQueue.empty()) && (priorityQueue.top().getScore() == 0)) {
            break;
        }
    }

    // Returning the computed problems as a stream of assumptions.
    return new StreamPriorityQueueAdapter(priorityQueue);
}
