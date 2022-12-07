/**
* @date 07/12/22
* @file FinalConsistencyChecker.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_FINALCONSISTENCYCHECKER_HPP
#define PANORAMYX_FINALCONSISTENCYCHECKER_HPP

#include "IConsistencyChecker.hpp"
#include "../../../libs/autis/libs/universe/universe/include/core/IUniverseSolver.hpp"

namespace Panoramyx {

/**
 * @class FinalConsistencyChecker
 *
 * @brief 
 * @file FinalConsistencyChecker.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class FinalConsistencyChecker:public IConsistencyChecker {
    private:
        Universe::IUniverseSolver* solver;
    public:
        explicit FinalConsistencyChecker(Universe::IUniverseSolver *solver);

        bool checkPartial(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) override;

        bool checkFinal(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) override;
    };

} // Panoramyx

#endif //PANORAMYX_FINALCONSISTENCYCHECKER_HPP
