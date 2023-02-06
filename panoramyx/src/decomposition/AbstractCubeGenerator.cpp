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
 * @file AbstractCubeGenerator.cpp
 * @brief Provides a base implementation for cube generators.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <fstream>
#include "../../include/decomposition/AbstractCubeGenerator.hpp"
#include "../../../libs/autis/libs/universe/universe/include/csp/IUniverseCspSolver.hpp"
#include "../../../libs/autis/autis/include/core/Scanner.hpp"
#include "../../../libs/autis/autis/include/xcsp/AutisXcspParserAdapter.hpp"

using namespace Panoramyx;
using namespace Universe;

void AbstractCubeGenerator::setSolver(IUniverseSolver *s) {
    this->solver = s;
}

void AbstractCubeGenerator::setConsistencyChecker(IConsistencyChecker *checker) {
    this->consistencyChecker = checker;
}

void AbstractCubeGenerator::loadInstance(const std::string &filename) {
    std::ifstream input(filename);
    Autis::Scanner scanner(input);
    auto parser = make_unique<Autis::AutisXCSPParserAdapter>(scanner, dynamic_cast<Universe::IUniverseCspSolver*>(solver));
    parser->parse();
}
