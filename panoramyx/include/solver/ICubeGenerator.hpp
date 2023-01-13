/**
* @date 21/11/22
* @file ICubeGenerator.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_ICUBEGENERATOR_HPP
#define PANORAMYX_ICUBEGENERATOR_HPP

#include "../../../libs/autis/libs/universe/universe/include/core/IUniverseSolver.hpp"
#include "../utils/Stream.hpp"
#include "../core/IConsistencyChecker.hpp"

namespace Panoramyx {

/**
 * @class ICubeGenerator
 *
 * @brief 
 * @file ICubeGenerator.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class ICubeGenerator {
    public:
        virtual void setConsistencyChecker(IConsistencyChecker* solver)=0;
        virtual void setSolver(Universe::IUniverseSolver* solver)=0;
        virtual Stream<std::vector<Universe::UniverseAssumption<Universe::BigInteger>>>* generateCubes()=0;
        virtual ~ICubeGenerator()=default;
    };

} // Panoramyx

#endif //PANORAMYX_ICUBEGENERATOR_HPP
