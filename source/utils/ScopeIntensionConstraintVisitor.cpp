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

#include <crillab-panoramyx/utils/ScopeIntensionConstraintVisitor.hpp>
#include <crillab-universe/csp/intension/UniverseVariableIntensionConstraint.hpp>

using namespace std;

using namespace Panoramyx;
using namespace Universe;

void ScopeIntensionConstraintVisitor::visit(UniverseConstantIntensionConstraint *constant) {
    // Nothing to do.
}

void ScopeIntensionConstraintVisitor::visit(UniverseVariableIntensionConstraint *variable) {
    scope.emplace_back(variable->getIdentifier());
}

void ScopeIntensionConstraintVisitor::visit(UniverseUnaryIntensionConstraint *constraint) {
    // Nothing to do.
}

void ScopeIntensionConstraintVisitor::visit(UniverseBinaryIntensionConstraint *constraint) {
    // Nothing to do.
}

void ScopeIntensionConstraintVisitor::visit(UniverseNaryIntensionConstraint *constraint) {
    // Nothing to do.
}

void ScopeIntensionConstraintVisitor::visit(UniverseIfThenElseIntensionConstraint *ifThenElse) {
    // Nothing to do.
}

void ScopeIntensionConstraintVisitor::visit(UniverseSetIntensionConstraint *set) {
    // Nothing to do.
}

void ScopeIntensionConstraintVisitor::visit(UniverseRangeIntensionConstraint *range) {
    // Nothing to do.
}

const vector<string> &ScopeIntensionConstraintVisitor::getScope() const {
    return scope;
}
