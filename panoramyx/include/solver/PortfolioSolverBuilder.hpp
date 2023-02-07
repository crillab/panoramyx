/**
* @date 07/02/23
* @file PortfolioSolverBuilder.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_PORTFOLIOSOLVERBUILDER_HPP
#define PANORAMYX_PORTFOLIOSOLVERBUILDER_HPP

#include "AbstractSolverBuilder.hpp"

namespace Panoramyx {

/**
 * @class PortfolioSolverBuilder
 *
 * @brief 
 * @file PortfolioSolverBuilder.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class PortfolioSolverBuilder:public AbstractSolverBuilder {
    private:
        IAllocationStrategy* allocationStrategy;
    public:
        PortfolioSolverBuilder* withAllocationStrategy(IAllocationStrategy* allocationStrategy);
        AbstractParallelSolver *build() override;
    };

} // Panoramyx

#endif //PANORAMYX_PORTFOLIOSOLVERBUILDER_HPP
