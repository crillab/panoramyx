/**
* @date 14/11/22
* @file PortfolioSolver.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_PORTFOLIOSOLVER_HPP
#define PANORAMYX_PORTFOLIOSOLVER_HPP

#include "AbstractParallelSolver.hpp"

namespace Panoramyx {

/**
 * @class PortfolioSolver
 *
 * @brief 
 * @file PortfolioSolver.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class PortfolioSolver:public AbstractParallelSolver {
    public:
        explicit PortfolioSolver(INetworkCommunication *comm);

        void solve(unsigned int i) override;

        void solve(unsigned int i, const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) override;

        void solve(unsigned int i, const std::string &filename) override;

        std::vector<Universe::BigInteger> solution() override;

        int nVariables() override;

        int nConstraints() override;

        void addSolver(RemoteSolver *s) override;

        ~PortfolioSolver() override = default;

    protected:
        void readMessage(const Message *message) override;
    };

} // Panoramyx

#endif //PANORAMYX_PORTFOLIOSOLVER_HPP
