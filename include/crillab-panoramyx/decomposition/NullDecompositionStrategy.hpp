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
 * @file NullDecompositionStrategy.hpp
 * @brief Provides a null implementation of a decomposition strategy.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_NULLDECOMPOSITIONSTRATEGY_HPP
#define PANORAMYX_NULLDECOMPOSITIONSTRATEGY_HPP

#include "../core/IDecompositionStrategy.hpp"

namespace Panoramyx {

    /**
     * The NullDecompositionStrategy provides a null implementation of a decomposition
     * strategy, which does not decompose the input problem.
     */
    class NullDecompositionStrategy : public Panoramyx::IDecompositionStrategy {

    public:

        /**
         * Destroys this NullDecompositionStrategy.
         */
        ~NullDecompositionStrategy() override = default;

        /**
         * Decomposes the given problem into several sub-problems.
         *
         * @param problem The problem to decompose.
         *
         * @return The stream of the sub-problems that have been generated.
         */
        Panoramyx::Stream<Universe::IUniverseProblem *> *decompose(Universe::IUniverseProblem *problem) override;

    };

}

#endif
