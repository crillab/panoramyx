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
 * @file ProblemUnderAssumption.hpp
 * @brief Stores the information regarding a assumptions under which a problem is to be considered.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_PROBLEMUNDERASSUMPTION_HPP
#define PANORAMYX_PROBLEMUNDERASSUMPTION_HPP

#include <vector>

#include <crillab-universe/core/UniverseAssumption.hpp>
#include <crillab-universe/core/UniverseType.hpp>

namespace Panoramyx {

    /**
     * The ProblemUnderAssumption stores the information regarding a conjunction of assumptions under which a
     * problem is to be considered.
     */
    class ProblemUnderAssumption {

    private:

        /**
         * The conjunction of assumptions under which the problem is to be considered.
         */
        std::vector<Universe::UniverseAssumption<Universe::BigInteger>> assumptions;

        /**
         * The score of the assumptions.
         */
        long score;

    public:

        /**
         * Creates a new ProblemUnderAssumption.
         *
         * @param assumptions The conjunction of assumptions under which the problem is to be considered.
         * @param score The score of the assumptions.
         */
        ProblemUnderAssumption(
                const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumptions, long score);

        /**
         * Gives the conjunction of assumptions under which the problem is to be considered.
         *
         * @return The assumptions under which the problem is to be considered.
         */
        [[nodiscard]] const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &getAssumptions() const;

        /**
         * Gives the score of the assumptions.
         *
         * @return The score of the assumptions.
         */
        [[nodiscard]] long getScore() const;

        /**
         * Compares this ProblemUnderAssumption with another one, depending on their scores.
         *
         * @param rhs The other ProblemUnderAssumption to compare this one with.
         *
         * @return Whether the score of this problem is lower than that of the right-han side.
         */
        bool operator<(const ProblemUnderAssumption &rhs) const;

        /**
         * Compares this ProblemUnderAssumption with another one, depending on their scores.
         *
         * @param rhs The other ProblemUnderAssumption to compare this one with.
         *
         * @return Whether the score of this problem is lower than or equal to that of the right-han side.
         */
        bool operator<=(const ProblemUnderAssumption &rhs) const;

        /**
         * Compares this ProblemUnderAssumption with another one, depending on their scores.
         *
         * @param rhs The other ProblemUnderAssumption to compare this one with.
         *
         * @return Whether the score of this problem is greater than that of the right-han side.
         */
        bool operator>(const ProblemUnderAssumption &rhs) const;

        /**
         * Compares this ProblemUnderAssumption with another one, depending on their scores.
         *
         * @param rhs The other ProblemUnderAssumption to compare this one with.
         *
         * @return Whether the score of this problem is greater than or equal to that of the right-han side.
         */
        bool operator>=(const ProblemUnderAssumption &rhs) const;

    };

}

#endif
