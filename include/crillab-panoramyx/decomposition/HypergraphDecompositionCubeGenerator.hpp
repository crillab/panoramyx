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
 * @file HypergraphDecompositionCubeGenerator.hpp
 * @brief Generates cubes following the decomposition of the dual hypergraph of the problem.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_HYPERGRAPHDECOMPOSITIONCUBEGENERATOR_HPP
#define PANORAMYX_HYPERGRAPHDECOMPOSITIONCUBEGENERATOR_HPP

#include "AbstractCubeGenerator.hpp"
#include "IHypergraphDecompositionSolver.hpp"

namespace Panoramyx {

    /**
     * The HypergraphDecompositionCubeGenerator generates cubes following
     * the decomposition of the dual hypergraph of the problem.
     */
    class HypergraphDecompositionCubeGenerator : public Panoramyx::AbstractCubeGenerator {

    private:

        /**
         * The decomposition solver used to decompose the dual hypergraph of the problem.
         */
        Panoramyx::IHypergraphDecompositionSolver *decompositionSolver;

    public:

        /**
         * Creates a new HypergraphDecompositionCubeGenerator.
         *
         * @param nbCubesMax The maximum number of cubes to generate.
         * @param decompositionSolver The decomposition solver used to decompose the dual hypergraph of the problem.
         */
        explicit HypergraphDecompositionCubeGenerator(
                int nbCubesMax, Panoramyx::IHypergraphDecompositionSolver *decompositionSolver);

        /**
         * Loads the instance to generate cubes from.
         *
         * @param filename The file to load the instance from.
         */
        void loadInstance(const std::string &filename) override;

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
