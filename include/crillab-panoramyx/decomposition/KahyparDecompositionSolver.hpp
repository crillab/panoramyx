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
 * @file KahyparDecompositionSolver.hpp
 * @brief Provides an hypergraph decomposition solver based on kahypar.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_KAHYPARDECOMPOSITIONSOLVER_HPP
#define PANORAMYX_KAHYPARDECOMPOSITIONSOLVER_HPP

#include <libkahypar.h>

#include "AbstractHypergraphDecompositionSolver.hpp"

namespace Panoramyx {

    /**
     * The KahyparDecompositionSolver is an hypergraph decomposition solver based on KaHyPar.
     */
    class KahyparDecompositionSolver : public Panoramyx::AbstractHypergraphDecompositionSolver {

    private:

        /**
         * The imbalance parameter for KaHyPar.
         */
        double imbalance;

        /**
         * The number of partitions to compute for the hypergraph.
         */
        int nbBlocks;

        /**
         * The KaHyPar context used to compute the partitions.
         */
        kahypar_context_s* context;

        /**
         * The partition that has been computed by KaHyPar.
         */
        int *partition;

        std::vector<std::vector<int>> constraintPartitionAsVector;
        std::vector<std::string> cutsetVector;
        std::vector<std::vector<std::string>> variablePartitionAsVector;

    public:

        /**
         * Creates a new KahyparDecompositionSolver.
         *
         * @param imbalance The imbalance parameter for KaHyPar.
         * @param nbPartitions The number of partitions to compute for the hypergraph.
         * @param configurationFile The configuration file for KaHyPar.
         */
        KahyparDecompositionSolver(double imbalance, int nbPartitions, const std::string &configurationFile);

        /**
         * Destroys this KahyparDecompositionSolver.
         */
        ~KahyparDecompositionSolver();

        /**
         * Solves the problem associated to this solver.
         *
         * @return The outcome of the search conducted by the solver.
         */
        Universe::UniverseSolverResult solve() override;

        /**
         * Solves the problem stored in the given file.
         * The solver is expected to parse the problem itself.
         *
         * @param filename The name of the file containing the problem to solve.
         *
         * @return The outcome of the search conducted by the solver.
         */
        Universe::UniverseSolverResult solve(const std::string &filename) override;

        /**
         * Solves the problem associated to this solver.
         *
         * @param assumptions The assumptions to consider when solving.
         *
         * @return The outcome of the search conducted by the solver.
         */
        Universe::UniverseSolverResult solve(
                const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumptions) override;

        /**
         * Gives the partition computed by this solver.
         * The partition is represented as a vector of vector of constraint identifiers, where the i-th vector
         * contains the constraint identifiers of the i-th partition.
         *
         * @return The computed partition.
         */
        std::vector<std::vector<int>> getPartition() override;

        /**
         * Gives the partition computed by this solver.
         * The partition is represented as a vector of vector of variable identifiers, where the i-th vector
         * contains the variable identifiers of the i-th partition.
         *
         * @return The computed partition.
         */
        virtual std::vector<std::vector<std::string>> getVariablePartition() override;

        /**
         * Gives the cutset of the problem to solve.
         *
         * @return The variables belonging to the cutset.
         */
        std::vector<std::string> cutset() override;

    };

}

#endif
