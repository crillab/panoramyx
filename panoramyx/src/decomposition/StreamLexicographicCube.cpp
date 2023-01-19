/**
* @date 07/12/22
* @file StreamLexicographicCube.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include "../../include/decomposition/StreamLexicographicCube.hpp"
#include "../../include/core/IConsistencyChecker.hpp"
#include <cassert>

using namespace Universe;
using namespace std;
using namespace Panoramyx;

/**
@class StreamLexicographicCube
@brief Definition of the class StreamLexicographicCube. 
@file StreamLexicographicCube.cpp
*/
StreamLexicographicCube::StreamLexicographicCube(const std::map<std::string, Universe::IUniverseVariable *> &mapping,
                                                 size_t nbCubeMax, IConsistencyChecker *checker) : mapping(mapping),
                                                                                                   nbCubeMax(nbCubeMax),
                                                                                                   checker(checker) {

}

std::vector<UniverseAssumption<BigInteger>> StreamLexicographicCube::next() {
    if (current.empty()) {
        generateFirst();
    } else {
        generateNext();
    }
    return current;
}

bool StreamLexicographicCube::hasNext() const {
    return indexesCurrentValues.empty() || !variablesFinish[variablesFinish.size() - 1];
}

void StreamLexicographicCube::generateFirst() {
    size_t nb = 1;
    for (auto &items: mapping) {
        current.emplace_back(items.second->getId(), true, items.second->getDomain()->getValues().at(0));
        variables.push_back(items.second);
        indexesCurrentValues.emplace_back(0);
        bool currentVariableIsFinish = items.second->getDomain()->size() == 1;
        if (!variablesFinish.empty()) {
            currentVariableIsFinish &= variablesFinish[variablesFinish.size() - 1];
        }
        variablesFinish.emplace_back(currentVariableIsFinish);
        nb *= items.second->getDomain()->size();
        if (nb >= nbCubeMax) {
            break;
        }
    }
    if (!checker->checkPartial(current) || !checker->checkFinal(current)) {
        generateNext();
    }
}

void StreamLexicographicCube::generateNext() {
    bool partialOk;
    do {
        partialOk = true;
        int i;
        for (i = current.size() - 1; i >= 0 && indexesCurrentValues[i] == variables[i]->getDomain()->size() - 1; i--) {
            current.pop_back();
        }
        if (i < 0) {
            current.clear();
            return;
        }
        current.pop_back();
        current.emplace_back(variables[i]->getId(), true,
                             variables[i]->getDomain()->getValues().at(++indexesCurrentValues[i]));
        if (!checker->checkPartial(current)) {
            partialOk = false;
            continue;
        }
        variablesFinish[i] = indexesCurrentValues[i] == variables[i]->getDomain()->size() - 1;
        if (i != 0) {
            variablesFinish[i] = variablesFinish[i] && variablesFinish[i - 1];
        }

        for (i++; i < variables.size(); i++) {
            partialOk = false;
            for (int v = 0; v < variables[i]->getDomain()->size(); v++) {
                current.emplace_back(variables[i]->getId(), true, variables[i]->getDomain()->getValues().at(v));
                indexesCurrentValues[i] = v;
                variablesFinish[i] = variables[i]->getDomain()->size() == v + 1 && variablesFinish[i - 1];
                if (checker->checkPartial(current)) {
                    partialOk = true;
                    break;
                }
               current.pop_back();
            }
            if (!partialOk) {
                break;
            }
        }
    } while (!partialOk || !checker->checkFinal(current));
}

