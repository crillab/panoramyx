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
 * @file LexicographicIntervalCubeGenerator.hpp
 * @brief Generates cubes of intervals following a lexicographic order.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_LEXICOGRAPHICINTERVALCUBEGENERATOR_HPP
#define PANORAMYX_LEXICOGRAPHICINTERVALCUBEGENERATOR_HPP

#include "AbstractCubeGenerator.hpp"

namespace Panoramyx {

    /**
     * The LexicographicIntervalCubeGenerator generates cubes following a lexicographic order
     * on the variables and the values of their domains, decomposed into intervals.
     */
    class LexicographicIntervalCubeGenerator : public Panoramyx::AbstractCubeGenerator {

    private:

        /**
         * The number of intervals in which to decompose the domain of variables.
         */
        int nbIntervals;

    public:

        /**
         * Creates a new LexicographicIntervalCubeGenerator.
         *
         * @param nbCubesMax The maximum number of cubes to generate.
         * @param nbIntervals The number of intervals in which to decompose the domain of variables.
         */
        explicit LexicographicIntervalCubeGenerator(int nbCubesMax, int nbIntervals);

        /**
         * Generates the cubes representing the assumptions to distribute among the
         * solvers that are run in parallel.
         *
         * @return The stream of the generated cubes.
         */
        Panoramyx::Stream<std::vector<Universe::UniverseAssumption<Universe::BigInteger>>> *generateCubes() override;

    };

}

#endif
