/**
* @date 07/12/22
* @file PartialConsistencyChecker.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_PARTIALCONSISTENCYCHECKER_HPP
#define PANORAMYX_PARTIALCONSISTENCYCHECKER_HPP

#include "IConsistencyChecker.hpp"
#include "../../../libs/autis/libs/universe/universe/include/core/IUniverseSolver.hpp"

namespace Panoramyx {

/**
 * @class PartialConsistencyChecker
 *
 * @brief 
 * @file PartialConsistencyChecker.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class PartialConsistencyChecker:public IConsistencyChecker {
    private:
        Universe::IUniverseSolver* solver;
    public:
        explicit PartialConsistencyChecker(Universe::IUniverseSolver *solver);

        bool checkPartial(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) override;
        bool checkFinal(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) override;
    };

} // Panoramyx

#endif //PANORAMYX_PARTIALCONSISTENCYCHECKER_HPP
