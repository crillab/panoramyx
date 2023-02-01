/**
* @date 31/01/23
* @file RangeBaseAllocationStrategy.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_RANGEBASEALLOCATIONSTRATEGY_HPP
#define PANORAMYX_RANGEBASEALLOCATIONSTRATEGY_HPP

#include "IAllocationStrategy.hpp"
#include "../../utils/Stream.hpp"

namespace Panoramyx {

/**
 * @class RangeBaseAllocationStrategy
 *
 * @brief 
 * @file RangeBaseAllocationStrategy.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class RangeBaseAllocationStrategy:public IAllocationStrategy {
    private:
        std::function<Stream<Universe::BigInteger>*(Universe::BigInteger, Universe::BigInteger, int)> rangeIterator;

    public:
        explicit RangeBaseAllocationStrategy(std::function<Stream<Universe::BigInteger>*(Universe::BigInteger, Universe::BigInteger, int)> rangeIterator);

        std::vector<Universe::BigInteger> computeBoundAllocation(const std::vector<Universe::BigInteger> &currentBounds,
                                                                 const Universe::BigInteger &currentMin,
                                                                 const Universe::BigInteger &currentMax) override;

    };

} // Panoramyx

#endif //PANORAMYX_RANGEBASEALLOCATIONSTRATEGY_HPP