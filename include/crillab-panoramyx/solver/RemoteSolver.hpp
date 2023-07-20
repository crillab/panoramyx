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
 * @file RemoteSolver.hpp
 * @brief Provides a solver implementation encapsulating the communication with a remote solver.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_REMOTESOLVER_HPP
#define PANORAMYX_REMOTESOLVER_HPP

#include <map>
#include <mutex>
#include <optional>
#include <ostream>
#include <vector>

#include <loguru/loguru.hpp>

#include <crillab-universe/core/IUniverseSolver.hpp>
#include <crillab-universe/optim/IOptimizationSolver.hpp>

#include "../network/INetworkCommunication.hpp"
#include "../network/MessageBuilder.hpp"

namespace Panoramyx {

    /**
     * The RemoteSolver provides a solver implementation encapsulating the communication
     * with a remote solver.
     */
    class RemoteSolver : public Universe::IUniverseSolver, public Universe::IOptimizationSolver {

    private:

        /**
         * The network communication used to communicate with the (real) remote solver.
         */
        Panoramyx::INetworkCommunication *communicator;

        /**
         * The mutex guaranteeing that two messages will not interfere.
         */
        std::mutex mutex;

        /**
         * The rank of this solver among all remote solvers (as assigned by the network communication strategy).
         */
        int rank;

        /**
         * The index of this solver among all remote solvers (as assigned by the main solver).
         */
        unsigned index;

        /**
         * The number of variables in the solver (the value is cached).
         */
        int nbVariables = -1;

        /**
         * The auxiliary variables declared in the solver (the vector is cached).
         */
        std::vector<std::string> auxiliaryVariables;

        /**
         * The number of constraints in the solver (the value is cached).
         */
        int nbConstraints = -1;

        /**
         * Whether the problem to solve is an optimization problem.
         * It is an optional because it is lazily cached.
         */
        std::optional<bool> optimization;

    public:

        /**
         * Creates a new RemoteSolver.
         *
         * @param rank The rank of this solver among all remote solvers (as assigned by the network
         *        communication strategy).
         */
        explicit RemoteSolver(int rank);

        /**
         * Destroys this RemoteSolver.
         */
        ~RemoteSolver() override = default;

        /**
         * Sets the index of this solver, as assigned by the main solver.
         *
         * @param i The index to set.
         */
        void setIndex(unsigned i);

        /**
         * Gives the index of this solver, as assigned by the main solver.
         *
         * @return The index of the solver.
         */
        unsigned int getIndex() const;

        /**
         * Sets the network communication used to communicate with the (real) remote solver.
         *
         * @param communicator The communicator to set.
         */
        void setCommunicator(INetworkCommunication *communicator);

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
         * Gives the number of constraints defined in this solver.
         *
         * @return The number of constraints.
         */
        int nConstraints() override;

        /**
         * Gives the list of the constraints in this solver.
         *
         * @return The list of the constraints.
         */
        const std::vector<Universe::IUniverseConstraint *> &getConstraints() override;

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
         * Loads a problem instance from a file.
         */
        void loadInstance(const std::string &filename) override;

        /**
         * Solves the problem associated to this solver.
         *
         * @return The outcome of the search conducted by the solver.
         */
        Universe::UniverseSolverResult solve() override;

        /**
         * Solves the problem stored in the given file.
         * The solver is expected to parse the problem itself.
         *
         * @param filename The name of the file containing the problem to solve.
         *
         * @return The outcome of the search conducted by the solver.
         */
        Universe::UniverseSolverResult solve(const std::string &filename) override;

        /**
         * Solves the problem associated to this solver.
         *
         * @param assumptions The assumptions to consider when solving.
         *
         * @return The outcome of the search conducted by the solver.
         */
        Universe::UniverseSolverResult solve(
                const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumptions) override;

        /**
         * Interrupts (asynchronously) the search currently performed by this solver.
         */
        void interrupt() override;

        /**
         * Terminates the search performed by the remote solver.
         */
        void endSearch();

        /**
         * Gives the current result obtained by this solver so far.
         *
         * @return The result obtained by this solver.
         */
        Universe::UniverseSolverResult getResult();

        /**
         * Gives the solution found by this solver (if any).
         *
         * @return The solution found by this solver.
         */
        std::vector<Universe::BigInteger> solution() override;

        /**
         * Gives the mapping between the name of a variable and the assignment found for this
         * variable by this solver (if any).
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
        bool checkSolution()  override;

        /**
         * Checks whether the given assignment is a solution of the problem.
         * Said differently, this method ensures that the given assignment satisfies all the
         * constraints of the problem solved by the solver.
         *
         * @param assignment The assignment to check as a solution.
         *
         * @return Whether the given assignment is a solution of the problem.
         */
        bool checkSolution(const std::map<std::string, Universe::BigInteger> &assignment)  override;

        /**
         * Casts this solver into an IOptimizationSolver.
         *
         * @return The casted optimization solver.
         */
        Universe::IOptimizationSolver *toOptimizationSolver() override;

        /**
         * Checks whether the optimization problem is a minimization problem.
         *
         * @return Whether the underlying problem is a minimization problem.
         */
        bool isMinimization() override;

        /**
         * Sets the bounds for the optimization problem to solve.
         *
         * @param lb The lower bound to set.
         * @param ub The upper bound to set.
         */
        void setBounds(const Universe::BigInteger &lb, const Universe::BigInteger &ub) override;

        /**
         * Sets the lower bound for the optimization problem to solve.
         *
         * @param lb The lower bound to set.
         */
        void setLowerBound(const Universe::BigInteger &lb) override;

        /**
         * Gives the current (best) lower bound of the underlying optimization problem.
         *
         * @return The current lower bound.
         */
        Universe::BigInteger getLowerBound() override;

        /**
         * Sets the upper bound for the optimization problem to solve.
         *
         * @param ub The upper bound to set.
         */
        void setUpperBound(const Universe::BigInteger &ub) override;

        /**
         * Gives the current (best) upper bound of the underlying optimization problem.
         *
         * @return The current upper bound.
         */
        Universe::BigInteger getUpperBound() override;

        /**
         * Gives the current (best) bound that have been found by this solver.
         * It is the current lower or upper bound, depending on whether the problem is a
         * minimization or maximization problem.
         *
         * @return The current bound.
         */
        Universe::BigInteger getCurrentBound() override;

    };

}

#endif
