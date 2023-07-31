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
 * @file IHypergraphDecompositionSolver.hpp
 * @brief Provides a strategy for identifying the cutset of the dual hypergraph of a problem.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_IHYPERGRAPHDECOMPOSITIONSOLVER_HPP
#define PANORAMYX_IHYPERGRAPHDECOMPOSITIONSOLVER_HPP

#include <crillab-universe/csp/IUniverseCspSolver.hpp>

namespace Panoramyx {

    /**
     * The IHypergraphDecompositionSolver defines a strategy for identifying the cutset
     * of the dual hypergraph of a problem.
     */
    class IHypergraphDecompositionSolver : public Universe::IUniverseCspSolver {

    public:

        /**
         * Destroys this IHypergraphDecompositionSolver.
         */
        virtual ~IHypergraphDecompositionSolver() = default;

        /**
         * Gives the partition computed by this solver.
         * The partition is represented as a vector of vector of constraint identifiers, where the i-th vector
         * contains the constraint identifiers of the i-th partition.
         *
         * @return The computed partition.
         */
        virtual std::vector<std::vector<int>> getPartition() = 0;

        /**
         * Gives the cutset of the problem to solve.
         *
         * @return The variables belonging to the cutset.
         */
        virtual std::vector<std::string> cutset() = 0;

    };

}

#endif
