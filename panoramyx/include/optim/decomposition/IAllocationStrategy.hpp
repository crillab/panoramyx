/**
* @date 31/01/23
* @file IAllocationStrategy.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_IALLOCATIONSTRATEGY_HPP
#define PANORAMYX_IALLOCATIONSTRATEGY_HPP

#include <vector>
#include "../../../../libs/autis/libs/universe/universe/include/core/UniverseType.hpp"

namespace Panoramyx {

/**
 * @class IAllocationStrategy
 *
 * @brief 
 * @file IAllocationStrategy.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class IAllocationStrategy {
    public:
        virtual std::vector<Universe::BigInteger>
        computeBoundAllocation(const std::vector<Universe::BigInteger> &currentBounds,
                               const Universe::BigInteger &currentMin, const Universe::BigInteger &currentMax) = 0;

        virtual void setMinimization(bool min)=0;
    };

} // Panoramyx

#endif //PANORAMYX_IALLOCATIONSTRATEGY_HPP
