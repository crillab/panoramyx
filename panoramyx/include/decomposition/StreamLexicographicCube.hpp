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
 * @file StreamLexicographicCube.hpp
 * @brief Provides a stream of lexicographic cubes.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_STREAMLEXICOGRAPHICCUBE_HPP
#define PANORAMYX_STREAMLEXICOGRAPHICCUBE_HPP

#include <map>

#include "../../../libs/autis/libs/universe/universe/include/core/UniverseAssumption.hpp"
#include "../../../libs/autis/libs/universe/universe/include/core/UniverseType.hpp"
#include "../../../libs/autis/libs/universe/universe/include/core/problem/IUniverseVariable.hpp"

#include "../core/IConsistencyChecker.hpp"
#include "../utils/Stream.hpp"

namespace Panoramyx {

    /**
     * The StreamLexicographicCube provides a stream of lexicographic cubes, that are
     * generated on the fly as the stream is being iterated over.
     */
    class StreamLexicographicCube : public Panoramyx::Stream<std::vector<Universe::UniverseAssumption<Universe::BigInteger>>> {

    private:

        /**
         * The mapping of the variables of the problem to solve.
         */
        const std::map<std::string, Universe::IUniverseVariable *> &mapping;

        /**
         * The consistency checker used to check the consistency of the cubes.
         */
        Panoramyx::IConsistencyChecker *consistencyChecker;

        /**
         * The maximum number of cubes to generate.
         */
        size_t nbCubeMax;

        /**
         * The current cube.
         */
        std::vector<Universe::UniverseAssumption<Universe::BigInteger>> current;

        /**
         * The vector of the variables appearing in the current cube.
         */
        std::vector<Universe::IUniverseVariable *> variables;

        /**
         * The vector of the indices of the values assumed for the variables in the current cube.
         */
        std::vector<int> indexesCurrentValues;

        /**
         * The vector telling if all variables before a given index are fully explored.
         */
        std::vector<bool> variablesFinished;

    public:

        /**
         * Creates a new StreamLexicographicCube.
         *
         * @param mapping The mapping of the variables of the problem to solve.
         * @param checker The consistency checker used to check the consistency of the cubes.
         * @param nbCubeMax The maximum number of cubes to generate.
         */
        explicit StreamLexicographicCube(const std::map<std::string, Universe::IUniverseVariable *> &mapping,
                                         Panoramyx::IConsistencyChecker *checker,
                                         size_t nbCubeMax);

        /**
         * Destroys this StreamLexicographicCube.
         */
        ~StreamLexicographicCube() override = default;

        /**
         * Checks whether there is another cube in this stream.
         *
         * @return Whether there is another cube in this stream.
         */
        [[nodiscard]] bool hasNext() const override;

        /**
         * Gives the next cube in this stream.
         * The behavior of this method is undefined if hasNext() returned false.
         *
         * @return The next cube.
         */
        std::vector<Universe::UniverseAssumption<Universe::BigInteger>> next() override;

    private:

        /**
         * Generates the first cube, and initializes all internal data-structures.
         */
        void generateFirst();

        /**
         * Generates the next cube.
         * At least one cube must have been generated before invoking this method.
         */
        void generateNext();

    };

}

#endif
