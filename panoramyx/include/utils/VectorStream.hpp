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
 * @file VectorStream.hpp
 * @brief Defines an adapter of a vector as a Stream.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_VECTORSTREAM_HPP
#define PANORAMYX_VECTORSTREAM_HPP

#include <vector>

#include "Stream.hpp"

namespace Panoramyx {

    /**
     * The VectorStream adapts a vector (from the STL) as a Stream.
     *
     * @tparam E The type of the elements to iterate over.
     */
    template<typename E>
    class VectorStream : public Stream<E> {

    private:

        /**
         * The reference to the adapted vector.
         */
        const std::vector<E> &vector;

        /**
         * The current index in the vector.
         */
        size_t index;

    public:

        /**
         * Creates a new VectorStream.
         *
         * @param vector The vector to adapt.
         */
        explicit VectorStream(const std::vector<E> &vector) :
                vector(vector),
                index(0) {
            // Nothing to do: everything is already initialized.
        }

        /**
         * Destroys this VectorStream.
         */
        ~VectorStream() override = default;

        /**
         * Checks whether there is another element in this Stream.
         *
         * @return Whether there is another element in this Stream.
         */
        [[nodiscard]] bool hasNext() const override {
            return index < vector.size();
        }

        /**
         * Gives the next element in this stream.
         * The behavior of this method is undefined if hasNext() returned false.
         *
         * @return The next element.
         */
        E next() override {
            return vector[index++];
        }

    };

}

#endif
