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
 * @file RangeBasedAllocationStrategy.hpp
 * @brief Defines an aggressive bound allocation that divides a range of bounds into several sub-intervals.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <utility>

#include <loguru.hpp>

#include <crillab-panoramyx/optim/decomposition/AggressiveRangeBasedAllocationStrategy.hpp>

using namespace std;

using namespace Panoramyx;
using namespace Universe;

AggressiveRangeBasedAllocationStrategy::AggressiveRangeBasedAllocationStrategy(
        function<Stream<BigInteger> *(BigInteger, BigInteger, int)> rangeIterator) :
        rangeIterator(std::move(rangeIterator)),
        minimization(true) {
    // Nothing to do: everything is already initialized.
}

void AggressiveRangeBasedAllocationStrategy::setMinimization(bool min) {
    minimization = min;
}

vector<BigInteger> AggressiveRangeBasedAllocationStrategy::computeBoundAllocation(
        const vector<BigInteger> &currentBounds, const BigInteger &currentMin, const BigInteger &currentMax) {
    // All bounds must be reassigned.
    vector<Universe::BigInteger> newBounds;
    for (auto bound : *rangeIterator(currentMin, currentMax, currentBounds.size() - 1)) {
        newBounds.push_back(bound);
        LOG_F(INFO, "allocating completely new bound %lld", (long long) bound);
    }

    // Filling the last bounds with the maximum (these bounds will be ignored).
    while (newBounds.size() < currentBounds.size()) {
        newBounds.push_back(newBounds.back());
        LOG_F(INFO, "duplicating %lld to fill the bounds", (long long) newBounds.back());
    }

    return newBounds;
}
