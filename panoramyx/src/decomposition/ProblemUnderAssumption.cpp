/**
* @date 28/06/23
* @file ProblemUnderAssumption.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include "../../include/decomposition/ProblemUnderAssumption.hpp"

namespace Panoramyx {

/**
@class ProblemUnderAssumption
@brief Definition of the class ProblemUnderAssumption. 
@file ProblemUnderAssumption.cpp
*/
    ProblemUnderAssumption::ProblemUnderAssumption(
            const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts, long score) : assumpts(
            assumpts), score(score) {}

    const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &ProblemUnderAssumption::getAssumpts() const {
        return assumpts;
    }

    bool ProblemUnderAssumption::operator<(const ProblemUnderAssumption &rhs) const {
        return score < rhs.score;
    }

    bool ProblemUnderAssumption::operator>(const ProblemUnderAssumption &rhs) const {
        return rhs < *this;
    }

    bool ProblemUnderAssumption::operator<=(const ProblemUnderAssumption &rhs) const {
        return !(rhs < *this);
    }

    bool ProblemUnderAssumption::operator>=(const ProblemUnderAssumption &rhs) const {
        return !(*this < rhs);
    }


} // Panoramyx