/**
* @date 05/12/22
* @file AbstractCubeGenerator.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include "../../include/decomposition/AbstractCubeGenerator.hpp"

namespace Panoramyx {

/**
@class AbstractCubeGenerator
@brief Definition of the class AbstractCubeGenerator. 
@file AbstractCubeGenerator.cpp
*/

    void AbstractCubeGenerator::setSolver(Universe::IUniverseSolver *solver) {
        this->solver=solver;
    }

    bool AbstractCubeGenerator::checkConsistency(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>>& cube) {
        auto result = this->solver->solve(cube);
        return result!=Universe::UniverseSolverResult::UNSATISFIABLE;
    }
} // Panoramyx