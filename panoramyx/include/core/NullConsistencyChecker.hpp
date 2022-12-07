/**
* @date 07/12/22
* @file NullConsistencyChecker.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_NULLCONSISTENCYCHECKER_HPP
#define PANORAMYX_NULLCONSISTENCYCHECKER_HPP

#include "IConsistencyChecker.hpp"

namespace Panoramyx {

/**
 * @class NullConsistencyChecker
 *
 * @brief 
 * @file NullConsistencyChecker.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class NullConsistencyChecker: public IConsistencyChecker {
    public:
        bool checkPartial(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) override;

        bool checkFinal(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) override;
    };

} // Panoramyx

#endif //PANORAMYX_NULLCONSISTENCYCHECKER_HPP
