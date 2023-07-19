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
 * @file AggressiveRangeBasedAllocationStrategy.hpp
 * @brief Defines an aggressive bound allocation that divides a range of bounds into several sub-intervals.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_AGGRESSIVERANGEBASEDALLOCATIONSTRATEGY_HPP
#define PANORAMYX_AGGRESSIVERANGEBASEDALLOCATIONSTRATEGY_HPP

#include "IBoundAllocationStrategy.hpp"

#include "../../utils/Stream.hpp"

namespace Panoramyx {

    /**
     * The AggressiveRangeBasedAllocationStrategy defines a bound allocation that divides a range of bounds
     * into several sub-intervals and aggressively assigns these bounds to the solvers.
     */
    class AggressiveRangeBasedAllocationStrategy: public IBoundAllocationStrategy {

    private:

        /**
         * Whether the problem to solve is a minimization problem.
         */
        bool minimization;

        /**
         * The factory allowing to instantiate the iterator over the range.
         */
        std::function<Panoramyx::Stream<Universe::BigInteger> *(Universe::BigInteger, Universe::BigInteger, int)> rangeIterator;

    public:

        /**
         * Creates a new AggressiveRangeBasedAllocationStrategy.
         *
         * @param rangeIterator The factory allowing to instantiate the iterator over the range.
         */
        explicit AggressiveRangeBasedAllocationStrategy(
                std::function<Stream<Universe::BigInteger> *(Universe::BigInteger, Universe::BigInteger, int)> rangeIterator);

        /**
         * Destroys this AggressiveRangeBasedAllocationStrategy.
         */
        ~AggressiveRangeBasedAllocationStrategy() = default;

        /**
         * Sets whether the problem to solve is a minimization problem.
         *
         * @param min Whether the problem is a minimization problem.
         */
        void setMinimization(bool min) override;

        /**
         * Computes the bound allocation to apply.
         * Identical bounds means the previous bound should not be updated.
         * Bounds equal to the current maximum should be ignored (there is not enough bounds for all solvers).
         *
         * @param currentBounds The current bounds that are assigned to the solvers.
         * @param currentMin The current minimum bound found by one of the solvers.
         * @param currentMax The current maximum bound found by one of the solvers.
         *
         * @return The new vector of bounds.
         */
        std::vector<Universe::BigInteger> computeBoundAllocation(const std::vector<Universe::BigInteger> &currentBounds,
                                                                 const Universe::BigInteger &currentMin,
                                                                 const Universe::BigInteger &currentMax) override;

    };

}

#endif
