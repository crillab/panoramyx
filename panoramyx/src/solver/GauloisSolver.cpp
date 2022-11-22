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
#include "../../include/network/MessageBuilder.hpp"
#include "../../../libs/autis/autis/include/xcsp/AutisXcspParserAdapter.hpp"
#include "../../../libs/autis/libs/universe/libs/easy-jni/easyjni/JavaVirtualMachineRegistry.h"
#include "loguru.hpp"

namespace Panoramyx {

/**
@class GauloisSolver
@brief Definition of the class GauloisSolver. 
@file GauloisSolver.cpp
*/


    GauloisSolver::GauloisSolver(Universe::IUniverseSolver *solver,INetworkCommunication* comm) : solver(solver),comm(comm) {

    }

    Universe::UniverseSolverResult GauloisSolver::solve() {
        loadMutex.lock();
        nbSolved++;
        auto r=solver->solve();
        loadMutex.unlock();
        return r;
    }

    Universe::UniverseSolverResult GauloisSolver::solve(const std::string &filename) {
        loadMutex.lock();
        nbSolved++;
        ifstream input(filename);
        Autis::Scanner scanner(input);
        auto parser = make_unique<Autis::AutisXCSPParserAdapter>(scanner, dynamic_cast<Universe::IUniverseCspSolver *>(solver));
        parser->parse();
        auto result= solver->solve();
        loadMutex.unlock();
        return result;
    }

    Universe::UniverseSolverResult
    GauloisSolver::solve(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &asumpts) {
        loadMutex.lock();
        nbSolved++;
        auto r= solver->solve(asumpts);
        DLOG_F(INFO,"result after solve(assumpts): %s",r==Universe::UniverseSolverResult::SATISFIABLE?"satisfiable":"unsatisfiable");
        loadMutex.unlock();
        return r;
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
        loadMutex.lock();
        DLOG_F(INFO,"call reset");
        solver->reset();
        loadMutex.unlock();
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
        for(int i=0;i<nbSolved;i++) {
            finished.acquire();
        }
    }

    void GauloisSolver::readMessage(Message *m) {
        DLOG_F(INFO,"Gaulois Solver: readMessage - %s",m->methodName);
        if(strncmp(m->methodName, PANO_MESSAGE_SOLVE_FILENAME, sizeof(m->methodName)) == 0){
            std::string filename(m->parameters);
            this->solve(filename,m);
        }else if(strncmp(m->methodName, PANO_MESSAGE_SOLVE_ASSUMPTIONS, sizeof(m->methodName)) == 0){
            std::vector<Universe::UniverseAssumption<Universe::BigInteger>> assumpts;
            for(int i=0,n=0;n<m->nbParameters;n+=3){
                int varId = m->read<int>(i);
                i+=sizeof(int);
                bool equal = m->read<bool>(i);
                i+= sizeof(bool);
                char* ptr=m->parameters+i;
                std::string param(ptr, strlen(ptr)+1);
                DLOG_F(INFO,"%d %s '%s'",varId,equal?"=":"!=",param.c_str());
                Universe::BigInteger  tmp=Universe::bigIntegerValueOf(param);
                assumpts.emplace_back(varId,equal,tmp);
                i+=param.size();
            }
            this->solve(assumpts,m);
        }else if(strncmp(m->methodName, PANO_MESSAGE_RESET, sizeof(m->methodName)) == 0){
            this->reset();
        }else if(strncmp(m->methodName,PANO_MESSAGE_LOAD, sizeof(m->methodName))==0) {
            std::string filename(m->parameters);
            this->load(filename);
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
            finished.release();
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
            finished.release();
        });
        t.detach();
    }

    Universe::UniverseSolverResult
    GauloisSolver::solve(std::vector<Universe::UniverseAssumption<Universe::BigInteger>> asumpts, Message *m) {
        int src=m->src;
        std::thread t([this,src,asumpts]() {
            DLOG_F(INFO,"Run solve(assumpts,m) in a new thread.");
            auto result = this->solve(asumpts);
            sendResult(src, result);
            easyjni::JavaVirtualMachineRegistry::detachCurrentThread();
            finished.release();
        });
        t.detach();
    }

    void GauloisSolver::load(std::string filename) {
        loadMutex.lock();
        ifstream input(filename);
        Autis::Scanner scanner(input);
        auto parser = make_unique<Autis::AutisXCSPParserAdapter>(scanner, dynamic_cast<Universe::IUniverseCspSolver *>(solver));
        parser->parse();
        loadMutex.unlock();
    }
} // Panoramyx