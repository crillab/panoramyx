/**
* @date 07/12/22
* @file FinalConsistencyChecker.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include "../../include/core/FinalConsistencyChecker.hpp"

namespace Panoramyx {

/**
@class FinalConsistencyChecker
@brief Definition of the class FinalConsistencyChecker. 
@file FinalConsistencyChecker.cpp
*/

    FinalConsistencyChecker::FinalConsistencyChecker(Universe::IUniverseSolver *solver) : solver(solver) {}
    bool FinalConsistencyChecker::checkPartial(
            const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) {
        return true;
    }

    bool FinalConsistencyChecker::checkFinal(
            const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) {
        return solver->solve(assumpts)!=Universe::UniverseSolverResult::UNSATISFIABLE;
    }


} // Panoramyx