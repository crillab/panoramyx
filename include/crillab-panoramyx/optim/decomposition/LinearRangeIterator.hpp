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
 * @file LinearRangeIterator.hpp
 * @brief Defines an iterator over an interval that applies a linear step between elements.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_LINEARRANGEITERATOR_HPP
#define PANORAMYX_LINEARRANGEITERATOR_HPP

#include <crillab-universe/core/UniverseType.hpp>

#include "../../utils/Stream.hpp"

namespace Panoramyx {

    /**
     * The LinearRangeIterator defines an iterator over an interval that applies
     * a linear step between elements.
     */
    class LinearRangeIterator : public Panoramyx::Stream<Universe::BigInteger> {

    private:

        /**
         * The current value in the interval on which the iteration is performed.
         */
        Universe::BigInteger current;

        /**
         * The maximum value of the interval on which the iteration is performed
         * (exclusive).
         */
        Universe::BigInteger maximum;

        /**
         * The size of the step applied at each iteration.
         */
        Universe::BigInteger step;

    public:

        /**
         * Creates a new LinearRangeIterator.
         *
         * @param min The minimum value of the interval on which the iteration is performed (inclusive).
         * @param max The maximum value of the interval on which the iteration is performed (exclusive).
         * @param numberOfSteps The number of steps for the iteration.
         */
        LinearRangeIterator(Universe::BigInteger min, Universe::BigInteger max, int numberOfSteps);

        /**
         * Destroys this LinearRangeIterator.
         */
        ~LinearRangeIterator() override = default;

        /**
         * Checks whether there is another element in this stream.
         *
         * @return Whether there is another element in this stream.
         */
        [[nodiscard]] bool hasNext() const override;

        /**
         * Gives the next element in this stream.
         * The behavior of this method is undefined if hasNext() returned false.
         *
         * @return The next element.
         */
        Universe::BigInteger next() override;

    };

}

#endif
