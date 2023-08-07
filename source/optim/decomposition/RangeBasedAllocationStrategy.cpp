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
 * @brief Defines a bound allocation that divides a range of bounds into several sub-intervals.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <utility>

#include <loguru.hpp>

#include <crillab-panoramyx/optim/decomposition/RangeBasedAllocationStrategy.hpp>

using namespace std;

using namespace Panoramyx;
using namespace Universe;

RangeBasedAllocationStrategy::RangeBasedAllocationStrategy(
        function<Stream<BigInteger> *(BigInteger, BigInteger, int)> rangeIterator) :
        rangeIterator(std::move(rangeIterator)),
        minimization(true) {
    // Nothing to do: everything is already initialized.
}

void RangeBasedAllocationStrategy::setMinimization(bool min) {
    minimization = min;
}

vector<BigInteger> RangeBasedAllocationStrategy::computeBoundAllocation(
        const vector<BigInteger> &currentBounds, const BigInteger &currentMin, const BigInteger &currentMax) {
    // FIXME This method is too long!
    // Looking for solvers that are out of bounds.
    int indexLower = -1;
    int indexUpper = 0;
    for (int i = 0; i < currentBounds.size() - 1; i++) {
        if (currentBounds[i + 1] >= currentMax) {
            // All remaining solvers are above the maximum.
            indexUpper = i;
            break;

        } else if ((currentBounds[i] > currentMin) && (indexLower < 0)) {
            // This is the first solver above the minimum.
            indexLower = i;
        }
    }

    // Computing the new bounds.
    vector<BigInteger> newBounds;
    if (((currentBounds[indexLower] - currentMin) < indexLower) ||
        ((currentMax - currentBounds[indexUpper]) < (currentBounds.size() - indexUpper - 1))) {
        // There is not enough bounds: all bounds must be recomputed.
        for (auto bound : *rangeIterator(currentMin, currentMax, currentBounds.size() - 1)) {
            newBounds.push_back(bound);
            DLOG_F(INFO, "allocating completely new bound %lld", (long long) bound);
        }
        return newBounds;
    }

    // Computing the bounds for solvers that are currently below the minimum.
    for (auto bound : *rangeIterator(currentMin, currentBounds[indexLower], indexLower)) {
        newBounds.push_back(bound);
        DLOG_F(INFO, "allocating completely new bound %lld", (long long) bound);
    }

    // Reusing the bounds of solvers that are within the current interval
    for (int i = indexLower + 1; i < indexUpper; i++) {
        newBounds.push_back(currentBounds[i]);
        DLOG_F(INFO, "allocating the same bound %lld", (long long) currentBounds[i]);
    }

    // Computing the bounds for solvers that are currently above the maximum.
    for (auto bound : *rangeIterator(currentBounds[indexUpper], currentMax, currentBounds.size() - indexUpper - 1)) {
        newBounds.push_back(bound);
        DLOG_F(INFO, "allocating completely new bound %lld", (long long) bound);
    }

    // Filling the last bounds with the maximum (these bounds will be ignored).
    while (newBounds.size() < currentBounds.size()) {
        newBounds.push_back(newBounds.back());
        DLOG_F(INFO, "duplicating %lld to fill the bounds", (long long) newBounds.back());
    }

    return newBounds;
}
