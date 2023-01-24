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
 * @file ISolverConfigurationStrategy.hpp
 * @brief Defines a strategy for decomposing problems.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_IDECOMPOSITIONSTRATEGY_HPP
#define PANORAMYX_IDECOMPOSITIONSTRATEGY_HPP

#include <vector>

#include "../../../libs/autis/libs/universe/universe/include/core/problem/IUniverseProblem.hpp"

#include "../utils/VectorStream.hpp"

namespace Panoramyx {

    /**
     * The IDecompositionStrategy defines a strategy for decomposing problems.
     */
    class IDecompositionStrategy {

    public:

        /**
         * Destroys this IDecompositionStrategy.
         */
        virtual ~IDecompositionStrategy() = default;

        /**
         * Decomposes the given problem into several sub-problems.
         *
         * @param problem The problem to decompose.
         *
         * @return The stream of the sub-problems that have been generated.
         */
        virtual Panoramyx::Stream<Universe::IUniverseProblem *> *decompose(Universe::IUniverseProblem *problem) = 0;

    };

}

#endif
