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
#include "../../../libs/loguru/loguru.hpp"
#include "../../../libs/autis/libs/universe/universe/include/optim/IOptimizationSolver.hpp"

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
            Message *m = mb.forMethod(PANO_MESSAGE_SOLVE_FILENAME).withParameter(filename).withTag(
                    PANO_TAG_SOLVE).build();
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

                DLOG_F(INFO, "add assumption: %d %s '%s'", assumpt.getVariableId(), assumpt.isEqual() ? "=" : "!=",
                       Universe::toString(assumpt.getValue()).c_str());
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
            char *ptr = m->parameters;
            for (int i = 0, n = 0; n < m->nbParameters; i++) {
                if (ptr[i] == '\0') {
                    std::string param(ptr, i);
                    bigbig.push_back(Universe::bigIntegerValueOf(param));
                    ptr += i + 1;
                    i = -1;
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

        void loadFilename(const std::string &filename) {
            MessageBuilder mb;
            Message *m = mb.forMethod(PANO_MESSAGE_LOAD).withParameter(filename).withTag(PANO_TAG_SOLVE).build();
            comm->send(m, rank);
            free(m);
        }

        [[nodiscard]] const std::map<std::string, Universe::IUniverseVariable *> &getVariablesMapping() override {
            return {};
        }

        std::map<std::string, Universe::BigInteger> mapSolution() override {
            return {};
        }

        void setLowerBound(const Universe::BigInteger &lb) override {
            MessageBuilder mb;
            Message *m = mb.forMethod(PANO_MESSAGE_LOWER_BOUND).withParameter(lb).withTag(PANO_TAG_SOLVE).build();
            comm->send(m, rank);
            free(m);
        }

        void setUpperBound(const Universe::BigInteger &ub) override {
            MessageBuilder mb;
            Message *m = mb.forMethod(PANO_MESSAGE_UPPER_BOUND).withParameter(ub).withTag(PANO_TAG_SOLVE).build();
            comm->send(m, rank);
            free(m);
        }

        void setBounds(const Universe::BigInteger &lb, const Universe::BigInteger &ub) override {
            MessageBuilder mb;
            Message *m = mb.forMethod(PANO_MESSAGE_LOWER_UPPER_BOUND).withParameter(lb).withParameter(ub).withTag(
                    PANO_TAG_SOLVE).build();
            comm->send(m, rank);
            free(m);
        }

        Universe::BigInteger getCurrentBound() override {
            mutex.lock();
            MessageBuilder mb;
            Message *m = mb.forMethod(PANO_MESSAGE_GET_CURRENT_BOUND).withTag(
                    PANO_TAG_RESPONSE).build();
            comm->send(m, rank);
            free(m);

            m = comm->receive(PANO_TAG_RESPONSE, rank, 1024);
            std::string param(m->parameters, strlen(m->parameters)+1);
            Universe::BigInteger  newBound=Universe::bigIntegerValueOf(param);
            mutex.unlock();
            free(m);
            return newBound;
        }

        bool isMinimization() override {
            mutex.lock();
            MessageBuilder mb;
            Message *m = mb.forMethod(PANO_MESSAGE_IS_MINIMIZATION).withTag(
                    PANO_TAG_RESPONSE).build();
            comm->send(m, rank);
            free(m);

            m = comm->receive(PANO_TAG_RESPONSE, rank, 1024);
            bool r = m->read<bool>();
            mutex.unlock();
            free(m);
            return r;
        }
    };

} // Panoramyx

#endif //PANORAMYX_REMOTESOLVER_HPP
