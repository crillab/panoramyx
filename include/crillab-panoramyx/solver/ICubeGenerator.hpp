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
 * @file ICubeGenerator.hpp
 * @brief Defines a strategy for generating cubes in EPS-based solvers.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_ICUBEGENERATOR_HPP
#define PANORAMYX_ICUBEGENERATOR_HPP

#include <crillab-universe/core/IUniverseSolver.hpp>

#include "../core/IConsistencyChecker.hpp"
#include "../utils/Stream.hpp"

namespace Panoramyx {

    /**
     * The ICubeGenerator defines a strategy for generating cubes in EPS-based solvers.
     */
    class ICubeGenerator {

    public:

        /**
         * Destroys this ICubeGenerator.
         */
        virtual ~ICubeGenerator() = default;

        /**
         * Sets the solver that solves the associated problem.
         * It is used to retrieve information about this problem.
         *
         * @param solver The solver to set.
         */
        virtual void setSolver(Universe::IUniverseSolver *solver) = 0;

        /**
         * Sets the consistencyChecker used to check the consistency of the generated cubes.
         *
         * @param checker The consistency checker to set.
         */
        virtual void setConsistencyChecker(Panoramyx::IConsistencyChecker *checker) = 0;

        /**
         * Generates the cubes representing the assumptions to distribute among the
         * solvers that are run in parallel.
         *
         * @return The stream of the generated cubes.
         */
        virtual Panoramyx::Stream<std::vector<Universe::UniverseAssumption<Universe::BigInteger>>> *generateCubes() = 0;

        virtual void loadInstance(const std::string &filename)=0;
    };

}

#endif
