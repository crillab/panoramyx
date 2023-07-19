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
 * @file CartesianProductIterativeRefinementCubeGenerator.cpp
 * @brief Generates cubes following the Cartesian product iterative refinement strategy.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <crillab-panoramyx/decomposition/CartesianProductIterativeRefinementCubeGenerator.hpp>

using namespace std;

using namespace Panoramyx;
using namespace Universe;

CartesianProductIterativeRefinementCubeGenerator::CartesianProductIterativeRefinementCubeGenerator(int nbCubesMax)
        : AbstractProblemUnderAssumptionCubeGenerator(nbCubesMax) {
    // Nothing to do: everything is already initialized.
}

long CartesianProductIterativeRefinementCubeGenerator::computeScore(const vector<UniverseAssumption<BigInteger>> &assumpts) {
    solver->reset();

    // Checking whether the assumptions are arc-consistent.
    const UniverseSolverResult result = solver->solve(assumpts);
    if (result == Universe::UniverseSolverResult::UNSATISFIABLE) {
        return -1;
    }

    // Computing the size of the cartesian product of the domains of the current variables.
    unsigned long cartesianProduct = 1;
    for (const auto &variable : solver->getVariablesMapping()) {
        cartesianProduct *= variable.second->getDomain()->currentSize();
    }

    return cartesianProduct;
}
