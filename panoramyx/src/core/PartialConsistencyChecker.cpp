/**
* @date 07/12/22
* @file PartialConsistencyChecker.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include "../../include/core/PartialConsistencyChecker.hpp"

namespace Panoramyx {

/**
@class PartialConsistencyChecker
@brief Definition of the class PartialConsistencyChecker. 
@file PartialConsistencyChecker.cpp
*/

    PartialConsistencyChecker::PartialConsistencyChecker(Universe::IUniverseSolver *solver) : solver(solver) {}

    bool PartialConsistencyChecker::checkPartial(
            const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) {
        auto result = solver->solve(assumpts)!=Universe::UniverseSolverResult::UNSATISFIABLE;
        solver->reset();
        return result;
    }

    bool PartialConsistencyChecker::checkFinal(
            const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) {
        return true;
    }

} // Panoramyx