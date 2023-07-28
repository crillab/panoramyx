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
 * @file AbstractHypergraphDecompositionSolver.cpp
 * @brief Provides a base implementation of IHypergraphDecompositionSolver
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <crillab-autis/cnf/CnfParser.hpp>
#include <crillab-autis/pb/OpbParser.hpp>
#include <crillab-autis/xcsp/AutisXcspParserAdapter.hpp>

#include <crillab-except/except.hpp>
#include <crillab-panoramyx/decomposition/AbstractHypergraphDecompositionSolver.hpp>
#include <crillab-panoramyx/utils/ScopeIntensionConstraintVisitor.hpp>

using namespace std;

using namespace Autis;
using namespace Except;
using namespace Panoramyx;
using namespace Universe;

void AbstractHypergraphDecompositionSolver::reset() {
    // Nothing to do.
}

int AbstractHypergraphDecompositionSolver::nVariables() {
    return constraintsWithVariables.size();
}

const map<string, IUniverseVariable *> &AbstractHypergraphDecompositionSolver::getVariablesMapping() {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

const vector<string> &AbstractHypergraphDecompositionSolver::getAuxiliaryVariables() {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

const vector<IUniverseConstraint *> &AbstractHypergraphDecompositionSolver::getConstraints() {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

void AbstractHypergraphDecompositionSolver::decisionVariables(const vector<string> &variables) {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

void AbstractHypergraphDecompositionSolver::valueHeuristicStatic(
        const vector<string> &variables, const vector<BigInteger> &orderedValues) {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

int AbstractHypergraphDecompositionSolver::nConstraints() {
    return constrId - 1;
}

bool AbstractHypergraphDecompositionSolver::isOptimization() {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

void AbstractHypergraphDecompositionSolver::setTimeout(long seconds) {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

void AbstractHypergraphDecompositionSolver::setTimeoutMs(long mseconds) {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

void AbstractHypergraphDecompositionSolver::setVerbosity(int level) {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

void AbstractHypergraphDecompositionSolver::addSearchListener(IUniverseSearchListener *listener) {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

void AbstractHypergraphDecompositionSolver::removeSearchListener(IUniverseSearchListener *listener) {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

void AbstractHypergraphDecompositionSolver::setLogFile(const string &filename) {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

void AbstractHypergraphDecompositionSolver::setLogStream(ostream &stream) {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

void AbstractHypergraphDecompositionSolver::loadInstance(const string &filename) {
    ifstream stream(filename);
    Scanner scanner(stream);

    if (filename.ends_with(".cnf")) {
        CnfParser parser(scanner, this);
        parser.parse();

    } else if (filename.ends_with(".opb")) {
        OpbParser parser(scanner, this);
        parser.parse();

    } else if (filename.ends_with(".xml")) {
        AutisXCSPParserAdapter parser(scanner, this);
        parser.parse();

    } else {
        throw UnsupportedOperationException("could not determine file type for " + filename);
    }
}

void AbstractHypergraphDecompositionSolver::interrupt() {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

vector<BigInteger> AbstractHypergraphDecompositionSolver::solution() {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

map<string, BigInteger> AbstractHypergraphDecompositionSolver::mapSolution() {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

map<string, BigInteger> AbstractHypergraphDecompositionSolver::mapSolution(bool excludeAux) {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

bool AbstractHypergraphDecompositionSolver::checkSolution() {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

bool AbstractHypergraphDecompositionSolver::checkSolution(const map<string, BigInteger> &assignment) {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

IOptimizationSolver *AbstractHypergraphDecompositionSolver::toOptimizationSolver() {
    throw UnsupportedOperationException("unsupported for hypergraph decomposition solver");
}

void AbstractHypergraphDecompositionSolver::newVariable(const string &id, int min, int max) {
    // Nothing to do.
}

void AbstractHypergraphDecompositionSolver::newVariable(const string &id, const BigInteger &min, const BigInteger &max) {
    // Nothing to do.
}

void AbstractHypergraphDecompositionSolver::newVariable(const string &id, const vector<int> &values) {
    // Nothing to do.
}

void AbstractHypergraphDecompositionSolver::newVariable(const string &id, const vector<BigInteger> &values) {
    // Nothing to do.
}

void AbstractHypergraphDecompositionSolver::newVariableSymbolic(const string &id, const vector<string> &values) {
    // Nothing to do.
}

void AbstractHypergraphDecompositionSolver::addClause(const vector<int> &literals) {
    addConstraint(toString(literals));
}

void AbstractHypergraphDecompositionSolver::addPseudoBoolean(const vector<int> &literals,
                                                             const vector<BigInteger> &coefficients,
                                                             bool moreThan, const BigInteger &degree) {
    addConstraint(toString(literals));
}

void AbstractHypergraphDecompositionSolver::addAtMost(const vector<int> &literals, int degree) {
    addConstraint(toString(literals));
}

void AbstractHypergraphDecompositionSolver::addAtMost(const vector<int> &literals, const vector<int> &coefficients,
                                                      int degree) {
    addConstraint(toString(literals));
}

void AbstractHypergraphDecompositionSolver::addAtMost(const vector<int> &literals,
                                                      const vector<BigInteger> &coefficients,
                                                      const BigInteger &degree) {
    addConstraint(toString(literals));
}

void AbstractHypergraphDecompositionSolver::addAtLeast(const vector<int> &literals, int degree) {
    addConstraint(toString(literals));
}

void AbstractHypergraphDecompositionSolver::addAtLeast(const vector<int> &literals, const vector<int> &coefficients,
                                                       int degree) {
    addConstraint(toString(literals));
}

void AbstractHypergraphDecompositionSolver::addAtLeast(const vector<int> &literals,
                                                       const vector<BigInteger> &coefficients,
                                                       const BigInteger &degree) {
    addConstraint(toString(literals));
}

void AbstractHypergraphDecompositionSolver::addExactly(const vector<int> &literals, int degree) {
    addConstraint(toString(literals));
}

void AbstractHypergraphDecompositionSolver::addExactly(const vector<int> &literals, const vector<int> &coefficients,
                                                       int degree) {
    addConstraint(toString(literals));
}

void AbstractHypergraphDecompositionSolver::addExactly(const vector<int> &literals,
                                                       const vector<BigInteger> &coefficients,
                                                       const BigInteger &degree) {
    addConstraint(toString(literals));
}

void AbstractHypergraphDecompositionSolver::addInstantiation(const string &variable, int value) {
    addConstraint({variable});
}

void AbstractHypergraphDecompositionSolver::addInstantiation(const string &variable, const BigInteger &value) {
    addConstraint({variable});
}

void AbstractHypergraphDecompositionSolver::addInstantiation(const string &variable, const string &value) {
    addConstraint({variable});
}

void AbstractHypergraphDecompositionSolver::addInstantiation(const vector<string> &variables,
                                                             const vector<int> &values) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addInstantiation(const vector<string> &variables,
                                                             const vector<BigInteger> &values) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addInstantiationSymbolic(const vector<string> &variables,
                                                                     const vector<string> &values) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addClause(const vector<string> &positive,
                                                      const vector<string> &negative) {
    addConstraint(flatten({positive, negative}));
}

void AbstractHypergraphDecompositionSolver::addLogical(UniverseBooleanOperator op,
                                                       const vector<string> &variables) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addLogical(const string &variable, bool equiv,
                                                       UniverseBooleanOperator op,
                                                       const vector<string> &variables) {
    addConstraint(flatten({{variable}, variables}));
}

void AbstractHypergraphDecompositionSolver::addLogical(const string &variable, const string &left,
                                                       UniverseRelationalOperator op,
                                                       const BigInteger &right) {
    addConstraint({variable, left});
}

void AbstractHypergraphDecompositionSolver::addLogical(const string &variable, const string &left,
                                                       UniverseRelationalOperator op, const string &right) {
    addConstraint({variable, left, right});
}

void AbstractHypergraphDecompositionSolver::addAllDifferent(const vector<string> &variables) {
    addConstraint({variables});
}

void AbstractHypergraphDecompositionSolver::addAllDifferent(const vector<string> &variables,
                                                            const vector<BigInteger> &except) {
    addConstraint({variables});
}

void
AbstractHypergraphDecompositionSolver::addAllDifferentMatrix(const vector<vector<string>> &variableMatrix) {
    addConstraint(flatten(variableMatrix));
}

void
AbstractHypergraphDecompositionSolver::addAllDifferentMatrix(const vector<vector<string>> &variableMatrix,
                                                             const vector<BigInteger> &except) {
    addConstraint(flatten(variableMatrix));
}

void AbstractHypergraphDecompositionSolver::addAllDifferentList(const vector<vector<string>> &variableLists) {
    addConstraint(flatten(variableLists));
}

void AbstractHypergraphDecompositionSolver::addAllDifferentList(const vector<vector<string>> &variableLists,
                                                                const vector<vector<BigInteger>> &except) {
    addConstraint(flatten(variableLists));
}

void AbstractHypergraphDecompositionSolver::addAllDifferentIntension(
        const vector<IUniverseIntensionConstraint *> &intensionConstraints) {
    addConstraint(scopeOf(intensionConstraints));
}

void AbstractHypergraphDecompositionSolver::addChannel(const vector<string> &variables, int startIndex) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addChannel(const vector<string> &variables, int startIndex,
                                                       const string &value) {
    addConstraint(flatten({variables, {value}}));
}

void AbstractHypergraphDecompositionSolver::addChannel(const vector<string> &variables, int startIndex,
                                                       const vector<string> &otherVariables, int otherStartIndex) {
    addConstraint(flatten({variables, otherVariables}));
}

void AbstractHypergraphDecompositionSolver::addCardinalityWithConstantValuesAndConstantCounts(
        const vector<string> &variables, const vector<BigInteger> &values,
        const vector<BigInteger> &occurs, bool closed) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addCardinalityWithConstantValuesAndConstantIntervalCounts(
        const vector<string> &variables, const vector<BigInteger> &values,
        const vector<BigInteger> &occursMin, const vector<BigInteger> &occursMax, bool closed) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addCardinalityWithConstantValuesAndVariableCounts(
        const vector<string> &variables, const vector<BigInteger> &values,
        const vector<string> &occurs, bool closed) {
    addConstraint(flatten({variables, occurs}));
}

void AbstractHypergraphDecompositionSolver::addCardinalityWithVariableValuesAndConstantCounts(
        const vector<string> &variables, const vector<string> &values,
        const vector<BigInteger> &occurs, bool closed) {
    addConstraint(flatten({variables, values}));
}

void AbstractHypergraphDecompositionSolver::addCardinalityWithVariableValuesAndConstantIntervalCounts(
        const vector<string> &variables, const vector<string> &values,
        const vector<BigInteger> &occursMin, const vector<BigInteger> &occursMax, bool closed) {
    addConstraint(flatten({variables, values}));
}

void AbstractHypergraphDecompositionSolver::addCardinalityWithVariableValuesAndVariableCounts(
        const vector<string> &variables, const vector<string> &values, const vector<string> &occurs,
        bool closed) {
    addConstraint(flatten({variables, values, occurs}));
}

void AbstractHypergraphDecompositionSolver::addAtLeast(const vector<string> &variables, const BigInteger &value,
                                                       const BigInteger &count) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addExactly(const vector<string> &variables, const BigInteger &value,
                                                       const BigInteger &count) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addExactly(const vector<string> &variables, const BigInteger &value,
                                                       const string &count) {
    addConstraint(flatten({variables, {count}}));
}

void AbstractHypergraphDecompositionSolver::addAmong(const vector<string> &variables,
                                                     const vector<BigInteger> &values,
                                                     const BigInteger &count) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addAmong(const vector<string> &variables,
                                                     const vector<BigInteger> &values, const string &count) {
    addConstraint(flatten({variables, {count}}));
}

void AbstractHypergraphDecompositionSolver::addAtMost(const vector<string> &variables, const BigInteger &value,
                                                      const BigInteger &count) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addCountWithConstantValues(const vector<string> &variables,
                                                                       const vector<BigInteger> &values,
                                                                       UniverseRelationalOperator op,
                                                                       const BigInteger &count) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addCountWithConstantValues(const vector<string> &variables,
                                                                       const vector<BigInteger> &values,
                                                                       UniverseRelationalOperator op,
                                                                       const string &count) {
    addConstraint(flatten({variables, {count}}));
}

void AbstractHypergraphDecompositionSolver::addCountWithConstantValues(const vector<string> &variables,
                                                                       const vector<BigInteger> &values,
                                                                       UniverseSetBelongingOperator op,
                                                                       const BigInteger &min, const BigInteger &max) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addCountWithConstantValues(const vector<string> &variables,
                                                                       const vector<BigInteger> &values,
                                                                       UniverseSetBelongingOperator op,
                                                                       const vector<BigInteger> &set) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addCountWithVariableValues(const vector<string> &variables,
                                                                       const vector<string> &values,
                                                                       UniverseRelationalOperator op,
                                                                       const BigInteger &count) {
    addConstraint(flatten({variables, values}));
}

void AbstractHypergraphDecompositionSolver::addCountWithVariableValues(const vector<string> &variables,
                                                                       const vector<string> &values,
                                                                       UniverseRelationalOperator op,
                                                                       const string &count) {
    addConstraint(flatten({variables, values, {count}}));
}

void AbstractHypergraphDecompositionSolver::addCountWithVariableValues(const vector<string> &variables,
                                                                       const vector<string> &values,
                                                                       UniverseSetBelongingOperator op,
                                                                       const BigInteger &min, const BigInteger &max) {
    addConstraint(flatten({variables, values}));
}

void AbstractHypergraphDecompositionSolver::addCountWithVariableValues(const vector<string> &variables,
                                                                       const vector<string> &values,
                                                                       UniverseSetBelongingOperator op,
                                                                       const vector<BigInteger> &set) {
    addConstraint(flatten({variables, values}));

}

void AbstractHypergraphDecompositionSolver::addCountIntensionWithConstantValues(
        const vector<IUniverseIntensionConstraint *> &expressions, const vector<BigInteger> &values,
        UniverseRelationalOperator op, const BigInteger &count) {
    addConstraint(scopeOf(expressions));

}

void AbstractHypergraphDecompositionSolver::addCountIntensionWithConstantValues(
        const vector<IUniverseIntensionConstraint *> &expressions, const vector<BigInteger> &values,
        UniverseRelationalOperator op, const string &count) {
    addConstraint(flatten({scopeOf(expressions), {count}}));
}

void AbstractHypergraphDecompositionSolver::addCountIntensionWithConstantValues(
        const vector<IUniverseIntensionConstraint *> &expressions, const vector<BigInteger> &values,
        UniverseSetBelongingOperator op, const BigInteger &min, const BigInteger &max) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::addCountIntensionWithConstantValues(
        const vector<IUniverseIntensionConstraint *> &expressions, const vector<BigInteger> &values,
        UniverseSetBelongingOperator op, const vector<BigInteger> &set) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::addNValues(const vector<string> &variables,
                                                       UniverseRelationalOperator op, const BigInteger &nb) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addNValuesExcept(const vector<string> &variables,
                                                             UniverseRelationalOperator op,
                                                             const BigInteger &nb,
                                                             const vector<BigInteger> &except) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addNValues(const vector<string> &variables,
                                                       UniverseRelationalOperator op, const string &nb) {
    addConstraint(flatten({variables, {nb}}));
}

void AbstractHypergraphDecompositionSolver::addNValuesExcept(const vector<string> &variables,
                                                             UniverseRelationalOperator op, const string &nb,
                                                             const vector<BigInteger> &except) {
    addConstraint(flatten({variables, {nb}}));
}

void AbstractHypergraphDecompositionSolver::addNValues(const vector<string> &variables,
                                                       UniverseSetBelongingOperator op, const BigInteger &min,
                                                       const BigInteger &max) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addNValuesExcept(const vector<string> &variables,
                                                             UniverseSetBelongingOperator op,
                                                             const BigInteger &min, const BigInteger &max,
                                                             const vector<BigInteger> &except) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addNValues(const vector<string> &variables,
                                                       UniverseSetBelongingOperator op,
                                                       const vector<BigInteger> &set) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addNValuesExcept(const vector<string> &variables,
                                                             UniverseSetBelongingOperator op,
                                                             const vector<BigInteger> &set,
                                                             const vector<BigInteger> &except) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addNValuesIntension(
        const vector<IUniverseIntensionConstraint *> &expressions, UniverseRelationalOperator op,
        const BigInteger &nb) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::addNValuesIntension(
        const vector<IUniverseIntensionConstraint *> &expressions, UniverseRelationalOperator op,
        const string &nb) {
    addConstraint(flatten({scopeOf(expressions), {nb}}));
}

void AbstractHypergraphDecompositionSolver::addNValuesIntension(
        const vector<IUniverseIntensionConstraint *> &expressions, UniverseSetBelongingOperator op,
        const BigInteger &min, const BigInteger &max) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::addNValuesIntension(
        const vector<IUniverseIntensionConstraint *> &expressions, UniverseSetBelongingOperator op,
        const vector<BigInteger> &set) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::addBinPacking(const vector<string> &variables,
                                                          const vector<BigInteger> &sizes,
                                                          UniverseRelationalOperator op,
                                                          const BigInteger &value) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addBinPacking(const vector<string> &variables,
                                                          const vector<BigInteger> &sizes,
                                                          UniverseRelationalOperator op,
                                                          const string &variable) {
    addConstraint(flatten({variables, {variable}}));
}

void AbstractHypergraphDecompositionSolver::addBinPacking(const vector<string> &variables,
                                                          const vector<BigInteger> &sizes,
                                                          UniverseSetBelongingOperator op,
                                                          const BigInteger &min, const BigInteger &max) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addBinPacking(const vector<string> &variables,
                                                          const vector<BigInteger> &sizes,
                                                          UniverseSetBelongingOperator op,
                                                          const vector<BigInteger> &set) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addBinPackingWithConstantCapacities(const vector<string> &variables,
                                                                                const vector<BigInteger> &sizes,
                                                                                const vector<BigInteger> &capacities,
                                                                                bool loads) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addBinPackingWithVariableCapacities(const vector<string> &variables,
                                                                                const vector<BigInteger> &sizes,
                                                                                const vector<string> &capacities,
                                                                                bool loads) {
    addConstraint(flatten({variables, capacities}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeConstantLengthsConstantHeights(const vector<string> &origins,
                                                                                   const vector<BigInteger> &lengths,
                                                                                   const vector<BigInteger> &heights,
                                                                                   UniverseRelationalOperator op,
                                                                                   const BigInteger &value) {
    addConstraint(origins);
}

void
AbstractHypergraphDecompositionSolver::addCumulativeConstantLengthsConstantHeights(const vector<string> &origins,
                                                                                   const vector<BigInteger> &lengths,
                                                                                   const vector<string> &ends,
                                                                                   const vector<BigInteger> &heights,
                                                                                   UniverseRelationalOperator op,
                                                                                   const BigInteger &value) {
    addConstraint(flatten({origins, ends}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeConstantLengthsConstantHeights(const vector<string> &origins,
                                                                                   const vector<BigInteger> &lengths,
                                                                                   const vector<BigInteger> &heights,
                                                                                   UniverseRelationalOperator op,
                                                                                   const string &value) {
    addConstraint(flatten({origins, {value}}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeConstantLengthsConstantHeights(const vector<string> &origins,
                                                                                   const vector<BigInteger> &lengths,
                                                                                   const vector<string> &ends,
                                                                                   const vector<BigInteger> &heights,
                                                                                   UniverseRelationalOperator op,
                                                                                   const string &value) {
    addConstraint(flatten({origins, ends, {value}}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeConstantLengthsConstantHeights(const vector<string> &origins,
                                                                                   const vector<BigInteger> &lengths,
                                                                                   const vector<BigInteger> &heights,
                                                                                   UniverseSetBelongingOperator op,
                                                                                   const BigInteger &min,
                                                                                   const BigInteger &max) {
    addConstraint(origins);
}

void
AbstractHypergraphDecompositionSolver::addCumulativeConstantLengthsConstantHeights(const vector<string> &origins,
                                                                                   const vector<BigInteger> &lengths,
                                                                                   const vector<string> &ends,
                                                                                   const vector<BigInteger> &heights,
                                                                                   UniverseSetBelongingOperator op,
                                                                                   const BigInteger &min,
                                                                                   const BigInteger &max) {
    addConstraint(flatten({origins, ends}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeConstantLengthsConstantHeights(const vector<string> &origins,
                                                                                   const vector<BigInteger> &lengths,
                                                                                   const vector<BigInteger> &heights,
                                                                                   UniverseSetBelongingOperator op,
                                                                                   const vector<BigInteger> &set) {
    addConstraint(origins);
}

void
AbstractHypergraphDecompositionSolver::addCumulativeConstantLengthsConstantHeights(const vector<string> &origins,
                                                                                   const vector<BigInteger> &lengths,
                                                                                   const vector<string> &ends,
                                                                                   const vector<BigInteger> &heights,
                                                                                   UniverseSetBelongingOperator op,
                                                                                   const vector<BigInteger> &set) {
    addConstraint(flatten({origins, ends}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeConstantLengthsVariableHeights(const vector<string> &origins,
                                                                                   const vector<BigInteger> &lengths,
                                                                                   const vector<string> &heights,
                                                                                   UniverseRelationalOperator op,
                                                                                   const BigInteger &value) {
    addConstraint(flatten({origins, heights}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeConstantLengthsVariableHeights(const vector<string> &origins,
                                                                                   const vector<BigInteger> &lengths,
                                                                                   const vector<string> &ends,
                                                                                   const vector<string> &heights,
                                                                                   UniverseRelationalOperator op,
                                                                                   const BigInteger &value) {
    addConstraint(flatten({origins, ends, heights}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeConstantLengthsVariableHeights(const vector<string> &origins,
                                                                                   const vector<BigInteger> &lengths,
                                                                                   const vector<string> &heights,
                                                                                   UniverseRelationalOperator op,
                                                                                   const string &value) {
    addConstraint(flatten({origins, heights, {value}}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeConstantLengthsVariableHeights(const vector<string> &origins,
                                                                                   const vector<BigInteger> &lengths,
                                                                                   const vector<string> &ends,
                                                                                   const vector<string> &heights,
                                                                                   UniverseRelationalOperator op,
                                                                                   const string &value) {
    addConstraint(flatten({origins, ends, heights, {value}}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeConstantLengthsVariableHeights(const vector<string> &origins,
                                                                                   const vector<BigInteger> &lengths,
                                                                                   const vector<string> &heights,
                                                                                   UniverseSetBelongingOperator op,
                                                                                   const BigInteger &min,
                                                                                   const BigInteger &max) {
    addConstraint(flatten({origins, heights}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeConstantLengthsVariableHeights(const vector<string> &origins,
                                                                                   const vector<BigInteger> &lengths,
                                                                                   const vector<string> &ends,
                                                                                   const vector<string> &heights,
                                                                                   UniverseSetBelongingOperator op,
                                                                                   const BigInteger &min,
                                                                                   const BigInteger &max) {
    addConstraint(flatten({origins, ends, heights}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeConstantLengthsVariableHeights(const vector<string> &origins,
                                                                                   const vector<BigInteger> &lengths,
                                                                                   const vector<string> &heights,
                                                                                   UniverseSetBelongingOperator op,
                                                                                   const vector<BigInteger> &set) {
    addConstraint(flatten({origins, heights}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeConstantLengthsVariableHeights(const vector<string> &origins,
                                                                                   const vector<BigInteger> &lengths,
                                                                                   const vector<string> &ends,
                                                                                   const vector<string> &heights,
                                                                                   UniverseSetBelongingOperator op,
                                                                                   const vector<BigInteger> &set) {
    addConstraint(flatten({origins, ends, heights}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeVariableLengthsConstantHeights(const vector<string> &origins,
                                                                                   const vector<string> &lengths,
                                                                                   const vector<BigInteger> &heights,
                                                                                   UniverseRelationalOperator op,
                                                                                   const BigInteger &value) {
    addConstraint(flatten({origins, lengths}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeVariableLengthsConstantHeights(const vector<string> &origins,
                                                                                   const vector<string> &lengths,
                                                                                   const vector<string> &ends,
                                                                                   const vector<BigInteger> &heights,
                                                                                   UniverseRelationalOperator op,
                                                                                   const BigInteger &value) {
    addConstraint(flatten({origins, lengths, ends}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeVariableLengthsConstantHeights(const vector<string> &origins,
                                                                                   const vector<string> &lengths,
                                                                                   const vector<BigInteger> &heights,
                                                                                   UniverseRelationalOperator op,
                                                                                   const string &value) {
    addConstraint(flatten({origins, lengths, {value}}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeVariableLengthsConstantHeights(const vector<string> &origins,
                                                                                   const vector<string> &lengths,
                                                                                   const vector<string> &ends,
                                                                                   const vector<BigInteger> &heights,
                                                                                   UniverseRelationalOperator op,
                                                                                   const string &value) {
    addConstraint(flatten({origins, lengths, ends, {value}}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeVariableLengthsConstantHeights(const vector<string> &origins,
                                                                                   const vector<string> &lengths,
                                                                                   const vector<BigInteger> &heights,
                                                                                   UniverseSetBelongingOperator op,
                                                                                   const BigInteger &min,
                                                                                   const BigInteger &max) {
    addConstraint(flatten({origins, lengths}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeVariableLengthsConstantHeights(const vector<string> &origins,
                                                                                   const vector<string> &lengths,
                                                                                   const vector<string> &ends,
                                                                                   const vector<BigInteger> &heights,
                                                                                   UniverseSetBelongingOperator op,
                                                                                   const BigInteger &min,
                                                                                   const BigInteger &max) {
    addConstraint(flatten({origins, lengths, ends}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeVariableLengthsConstantHeights(const vector<string> &origins,
                                                                                   const vector<string> &lengths,
                                                                                   const vector<BigInteger> &heights,
                                                                                   UniverseSetBelongingOperator op,
                                                                                   const vector<BigInteger> &set) {
    addConstraint(flatten({origins, lengths}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeVariableLengthsConstantHeights(const vector<string> &origins,
                                                                                   const vector<string> &lengths,
                                                                                   const vector<string> &ends,
                                                                                   const vector<BigInteger> &heights,
                                                                                   UniverseSetBelongingOperator op,
                                                                                   const vector<BigInteger> &set) {
    addConstraint(flatten({origins, lengths, ends}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeVariableLengthsVariableHeights(const vector<string> &origins,
                                                                                   const vector<string> &lengths,
                                                                                   const vector<string> &heights,
                                                                                   UniverseRelationalOperator op,
                                                                                   const BigInteger &value) {
    addConstraint(flatten({origins, lengths, heights}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeVariableLengthsVariableHeights(const vector<string> &origins,
                                                                                   const vector<string> &lengths,
                                                                                   const vector<string> &ends,
                                                                                   const vector<string> &heights,
                                                                                   UniverseRelationalOperator op,
                                                                                   const BigInteger &value) {
    addConstraint(flatten({origins, lengths, ends, heights}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeVariableLengthsVariableHeights(const vector<string> &origins,
                                                                                   const vector<string> &lengths,
                                                                                   const vector<string> &heights,
                                                                                   UniverseRelationalOperator op,
                                                                                   const string &value) {
    addConstraint(flatten({origins, lengths, heights, {value}}));

}

void
AbstractHypergraphDecompositionSolver::addCumulativeVariableLengthsVariableHeights(const vector<string> &origins,
                                                                                   const vector<string> &lengths,
                                                                                   const vector<string> &ends,
                                                                                   const vector<string> &heights,
                                                                                   UniverseRelationalOperator op,
                                                                                   const string &value) {
    addConstraint(flatten({origins, lengths, ends, heights, {value}}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeVariableLengthsVariableHeights(const vector<string> &origins,
                                                                                   const vector<string> &lengths,
                                                                                   const vector<string> &heights,
                                                                                   UniverseSetBelongingOperator op,
                                                                                   const BigInteger &min,
                                                                                   const BigInteger &max) {
    addConstraint(flatten({origins, lengths, heights}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeVariableLengthsVariableHeights(const vector<string> &origins,
                                                                                   const vector<string> &lengths,
                                                                                   const vector<string> &ends,
                                                                                   const vector<string> &heights,
                                                                                   UniverseSetBelongingOperator op,
                                                                                   const BigInteger &min,
                                                                                   const BigInteger &max) {
    addConstraint(flatten({origins, lengths, ends, heights}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeVariableLengthsVariableHeights(const vector<string> &origins,
                                                                                   const vector<string> &lengths,
                                                                                   const vector<string> &heights,
                                                                                   UniverseSetBelongingOperator op,
                                                                                   const vector<BigInteger> &set) {
    addConstraint(flatten({origins, lengths, heights}));
}

void
AbstractHypergraphDecompositionSolver::addCumulativeVariableLengthsVariableHeights(const vector<string> &origins,
                                                                                   const vector<string> &lengths,
                                                                                   const vector<string> &ends,
                                                                                   const vector<string> &heights,
                                                                                   UniverseSetBelongingOperator op,
                                                                                   const vector<BigInteger> &set) {
    addConstraint(flatten({origins, lengths, ends, heights}));
}

void AbstractHypergraphDecompositionSolver::addKnapsack(const vector<string> &variables,
                                                        const vector<BigInteger> &weights,
                                                        UniverseRelationalOperator wOperator,
                                                        const BigInteger &wValue,
                                                        const vector<BigInteger> &profits,
                                                        UniverseRelationalOperator pOperator,
                                                        const BigInteger &pValue) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addKnapsack(const vector<string> &variables,
                                                        const vector<BigInteger> &weights,
                                                        UniverseRelationalOperator wOperator,
                                                        const BigInteger &wValue,
                                                        const vector<BigInteger> &profits,
                                                        UniverseRelationalOperator pOperator,
                                                        const string &pVariable) {
    addConstraint(flatten({variables,{pVariable}}));
}

void AbstractHypergraphDecompositionSolver::addKnapsack(const vector<string> &variables,
                                                        const vector<BigInteger> &weights,
                                                        UniverseRelationalOperator wOperator,
                                                        const BigInteger &wValue,
                                                        const vector<BigInteger> &profits,
                                                        UniverseSetBelongingOperator pOperator,
                                                        const BigInteger &pMin, const BigInteger &pMax) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addKnapsack(const vector<string> &variables,
                                                        const vector<BigInteger> &weights,
                                                        UniverseRelationalOperator wOperator,
                                                        const BigInteger &wValue,
                                                        const vector<BigInteger> &profits,
                                                        UniverseSetBelongingOperator pOperator,
                                                        const vector<BigInteger> &pSet) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addKnapsack(const vector<string> &variables,
                                                        const vector<BigInteger> &weights,
                                                        UniverseRelationalOperator wOperator,
                                                        const string &wVariable,
                                                        const vector<BigInteger> &profits,
                                                        UniverseRelationalOperator pOperator,
                                                        const BigInteger &pValue) {
    addConstraint(flatten({variables,{wVariable}}));
}

void AbstractHypergraphDecompositionSolver::addKnapsack(const vector<string> &variables,
                                                        const vector<BigInteger> &weights,
                                                        UniverseRelationalOperator wOperator,
                                                        const string &wVariable,
                                                        const vector<BigInteger> &profits,
                                                        UniverseRelationalOperator pOperator,
                                                        const string &pVariable) {
    addConstraint(flatten({variables,{wVariable}, {pVariable}}));
}

void AbstractHypergraphDecompositionSolver::addKnapsack(const vector<string> &variables,
                                                        const vector<BigInteger> &weights,
                                                        UniverseRelationalOperator wOperator,
                                                        const string &wVariable,
                                                        const vector<BigInteger> &profits,
                                                        UniverseSetBelongingOperator pOperator,
                                                        const BigInteger &pMin, const BigInteger &pMax) {
    addConstraint(flatten({variables,{wVariable}}));
}

void AbstractHypergraphDecompositionSolver::addKnapsack(const vector<string> &variables,
                                                        const vector<BigInteger> &weights,
                                                        UniverseRelationalOperator wOperator,
                                                        const string &wVariable,
                                                        const vector<BigInteger> &profits,
                                                        UniverseSetBelongingOperator pOperator,
                                                        const vector<BigInteger> &pSet) {
    addConstraint(flatten({variables,{wVariable}}));
}

void AbstractHypergraphDecompositionSolver::addKnapsack(const vector<string> &variables,
                                                        const vector<BigInteger> &weights,
                                                        UniverseSetBelongingOperator wOperator,
                                                        const BigInteger &wMin, const BigInteger &wMax,
                                                        const vector<BigInteger> &profits,
                                                        UniverseRelationalOperator pOperator,
                                                        const BigInteger &pValue) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addKnapsack(const vector<string> &variables,
                                                        const vector<BigInteger> &weights,
                                                        UniverseSetBelongingOperator wOperator,
                                                        const BigInteger &wMin, const BigInteger &wMax,
                                                        const vector<BigInteger> &profits,
                                                        UniverseRelationalOperator pOperator,
                                                        const string &pVariable) {
    addConstraint(flatten({variables,{pVariable}}));
}

void AbstractHypergraphDecompositionSolver::addKnapsack(const vector<string> &variables,
                                                        const vector<BigInteger> &weights,
                                                        UniverseSetBelongingOperator wOperator,
                                                        const BigInteger &wMin, const BigInteger &wMax,
                                                        const vector<BigInteger> &profits,
                                                        UniverseSetBelongingOperator pOperator,
                                                        const BigInteger &pMin, const BigInteger &pMax) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addKnapsack(const vector<string> &variables,
                                                        const vector<BigInteger> &weights,
                                                        UniverseSetBelongingOperator wOperator,
                                                        const BigInteger &wMin, const BigInteger &wMax,
                                                        const vector<BigInteger> &profits,
                                                        UniverseSetBelongingOperator pOperator,
                                                        const vector<BigInteger> &pSet) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addKnapsack(const vector<string> &variables,
                                                        const vector<BigInteger> &weights,
                                                        UniverseSetBelongingOperator wOperator,
                                                        const vector<BigInteger> &wSet,
                                                        const vector<BigInteger> &profits,
                                                        UniverseRelationalOperator pOperator,
                                                        const BigInteger &pValue) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addKnapsack(const vector<string> &variables,
                                                        const vector<BigInteger> &weights,
                                                        UniverseSetBelongingOperator wOperator,
                                                        const vector<BigInteger> &wSet,
                                                        const vector<BigInteger> &profits,
                                                        UniverseRelationalOperator pOperator,
                                                        const string &pVariable) {
    addConstraint(flatten({variables, {pVariable}}));
}

void AbstractHypergraphDecompositionSolver::addKnapsack(const vector<string> &variables,
                                                        const vector<BigInteger> &weights,
                                                        UniverseSetBelongingOperator wOperator,
                                                        const vector<BigInteger> &wSet,
                                                        const vector<BigInteger> &profits,
                                                        UniverseSetBelongingOperator pOperator,
                                                        const BigInteger &pMin, const BigInteger &pMax) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addKnapsack(const vector<string> &variables,
                                                        const vector<BigInteger> &weights,
                                                        UniverseSetBelongingOperator wOperator,
                                                        const vector<BigInteger> &wSet,
                                                        const vector<BigInteger> &profits,
                                                        UniverseSetBelongingOperator pOperator,
                                                        const vector<BigInteger> &pSet) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addStretch(const vector<string> &variables,
                                                       const vector<BigInteger> &values,
                                                       const vector<BigInteger> &widthsMin,
                                                       const vector<BigInteger> &widthsMax) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addStretch(const vector<string> &variables,
                                                       const vector<BigInteger> &values,
                                                       const vector<BigInteger> &widthsMin,
                                                       const vector<BigInteger> &widthsMax,
                                                       const vector<vector<BigInteger>> &patterns) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addElement(const vector<string> &variables,
                                                       UniverseRelationalOperator op,
                                                       const BigInteger &value) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addElement(const vector<string> &variables,
                                                       UniverseRelationalOperator op, const string &value) {
    addConstraint(flatten({variables, {value}}));
}

void AbstractHypergraphDecompositionSolver::addElement(const vector<string> &variables,
                                                       UniverseSetBelongingOperator op, const BigInteger &min,
                                                       const BigInteger &max) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addElement(const vector<string> &variables,
                                                       UniverseSetBelongingOperator op,
                                                       const vector<BigInteger> &set) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addElementConstantValues(const vector<BigInteger> &values,
                                                                     int startIndex, const string &index,
                                                                     UniverseRelationalOperator op,
                                                                     const BigInteger &value) {
    addConstraint({index});
}

void AbstractHypergraphDecompositionSolver::addElementConstantValues(const vector<BigInteger> &values,
                                                                     int startIndex, const string &index,
                                                                     UniverseRelationalOperator op,
                                                                     const string &variable) {
    addConstraint({index, variable});
}

void AbstractHypergraphDecompositionSolver::addElementConstantValues(const vector<BigInteger> &values,
                                                                     int startIndex, const string &index,
                                                                     UniverseSetBelongingOperator op,
                                                                     const BigInteger &min, const BigInteger &max) {
    addConstraint({index});
}

void AbstractHypergraphDecompositionSolver::addElementConstantValues(const vector<BigInteger> &values,
                                                                     int startIndex, const string &index,
                                                                     UniverseSetBelongingOperator op,
                                                                     const vector<BigInteger> &set) {
    addConstraint({index});
}

void AbstractHypergraphDecompositionSolver::addElement(const vector<string> &variables, int startIndex,
                                                       const string &index, UniverseRelationalOperator op,
                                                       const BigInteger &value) {
    addConstraint(flatten({variables, {index}}));
}

void AbstractHypergraphDecompositionSolver::addElement(const vector<string> &variables, int startIndex,
                                                       const string &index, UniverseRelationalOperator op,
                                                       const string &variable) {
    addConstraint(flatten({variables, {index, variable}}));
}

void AbstractHypergraphDecompositionSolver::addElement(const vector<string> &variables, int startIndex,
                                                       const string &index, UniverseSetBelongingOperator op,
                                                       const BigInteger &min, const BigInteger &max) {
    addConstraint(flatten({variables, {index}}));
}

void AbstractHypergraphDecompositionSolver::addElement(const vector<string> &variables, int startIndex,
                                                       const string &index, UniverseSetBelongingOperator op,
                                                       const vector<BigInteger> &set) {
    addConstraint(flatten({variables, {index}}));
}

void
AbstractHypergraphDecompositionSolver::addElementConstantMatrix(const vector<vector<BigInteger>> &matrix,
                                                                int startRowIndex, const string &rowIndex,
                                                                int startColIndex, const string &colIndex,
                                                                UniverseRelationalOperator op,
                                                                const BigInteger &value) {
    addConstraint({rowIndex, colIndex});
}

void
AbstractHypergraphDecompositionSolver::addElementConstantMatrix(const vector<vector<BigInteger>> &matrix,
                                                                int startRowIndex, const string &rowIndex,
                                                                int startColIndex, const string &colIndex,
                                                                UniverseRelationalOperator op,
                                                                const string &value) {
    addConstraint({rowIndex, colIndex, value});
}

void
AbstractHypergraphDecompositionSolver::addElementConstantMatrix(const vector<vector<BigInteger>> &matrix,
                                                                int startRowIndex, const string &rowIndex,
                                                                int startColIndex, const string &colIndex,
                                                                UniverseSetBelongingOperator op,
                                                                const BigInteger &min, const BigInteger &max) {
    addConstraint({rowIndex, colIndex});
}

void
AbstractHypergraphDecompositionSolver::addElementConstantMatrix(const vector<vector<BigInteger>> &matrix,
                                                                int startRowIndex, const string &rowIndex,
                                                                int startColIndex, const string &colIndex,
                                                                UniverseSetBelongingOperator op,
                                                                const vector<BigInteger> &set) {
    addConstraint({rowIndex, colIndex});
}

void AbstractHypergraphDecompositionSolver::addElementMatrix(const vector<vector<string>> &matrix,
                                                             int startRowIndex, const string &rowIndex,
                                                             int startColIndex, const string &colIndex,
                                                             UniverseRelationalOperator op,
                                                             const BigInteger &value) {
    addConstraint(flatten({flatten(matrix), {rowIndex, colIndex}}));
}

void AbstractHypergraphDecompositionSolver::addElementMatrix(const vector<vector<string>> &matrix,
                                                             int startRowIndex, const string &rowIndex,
                                                             int startColIndex, const string &colIndex,
                                                             UniverseRelationalOperator op,
                                                             const string &value) {
    addConstraint(flatten({flatten(matrix), {rowIndex, colIndex, value}}));
}

void AbstractHypergraphDecompositionSolver::addElementMatrix(const vector<vector<string>> &matrix,
                                                             int startRowIndex, const string &rowIndex,
                                                             int startColIndex, const string &colIndex,
                                                             UniverseSetBelongingOperator op,
                                                             const BigInteger &min, const BigInteger &max) {
    addConstraint(flatten({flatten(matrix), {rowIndex, colIndex}}));
}

void AbstractHypergraphDecompositionSolver::addElementMatrix(const vector<vector<string>> &matrix,
                                                             int startRowIndex, const string &rowIndex,
                                                             int startColIndex, const string &colIndex,
                                                             UniverseSetBelongingOperator op,
                                                             const vector<BigInteger> &set) {
    addConstraint(flatten({flatten(matrix), {rowIndex, colIndex}}));
}

void AbstractHypergraphDecompositionSolver::addPrecedence(const vector<string> &variables) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addPrecedence(const vector<string> &variables,
                                                          const vector<BigInteger> &values, bool covered) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addSupport(const string &variable,
                                                       const vector<BigInteger> &allowedValues,
                                                       bool hasStar) {
    addConstraint({variable});
}

void AbstractHypergraphDecompositionSolver::addSupportSymbolic(const string &variable,
                                                               const vector<string> &allowedValues, bool hasStar) {
    addConstraint({variable});
}

void AbstractHypergraphDecompositionSolver::addSupport(const vector<string> &variableTuple,
                                                       const vector<vector<BigInteger>> &allowedValues,
                                                       bool hasStar) {
    addConstraint(variableTuple);
}

void AbstractHypergraphDecompositionSolver::addSupportSymbolic(const vector<string> &variableTuple,
                                                               const vector<vector<string>> &allowedValues,
                                                               bool hasStar) {
    addConstraint(variableTuple);
}

void AbstractHypergraphDecompositionSolver::addConflicts(const string &variable,
                                                         const vector<BigInteger> &forbiddenValues,
                                                         bool hasStar) {
    addConstraint({variable});
}

void AbstractHypergraphDecompositionSolver::addConflictsSymbolic(const string &variable,
                                                                 const vector<string> &forbiddenValues,
                                                                 bool hasStar) {
    addConstraint({variable});
}

void AbstractHypergraphDecompositionSolver::addConflicts(const vector<string> &variableTuple,
                                                         const vector<vector<BigInteger>> &forbiddenValues,
                                                         bool hasStar) {
    addConstraint(variableTuple);
}

void AbstractHypergraphDecompositionSolver::addConflictsSymbolic(const vector<string> &variableTuple,
                                                                 const vector<vector<string>> &forbiddenValues,
                                                                 bool hasStar) {
    addConstraint(variableTuple);
}

void AbstractHypergraphDecompositionSolver::addIntension(IUniverseIntensionConstraint *constr) {
    addConstraint(scopeOf(constr));
}

void
AbstractHypergraphDecompositionSolver::addPrimitive(const string &variable, UniverseRelationalOperator op,
                                                    const BigInteger &value) {
    addConstraint({variable});
}

void AbstractHypergraphDecompositionSolver::addPrimitive(const string &variable,
                                                         UniverseArithmeticOperator arithOp,
                                                         const BigInteger &leftHandSide,
                                                         UniverseRelationalOperator relOp,
                                                         const BigInteger &rightHandSide) {
    addConstraint({variable});
}

void AbstractHypergraphDecompositionSolver::addPrimitive(const string &variable,
                                                         UniverseArithmeticOperator arithOp,
                                                         const string &leftHandSide,
                                                         UniverseRelationalOperator relOp,
                                                         const BigInteger &rightHandSide) {
    addConstraint({variable, leftHandSide});
}

void AbstractHypergraphDecompositionSolver::addPrimitive(const string &variable,
                                                         UniverseArithmeticOperator arithOp,
                                                         const BigInteger &leftHandSide,
                                                         UniverseRelationalOperator relOp,
                                                         const string &rightHandSide) {
    addConstraint({variable, rightHandSide});
}

void AbstractHypergraphDecompositionSolver::addPrimitive(const string &variable,
                                                         UniverseArithmeticOperator arithOp,
                                                         const string &leftHandSide,
                                                         UniverseRelationalOperator relOp,
                                                         const string &rightHandSide) {
    addConstraint({variable, leftHandSide, rightHandSide});
}

void AbstractHypergraphDecompositionSolver::addPrimitive(UniverseArithmeticOperator arithOp,
                                                         const string &variable, const string &rightHandSide) {
    addConstraint({variable, rightHandSide});
}

void
AbstractHypergraphDecompositionSolver::addPrimitive(const string &variable, UniverseSetBelongingOperator op,
                                                    const BigInteger &min, const BigInteger &max) {
    addConstraint({variable});
}

void
AbstractHypergraphDecompositionSolver::addPrimitive(const string &variable, UniverseSetBelongingOperator op,
                                                    const vector<BigInteger> &values) {
    addConstraint({variable});
}

void AbstractHypergraphDecompositionSolver::addMinimum(const vector<string> &variables,
                                                       UniverseRelationalOperator op,
                                                       const BigInteger &value) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addMinimum(const vector<string> &variables,
                                                       UniverseRelationalOperator op, const string &value) {
    addConstraint(flatten({variables, {value}}));
}

void AbstractHypergraphDecompositionSolver::addMinimum(const vector<string> &variables,
                                                       UniverseSetBelongingOperator op, const BigInteger &min,
                                                       const BigInteger &max) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addMinimum(const vector<string> &variables,
                                                       UniverseSetBelongingOperator op,
                                                       const vector<BigInteger> &set) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addMinimumIndex(const vector<string> &variables, int startIndex,
                                                            const string &index,
                                                            UniverseRelationalOperator op,
                                                            const BigInteger &value) {
    addConstraint(flatten({variables, {index}}));
}

void AbstractHypergraphDecompositionSolver::addMinimumIndex(const vector<string> &variables, int startIndex,
                                                            const string &index,
                                                            UniverseRelationalOperator op,
                                                            const string &value) {
    addConstraint(flatten({variables, {index, value}}));
}

void AbstractHypergraphDecompositionSolver::addMinimumIndex(const vector<string> &variables, int startIndex,
                                                            const string &index,
                                                            UniverseSetBelongingOperator op,
                                                            const BigInteger &min, const BigInteger &max) {
    addConstraint(flatten({variables, {index}}));
}

void AbstractHypergraphDecompositionSolver::addMinimumIndex(const vector<string> &variables, int startIndex,
                                                            const string &index,
                                                            UniverseSetBelongingOperator op,
                                                            const vector<BigInteger> &set) {
    addConstraint(flatten({variables, {index}}));
}

void AbstractHypergraphDecompositionSolver::addMinimumIntension(
        const vector<IUniverseIntensionConstraint *> &intensionConstraints,
        UniverseRelationalOperator op, const BigInteger &value) {
    addConstraint(scopeOf(intensionConstraints));
}

void AbstractHypergraphDecompositionSolver::addMinimumIntension(
        const vector<IUniverseIntensionConstraint *> &intensionConstraints,
        UniverseRelationalOperator op, const string &value) {
    addConstraint(flatten({scopeOf(intensionConstraints), {value}}));
}

void AbstractHypergraphDecompositionSolver::addMinimumIntension(
        const vector<IUniverseIntensionConstraint *> &intensionConstraints,
        UniverseSetBelongingOperator op, const BigInteger &min, const BigInteger &max) {
    addConstraint(scopeOf(intensionConstraints));
}

void AbstractHypergraphDecompositionSolver::addMinimumIntension(
        const vector<IUniverseIntensionConstraint *> &intensionConstraints,
        UniverseSetBelongingOperator op, const vector<BigInteger> &set) {
    addConstraint(scopeOf(intensionConstraints));
}

void AbstractHypergraphDecompositionSolver::addMinimumArg(const vector<string> &variables,
                                                          UniverseRelationalOperator op,
                                                          const BigInteger &value) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addMinimumArg(const vector<string> &variables,
                                                          UniverseRelationalOperator op,
                                                          const string &value) {
    addConstraint(flatten({variables, {value}}));
}

void AbstractHypergraphDecompositionSolver::addMinimumArg(const vector<string> &variables,
                                                          UniverseSetBelongingOperator op,
                                                          const BigInteger &min, const BigInteger &max) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addMinimumArg(const vector<string> &variables,
                                                          UniverseSetBelongingOperator op,
                                                          const vector<BigInteger> &set) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addMinimumArgIntension(
        const vector<IUniverseIntensionConstraint *> &intensionConstraints,
        UniverseRelationalOperator op, const BigInteger &value) {
    addConstraint(scopeOf(intensionConstraints));
}

void AbstractHypergraphDecompositionSolver::addMinimumArgIntension(
        const vector<IUniverseIntensionConstraint *> &intensionConstraints,
        UniverseRelationalOperator op, const string &value) {
    addConstraint(flatten({scopeOf(intensionConstraints), {value}}));
}

void AbstractHypergraphDecompositionSolver::addMinimumArgIntension(
        const vector<IUniverseIntensionConstraint *> &intensionConstraints,
        UniverseSetBelongingOperator op, const BigInteger &min, const BigInteger &max) {
    addConstraint(scopeOf(intensionConstraints));
}

void AbstractHypergraphDecompositionSolver::addMinimumArgIntension(
        const vector<IUniverseIntensionConstraint *> &intensionConstraints,
        UniverseSetBelongingOperator op, const vector<BigInteger> &set) {
    addConstraint(scopeOf(intensionConstraints));
}

void AbstractHypergraphDecompositionSolver::addMaximum(const vector<string> &variables,
                                                       UniverseRelationalOperator op,
                                                       const BigInteger &value) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addMaximum(const vector<string> &variables,
                                                       UniverseRelationalOperator op, const string &value) {
    addConstraint(flatten({variables, {value}}));
}

void AbstractHypergraphDecompositionSolver::addMaximum(const vector<string> &variables,
                                                       UniverseSetBelongingOperator op, const BigInteger &min,
                                                       const BigInteger &max) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addMaximum(const vector<string> &variables,
                                                       UniverseSetBelongingOperator op,
                                                       const vector<BigInteger> &set) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addMaximumIndex(const vector<string> &variables, int startIndex,
                                                            const string &index,
                                                            UniverseRelationalOperator op,
                                                            const BigInteger &value) {
    addConstraint(flatten({variables, {index}}));
}

void AbstractHypergraphDecompositionSolver::addMaximumIndex(const vector<string> &variables, int startIndex,
                                                            const string &index,
                                                            UniverseRelationalOperator op,
                                                            const string &value) {
    addConstraint(flatten({variables, {index, value}}));
}

void AbstractHypergraphDecompositionSolver::addMaximumIndex(const vector<string> &variables, int startIndex,
                                                            const string &index,
                                                            UniverseSetBelongingOperator op,
                                                            const BigInteger &min, const BigInteger &max) {
    addConstraint(flatten({variables, {index}}));
}

void AbstractHypergraphDecompositionSolver::addMaximumIndex(const vector<string> &variables, int startIndex,
                                                            const string &index,
                                                            UniverseSetBelongingOperator op,
                                                            const vector<BigInteger> &set) {
    addConstraint(flatten({variables, {index}}));
}

void AbstractHypergraphDecompositionSolver::addMaximumIntension(
        const vector<IUniverseIntensionConstraint *> &intensionConstraints,
        UniverseRelationalOperator op, const BigInteger &value) {
    addConstraint(scopeOf(intensionConstraints));
}

void AbstractHypergraphDecompositionSolver::addMaximumIntension(
        const vector<IUniverseIntensionConstraint *> &intensionConstraints,
        UniverseRelationalOperator op, const string &value) {
    addConstraint(flatten({scopeOf(intensionConstraints), {value}}));
}

void AbstractHypergraphDecompositionSolver::addMaximumIntension(
        const vector<IUniverseIntensionConstraint *> &intensionConstraints,
        UniverseSetBelongingOperator op, const BigInteger &min, const BigInteger &max) {
    addConstraint(scopeOf(intensionConstraints));
}

void AbstractHypergraphDecompositionSolver::addMaximumIntension(
        const vector<IUniverseIntensionConstraint *> &intensionConstraints,
        UniverseSetBelongingOperator op, const vector<BigInteger> &set) {
    addConstraint(scopeOf(intensionConstraints));
}

void AbstractHypergraphDecompositionSolver::addMaximumArg(const vector<string> &variables,
                                                          UniverseRelationalOperator op,
                                                          const BigInteger &value) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addMaximumArg(const vector<string> &variables,
                                                          UniverseRelationalOperator op,
                                                          const string &value) {
    addConstraint(flatten({variables, {value}}));
}

void AbstractHypergraphDecompositionSolver::addMaximumArg(const vector<string> &variables,
                                                          UniverseSetBelongingOperator op,
                                                          const BigInteger &min, const BigInteger &max) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addMaximumArg(const vector<string> &variables,
                                                          UniverseSetBelongingOperator op,
                                                          const vector<BigInteger> &set) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addMaximumArgIntension(
        const vector<IUniverseIntensionConstraint *> &intensionConstraints,
        UniverseRelationalOperator op, const BigInteger &value) {
    addConstraint(scopeOf(intensionConstraints));
}

void AbstractHypergraphDecompositionSolver::addMaximumArgIntension(
        const vector<IUniverseIntensionConstraint *> &intensionConstraints,
        UniverseRelationalOperator op, const string &value) {
    addConstraint(flatten({scopeOf(intensionConstraints), {value}}));
}

void AbstractHypergraphDecompositionSolver::addMaximumArgIntension(
        const vector<IUniverseIntensionConstraint *> &intensionConstraints,
        UniverseSetBelongingOperator op, const BigInteger &min, const BigInteger &max) {
    addConstraint(scopeOf(intensionConstraints));
}

void AbstractHypergraphDecompositionSolver::addMaximumArgIntension(
        const vector<IUniverseIntensionConstraint *> &intensionConstraints,
        UniverseSetBelongingOperator op, const vector<BigInteger> &set) {
    addConstraint(scopeOf(intensionConstraints));
}

void AbstractHypergraphDecompositionSolver::addNoOverlap(const vector<string> &variables,
                                                         const vector<BigInteger> &length) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addNoOverlap(const vector<string> &variables,
                                                         const vector<BigInteger> &length, bool zeroIgnored) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addNoOverlapVariableLength(const vector<string> &variables,
                                                                       const vector<string> &length) {
    addConstraint(flatten({variables, length}));
}

void AbstractHypergraphDecompositionSolver::addNoOverlapVariableLength(const vector<string> &variables,
                                                                       const vector<string> &length,
                                                                       bool zeroIgnored) {
    addConstraint(flatten({variables, length}));
}

void AbstractHypergraphDecompositionSolver::addBiDimensionalNoOverlap(const vector<string> &xVariables,
                                                                      const vector<string> &yVariables,
                                                                      const vector<string> &xLength,
                                                                      const vector<BigInteger> &yLength) {
    addConstraint(flatten({xVariables, yVariables, xLength}));
}

void AbstractHypergraphDecompositionSolver::addBiDimensionalNoOverlap(const vector<string> &xVariables,
                                                                      const vector<string> &yVariables,
                                                                      const vector<string> &xLength,
                                                                      const vector<BigInteger> &yLength,
                                                                      bool zeroIgnored) {
    addConstraint(flatten({xVariables, yVariables, xLength}));
}

void
AbstractHypergraphDecompositionSolver::addMultiDimensionalNoOverlap(const vector<vector<string>> &variables,
                                                                    const vector<vector<BigInteger>> &length) {
    addConstraint(flatten(variables));
}

void
AbstractHypergraphDecompositionSolver::addMultiDimensionalNoOverlap(const vector<vector<string>> &variables,
                                                                    const vector<vector<BigInteger>> &length,
                                                                    bool zeroIgnored) {
    addConstraint(flatten(variables));
}

void AbstractHypergraphDecompositionSolver::addMultiDimensionalNoOverlapVariableLength(
        const vector<vector<string>> &variables, const vector<vector<string>> &length) {
    addConstraint(flatten({flatten(variables), flatten(length)}));
}

void AbstractHypergraphDecompositionSolver::addMultiDimensionalNoOverlapVariableLength(
        const vector<vector<string>> &variables, const vector<vector<string>> &length,
        bool zeroIgnored) {
    addConstraint(flatten({flatten(variables), flatten(length)}));
}

void AbstractHypergraphDecompositionSolver::addOrdered(const vector<string> &variables,
                                                       UniverseRelationalOperator op) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addOrderedWithConstantLength(const vector<string> &variables,
                                                                         const vector<BigInteger> &lengths,
                                                                         UniverseRelationalOperator op) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addOrderedWithVariableLength(const vector<string> &variables,
                                                                         const vector<string> &lengths,
                                                                         UniverseRelationalOperator op) {
    addConstraint(flatten({variables, lengths}));
}

void AbstractHypergraphDecompositionSolver::addAllEqual(const vector<string> &variables) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addAllEqualIntension(
        const vector<IUniverseIntensionConstraint *> &expressions) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::addNotAllEqual(const vector<string> &variables) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addLex(const vector<vector<string>> &tuples,
                                                   UniverseRelationalOperator op) {
    addConstraint(flatten(tuples));
}

void AbstractHypergraphDecompositionSolver::addLex(const vector<vector<string>> &variables,
                                                   const vector<vector<BigInteger>> &limit,
                                                   UniverseRelationalOperator op) {
    addConstraint(flatten(variables));
}

void AbstractHypergraphDecompositionSolver::addLexMatrix(const vector<vector<string>> &matrix,
                                                         UniverseRelationalOperator op) {
    addConstraint(flatten(matrix));
}

void AbstractHypergraphDecompositionSolver::addSum(const vector<string> &variables,
                                                   UniverseRelationalOperator op, const BigInteger &value) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addSum(const vector<string> &variables,
                                                   UniverseRelationalOperator op,
                                                   const string &rightVariable) {
    addConstraint(flatten({variables, {rightVariable}}));
}

void AbstractHypergraphDecompositionSolver::addSum(const vector<string> &variables,
                                                   UniverseSetBelongingOperator op, const BigInteger &min,
                                                   const BigInteger &max) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addSum(const vector<string> &variables,
                                                   UniverseSetBelongingOperator op,
                                                   const vector<BigInteger> &values) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addSum(const vector<string> &variables,
                                                   const vector<BigInteger> &coefficients,
                                                   UniverseRelationalOperator op, const BigInteger &value) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addSum(const vector<string> &variables,
                                                   const vector<BigInteger> &coefficients,
                                                   UniverseRelationalOperator op,
                                                   const string &rightVariable) {
    addConstraint(flatten({variables, {rightVariable}}));
}

void AbstractHypergraphDecompositionSolver::addSum(const vector<string> &variables,
                                                   const vector<BigInteger> &coefficients,
                                                   UniverseSetBelongingOperator op, const BigInteger &min,
                                                   const BigInteger &max) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addSum(const vector<string> &variables,
                                                   const vector<BigInteger> &coefficients,
                                                   UniverseSetBelongingOperator op,
                                                   const vector<BigInteger> &values) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addSumIntension(
        const vector<IUniverseIntensionConstraint *> &expressions, UniverseRelationalOperator op,
        const BigInteger &value) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::addSumIntension(
        const vector<IUniverseIntensionConstraint *> &expressions, UniverseRelationalOperator op,
        const string &rightVariable) {
    addConstraint(flatten({scopeOf(expressions), {rightVariable}}));
}

void AbstractHypergraphDecompositionSolver::addSumIntension(
        const vector<IUniverseIntensionConstraint *> &expressions, UniverseSetBelongingOperator op,
        const BigInteger &min, const BigInteger &max) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::addSumIntension(
        const vector<IUniverseIntensionConstraint *> &expressions, UniverseSetBelongingOperator op,
        const vector<BigInteger> &values) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::addSumIntension(
        const vector<IUniverseIntensionConstraint *> &expressions,
        const vector<BigInteger> &coefficients, UniverseRelationalOperator op,
        const BigInteger &value) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::addSumIntension(
        const vector<IUniverseIntensionConstraint *> &expressions,
        const vector<BigInteger> &coefficients, UniverseRelationalOperator op,
        const string &rightVariable) {
    addConstraint(flatten({scopeOf(expressions), {rightVariable}}));
}

void AbstractHypergraphDecompositionSolver::addSumIntension(
        const vector<IUniverseIntensionConstraint *> &expressions,
        const vector<BigInteger> &coefficients, UniverseSetBelongingOperator op,
        const BigInteger &min, const BigInteger &max) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::addSumIntension(
        const vector<IUniverseIntensionConstraint *> &expressions,
        const vector<BigInteger> &coefficients, UniverseSetBelongingOperator op,
        const vector<BigInteger> &values) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::addSumWithVariableCoefficients(const vector<string> &variables,
                                                                           const vector<string> &coefficients,
                                                                           UniverseRelationalOperator op,
                                                                           const BigInteger &value) {
    addConstraint(flatten({variables, coefficients}));
}

void AbstractHypergraphDecompositionSolver::addSumWithVariableCoefficients(const vector<string> &variables,
                                                                           const vector<string> &coefficients,
                                                                           UniverseRelationalOperator op,
                                                                           const string &rightVariable) {
    addConstraint(flatten({variables, coefficients, {rightVariable}}));
}

void AbstractHypergraphDecompositionSolver::addSumWithVariableCoefficients(const vector<string> &variables,
                                                                           const vector<string> &coefficients,
                                                                           UniverseSetBelongingOperator op,
                                                                           const BigInteger &min,
                                                                           const BigInteger &max) {
    addConstraint(flatten({variables, coefficients}));
}

void AbstractHypergraphDecompositionSolver::addSumWithVariableCoefficients(const vector<string> &variables,
                                                                           const vector<string> &coefficients,
                                                                           UniverseSetBelongingOperator op,
                                                                           const vector<BigInteger> &values) {
    addConstraint(flatten({variables, coefficients}));
}

void AbstractHypergraphDecompositionSolver::addSumIntensionWithVariableCoefficients(
        const vector<IUniverseIntensionConstraint *> &expressions, const vector<string> &coefficients,
        UniverseRelationalOperator op, const BigInteger &value) {
    addConstraint(flatten({scopeOf(expressions), coefficients}));
}

void AbstractHypergraphDecompositionSolver::addSumIntensionWithVariableCoefficients(
        const vector<IUniverseIntensionConstraint *> &expressions, const vector<string> &coefficients,
        UniverseRelationalOperator op, const string &rightVariable) {
    addConstraint(flatten({scopeOf(expressions), coefficients, {rightVariable}}));
}

void AbstractHypergraphDecompositionSolver::addSumIntensionWithVariableCoefficients(
        const vector<IUniverseIntensionConstraint *> &expressions, const vector<string> &coefficients,
        UniverseSetBelongingOperator op, const BigInteger &min, const BigInteger &max) {
    addConstraint(flatten({scopeOf(expressions), coefficients}));
}

void AbstractHypergraphDecompositionSolver::addSumIntensionWithVariableCoefficients(
        const vector<IUniverseIntensionConstraint *> &expressions, const vector<string> &coefficients,
        UniverseSetBelongingOperator op, const vector<BigInteger> &values) {
    addConstraint(flatten({scopeOf(expressions), coefficients}));
}

void AbstractHypergraphDecompositionSolver::addCircuit(const vector<string> &variables, int startIndex) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addCircuit(const vector<string> &variables, int startIndex,
                                                       const BigInteger &size) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addCircuit(const vector<string> &variables, int startIndex,
                                                       const string &size) {
    addConstraint(flatten({variables, {size}}));
}

void AbstractHypergraphDecompositionSolver::addMDD(const vector<string> &variables,
                                                   const vector<UniverseTransition> &transitions) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addRegular(const vector<string> &variables,
                                                       const vector<UniverseTransition> &transitions,
                                                       const string &startState,
                                                       const vector<string> &finalStates) {
    addConstraint(flatten({variables, {startState}, finalStates}));
}

void AbstractHypergraphDecompositionSolver::addFlow(const vector<string> &variables,
                                                    const vector<BigInteger> &balance,
                                                    const vector<vector<BigInteger>> &edges) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addFlow(const vector<string> &variables,
                                                    const vector<BigInteger> &balance,
                                                    const vector<vector<BigInteger>> &edges,
                                                    const vector<BigInteger> &weights,
                                                    UniverseRelationalOperator op,
                                                    const BigInteger &totalCost) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addFlow(const vector<string> &variables,
                                                    const vector<BigInteger> &balance,
                                                    const vector<vector<BigInteger>> &edges,
                                                    const vector<BigInteger> &weights,
                                                    UniverseRelationalOperator op, const string &totalCost) {
    addConstraint(flatten({variables, {totalCost}}));
}

void AbstractHypergraphDecompositionSolver::addFlow(const vector<string> &variables,
                                                    const vector<BigInteger> &balance,
                                                    const vector<vector<BigInteger>> &edges,
                                                    const vector<BigInteger> &weights,
                                                    UniverseSetBelongingOperator op, const BigInteger &min,
                                                    const BigInteger &max) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::addFlow(const vector<string> &variables,
                                                    const vector<BigInteger> &balance,
                                                    const vector<vector<BigInteger>> &edges,
                                                    const vector<BigInteger> &weights,
                                                    UniverseSetBelongingOperator op,
                                                    const vector<BigInteger> &set) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::minimizeVariable(const string &variable) {
    addConstraint({variable});
}

void AbstractHypergraphDecompositionSolver::minimizeExpression(IUniverseIntensionConstraint *expression) {
    addConstraint(scopeOf(expression));
}

void AbstractHypergraphDecompositionSolver::maximizeVariable(const string &variable) {
    addConstraint({variable});
}

void AbstractHypergraphDecompositionSolver::maximizeExpression(IUniverseIntensionConstraint *expression) {
    addConstraint(scopeOf(expression));
}

void AbstractHypergraphDecompositionSolver::minimizeSum(const vector<string> &variables) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::minimizeSum(const vector<string> &variables,
                                                        const vector<BigInteger> &coefficients) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::minimizeExpressionSum(
        const vector<IUniverseIntensionConstraint *> &expressions) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::minimizeExpressionSum(
        const vector<IUniverseIntensionConstraint *> &expressions,
        const vector<BigInteger> &coefficients) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::maximizeSum(const vector<string> &variables) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::maximizeSum(const vector<string> &variables,
                                                        const vector<BigInteger> &coefficients) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::maximizeExpressionSum(
        const vector<IUniverseIntensionConstraint *> &expressions) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::maximizeExpressionSum(
        const vector<IUniverseIntensionConstraint *> &expressions,
        const vector<BigInteger> &coefficients) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::minimizeProduct(const vector<string> &variables) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::minimizeProduct(const vector<string> &variables,
                                                            const vector<BigInteger> &coefficients) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::minimizeExpressionProduct(
        const vector<IUniverseIntensionConstraint *> &expressions) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::minimizeExpressionProduct(
        const vector<IUniverseIntensionConstraint *> &expressions,
        const vector<BigInteger> &coefficients) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::maximizeProduct(const vector<string> &variables) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::maximizeProduct(const vector<string> &variables,
                                                            const vector<BigInteger> &coefficients) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::maximizeExpressionProduct(
        const vector<IUniverseIntensionConstraint *> &expressions) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::maximizeExpressionProduct(
        const vector<IUniverseIntensionConstraint *> &expressions,
        const vector<BigInteger> &coefficients) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::minimizeMinimum(const vector<string> &variables) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::minimizeMinimum(const vector<string> &variables,
                                                            const vector<BigInteger> &coefficients) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::minimizeExpressionMinimum(
        const vector<IUniverseIntensionConstraint *> &expressions) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::minimizeExpressionMinimum(
        const vector<IUniverseIntensionConstraint *> &expressions,
        const vector<BigInteger> &coefficients) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::maximizeMinimum(const vector<string> &variables) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::maximizeMinimum(const vector<string> &variables,
                                                            const vector<BigInteger> &coefficients) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::maximizeExpressionMinimum(
        const vector<IUniverseIntensionConstraint *> &expressions) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::maximizeExpressionMinimum(
        const vector<IUniverseIntensionConstraint *> &expressions,
        const vector<BigInteger> &coefficients) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::minimizeMaximum(const vector<string> &variables) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::minimizeMaximum(const vector<string> &variables,
                                                            const vector<BigInteger> &coefficients) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::minimizeExpressionMaximum(
        const vector<IUniverseIntensionConstraint *> &expressions) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::minimizeExpressionMaximum(
        const vector<IUniverseIntensionConstraint *> &expressions,
        const vector<BigInteger> &coefficients) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::maximizeMaximum(const vector<string> &variables) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::maximizeMaximum(const vector<string> &variables,
                                                            const vector<BigInteger> &coefficients) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::maximizeExpressionMaximum(
        const vector<IUniverseIntensionConstraint *> &expressions) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::maximizeExpressionMaximum(
        const vector<IUniverseIntensionConstraint *> &expressions,
        const vector<BigInteger> &coefficients) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::minimizeNValues(const vector<string> &variables) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::minimizeNValues(const vector<string> &variables,
                                                            const vector<BigInteger> &coefficients) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::minimizeExpressionNValues(
        const vector<IUniverseIntensionConstraint *> &expressions) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::minimizeExpressionNValues(
        const vector<IUniverseIntensionConstraint *> &expressions,
        const vector<BigInteger> &coefficients) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::maximizeNValues(const vector<string> &variables) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::maximizeNValues(const vector<string> &variables,
                                                            const vector<BigInteger> &coefficients) {
    addConstraint(variables);
}

void AbstractHypergraphDecompositionSolver::maximizeExpressionNValues(
        const vector<IUniverseIntensionConstraint *> &expressions) {
    addConstraint(scopeOf(expressions));
}

void AbstractHypergraphDecompositionSolver::maximizeExpressionNValues(
        const vector<IUniverseIntensionConstraint *> &expressions,
        const vector<BigInteger> &coefficients) {
    addConstraint(scopeOf(expressions));
}

vector<string>
AbstractHypergraphDecompositionSolver::toString(vector<int> literals) {
    vector<string> scope;
    for (int l : literals) {
        scope.push_back(to_string(abs(l)));
    }
    return scope;
}

vector<string>
AbstractHypergraphDecompositionSolver::scopeOf(vector<IUniverseIntensionConstraint *> constraints) {
    vector<vector<string>> scopes;
    for (auto & constr : constraints) {
        scopes.emplace_back(scopeOf(constr));
    }
    return flatten(scopes);
}

vector<string>
AbstractHypergraphDecompositionSolver::scopeOf(IUniverseIntensionConstraint *constraint) {
    ScopeIntensionConstraintVisitor visitor;
    constraint->accept(&visitor);
    return visitor.getScope();
}

vector<string>
AbstractHypergraphDecompositionSolver::flatten(const vector<vector<string>> &scopes) {
    vector<string> flat;
    for (auto &scope : scopes) {
        flat.insert(flat.end(), scope.begin(), scope.end());
    }
    return flat;
}

void AbstractHypergraphDecompositionSolver::addConstraint(const vector<string> &scope) {
    for (auto &id : scope) {
        constraintsWithVariables[id].insert(constrId);
    }
    constrId++;
}

Hypergraph *AbstractHypergraphDecompositionSolver::getHypergraph() {
    HypergraphBuilder *builder = HypergraphBuilder::createHypergraph(constrId - 1, constraintsWithVariables.size());
    for(auto &entry : constraintsWithVariables) {
        auto nb = entry.second.size();
        int *vertices = new int[nb];
        int i = 0;
        for (int ctr : entry.second) {
            vertices[i] = ctr;
            orderedVariables.push_back(entry.first);
            i++;
        }
        builder->withHyperedge(UnweightedHyperedge::joining(nb, vertices));
    }
    return builder->build();

}
