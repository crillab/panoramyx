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
 * @file PortfolioSolverBuilder.hpp
 * @brief The builder for portfolio solvers.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_PORTFOLIOSOLVERBUILDER_HPP
#define PANORAMYX_PORTFOLIOSOLVERBUILDER_HPP

#include "AbstractSolverBuilder.hpp"

namespace Panoramyx {

    /**
     * The PortfolioSolverBuilder allows to build portfolio solvers.
     */
    class PortfolioSolverBuilder : public Panoramyx::AbstractSolverBuilder {

    private:

        /**
         * The bound allocation strategy used to distribute objective bounds among the solvers.
         */
        Panoramyx::IBoundAllocationStrategy *allocationStrategy;

    public:

        /**
         * Sets the bound allocation strategy to use to distribute objective bounds among the solvers.
         *
         * @param allocationStrategy The bound allocation strategy to use.
         *
         * @return This builder.
         */
        Panoramyx::PortfolioSolverBuilder *withAllocationStrategy(
                Panoramyx::IBoundAllocationStrategy *allocationStrategy);

        /**
         * Builds the solver.
         *
         * @return The built solver.
         */
        Panoramyx::AbstractParallelSolver *build() override;

    };

}

#endif
