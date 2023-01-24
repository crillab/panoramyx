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
 * @file PartialConsistencyChecker.cpp
 * @brief Provides a consistency checker that checks cube consistency each time an assumption is added.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include "../../include/core/PartialConsistencyChecker.hpp"

using namespace std;

using namespace Panoramyx;
using namespace Universe;

PartialConsistencyChecker::PartialConsistencyChecker(IUniverseSolver *solver) :
        solver(solver) {
    // Nothing to do: everything is already initialized.
}

bool PartialConsistencyChecker::checkPartial(const vector<UniverseAssumption<BigInteger>> &cube) {
    auto result = solver->solve(cube);
    solver->reset();
    return result != Universe::UniverseSolverResult::UNSATISFIABLE;
}

bool PartialConsistencyChecker::checkFinal(const std::vector<UniverseAssumption<BigInteger>> &) {
    // Final checks are not needed.
    return true;
}
