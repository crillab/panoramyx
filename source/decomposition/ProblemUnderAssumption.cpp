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
 * @file ProblemUnderAssumption.cpp
 * @brief Stores the information regarding a assumptions under which a problem is to be considered.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <crillab-panoramyx/decomposition/ProblemUnderAssumption.hpp>

using namespace std;

using namespace Panoramyx;
using namespace Universe;

ProblemUnderAssumption::ProblemUnderAssumption(const vector<UniverseAssumption<BigInteger>> &assumptions, long score) :
        assumptions(assumptions),
        score(score) {
    // Nothing to do: everything is already initialized.
}

const vector<UniverseAssumption<BigInteger>> &ProblemUnderAssumption::getAssumptions() const {
    return assumptions;
}

long ProblemUnderAssumption::getScore() const {
    return score;
}

bool ProblemUnderAssumption::operator<(const ProblemUnderAssumption &rhs) const {
    return score < rhs.score;
}

bool ProblemUnderAssumption::operator>(const ProblemUnderAssumption &rhs) const {
    return rhs < *this;
}

bool ProblemUnderAssumption::operator<=(const ProblemUnderAssumption &rhs) const {
    return !(rhs < *this);
}

bool ProblemUnderAssumption::operator>=(const ProblemUnderAssumption &rhs) const {
    return !(*this < rhs);
}
