/**
* @date 28/06/23
* @file AbstractProblemUnderAssumptionCubeGenerator.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_ABSTRACTPROBLEMUNDERASSUMPTIONCUBEGENERATOR_HPP
#define PANORAMYX_ABSTRACTPROBLEMUNDERASSUMPTIONCUBEGENERATOR_HPP

#include <queue>
#include "AbstractCubeGenerator.hpp"
#include "ProblemUnderAssumption.hpp"

namespace Panoramyx {

/**
 * @class AbstractProblemUnderAssumptionCubeGenerator
 *
 * @brief 
 * @file AbstractProblemUnderAssumptionCubeGenerator.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class AbstractProblemUnderAssumptionCubeGenerator: public AbstractCubeGenerator {
    public:
        explicit AbstractProblemUnderAssumptionCubeGenerator(int nbCubesMax);

        Stream<std::vector<Universe::UniverseAssumption<Universe::BigInteger>>> *generateCubes() override;

    protected:
        virtual long computeScore(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>>& assumpts)=0;
    private:
        std::priority_queue<ProblemUnderAssumption> priorityQueue;
    };

} // Panoramyx

#endif //PANORAMYX_ABSTRACTPROBLEMUNDERASSUMPTIONCUBEGENERATOR_HPP
