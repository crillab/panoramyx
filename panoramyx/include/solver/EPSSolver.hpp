/**
* @date 21/11/22
* @file EpsSolver.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_EPSSOLVER_HPP
#define PANORAMYX_EPSSOLVER_HPP

#include "AbstractParallelSolver.hpp"
#include "ICubeGenerator.hpp"

namespace Panoramyx {

/**
 * @class EPSSolver
 *
 * @brief 
 * @file EpsSolver.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class EPSSolver:public AbstractParallelSolver {
    private:
        ICubeGenerator* generator;
        BlockingDeque<Universe::IUniverseSolver*> availableSolvers;
        std::counting_semaphore<42> cubes;
    public:
        EPSSolver(INetworkCommunication *comm, ICubeGenerator *generator);

        void solve(unsigned int i) override;

        void
        solve(unsigned int i, const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) override;

        void solve(unsigned int i, const std::string &filename) override;

        void startSearch() override;

        ~EPSSolver() override = default;

        int nVariables() override;

        int nConstraints() override;

        std::vector<Universe::BigInteger> solution() override;

    protected:
        void readMessage(const Message *message) override;

    };

} // Panoramyx

#endif //PANORAMYX_EPSSOLVER_HPP
