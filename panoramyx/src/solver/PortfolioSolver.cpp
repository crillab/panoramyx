/**
* @date 14/11/22
* @file PortfolioSolver.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include <cstring>
#include "../../include/solver/PortfolioSolver.hpp"
#include "../../include/network/Message.hpp"

namespace Panoramyx {

/**
@class PortfolioSolver
@brief Definition of the class PortfolioSolver. 
@file PortfolioSolver.cpp
*/

    PortfolioSolver::PortfolioSolver(INetworkCommunication *comm) : AbstractParallelSolver(comm) {}

    void PortfolioSolver::addSolver(RemoteSolver *s) {
        AbstractParallelSolver::addSolver(s);

    }

    void PortfolioSolver::solve(unsigned int i) {
        solvers[i]->solve();
    }

    void
    PortfolioSolver::solve(unsigned int i, const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) {
            solvers[i]->solve(assumpts);
    }

    void PortfolioSolver::solve(unsigned int i, const std::string &filename) {
        solvers[i]->solve(filename);
    }

    void PortfolioSolver::readMessage(const Message *message) {
        if(strncmp(message->name, PANO_MESSAGE_SATISFIABLE, sizeof(message->name)) == 0){
            winner = *((const unsigned *)message->parameters);
            result=Universe::UniverseSolverResult::SATISFIABLE;
            solved.release();
        }else if(strncmp(message->name, PANO_MESSAGE_UNSATISFIABLE, sizeof(message->name)) == 0){
            winner = *((const unsigned *)message->parameters);
            result=Universe::UniverseSolverResult::UNSATISFIABLE;
            solved.release();
        }else if(strncmp(message->name, PANO_MESSAGE_END_SEARCH, sizeof(message->name)) == 0){
            endSolvers--;
            if(endSolvers<=0){
                interrupted= true;
                end.release();
            }
        }else if(strncmp(message->name, PANO_MESSAGE_NEW_BOUND_FOUND, sizeof(message->name)) == 0){
            std::string param(message->parameters, strlen(message->parameters)+1);
            Universe::BigInteger  newBound=Universe::bigIntegerValueOf(param);
            if(isMinimization && newBound < upperBound){
                upperBound=newBound;
                for(auto solver:solvers){
                    solver->setUpperBound(upperBound);
                }
            }else if(!isMinimization && lowerBound<newBound){
                lowerBound=newBound;
                for(auto solver:solvers){
                    solver->setLowerBound(lowerBound);
                }
            }

        }
    }

    std::vector<Universe::BigInteger> PortfolioSolver::solution() {
        return solvers[winner]->solution();
    }

    int PortfolioSolver::nVariables() {
        return solvers[0]->nVariables();
    }

    int PortfolioSolver::nConstraints() {
        return solvers[0]->nConstraints();
    }

    const std::map<std::string, Universe::IUniverseVariable *> &PortfolioSolver::getVariablesMapping()  {
        return {};
    }

    std::map<std::string, Universe::BigInteger> PortfolioSolver::mapSolution() {
        return std::map<std::string, Universe::BigInteger>();
    }

} // Panoramyx