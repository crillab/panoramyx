/**
* @date 31/01/23
* @file LinearRangeIterator.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include "crillab-panoramyx/optim/decomposition/LinearRangeIterator.hpp"

namespace Panoramyx {

/**
@class LinearRangeIterator
@brief Definition of the class LinearRangeIterator. 
@file LinearRangeIterator.cpp
*/


    LinearRangeIterator::LinearRangeIterator(Universe::BigInteger min, Universe::BigInteger max, int numberOfSteps)
            : current(min), max(max), step(numberOfSteps == 0 ? (max - min): std::max((max - (min + 1)) / numberOfSteps, (Universe::BigInteger) 1)) {}

    bool LinearRangeIterator::hasNext() const {
        return current < max;
    }

    Universe::BigInteger LinearRangeIterator::next() {
        auto n = current;
        current += step;
        return n;
    }

} // Panoramyx