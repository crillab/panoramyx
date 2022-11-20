/**
* @date 14/11/22
* @file AbstractParallelSolver.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_ABSTRACTPARALLELSOLVER_HPP
#define PANORAMYX_ABSTRACTPARALLELSOLVER_HPP

#include <vector>
#include "../../../libs/autis/libs/universe/universe/include/core/IUniverseSolver.hpp"
#include "../network/NetworkTypes.hpp"
#include "../utils/BlockingDeque.hpp"
#include "RemoteSolver.hpp"

namespace Panoramyx {

/**
 * @class AbstractParallelSolver
 *
 * @brief 
 * @file AbstractParallelSolver.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class AbstractParallelSolver : public Universe::IUniverseSolver {
    protected:
        std::vector<RemoteSolver*> solvers;
        INetworkCommunication* comm;
        bool interrupted = false;
        std::binary_semaphore solved = std::binary_semaphore(0);
        std::binary_semaphore end = std::binary_semaphore(0);

        Universe::UniverseSolverResult result=Universe::UniverseSolverResult::UNKNOWN;

        virtual void readMessages();
        virtual void readMessage(const Message* message)=0;
        virtual void endSearch()=0;
    public:
        explicit AbstractParallelSolver(INetworkCommunication *comm);

        Universe::UniverseSolverResult solve() override;

        Universe::UniverseSolverResult solve(const std::string &filename) override;

        Universe::UniverseSolverResult solve(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) override;

        virtual void solve(unsigned i) = 0;

        virtual void solve(unsigned i, const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) = 0;

        virtual void solve(unsigned i, const std::string &filename) = 0;

        void interrupt() override;

        void setVerbosity(int level) override;

        void setTimeout(long seconds) override;

        void setTimeoutMs(long mseconds) override;

        void reset() override;

        void setLogFile(const std::string &filename) override;

        virtual void addSolver(RemoteSolver* s);
    };
    using Abraracourcix=AbstractParallelSolver;
} // Panoramyx

#endif //PANORAMYX_ABSTRACTPARALLELSOLVER_HPP
