/**
* @date 28/06/23
* @file ProblemUnderAssumption.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_PROBLEMUNDERASSUMPTION_HPP
#define PANORAMYX_PROBLEMUNDERASSUMPTION_HPP

#include <vector>
#include "../../../libs/autis/libs/universe/universe/include/core/UniverseType.hpp"
#include "../../../libs/autis/libs/universe/universe/include/core/UniverseAssumption.hpp"

namespace Panoramyx {

/**
 * @class ProblemUnderAssumption
 *
 * @brief 
 * @file ProblemUnderAssumption.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class ProblemUnderAssumption {
    public:
        ProblemUnderAssumption(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts,
                               long score);

        [[nodiscard]] const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &getAssumpts() const;

        bool operator<(const ProblemUnderAssumption &rhs) const;

        bool operator>(const ProblemUnderAssumption &rhs) const;

        bool operator<=(const ProblemUnderAssumption &rhs) const;

        bool operator>=(const ProblemUnderAssumption &rhs) const;


    private:
        std::vector<Universe::UniverseAssumption<Universe::BigInteger>> assumpts;
        long score;
    };

} // Panoramyx

#endif //PANORAMYX_PROBLEMUNDERASSUMPTION_HPP
