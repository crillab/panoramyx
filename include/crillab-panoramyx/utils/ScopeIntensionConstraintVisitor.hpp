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
 * @file ScopeIntensionConstraintVisitor.hpp
 * @brief A visitor for determining the scope of an intension constraint.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <string>
#include <vector>

#include <crillab-universe/csp/intension/IUniverseIntensionConstraintVisitor.hpp>

#ifndef PANORAMYX_SCOPEINTENSIONCONSTRAINTVISITOR_HPP
#define PANORAMYX_SCOPEINTENSIONCONSTRAINTVISITOR_HPP

namespace Panoramyx {

    /**
     * The ScopeIntensionConstraintVisitor is an instension constraint visitor for determining the scope
     * of intension constraints.
     */
    class ScopeIntensionConstraintVisitor : public Universe::IUniverseIntensionConstraintVisitor {

    private:

        /**
         * The scope of the visited intension constraint.
         */
        std::vector<std::string> scope;

    public:

        /**
         * Destroys this ScopeIntensionConstraintVisitor.
         */
        ~ScopeIntensionConstraintVisitor() override = default;

        /**
         * Visits a constant that appears in an intension constraint.
         *
         * @param constant The constant to visit.
         */
        void visit(Universe::UniverseConstantIntensionConstraint *constant) override;

        /**
         * Visits a variable that appears in an intension constraint.
         *
         * @param variable The variable to visit.
         */
        void visit(Universe::UniverseVariableIntensionConstraint *variable) override;

        /**
         * Visits a unary constraint that appears in an intension constraint.
         *
         * @param constraint The constraint to visit.
         */
        void visit(Universe::UniverseUnaryIntensionConstraint *constraint) override;

        /**
         * Visits a binary constraint that appears in an intension constraint.
         *
         * @param constraint The constraint to visit.
         */
        void visit(Universe::UniverseBinaryIntensionConstraint *constraint) override;

        /**
         * Visits an n-ary constraint that appears in an intension constraint.
         *
         * @param constraint The constraint to visit.
         */
        void visit(Universe::UniverseNaryIntensionConstraint *constraint) override;

        /**
         * Visits an if-then-else constraint that appears in an intension constraint.
         *
         * @param ifThenElse The constraint to visit.
         */
        void visit(Universe::UniverseIfThenElseIntensionConstraint *ifThenElse) override;

        /**
         * Visits a set that appears in an intension constraint.
         *
         * @param set The set to visit.
         */
        void visit(Universe::UniverseSetIntensionConstraint *set) override;

        /**
         * Visits a range that appears in an intension constraint.
         *
         * @param range The range to visit.
         */
        void visit(Universe::UniverseRangeIntensionConstraint *range) override;

        /**
         * Gives the scope of the constraint that has been visited.
         *
         * @return The scope of the visited constraint.
         */
        const std::vector<std::string> &getScope() const;

    };

}

#endif
