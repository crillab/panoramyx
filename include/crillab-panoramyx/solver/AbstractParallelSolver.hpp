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
 * @file AbstractParallelSolver.hpp
 * @brief The parent class of all the solvers that run in parallel.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_ABSTRACTPARALLELSOLVER_HPP
#define PANORAMYX_ABSTRACTPARALLELSOLVER_HPP

#include <map>
#include <ostream>
#include <vector>

#include <crillab-universe/core/IUniverseSolver.hpp>

#include "RemoteSolver.hpp"
#include "../network/Message.hpp"
#include "../optim/decomposition/IBoundAllocationStrategy.hpp"
#include "../utils/BlockingDeque.hpp"

namespace Panoramyx {

    /**
     * The AbstractParallelSolver is the parent class of all the solvers that run
     * in parallel.
     */
    class AbstractParallelSolver : public Universe::IUniverseSolver, public Universe::IOptimizationSolver {

    protected:

        /**
         * The interface used to communicate with the different solvers.
         */
        INetworkCommunication *communicator;

        /**
         * The allocation strategy used to allocate bounds to the different solvers.
         */
        Panoramyx::IBoundAllocationStrategy *allocationStrategy;

        /**
         * The solvers that run in parallel.
         */
        std::vector<Panoramyx::RemoteSolver *> solvers;

        /**
         * The vector telling which solvers are currently running.
         */
        std::vector<bool> currentRunningSolvers;

        /**
         * The number of solvers that are currently running.
         */
        int runningSolvers;

        /**
         * Whether the problem to solve is a minimization problem.
         */
        bool minimization;

        /**
         * The current lower bound of the optimization problem.
         */
        Universe::BigInteger lowerBound;

        /**
         * The current upper bound of the optimization problem.
         */
        Universe::BigInteger upperBound;

        /**
         * The bounds that are currently assigned to the different solvers.
         */
        std::vector<Universe::BigInteger> currentBounds;

        /**
         * The index of the solver that found the answer.
         */
        unsigned winner;

        /**
         * The result of the solving process.
         */
        Universe::UniverseSolverResult result;

        /**
         * The assignment corresponding to the current (best) solution.
         */
        std::map<std::string, Universe::BigInteger> bestSolution;

        /**
         * The mutex avoiding to read and write the best solution at the same time.
         */
        std::mutex solutionMutex;

        /**
         * The semaphore allowing to wait for a final answer from the solvers.
         */
        std::binary_semaphore solved;

        /**
         * Whether the solver has been interrupted.
         */
        bool interrupted;

        /**
         * The semaphore allowing to wait for the solvers to terminate properly.
         */
        std::binary_semaphore end;

    public:

        /**
         * Creates a new AbstractParallelSolver.
         *
         * @param comm The interface used to communicate with the different solvers.
         * @param allocationStrategy The allocation strategy used to allocate bounds to the different solvers.
         */
        explicit AbstractParallelSolver(Panoramyx::INetworkCommunication *comm,
                                        Panoramyx::IBoundAllocationStrategy *allocationStrategy = nullptr);

        /**
         * Destroys this AbstractParallelSolver.
         */
        ~AbstractParallelSolver() override = default;

        /**
         * Adds a solver to the set of solvers to run in parallel.
         *
         * @param solver The solver to add.
         */
        void addSolver(Panoramyx::RemoteSolver *solver);

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

    protected:

        /**
         * Reads (in a dedicated thread) all the messages that are received.
         */
        virtual void readMessages();

        /**
         * Reads a message, and performs the corresponding operation.
         *
         * @param message The message that has been received.
         */
        virtual void readMessage(const Message *message);

        /**
         * Prepares the solver at the given index to use it later on.
         *
         * @param solverIndex The index of the solver that is ready.
         */
        virtual void ready(unsigned solverIndex);

        /**
         * Applies some initialization before actually starting the search.
         */
        virtual void beforeSearch();

        /**
         * Applies some initialization to a particular solver before actually starting the search.
         *
         * @param solverIndex The index of the solver to initialize.
         */
        virtual void beforeSearch(unsigned solverIndex);

        /**
         * Actually starts the search performed by the different solvers.
         */
        virtual void startSearch() = 0;

        /**
         * Actually starts the search performed by the different solvers.
         *
         * @param assumpts The assumptions to apply when solving.
         */
        virtual void startSearch(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) = 0;

        /**
         * Updates the search when a solver has found a solution of its problem.
         *
         * @param solverIndex The index of the solver that found a solution.
         */
        virtual void onSatisfiableFound(unsigned int solverIndex);

        /**
         * Reconsiders the current task allocation when a new bound is found.
         *
         * @param bound The new bound that has been found.
         */
        virtual void onNewBoundFound(const Universe::BigInteger &bound, unsigned int i);

        /**
         * Updates the search when a solver proved the unsatisfiability of its problem.
         *
         * @param solverIndex The index of the solver that returned UNSATISFIABLE.
         */
        virtual void onUnsatisfiableFound(unsigned int solverIndex);

        /**
         * Updates the search when a solver did not find any solution.
         *
         * @param solverIndex The index of the solver that returned UNKNOWN.
         */
        virtual void onUnknown(unsigned int solverIndex);

        /**
         * Updates the bounds allocated to the different solvers when new information is obtained.
         */
        virtual void updateBounds();

        /**
         * Terminates the search.
         */
        virtual void endSearch();

    private:

        /**
         * Actually solves the problem associated to this solver.
         *
         * @param assumptions The assumptions to consider when solving.
         *
         * @return The outcome of the search conducted by the solver.
         */
        Universe::UniverseSolverResult internalSolve(
                const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts);

    };

    /**
     * Abraracourcix is the parent class of all the solvers that run in parallel.
     */
    using Abraracourcix = AbstractParallelSolver;

}

#endif
