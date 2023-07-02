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
 * @file PortfolioSolver.hpp
 * @brief Defines a parallel solver that runs different solvers on the same instance.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_PORTFOLIOSOLVER_HPP
#define PANORAMYX_PORTFOLIOSOLVER_HPP

#include "AbstractParallelSolver.hpp"

namespace Panoramyx {

    /**
     * The PortfolioSolver defines a parallel solver that runs different
     * solvers on the same instance.
     */
    class PortfolioSolver : public Panoramyx::AbstractParallelSolver {


    public:

        /**
         * Creates a new PortfolioSolver.
         *
         * @param comm The interface used to communicate with the different solvers.
         * @param allocationStrategy The allocation strategy used to allocate bounds to the different solvers.
         */
        explicit PortfolioSolver(
                Panoramyx::INetworkCommunication *comm, Panoramyx::IAllocationStrategy *allocationStrategy = nullptr);

        /**
         * Destroys this PortfolioSolver.
         */
        ~PortfolioSolver() override = default;

    protected:

        /**
         * Applies some initialization before actually starting the search.
         */
        void beforeSearch() override;

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
         * Launches the solving process of the i-th solver.
         *
         * @param i The index of the solver to launch.
         */
        virtual void solve(unsigned i);

        /**
         * Launches the solving process of the i-th solver.
         *
         * @param i The index of the solver to launch.
         * @param assumpts The assumptions to apply when solving the instance.
         */
        virtual void solve(unsigned i, const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts);

        /**
         * Reconsiders the current task allocation when a new bound is found.
         *
         * @param bound The new bound that has been found.
         */
        void onNewBoundFound(const Universe::BigInteger &bound, unsigned int i) override;

        /**
         * Updates the search when a solver proved the unsatisfiability of its problem.
         *
         * @param solverIndex The index of the solver that returned UNSATISFIABLE.
         */
        void onUnsatisfiableFound(unsigned solverIndex) override;


        void updateBounds();
    };

}

#endif
