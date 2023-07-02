/**
 * @date 15/11/22
 * @file GauloisSolver.hpp
 * @brief
 * @author Thibault Falque
 * @author Romain Wallon
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_GAULOISSOLVER_HPP
#define PANORAMYX_GAULOISSOLVER_HPP

#include <semaphore>
#include <mutex>
#include <crillab-universe/core/IUniverseSolver.hpp>
#include <crillab-universe/optim/IOptimizationSolver.hpp>
#include "../network/INetworkCommunication.hpp"

namespace Panoramyx {

/**
 * @class GauloisSolver
 *
 * @brief
 * @file GauloisSolver.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

class GauloisSolver : public Universe::IUniverseSolver, public Universe::IOptimizationSolver {
   private:
    Universe::IUniverseSolver *solver;
    INetworkCommunication *comm;
    bool interrupted = false;
    bool finishedB = false;
    std::binary_semaphore finished = std::binary_semaphore(0);
    std::mutex loadMutex;
    std::mutex boundMutex;
    int nbSolved = 0;
    bool optimization;
    std::map<std::string,Universe::BigInteger> currentSolution;
    Universe::BigInteger currentBound;

    unsigned index;

    std::vector<Universe::BigInteger> solution(Message *m);
    std::map<std::string,Universe::BigInteger> mapSolution(Message *m);

    std::vector<Universe::BigInteger> sol;

    void readMessage(Message *m);
    int nVariables(Message *m);
    int nConstraints(Message *m);
    Universe::BigInteger getLowerBound(Message *m);
    Universe::BigInteger getUpperBound(Message *m);
    Universe::BigInteger getCurrentBound(Message *m);
    bool isMinimization(Message *m);

    Universe::UniverseSolverResult solve(Message *m);

    Universe::UniverseSolverResult solve(std::string filename, Message *m);

    Universe::UniverseSolverResult
    solve(std::vector<Universe::UniverseAssumption<Universe::BigInteger>> asumpts, Message *m);

    Universe::IOptimizationSolver *getOptimSolver();

   public:
    explicit GauloisSolver(Universe::IUniverseSolver *solver, INetworkCommunication *comm);

    Universe::UniverseSolverResult solve() override;

    Universe::UniverseSolverResult solve(const std::string &filename) override;

    Universe::UniverseSolverResult
    solve(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &asumpts) override;

    void interrupt() override;

    void setVerbosity(int level) override;

    void setTimeout(long seconds) override;

    void setTimeoutMs(long mseconds) override;

    void reset() override;

    std::vector<Universe::BigInteger> solution() override;

    int nVariables() override;

    int nConstraints() override;

    void setLogFile(const std::string &filename) override;

    void start();

    ~GauloisSolver() override = default;

    void sendResult(int src, Universe::UniverseSolverResult result);

    void loadInstance(const std::string& filename) override;

    const std::map<std::string, Universe::IUniverseVariable *> &getVariablesMapping() override;

    const std::vector<Universe::IUniverseConstraint *> &getConstraints() override;

    std::map<std::string, Universe::BigInteger> mapSolution() override;

    void setLowerBound(const Universe::BigInteger &lb) override;

    void setUpperBound(const Universe::BigInteger &ub) override;

    void setBounds(const Universe::BigInteger &lb, const Universe::BigInteger &ub) override;

    Universe::BigInteger getCurrentBound() override;

    bool isMinimization() override;

    bool isOptimization(Message *m);

    Universe::BigInteger getLowerBound() override;
    Universe::BigInteger getUpperBound() override;

    bool isOptimization() override;

    void decisionVariables(const std::vector<std::string> &variables) override;
    void decisionVariables(Message* message);

    void addSearchListener(Universe::IUniverseSearchListener *listener) override;

    void setLogStream(std::ostream &stream) override;

    std::map<std::string, Universe::BigInteger> mapSolution(bool excludeAux) override;

    Universe::IOptimizationSolver *toOptimizationSolver() override;

    const std::vector<std::string> &getAuxiliaryVariables() override;

    void valueHeuristicStatic(const std::vector<std::string> &variables,
                              const std::vector<Universe::BigInteger> &orderedValues) override;

    void removeSearchListener(Universe::IUniverseSearchListener *listener) override;

    bool checkSolution() override;

    bool checkSolution(const std::map<std::string, Universe::BigInteger> &assignment) override;

    void getAuxiliaryVariables(Message *pMessage);

    void checkSolutionAssignment(Message *pMessage);

    void valueHeuristicStatic(Message *pMessage);

    void checkSolution(Message *pMessage);


};

using GallicSolver = GauloisSolver;

}  // namespace Panoramyx

#endif  // PANORAMYX_GAULOISSOLVER_HPP
