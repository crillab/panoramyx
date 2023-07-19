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
 * @file LinearRangeIterator.cpp
 * @brief Defines an iterator over an interval that applies a linear step between elements.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <crillab-panoramyx/optim/decomposition/LinearRangeIterator.hpp>

using namespace std;

using namespace Panoramyx;
using namespace Universe;

LinearRangeIterator::LinearRangeIterator(BigInteger min, BigInteger max, int numberOfSteps) :
        current(min),
        maximum(max),
        step(numberOfSteps == 0 ? (max - min) : std::max((max - (min + 1)) / numberOfSteps, (BigInteger) 1)) {
    // Nothing to do: everything is already initialized.
}

bool LinearRangeIterator::hasNext() const {
    return current < maximum;
}

BigInteger LinearRangeIterator::next() {
    BigInteger n = current;
    current += step;
    return n;
}
