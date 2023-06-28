/**
* @date 28/06/23
* @file CartesianProductIterativeRefinementCubeGenerator.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_CARTESIANPRODUCTITERATIVEREFINEMENTCUBEGENERATOR_HPP
#define PANORAMYX_CARTESIANPRODUCTITERATIVEREFINEMENTCUBEGENERATOR_HPP

#include "AbstractProblemUnderAssumptionCubeGenerator.hpp"

namespace Panoramyx {

/**
 * @class CartesianProductIterativeRefinementCubeGenerator
 *
 * @brief 
 * @file CartesianProductIterativeRefinementCubeGenerator.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class CartesianProductIterativeRefinementCubeGenerator:public AbstractProblemUnderAssumptionCubeGenerator {
    public:
        CartesianProductIterativeRefinementCubeGenerator(int nbCubesMax);

        ~CartesianProductIterativeRefinementCubeGenerator() override = default;

    protected:
        long computeScore(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) override;
    };

} // Panoramyx

#endif //PANORAMYX_CARTESIANPRODUCTITERATIVEREFINEMENTCUBEGENERATOR_HPP
