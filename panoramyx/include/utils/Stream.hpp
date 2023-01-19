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
 * @file Stream.hpp
 * @brief Defines a lazy iterable, that computes its next element when needed.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_STREAM_HPP
#define PANORAMYX_STREAM_HPP

#include <functional>
#include <vector>

namespace Panoramyx {

    /**
     * The IteratorStream adapts a Stream to a C++ "iterator".
     * it is forward-declared as the iterator and the stream are dependent on each other.
     *
     * @tparam E The type of the elements in the stream.
     */
    template<typename E>
    class IteratorStream;

    /**
     * A Stream is a lazy iterable object, which only computes its next element
     * when needed.
     *
     * @tparam E The type of the elements in the stream.
     */
    template<typename E>
    class Stream {

    public:

        /**
         * Destroys this Stream.
         */
        virtual ~Stream() = default;

        /**
         * Checks whether there is another element in this Stream.
         *
         * @return Whether there is another element in this Stream.
         */
        [[nodiscard]] virtual bool hasNext() const = 0;

        /**
         * Gives the next element in this stream.
         * The behavior of this method is undefined if hasNext() returned false.
         *
         * @return The next element.
         */
        virtual E next() = 0;

        /**
         * Gives the iterator starting at the beginning of this stream.
         *
         * @return The iterator starting at the beginning of this stream.
         */
        Panoramyx::IteratorStream<E> begin();

        /**
         * Gives the iterator marking the end of this stream.
         *
         * @return The iterator marking the end of this stream.
         */
        Panoramyx::IteratorStream<E> end();

        /**
         * Executes a function on all the elements of this stream.
         *
         * @param f The function to execute.
         */
        void forEach(std::function<void(E)> f) {
            for (E e: *this) {
                f(e);
            }
        }

    };

    /**
     * The IteratorStream adapts a Stream to a C++ "iterator".
     *
     * @tparam E The type of the elements in the stream.
     */
    template<typename E>
    class IteratorStream {

    private:

        /**
         * The stream on which to iterate over.
         */
        Panoramyx::Stream<E> &stream;

        /**
         * Whether this iterator has reached the end of the stream.
         */
        bool ended;

        /**
         * Creates a new IteratorStream.
         *
         * @param stream The stream on which to iterate over.
         * @param ended Whether this iterator marks the end of the stream.
         */
        explicit IteratorStream(Panoramyx::Stream<E> &stream, bool ended = false) :
                stream(stream),
                ended(ended) {
            // Nothing to do, everything is already initialized.
        }

    public:

        /**
         * Gives the next element in the stream.
         *
         * @return The next element.
         */
        E operator*() {
            return stream.next();
        }

        /**
         * Moves this iterator to the next element in the stream.
         *
         * @return This iterator, after having been moved.
         */
        Panoramyx::IteratorStream<E> &operator++() {
            if (!stream.hasNext()) {
                ended = true;
            }
            return *this;
        }

        /**
         * Compares this iterator with another one, to check whether the end of
         * the stream has been reached.
         * This method should be used with caution, and only in for-each constructs
         * (that is, in compiler-generated code).
         * The comparison between iterators on different streams is undefined!
         *
         * @param other The iterator to compare this one with.
         *
         * @return Whether this iterator and the other are different, based on whether
         *         they have both reached the end of the stream.
         */
        bool operator!=(Panoramyx::IteratorStream<E> &other) {
            return ended != other.ended;
        }

        /**
         * The Stream class is a friend as it is the only class allowed to
         * instantiate an iterator.
         */
        friend class Panoramyx::Stream<E>;

    };

    template<typename E>
    Panoramyx::IteratorStream<E> Stream<E>::begin() {
        return Panoramyx::IteratorStream<E>(*this);
    }

    template<typename E>
    Panoramyx::IteratorStream<E> Stream<E>::end() {
        return Panoramyx::IteratorStream<E>(*this, true);
    }

}

#endif
