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
 * @file StreamLexicographicCube.cpp
 * @brief Provides a stream of lexicographic cubes.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <loguru.hpp>

#include <crillab-panoramyx/decomposition/StreamLexicographicCube.hpp>

using namespace std;

using namespace Universe;
using namespace Panoramyx;

StreamLexicographicCube::StreamLexicographicCube(const map<string, IUniverseVariable *> &mapping,
                                                 IConsistencyChecker *checker,
                                                 size_t nbCubeMax) :
        branchingVariables(),
        mapping(mapping),
        consistencyChecker(checker),
        nbCubeMax(nbCubeMax),
        current(),
        variables(),
        indexesCurrentValues(),
        variablesFinished() {
    for (auto &variable : mapping) {
        branchingVariables.push_back(variable.first);
    }
}

StreamLexicographicCube::StreamLexicographicCube(const vector<string> &branchingVariables,
                                                 const map<string, IUniverseVariable *> &mapping,
                                                 IConsistencyChecker *checker,
                                                 size_t nbCubeMax) :
        branchingVariables(branchingVariables),
        mapping(mapping),
        consistencyChecker(checker),
        nbCubeMax(nbCubeMax),
        current(),
        variables(),
        indexesCurrentValues(),
        variablesFinished() {
    LOG_F(INFO, "size of branching variables: %d", branchingVariables.size());
    if (branchingVariables.empty()) {
        for (auto &variable: mapping) {
            this->branchingVariables.push_back(variable.first);
        }
    }
}

bool StreamLexicographicCube::hasNext() const {
    return indexesCurrentValues.empty() || !variablesFinished[variablesFinished.size() - 1];
}

vector<UniverseAssumption<BigInteger>> StreamLexicographicCube::next() {
    if (indexesCurrentValues.empty()) {
        // No cube has been generated yet.
        generateFirst();

    } else {
        // Generating the next cube.
        generateNext();
    }

    // Once generated, we return the current cube.
    return current;
}

void StreamLexicographicCube::generateFirst() {
    size_t estimatedCubeCount = 1;
    for (auto &identifier : branchingVariables) {
        // Initializing internal data-structures.
        auto *variable = mapping.at(identifier);
        variables.push_back(variable);
        indexesCurrentValues.emplace_back(0);
        variablesFinished.emplace_back(false);

        // Assuming the first value of the domain for this variable.
        assume(variables.size() - 1, 0);

        // Updating the estimated count of cubes based on the size of the domain of the current variable.
        estimatedCubeCount *= variable->getDomain()->size();
        if (estimatedCubeCount >= nbCubeMax) {
            break;
        }
    }

    if (!consistencyChecker->checkPartial(current) || !consistencyChecker->checkFinal(current)) {
        // The first cube is inconsistent, we must generate another one.
        generateNext();
    }
}

void StreamLexicographicCube::generateNext() {
    bool partiallyConsistent;

    do {
        // Backtracking to a variable whose domain is not fully explored.
        int varIndex = backtrack();
        if (varIndex < 0) {
            // We have tried all remaining assignments, and no one is consistent.
            current.clear();
            return;
        }

        // Assuming the next value for the current variable.
        assume(varIndex, indexesCurrentValues[varIndex] + 1);
        partiallyConsistent = consistencyChecker->checkPartial(current);

        // Completing the cube with the remaining variables.
        for (varIndex++; partiallyConsistent && (varIndex < variables.size()); varIndex++) {
            partiallyConsistent = false;

            // Looking for a consistent value.
            for (int valIndex = 0; valIndex < variables[varIndex]->getDomain()->size(); valIndex++) {
                assume(varIndex, valIndex);
                if (consistencyChecker->checkPartial(current)) {
                    partiallyConsistent = true;
                    break;
                }
                current.pop_back();
            }
        }
    } while (!partiallyConsistent || !consistencyChecker->checkFinal(current));
}

int StreamLexicographicCube::backtrack() {
    int varIndex;

    for (varIndex = current.size() - 1; varIndex >= 0; varIndex--) {
        // Undoing the assignment.
        current.pop_back();

        if (indexesCurrentValues[varIndex] != (variables[varIndex]->getDomain()->size() - 1)) {
            // Some values are still to be explored in the domain of this variable.
            break;
        }
    }

    return varIndex;
}

void StreamLexicographicCube::assume(int varIndex, int valIndex) {
    // Adding the assumption to the cube.
    current.emplace_back(variables[varIndex]->getName(),
                         true,
                         variables[varIndex]->getDomain()->getValues().at(valIndex));

    // Updating the index of the current value.
    indexesCurrentValues[varIndex] = valIndex;

    // Checking whether the domain of the variable has been fully explored.
    variablesFinished[varIndex] = (indexesCurrentValues[varIndex] == (variables[varIndex]->getDomain()->size() - 1));
    if (varIndex != 0) {
        variablesFinished[varIndex] = variablesFinished[varIndex] && variablesFinished[varIndex - 1];
    }
}
