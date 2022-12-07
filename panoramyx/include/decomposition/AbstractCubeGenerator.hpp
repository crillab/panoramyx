/**
* @date 05/12/22
* @file AbstractCubeGenerator.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_ABSTRACTCUBEGENERATOR_HPP
#define PANORAMYX_ABSTRACTCUBEGENERATOR_HPP

#include "../solver/ICubeGenerator.hpp"

namespace Panoramyx {

/**
 * @class AbstractCubeGenerator
 *
 * @brief 
 * @file AbstractCubeGenerator.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class AbstractCubeGenerator: public ICubeGenerator {
    protected:
        Universe::IUniverseSolver* solver;
        bool checkConsistency(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>>& cube);
    public:
        void setSolver(Universe::IUniverseSolver *solver) override;
        ~AbstractCubeGenerator() override = default;
    };

} // Panoramyx

#endif //PANORAMYX_ABSTRACTCUBEGENERATOR_HPP
