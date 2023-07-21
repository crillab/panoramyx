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

#include <crillab-panoramyx/decomposition/StreamLexicographicIntervalCube.hpp>

using namespace std;

using namespace Universe;
using namespace Panoramyx;

StreamLexicographicIntervalCube::StreamLexicographicIntervalCube(
        const map<string, IUniverseVariable *> &mapping, size_t nbCubeMax, int nbIntervals) :
        mapping(mapping),
        nbCubeMax(nbCubeMax),
        nbIntervals(nbIntervals),
        current(),
        variables(),
        indexesCurrentValues(),
        variablesFinished() {
    // Nothing to do: everything is already initialized.
}

bool StreamLexicographicIntervalCube::hasNext() const {
    return indexesCurrentValues.empty() || !variablesFinished[variablesFinished.size() - 1];
}

vector<UniverseAssumption<BigInteger>> StreamLexicographicIntervalCube::next() {
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

void StreamLexicographicIntervalCube::generateFirst() {
    size_t estimatedCubeCount = 1;

    for (auto &variable: mapping) {
        // Initializing internal data-structures.
        variables.push_back(variable.second);
        indexesCurrentValues.emplace_back(0);
        variablesFinished.emplace_back(false);

        // Updating the estimated count of cubes based on the size of the domain of the current variable.
        if (variable.second->getDomain()->size() < nbIntervals) {
            assume(variables.size() - 1, 0);
            estimatedCubeCount *= variable.second->getDomain()->size();

        } else {
            assume(variables.size() - 1, 0, variable.second->getDomain()->size() / nbIntervals);
            estimatedCubeCount *= nbIntervals;
        }

        // When there are enough cubes, we stop.
        if (estimatedCubeCount >= nbCubeMax) {
            break;
        }
    }
}

void StreamLexicographicIntervalCube::generateNext() {
    // Backtracking to a variable whose domain is not fully explored.
    int varIndex = backtrack();
    if (varIndex < 0) {
        // We have tried all remaining assignments, and no one is consistent.
        current.clear();
        return;
    }

    // Assuming the next value for the current variable.
    if (variables[varIndex]->getDomain()->size() < nbIntervals) {
        assume(varIndex, indexesCurrentValues[varIndex] + 1);

    } else {
        assume(varIndex, indexesCurrentValues[varIndex] + 1, variables[varIndex]->getDomain()->size() / nbIntervals);
    }

    // Completing the cube with the remaining variables.
    for (varIndex++; varIndex < variables.size(); varIndex++) {
        if (variables[varIndex]->getDomain()->size() < nbIntervals) {
            assume(varIndex, 0);

        } else {
            assume(varIndex, 0, variables[varIndex]->getDomain()->size() / nbIntervals);
        }
    }
}

int StreamLexicographicIntervalCube::backtrack() {
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

void StreamLexicographicIntervalCube::assume(int varIndex, int valIndex) {
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

void StreamLexicographicIntervalCube::assume(int varIndex, int valIndex, int intervalSize) {
    // Adding the minimum assumption to the cube.
    current.emplace_back(variables[varIndex]->getName(),
                         false,
                         variables[varIndex]->getDomain()->getValues().at(valIndex));

    // Adding the maximum assumption to the cube.
    int maxIndex = valIndex + intervalSize;
    if (maxIndex >= variables[varIndex]->getDomain()->size()) {
        maxIndex = variables[varIndex]->getDomain()->size() - 1;
        current.emplace_back(variables[varIndex]->getName(),
                             false,
                             variables[varIndex]->getDomain()->getValues().at(maxIndex) + 1);
        variablesFinished[varIndex] = true;
    } else {
        current.emplace_back(variables[varIndex]->getName(),
                             false,
                             variables[varIndex]->getDomain()->getValues().at(maxIndex));
        variablesFinished[varIndex] = false;
    }

    // Updating the index of the current value.
    indexesCurrentValues[varIndex] = maxIndex - 1;

    // Checking whether the domain of the variable has been fully explored.
    if (varIndex != 0) {
        variablesFinished[varIndex] = variablesFinished[varIndex] && variablesFinished[varIndex - 1];
    }
}
