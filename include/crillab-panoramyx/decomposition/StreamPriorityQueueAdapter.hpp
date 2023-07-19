/**
 * PANORAMYX - Programming pArallel coNstraint sOlveRs mAde aMazingly easY.
 * Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 * If not, see {@link http://www.gnu.org/licenses}.
 */

/**
 * @file StreamPriorityQueueAdapter.hpp
 * @brief Adapts a priority queue of ProblemUnderAssumptions to a stream of assumptions.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_STREAMPRIORITYQUEUEADAPTER_HPP
#define PANORAMYX_STREAMPRIORITYQUEUEADAPTER_HPP

#include <queue>

#include "AbstractCubeGenerator.hpp"
#include "ProblemUnderAssumption.hpp"

namespace Panoramyx {

    /**
     * The StreamPriorityQueueAdapter adapts a priority queue of ProblemUnderAssumptions to a stream of assumptions.
     */
    class StreamPriorityQueueAdapter :
            public Panoramyx::Stream<std::vector<Universe::UniverseAssumption<Universe::BigInteger>>> {

    private:

        /**
         * The adapted priority queue.
         */
        std::priority_queue<Panoramyx::ProblemUnderAssumption> &priorityQueue;

    public:

        /**
         * Creates a new StreamPriorityQueueAdapter.
         *
         * @param priorityQueue The priority queue to adapt.
         */
        explicit StreamPriorityQueueAdapter(std::priority_queue<Panoramyx::ProblemUnderAssumption> &priorityQueue);

        /**
         * Destroys this StreamPriorityQueueAdapter.
         */
        ~StreamPriorityQueueAdapter() override = default;

        /**
         * Checks whether there is another assumption in this stream.
         *
         * @return Whether there is another assumption in this stream.
         */
        bool hasNext() const override;

        /**
         * Gives the next assumption in this stream.
         * The behavior of this method is undefined if hasNext() returned false.
         *
         * @return The next element.
         */
        std::vector<Universe::UniverseAssumption<Universe::BigInteger>> next() override;

    };

}

#endif
