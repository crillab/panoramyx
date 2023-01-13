/**
* @date 05/12/22
* @file AbstractCubeGenerator.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include "../../include/decomposition/AbstractCubeGenerator.hpp"
#include "../../../libs/autis/libs/universe/universe/include/csp/IUniverseCspSolver.hpp"

namespace Panoramyx {

/**
@class AbstractCubeGenerator
@brief Definition of the class AbstractCubeGenerator. 
@file AbstractCubeGenerator.cpp
*/

    void AbstractCubeGenerator::setConsistencyChecker(IConsistencyChecker *cc) {
        this->consistencyChecker=cc;
    }

    bool AbstractCubeGenerator::checkConsistency(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>>& cube) {
        return this->consistencyChecker->checkFinal(cube);
    }

    void AbstractCubeGenerator::setSolver(Universe::IUniverseSolver *s) {
        this->solver=s;
    }
} // Panoramyx