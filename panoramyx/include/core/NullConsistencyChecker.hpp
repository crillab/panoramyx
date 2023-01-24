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
 * @file NullConsistencyChecker.hpp
 * @brief Provides a consistency checker that never checks cube consistency.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_NULLCONSISTENCYCHECKER_HPP
#define PANORAMYX_NULLCONSISTENCYCHECKER_HPP

#include "IConsistencyChecker.hpp"

namespace Panoramyx {

    /**
     * The NullConsistencyChecker is a consistency checker that never checks
     * cube consistency.
     */
    class NullConsistencyChecker : public Panoramyx::IConsistencyChecker {

    public:

        /**
         * Destroys this NullConsistencyChecker.
         */
        ~NullConsistencyChecker() override = default;

        /**
         * Checks the consistency of a partial cube, i.e., a cube in which all assumptions have not been added yet.
         *
         * @param cube The assumptions to check.
         *
         * @return Whether the given (partial) cube is consistent.
         */
        bool checkPartial(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &cube) override;

        /**
         * Checks the consistency of a final cube, i.e., a cube in which all assumptions have been added.
         *
         * @param cube The assumptions to check.
         *
         * @return Whether the given (final) cube is consistent.
         */
        bool checkFinal(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &cube) override;

    };

}

#endif
