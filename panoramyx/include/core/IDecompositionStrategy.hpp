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
#include "../../../libs/universe/universe/include/core/problem/IUniverseProblem.hpp"
#include "../utils/VectorStream.hpp"

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
        virtual Stream<Universe::IUniverseProblem*>* decompose(Universe::IUniverseProblem* problem)=0;

        virtual ~IDecompositionStrategy()=default;
    };

} // Panoramyx

#endif //PANORAMYX_IDECOMPOSITIONSTRATEGY_HPP
