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
 * @file BlockingDeque.hpp
 * @brief Defines a queue of elements waiting for elements to be added when empty.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_BLOCKINGDEQUE_HPP
#define PANORAMYX_BLOCKINGDEQUE_HPP

#include <deque>
#include <mutex>
#include <semaphore>

#include "../../../libs/exception/except/except.hpp"

namespace Panoramyx {

    /**
     * The BlockingDeque defines a queue of elements, that waits for elements to be
     * added when it is empty.
     * There is no size limit for such a queue.
     *
     * @tparam E The type of the elements in the queue.
     */
    template<typename E>
    class BlockingDeque {

    private:

        /**
         * The queue in which the elements are added.
         */
        std::deque<E> deque;

        /**
         * The mutex ensuring that only one thread can access to deque at a given time.
         */
        std::mutex mutex;

        /**
         * The semaphore used to wait until an element is present in the queue.
         */
        std::binary_semaphore semaphore;

    public:

        /**
         * Creates a new BlockingDeque.
         * The queue is initially empty.
         */
        explicit BlockingDeque() :
                semaphore(0) {
            // Nothing to do: everything is already initialized.
        }

        /**
         * Destroys this BlockingDeque.
         */
        ~BlockingDeque() = default;

        /**
         * Adds an element to this queue.
         *
         * @param e The element to add.
         */
        void add(E e) {
            deque.push_back(e);
            semaphore.release();
        }

        /**
         * Gives the next element from this queue.
         *
         * @return The element at the front of this queue.
         */
        E get() {
            // FIXME Probable UB if multiple threads invoke get(), but why?
            semaphore.acquire();
            mutex.lock();

            if (deque.empty()) {
                // FIXME Maybe do not release the semaphore when clearing to avoid this?
                mutex.unlock();
                throw Except::NoSuchElementException("deque is empty");
            }

            E e = deque.front();
            deque.pop_front();
            mutex.unlock();
            return e;
        }

        /**
         * Removes all the elements from this queue.
         */
        void clear() {
            // FIXME Why do we release the semaphore here?
            deque.clear();
            semaphore.release();
        }

    };

}

#endif
