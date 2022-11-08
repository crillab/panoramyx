/**
* @date 11/10/22
* @file RemoteSolver.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_REMOTESOLVER_HPP
#define PANORAMYX_REMOTESOLVER_HPP

#include "../../../libs/universe/universe/include/core/IUniverseSolver.hpp"

namespace Panoramyx {

/**
 * @class RemoteSolver
 *
 * @brief 
 * @file RemoteSolver.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    template<typename T>
    class RemoteSolver : public Universe::IUniverseSolver {
    private:
        int rank;
    public:
        explicit RemoteSolver(int rank) : rank(rank) {}

        Universe::UniverseSolverResult solve() override {
            return Universe::UniverseSolverResult::UNKNOWN;
        }

        Universe::UniverseSolverResult solve(std::string filename) override {
            return Universe::UniverseSolverResult::UNKNOWN;
        }

        Universe::UniverseSolverResult solve(std::vector<Universe::UniverseAssumption<T>> assumpts) override {
            return Universe::UniverseSolverResult::UNKNOWN;
        }

        void interrupt() override {

        }

        void setVerbosity(int level) override {

        }

        void setTimeout(long seconds) override {

        }

        void setTimeoutMs(long mseconds) override {

        }

        void reset() override {

        }

        std::vector<Universe::BigInteger> solution() override {
            return std::vector<Universe::BigInteger>();
        }

        int nVariables() override {
            return 0;
        }

        int nConstraints() override {
            return 0;
        }

        void setLogFile(const std::string &filename) override {

        }

        ~RemoteSolver() override = default;

    };

} // Panoramyx

#endif //PANORAMYX_REMOTESOLVER_HPP
