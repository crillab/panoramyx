/**
* @date 10/10/22
* @file IDecompositionStrategy.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_IDECOMPOSITIONSTRATEGY_HPP
#define PANORAMYX_IDECOMPOSITIONSTRATEGY_HPP

#include <vector>
#include "../utils/VectorStream.hpp"
#include "../../../libs/autis/libs/universe/universe/include/core/problem/IUniverseProblem.hpp"

namespace Panoramyx {

/**
 * @class IDecompositionStrategy
 *
 * @brief 
 * @file IDecompositionStrategy.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class IDecompositionStrategy {
    public:
        /**
         * Decomposes the given problem into several sub-problems.
         *
         * @param problem The problem to decompose.
         *
         * @return The stream of the sub-problems that have been generated.
         */
        virtual Stream<Universe::IUniverseProblem*>* decompose(Universe::IUniverseProblem* problem)=0;

        virtual ~IDecompositionStrategy()=default;
    };

} // Panoramyx

#endif //PANORAMYX_IDECOMPOSITIONSTRATEGY_HPP
