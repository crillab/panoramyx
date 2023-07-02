/**
* @date 31/01/23
* @file RangeBaseAllocationStrategy.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_AGGRESSIVERANGEBASEALLOCATIONSTRATEGY_HPP
#define PANORAMYX_AGGRESSIVERANGEBASEALLOCATIONSTRATEGY_HPP

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

    class AggressiveRangeBaseAllocationStrategy:public IAllocationStrategy {
    private:
        std::function<Stream<Universe::BigInteger>*(Universe::BigInteger, Universe::BigInteger, int)> rangeIterator;
        bool minimization;

    public:
        explicit AggressiveRangeBaseAllocationStrategy(std::function<Stream<Universe::BigInteger>*(Universe::BigInteger, Universe::BigInteger, int)> rangeIterator);

        std::vector<Universe::BigInteger> computeBoundAllocation(const std::vector<Universe::BigInteger> &currentBounds,
                                                                 const Universe::BigInteger &currentMin,
                                                                 const Universe::BigInteger &currentMax) override;

        void setMinimization(bool min) override;

    };

} // Panoramyx

#endif
