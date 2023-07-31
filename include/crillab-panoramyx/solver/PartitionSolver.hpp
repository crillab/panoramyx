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
 * @file PartitionSolver.hpp
 * @brief Defines a parallel solver that solves in parallel independent sub-problems of the original problem.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_PARTITIONSOLVER_HPP
#define PANORAMYX_PARTITIONSOLVER_HPP

#include "../decomposition/IHypergraphDecompositionSolver.hpp"

#include "AbstractParallelSolver.hpp"
#include "ICubeGenerator.hpp"

namespace Panoramyx {

    /**
     * The PartitionSolver defines a parallel solver that solves in parallel independent
     * sub-problems of the original problem.
     */
    class PartitionSolver : public Panoramyx::AbstractParallelSolver {

    private:

        /**
         * The decomposition solver used to compute a partition of the input problem.
         */
        Panoramyx::IHypergraphDecompositionSolver *decompositionSolver;

        /**
         * The cube generator based on the decomposition solver above.
         */
        Panoramyx::ICubeGenerator *generator;

        /**
         * The vector associating to each partition the constraints it contains.
         */
        std::vector<std::vector<int>> constraintsInPartitions;

        /**
         * The semaphore allowing to wait for the answer of all the solvers running on the sub-problems.
         */
        std::counting_semaphore<42> partitions;

        /**
         * The number of solutions found so far.
         */
        int numberOfSolutions;

    public:

        /**
         * Creates a new PartitionSolver.
         *
         * @param comm The interface used to communicate with the different solvers.
         * @param decompositionSolver The decomposition solver used to compute a partition of the input problem.
         */
        PartitionSolver(Panoramyx::INetworkCommunication *comm,
                        Panoramyx::IHypergraphDecompositionSolver *decompositionSolver);

        /**
         * Destroys this PartitionSolver.
         */
        ~PartitionSolver() override;

        /**
         * Loads the instance to solve.
         *
         * @param filename The name of the file to load the instance from.
         */
        void loadInstance(const std::string &filename) override;

    protected:

        /**
         * Applies some initialization before actually starting the search.
         */
        void beforeSearch() override;

        /**
         * Applies some initialization to a particular solver before actually starting the search.
         *
         * @param solverIndex The index of the solver to initialize.
         */
        void beforeSearch(unsigned int solverIndex) override;

        /**
         * Actually starts the search performed by the different solvers.
         */
        void startSearch() override;

        /**
         * Actually starts the search performed by the different solvers.
         *
         * @param assumpts The assumptions to apply when solving.
         */
        void startSearch(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) override;

        /**
         * Updates the search when a solver has found a solution of its problem.
         *
         * @param solverIndex The index of the solver that found a solution.
         */
        void onSatisfiableFound(unsigned solverIndex) override;

        /**
         * Updates the search when a solver proved the unsatisfiability of its problem.
         *
         * @param solverIndex The index of the solver that returned UNSATISFIABLE.
         */
        void onUnsatisfiableFound(unsigned solverIndex) override;

        /**
         * Updates the search when a solver did not find any solution.
         *
         * @param solverIndex The index of the solver that returned UNKNOWN.
         */
        void onUnknown(unsigned solverIndex) override;

    };

}

#endif
