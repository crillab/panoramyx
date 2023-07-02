/**
* @date 28/06/23
* @file CartesianProductIterativeRefinementCubeGenerator.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include "crillab-panoramyx/decomposition/CartesianProductIterativeRefinementCubeGenerator.hpp"
#include "loguru.hpp"

using namespace Universe;
using namespace std;

namespace Panoramyx {

/**
@class CartesianProductIterativeRefinementCubeGenerator
@brief Definition of the class CartesianProductIterativeRefinementCubeGenerator. 
@file CartesianProductIterativeRefinementCubeGenerator.cpp
*/


    CartesianProductIterativeRefinementCubeGenerator::CartesianProductIterativeRefinementCubeGenerator(int nbCubesMax): AbstractProblemUnderAssumptionCubeGenerator(nbCubesMax) {}

    long CartesianProductIterativeRefinementCubeGenerator::computeScore(
            const vector<UniverseAssumption<BigInteger>> &assumpts) {
        solver->reset();
        auto result = solver->solve(assumpts);
        DLOG_F(INFO,"result compute score %d",result);
        if(result==Universe::UniverseSolverResult::UNSATISFIABLE){
            return -1;
        }
        unsigned long totalSize =1;
        for(auto& a:solver->getVariablesMapping()){
            totalSize*=a.second->getDomain()->currentSize();
        }
        return totalSize;
    }

} // Panoramyx