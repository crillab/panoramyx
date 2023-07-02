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

#include <mutex>
#include <optional>
#include <vector>
#include <ostream>
#include <map>

#include <crillab-universe/core/IUniverseSolver.hpp>
#include "../network/INetworkCommunication.hpp"
#include "../network/MessageBuilder.hpp"
#include <loguru/loguru.hpp>
#include <crillab-universe/optim/IOptimizationSolver.hpp>

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

    class RemoteSolver : public Universe::IUniverseSolver, public Universe::IOptimizationSolver {
    private:
        int rank;
        unsigned index;
        INetworkCommunication *comm;
        std::mutex mutex;
        int nbVariables = -1;
        int nbConstraints = -1;
        std::optional<bool> isOptim;
        std::vector<std::string> auxiliaryVariables;
    public:
        explicit RemoteSolver(int rank);

        Universe::UniverseSolverResult solve() override;

        Universe::UniverseSolverResult solve(const std::string &filename) override;

        Universe::UniverseSolverResult
        solve(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) override;

        bool isOptimization() override;

        void interrupt() override;

        void setVerbosity(int level) override;

        void setTimeout(long seconds) override;

        void setTimeoutMs(long mseconds) override;

        void reset() override;

        std::vector<Universe::BigInteger> solution() override;

        int nVariables() override;

        int nConstraints() override;

        void setLogFile(const std::string &filename) override;

        void setIndex(unsigned i);

        void setComm(INetworkCommunication *c);

        ~RemoteSolver() override = default;

        void endSearch();

        void loadInstance(const std::string &filename) override;

        void decisionVariables(const std::vector<std::string> &variables) override;

        void addSearchListener(Universe::IUniverseSearchListener *listener) override;

        void setLogStream(std::ostream &stream) override;

        std::map<std::string, Universe::BigInteger> mapSolution(bool excludeAux) override;

        IOptimizationSolver *toOptimizationSolver() override;

        [[nodiscard]] const std::map<std::string, Universe::IUniverseVariable *> &getVariablesMapping() override;

        std::map<std::string, Universe::BigInteger> mapSolution() override;

        void setLowerBound(const Universe::BigInteger &lb) override;

        void setUpperBound(const Universe::BigInteger &ub) override;

        void setBounds(const Universe::BigInteger &lb, const Universe::BigInteger &ub) override;

        Universe::BigInteger getCurrentBound() override;

        bool isMinimization() override;

        Universe::BigInteger getLowerBound() override;
        Universe::BigInteger getUpperBound() override;

        unsigned int getIndex() const;

        const std::vector<std::string> &getAuxiliaryVariables() override;

        void valueHeuristicStatic(const std::vector<std::string> &variables,
                                  const std::vector<Universe::BigInteger> &orderedValues) override;

        bool checkSolution() override;

        bool checkSolution(const std::map<std::string, Universe::BigInteger> &assignment)  override;

        const std::vector<Universe::IUniverseConstraint *> &getConstraints() override;
    };

}

#endif
