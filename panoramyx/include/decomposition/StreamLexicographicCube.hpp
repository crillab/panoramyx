/**
* @date 07/12/22
* @file StreamLexicographicCube.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_STREAMLEXICOGRAPHICCUBE_HPP
#define PANORAMYX_STREAMLEXICOGRAPHICCUBE_HPP

#include <map>
#include "../utils/Stream.hpp"

#include "../../../libs/autis/libs/universe/universe/include/core/UniverseType.hpp"
#include "../../../libs/autis/libs/universe/universe/include/core/UniverseAssumption.hpp"
#include "../../../libs/autis/libs/universe/universe/include/core/problem/IUniverseVariable.hpp"
#include "../core/IConsistencyChecker.hpp"
#include "../../../libs/autis/libs/universe/universe/include/csp/IUniverseCspSolver.hpp"

namespace Panoramyx {

/**
 * @class StreamLexicographicCube
 *
 * @brief 
 * @file StreamLexicographicCube.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class StreamLexicographicCube : public Stream<std::vector<Universe::UniverseAssumption<Universe::BigInteger>>> {
    private:
        const std::map<std::string, Universe::IUniverseVariable *> &mapping;
        std::vector<Universe::UniverseAssumption<Universe::BigInteger>> current;
        std::vector<bool> variablesFinish;
        std::vector<int> indexesCurrentValues;
        std::vector<Universe::IUniverseVariable *> variables;
        size_t nbCubeMax;
        IConsistencyChecker *checker;


        void generateFirst();

        void generateNext();

    public:

        explicit StreamLexicographicCube(const std::map<std::string, Universe::IUniverseVariable *> &mapping,
                                         size_t nbCubeMax, IConsistencyChecker *checker);

        std::vector<Universe::UniverseAssumption<Universe::BigInteger>> next() override;

        [[nodiscard]] bool hasNext() const override;

        ~StreamLexicographicCube() override = default;
    };

} // Panoramyx

#endif //PANORAMYX_STREAMLEXICOGRAPHICCUBE_HPP
