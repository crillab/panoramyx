/**
* @date 11/10/22
* @file RemoteSolver.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include "../../include/solver/RemoteSolver.hpp"
#include "../../include/network/Message.hpp"

using namespace Panoramyx;

RemoteSolver::RemoteSolver(int rank) : rank(rank) {}

Universe::UniverseSolverResult RemoteSolver::solve() {
    MessageBuilder mb;
    Message *m = mb.named(PANO_MESSAGE_SOLVE).withTag(PANO_TAG_SOLVE).build();
    comm->send(m, rank);
    free(m);
    return Universe::UniverseSolverResult::UNKNOWN;
}

Universe::UniverseSolverResult RemoteSolver::solve(const std::string &filename) {
    MessageBuilder mb;
    Message *m = mb.named(PANO_MESSAGE_SOLVE_FILENAME).withParameter(filename).withTag(
            PANO_TAG_SOLVE).build();
    comm->send(m, rank);
    free(m);
    return Universe::UniverseSolverResult::UNKNOWN;
}

Universe::UniverseSolverResult
RemoteSolver::solve(const std::vector<Universe::UniverseAssumption<Universe::BigInteger>> &assumpts) {
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_SOLVE_ASSUMPTIONS);
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

void RemoteSolver::interrupt() {
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_INTERRUPT);
    Message *m = mb.withTag(PANO_TAG_SOLVE).build();
    comm->send(m, rank);
    free(m);
}

void RemoteSolver::setVerbosity(int level) {
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_SET_VERBOSITY);
    mb.withParameter(level);
    Message *m = mb.withTag(PANO_TAG_CONFIG).build();
    comm->send(m, rank);
    free(m);
}

void RemoteSolver::setTimeout(long seconds) {
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_SET_TIMEOUT);
    mb.withParameter(seconds);
    Message *m = mb.withTag(PANO_TAG_CONFIG).build();
    comm->send(m, rank);
    free(m);
}

void RemoteSolver::setTimeoutMs(long mseconds) {
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_SET_TIMEOUT_MS);
    mb.withParameter(mseconds);
    Message *m = mb.withTag(PANO_TAG_CONFIG).build();
    comm->send(m, rank);
    free(m);
}

void RemoteSolver::reset() {
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_RESET);
    Message *m = mb.withTag(PANO_TAG_SOLVE).build();
    comm->send(m, rank);
    free(m);
}

std::vector<Universe::BigInteger> RemoteSolver::solution() {
    unsigned long size = nVariables() * sizeof(long long) + sizeof(Message) + PANO_DEFAULT_RAB;
    mutex.lock();
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_SOLUTION);
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

int RemoteSolver::nVariables() {
    if (nbVariables < 0) {
        mutex.lock();
        MessageBuilder mb;
        mb.named(PANO_MESSAGE_N_VARIABLES);
        Message *m = mb.withTag(PANO_TAG_RESPONSE).build();
        comm->send(m, rank);
        free(m);

        m = comm->receive(PANO_TAG_RESPONSE, rank, PANO_DEFAULT_MESSAGE_SIZE);
        mutex.unlock();
        nbVariables = *((const int *) m->parameters);
    }
    return nbVariables;
}

int RemoteSolver::nConstraints() {
    if (nbConstraints < 0) {
        mutex.lock();
        MessageBuilder mb;
        mb.named(PANO_MESSAGE_N_CONSTRAINTS);
        Message *m = mb.withTag(PANO_TAG_RESPONSE).build();
        comm->send(m, rank);
        free(m);

        m = comm->receive(PANO_TAG_RESPONSE, rank, 1024);
        mutex.unlock();
        nbConstraints = *((const int *) m->parameters);
    }
    return nbConstraints;
}

void RemoteSolver::setLogFile(const std::string &filename) {

}

void RemoteSolver::setIndex(unsigned i) {
    this->index = i;
}

void RemoteSolver::setComm(INetworkCommunication *c) {
    comm = c;
}


void RemoteSolver::endSearch() {
    MessageBuilder mb;
    mb.named(PANO_MESSAGE_END_SEARCH);
    Message *m = mb.withTag(PANO_TAG_SOLVE).build();
    comm->send(m, rank);
    free(m);
}

void RemoteSolver::loadFilename(const std::string &filename) {
    MessageBuilder mb;
    Message *m = mb.named(PANO_MESSAGE_LOAD).withParameter(filename).withTag(PANO_TAG_SOLVE).build();
    comm->send(m, rank);
    free(m);
}

[[nodiscard]] const std::map<std::string, Universe::IUniverseVariable *> &RemoteSolver::getVariablesMapping() {
    return {};
}

std::map<std::string, Universe::BigInteger> RemoteSolver::mapSolution() {
    return {};
}

void RemoteSolver::setLowerBound(const Universe::BigInteger &lb) {
    MessageBuilder mb;
    Message *m = mb.named(PANO_MESSAGE_LOWER_BOUND).withParameter(lb).withTag(PANO_TAG_SOLVE).build();
    comm->send(m, rank);
    free(m);
}

void RemoteSolver::setUpperBound(const Universe::BigInteger &ub) {
    MessageBuilder mb;
    Message *m = mb.named(PANO_MESSAGE_UPPER_BOUND).withParameter(ub).withTag(PANO_TAG_SOLVE).build();
    comm->send(m, rank);
    free(m);
}

void RemoteSolver::setBounds(const Universe::BigInteger &lb, const Universe::BigInteger &ub) {
    MessageBuilder mb;
    Message *m = mb.named(PANO_MESSAGE_LOWER_UPPER_BOUND).withParameter(lb).withParameter(ub).withTag(
            PANO_TAG_SOLVE).build();
    comm->send(m, rank);
    free(m);
}

Universe::BigInteger RemoteSolver::getCurrentBound() {
    mutex.lock();
    MessageBuilder mb;
    Message *m = mb.named(PANO_MESSAGE_GET_CURRENT_BOUND).withTag(
            PANO_TAG_RESPONSE).build();
    comm->send(m, rank);
    free(m);

    m = comm->receive(PANO_TAG_RESPONSE, rank, 1024);
    std::string param(m->parameters, strlen(m->parameters) + 1);
    Universe::BigInteger newBound = Universe::bigIntegerValueOf(param);
    mutex.unlock();
    free(m);
    return newBound;
}

bool RemoteSolver::isMinimization() {
    mutex.lock();
    MessageBuilder mb;
    Message *m = mb.named(PANO_MESSAGE_IS_MINIMIZATION).withTag(
            PANO_TAG_RESPONSE).build();
    comm->send(m, rank);
    free(m);

    m = comm->receive(PANO_TAG_RESPONSE, rank, 1024);
    bool r = m->read<bool>();
    mutex.unlock();
    free(m);
    return r;
}

Universe::BigInteger RemoteSolver::getLowerBound() {
    mutex.lock();
    MessageBuilder mb;
    Message *m = mb.named(PANO_MESSAGE_GET_LOWER_BOUND).withTag(
            PANO_TAG_RESPONSE).build();
    comm->send(m, rank);
    free(m);

    m = comm->receive(PANO_TAG_RESPONSE, rank, 1024);
    std::string param(m->parameters, strlen(m->parameters) + 1);
    Universe::BigInteger newBound = Universe::bigIntegerValueOf(param);
    mutex.unlock();
    free(m);
    return newBound;
}

Universe::BigInteger RemoteSolver::getUpperBound() {
    mutex.lock();
    MessageBuilder mb;
    Message *m = mb.named(PANO_MESSAGE_GET_UPPER_BOUND).withTag(
            PANO_TAG_RESPONSE).build();
    comm->send(m, rank);
    free(m);

    m = comm->receive(PANO_TAG_RESPONSE, rank, 1024);
    std::string param(m->parameters, strlen(m->parameters) + 1);
    Universe::BigInteger newBound = Universe::bigIntegerValueOf(param);
    mutex.unlock();
    free(m);
    return newBound;
}
