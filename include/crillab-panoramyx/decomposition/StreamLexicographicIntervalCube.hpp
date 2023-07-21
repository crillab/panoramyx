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
 * @file StreamLexicographicIntervalCube.hpp
 * @brief Provides a stream of lexicographic interval cubes.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_STREAMLEXICOGRAPHICINTERVALCUBE_HPP
#define PANORAMYX_STREAMLEXICOGRAPHICINTERVALCUBE_HPP

#include <map>

#include <crillab-universe/core/UniverseAssumption.hpp>
#include <crillab-universe/core/UniverseType.hpp>
#include <crillab-universe/core/problem/IUniverseVariable.hpp>

#include "../core/IConsistencyChecker.hpp"
#include "../utils/Stream.hpp"

namespace Panoramyx {

    /**
     * The StreamLexicographicCube provides a stream of lexicographic interval cubes, that are
     * generated on the fly as the stream is being iterated over.
     */
    class StreamLexicographicIntervalCube :
            public Panoramyx::Stream<std::vector<Universe::UniverseAssumption<Universe::BigInteger>>> {

    private:

        /**
         * The mapping of the variables of the problem to solve.
         */
        const std::map<std::string, Universe::IUniverseVariable *> &mapping;

        /**
         * The maximum number of cubes to generate.
         */
        size_t nbCubeMax;

        /**
         * The number of intervals in which to decompose the domain of variables.
         */
        int nbIntervals;

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
         * @param nbCubeMax The maximum number of cubes to generate.
         * @param nbIntervals The number of intervals in which to decompose the domain of variables.
         */
        explicit StreamLexicographicIntervalCube(const std::map<std::string, Universe::IUniverseVariable *> &mapping,
                                                 size_t nbCubeMax, int nbIntervals);

        /**
         * Destroys this StreamLexicographicCube.
         */
        ~StreamLexicographicIntervalCube() override = default;

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

        /**
         * Backtracks to a variable whose domain is not fully explored.
         *
         * @return The index of the next variable to assign, or -1 if there is no such variable.
         */
        int backtrack();

        /**
         * Adds an assumption to the current cube.
         *
         * @param varIndex The index of the variable to assume.
         * @param valIndex The index of the assumed value for the variable.
         */
        void assume(int varIndex, int valIndex);

        /**
         * Adds an assumption to the current cube.
         *
         * @param varIndex The index of the variable to assume.
         * @param valIndex The index of the assumed value for the variable.
         * @param intervalSize The size of the interval to make an assumption on.
         */
        void assume(int varIndex, int valIndex, int intervalSize);

    };

}

#endif
