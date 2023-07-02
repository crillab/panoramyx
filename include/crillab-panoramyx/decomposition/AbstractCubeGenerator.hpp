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
 * @file AbstractCubeGenerator.hpp
 * @brief Provides a base implementation for cube generators.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_ABSTRACTCUBEGENERATOR_HPP
#define PANORAMYX_ABSTRACTCUBEGENERATOR_HPP

#include "../core/IConsistencyChecker.hpp"
#include "../solver/ICubeGenerator.hpp"

namespace Panoramyx {

    /**
     * The AbstractCubeGenerator provides a base implementation for cube generators.
     */
    class AbstractCubeGenerator : public Panoramyx::ICubeGenerator {

    protected:

        /**
         * The solver providing the metadata about the problem to solve.
         */
        Universe::IUniverseSolver *solver;

        /**
         * The consistency checker used to check whether the generated cube are consistent.
         */
        Panoramyx::IConsistencyChecker *consistencyChecker;

        /**
         * The maximum number of cubes to generate.
         */
        int nbCubesMax;
    public:

        explicit AbstractCubeGenerator(int nbCubesMax);

        /**
         * Destroys this AbstractCubeGenerator.
         */
        ~AbstractCubeGenerator() override = default;

        /**
         * Sets the solver providing the metadata about the problem to solve.
         *
         * @param solver The solver to set.
         */
        void setSolver(Universe::IUniverseSolver *solver) override;

        /**
         * Sets the consistency checker used to check whether the generated cube are consistent.
         *
         * @param checker The consistency checker to set.
         */
        void setConsistencyChecker(Panoramyx::IConsistencyChecker *checker) override;

        void loadInstance(const std::string &filename) override;
    };

}

#endif
