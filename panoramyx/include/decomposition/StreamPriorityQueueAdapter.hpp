/**
* @date 28/06/23
* @file StreamPriorityQueueAdapter.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_STREAMPRIORITYQUEUEADAPTER_HPP
#define PANORAMYX_STREAMPRIORITYQUEUEADAPTER_HPP

#include <queue>
#include "AbstractCubeGenerator.hpp"
#include "ProblemUnderAssumption.hpp"

namespace Panoramyx {


/**
 * @class StreamPriorityQueueAdapter
 *
 * @brief 
 * @file StreamPriorityQueueAdapter.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class StreamPriorityQueueAdapter
            : public Panoramyx::Stream<std::vector<Universe::UniverseAssumption<Universe::BigInteger>>> {
    public:
        explicit StreamPriorityQueueAdapter(std::priority_queue<ProblemUnderAssumption> &priorityQueue);

        ~StreamPriorityQueueAdapter() override = default;

        bool hasNext() const override;

        std::vector<Universe::UniverseAssumption<Universe::BigInteger>> next() override;
    private:
        std::priority_queue<ProblemUnderAssumption>& priorityQueue;
    };

} // Panoramyx

#endif //PANORAMYX_STREAMPRIORITYQUEUEADAPTER_HPP
