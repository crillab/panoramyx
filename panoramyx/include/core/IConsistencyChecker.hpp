/**
* @date 07/12/22
* @file IConsistencyChecker.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_ICONSISTENCYCHECKER_HPP
#define PANORAMYX_ICONSISTENCYCHECKER_HPP

#include <vector>
#include "../../../libs/autis/libs/universe/universe/include/core/UniverseType.hpp"
#include "../../../libs/autis/libs/universe/universe/include/core/UniverseAssumption.hpp"

namespace Panoramyx {

/**
 * @class IConsistencyChecker
 *
 * @brief 
 * @file IConsistencyChecker.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class IConsistencyChecker {
    public:
        virtual bool checkPartial(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>>& assumpts) = 0;
        virtual bool checkFinal(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>>& assumpts) = 0;
    };

} // Panoramyx

#endif //PANORAMYX_ICONSISTENCYCHECKER_HPP
