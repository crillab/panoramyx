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
 * @file LogarithmicRangeIterator.cpp
 * @brief Defines an iterator over a range that applies a logarithmic step between elements.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <cmath>

#include <crillab-panoramyx/optim/decomposition/LogarithmicRangeIterator.hpp>

using namespace std;

using namespace Panoramyx;
using namespace Universe;

LogarithmicRangeIterator::LogarithmicRangeIterator(BigInteger min, BigInteger max,
                                                   int numberOfSteps, bool increasing) :
        minimum(min),
        currentValue(min),
        maximum(max),
        numberOfSteps(numberOfSteps + 1),
        increasing(increasing),
        scale((max - min) / log(2 + numberOfSteps)),
        currentStep(0) {
    // Nothing to do: everything is already initialized.
}

bool LogarithmicRangeIterator::hasNext() const {
    return currentStep < numberOfSteps;
}

BigInteger LogarithmicRangeIterator::next() {
    auto next = currentValue;

    // Moving to the next value.
    auto nextValue = computeNextValue(currentStep);
    currentStep++;

    // Ensuring that the last value is the maximum.
    if (currentStep == numberOfSteps) {
        return (BigInteger) maximum;
    }

    // Ensuring that the next value will be different.
    if (nextValue == currentValue) {
        currentValue += 1;
    } else {
        currentValue = nextValue;
    }

    return (BigInteger) next;
}

long double LogarithmicRangeIterator::computeNextValue(int step) {
    if (increasing) {
        return maximum - (log(numberOfSteps - step) * scale);
    }

    return minimum + (log(2 + step) * scale);
}
