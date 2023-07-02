/**
* @date 31/01/23
* @file LinearRangeIterator.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/

#include <cmath>
#include <iostream>
#include "crillab-panoramyx/optim/decomposition/LogarithmicRangeIterator.hpp"

namespace Panoramyx {

/**
@class LinearRangeIterator
@brief Definition of the class LinearRangeIterator. 
@file LinearRangeIterator.cpp
*/


    LogarithmicRangeIterator::LogarithmicRangeIterator(Universe::BigInteger min, Universe::BigInteger max,
                                                       int numberOfSteps, bool increasing)
            : minimum(min), currentValue(min), maximum(max), numberOfSteps(numberOfSteps + 1),
              scale((max - min) / std::log(2 + numberOfSteps)), currentStep(0), increasing(increasing) {

    }

    bool LogarithmicRangeIterator::hasNext() const {
        return currentStep < numberOfSteps;
    }

    Universe::BigInteger LogarithmicRangeIterator::next() {
        auto next = currentValue;

        // Moving to the next value.
        auto nextValue = computeNextValue(currentStep);
        currentStep++;

        if (currentStep == numberOfSteps) {
            return (Universe::BigInteger) maximum;
        }

        // Ensuring that the next value will be different.
        if (nextValue == currentValue) {
            currentValue += 1;

        } else {
            currentValue = nextValue;
        }

        return (Universe::BigInteger) next;

    }

    long double LogarithmicRangeIterator::computeNextValue(int step) {
        if (increasing) {
            return maximum - (std::log(numberOfSteps - step) * scale);
        }
        return minimum + (std::log(2 + step) * scale);
    }

} // Panoramyx