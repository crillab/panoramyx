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
 * @file EPSSolver.hpp
 * @brief Defines a parallel solver that applies a cube-and-conquer approach to solve an instance.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_EPSSOLVER_HPP
#define PANORAMYX_EPSSOLVER_HPP

#include "AbstractParallelSolver.hpp"
#include "ICubeGenerator.hpp"

namespace Panoramyx {

    /**
     * The EPSSolver defines a parallel solver that applies a cube-and-conquer
     * approach to solve an instance.
     */
    class EPSSolver : public Panoramyx::AbstractParallelSolver {

    private:

        /**
         * The generator for the cubes to assign to the different solvers.
         */
        Panoramyx::ICubeGenerator *generator;

        /**
         * The available solvers, that are waiting for tasks.
         */
        Panoramyx::BlockingDeque<Universe::IUniverseSolver *> availableSolvers;

        /**
         * The semaphore used to wait for tasks to be created.
         */
        std::counting_semaphore<42> cubes;

    public:

        /**
         * Creates a new EPSSolver.
         *
         * @param comm The interface used to communicate with the different solvers.
         * @param generator The generator for the cubes to assign to the different solvers.
         */
        EPSSolver(Panoramyx::INetworkCommunication *comm, Panoramyx::ICubeGenerator *generator);

        /**
         * Destroys this EPSSolver.
         */
        ~EPSSolver() override = default;

        /**
         * Loads the instance to solve.
         *
         * @param filename The name of the file to load the instance from.
         */
        void loadInstance(const std::string &filename) override;

    protected:

        /**
         * Prepares the solver at the given index to use it later on.
         *
         * @param solverIndex The index of the solver that is ready.
         */
        void ready(unsigned solverIndex) override;

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
         * Waits until all cubes have been solved.
         *
         * @param nbCubes The number of cubes to wait for.
         */
        virtual void waitForAllCubes(int nbCubes);

    };

}

#endif
