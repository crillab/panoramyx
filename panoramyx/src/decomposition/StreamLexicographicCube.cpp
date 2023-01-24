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

#include "../../include/decomposition/StreamLexicographicCube.hpp"

using namespace Universe;
using namespace std;
using namespace Panoramyx;

StreamLexicographicCube::StreamLexicographicCube(const map<string, IUniverseVariable *> &mapping,
                                                 IConsistencyChecker *checker,
                                                 size_t nbCubeMax) :
        mapping(mapping),
        consistencyChecker(checker),
        nbCubeMax(nbCubeMax),
        current(),
        variables(),
        indexesCurrentValues(),
        variablesFinished() {
    // Nothing to do: everything is already initialized.
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

    for (auto &variable : mapping) {
        // Assuming the first value of the domain for this variable.
        current.emplace_back(variable.second->getId(), true, variable.second->getDomain()->getValues().at(0));
        variables.push_back(variable.second);
        indexesCurrentValues.emplace_back(0);

        // Checking whether the domain of this variable is fully explored.
        // As we assumed the first value, it is the case iff the domain is a singleton.
        bool currentVariableIsFinish = (variable.second->getDomain()->size() == 1);
        if (!variablesFinished.empty()) {
            currentVariableIsFinish &= variablesFinished[variablesFinished.size() - 1];
        }
        variablesFinished.emplace_back(currentVariableIsFinish);

        // Updating the estimated count of cubes based on the size of the domain of the current variable.
        estimatedCubeCount *= variable.second->getDomain()->size();
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
    bool partialOk;
    do {
        partialOk = true;
        int i;
        for (i = current.size() - 1; i >= 0 && indexesCurrentValues[i] == variables[i]->getDomain()->size() - 1; i--) {
            current.pop_back();
        }
        if (i < 0) {
            current.clear();
            return;
        }
        current.pop_back();
        current.emplace_back(variables[i]->getId(), true,
                             variables[i]->getDomain()->getValues().at(++indexesCurrentValues[i]));
        if (!consistencyChecker->checkPartial(current)) {
            partialOk = false;
            continue;
        }
        variablesFinished[i] = indexesCurrentValues[i] == variables[i]->getDomain()->size() - 1;
        if (i != 0) {
            variablesFinished[i] = variablesFinished[i] && variablesFinished[i - 1];
        }

        for (i++; i < variables.size(); i++) {
            partialOk = false;
            for (int v = 0; v < variables[i]->getDomain()->size(); v++) {
                current.emplace_back(variables[i]->getId(), true, variables[i]->getDomain()->getValues().at(v));
                indexesCurrentValues[i] = v;
                variablesFinished[i] = variables[i]->getDomain()->size() == v + 1 && variablesFinished[i - 1];
                if (consistencyChecker->checkPartial(current)) {
                    partialOk = true;
                    break;
                }
               current.pop_back();
            }
            if (!partialOk) {
                break;
            }
        }
    } while (!partialOk || !consistencyChecker->checkFinal(current));
}

