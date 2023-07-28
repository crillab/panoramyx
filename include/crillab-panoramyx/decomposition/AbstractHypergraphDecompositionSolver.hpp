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
 * @file AbstractHypergraphDecompositionSolver.hpp
 * @brief Provides a base implementation of IHypergraphDecompositionSolver
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_ABSTRACTHYPERGRAPHDECOMPOSITIONSOLVER_HPP
#define PANORAMYX_ABSTRACTHYPERGRAPHDECOMPOSITIONSOLVER_HPP

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "../utils/hypergraph/Hypergraph.hpp"

#include "IHypergraphDecompositionSolver.hpp"
#include "crillab-panoramyx/utils/hypergraph/HypergraphBuilder.hpp"

namespace Panoramyx {

    /**
     * The AbstractHypergraphDecompositionSolver is the base class for implementations of
     * IHypergraphDecompositionSolver.
     */
    class AbstractHypergraphDecompositionSolver : public Panoramyx::IHypergraphDecompositionSolver {

    private:

        /**
         * The identifier of the current constraint.
         */
        int constrId = 1;

        /**
         * The dual hypergraph of the problem to solve.
         */
        Panoramyx::Hypergraph *hypergraph;

    protected:

        /**
         * The map associating each variable identifier to the indices of the constraints in which it appears.
         */
        std::unordered_map<std::string, std::set<int>> constraintsWithVariables;

        /**
         * The variables in the order in which they are added to the hypergraph.
         */
        std::vector<std::string> orderedVariables;

    public:

        /**
         * Resets this solver in its original state.
         */
        void reset() override;

        /**
         * Gives the number of variables defined in this solver.
         *
         * @return The number of variables.
         */
        int nVariables() override;

        /**
         * Gives the mapping of the variables in this solver.
         *
         * @return The mapping of the variables.
         */
        [[nodiscard]] const std::map<std::string, Universe::IUniverseVariable *> &getVariablesMapping() override;

        /**
         * Gives the vector of the auxiliary variables used by the solver.
         * These variables are those that the solver defines to help it represent the
         * problem (for instance, to reify constraints).
         *
         * @return The list of the auxiliary variables, given by their name.
         */
        const std::vector<std::string> &getAuxiliaryVariables() override;

        /**
         * Gives the list of the constraints in this solver.
         *
         * @return The list of the constraints.
         */
        const std::vector<Universe::IUniverseConstraint*> &getConstraints() override;

        /**
         * Advises this solver to focus on some variables to make decisions.
         *
         * @param variables The variables on which to make decisions.
         */
        void decisionVariables(const std::vector<std::string> &variables) override;

        /**
         * Forces a static order on the values to try for some variables.
         *
         * @param variables The variables for which a static order is set.
         * @param orderedValues The values to try for the specified variables, in the desired
         *        order.
         */
        void valueHeuristicStatic(const std::vector<std::string> &variables,
                                  const std::vector<Universe::BigInteger> &orderedValues) override;

        /**
         * Gives the number of constraints defined in this solver.
         *
         * @return The number of constraints.
         */
        int nConstraints() override;

        /**
         * Checks whether the associated problem is an optimization problem.
         *
         * @return Whether the problem is an optimization problem.
         */
        bool isOptimization() override;

        /**
         * Sets the time limit before interrupting the search.
         *
         * @param seconds The time limit to set (in seconds).
         */
        void setTimeout(long seconds) override;

        /**
         * Sets the time limit before interrupting the search.
         *
         * @param seconds The time limit to set (in milli-seconds).
         */
        void setTimeoutMs(long mseconds) override;

        /**
         * Sets the verbosity level of the solver.
         *
         * @param level The verbosity level to set.
         */
        void setVerbosity(int level) override;

        /**
         * Adds a listener to this solver, which listens to the events occurring in
         * the solver during the search.
         *
         * @param listener The listener to add.
         */
        void addSearchListener(Universe::IUniverseSearchListener *listener) override;

        /**
         * Removes a listener from this solver, so that the listener does not listen to
         * the events occurring in the solver during the search anymore.
         *
         * @param listener The listener to remove.
         */
        void removeSearchListener(Universe::IUniverseSearchListener *listener) override;

        /**
         * Sets the log file to be used by the solver.
         *
         * @param filename The name of the log file.
         */
        void setLogFile(const std::string &filename) override;

        /**
         * Sets the output stream to be used by the solver for logging.
         *
         * @param stream The logging output stream.
         */
        void setLogStream(std::ostream &stream) override;

        /**
         * Loads a problem stored in the given file.
         * The solver is expected to parse the problem itself.
         *
         * @param filename The name of the file containing the problem to solve.
         */
        void loadInstance(const std::string &filename) override;

        /**
         * Interrupts (asynchronously) the search currently performed by this solver.
         */
        void interrupt() override;

        /**
         * Gives the solution found by this solver (if any).
         *
         * @return The solution found by this solver.
         */
        std::vector<Universe::BigInteger> solution() override;

        /**
         * Gives the mapping between the names of the variables and the assignment found
         * by this solver (if any).
         *
         * @return The solution found by this solver.
         */
        std::map<std::string, Universe::BigInteger> mapSolution() override;

        /**
         * Gives the mapping between the names of the variables and the assignment found
         * by this solver (if any).
         *
         * @param excludeAux Whether auxiliary variables should be excluded from the solution.
         *
         * @return The solution found by this solver.
         */
        std::map<std::string, Universe::BigInteger> mapSolution(bool excludeAux) override;

        /**
         * Checks the last solution that has been computed by the solver.
         * Said differently, this method ensures that the last solution satisfies all the
         * constraints of the problem solved by the solver.
         *
         * @return Whether the last solution is correct.
         */
        bool checkSolution() override;

        /**
         * Checks whether the given assignment is a solution of the problem.
         * Said differently, this method ensures that the given assignment satisfies all the
         * constraints of the problem solved by the solver.
         *
         * @param assignment The assignment to check as a solution.
         *
         * @return Whether the given assignment is a solution of the problem.
         */
        bool checkSolution(const std::map<std::string, Universe::BigInteger> &assignment) override;

        /**
         * Casts this solver into an IOptimizationSolver.
         *
         * @return The casted optimization solver.
         */
        Universe::IOptimizationSolver *toOptimizationSolver() override;

        /**
         * Adds a new variable to this solver.
         *
         * @param id The identifier of the variable to create.
         * @param min The minimum value of the domain of the variable.
         * @param max The maximum value of the domain of the variable.
         */
        void newVariable(const std::string &id, int min, int max) override;

        /**
         * Adds a new variable to this solver.
         *
         * @param id The identifier of the variable to create.
         * @param min The minimum value of the domain of the variable.
         * @param max The maximum value of the domain of the variable.
         */
        void newVariable(
                const std::string &id, const Universe::BigInteger &min, const Universe::BigInteger &max) override;

        /**
         * Adds a new variable to this solver.
         *
         * @param id The identifier of the variable to create.
         * @param values The values of the domain of the variable.
         */
        void newVariable(const std::string &id, const std::vector<int> &values) override;

        /**
         * Adds a new variable to this solver.
         *
         * @param id The identifier of the variable to create.
         * @param values The values of the domain of the variable.
         */
        void newVariable(const std::string &id, const std::vector<Universe::BigInteger> &values) override;

        /**
         * Adds a new symbolic variable to this solver.
         *
         * @param id The identifier of the variable to create.
         * @param values The values of the domain of the variable.
         */
        void newVariableSymbolic(const std::string &id, const std::vector<std::string> &values) override;

        /**
         * Adds to this solver a clause from a set of literals.
         * The literals are represented by non-zero integers such that opposite literals
         * are represented by opposite values (following the classical DIMACS way of
         * representing literals).
         *
         * @param literals The literals of the clause to add.
         *
         * @throws UniverseContradictionException If the clause to add is inconsistent.
         */
        void addClause(const std::vector<int> &literals) override;

        /**
         * Creates a pseudo-Boolean constraint of type at-least or at-most.
         *
         * @param literals The literals of the constraint to add.
         * @param coefficients The coefficients of the literals.
         * @param moreThan Whether the constraint is an at-least constraint, or an at-most constraint.
         * @param degree The degree of the constraint.
         *
         * @throws UniverseContradictionException If the constraint to add is inconsistent.
         */
        void addPseudoBoolean(const std::vector<int> &literals, const std::vector<Universe::BigInteger> &coefficients,
                              bool moreThan, const Universe::BigInteger &degree) override;

        /**
         * Creates an at-most cardinality constraint.
         *
         * @param literals The literals of the constraint to add.
         * @param degree The degree of the constraint.
         *
         * @throws UniverseContradictionException If the constraint to add is inconsistent.
         */
        void addAtMost(const std::vector<int> &literals, int degree) override;

        /**
         * Creates an at-most pseudo-Boolean constraint.
         *
         * @param literals The literals of the constraint to add.
         * @param coefficients The coefficients of the literals.
         * @param degree The degree of the constraint.
         *
         * @throws UniverseContradictionException If the constraint to add is inconsistent.
         */
        void addAtMost(
                const std::vector<int> &literals, const std::vector<int> &coefficients, int degree) override;

        /**
         * Creates an at-most pseudo-Boolean constraint.
         *
         * @param literals The literals of the constraint to add.
         * @param coefficients The coefficients of the literals.
         * @param degree The degree of the constraint.
         *
         * @throws UniverseContradictionException If the constraint to add is inconsistent.
         */
        void addAtMost(const std::vector<int> &literals,
                       const std::vector<Universe::BigInteger> &coefficients, const Universe::BigInteger &degree) override;

        /**
         * Creates an at-least cardinality constraint.
         *
         * @param literals The literals of the constraint to add.
         * @param degree The degree of the constraint.
         *
         * @throws UniverseContradictionException If the constraint to add is inconsistent.
         */
        void addAtLeast(const std::vector<int> &literals, int degree) override;

        /**
         * Creates an at-least pseudo-Boolean constraint.
         *
         * @param literals The literals of the constraint to add.
         * @param coefficients The coefficients of the literals.
         * @param degree The degree of the constraint.
         *
         * @throws UniverseContradictionException If the constraint to add is inconsistent.
         */
        void addAtLeast(
                const std::vector<int> &literals, const std::vector<int> &coefficients, int degree) override;

        /**
         * Creates an at-least pseudo-Boolean constraint.
         *
         * @param literals The literals of the constraint to add.
         * @param coefficients The coefficients of the literals.
         * @param degree The degree of the constraint.
         *
         * @throws UniverseContradictionException If the constraint to add is inconsistent.
         */
        void addAtLeast(const std::vector<int> &literals,
                        const std::vector<Universe::BigInteger> &coefficients, const Universe::BigInteger &degree) override;

        /**
         * Creates an exactly cardinality constraint.
         *
         * @param literals The literals of the constraint to add.
         * @param degree The degree of the constraint.
         *
         * @throws UniverseContradictionException If the constraint to add is inconsistent.
         */
        void addExactly(const std::vector<int> &literals, int degree) override;

        /**
         * Creates an exactly pseudo-Boolean constraint.
         *
         * @param literals The literals of the constraint to add.
         * @param coefficients The coefficients of the literals.
         * @param degree The degree of the constraint.
         *
         * @throws UniverseContradictionException If the constraint to add is inconsistent.
         */
        void addExactly(
                const std::vector<int> &literals, const std::vector<int> &coefficients, int degree) override;

        /**
         * Creates an exactly pseudo-Boolean constraint.
         *
         * @param literals The literals of the constraint to add.
         * @param coefficients The coefficients of the literals.
         * @param degree The degree of the constraint.
         *
         * @throws UniverseContradictionException If the constraint to add is inconsistent.
         */
        void addExactly(const std::vector<int> &literals,
                        const std::vector<Universe::BigInteger> &coefficients, const Universe::BigInteger &degree) override;


