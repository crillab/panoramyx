/**
* @date 07/02/23
* @file PortfolioSolverBuilder.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include "crillab-panoramyx/solver/PortfolioSolverBuilder.hpp"
#include "crillab-panoramyx/solver/PortfolioSolver.hpp"

namespace Panoramyx {

/**
@class PortfolioSolverBuilder
@brief Definition of the class PortfolioSolverBuilder. 
@file PortfolioSolverBuilder.cpp
*/

    AbstractParallelSolver *PortfolioSolverBuilder::build() {
        return new PortfolioSolver(networkCommunication,allocationStrategy);
    }

    PortfolioSolverBuilder *PortfolioSolverBuilder::withAllocationStrategy(IAllocationStrategy *allocationStrategy) {
        this->allocationStrategy=allocationStrategy;
        return this;
    }
} // Panoramyx