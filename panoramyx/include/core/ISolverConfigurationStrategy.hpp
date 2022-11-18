/**
* @date 11/10/22
* @file ISolverConfigurationStrategy.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_ISOLVERCONFIGURATIONSTRATEGY_HPP
#define PANORAMYX_ISOLVERCONFIGURATIONSTRATEGY_HPP

#include "Configuration.hpp"
#include "../../../libs/autis/libs/universe/universe/include/core/problem/IUniverseProblem.hpp"
#include "../../../libs/autis/libs/universe/universe/include/core/IUniverseSolver.hpp"

namespace Panoramyx {

/**
 * @class ISolverConfigurationStrategy
 *
 * @brief 
 * @file ISolverConfigurationStrategy.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class ISolverConfigurationStrategy {
    public:
        virtual Universe::IUniverseSolver *configure(Universe::IUniverseProblem *problem, Configuration &config) = 0;
        virtual ~ISolverConfigurationStrategy() = default;
    };

} // Panoramyx

#endif //PANORAMYX_ISOLVERCONFIGURATIONSTRATEGY_HPP
