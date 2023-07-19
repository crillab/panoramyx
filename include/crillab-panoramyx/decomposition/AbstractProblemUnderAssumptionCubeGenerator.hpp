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
 * @file AbstractProblemUnderAssumptionCubeGenerator.hpp
 * @brief Defines the common behavior for cube generators based on ProblemUnderAssumption instances.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_ABSTRACTPROBLEMUNDERASSUMPTIONCUBEGENERATOR_HPP
#define PANORAMYX_ABSTRACTPROBLEMUNDERASSUMPTIONCUBEGENERATOR_HPP

#include <queue>

#include "AbstractCubeGenerator.hpp"
#include "ProblemUnderAssumption.hpp"

namespace Panoramyx {

    /**
     * The AbstractProblemUnderAssumptionCubeGenerator defines the common behavior for cube generators
     * based on ProblemUnderAssumption instances.
     */
    class AbstractProblemUnderAssumptionCubeGenerator: public Panoramyx::AbstractCubeGenerator {

    private:

        /**
         * The queue of generated problems.
         */
        std::priority_queue<Panoramyx::ProblemUnderAssumption> priorityQueue;

    public:

        /**
         * Creates a new AbstractProblemUnderAssumptionCubeGenerator.
         *
         * @param nbCubesMax The maximum number of cubes to generate.
         */
        explicit AbstractProblemUnderAssumptionCubeGenerator(int nbCubesMax);

        /**
         * Generates the cubes representing the assumptions to distribute among the
         * solvers that are run in parallel.
         *
         * @return The stream of the generated cubes.
         */
        Panoramyx::Stream<std::vector<Universe::UniverseAssumption<Universe::BigInteger>>> *generateCubes() override;

    protected:

        /**
         * Computes the score for a conjunction of assumptions.
         *
         * @param assumptions The assumptions to compute the score of.
         *
         * @return The score for the assumptions.
         */
        virtual long computeScore(
                const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumptions) = 0;

    };

}

#endif
