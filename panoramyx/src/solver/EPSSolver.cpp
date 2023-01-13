/**
* @date 21/11/22
* @file EpsSolver.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include <thread>
#include "../../include/solver/EPSSolver.hpp"
#include "../../../libs/loguru/loguru.hpp"
namespace Panoramyx {

/**
@class EPSSolver
@brief Definition of the class EPSSolver. 
@file EpsSolver.cpp
*/

    EPSSolver::EPSSolver(INetworkCommunication *comm, ICubeGenerator *generator) : AbstractParallelSolver(comm),
                                                                                   generator(generator), cubes(0) {}


    void EPSSolver::solve(unsigned int i) {
        availableSolvers.add(solvers[i]);
    }

    void
    EPSSolver::solve(unsigned int i, const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) {
        availableSolvers.add(solvers[i]);
    }

    void EPSSolver::solve(unsigned int i, const std::string &filename) {
        availableSolvers.add(solvers[i]);
    }

    void EPSSolver::readMessage(const Message *message) {
        if(strncmp(message->methodName,PANO_MESSAGE_SATISFIABLE,sizeof(message->methodName))==0){
            winner = message->read<unsigned>();;
            result=Universe::UniverseSolverResult::SATISFIABLE;
            availableSolvers.clear();
            solved.release();
            cubes.release();
        }else if(strncmp(message->methodName,PANO_MESSAGE_UNSATISFIABLE,sizeof(message->methodName))==0){
            auto src = message->read<unsigned>();
            solvers[src]->reset();
            availableSolvers.add(solvers[src]);
            cubes.release();
        }else if(strncmp(message->methodName,PANO_MESSAGE_END_SEARCH,sizeof(message->methodName))==0){
            endSolvers--;
            if(endSolvers<=0){
                interrupted= true;
                end.release();
            }
        }
    }

    void EPSSolver::startSearch() {
        std::thread t([this](){
            int nbCubes = 0;
            for(auto c:*this->generator->generateCubes()){
                if(c.empty()){
                    result=Universe::UniverseSolverResult::UNSATISFIABLE;
                    solved.release();
                    return;
                }
                try {
                    nbCubes++;
                    DLOG_F(INFO,"generate cubes %ld",nbCubes);
                    auto s = availableSolvers.get();
                    s->solve(c);
                }catch (Except::NoSuchElementException& e){
                    return;
                }
            }
            for(int i=0;i<nbCubes;i++){
                cubes.acquire();
                if(result==Universe::UniverseSolverResult::SATISFIABLE){
                    return;
                }
            }
            result=Universe::UniverseSolverResult::UNSATISFIABLE;
            solved.release();
        });
        t.detach();
    }

    int EPSSolver::nVariables() {
        return 0;
    }

    int EPSSolver::nConstraints() {
        return 0;
    }

    std::vector<Universe::BigInteger> EPSSolver::solution() {
        return std::vector<Universe::BigInteger>();
    }

    const std::map<std::string, Universe::IUniverseVariable *> &EPSSolver::getVariablesMapping()  {
        return {};
    }

    std::map<std::string, Universe::BigInteger> EPSSolver::mapSolution() {
        return std::map<std::string, Universe::BigInteger>();
    }
} // Panoramyx