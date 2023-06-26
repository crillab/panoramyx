/**
* @date 31/01/23
* @file RangeBaseAllocationStrategy.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include <utility>
#include <cassert>

#include "../../../include/optim/decomposition/AggressiveRangeBaseAllocationStrategy.hpp"
#include "loguru.hpp"

namespace Panoramyx {

/**
@class RangeBaseAllocationStrategy
@brief Definition of the class RangeBaseAllocationStrategy. 
@file RangeBaseAllocationStrategy.cpp
*/

    AggressiveRangeBaseAllocationStrategy::AggressiveRangeBaseAllocationStrategy(
            std::function<Stream<Universe::BigInteger> *(Universe::BigInteger, Universe::BigInteger,
                                                         int)> rangeIterator)
            : rangeIterator(std::move(rangeIterator)), minimization(true) {}

    std::vector<Universe::BigInteger>
    AggressiveRangeBaseAllocationStrategy::computeBoundAllocation(
            const std::vector<Universe::BigInteger> &currentBounds,
            const Universe::BigInteger &currentMin,
            const Universe::BigInteger &currentMax) {


        std::vector<Universe::BigInteger> result;

        for (auto b: *rangeIterator(currentMin, currentMax, currentBounds.size() - 1)) {
            result.push_back(b);
            DLOG_F(INFO, "adding completely new bound %lld", b);
        }

        while(result.size()!=currentBounds.size()){
            result.push_back(result.back());
        }

        return result;
    }

    void AggressiveRangeBaseAllocationStrategy::setMinimization(bool min) {
        minimization = min;
    }


} // Panoramyx