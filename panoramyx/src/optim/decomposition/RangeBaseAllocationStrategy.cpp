/**
* @date 31/01/23
* @file RangeBaseAllocationStrategy.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include <utility>

#include "../../../include/optim/decomposition/RangeBaseAllocationStrategy.hpp"

namespace Panoramyx {

/**
@class RangeBaseAllocationStrategy
@brief Definition of the class RangeBaseAllocationStrategy. 
@file RangeBaseAllocationStrategy.cpp
*/

    RangeBaseAllocationStrategy::RangeBaseAllocationStrategy(std::function<Stream<Universe::BigInteger>*(Universe::BigInteger, Universe::BigInteger, int)> rangeIterator)
            : rangeIterator(std::move(rangeIterator)) {}

    std::vector<Universe::BigInteger>
    RangeBaseAllocationStrategy::computeBoundAllocation(const std::vector<Universe::BigInteger> &currentBounds,
                                                        const Universe::BigInteger &currentMin,
                                                        const Universe::BigInteger &currentMax) {
        std::vector<Universe::BigInteger> result;
        for(auto b:*rangeIterator(currentMin,currentMax,currentBounds.size()-1)){
            result.push_back(b);
        }
        return result;
    }


} // Panoramyx