/**
* @date 11/10/22
* @file NullDecompositionStrategy.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include "../../include/decomposition/NullDecompositionStrategy.hpp"

namespace Panoramyx {

/**
@class NullDecompositionStrategy
@brief Definition of the class NullDecompositionStrategy. 
@file NullDecompositionStrategy.cpp
*/

    Stream<Universe::IUniverseProblem *> *NullDecompositionStrategy::decompose(Universe::IUniverseProblem *problem) {
        return new VectorStream<Universe::IUniverseProblem*>({problem});
    }
} // Panoramyx