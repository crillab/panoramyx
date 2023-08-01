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
 * @file PanoramyxSolver.hpp
 * @brief Defines an interface for the solvers that are used in a parallel solver.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <crillab-universe/core/IUniverseSolver.hpp>
#include <crillab-universe/optim/IOptimizationSolver.hpp>

#include "../network/INetworkCommunication.hpp"

#ifndef PANORAMYX_PANORAMYXSOLVER_HPP
#define PANORAMYX_PANORAMYXSOLVER_HPP

namespace Panoramyx {

    /**
     * The PanoramyxSolver defines an interface for the solvers that are used in a parallel solver.
     */
    class PanoramyxSolver : public Universe::IUniverseSolver, public Universe::IOptimizationSolver{

    public:

        /**
         * Sets the index of this solver, as assigned by the main solver.
         *
         * @param i The index to set.
         */
        virtual void setIndex(unsigned int i) = 0;

        /**
         * Gives the index of this solver, as assigned by the main solver.
         *
         * @return The index of the solver.
         */
        virtual unsigned int getIndex() const = 0;

        /**
         * Sets the network communication used to communicate with the underlying solver.
         *
         * @param communicator The communicator to set.
         */
        virtual void setCommunicator(Panoramyx::INetworkCommunication *communicator) = 0;

        /**
         * Terminates the search performed by this solver.
         */
        virtual void endSearch() = 0;

        /**
         * Gives the current result obtained by this solver so far.
         *
         * @return The result obtained by this solver.
         */
        virtual Universe::UniverseSolverResult getResult() = 0;

    };

}

#endif
