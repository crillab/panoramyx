/**
* @date 31/01/23
* @file LinearRangeIterator.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_LOGARITHMICRANGEITERATOR_HPP
#define PANORAMYX_LOGARITHMICRANGEITERATOR_HPP

#include "../../../../libs/autis/libs/universe/universe/include/core/UniverseType.hpp"
#include "../../utils/Stream.hpp"

namespace Panoramyx {

/**
 * @class LinearRangeIterator
 *
 * @brief 
 * @file LinearRangeIterator.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class LogarithmicRangeIterator : public Stream<Universe::BigInteger> {
    private:
        long double minimum;

        /**
         * The current value in the interval on which the iteration is performed.
         */
        long double currentValue;

        /**
         * The maximum value of the interval on which the iteration is performed
         * (exclusive).
         */
        long double maximum;

        /**
         * The number of steps for the iteration.
         */
        int numberOfSteps;

        /**
         * The scale to apply so as to iterate logarithmically over the values of the
         * interval.
         */
        long double scale;

        /**
         * The index of the current iteration step.
         */
        int currentStep;

        bool increasing;


    public:

        LogarithmicRangeIterator(Universe::BigInteger min, Universe::BigInteger max, int numberOfSteps, bool increasing = true);

        ~LogarithmicRangeIterator() override = default;

        [[nodiscard]] bool hasNext() const override;

        Universe::BigInteger next() override;

    protected:

        virtual long double computeNextValue(int step);
    };

} // Panoramyx

#endif
