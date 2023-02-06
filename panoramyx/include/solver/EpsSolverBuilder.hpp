/**
* @file EpsSolverBuilder.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_EPSSOLVERBUILDER_HPP
#define PANORAMYX_EPSSOLVERBUILDER_HPP

#include "AbstractSolverBuilder.hpp"
#include "ICubeGenerator.hpp"

namespace Panoramyx {

/**
 * @class EPSSolverBuilder
 *
 * @brief 
 * @file EpsSolverBuilder.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class EPSSolverBuilder:public AbstractSolverBuilder {
    private:
        ICubeGenerator* cubeGenerator;
    public:
        EPSSolverBuilder * withCubeGenerator(ICubeGenerator* cubeGenerator);

        AbstractParallelSolver *build() override;
    };

} // Panoramyx

#endif //PANORAMYX_EPSSOLVERBUILDER_HPP
