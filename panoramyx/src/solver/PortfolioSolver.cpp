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

namespace Panoramyx {

/**
@class PortfolioSolver
@brief Definition of the class PortfolioSolver. 
@file PortfolioSolver.cpp
*/

    PortfolioSolver::PortfolioSolver(INetworkCommunication *comm) : AbstractParallelSolver(comm),endSolvers(0) {}

    void PortfolioSolver::addSolver(RemoteSolver *s) {
        AbstractParallelSolver::addSolver(s);
        endSolvers++;
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
        if(strncmp(message->methodName,PANO_MESSAGE_SATISFIABLE,sizeof(message->methodName))==0){
            winner = *((const unsigned *)message->parameters);
            result=Universe::UniverseSolverResult::SATISFIABLE;
            solved.release();
        }else if(strncmp(message->methodName,PANO_MESSAGE_UNSATISFIABLE,sizeof(message->methodName))==0){
            winner = *((const unsigned *)message->parameters);
            result=Universe::UniverseSolverResult::UNSATISFIABLE;
            solved.release();
        }else if(strncmp(message->methodName,PANO_MESSAGE_END_SEARCH,sizeof(message->methodName))==0){
            endSolvers--;
            if(endSolvers<=0){
                interrupted= true;
                end.release();
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


    void PortfolioSolver::endSearch() {
        for (auto &solver : solvers){
            solver->endSearch();
        }
//        MessageBuilder mb;
//        mb.forMethod(PANO_MESSAGE_END_SEARCH);
//        Message *m = mb.withTag(PANO_TAG_SOLVE).build();
//        comm->send(m, 0);
//        free(m);
    }

} // Panoramyx