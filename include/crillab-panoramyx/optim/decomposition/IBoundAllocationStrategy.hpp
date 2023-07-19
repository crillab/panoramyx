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
 * @file IBoundAllocationStrategy.hpp
 * @brief Defines a strategy for allocating bounds for optimization problems.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_IBOUNDALLOCATIONSTRATEGY_HPP
#define PANORAMYX_IBOUNDALLOCATIONSTRATEGY_HPP

#include <vector>

#include <crillab-universe/core/UniverseType.hpp>

namespace Panoramyx {

    /**
     * The IBoundAllocationStrategy defines a strategy for allocating bounds for optimization problems.
     */
    class IBoundAllocationStrategy {

    public:

        /**
         * Sets whether the problem to solve is a minimization problem.
         *
         * @param min Whether the problem is a minimization problem.
         */
        virtual void setMinimization(bool min) = 0;

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
        virtual std::vector<Universe::BigInteger> computeBoundAllocation(
                const std::vector<Universe::BigInteger> &currentBounds,
                const Universe::BigInteger &currentMin,
                const Universe::BigInteger &currentMax) = 0;

    };

}

#endif
