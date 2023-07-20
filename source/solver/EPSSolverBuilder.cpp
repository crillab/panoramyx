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
 * @file EPSSolverBuilder.cpp
 * @brief The builder for EPS solvers.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <crillab-panoramyx/solver/EPSSolverBuilder.hpp>
#include <crillab-panoramyx/solver/EPSSolver.hpp>

using namespace Panoramyx;

EPSSolverBuilder *EPSSolverBuilder::withCubeGenerator(ICubeGenerator *cubeGenerator) {
    this->cubeGenerator = cubeGenerator;
    return this;
}

AbstractParallelSolver *EPSSolverBuilder::build() {
    return new EPSSolver(this->networkCommunication, this->cubeGenerator);
}
