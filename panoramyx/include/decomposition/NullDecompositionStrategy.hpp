/**
* @date 11/10/22
* @file NullDecompositionStrategy.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_NULLDECOMPOSITIONSTRATEGY_HPP
#define PANORAMYX_NULLDECOMPOSITIONSTRATEGY_HPP

#include "../core/IDecompositionStrategy.hpp"

namespace Panoramyx {

/**
 * @class NullDecompositionStrategy
 *
 * @brief 
 * @file NullDecompositionStrategy.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class NullDecompositionStrategy : public IDecompositionStrategy{
    public:
        Stream<Universe::IUniverseProblem *> *decompose(Universe::IUniverseProblem *problem) override;
    };

} // Panoramyx

#endif //PANORAMYX_NULLDECOMPOSITIONSTRATEGY_HPP
