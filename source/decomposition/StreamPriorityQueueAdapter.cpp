/**
* @date 28/06/23
* @file StreamPriorityQueueAdapter.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include "crillab-panoramyx/decomposition/StreamPriorityQueueAdapter.hpp"

using namespace Universe;
using namespace std;

namespace Panoramyx {

/**
@class StreamPriorityQueueAdapter
@brief Definition of the class StreamPriorityQueueAdapter.
@file StreamPriorityQueueAdapter.cpp
*/

    StreamPriorityQueueAdapter::StreamPriorityQueueAdapter(priority_queue <ProblemUnderAssumption> &priorityQueue)
            : priorityQueue(priorityQueue) {}

    bool StreamPriorityQueueAdapter::hasNext() const {
        return !priorityQueue.empty();
    }

    vector<UniverseAssumption<BigInteger>> StreamPriorityQueueAdapter::next() {
        auto elt = priorityQueue.top();
        priorityQueue.pop();
        return elt.getAssumpts();
    }


} // Panoramyx