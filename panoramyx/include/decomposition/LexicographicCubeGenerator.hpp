/**
* @date 05/12/22
* @file LexicographicCubeGenerator.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_LEXICOGRAPHICCUBEGENERATOR_HPP
#define PANORAMYX_LEXICOGRAPHICCUBEGENERATOR_HPP

#include "AbstractCubeGenerator.hpp"

namespace Panoramyx {

/**
 * @class LexicographicCubeGenerator
 *
 * @brief 
 * @file LexicographicCubeGenerator.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class LexicographicCubeGenerator: public AbstractCubeGenerator {
    private:
        int nbCubesMax;

    public:
        explicit LexicographicCubeGenerator(int nbCubesMax);
        Stream<std::vector<Universe::UniverseAssumption<Universe::BigInteger>>> *generateCubes() override;
    };

} // Panoramyx

#endif //PANORAMYX_LEXICOGRAPHICCUBEGENERATOR_HPP