        /**
         * Adds to this solver an instantiation constraint.
         *
         * @param variable The variable to assign.
         * @param value The value to assign to the variable.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addInstantiation(const std::string &variable, int value) override;

        /**
         * Adds to this solver an instantiation constraint.
         *
         * @param variable The variable to assign.
         * @param value The value to assign to the variable.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addInstantiation(const std::string &variable, const Universe::BigInteger &value) override;

        /**
         * Adds to this solver an instantiation constraint.
         *
         * @param variable The variable to assign.
         * @param value The value to assign to the variable.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addInstantiation(const std::string &variable, const std::string &value) override;

        /**
         * Adds to this solver an instantiation constraint.
         *
         * @param variables The variables to assign.
         * @param values The values to assign to the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addInstantiation(const std::vector<std::string> &variables, const std::vector<int> &values) override;

        /**
         * Adds to this solver an instantiation constraint.
         *
         * @param variables The variables to assign.
         * @param values The values to assign to the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addInstantiation(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &values) override;

        /**
         * Adds to this solver an instantiation constraint.
         *
         * @param variables The variables to assign.
         * @param values The values to assign to the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addInstantiationSymbolic(
                const std::vector<std::string> &variables, const std::vector<std::string> &values) override;

        /**
         * Adds to this solver a clause constraint.
         *
         * @param positive The (Boolean) variables appearing positively in the clause.
         * @param negative The (Boolean) variables appearing negatively in the clause.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addClause(const std::vector<std::string> &positive, const std::vector<std::string> &negative) override;

        /**
         * Adds to this solver a logical constraint.
         *
         * @param op The Boolean operator to apply on the variables.
         * @param variables The (Boolean) variables on which the operator is applied.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addLogical(Universe::UniverseBooleanOperator op, const std::vector<std::string> &variables) override;

        /**
         * Adds to this solver a logical constraint.
         *
         * @param variable The (Boolean) variable whose assignment depends on the truth value
         *        of the logical operation.
         * @param equiv Whether variable must be equivalent to the truth value of the
         *        logical operation.
         * @param op The Boolean operator to apply on the variables.
         * @param variables The (Boolean) variables on which the operator is applied.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addLogical(const std::string &variable, bool equiv, Universe::UniverseBooleanOperator op,
                        const std::vector<std::string> &variables) override;

        /**
         * Adds to this solver a logical constraint.
         *
         * @param variable The (Boolean) variable whose assignment depends on the truth value
         *        of the comparison between left and right.
         * @param left The variable on the left-hand side of the comparison.
         * @param op The relational operator used to compare left and right.
         * @param right The value on the right-hand side of the comparison.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addLogical(const std::string &variable, const std::string &left,
                        Universe::UniverseRelationalOperator op, const Universe::BigInteger &right) override;

        /**
         * Adds to this solver a logical constraint.
         *
         * @param variable The (Boolean) variable whose assignment depends on the truth value
         *        of the comparison between left and right.
         * @param left The variable on the left-hand side of the comparison.
         * @param op The relational operator used to compare left and right.
         * @param right The variable on the right-hand side of the comparison.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addLogical(const std::string &variable, const std::string &left,
                        Universe::UniverseRelationalOperator op, const std::string &right) override;

        /**
         * Adds to this solver an all-different constraint.
         *
         * @param variables The variables that should all be different.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addAllDifferent(const std::vector<std::string> &variables) override;

        /**
         * Adds to this solver an all-different constraint.
         *
         * @param variables The variables that should all be different.
         * @param except The values not to consider in the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addAllDifferent(const std::vector<std::string> &variables,
                             const std::vector<Universe::BigInteger> &except) override;

        /**
         * Adds to this solver an all-different constraint.
         *
         * @param variableMatrix The matrix of variables that should all be different.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addAllDifferentMatrix(const std::vector<std::vector<std::string>> &variableMatrix) override;

        /**
         * Adds to this solver an all-different constraint.
         *
         * @param variableMatrix The matrix of variables that should all be different.
         * @param except The values not to consider in the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addAllDifferentMatrix(const std::vector<std::vector<std::string>> &variableMatrix,
                                   const std::vector<Universe::BigInteger> &except) override;

        /**
         * Adds to this solver an all-different constraint.
         *
         * @param variableLists The lists of variables that should all be different.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addAllDifferentList(const std::vector<std::vector<std::string>> &variableLists) override;

        /**
         * Adds to this solver an all-different constraint.
         *
         * @param variableLists The lists of variables that should all be different.
         * @param except The values not to consider in the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addAllDifferentList(const std::vector<std::vector<std::string>> &variableLists,
                                 const std::vector<std::vector<Universe::BigInteger>> &except) override;

        /**
         * Adds to this solver an all-different constraint.
         *
         * @param intensionConstraints The intension constraints that should all be different.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addAllDifferentIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &intensionConstraints) override;

        /**
         * Adds to this solver a channel constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param startIndex The index at which the constraint starts.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addChannel(const std::vector<std::string> &variables, int startIndex) override;

        /**
         * Adds to this solver a channel constraint.
         *
         * @param variables The variables among which exactly one should be satisfied starting from the given index.
         * @param startIndex The index at which the constraint starts.
         * @param value The variable containing the index of the satisfied variable.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addChannel(
                const std::vector<std::string> &variables, int startIndex, const std::string &value) override;

        /**
         * Adds to this solver a channel constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param startIndex The index at which the constraint starts on the first vector of variables.
         * @param otherVariables The variables with which to channel the variables of the first vector.
         * @param otherStartIndex The index at which the constraint starts on the second vector of variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addChannel(const std::vector<std::string> &variables, int startIndex,
                        const std::vector<std::string> &otherVariables, int otherStartIndex) override;

        /**
         * Adds to this solver a cardinality constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param values The assignable values to count.
         * @param occurs The number of times each value can be assigned.
         * @param closed Whether only the values in value can be assigned to the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCardinalityWithConstantValuesAndConstantCounts(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &values,
                const std::vector<Universe::BigInteger> &occurs, bool closed) override;

        /**
         * Adds to this solver a cardinality constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param values The assignable values to count.
         * @param occursMin The minimum number of times each value can be assigned.
         * @param occursMax The maximum number of times each value can be assigned.
         * @param closed Whether only the values in value can be assigned to the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCardinalityWithConstantValuesAndConstantIntervalCounts(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &values,
                const std::vector<Universe::BigInteger> &occursMin, const std::vector<Universe::BigInteger> &occursMax,
                bool closed) override;

        /**
         * Adds to this solver a cardinality constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param values The assignable values to count.
         * @param occurs The variables encoding the number of times each value can be assigned.
         * @param closed Whether only the values in value can be assigned to the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCardinalityWithConstantValuesAndVariableCounts(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &values,
                const std::vector<std::string> &occurs, bool closed) override;

        /**
         * Adds to this solver a cardinality constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param values The variables encoding the assignable values to count.
         * @param occurs The number of times each value can be assigned.
         * @param closed Whether only the values in value can be assigned to the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCardinalityWithVariableValuesAndConstantCounts(
                const std::vector<std::string> &variables, const std::vector<std::string> &values,
                const std::vector<Universe::BigInteger> &occurs, bool closed) override;

        /**
         * Adds to this solver a cardinality constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param values The variables encoding the assignable values to count.
         * @param occursMin The minimum number of times each value can be assigned.
         * @param occursMax The maximum number of times each value can be assigned.
         * @param closed Whether only the values in value can be assigned to the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCardinalityWithVariableValuesAndConstantIntervalCounts(
                const std::vector<std::string> &variables, const std::vector<std::string> &values,
                const std::vector<Universe::BigInteger> &occursMin, const std::vector<Universe::BigInteger> &occursMax,
                bool closed) override;

        /**
         * Adds to this solver a cardinality constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param values The variables encoding the assignable values to count.
         * @param occurs The variables encoding the number of times each value can be assigned.
         * @param closed Whether only the values in value can be assigned to the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCardinalityWithVariableValuesAndVariableCounts(
                const std::vector<std::string> &variables, const std::vector<std::string> &values,
                const std::vector<std::string> &occurs, bool closed) override;

        /**
         * Adds to this solver an at-least constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param value The value to count the assignments of.
         * @param count The minimum number of times the value can be assigned among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addAtLeast(const std::vector<std::string> &variables,
                        const Universe::BigInteger &value, const Universe::BigInteger &count) override;

        /**
         * Adds to this solver an exactly constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param value The value to count the assignments of.
         * @param count The number of times the value can be assigned among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addExactly(const std::vector<std::string> &variables,
                        const Universe::BigInteger &value, const Universe::BigInteger &count) override;

        /**
         * Adds to this solver an exactly constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param value The value to count the assignments of.
         * @param count The variable encoding the number of times the value can be assigned among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addExactly(const std::vector<std::string> &variables,
                        const Universe::BigInteger &value, const std::string &count) override;

        /**
         * Adds to this solver an among constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param values The values to count the assignments of.
         * @param count The number of times the value can be assigned among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addAmong(const std::vector<std::string> &variables,
                      const std::vector<Universe::BigInteger> &values, const Universe::BigInteger &count) override;

        /**
         * Adds to this solver an among constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param values The values to count the assignments of.
         * @param count The variable encoding the number of times the values can be assigned among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addAmong(const std::vector<std::string> &variables,
                      const std::vector<Universe::BigInteger> &values, const std::string &count) override;

        /**
         * Adds to this solver an at-most constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param value The value to count the assignments of.
         * @param count The maximum number of times the value can be assigned among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addAtMost(const std::vector<std::string> &variables,
                       const Universe::BigInteger &value, const Universe::BigInteger &count) override;

        /**
         * Adds to this solver a count constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param values The values to count the assignments of.
         * @param op The operator to use to compare the number of assignments to their expected count.
         * @param count The number of times the values can be assigned among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCountWithConstantValues(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &values,
                Universe::UniverseRelationalOperator op, const Universe::BigInteger &count) override;

        /**
         * Adds to this solver a count constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param values The values to count the assignments of.
         * @param op The operator to use to compare the number of assignments to their expected count.
         * @param count The variable encoding the number of times the values can be assigned among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCountWithConstantValues(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &values,
                Universe::UniverseRelationalOperator op, const std::string &count) override;

        /**
         * Adds to this solver a count constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param values The values to count the assignments of.
         * @param op The operator to use to check whether the number of assignments is within a range.
         * @param min The minimum number of times the value can be assigned among the variables.
         * @param max The maximum number of times the value can be assigned among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCountWithConstantValues(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &values,
                Universe::UniverseSetBelongingOperator op, const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a count constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param values The values to count the assignments of.
         * @param op The operator to use to check whether the number of assignments is within a set.
         * @param set The set containing the allowed numbers of times the values can be assigned among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCountWithConstantValues(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &values,
                Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a count constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param values The variables encoding the values to count the assignments of.
         * @param op The operator to use to compare the number of assignments to their expected count.
         * @param count The number of times the values can be assigned among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCountWithVariableValues(
                const std::vector<std::string> &variables, const std::vector<std::string> &values,
                Universe::UniverseRelationalOperator op, const Universe::BigInteger &count) override;

        /**
         * Adds to this solver a count constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param values The variables encoding the values to count the assignments of.
         * @param op The operator to use to compare the number of assignments to their expected count.
         * @param count The variable encoding the number of times the values can be assigned among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCountWithVariableValues(
                const std::vector<std::string> &variables, const std::vector<std::string> &values,
                Universe::UniverseRelationalOperator op, const std::string &count) override;


        /**
         * Adds to this solver a count constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param values The variables encoding the values to count the assignments of.
         * @param op The operator to use to check whether the number of assignments is within a range.
         * @param min The minimum number of times the value can be assigned among the variables.
         * @param max The maximum number of times the value can be assigned among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCountWithVariableValues(
                const std::vector<std::string> &variables, const std::vector<std::string> &values,
                Universe::UniverseSetBelongingOperator op, const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a count constraint.
         *
         * @param variables The variables to count the assignments of.
         * @param values The variables encoding the values to count the assignments of.
         * @param op The operator to use to check whether the number of assignments is within a set.
         * @param set The set containing the allowed numbers of times the values can be assigned among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCountWithVariableValues(
                const std::vector<std::string> &variables, const std::vector<std::string> &values,
                Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a count constraint.
         *
         * @param expressions The expressions to count the assignments of.
         * @param values The values to count the assignments of.
         * @param op The operator to use to compare the number of assignments to their expected count.
         * @param count The variable encoding the number of times the values can be assigned among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCountIntensionWithConstantValues(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<Universe::BigInteger> &values,
                Universe::UniverseRelationalOperator op,
                const Universe::BigInteger &count) override;

        /**
         * Adds to this solver a count constraint.
         *
         * @param expressions The expressions to count the assignments of.
         * @param values The values to count the assignments of.
         * @param op The operator to use to compare the number of assignments to their expected count.
         * @param count The variable encoding the number of times the values can be assigned among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCountIntensionWithConstantValues(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<Universe::BigInteger> &values,
                Universe::UniverseRelationalOperator op,
                const std::string &count) override;

        /**
         * Adds to this solver a count constraint.
         *
         * @param expressions The expressions to count the assignments of.
         * @param values The values to count the assignments of.
         * @param op The operator to use to check whether the number of assignments is within a range.
         * @param min The minimum number of times the value can be assigned among the variables.
         * @param max The maximum number of times the value can be assigned among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCountIntensionWithConstantValues(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<Universe::BigInteger> &values,
                Universe::UniverseSetBelongingOperator op,
                const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a count constraint.
         *
         * @param expressions The expressions to count the assignments of.
         * @param values The values to count the assignments of.
         * @param op The operator to use to check whether the number of assignments is within a set.
         * @param set The set containing the allowed numbers of times the values can be assigned among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCountIntensionWithConstantValues(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<Universe::BigInteger> &values,
                Universe::UniverseSetBelongingOperator op,
                const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver an n-values constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param op The relational operator used in the constraint.
         * @param nb The number of distinct values to count.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addNValues(const std::vector<std::string> &variables,
                        Universe::UniverseRelationalOperator op, const Universe::BigInteger &nb) override;

        /**
         * Adds to this solver an n-values constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param op The relational operator used in the constraint.
         * @param nb The number of distinct values to count.
         * @param except The values that should not be counted.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addNValuesExcept(const std::vector<std::string> &variables,
                              Universe::UniverseRelationalOperator op, const Universe::BigInteger &nb,
                              const std::vector<Universe::BigInteger> &except) override;

        /**
         * Adds to this solver an n-values constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param op The relational operator used in the constraint.
         * @param nb The variable counting the number of distinct values.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addNValues(const std::vector<std::string> &variables,
                        Universe::UniverseRelationalOperator op, const std::string &nb) override;

        /**
         * Adds to this solver an n-values constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param op The relational operator used in the constraint.
         * @param nb The variable counting the number of distinct values.
         * @param except The values that should not be counted.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addNValuesExcept(
                const std::vector<std::string> &variables, Universe::UniverseRelationalOperator op,
                const std::string &nb, const std::vector<Universe::BigInteger> &except) override;

        /**
         * Adds to this solver an n-values constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param set The set operator used in the constraint.
         * @param min The minimum number of distinct values to count.
         * @param max The maximum number of distinct values to count.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addNValues(
                const std::vector<std::string> &variables,
                Universe::UniverseSetBelongingOperator op,
                const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver an n-values constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param set The set operator used in the constraint.
         * @param min The minimum number of distinct values to count.
         * @param max The maximum number of distinct values to count.
         * @param except The values that should not be counted.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addNValuesExcept(
                const std::vector<std::string> &variables,
                Universe::UniverseSetBelongingOperator op,
                const Universe::BigInteger &min,
                const Universe::BigInteger &max,
                const std::vector<Universe::BigInteger> &except) override;

        /**
         * Adds to this solver an n-values constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param op The set operator used in the constraint.
         * @param set The allowed numbers of distinct values.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addNValues(
                const std::vector<std::string> &variables,
                Universe::UniverseSetBelongingOperator op,
                const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver an n-values constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param op The set operator used in the constraint.
         * @param set The allowed numbers of distinct values.
         * @param except The values that should not be counted.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addNValuesExcept(
                const std::vector<std::string> &variables,
                Universe::UniverseSetBelongingOperator op,
                const std::vector<Universe::BigInteger> &set,
                const std::vector<Universe::BigInteger> &except) override;

        /**
         * Adds to this solver an n-values constraint.
         *
         * @param expressions The expressions appearing in the constraint.
         * @param op The relational operator used in the constraint.
         * @param nb The number of distinct values to count.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addNValuesIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                Universe::UniverseRelationalOperator op, const Universe::BigInteger &nb) override;

        /**
         * Adds to this solver an n-values constraint.
         *
         * @param expressions The expressions appearing in the constraint.
         * @param op The relational operator used in the constraint.
         * @param nb The variable counting the number of distinct values.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addNValuesIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                Universe::UniverseRelationalOperator op, const std::string &nb) override;

        /**
         * Adds to this solver an n-values constraint.
         *
         * @param expressions The expressions appearing in the constraint.
         * @param op The set operator used in the constraint.
         * @param min The minimum number of distinct values to count.
         * @param max The maximum number of distinct values to count.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addNValuesIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                Universe::UniverseSetBelongingOperator op,
                const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver an n-values constraint.
         *
         * @param expressions The expressions appearing in the constraint.
         * @param op The set operator used in the constraint.
         * @param set The allowed numbers of distinct values.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addNValuesIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                Universe::UniverseSetBelongingOperator op,
                const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a bin-packing constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param sizes The sizes of the elements to pack.
         * @param op The operator used to ensure the capacity of the bin.
         * @param value The value of the bins capacity.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addBinPacking(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &sizes,
                Universe::UniverseRelationalOperator op,
                const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a bin-packing constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param sizes The sizes of the elements to pack.
         * @param op The operator used to ensure the capacity of the bins.
         * @param variable The variable encoding the bins capacity.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addBinPacking(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &sizes,
                Universe::UniverseRelationalOperator op,
                const std::string &variable) override;

        /**
         * Adds to this solver a bin-packing constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param sizes The sizes of the elements to pack.
         * @param op The operator used to ensure the capacity of the bins.
         * @param min The minimum capacity of the bins.
         * @param max The maximum capacity of the bins.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addBinPacking(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &sizes,
                Universe::UniverseSetBelongingOperator op,
                const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a bin-packing constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param sizes The sizes of the elements to pack.
         * @param op The operator used to ensure the capacity of the bins.
         * @param set The allowed capacities for the bins.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addBinPacking(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &sizes,
                Universe::UniverseSetBelongingOperator op,
                const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a bin-packing constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param sizes The sizes of the elements to pack.
         * @param capacities The capacities of each bin.
         * @param loads Whether bin loads should be computed.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addBinPackingWithConstantCapacities(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &sizes,
                const std::vector<Universe::BigInteger> &capacities,
                bool loads) override;

        /**
         * Adds to this solver a bin-packing constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param sizes The sizes of the elements to pack.
         * @param capacities The variables encoding the capacities of each bin.
         * @param loads Whether bin loads should be computed.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addBinPackingWithVariableCapacities(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &sizes,
                const std::vector<std::string> &capacities,
                bool loads) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The lengths of the tasks to assign.
         * @param heights The heights of the tasks to assign.
         * @param op The operator to compare the cumulative use with.
         * @param value The value for the cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeConstantLengthsConstantHeights(
                const std::vector<std::string> &origins, const std::vector<Universe::BigInteger> &lengths,
                const std::vector<Universe::BigInteger> &heights, Universe::UniverseRelationalOperator op,
                const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The lengths of the tasks to assign.
         * @param ends The variables encoding the ends of the resources.
         * @param heights The heights of the tasks to assign.
         * @param op The operator to compare the cumulative use with.
         * @param value The value for the cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeConstantLengthsConstantHeights(
                const std::vector<std::string> &origins, const std::vector<Universe::BigInteger> &lengths,
                const std::vector<std::string> &ends, const std::vector<Universe::BigInteger> &heights,
                Universe::UniverseRelationalOperator op, const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The lengths of the tasks to assign.
         * @param heights The heights of the tasks to assign.
         * @param op The operator to compare the cumulative use with.
         * @param value The variable encoding the value for the cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeConstantLengthsConstantHeights(
                const std::vector<std::string> &origins, const std::vector<Universe::BigInteger> &lengths,
                const std::vector<Universe::BigInteger> &heights, Universe::UniverseRelationalOperator op,
                const std::string &value) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The lengths of the tasks to assign.
         * @param ends The variables encoding the ends of the resources.
         * @param heights The heights of the tasks to assign.
         * @param op The operator to compare the cumulative use with.
         * @param value The variable encoding the value for the cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeConstantLengthsConstantHeights(
                const std::vector<std::string> &origins, const std::vector<Universe::BigInteger> &lengths,
                const std::vector<std::string> &ends, const std::vector<Universe::BigInteger> &heights,
                Universe::UniverseRelationalOperator op, const std::string &value) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The lengths of the tasks to assign.
         * @param heights The heights of the tasks to assign.
         * @param op The operator to use to check whether the cumulative use is within a range.
         * @param min The minimum cumulative use.
         * @param max The maximum cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeConstantLengthsConstantHeights(
                const std::vector<std::string> &origins, const std::vector<Universe::BigInteger> &lengths,
                const std::vector<Universe::BigInteger> &heights, Universe::UniverseSetBelongingOperator op,
                const Universe::BigInteger &min, const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The lengths of the tasks to assign.
         * @param ends The variables encoding the ends of the resources.
         * @param heights The heights of the tasks to assign.
         * @param op The operator to use to check whether the cumulative use is within a range.
         * @param min The minimum cumulative use.
         * @param max The maximum cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeConstantLengthsConstantHeights(
                const std::vector<std::string> &origins, const std::vector<Universe::BigInteger> &lengths,
                const std::vector<std::string> &ends, const std::vector<Universe::BigInteger> &heights,
                Universe::UniverseSetBelongingOperator op, const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The lengths of the tasks to assign.
         * @param heights The heights of the tasks to assign.
         * @param op The operator to use to check whether the number of assignments is within a set.
         * @param set The set containing the allowed cumulative uses.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeConstantLengthsConstantHeights(
                const std::vector<std::string> &origins, const std::vector<Universe::BigInteger> &lengths,
                const std::vector<Universe::BigInteger> &heights, Universe::UniverseSetBelongingOperator op,
                const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The lengths of the tasks to assign.
         * @param ends The variables encoding the ends of the resources.
         * @param heights The heights of the tasks to assign.
         * @param op The operator to use to check whether the number of assignments is within a set.
         * @param set The set containing the allowed cumulative uses.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeConstantLengthsConstantHeights(
                const std::vector<std::string> &origins, const std::vector<Universe::BigInteger> &lengths,
                const std::vector<std::string> &ends, const std::vector<Universe::BigInteger> &heights,
                Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The lengths of the tasks to assign.
         * @param heights The variable encoding the heights of the tasks to assign.
         * @param op The operator to compare the cumulative use with.
         * @param value The value for the cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeConstantLengthsVariableHeights(
                const std::vector<std::string> &origins, const std::vector<Universe::BigInteger> &lengths,
                const std::vector<std::string> &heights, Universe::UniverseRelationalOperator op,
                const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The lengths of the tasks to assign.
         * @param ends The variables encoding the ends of the resources.
         * @param heights The variable encoding the heights of the tasks to assign.
         * @param op The operator to compare the cumulative use with.
         * @param value The value for the cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeConstantLengthsVariableHeights(
                const std::vector<std::string> &origins, const std::vector<Universe::BigInteger> &lengths,
                const std::vector<std::string> &ends, const std::vector<std::string> &heights,
                Universe::UniverseRelationalOperator op, const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The lengths of the tasks to assign.
         * @param heights The variable encoding the heights of the tasks to assign.
         * @param op The operator to compare the cumulative use with.
         * @param value The variable encoding the value for the cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeConstantLengthsVariableHeights(
                const std::vector<std::string> &origins, const std::vector<Universe::BigInteger> &lengths,
                const std::vector<std::string> &heights, Universe::UniverseRelationalOperator op,
                const std::string &value) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The lengths of the tasks to assign.
         * @param ends The variables encoding the ends of the resources.
         * @param heights The variable encoding the heights of the tasks to assign.
         * @param op The operator to compare the cumulative use with.
         * @param value The variable encoding the value for the cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeConstantLengthsVariableHeights(
                const std::vector<std::string> &origins, const std::vector<Universe::BigInteger> &lengths,
                const std::vector<std::string> &ends, const std::vector<std::string> &heights,
                Universe::UniverseRelationalOperator op, const std::string &value) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The lengths of the tasks to assign.
         * @param heights The variable encoding the heights of the tasks to assign.
         * @param op The operator to use to check whether the cumulative use is within a range.
         * @param min The minimum cumulative use.
         * @param max The maximum cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeConstantLengthsVariableHeights(
                const std::vector<std::string> &origins, const std::vector<Universe::BigInteger> &lengths,
                const std::vector<std::string> &heights, Universe::UniverseSetBelongingOperator op,
                const Universe::BigInteger &min, const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The lengths of the tasks to assign.
         * @param ends The variables encoding the ends of the resources.
         * @param heights The variable encoding the heights of the tasks to assign.
         * @param op The operator to use to check whether the cumulative use is within a range.
         * @param min The minimum cumulative use.
         * @param max The maximum cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeConstantLengthsVariableHeights(
                const std::vector<std::string> &origins, const std::vector<Universe::BigInteger> &lengths,
                const std::vector<std::string> &ends, const std::vector<std::string> &heights,
                Universe::UniverseSetBelongingOperator op, const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The lengths of the tasks to assign.
         * @param heights The variable encoding the heights of the tasks to assign.
         * @param op The operator to use to check whether the number of assignments is within a set.
         * @param set The set containing the allowed cumulative uses.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeConstantLengthsVariableHeights(
                const std::vector<std::string> &origins, const std::vector<Universe::BigInteger> &lengths,
                const std::vector<std::string> &heights, Universe::UniverseSetBelongingOperator op,
                const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The lengths of the tasks to assign.
         * @param ends The variables encoding the ends of the resources.
         * @param heights The variable encoding the heights of the tasks to assign.
         * @param op The operator to use to check whether the number of assignments is within a set.
         * @param set The set containing the allowed cumulative uses.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeConstantLengthsVariableHeights(
                const std::vector<std::string> &origins, const std::vector<Universe::BigInteger> &lengths,
                const std::vector<std::string> &ends, const std::vector<std::string> &heights,
                Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The variables encoding the lengths of the tasks to assign.
         * @param heights The heights of the tasks to assign.
         * @param op The operator to compare the cumulative use with.
         * @param value The value for the cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeVariableLengthsConstantHeights(
                const std::vector<std::string> &origins, const std::vector<std::string> &lengths,
                const std::vector<Universe::BigInteger> &heights, Universe::UniverseRelationalOperator op,
                const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The variables encoding the lengths of the tasks to assign.
         * @param ends The variables encoding the ends of the resources.
         * @param heights The heights of the tasks to assign.
         * @param op The operator to compare the cumulative use with.
         * @param value The value for the cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeVariableLengthsConstantHeights(
                const std::vector<std::string> &origins, const std::vector<std::string> &lengths,
                const std::vector<std::string> &ends, const std::vector<Universe::BigInteger> &heights,
                Universe::UniverseRelationalOperator op, const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The variables encoding the lengths of the tasks to assign.
         * @param heights The heights of the tasks to assign.
         * @param op The operator to compare the cumulative use with.
         * @param value The variable encoding the value for the cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeVariableLengthsConstantHeights(
                const std::vector<std::string> &origins, const std::vector<std::string> &lengths,
                const std::vector<Universe::BigInteger> &heights, Universe::UniverseRelationalOperator op,
                const std::string &value) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The variables encoding the lengths of the tasks to assign.
         * @param ends The variables encoding the ends of the resources.
         * @param heights The heights of the tasks to assign.
         * @param op The operator to compare the cumulative use with.
         * @param value The variable encoding the value for the cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeVariableLengthsConstantHeights(
                const std::vector<std::string> &origins, const std::vector<std::string> &lengths,
                const std::vector<std::string> &ends, const std::vector<Universe::BigInteger> &heights,
                Universe::UniverseRelationalOperator op, const std::string &value) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The variables encoding the lengths of the tasks to assign.
         * @param heights The heights of the tasks to assign.
         * @param op The operator to use to check whether the cumulative use is within a range.
         * @param min The minimum cumulative use.
         * @param max The maximum cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeVariableLengthsConstantHeights(
                const std::vector<std::string> &origins, const std::vector<std::string> &lengths,
                const std::vector<Universe::BigInteger> &heights, Universe::UniverseSetBelongingOperator op,
                const Universe::BigInteger &min, const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The variables encoding the lengths of the tasks to assign.
         * @param ends The variables encoding the ends of the resources.
         * @param heights The heights of the tasks to assign.
         * @param op The operator to use to check whether the cumulative use is within a range.
         * @param min The minimum cumulative use.
         * @param max The maximum cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeVariableLengthsConstantHeights(
                const std::vector<std::string> &origins, const std::vector<std::string> &lengths,
                const std::vector<std::string> &ends, const std::vector<Universe::BigInteger> &heights,
                Universe::UniverseSetBelongingOperator op, const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The variables encoding the lengths of the tasks to assign.
         * @param heights The heights of the tasks to assign.
         * @param op The operator to use to check whether the number of assignments is within a set.
         * @param set The set containing the allowed cumulative uses.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeVariableLengthsConstantHeights(
                const std::vector<std::string> &origins, const std::vector<std::string> &lengths,
                const std::vector<Universe::BigInteger> &heights, Universe::UniverseSetBelongingOperator op,
                const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The variables encoding the lengths of the tasks to assign.
         * @param ends The variables encoding the ends of the resources.
         * @param heights The heights of the tasks to assign.
         * @param op The operator to use to check whether the number of assignments is within a set.
         * @param set The set containing the allowed cumulative uses.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeVariableLengthsConstantHeights(
                const std::vector<std::string> &origins, const std::vector<std::string> &lengths,
                const std::vector<std::string> &ends, const std::vector<Universe::BigInteger> &heights,
                Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The variables encoding the lengths of the tasks to assign.
         * @param heights The variables encoding the heights of the tasks to assign.
         * @param op The operator to compare the cumulative use with.
         * @param value The value for the cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeVariableLengthsVariableHeights(
                const std::vector<std::string> &origins, const std::vector<std::string> &lengths,
                const std::vector<std::string> &heights, Universe::UniverseRelationalOperator op,
                const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The variables encoding the lengths of the tasks to assign.
         * @param ends The variables encoding the ends of the resources.
         * @param heights The variables encoding the heights of the tasks to assign.
         * @param op The operator to compare the cumulative use with.
         * @param value The value for the cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeVariableLengthsVariableHeights(
                const std::vector<std::string> &origins, const std::vector<std::string> &lengths,
                const std::vector<std::string> &ends, const std::vector<std::string> &heights,
                Universe::UniverseRelationalOperator op, const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The variables encoding the lengths of the tasks to assign.
         * @param heights The variables encoding the heights of the tasks to assign.
         * @param op The operator to compare the cumulative use with.
         * @param value The variable encoding the value for the cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeVariableLengthsVariableHeights(
                const std::vector<std::string> &origins, const std::vector<std::string> &lengths,
                const std::vector<std::string> &heights, Universe::UniverseRelationalOperator op,
                const std::string &value) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The variables encoding the lengths of the tasks to assign.
         * @param ends The variables encoding the ends of the resources.
         * @param heights The variables encoding the heights of the tasks to assign.
         * @param op The operator to compare the cumulative use with.
         * @param value The variable encoding the value for the cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeVariableLengthsVariableHeights(
                const std::vector<std::string> &origins, const std::vector<std::string> &lengths,
                const std::vector<std::string> &ends, const std::vector<std::string> &heights,
                Universe::UniverseRelationalOperator op, const std::string &value) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The variables encoding the lengths of the tasks to assign.
         * @param heights The variables encoding the heights of the tasks to assign.
         * @param op The operator to use to check whether the cumulative use is within a range.
         * @param min The minimum cumulative use.
         * @param max The maximum cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeVariableLengthsVariableHeights(
                const std::vector<std::string> &origins, const std::vector<std::string> &lengths,
                const std::vector<std::string> &heights, Universe::UniverseSetBelongingOperator op,
                const Universe::BigInteger &min, const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The variables encoding the lengths of the tasks to assign.
         * @param ends The variables encoding the ends of the resources.
         * @param heights The variables encoding the heights of the tasks to assign.
         * @param op The operator to use to check whether the cumulative use is within a range.
         * @param min The minimum cumulative use.
         * @param max The maximum cumulative use.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeVariableLengthsVariableHeights(
                const std::vector<std::string> &origins, const std::vector<std::string> &lengths,
                const std::vector<std::string> &ends, const std::vector<std::string> &heights,
                Universe::UniverseSetBelongingOperator op, const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The variables encoding the lengths of the tasks to assign.
         * @param heights The variables encoding the heights of the tasks to assign.
         * @param op The operator to use to check whether the number of assignments is within a set.
         * @param set The set containing the allowed cumulative uses.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeVariableLengthsVariableHeights(
                const std::vector<std::string> &origins, const std::vector<std::string> &lengths,
                const std::vector<std::string> &heights, Universe::UniverseSetBelongingOperator op,
                const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a cumulative constraint.
         *
         * @param origins The variables encoding the origins of the resources.
         * @param lengths The variables encoding the lengths of the tasks to assign.
         * @param ends The variables encoding the ends of the resources.
         * @param heights The variables encoding the heights of the tasks to assign.
         * @param op The operator to use to check whether the number of assignments is within a set.
         * @param set The set containing the allowed cumulative uses.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCumulativeVariableLengthsVariableHeights(
                const std::vector<std::string> &origins, const std::vector<std::string> &lengths,
                const std::vector<std::string> &ends, const std::vector<std::string> &heights,
                Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a knapsack constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param weights The weights of the elements to put in the knapsack.
         * @param wOperator The operator for comparing the weight of the knapsack.
         * @param wValue The total weight of the knapsack.
         * @param profits The profits of the elements to put in the knapsack.
         * @param pOperator The operator for comparing the profit of the knapsack.
         * @param pValue The total profit of the knapsack.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addKnapsack(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &weights,
                Universe::UniverseRelationalOperator wOperator,
                const Universe::BigInteger &wValue,
                const std::vector<Universe::BigInteger> &profits,
                Universe::UniverseRelationalOperator pOperator,
                const Universe::BigInteger &pValue) override;

        /**
         * Adds to this solver a knapsack constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param weights The weights of the elements to put in the knapsack.
         * @param wOperator The operator for comparing the weight of the knapsack.
         * @param wValue The total weight of the knapsack.
         * @param profits The profits of the elements to put in the knapsack.
         * @param pOperator The operator for comparing the profit of the knapsack.
         * @param pVariable The variable encoding the total profit of the knapsack.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addKnapsack(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &weights,
                Universe::UniverseRelationalOperator wOperator,
                const Universe::BigInteger &wValue,
                const std::vector<Universe::BigInteger> &profits,
                Universe::UniverseRelationalOperator pOperator,
                const std::string &pVariable) override;

        /**
         * Adds to this solver a knapsack constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param weights The weights of the elements to put in the knapsack.
         * @param wOperator The operator for comparing the weight of the knapsack.
         * @param wValue The total weight of the knapsack.
         * @param profits The profits of the elements to put in the knapsack.
         * @param pOperator The operator for comparing the profit of the knapsack.
         * @param pMin The minimum profit of the knapsack.
         * @param pMax The maximum profit of the knapsack.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addKnapsack(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &weights,
                Universe::UniverseRelationalOperator wOperator,
                const Universe::BigInteger &wValue,
                const std::vector<Universe::BigInteger> &profits,
                Universe::UniverseSetBelongingOperator pOperator,
                const Universe::BigInteger &pMin,
                const Universe::BigInteger &pMax) override;

        /**
         * Adds to this solver a knapsack constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param weights The weights of the elements to put in the knapsack.
         * @param wOperator The operator for comparing the weight of the knapsack.
         * @param wValue The total weight of the knapsack.
         * @param profits The profits of the elements to put in the knapsack.
         * @param pOperator The operator for comparing the profit of the knapsack.
         * @param pSet The allowed total profits of the knapsack.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addKnapsack(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &weights,
                Universe::UniverseRelationalOperator wOperator,
                const Universe::BigInteger &wValue,
                const std::vector<Universe::BigInteger> &profits,
                Universe::UniverseSetBelongingOperator pOperator,
                const std::vector<Universe::BigInteger> &pSet) override;

        /**
         * Adds to this solver a knapsack constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param weights The weights of the elements to put in the knapsack.
         * @param wOperator The operator for comparing the weight of the knapsack.
         * @param wVariable The variable encoding the total weight of the knapsack.
         * @param profits The profits of the elements to put in the knapsack.
         * @param pOperator The operator for comparing the profit of the knapsack.
         * @param pValue The variable encoding the total profit of the knapsack.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addKnapsack(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &weights,
                Universe::UniverseRelationalOperator wOperator,
                const std::string &wVariable,
                const std::vector<Universe::BigInteger> &profits,
                Universe::UniverseRelationalOperator pOperator,
                const Universe::BigInteger &pValue) override;

        /**
         * Adds to this solver a knapsack constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param weights The weights of the elements to put in the knapsack.
         * @param wOperator The operator for comparing the weight of the knapsack.
         * @param wVariable The variable encoding the total weight of the knapsack.
         * @param profits The profits of the elements to put in the knapsack.
         * @param pOperator The operator for comparing the profit of the knapsack.
         * @param pVariable The variable encoding the total profit of the knapsack.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addKnapsack(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &weights,
                Universe::UniverseRelationalOperator wOperator,
                const std::string &wVariable,
                const std::vector<Universe::BigInteger> &profits,
                Universe::UniverseRelationalOperator pOperator,
                const std::string &pVariable) override;

        /**
         * Adds to this solver a knapsack constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param weights The weights of the elements to put in the knapsack.
         * @param wOperator The operator for comparing the weight of the knapsack.
         * @param wVariable The variable encoding the total weight of the knapsack.
         * @param profits The profits of the elements to put in the knapsack.
         * @param pOperator The operator for comparing the profit of the knapsack.
         * @param pMin The minimum profit of the knapsack.
         * @param pMax The maximum profit of the knapsack.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addKnapsack(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &weights,
                Universe::UniverseRelationalOperator wOperator,
                const std::string &wVariable,
                const std::vector<Universe::BigInteger> &profits,
                Universe::UniverseSetBelongingOperator pOperator,
                const Universe::BigInteger &pMin,
                const Universe::BigInteger &pMax) override;

        /**
         * Adds to this solver a knapsack constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param weights The weights of the elements to put in the knapsack.
         * @param wOperator The operator for comparing the weight of the knapsack.
         * @param wVariable The variable encoding the total weight of the knapsack.
         * @param profits The profits of the elements to put in the knapsack.
         * @param pOperator The operator for comparing the profit of the knapsack.
         * @param pSet The allowed total profits of the knapsack.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addKnapsack(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &weights,
                Universe::UniverseRelationalOperator wOperator,
                const std::string &wVariable,
                const std::vector<Universe::BigInteger> &profits,
                Universe::UniverseSetBelongingOperator pOperator,
                const std::vector<Universe::BigInteger> &pSet) override;

        /**
         * Adds to this solver a knapsack constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param weights The weights of the elements to put in the knapsack.
         * @param wOperator The operator for comparing the weight of the knapsack.
         * @param wMin The minimum weight of the knapsack.
         * @param wMax The maximum weight of the knapsack.
         * @param profits The profits of the elements to put in the knapsack.
         * @param pOperator The operator for comparing the profit of the knapsack.
         * @param pValue The total profit of the knapsack.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addKnapsack(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &weights,
                Universe::UniverseSetBelongingOperator wOperator,
                const Universe::BigInteger &wMin,
                const Universe::BigInteger &wMax,
                const std::vector<Universe::BigInteger> &profits,
                Universe::UniverseRelationalOperator pOperator,
                const Universe::BigInteger &pValue) override;

        /**
         * Adds to this solver a knapsack constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param weights The weights of the elements to put in the knapsack.
         * @param wOperator The operator for comparing the weight of the knapsack.
         * @param wMin The minimum weight of the knapsack.
         * @param wMax The maximum weight of the knapsack.
         * @param profits The profits of the elements to put in the knapsack.
         * @param pOperator The operator for comparing the profit of the knapsack.
         * @param pVariable The variable encoding the total profit of the knapsack.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addKnapsack(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &weights,
                Universe::UniverseSetBelongingOperator wOperator,
                const Universe::BigInteger &wMin,
                const Universe::BigInteger &wMax,
                const std::vector<Universe::BigInteger> &profits,
                Universe::UniverseRelationalOperator pOperator,
                const std::string &pVariable) override;

        /**
         * Adds to this solver a knapsack constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param weights The weights of the elements to put in the knapsack.
         * @param wOperator The operator for comparing the weight of the knapsack.
         * @param wMin The minimum weight of the knapsack.
         * @param wMax The maximum weight of the knapsack.
         * @param profits The profits of the elements to put in the knapsack.
         * @param pOperator The operator for comparing the profit of the knapsack.
         * @param pMin The minimum profit of the knapsack.
         * @param pMax The maximum profit of the knapsack.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addKnapsack(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &weights,
                Universe::UniverseSetBelongingOperator wOperator,
                const Universe::BigInteger &wMin,
                const Universe::BigInteger &wMax,
                const std::vector<Universe::BigInteger> &profits,
                Universe::UniverseSetBelongingOperator pOperator,
                const Universe::BigInteger &pMin,
                const Universe::BigInteger &pMax) override;

        /**
         * Adds to this solver a knapsack constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param weights The weights of the elements to put in the knapsack.
         * @param wOperator The operator for comparing the weight of the knapsack.
         * @param wMin The minimum weight of the knapsack.
         * @param wMax The maximum weight of the knapsack.
         * @param profits The profits of the elements to put in the knapsack.
         * @param pOperator The operator for comparing the profit of the knapsack.
         * @param pSet The allowed total profits of the knapsack.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addKnapsack(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &weights,
                Universe::UniverseSetBelongingOperator wOperator,
                const Universe::BigInteger &wMin,
                const Universe::BigInteger &wMax,
                const std::vector<Universe::BigInteger> &profits,
                Universe::UniverseSetBelongingOperator pOperator,
                const std::vector<Universe::BigInteger> &pSet) override;

        /**
         * Adds to this solver a knapsack constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param weights The weights of the elements to put in the knapsack.
         * @param wOperator The operator for comparing the weight of the knapsack.
         * @param wSet The allowed total weights of the knapsack.
         * @param profits The profits of the elements to put in the knapsack.
         * @param pOperator The operator for comparing the profit of the knapsack.
         * @param pValue The variable encoding the total profit of the knapsack.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addKnapsack(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &weights,
                Universe::UniverseSetBelongingOperator wOperator,
                const std::vector<Universe::BigInteger> &wSet,
                const std::vector<Universe::BigInteger> &profits,
                Universe::UniverseRelationalOperator pOperator,
                const Universe::BigInteger &pValue) override;

        /**
         * Adds to this solver a knapsack constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param weights The weights of the elements to put in the knapsack.
         * @param wOperator The operator for comparing the weight of the knapsack.
         * @param wSet The allowed total weights of the knapsack.
         * @param profits The profits of the elements to put in the knapsack.
         * @param pOperator The operator for comparing the profit of the knapsack.
         * @param pVariable The variable encoding the total profit of the knapsack.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addKnapsack(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &weights,
                Universe::UniverseSetBelongingOperator wOperator,
                const std::vector<Universe::BigInteger> &wSet,
                const std::vector<Universe::BigInteger> &profits,
                Universe::UniverseRelationalOperator pOperator,
                const std::string &pVariable) override;

        /**
         * Adds to this solver a knapsack constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param weights The weights of the elements to put in the knapsack.
         * @param wOperator The operator for comparing the weight of the knapsack.
         * @param wSet The allowed total weights of the knapsack.
         * @param profits The profits of the elements to put in the knapsack.
         * @param pOperator The operator for comparing the profit of the knapsack.
         * @param pMin The minimum profit of the knapsack.
         * @param pMax The maximum profit of the knapsack.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addKnapsack(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &weights,
                Universe::UniverseSetBelongingOperator wOperator,
                const std::vector<Universe::BigInteger> &wSet,
                const std::vector<Universe::BigInteger> &profits,
                Universe::UniverseSetBelongingOperator pOperator,
                const Universe::BigInteger &pMin,
                const Universe::BigInteger &pMax) override;

        /**
         * Adds to this solver a knapsack constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param weights The weights of the elements to put in the knapsack.
         * @param wOperator The operator for comparing the weight of the knapsack.
         * @param wSet The allowed total weights of the knapsack.
         * @param profits The profits of the elements to put in the knapsack.
         * @param pOperator The operator for comparing the profit of the knapsack.
         * @param pSet The allowed total profits of the knapsack.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addKnapsack(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &weights,
                Universe::UniverseSetBelongingOperator wOperator,
                const std::vector<Universe::BigInteger> &wSet,
                const std::vector<Universe::BigInteger> &profits,
                Universe::UniverseSetBelongingOperator pOperator,
                const std::vector<Universe::BigInteger> &pSet) override;

        /**
         * Adds to this solver a stretch constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param values The values to assign to the variables.
         * @param widthsMin The minimum widths of the sequences to build.
         * @param widthsMax The maximum widths of the sequences to build.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addStretch(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &values,
                const std::vector<Universe::BigInteger> &widthsMin,
                const std::vector<Universe::BigInteger> &widthsMax) override;

        /**
         * Adds to this solver a stretch constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param values The values to assign to the variables.
         * @param widthsMin The minimum widths of the sequences to build.
         * @param widthsMax The maximum widths of the sequences to build.
         * @param patterns The possible successive values between stretches.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addStretch(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &values,
                const std::vector<Universe::BigInteger> &widthsMin,
                const std::vector<Universe::BigInteger> &widthsMax,
                const std::vector<std::vector<Universe::BigInteger>> &patterns) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param op The relational operator used to compare the value with those assigned to the variables.
         * @param value The value to look for among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addElement(const std::vector<std::string> &variables,
                        Universe::UniverseRelationalOperator op, const Universe::BigInteger &value) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param op The relational operator used to compare the value with those assigned to the variables.
         * @param value The variable encoding the value to look for among the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addElement(const std::vector<std::string> &variables,
                        Universe::UniverseRelationalOperator op, const std::string &value) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param op The operator used to check whether a variable is assigned in the range.
         * @param min The minimum value of the range.
         * @param max The maximum value of the range.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addElement(const std::vector<std::string> &variables, Universe::UniverseSetBelongingOperator op,
                        const Universe::BigInteger &min, const Universe::BigInteger &max) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param op The operator used to check whether a variable is assigned in the set.
         * @param set The set of values to look for.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addElement(const std::vector<std::string> &variables, Universe::UniverseSetBelongingOperator op,
                        const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param values The values among which to look for the variable.
         * @param startIndex The index at which to start looking for the variable.
         * @param index The index at which the variable appears in the values.
         * @param op The relational operator used to compare the value with those assigned to the variables.
         * @param value The value to look for.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addElementConstantValues(const std::vector<Universe::BigInteger> &values,
                                      int startIndex, const std::string &index, Universe::UniverseRelationalOperator op,
                                      const Universe::BigInteger &value) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param values The values among which to look for the variable.
         * @param startIndex The index at which to start looking for the variable.
         * @param index The index at which the variable appears in the values.
         * @param op The relational operator used to compare the value with those assigned to the variables.
         * @param variable The variable whose value is to be looked for.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addElementConstantValues(const std::vector<Universe::BigInteger> &values, int startIndex,
                                      const std::string &index, Universe::UniverseRelationalOperator op, const std::string &variable) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param values The values among which to look for the variable.
         * @param startIndex The index at which to start looking for the variable.
         * @param index The index at which the variable appears in the values.
         * @param op The operator used to check whether a variable is assigned in the range.
         * @param min The minimum value of the range.
         * @param max The maximum value of the range.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addElementConstantValues(const std::vector<Universe::BigInteger> &values, int startIndex,
                                      const std::string &index, Universe::UniverseSetBelongingOperator op,
                                      const Universe::BigInteger &min, const Universe::BigInteger &max) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param values The values among which to look for the variable.
         * @param startIndex The index at which to start looking for the variable.
         * @param index The index at which the variable appears in the values.
         * @param op The operator used to check whether a variable is assigned in the set.
         * @param set The set of values to look for.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addElementConstantValues(const std::vector<Universe::BigInteger> &values, int startIndex,
                                      const std::string &index, Universe::UniverseSetBelongingOperator op,
                                      const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param variables The variables among which to look for the value.
         * @param startIndex The index at which to start looking for the value.
         * @param index The index at which the value appears in the variables.
         * @param op The relational operator used to compare the value with those assigned to the variables.
         * @param value The value to look for.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addElement(const std::vector<std::string> &variables, int startIndex, const std::string &index,
                        Universe::UniverseRelationalOperator op, const Universe::BigInteger &value) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param variables The variables among which to look for the value.
         * @param startIndex The index at which to start looking for the variable.
         * @param index The index at which the variable appears in the values.
         * @param op The relational operator used to compare the value with those assigned to the variables.
         * @param variable The variable whose value is to be looked for.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addElement(const std::vector<std::string> &variables, int startIndex, const std::string &index,
                        Universe::UniverseRelationalOperator op, const std::string &variable) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param variables The variables among which to look for the value.
         * @param startIndex The index at which to start looking for the variable.
         * @param index The index at which the variable appears in the values.
         * @param op The operator used to check whether a variable is assigned in the range.
         * @param min The minimum value of the range.
         * @param max The maximum value of the range.
         *
         * @throws UniverseContradictionException If adding the constraint results in a
         *         trivial inconsistency.
         */
        void addElement(const std::vector<std::string> &variables, int startIndex, const std::string &index,
                        Universe::UniverseSetBelongingOperator op, const Universe::BigInteger &min,
                        const Universe::BigInteger &max) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param variables The variables among which to look for the value.
         * @param startIndex The index at which to start looking for the variable.
         * @param index The index at which the variable appears in the values.
         * @param op The operator used to check whether a variable is assigned in the set.
         * @param set The set of values to look for.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addElement(const std::vector<std::string> &variables, int startIndex, const std::string &index,
                        Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param matrix The matrix of values among which the value must appear.
         * @param startRowIndex The index of the row starting from which the value must appear.
         * @param rowIndex The variable encoding the index of the row at which the value appears.
         * @param startColIndex The index of the column starting from which the value must appear.
         * @param colIndex The variable encoding the index of the column at which the value appears.
         * @param op The relational operator used to compare the value with those assigned to the variables.
         * @param value The value to look for inside the matrix.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addElementConstantMatrix(const std::vector<std::vector<Universe::BigInteger>> &matrix,
                                      int startRowIndex, const std::string &rowIndex, int startColIndex, const std::string &colIndex,
                                      Universe::UniverseRelationalOperator op, const Universe::BigInteger &value) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param matrix The matrix of values among which the value must appear.
         * @param startRowIndex The index of the row starting from which the value must appear.
         * @param rowIndex The variable encoding the index of the row at which the value appears.
         * @param startColIndex The index of the column starting from which the value must appear.
         * @param colIndex The variable encoding the index of the column at which the value appears.
         * @param op The relational operator used to compare the value with those assigned to the variables.
         * @param value The variable whose value is to be looked for inside the matrix.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addElementConstantMatrix(const std::vector<std::vector<Universe::BigInteger>> &matrix,
                                      int startRowIndex, const std::string &rowIndex, int startColIndex, const std::string &colIndex,
                                      Universe::UniverseRelationalOperator op, const std::string &value) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param matrix The matrix of values among which the value must appear.
         * @param startRowIndex The index of the row starting from which the value must appear.
         * @param rowIndex The variable encoding the index of the row at which the value appears.
         * @param startColIndex The index of the column starting from which the value must appear.
         * @param colIndex The variable encoding the index of the column at which the value appears.
         * @param op The operator used to check whether a variable is assigned in the range.
         * @param min The minimum value of the range.
         * @param max The maximum value of the range.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addElementConstantMatrix(const std::vector<std::vector<Universe::BigInteger>> &matrix,
                                      int startRowIndex, const std::string &rowIndex, int startColIndex, const std::string &colIndex,
                                      Universe::UniverseSetBelongingOperator op, const Universe::BigInteger &min,
                                      const Universe::BigInteger &max) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param matrix The matrix of values among which the value must appear.
         * @param startRowIndex The index of the row starting from which the value must appear.
         * @param rowIndex The variable encoding the index of the row at which the value appears.
         * @param startColIndex The index of the column starting from which the value must appear.
         * @param colIndex The variable encoding the index of the column at which the value appears.
         * @param op The operator used to check whether a variable is assigned in the set.
         * @param set The set of values to look for.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addElementConstantMatrix(const std::vector<std::vector<Universe::BigInteger>> &matrix,
                                      int startRowIndex, const std::string &rowIndex, int startColIndex, const std::string &colIndex,
                                      Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param matrix The matrix of variables among which the value must be assigned.
         * @param startRowIndex The index of the row starting from which the value must appear.
         * @param rowIndex The variable encoding the index of the row at which the value appears.
         * @param startColIndex The index of the column starting from which the value must appear.
         * @param colIndex The variable encoding the index of the column at which the value appears.
         * @param op The relational operator used to compare the value with those assigned to the variables.
         * @param value The variable whose value is to be looked for inside the matrix.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addElementMatrix(const std::vector<std::vector<std::string>> &matrix,
                              int startRowIndex, const std::string &rowIndex, int startColIndex, const std::string &colIndex,
                              Universe::UniverseRelationalOperator op, const Universe::BigInteger &value) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param matrix The matrix of variables among which the value must be assigned.
         * @param startRowIndex The index of the row starting from which the value must appear.
         * @param rowIndex The variable encoding the index of the row at which the value appears.
         * @param startColIndex The index of the column starting from which the value must appear.
         * @param colIndex The variable encoding the index of the column at which the value appears.
         * @param op The relational operator used to compare the value with those assigned to the variables.
         * @param value The variable whose value is to be looked for inside the matrix.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addElementMatrix(const std::vector<std::vector<std::string>> &matrix,
                              int startRowIndex, const std::string &rowIndex, int startColIndex, const std::string &colIndex,
                              Universe::UniverseRelationalOperator op, const std::string &value) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param matrix The matrix of variables among which the value must be assigned.
         * @param startRowIndex The index of the row starting from which the value must appear.
         * @param rowIndex The variable encoding the index of the row at which the value appears.
         * @param startColIndex The index of the column starting from which the value must appear.
         * @param colIndex The variable encoding the index of the column at which the value appears.
         * @param op The operator used to check whether a variable is assigned in the range.
         * @param min The minimum value of the range.
         * @param max The maximum value of the range.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addElementMatrix(const std::vector<std::vector<std::string>> &matrix,
                              int startRowIndex, const std::string &rowIndex, int startColIndex, const std::string &colIndex,
                              Universe::UniverseSetBelongingOperator op, const Universe::BigInteger &min,
                              const Universe::BigInteger &max) override;

        /**
         * Adds to this solver an element constraint.
         *
         * @param matrix The matrix of variables among which the value must be assigned.
         * @param startRowIndex The index of the row starting from which the value must appear.
         * @param rowIndex The variable encoding the index of the row at which the value appears.
         * @param startColIndex The index of the column starting from which the value must appear.
         * @param colIndex The variable encoding the index of the column at which the value appears.
         * @param op The operator used to check whether a variable is assigned in the set.
         * @param set The set of values to look for.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addElementMatrix(const std::vector<std::vector<std::string>> &matrix,
                              int startRowIndex, const std::string &rowIndex, int startColIndex, const std::string &colIndex,
                              Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a precedence constraint.
         *
         * @param variables The variables appearing in the constraint.
         */
        void addPrecedence(const std::vector<std::string> &variables) override;

        /**
         * Adds to this solver a precedence constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param values The values on which the precedence constraint applies.
         * @param covered Whether each value of the specified list must be assigned by at
         *        least one variable in the scope of the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addPrecedence(const std::vector<std::string> &variables,
                           const std::vector<Universe::BigInteger> &values, bool covered) override;

        /**
         * Adds to this solver an extension constraint describing the support of a variable.
         *
         * @param variable The variable for which the support is given.
         * @param allowedValues The values allowed for the variable.
         * @param hasStar Whether the allowed values contain stars (to mark that any value is allowed).
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSupport(const std::string &variable,
                        const std::vector<Universe::BigInteger> &allowedValues, bool hasStar) override;

        /**
         * Adds to this solver an extension constraint describing the support of a variable.
         *
         * @param variable The variable for which the support is given.
         * @param allowedValues The values allowed for the variable.
         * @param hasStar Whether the allowed values contain stars (to mark that any value is allowed).
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSupportSymbolic(const std::string &variable,
                                const std::vector<std::string> &allowedValues, bool hasStar) override;

        /**
         * Adds to this solver an extension constraint describing the support of a tuple of variables.
         *
         * @param variableTuple The tuple of variables for which the support is given.
         * @param allowedValues The values allowed for the tuple variables.
         * @param hasStar Whether the allowed values contain stars (to mark that any value is allowed).
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSupport(const std::vector<std::string> &variableTuple,
                        const std::vector<std::vector<Universe::BigInteger>> &allowedValues, bool hasStar) override;

        /**
         * Adds to this solver an extension constraint describing the support of a tuple of variables.
         *
         * @param variableTuple The tuple of variables for which the support is given.
         * @param allowedValues The values allowed for the tuple variables.
         * @param hasStar Whether the allowed values contain stars (to mark that any value is allowed).
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSupportSymbolic(const std::vector<std::string> &variableTuple,
                                const std::vector<std::vector<std::string>> &allowedValues, bool hasStar) override;

        /**
         * Adds to this solver an extension constraint describing the conflicts of a variable.
         *
         * @param variable The variable for which the conflicts are given.
         * @param forbiddenValues The values forbidden for the variable.
         * @param hasStar Whether the forbidden values contain stars (to mark that any value is forbidden).
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addConflicts(const std::string &variable,
                          const std::vector<Universe::BigInteger> &forbiddenValues, bool hasStar) override;

        /**
         * Adds to this solver an extension constraint describing the conflicts of a variable.
         *
         * @param variable The variable for which the conflicts are given.
         * @param forbiddenValues The values forbidden for the variable.
         * @param hasStar Whether the forbidden values contain stars (to mark that any value is forbidden).
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addConflictsSymbolic(
                const std::string &variable, const std::vector<std::string> &forbiddenValues, bool hasStar) override;

        /**
         * Adds to this solver an extension constraint describing the conflicts of a tuple of variables.
         *
         * @param variableTuple The tuple of variables for which the conflicts are given.
         * @param forbiddenValues The values forbidden for the tuple variables.
         * @param hasStar Whether the forbidden values contain stars (to mark that any value is forbidden).
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addConflicts(const std::vector<std::string> &variableTuple,
                          const std::vector<std::vector<Universe::BigInteger>> &forbiddenValues, bool hasStar) override;

        /**
         * Adds to this solver an extension constraint describing the conflicts of a tuple of variables.
         *
         * @param variableTuple The tuple of variables for which the conflicts are given.
         * @param forbiddenValues The values forbidden for the tuple variables.
         * @param hasStar Whether the forbidden values contain stars (to mark that any value is forbidden).
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addConflictsSymbolic(const std::vector<std::string> &variableTuple,
                                  const std::vector<std::vector<std::string>> &forbiddenValues, bool hasStar) override;

        /**
         * Adds to this solver an intension constraint.
         *
         * @param constr The user-friendly representation of the constraint to add.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addIntension(Universe::IUniverseIntensionConstraint *constr) override;

        /**
         * Adds to this solver a primitive constraint.
         *
         * @param variable The variable appearing in the constraint.
         * @param op The operator used in the constraint.
         * @param value The value to compare the variable with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addPrimitive(const std::string &variable,
                          Universe::UniverseRelationalOperator op, const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a primitive constraint.
         *
         * @param variable The variable appearing in the constraint.
         * @param arithOp The arithmetic operator applied on the variable.
         * @param leftHandSide The value on the left-hand side of the constraint.
         * @param relOp The relational operator used to compare the right-hand side with the left-hand side.
         * @param rightHandSide The value on the right-hand side of the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addPrimitive(const std::string &variable, Universe::UniverseArithmeticOperator arithOp,
                          const Universe::BigInteger &leftHandSide, Universe::UniverseRelationalOperator relOp,
                          const Universe::BigInteger &rightHandSide) override;

        /**
         * Adds to this solver a primitive constraint.
         *
         * @param variable The variable appearing in the constraint.
         * @param arithOp The arithmetic operator applied on the variable.
         * @param leftHandSide The variable on the left-hand side of the constraint.
         * @param relOp The relational operator used to compare the right-hand side with the left-hand side.
         * @param rightHandSide The value on the right-hand side of the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addPrimitive(const std::string &variable, Universe::UniverseArithmeticOperator arithOp,
                          const std::string &leftHandSide, Universe::UniverseRelationalOperator relOp,
                          const Universe::BigInteger &rightHandSide) override;

        /**
         * Adds to this solver a primitive constraint.
         *
         * @param variable The variable appearing in the constraint.
         * @param arithOp The arithmetic operator applied on the variable.
         * @param leftHandSide The value on the left-hand side of the constraint.
         * @param relOp The relational operator used to compare the right-hand side with the left-hand side.
         * @param rightHandSide The variable on the right-hand side of the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addPrimitive(const std::string &variable, Universe::UniverseArithmeticOperator arithOp,
                          const Universe::BigInteger &leftHandSide, Universe::UniverseRelationalOperator relOp,
                          const std::string &rightHandSide) override;

        /**
         * Adds to this solver a primitive constraint.
         *
         * @param variable The variable appearing in the constraint.
         * @param arithOp The arithmetic operator applied on the variable.
         * @param leftHandSide The variable on the left-hand side of the constraint.
         * @param relOp The relational operator used to compare the right-hand side with the left-hand side.
         * @param rightHandSide The variable on the right-hand side of the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addPrimitive(const std::string &variable, Universe::UniverseArithmeticOperator arithOp,
                          const std::string &leftHandSide, Universe::UniverseRelationalOperator relOp,
                          const std::string &rightHandSide) override;

        /**
         * Adds to this solver a primitive constraint.
         *
         * @param arithOp The arithmetic operator applied on the variable.
         * @param variable The variable on which the operator is applied.
         * @param rightHandSide The variable on the right-hand side of the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addPrimitive(Universe::UniverseArithmeticOperator arithOp,
                          const std::string &variable, const std::string &rightHandSide) override;

        /**
         * Adds to this solver a primitive constraint.
         *
         * @param variable The variable appearing in the constraint.
         * @param op The operator defining whether the values are allowed or forbidden.
         * @param min The minimum value of the range on which the operator is applied.
         * @param max The maximum value of the range on which the operator is applied.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addPrimitive(const std::string &variable, Universe::UniverseSetBelongingOperator op,
                          const Universe::BigInteger &min, const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a primitive constraint.
         *
         * @param variable The variable appearing in the constraint.
         * @param op The operator defining whether the values are allowed or forbidden.
         * @param values The set of values on which the operator is applied.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addPrimitive(const std::string &variable,
                          Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &values) override;

        /**
         * Adds to this solver a minimum constraint.
         *
         * @param variables The variables to compute the minimum of.
         * @param op The relational operator to use to compare the minimum.
         * @param value The value to compare the minimum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimum(
                const std::vector<std::string> &variables, Universe::UniverseRelationalOperator op,
                const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a minimum constraint.
         *
         * @param variables The variables to compute the minimum of.
         * @param op The relational operator to use to compare the minimum.
         * @param value The variable to compare the minimum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimum(
                const std::vector<std::string> &variables, Universe::UniverseRelationalOperator op,
                const std::string &value) override;

        /**
         * Adds to this solver a minimum constraint.
         *
         * @param variables The variables to compute the minimum of.
         * @param op The operator checking whether the minimum is in the range.
         * @param min The minimum value for the minimum.
         * @param max The maximum value for the minimum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimum(
                const std::vector<std::string> &variables, Universe::UniverseSetBelongingOperator op,
                const Universe::BigInteger &min, const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a minimum constraint.
         *
         * @param variables The variables to compute the minimum of.
         * @param op The operator checking whether the minimum is in the set.
         * @param set The allowed values for the minimum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimum(
                const std::vector<std::string> &variables, Universe::UniverseSetBelongingOperator op,
                const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a minimum constraint.
         *
         * @param variables The variables to compute the minimum of.
         * @param startIndex The index at which to start looking for the minimum
         * @param index The variable encoding the index at which the minimum is.
         * @param op The relational operator to use to compare the minimum.
         * @param value The value to compare the minimum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimumIndex(
                const std::vector<std::string> &variables, int startIndex, const std::string &index,
                Universe::UniverseRelationalOperator op, const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a minimum constraint.
         *
         * @param variables The variables to compute the minimum of.
         * @param startIndex The index at which to start looking for the minimum
         * @param index The variable encoding the index at which the minimum is.
         * @param op The relational operator to use to compare the minimum.
         * @param value The variable to compare the minimum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimumIndex(
                const std::vector<std::string> &variables, int startIndex, const std::string &index,
                Universe::UniverseRelationalOperator op, const std::string &value) override;

        /**
         * Adds to this solver a minimum constraint.
         *
         * @param variables The variables to compute the minimum of.
         * @param startIndex The index at which to start looking for the minimum
         * @param index The variable encoding the index at which the minimum is.
         * @param op The operator checking whether the minimum is in the range.
         * @param min The minimum value for the minimum.
         * @param max The maximum value for the minimum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimumIndex(
                const std::vector<std::string> &variables, int startIndex, const std::string &index,
                Universe::UniverseSetBelongingOperator op, const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a minimum constraint.
         *
         * @param variables The variables to compute the minimum of.
         * @param startIndex The index at which to start looking for the minimum
         * @param index The variable encoding the index at which the minimum is.
         * @param op The operator checking whether the minimum is in the set.
         * @param set The allowed values for the minimum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimumIndex(
                const std::vector<std::string> &variables, int startIndex, const std::string &index,
                Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a minimum constraint.
         *
         * @param intensionConstraints The intension constraints to compute the minimum of.
         * @param op The relational operator to use to compare the minimum.
         * @param value The value to compare the minimum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimumIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &intensionConstraints,
                Universe::UniverseRelationalOperator op, const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a minimum constraint.
         *
         * @param intensionConstraints The intension constraints to compute the minimum of.
         * @param op The relational operator to use to compare the minimum.
         * @param value The variable to compare the minimum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimumIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &intensionConstraints,
                Universe::UniverseRelationalOperator op, const std::string &value) override;

        /**
         * Adds to this solver a minimum constraint.
         *
         * @param intensionConstraints The intension constraints to compute the minimum of.
         * @param op The operator checking whether the minimum is in the range.
         * @param min The minimum value for the minimum.
         * @param max The maximum value for the minimum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimumIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &intensionConstraints,
                Universe::UniverseSetBelongingOperator op, const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a minimum constraint.
         *
         * @param intensionConstraints The intension constraints to compute the minimum of.
         * @param op The operator checking whether the minimum is in the set.
         * @param set The allowed values for the minimum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimumIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &intensionConstraints,
                Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a minimum-arg constraint.
         *
         * @param variables The variables to compute the minimum of.
         * @param op The relational operator to use to compare the minimum.
         * @param value The value to compare the minimum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimumArg(
                const std::vector<std::string> &variables, Universe::UniverseRelationalOperator op,
                const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a minimum-arg constraint.
         *
         * @param variables The variables to compute the minimum of.
         * @param op The relational operator to use to compare the minimum.
         * @param value The variable to compare the minimum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimumArg(
                const std::vector<std::string> &variables, Universe::UniverseRelationalOperator op,
                const std::string &value) override;

        /**
         * Adds to this solver a minimum-arg constraint.
         *
         * @param variables The variables to compute the minimum of.
         * @param op The operator checking whether the minimum is in the range.
         * @param min The minimum value for the minimum.
         * @param max The maximum value for the minimum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimumArg(
                const std::vector<std::string> &variables, Universe::UniverseSetBelongingOperator op,
                const Universe::BigInteger &min, const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a minimum-arg constraint.
         *
         * @param variables The variables to compute the minimum of.
         * @param op The operator checking whether the minimum is in the set.
         * @param set The allowed values for the minimum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimumArg(
                const std::vector<std::string> &variables, Universe::UniverseSetBelongingOperator op,
                const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a minimum-arg constraint.
         *
         * @param intensionConstraints The intension constraints to compute the minimum of.
         * @param op The relational operator to use to compare the minimum.
         * @param value The value to compare the minimum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimumArgIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &intensionConstraints,
                Universe::UniverseRelationalOperator op, const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a minimum-arg constraint.
         *
         * @param intensionConstraints The intension constraints to compute the minimum of.
         * @param op The relational operator to use to compare the minimum.
         * @param value The variable to compare the minimum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimumArgIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &intensionConstraints,
                Universe::UniverseRelationalOperator op, const std::string &value) override;

        /**
         * Adds to this solver a minimum-arg constraint.
         *
         * @param intensionConstraints The intension constraints to compute the minimum of.
         * @param op The operator checking whether the minimum is in the range.
         * @param min The minimum value for the minimum.
         * @param max The maximum value for the minimum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimumArgIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &intensionConstraints,
                Universe::UniverseSetBelongingOperator op, const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a minimum-arg constraint.
         *
         * @param intensionConstraints The intension constraints to compute the minimum of.
         * @param op The operator checking whether the minimum is in the set.
         * @param set The allowed values for the minimum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMinimumArgIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &intensionConstraints,
                Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a maximum constraint.
         *
         * @param variables The variables to compute the maximum of.
         * @param op The relational operator to use to compare the maximum.
         * @param value The value to compare the maximum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximum(
                const std::vector<std::string> &variables, Universe::UniverseRelationalOperator op,
                const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a maximum constraint.
         *
         * @param variables The variables to compute the maximum of.
         * @param op The relational operator to use to compare the maximum.
         * @param value The variable to compare the maximum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximum(
                const std::vector<std::string> &variables, Universe::UniverseRelationalOperator op,
                const std::string &value) override;

        /**
         * Adds to this solver a maximum constraint.
         *
         * @param variables The variables to compute the maximum of.
         * @param op The operator checking whether the maximum is in the range.
         * @param min The minimum value for the maximum.
         * @param max The maximum value for the maximum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximum(
                const std::vector<std::string> &variables, Universe::UniverseSetBelongingOperator op,
                const Universe::BigInteger &min, const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a maximum constraint.
         *
         * @param variables The variables to compute the maximum of.
         * @param op The operator checking whether the maximum is in the set.
         * @param set The allowed values for the maximum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximum(
                const std::vector<std::string> &variables, Universe::UniverseSetBelongingOperator op,
                const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a maximum constraint.
         *
         * @param variables The variables to compute the maximum of.
         * @param startIndex The index at which to start looking for the maximum
         * @param index The variable encoding the index at which the maximum is.
         * @param op The relational operator to use to compare the maximum.
         * @param value The value to compare the maximum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximumIndex(
                const std::vector<std::string> &variables, int startIndex, const std::string &index,
                Universe::UniverseRelationalOperator op, const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a maximum constraint.
         *
         * @param variables The variables to compute the maximum of.
         * @param startIndex The index at which to start looking for the maximum
         * @param index The variable encoding the index at which the maximum is.
         * @param op The relational operator to use to compare the maximum.
         * @param value The variable to compare the maximum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximumIndex(
                const std::vector<std::string> &variables, int startIndex, const std::string &index,
                Universe::UniverseRelationalOperator op, const std::string &value) override;

        /**
         * Adds to this solver a maximum constraint.
         *
         * @param variables The variables to compute the maximum of.
         * @param startIndex The index at which to start looking for the maximum
         * @param index The variable encoding the index at which the maximum is.
         * @param op The operator checking whether the maximum is in the range.
         * @param min The minimum value for the maximum.
         * @param max The maximum value for the maximum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximumIndex(
                const std::vector<std::string> &variables, int startIndex, const std::string &index,
                Universe::UniverseSetBelongingOperator op, const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a maximum constraint.
         *
         * @param variables The variables to compute the maximum of.
         * @param startIndex The index at which to start looking for the maximum
         * @param index The variable encoding the index at which the maximum is.
         * @param op The operator checking whether the maximum is in the set.
         * @param set The allowed values for the maximum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximumIndex(
                const std::vector<std::string> &variables, int startIndex, const std::string &index,
                Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a maximum constraint.
         *
         * @param intensionConstraints The intension constraints to compute the maximum of.
         * @param op The relational operator to use to compare the maximum.
         * @param value The value to compare the maximum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximumIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &intensionConstraints,
                Universe::UniverseRelationalOperator op, const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a maximum constraint.
         *
         * @param intensionConstraints The intension constraints to compute the maximum of.
         * @param op The relational operator to use to compare the maximum.
         * @param value The variable to compare the maximum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximumIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &intensionConstraints,
                Universe::UniverseRelationalOperator op, const std::string &value) override;

        /**
         * Adds to this solver a maximum constraint.
         *
         * @param intensionConstraints The intension constraints to compute the maximum of.
         * @param op The operator checking whether the maximum is in the range.
         * @param min The minimum value for the maximum.
         * @param max The maximum value for the maximum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximumIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &intensionConstraints,
                Universe::UniverseSetBelongingOperator op,
                const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a maximum constraint.
         *
         * @param intensionConstraints The intension constraints to compute the maximum of.
         * @param op The operator checking whether the maximum is in the set.
         * @param set The allowed values for the maximum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximumIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &intensionConstraints,
                Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a maximum-arg constraint.
         *
         * @param variables The variables to compute the maximum of.
         * @param op The relational operator to use to compare the maximum.
         * @param value The value to compare the maximum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximumArg(
                const std::vector<std::string> &variables, Universe::UniverseRelationalOperator op,
                const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a maximum-arg constraint.
         *
         * @param variables The variables to compute the maximum of.
         * @param op The relational operator to use to compare the maximum.
         * @param value The variable to compare the maximum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximumArg(
                const std::vector<std::string> &variables, Universe::UniverseRelationalOperator op,
                const std::string &value) override;

        /**
         * Adds to this solver a maximum-arg constraint.
         *
         * @param variables The variables to compute the maximum of.
         * @param op The operator checking whether the maximum is in the range.
         * @param min The minimum value for the maximum.
         * @param max The maximum value for the maximum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximumArg(
                const std::vector<std::string> &variables, Universe::UniverseSetBelongingOperator op,
                const Universe::BigInteger &min, const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a maximum-arg constraint.
         *
         * @param variables The variables to compute the maximum of.
         * @param op The operator checking whether the maximum is in the set.
         * @param set The allowed values for the maximum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximumArg(
                const std::vector<std::string> &variables, Universe::UniverseSetBelongingOperator op,
                const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a maximum-arg constraint.
         *
         * @param intensionConstraints The intension constraints to compute the maximum of.
         * @param op The relational operator to use to compare the maximum.
         * @param value The value to compare the maximum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximumArgIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &intensionConstraints,
                Universe::UniverseRelationalOperator op, const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a maximum-arg constraint.
         *
         * @param intensionConstraints The intension constraints to compute the maximum of.
         * @param op The relational operator to use to compare the maximum.
         * @param value The variable to compare the maximum with.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximumArgIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &intensionConstraints,
                Universe::UniverseRelationalOperator op, const std::string &value) override;

        /**
         * Adds to this solver a maximum-arg constraint.
         *
         * @param intensionConstraints The intension constraints to compute the maximum of.
         * @param op The operator checking whether the maximum is in the range.
         * @param min The minimum value for the maximum.
         * @param max The maximum value for the maximum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximumArgIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &intensionConstraints,
                Universe::UniverseSetBelongingOperator op, const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a maximum-arg constraint.
         *
         * @param intensionConstraints The intension constraints to compute the maximum of.
         * @param op The operator checking whether the maximum is in the set.
         * @param set The allowed values for the maximum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMaximumArgIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &intensionConstraints,
                Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver a no-overlap constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param length The length associated to the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addNoOverlap(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &length) override;

        /**
         * Adds to this solver a no-overlap constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param length The length associated to the variables.
         * @param zeroIgnored Whether 0-lengths should be ignored.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addNoOverlap(const std::vector<std::string> &variables,
                          const std::vector<Universe::BigInteger> &length, bool zeroIgnored) override;

        /**
         * Adds to this solver a no-overlap constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param length The variable for the length of the other variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addNoOverlapVariableLength(
                const std::vector<std::string> &variables, const std::vector<std::string> &length) override;

        /**
         * Adds to this solver a no-overlap constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param length The variable for the length of the other variables.
         * @param zeroIgnored Whether 0-lengths should be ignored.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addNoOverlapVariableLength(const std::vector<std::string> &variables,
                                        const std::vector<std::string> &length, bool zeroIgnored) override;

        /**
         * Adds to this solver a no-overlap constraint.
         *
         * @param xVariables The variables appearing in the constraint on the x-axis.
         * @param yVariables The variables appearing in the constraint on the y-axis.
         * @param xLength The variables for the lengths associated to the variables on the x-axis.
         * @param yLength The lengths associated to the variables on the y-axis.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addBiDimensionalNoOverlap(const std::vector<std::string> &xVariables,
                                       const std::vector<std::string> &yVariables, const std::vector<std::string> &xLength,
                                       const std::vector<Universe::BigInteger> &yLength) override;

        /**
         * Adds to this solver a no-overlap constraint.
         *
         * @param xVariables The variables appearing in the constraint on the x-axis.
         * @param yVariables The variables appearing in the constraint on the y-axis.
         * @param xLength The variables for the lengths associated to the variables on the x-axis.
         * @param yLength The lengths associated to the variables on the y-axis.
         * @param zeroIgnored Whether 0-lengths should be ignored.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addBiDimensionalNoOverlap(const std::vector<std::string> &xVariables,
                                       const std::vector<std::string> &yVariables, const std::vector<std::string> &xLength,
                                       const std::vector<Universe::BigInteger> &yLength, bool zeroIgnored) override;

        /**
         * Adds to this solver a no-overlap constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param length The length associated to the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMultiDimensionalNoOverlap(const std::vector<std::vector<std::string>> &variables,
                                          const std::vector<std::vector<Universe::BigInteger>> &length) override;

        /**
         * Adds to this solver a no-overlap constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param length The length associated to the variables.
         * @param zeroIgnored Whether 0-lengths should be ignored.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMultiDimensionalNoOverlap(const std::vector<std::vector<std::string>> &variables,
                                          const std::vector<std::vector<Universe::BigInteger>> &length, bool zeroIgnored) override;

        /**
         * Adds to this solver a no-overlap constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param length The variable for the length of the other variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMultiDimensionalNoOverlapVariableLength(
                const std::vector<std::vector<std::string>> &variables,
                const std::vector<std::vector<std::string>> &length) override;

        /**
         * Adds to this solver a no-overlap constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param length The variable for the length of the other variables.
         * @param zeroIgnored Whether 0-lengths should be ignored.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMultiDimensionalNoOverlapVariableLength(
                const std::vector<std::vector<std::string>> &variables,
                const std::vector<std::vector<std::string>> &length, bool zeroIgnored) override;

        /**
         * Adds to this solver an ordered constraint.
         *
         * @param variables The variables that should be ordered.
         * @param op The relational operator defining the order of the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addOrdered(
                const std::vector<std::string> &variables, Universe::UniverseRelationalOperator op) override;

        /**
         * Adds to this solver an ordered constraint.
         *
         * @param variables The variables that should be ordered.
         * @param lengths The lengths that must exist between two consecutive variables.
         * @param op The relational operator defining the order of the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addOrderedWithConstantLength(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &lengths,
                Universe::UniverseRelationalOperator op) override;

        /**
         * Adds to this solver an ordered constraint.
         *
         * @param variables The variables that should be ordered.
         * @param lengths The variables encoding the lengths that must exist between two consecutive variables.
         * @param op The relational operator defining the order of the variables.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addOrderedWithVariableLength(const std::vector<std::string> &variables,
                                          const std::vector<std::string> &lengths, Universe::UniverseRelationalOperator op) override;

        /**
         * Adds to this solver an all-equal constraint.
         *
         * @param variables The variables that should all be equal.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addAllEqual(const std::vector<std::string> &variables) override;

        /**
         * Adds to this solver an all-equal constraint.
         *
         * @param expressions The expressions that should all be equal.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addAllEqualIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions) override;

        /**
         * Adds to this solver a not-all-equal constraint.
         *
         * @param variables The variables that should not be all equal.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addNotAllEqual(const std::vector<std::string> &variables) override;

        /**
         * Adds to this solver a lex constraint.
         *
         * @param tuples The tuple of variables that should be lexicographically ordered.
         * @param op The relational operator defining the order of the tuples.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addLex(
                const std::vector<std::vector<std::string>> &tuples, Universe::UniverseRelationalOperator op) override;

        /**
         * Adds to this solver a lex constraint.
         *
         * @param variables The variables that should be lexicographically ordered.
         * @param limit The limits of the constraint.
         * @param op The relational operator defining the order of the tuples.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addLex(
                const std::vector<std::vector<std::string>> &variables,
                const std::vector<std::vector<Universe::BigInteger>> &limit,
                Universe::UniverseRelationalOperator op) override;

        /**
         * Adds to this solver a lex-matrix constraint.
         *
         * @param matrix The matrix of variables that should be lexicographically ordered.
         * @param op The relational operator defining the order in the matrix.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addLexMatrix(
                const std::vector<std::vector<std::string>> &matrix, Universe::UniverseRelationalOperator op) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param op The relational operator used in the constraint.
         * @param value The value of the right-hand side of the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSum(
                const std::vector<std::string> &variables, Universe::UniverseRelationalOperator op,
                const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param op The relational operator used in the constraint.
         * @param rightVariable The variable on the right-hand side of the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSum(
                const std::vector<std::string> &variables, Universe::UniverseRelationalOperator op,
                const std::string &rightVariable) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param op The set operator used in the constraint.
         * @param min The minimum value for the sum.
         * @param max The maximum value for the sum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSum(
                const std::vector<std::string> &variables, Universe::UniverseSetBelongingOperator op,
                const Universe::BigInteger &min, const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param op The set operator used in the constraint.
         * @param values The set of allowed values for the sum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSum(
                const std::vector<std::string> &variables, Universe::UniverseSetBelongingOperator op,
                const std::vector<Universe::BigInteger> &values) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param coefficients The coefficients of the variables in the sum.
         * @param op The relational operator used in the constraint.
         * @param value The value of the right-hand side of the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSum(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &coefficients,
                Universe::UniverseRelationalOperator op, const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param coefficients The coefficients of the variables in the sum.
         * @param op The relational operator used in the constraint.
         * @param rightVariable The variable on the right-hand side of the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSum(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &coefficients,
                Universe::UniverseRelationalOperator op, const std::string &rightVariable) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param coefficients The coefficients of the variables in the sum.
         * @param op The set operator used in the constraint.
         * @param min The minimum value for the sum.
         * @param max The maximum value for the sum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSum(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &coefficients,
                Universe::UniverseSetBelongingOperator op, const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param coefficients The coefficients of the variables in the sum.
         * @param op The set operator used in the constraint.
         * @param values The set of allowed values for the sum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSum(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &coefficients,
                Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &values) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param expressions The expressions appearing in the constraint.
         * @param op The relational operator used in the constraint.
         * @param value The value of the right-hand side of the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSumIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                Universe::UniverseRelationalOperator op,
                const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param expressions The expressions appearing in the constraint.
         * @param op The relational operator used in the constraint.
         * @param rightVariable The variable on the right-hand side of the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSumIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                Universe::UniverseRelationalOperator op, const std::string &rightVariable) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param expressions The expressions appearing in the constraint.
         * @param op The set operator used in the constraint.
         * @param min The minimum value for the sum.
         * @param max The maximum value for the sum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSumIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                Universe::UniverseSetBelongingOperator op, const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param expressions The expressions appearing in the constraint.
         * @param op The set operator used in the constraint.
         * @param values The set of allowed values for the sum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSumIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &values) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param expressions The expressions appearing in the constraint.
         * @param coefficients The coefficients of the variables in the sum.
         * @param op The relational operator used in the constraint.
         * @param value The value of the right-hand side of the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSumIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<Universe::BigInteger> &coefficients, Universe::UniverseRelationalOperator op,
                const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param expressions The expressions appearing in the constraint.
         * @param coefficients The coefficients of the variables in the sum.
         * @param op The relational operator used in the constraint.
         * @param rightVariable The variable on the right-hand side of the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSumIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<Universe::BigInteger> &coefficients,
                Universe::UniverseRelationalOperator op, const std::string &rightVariable) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param expressions The expressions appearing in the constraint.
         * @param coefficients The coefficients of the variables in the sum.
         * @param op The set operator used in the constraint.
         * @param min The minimum value for the sum.
         * @param max The maximum value for the sum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSumIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<Universe::BigInteger> &coefficients, Universe::UniverseSetBelongingOperator op,
                const Universe::BigInteger &min, const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param expressions The expressions appearing in the constraint.
         * @param coefficients The coefficients of the variables in the sum.
         * @param op The set operator used in the constraint.
         * @param values The set of allowed values for the sum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSumIntension(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<Universe::BigInteger> &coefficients,
                Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &values) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param coefficients The coefficients of the variables in the sum.
         * @param op The relational operator used in the constraint.
         * @param value The value of the right-hand side of the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSumWithVariableCoefficients(
                const std::vector<std::string> &variables, const std::vector<std::string> &coefficients,
                Universe::UniverseRelationalOperator op, const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param coefficients The coefficients of the variables in the sum.
         * @param op The relational operator used in the constraint.
         * @param rightVariable The variable on the right-hand side of the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSumWithVariableCoefficients(
                const std::vector<std::string> &variables, const std::vector<std::string> &coefficients,
                Universe::UniverseRelationalOperator op, const std::string &rightVariable) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param coefficients The coefficients of the variables in the sum.
         * @param op The set operator used in the constraint.
         * @param min The minimum value for the sum.
         * @param max The maximum value for the sum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSumWithVariableCoefficients(
                const std::vector<std::string> &variables, const std::vector<std::string> &coefficients,
                Universe::UniverseSetBelongingOperator op, const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param variables The variables appearing in the constraint.
         * @param coefficients The coefficients of the variables in the sum.
         * @param op The set operator used in the constraint.
         * @param values The set of allowed values for the sum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSumWithVariableCoefficients(
                const std::vector<std::string> &variables, const std::vector<std::string> &coefficients,
                Universe::UniverseSetBelongingOperator op, const std::vector<Universe::BigInteger> &values) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param expressions The expressions appearing in the constraint.
         * @param coefficients The coefficients of the variables in the sum.
         * @param op The relational operator used in the constraint.
         * @param value The value of the right-hand side of the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSumIntensionWithVariableCoefficients(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<std::string> &coefficients, Universe::UniverseRelationalOperator op,
                const Universe::BigInteger &value) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param expressions The expressions appearing in the constraint.
         * @param coefficients The coefficients of the variables in the sum.
         * @param op The relational operator used in the constraint.
         * @param rightVariable The variable on the right-hand side of the constraint.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSumIntensionWithVariableCoefficients(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<std::string> &coefficients, Universe::UniverseRelationalOperator op,
                const std::string &rightVariable) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param expressions The expressions appearing in the constraint.
         * @param coefficients The coefficients of the variables in the sum.
         * @param op The set operator used in the constraint.
         * @param min The minimum value for the sum.
         * @param max The maximum value for the sum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSumIntensionWithVariableCoefficients(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<std::string> &coefficients, Universe::UniverseSetBelongingOperator op,
                const Universe::BigInteger &min, const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a sum constraint.
         *
         * @param expressions The expressions appearing in the constraint.
         * @param coefficients The coefficients of the variables in the sum.
         * @param op The set operator used in the constraint.
         * @param values The set of allowed values for the sum.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addSumIntensionWithVariableCoefficients(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<std::string> &coefficients, Universe::UniverseSetBelongingOperator op,
                const std::vector<Universe::BigInteger> &values) override;

        /**
         * Adds to this solver a circuit constraint.
         *
         * @param variables The variables representing the circuit.
         * @param startIndex The index of the variable at which the circuit starts.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCircuit(const std::vector<std::string> &variables, int startIndex) override;

        /**
         * Adds to this solver a circuit constraint.
         *
         * @param variables The variables representing the circuit.
         * @param startIndex The index of the variable at which the circuit starts.
         * @param size The size of the expected circuit.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCircuit(
                const std::vector<std::string> &variables, int startIndex, const Universe::BigInteger &size) override;

        /**
         * Adds to this solver a circuit constraint.
         *
         * @param variables The variables representing the circuit.
         * @param startIndex The index of the variable at which the circuit starts.
         * @param size The variable encoding the size of the expected circuit.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addCircuit(
                const std::vector<std::string> &variables, int startIndex, const std::string &size) override;

        /**
         * Adds to this solver an mdd constraint.
         *
         * @param variables The variables encoding a path in the MDD.
         * @param transitions The transitions of the MDD.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addMDD(
                const std::vector<std::string> &variables, const std::vector<Universe::UniverseTransition> &transitions) override;

        /**
         * Adds to this solver a regular constraint.
         *
         * @param variables The variables encoding a path in the MDD.
         * @param transitions The transitions of the MDD.
         * @param startState The variable encoding the start state of the MDD.
         * @param finalStates The variables encoding the final states of the MDD.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addRegular(
                const std::vector<std::string> &variables, const std::vector<Universe::UniverseTransition> &transitions,
                const std::string &startState, const std::vector<std::string> &finalStates) override;

        /**
         * Adds to this solver a flow constraint.
         *
         * @param variables The variable encoding the flow.
         * @param balance The balance in the network, i.e., the difference between input and output flows.
         * @param edges The edges in the network.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addFlow(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &balance,
                const std::vector<std::vector<Universe::BigInteger>> &edges) override;

        /**
         * Adds to this solver a flow constraint.
         *
         * @param variables The variable encoding the flow.
         * @param balance The balance in the network, i.e., the difference between input and output flows.
         * @param edges The edges in the network.
         * @param weights The weights on the edges of the network.
         * @param op The operator comparing the total cost.
         * @param totalCost The total cost of the flow.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addFlow(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &balance,
                const std::vector<std::vector<Universe::BigInteger>> &edges,
                const std::vector<Universe::BigInteger> &weights, Universe::UniverseRelationalOperator op,
                const Universe::BigInteger &totalCost) override;

        /**
         * Adds to this solver a flow constraint.
         *
         * @param variables The variable encoding the flow.
         * @param balance The balance in the network, i.e., the difference between input and output flows.
         * @param edges The edges in the network.
         * @param weights The weights on the edges of the network.
         * @param op The operator comparing the total cost.
         * @param totalCost The variable encoding the total cost of the flow.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addFlow(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &balance,
                const std::vector<std::vector<Universe::BigInteger>> &edges,
                const std::vector<Universe::BigInteger> &weights,
                Universe::UniverseRelationalOperator op,
                const std::string &totalCost) override;

        /**
         * Adds to this solver a flow constraint.
         *
         * @param variables The variable encoding the flow.
         * @param balance The balance in the network, i.e., the difference between input and output flows.
         * @param edges The edges in the network.
         * @param weights The weights on the edges of the network.
         * @param op The operator comparing the total cost.
         * @param min The minimum total cost of the flow.
         * @param max The maximum total cost of the flow.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addFlow(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &balance,
                const std::vector<std::vector<Universe::BigInteger>> &edges,
                const std::vector<Universe::BigInteger> &weights,
                Universe::UniverseSetBelongingOperator op,
                const Universe::BigInteger &min,
                const Universe::BigInteger &max) override;

        /**
         * Adds to this solver a flow constraint.
         *
         * @param variables The variable encoding the flow.
         * @param balance The balance in the network, i.e., the difference between input and output flows.
         * @param edges The edges in the network.
         * @param weights The weights on the edges of the network.
         * @param op The operator comparing the total cost.
         * @param set The possible values for the total cost.
         *
         * @throws UniverseContradictionException If adding the constraint results in a trivial inconsistency.
         */
        void addFlow(
                const std::vector<std::string> &variables,
                const std::vector<Universe::BigInteger> &balance,
                const std::vector<std::vector<Universe::BigInteger>> &edges,
                const std::vector<Universe::BigInteger> &weights,
                Universe::UniverseSetBelongingOperator op,
                const std::vector<Universe::BigInteger> &set) override;

        /**
         * Adds to this solver an objective function to minimize the value assigned to a variable.
         *
         * @param variable The variable to minimize the value of.
         */
        void minimizeVariable(const std::string &variable) override;

        /**
         * Adds to this solver an objective function to minimize the value of an expression.
         *
         * @param expression The expression to minimize the value of.
         */
        void minimizeExpression(Universe::IUniverseIntensionConstraint *expression) override;

        /**
         * Adds to this solver an objective function to maximize the value assigned to a variable.
         *
         * @param variable The variable to maximize the value of.
         */
        void maximizeVariable(const std::string &variable) override;

        /**
         * Adds to this solver an objective function to maximize the value of an expression.
         *
         * @param expression The expression to maximize the value of.
         */
        void maximizeExpression(Universe::IUniverseIntensionConstraint *expression) override;

        /**
         * Adds to this solver an objective function to minimize a sum of variables.
         *
         * @param variables The variables to minimize the sum of.
         */
        void minimizeSum(const std::vector<std::string> &variables) override;

        /**
         * Adds to this solver an objective function to minimize a sum of variables.
         *
         * @param variables The variables to minimize the sum of.
         * @param coefficients The coefficients of the variables in the sum.
         */
        void minimizeSum(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &coefficients) override;

        /**
         * Adds to this solver an objective function to minimize a sum of expressions.
         *
         * @param expressions The expressions to minimize the sum of.
         */
        void minimizeExpressionSum(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions) override;

        /**
         * Adds to this solver an objective function to minimize a sum of expressions.
         *
         * @param expressions The expressions to minimize the sum of.
         * @param coefficients The coefficients of the expressions in the sum.
         */
        void minimizeExpressionSum(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<Universe::BigInteger> &coefficients) override;

        /**
         * Adds to this solver an objective function to maximize a sum of variables.
         *
         * @param variables The variables to maximize the sum of.
         */
        void maximizeSum(const std::vector<std::string> &variables) override;

        /**
         * Adds to this solver an objective function to maximize a sum of variables.
         *
         * @param variables The variables to maximize the sum of.
         * @param coefficients The coefficients of the variables in the sum.
         */
        void maximizeSum(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &coefficients) override;

        /**
         * Adds to this solver an objective function to maximize a sum of expressions.
         *
         * @param expressions The expressions to maximize the sum of.
         */
        void maximizeExpressionSum(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions) override;

        /**
         * Adds to this solver an objective function to maximize a sum of expressions.
         *
         * @param expressions The expressions to maximize the sum of.
         * @param coefficients The coefficients of the expressions in the sum.
         */
        void maximizeExpressionSum(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<Universe::BigInteger> &coefficients) override;

        /**
         * Adds to this solver an objective function to minimize a product of variables.
         *
         * @param variables The variables to minimize the product of.
         */
        void minimizeProduct(const std::vector<std::string> &variables) override;

        /**
         * Adds to this solver an objective function to minimize a product of variables.
         *
         * @param variables The variables to minimize the product of.
         * @param coefficients The coefficients of the variables in the product.
         */
        void minimizeProduct(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &coefficients) override;

        /**
         * Adds to this solver an objective function to minimize a product of expressions.
         *
         * @param expressions The expressions to minimize the product of.
         */
        void minimizeExpressionProduct(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions) override;

        /**
         * Adds to this solver an objective function to minimize a product of expressions.
         *
         * @param expressions The expressions to minimize the product of.
         * @param coefficients The coefficients of the expressions in the product.
         */
        void minimizeExpressionProduct(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<Universe::BigInteger> &coefficients) override;

        /**
         * Adds to this solver an objective function to maximize a product of variables.
         *
         * @param variables The variables to maximize the product of.
         */
        void maximizeProduct(const std::vector<std::string> &variables) override;

        /**
         * Adds to this solver an objective function to maximize a product of variables.
         *
         * @param variables The variables to maximize the product of.
         * @param coefficients The coefficients of the variables in the product.
         */
        void maximizeProduct(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &coefficients) override;

        /**
         * Adds to this solver an objective function to maximize a product of expressions.
         *
         * @param expressions The expressions to maximize the product of.
         */
        void maximizeExpressionProduct(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions) override;

        /**
         * Adds to this solver an objective function to maximize a product of expressions.
         *
         * @param expressions The expressions to maximize the product of.
         * @param coefficients The coefficients of the expressions in the product.
         */
        void maximizeExpressionProduct(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<Universe::BigInteger> &coefficients) override;

        /**
         * Adds to this solver an objective function to minimize the minimum of variables.
         *
         * @param variables The variables to minimize the minimum of.
         */
        void minimizeMinimum(const std::vector<std::string> &variables) override;

        /**
         * Adds to this solver an objective function to minimize the minimum of variables.
         *
         * @param variables The variables to minimize the minimum of.
         * @param coefficients The coefficients of the variables in the minimum.
         */
        void minimizeMinimum(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &coefficients) override;

        /**
         * Adds to this solver an objective function to minimize the minimum of expressions.
         *
         * @param expressions The expressions to minimize the minimum of.
         */
        void minimizeExpressionMinimum(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions) override;

        /**
         * Adds to this solver an objective function to minimize the minimum of expressions.
         *
         * @param expressions The expressions to minimize the minimum of.
         * @param coefficients The coefficients of the expressions in the minimum.
         */
        void minimizeExpressionMinimum(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<Universe::BigInteger> &coefficients) override;

        /**
         * Adds to this solver an objective function to maximize the minimum of variables.
         *
         * @param variables The variables to maximize the minimum of.
         */
        void maximizeMinimum(const std::vector<std::string> &variables) override;

        /**
         * Adds to this solver an objective function to maximize the minimum of variables.
         *
         * @param variables The variables to maximize the minimum of.
         * @param coefficients The coefficients of the variables in the minimum.
         */
        void maximizeMinimum(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &coefficients) override;

        /**
         * Adds to this solver an objective function to maximize the minimum of expressions.
         *
         * @param expressions The expressions to maximize the minimum of.
         */
        void maximizeExpressionMinimum(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions) override;

        /**
         * Adds to this solver an objective function to maximize the minimum of expressions.
         *
         * @param expressions The expressions to maximize the minimum of.
         * @param coefficients The coefficients of the expressions in the minimum.
         */
        void maximizeExpressionMinimum(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<Universe::BigInteger> &coefficients) override;

        /**
         * Adds to this solver an objective function to minimize the maximum of variables.
         *
         * @param variables The variables to minimize the maximum of.
         */
        void minimizeMaximum(const std::vector<std::string> &variables) override;

        /**
         * Adds to this solver an objective function to minimize the maximum of variables.
         *
         * @param variables The variables to minimize the maximum of.
         * @param coefficients The coefficients of the variables in the maximum.
         */
        void minimizeMaximum(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &coefficients) override;

        /**
         * Adds to this solver an objective function to minimize the maximum of expressions.
         *
         * @param expressions The expressions to minimize the maximum of.
         */
        void minimizeExpressionMaximum(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions) override;

        /**
         * Adds to this solver an objective function to minimize the maximum of expressions.
         *
         * @param expressions The expressions to minimize the maximum of.
         * @param coefficients The coefficients of the expressions in the maximum.
         */
        void minimizeExpressionMaximum(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<Universe::BigInteger> &coefficients) override;

        /**
         * Adds to this solver an objective function to maximize the maximum of variables.
         *
         * @param variables The variables to maximize the maximum of.
         */
        void maximizeMaximum(const std::vector<std::string> &variables) override;

        /**
         * Adds to this solver an objective function to maximize the maximum of variables.
         *
         * @param variables The variables to maximize the maximum of.
         * @param coefficients The coefficients of the variables in the maximum.
         */
        void maximizeMaximum(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &coefficients) override;

        /**
         * Adds to this solver an objective function to maximize the maximum of expressions.
         *
         * @param expressions The expressions to maximize the maximum of.
         */
        void maximizeExpressionMaximum(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions) override;

        /**
         * Adds to this solver an objective function to maximize the maximum of expressions.
         *
         * @param expressions The expressions to maximize the maximum of.
         * @param coefficients The coefficients of the expressions in the maximum.
         */
        void maximizeExpressionMaximum(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<Universe::BigInteger> &coefficients) override;

        /**
         * Adds to this solver an objective function to minimize the number of values assigned to variables.
         *
         * @param variables The variables to minimize the number of values of.
         */
        void minimizeNValues(const std::vector<std::string> &variables) override;

        /**
         * Adds to this solver an objective function to minimize the number of values assigned to variables.
         *
         * @param variables The variables to minimize the number of values of.
         * @param coefficients The coefficients of the variables.
         */
        void minimizeNValues(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &coefficients) override;

        /**
         * Adds to this solver an objective function to minimize the number of values assigned to expressions.
         *
         * @param expressions The expressions to minimize the number of values of.
         */
        void minimizeExpressionNValues(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions) override;

        /**
         * Adds to this solver an objective function to minimize the number of values assigned to expressions.
         *
         * @param expressions The expressions to minimize the number of values of.
         * @param coefficients The coefficients of the expressions.
         */
        void minimizeExpressionNValues(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<Universe::BigInteger> &coefficients) override;

        /**
         * Adds to this solver an objective function to maximize the number of values assigned to variables.
         *
         * @param variables The variables to maximize the number of values of.
         */
        void maximizeNValues(const std::vector<std::string> &variables) override;

        /**
         * Adds to this solver an objective function to maximize the number of values assigned to variables.
         *
         * @param variables The variables to maximize the number of values of.
         * @param coefficients The coefficients of the variables.
         */
        void maximizeNValues(
                const std::vector<std::string> &variables, const std::vector<Universe::BigInteger> &coefficients) override;

        /**
         * Adds to this solver an objective function to maximize the number of values assigned to expressions.
         *
         * @param expressions The expressions to maximize the number of values of.
         */
        void maximizeExpressionNValues(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions) override;

        /**
         * Adds to this solver an objective function to maximize the number of values assigned to expressions.
         *
         * @param expressions The expressions to maximize the number of values of.
         * @param coefficients The coefficients of the expressions.
         */
        void maximizeExpressionNValues(
                const std::vector<Universe::IUniverseIntensionConstraint *> &expressions,
                const std::vector<Universe::BigInteger> &coefficients) override;

    private:

        /**
         * Converts a vector of DIMACS literals to a vector of string identifiers.
         *
         * @param literals The vector of DIMACS literals.
         *
         * @return The vector of variable identifiers corresponding to literals.
         */
        std::vector<std::string> toString(std::vector<int> literals);

        /**
         * Determines the scope of an intension constraint.
         *
         * @param constraint The constraint to compute the scope of.
         *
         * @return The names of the variable in the scope of the constraint.
         */
        std::vector<std::string> scopeOf(Universe::IUniverseIntensionConstraint *constraint);

        /**
         * Determines the scope of a vector of intension constraints.
         *
         * @param constraints The constraints to compute the scope of.
         *
         * @return The names of the variable in the scope of the constraints.
         */
        std::vector<std::string> scopeOf(std::vector<Universe::IUniverseIntensionConstraint *> constraints);

        /**
         * Flattens a vector of variable scopes into a single vector of variables.
         *
         * @param scopes The scopes to flatten.
         *
         * @return The flattened vector of variable identifiers.
         */
        std::vector<std::string> flatten(const std::vector<std::vector<std::string>> &scopes);

        /**
         * Adds a constraint with the given scope.
         *
         * @param scope The scope of the constraint to add.
         */
        void addConstraint(const std::vector<std::string> &scope);

    protected:

        /**
         * Gives the dual hypergraph of the problem to solve.
         * If the dual hypergraph has not been built yet, it is built now.
         *
         * @return The dual hypergraph of the problem to solve.
         */
        Panoramyx::Hypergraph *getHypergraph();

    };

}

#endif
