/**
* @date 07/12/22
* @file NullConsistencyChecker.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include "../../include/core/NullConsistencyChecker.hpp"

namespace Panoramyx {

/**
@class NullConsistencyChecker
@brief Definition of the class NullConsistencyChecker. 
@file NullConsistencyChecker.cpp
*/

    bool NullConsistencyChecker::checkPartial(
            const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) {
        return true;
    }

    bool NullConsistencyChecker::checkFinal(
            const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) {
        return true;
    }
} // Panoramyx