/**
* @date 06/02/23
* @file EpsSolverBuilder.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include "crillab-panoramyx/solver/EpsSolverBuilder.hpp"
#include "crillab-panoramyx/solver/EPSSolver.hpp"

namespace Panoramyx {

/**
@class EPSSolverBuilder
@brief Definition of the class EPSSolverBuilder. 
@file EpsSolverBuilder.cpp
*/




    EPSSolverBuilder * EPSSolverBuilder::withCubeGenerator(ICubeGenerator *cubeGenerator) {
        this->cubeGenerator=cubeGenerator;
        return this;
    }

    AbstractParallelSolver *EPSSolverBuilder::build() {
        return new EPSSolver(this->networkCommunication,this->cubeGenerator);
    }
} // Panoramyx