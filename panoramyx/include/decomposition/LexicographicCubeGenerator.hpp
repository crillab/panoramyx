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
 * @file LexicographicCubeGenerator.hpp
 * @brief Generates cubes following a lexicographic order.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_LEXICOGRAPHICCUBEGENERATOR_HPP
#define PANORAMYX_LEXICOGRAPHICCUBEGENERATOR_HPP

#include "AbstractCubeGenerator.hpp"

namespace Panoramyx {

    /**
     * The LexicographicCubeGenerator generates cubes following a lexicographic order
     * on the variables and the values of their domains
     */
    class LexicographicCubeGenerator : public Panoramyx::AbstractCubeGenerator {

    private:

        /**
         * The maximum number of cubes to generate.
         */
        int nbCubesMax;

    public:

        /**
         * Creates a new LexicographicCubeGenerator.
         *
         * @param nbCubesMax The maximum number of cubes to generate.
         */
        explicit LexicographicCubeGenerator(int nbCubesMax);

        /**
         * Generates the cube representing the assumptions to distribute among the
         * solvers that are run in parallel.
         *
         * @return The stream of the generated cubes.
         */
        Panoramyx::Stream<std::vector<Universe::UniverseAssumption<Universe::BigInteger>>> *generateCubes() override;

    };

}

#endif
