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

#include "../../../libs/autis/libs/universe/universe/include/core/IUniverseSolver.hpp"
#include "../network/INetworkCommunication.hpp"
#include "../network/MessageBuilder.hpp"

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

    class RemoteSolver : public Universe::IUniverseSolver {
    private:
        int rank;
        unsigned index;
        INetworkCommunication *comm;
        std::mutex mutex;
        int nbVariables = -1;
        int nbConstraints = -1;
    public:
        explicit RemoteSolver(int rank) : rank(rank) {}

        Universe::UniverseSolverResult solve() override {
            MessageBuilder mb;
            Message *m = mb.forMethod(PANO_MESSAGE_SOLVE).withTag(PANO_TAG_SOLVE).build();
            comm->send(m, rank);
            free(m);
            return Universe::UniverseSolverResult::UNKNOWN;
        }

        Universe::UniverseSolverResult solve(const std::string &filename) override {
            MessageBuilder mb;
            Message *m = mb.forMethod(PANO_MESSAGE_SOLVE_FILENAME).withParameter(filename).withTag(PANO_TAG_SOLVE).build();
            comm->send(m, rank);
            free(m);
            return Universe::UniverseSolverResult::UNKNOWN;
        }

        Universe::UniverseSolverResult
        solve(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) override {
            MessageBuilder mb;
            mb.forMethod(PANO_MESSAGE_SOLVE_ASSUMPTIONS);
            for (auto &assumpt: assumpts) {
                mb.withParameter(assumpt.getVariableId());
                mb.withParameter(assumpt.isEqual());
                mb.withParameter(Universe::toString(assumpt.getValue()));
            }
            Message *m = mb.withTag(PANO_TAG_SOLVE).build();
            comm->send(m, rank);
            free(m);
            return Universe::UniverseSolverResult::UNKNOWN;
        }

        void interrupt() override {
            MessageBuilder mb;
            mb.forMethod(PANO_MESSAGE_INTERRUPT);
            Message *m = mb.withTag(PANO_TAG_SOLVE).build();
            comm->send(m, rank);
            free(m);
        }

        void setVerbosity(int level) override {
            MessageBuilder mb;
            mb.forMethod(PANO_MESSAGE_SET_VERBOSITY);
            mb.withParameter(level);
            Message *m = mb.withTag(PANO_TAG_CONFIG).build();
            comm->send(m, rank);
            free(m);
        }

        void setTimeout(long seconds) override {
            MessageBuilder mb;
            mb.forMethod(PANO_MESSAGE_SET_TIMEOUT);
            mb.withParameter(seconds);
            Message *m = mb.withTag(PANO_TAG_CONFIG).build();
            comm->send(m, rank);
            free(m);
        }

        void setTimeoutMs(long mseconds) override {
            MessageBuilder mb;
            mb.forMethod(PANO_MESSAGE_SET_TIMEOUT_MS);
            mb.withParameter(mseconds);
            Message *m = mb.withTag(PANO_TAG_CONFIG).build();
            comm->send(m, rank);
            free(m);
        }

        void reset() override {
            MessageBuilder mb;
            mb.forMethod(PANO_MESSAGE_RESET);
            Message *m = mb.withTag(PANO_TAG_SOLVE).build();
            comm->send(m, rank);
            free(m);
        }

        std::vector<Universe::BigInteger> solution() override {
            unsigned long size = nVariables() * sizeof(long long) + sizeof(Message) + PANO_DEFAULT_RAB;
            mutex.lock();
            MessageBuilder mb;
            mb.forMethod(PANO_MESSAGE_SOLUTION);
            Message *m = mb.withTag(PANO_TAG_RESPONSE).build();
            comm->send(m, rank);
            free(m);
            m = comm->receive(PANO_TAG_RESPONSE, rank, size);
            mutex.unlock();
            std::vector<Universe::BigInteger> bigbig;
            char* ptr=m->parameters;
            for(int i=0,n=0;n<m->nbParameters;i++){
                if(ptr[i]=='\0'){
                    std::string param(ptr,i);
                    bigbig.push_back(Universe::bigIntegerValueOf(param));
                    ptr+=i+1;
                    i=-1;
                    n++;
                }

            }
            return bigbig;
        }

        int nVariables() override {
            if (nbVariables < 0) {
                mutex.lock();
                MessageBuilder mb;
                mb.forMethod(PANO_MESSAGE_N_VARIABLES);
                Message *m = mb.withTag(PANO_TAG_RESPONSE).build();
                comm->send(m, rank);
                free(m);

                m = comm->receive(PANO_TAG_RESPONSE, rank, PANO_DEFAULT_MESSAGE_SIZE);
                mutex.unlock();
                nbVariables = *((const int *) m->parameters);
            }
            return nbVariables;
        }

        int nConstraints() override {
            if (nbConstraints < 0) {
                mutex.lock();
                MessageBuilder mb;
                mb.forMethod(PANO_MESSAGE_N_CONSTRAINTS);
                Message *m = mb.withTag(PANO_TAG_RESPONSE).build();
                comm->send(m, rank);
                free(m);

                m = comm->receive(PANO_TAG_RESPONSE, rank, 1024);
                mutex.unlock();
                nbConstraints = *((const int *) m->parameters);
            }
            return nbConstraints;
        }

        void setLogFile(const std::string &filename) override {

        }

        void setIndex(unsigned i) {
            this->index = i;
        }

        void setComm(INetworkCommunication *c) {
            comm = c;
        }

        ~RemoteSolver() override = default;

        void endSearch() {
            MessageBuilder mb;
            mb.forMethod(PANO_MESSAGE_END_SEARCH);
            Message *m = mb.withTag(PANO_TAG_SOLVE).build();
            comm->send(m, rank);
            free(m);
        }
    };

} // Panoramyx

#endif //PANORAMYX_REMOTESOLVER_HPP
