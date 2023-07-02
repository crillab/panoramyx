/**
* @date 31/01/23
* @file LinearRangeIterator.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_LINEARRANGEITERATOR_HPP
#define PANORAMYX_LINEARRANGEITERATOR_HPP

#include <crillab-universe/core/UniverseType.hpp>
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

    class LinearRangeIterator:public Stream<Universe::BigInteger> {
    private:
        Universe::BigInteger current;
        Universe::BigInteger max;
        Universe::BigInteger step;

    public:

        LinearRangeIterator(Universe::BigInteger min, Universe::BigInteger max, int numberOfSteps);

        ~LinearRangeIterator() override = default;

        [[nodiscard]] bool hasNext() const override;

        Universe::BigInteger next() override;
    };

} // Panoramyx

#endif //PANORAMYX_LINEARRANGEITERATOR_HPP
