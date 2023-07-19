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
 * @file CartesianProductIterativeRefinementCubeGenerator.hpp
 * @brief Generates cubes following the Cartesian product iterative refinement strategy.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_CARTESIANPRODUCTITERATIVEREFINEMENTCUBEGENERATOR_HPP
#define PANORAMYX_CARTESIANPRODUCTITERATIVEREFINEMENTCUBEGENERATOR_HPP

#include "AbstractProblemUnderAssumptionCubeGenerator.hpp"

namespace Panoramyx {

    /**
     * The CartesianProductIterativeRefinementCubeGenerator generates cubes following the Cartesian product
     * iterative refinement strategy.
     */
    class CartesianProductIterativeRefinementCubeGenerator : public Panoramyx::AbstractProblemUnderAssumptionCubeGenerator {

    public:

        /**
         * Creates a new CartesianProductIterativeRefinementCubeGenerator.
         *
         * @param nbCubesMax The maximum number of cubes to generate.
         */
        CartesianProductIterativeRefinementCubeGenerator(int nbCubesMax);

        /**
         * Destroys this CartesianProductIterativeRefinementCubeGenerator.
         */
        ~CartesianProductIterativeRefinementCubeGenerator() override = default;

    protected:

        /**
         * Computes the score of a conjunction of assumptions.
         *
         * @param assumpts The assumptions to compute the score of.
         *
         * @return The score of the assumptions.
         */
        long computeScore(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) override;

    };

}

#endif
