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
 * @file LogarithmicRangeIterator.hpp
 * @brief Defines an iterator over an interval that applies a logarithmic step between elements.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_LOGARITHMICRANGEITERATOR_HPP
#define PANORAMYX_LOGARITHMICRANGEITERATOR_HPP

#include <crillab-universe/core/UniverseType.hpp>

#include "../../utils/Stream.hpp"

namespace Panoramyx {

    /**
     * The LogarithmicRangeIterator defines an iterator over an interval that applies
     * a logarithmic step between elements.
     */
    class LogarithmicRangeIterator : public Panoramyx::Stream<Universe::BigInteger> {

    private:

        /**
         * The minimum value of the interval on which the iteration is performed
         * (inclusive).
         */
        long double minimum;

        /**
         * The current value in the interval on which the iteration is performed.
         */
        long double currentValue;

        /**
         * The maximum value of the interval on which the iteration is performed
         * (exclusive).
         */
        long double maximum;

        /**
         * The number of steps for the iteration.
         */
        int numberOfSteps;

        /**
         * Whether the step is increasing.
         */
        bool increasing;

        /**
         * The scale to apply so as to iterate logarithmically over the values of the
         * interval.
         */
        long double scale;

        /**
         * The index of the current iteration step.
         */
        int currentStep;

    public:

        /**
         * Creates a new LogarithmicRangeIterator.
         *
         * @param min The minimum value of the interval on which the iteration is performed (inclusive).
         * @param max The maximum value of the interval on which the iteration is performed (exclusive).
         * @param numberOfSteps The number of steps for the iteration.
         * @param increasing Whether the step is increasing.
         */
        LogarithmicRangeIterator(Universe::BigInteger min, Universe::BigInteger max,
                                 int numberOfSteps, bool increasing = true);

        /**
         * Destroys this LogarithmicRangeIterator.
         */
        ~LogarithmicRangeIterator() override = default;

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

    protected:

        /**
         * Computes the next value of the iteration.
         *
         * @param step The index of the step.
         */
        virtual long double computeNextValue(int step);

    };

}

#endif
