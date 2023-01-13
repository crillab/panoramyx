/**
* @date 05/12/22
* @file LexicographicCubeGenerator.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include "../../include/decomposition/LexicographicCubeGenerator.hpp"
#include "../../include/decomposition/StreamLexicographicCube.hpp"

using namespace Panoramyx;
using namespace Universe;
using namespace std;

/**
@class LexicographicCubeGenerator
@brief Definition of the class LexicographicCubeGenerator. 
@file LexicographicCubeGenerator.cpp
*/

LexicographicCubeGenerator::LexicographicCubeGenerator(int nbCubesMax) : nbCubesMax(nbCubesMax) {

}
    Stream<vector<UniverseAssumption<BigInteger>>>* LexicographicCubeGenerator::generateCubes() {
        return new StreamLexicographicCube(solver->getVariablesMapping(), nbCubesMax, consistencyChecker);
    }

