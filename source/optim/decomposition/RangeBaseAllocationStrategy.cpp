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
#include <iostream>

#include "crillab-panoramyx/optim/decomposition/RangeBaseAllocationStrategy.hpp"
#include "loguru.hpp"

namespace Panoramyx {

/**
@class RangeBaseAllocationStrategy
@brief Definition of the class RangeBaseAllocationStrategy. 
@file RangeBaseAllocationStrategy.cpp
*/

    RangeBaseAllocationStrategy::RangeBaseAllocationStrategy(
            std::function<Stream<Universe::BigInteger> *(Universe::BigInteger, Universe::BigInteger,
                                                         int)> rangeIterator)
            : rangeIterator(std::move(rangeIterator)), minimization(true) {}

    std::vector<Universe::BigInteger>
    RangeBaseAllocationStrategy::computeBoundAllocation(const std::vector<Universe::BigInteger> &currentBounds,
                                                        const Universe::BigInteger &currentMin,
                                                        const Universe::BigInteger &currentMax) {

        unsigned long indexLower = -1;
        unsigned long indexUpper = 0;
        for (int i = 0; i < currentBounds.size() - 1; i++) {
            if (currentBounds[i + 1] >= currentMax) {
                indexUpper = i;
                break;
            } else if (currentBounds[i] > currentMin && indexLower < 0) {
                indexLower = i;
            }
        }

        std::vector<Universe::BigInteger> result;
        if (currentBounds[indexLower] - currentMin < indexLower || currentMax - currentBounds[indexUpper] < currentBounds.size() - 1 - indexUpper) {
            // Not enough bounds: recomputing all bounds.

            for (auto b: *rangeIterator(currentMin, currentMax, currentBounds.size() - 1)) {
                result.push_back(b);
                DLOG_F(INFO, "adding completely new bound %lld", b);
            }
            return result;
        }

        for (auto b: *rangeIterator(currentMin, currentBounds[indexLower], indexLower)) {
            result.push_back(b);
            DLOG_F(INFO, "adding completely new bound %lld", b);
        }
        for (int i = indexLower + 1; i < indexUpper; i++) {
            result.push_back(currentBounds[i]);
            DLOG_F(INFO, "reusing bound %lld", currentBounds[i]);
        }

        for (auto b: *rangeIterator(currentBounds[indexUpper], currentMax, currentBounds.size() - 1 - indexUpper)) {
            result.push_back(b);
            DLOG_F(INFO, "adding completely new bound %lld", b);
        }

        while (result.size() < currentBounds.size()) {
            result.push_back(currentMax);
        }
        assert(result.size() == currentBounds.size());
        return result;
    }

    void RangeBaseAllocationStrategy::setMinimization(bool min) {
        minimization = min;
    }


} // Panoramyx