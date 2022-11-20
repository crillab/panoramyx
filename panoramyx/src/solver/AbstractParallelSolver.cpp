/**
* @date 14/11/22
* @file AbstractParallelSolver.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include <thread>
#include <mpi.h>
#include "../../include/solver/AbstractParallelSolver.hpp"

namespace Panoramyx {

/**
@class AbstractParallelSolver
@brief Definition of the class AbstractParallelSolver.
@file AbstractParallelSolver.cpp
*/

    AbstractParallelSolver::AbstractParallelSolver(INetworkCommunication *comm) : comm(comm) {}
    Universe::UniverseSolverResult AbstractParallelSolver::solve() {
        readMessages();
        for (unsigned i = 0; i < solvers.size(); i++) {
            solvers[i]->setIndex(i);
            solvers[i]->setComm(comm);
            solve(i);
        }
        solved.acquire();
        endSearch();
        end.acquire();
        return result;
    }

    Universe::UniverseSolverResult AbstractParallelSolver::solve(const std::string &filename) {
        readMessages();
        for (unsigned i = 0; i < solvers.size(); i++) {
            solvers[i]->setIndex(i);
            solvers[i]->setComm(comm);
            solve(i, filename);
        }
        solved.acquire();
        endSearch();
        end.acquire();
        return result;
    }

    Universe::UniverseSolverResult
    AbstractParallelSolver::solve(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) {
        readMessages();
        for (unsigned i = 0; i < solvers.size(); i++) {
            solvers[i]->setIndex(i);
            solvers[i]->setComm(comm);
            solve(i, assumpts);
        }
        solved.acquire();
        endSearch();
        end.acquire();
        return result;
    }

    void AbstractParallelSolver::interrupt() {
        for (auto &s: solvers) {
            s->interrupt();
        }
        interrupted = true;
    }

    void AbstractParallelSolver::setVerbosity(int level) {
        for (auto &s: solvers) {
            s->setVerbosity(level);
        }
    }

    void AbstractParallelSolver::setTimeout(long seconds) {
        for (auto &s: solvers) {
            s->setTimeout(seconds);
        }
    }

    void AbstractParallelSolver::setTimeoutMs(long mseconds) {
        for (auto &s: solvers) {
            s->setTimeoutMs(mseconds);
        }
    }

    void AbstractParallelSolver::reset() {
        for (auto &s: solvers) {
            s->reset();
        }
    }

    void AbstractParallelSolver::setLogFile(const std::string &filename) {
        //TODO
    }

    void AbstractParallelSolver::readMessages() {
        std::thread rm([this]() {
            while (!interrupted) {
                auto message = comm->receive(PANO_TAG_SOLVE,MPI_ANY_SOURCE,PANO_DEFAULT_MESSAGE_SIZE);
                readMessage(message);
                free(message);
            }
        });
        rm.detach();

    }

    void AbstractParallelSolver::addSolver(RemoteSolver *s) {
        solvers.push_back(s);
    }


} // Panoramyx