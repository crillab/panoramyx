/**
* @date 15/11/22
* @file GauloisSolver.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include <mpi.h>
#include <thread>
#include <fstream>
#include "../../include/solver/GauloisSolver.hpp"
#include "../../include/network/INetworkCommunication.hpp"
#include "../../include/network/MessageBuilder.hpp"
#include "../../../libs/autis/autis/xcsp/AutisXcspParserAdapter.hpp"
#include "../../../libs/autis/libs/universe/universe/include/csp/IUniverseCspSolver.hpp"
#include "../../../libs/autis/libs/universe/libs/easy-jni/easyjni/JavaVirtualMachineRegistry.h"

namespace Panoramyx {

/**
@class GauloisSolver
@brief Definition of the class GauloisSolver. 
@file GauloisSolver.cpp
*/


    GauloisSolver::GauloisSolver(Universe::IUniverseSolver *solver,INetworkCommunication* comm) : solver(solver),comm(comm) {

    }

    Universe::UniverseSolverResult GauloisSolver::solve() {
        return solver->solve();
    }

    Universe::UniverseSolverResult GauloisSolver::solve(std::string filename) {
        ifstream input(filename);
        Autis::Scanner scanner(input);
        auto parser = make_unique<Autis::AutisXCSPParserAdapter>(scanner, dynamic_cast<Universe::IUniverseCspSolver *>(solver));
        parser->parse();
        auto result= solver->solve();
        return result;
    }

    Universe::UniverseSolverResult
    GauloisSolver::solve(std::vector<Universe::UniverseAssumption<Universe::BigInteger>> asumpts) {
        return solver->solve(asumpts);
    }

    void GauloisSolver::interrupt() {
        interrupted= true;
        solver->interrupt();
    }

    void GauloisSolver::setVerbosity(int level) {
        solver->setVerbosity(level);
    }

    void GauloisSolver::setTimeout(long seconds) {
        solver->setTimeout(seconds);
    }

    void GauloisSolver::setTimeoutMs(long mseconds) {
        solver->setTimeoutMs(mseconds);
    }

    void GauloisSolver::reset() {
        solver->reset();
    }

    std::vector<Universe::BigInteger> GauloisSolver::solution() {
        return solver->solution();
    }

    int GauloisSolver::nVariables() {
        return solver->nVariables();
    }

    int GauloisSolver::nConstraints() {
        return solver->nConstraints();
    }

    void GauloisSolver::setLogFile(const std::string &filename) {
        solver->setLogFile(filename);
    }

    void GauloisSolver::start() {
        while (!interrupted) {
            auto message = comm->receive(PANO_ANY_TAG, MPI_ANY_SOURCE, PANO_DEFAULT_MESSAGE_SIZE);
            readMessage(message);
            free(message);
        }
        mutex.acquire();
    }

    void GauloisSolver::readMessage(Message *m) {
        if(strncmp(m->methodName, PANO_MESSAGE_SOLVE_FILENAME, sizeof(m->methodName)) == 0){
            std::string filename(m->parameters);
            this->solve(filename,m);
        }else if(strncmp(m->methodName, PANO_MESSAGE_INTERRUPT, sizeof(m->methodName)) == 0){
            this->interrupt();
        }else if(strncmp(m->methodName, PANO_MESSAGE_SET_TIMEOUT, sizeof(m->methodName)) == 0){
            auto timeout = m->read<long>();
            this->setTimeout(timeout);
        }else if(strncmp(m->methodName, PANO_MESSAGE_SET_TIMEOUT_MS, sizeof(m->methodName)) == 0){
            auto timeout = m->read<long>();
            this->setTimeoutMs(timeout);
        }else if(strncmp(m->methodName, PANO_MESSAGE_SET_VERBOSITY, sizeof(m->methodName)) == 0){
            int level = m->read<int>();
            this->setVerbosity(level);
        }else if(strncmp(m->methodName, PANO_MESSAGE_SET_LOG_FILE, sizeof(m->methodName)) == 0){
            std::string filename(m->parameters);
            this->setLogFile(filename);
        }else if(strncmp(m->methodName, PANO_MESSAGE_END_SEARCH, sizeof(m->methodName)) == 0){
            MessageBuilder mb;
            Message* r =mb.forMethod(PANO_MESSAGE_END_SEARCH).withTag(PANO_TAG_SOLVE).build();
            comm->send(r,m->src);
            free(r);
            interrupt();
        }

    }

    std::vector<Universe::BigInteger> GauloisSolver::solution(Message *m) {
        MessageBuilder mb;
        mb.forMethod("solution");
        const std::vector<Universe::BigInteger> &solution1 = solver->solution();
        for (auto &big: solution1) {
            mb.withParameter(Universe::toString(big));
        }
        Message *r = mb.withTag(PANO_TAG_RESPONSE).build();
        comm->send(r, m->src);
        free(r);
        return solution1;
    }

    int GauloisSolver::nVariables(Message *m) {
        int n=solver->nVariables();
        MessageBuilder mb;
        Message* r=mb.forMethod(PANO_MESSAGE_N_VARIABLES).withTag(PANO_TAG_RESPONSE).withParameter(n).build();
        comm->send(r,m->src);
        free(r);
        return n;
    }

    int GauloisSolver::nConstraints(Message *m) {
        int n=solver->nConstraints();
        MessageBuilder mb;
        Message* r=mb.forMethod(PANO_MESSAGE_N_CONSTRAINTS).withTag(PANO_TAG_RESPONSE).withParameter(n).build();
        comm->send(r,m->src);
        free(r);
        return n;
    }

    Universe::UniverseSolverResult GauloisSolver::solve(Message *m) {
        int src=m->src;
        std::thread t([this,src]() {
            auto result = this->solve();
            sendResult(src, result);
        });
        t.detach();
    }

    void GauloisSolver::sendResult(int src, Universe::UniverseSolverResult result) {
        MessageBuilder mb;
        switch (result) {

            case Universe::UniverseSolverResult::SATISFIABLE:
                mb.forMethod(PANO_MESSAGE_SATISFIABLE);
                break;
            case Universe::UniverseSolverResult::UNSATISFIABLE:
                mb.forMethod(PANO_MESSAGE_UNSATISFIABLE);
                break;
            case Universe::UniverseSolverResult::UNKNOWN:
                mb.forMethod(PANO_MESSAGE_UNKNOWN);
                break;
            case Universe::UniverseSolverResult::UNSUPPORTED:
                mb.forMethod(PANO_MESSAGE_UNSUPPORTED);
                break;
            case Universe::UniverseSolverResult::OPTIMUM_FOUND:
                mb.forMethod(PANO_MESSAGE_OPTIMUM_FOUND);
                break;
        }
        Message* r = mb.withTag(PANO_TAG_SOLVE).build();
        comm->send(r, src);
        free(r);
    }

    Universe::UniverseSolverResult GauloisSolver::solve(std::string filename, Message *m) {
        int src=m->src;
        std::thread t([this,src,filename]() {
            auto result = this->solve(filename);
            sendResult(src, result);
            mutex.release();
        });
        t.detach();
    }

    Universe::UniverseSolverResult
    GauloisSolver::solve(std::vector<Universe::UniverseAssumption<Universe::BigInteger>> asumpts, Message *m) {
        int src=m->src;
        std::thread t([this,src,asumpts]() {
            auto result = this->solve(asumpts);
            sendResult(src, result);
        });
        t.detach();
    }
} // Panoramyx