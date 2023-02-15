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

#include <vector>

#include "../../../libs/autis/libs/universe/universe/include/core/IUniverseSolver.hpp"

#include "RemoteSolver.hpp"
#include "../network/Message.hpp"
#include "../optim/decomposition/IAllocationStrategy.hpp"
#include "../utils/BlockingDeque.hpp"

namespace Panoramyx {

    /**
     * The AbstractParallelSolver is the parent class of all the solvers that run
     * in parallel.
     */
    class AbstractParallelSolver : public Universe::IUniverseSolver, public Universe::IOptimizationSolver {

    protected:
        std::map<std::string, Universe::BigInteger> bestSolution;
        /**
         * The interface used to communicate with the different solvers.
         */
        INetworkCommunication *communicator;

        /**
         * The solvers that run in parallel.
         */
        std::vector<Panoramyx::RemoteSolver *> solvers;

        /**
         * The number of solvers that are currently running.
         */
        int runningSolvers;

        /**
         * The allocation strategy used to allocate bounds to the different solvers.
         */
        Panoramyx::IAllocationStrategy *allocationStrategy;

        /**
         * The bounds that are currently assigned to the different solvers.
         */
        std::vector<Universe::BigInteger> currentBounds;

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
         * Whether the solver has been interrupted.
         */
        bool interrupted;

        /**
         * The semaphore allowing to wait for a final answer from the solvers.
         */
        std::binary_semaphore solved;

        /**
         * The result of the solving process.
         */
        Universe::UniverseSolverResult result;

        /**
         * The index of the solver that found the answer.
         */
        unsigned winner;

        /**
         * The semaphore allowing to wait for the solvers to terminate properly.
         */
        std::binary_semaphore end;

        std::vector<bool> currentRunningSolvers;

    public:

        /**
         * Creates a new AbstractParallelSolver.
         *
         * @param comm The interface used to communicate with the different solvers.
         * @param allocationStrategy The allocation strategy used to allocate bounds to the different solvers.
         */
        explicit AbstractParallelSolver(
                Panoramyx::INetworkCommunication *comm, Panoramyx::IAllocationStrategy *allocationStrategy = nullptr);

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
         * Loads a problem instance from a file.
         */
        virtual void loadInstance(const std::string &filename);

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
         * Gives the number of constraints defined in this solver.
         *
         * @return The number of constraints.
         */
        int nConstraints() override;

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
         * Sets the log file to be used by the solver.
         *
         * @param filename The name of the log file.
         */
        void setLogFile(const std::string &filename) override;

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

        Universe::UniverseSolverResult getResult();

        Universe::BigInteger getCurrentBound() override;

        Universe::BigInteger getLowerBound() override;

        void setLowerBound(const Universe::BigInteger &lb) override;

        Universe::BigInteger getUpperBound() override;

        void setUpperBound(const Universe::BigInteger &ub) override;

        void setBounds(const Universe::BigInteger &lb, const Universe::BigInteger &ub) override;

        bool isMinimization() override;

        bool isOptimization() override;

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
