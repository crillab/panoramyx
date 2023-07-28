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
 * @file Hypergraph.hpp
 * @brief The interface defining an hypergraph.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_HYPERGRAPH_HPP
#define PANORAMYX_HYPERGRAPH_HPP

#include "Hyperedge.hpp"

namespace Panoramyx {

    /**
     * The Hypergraph defines the contract for representing hypergraphs.
     */
    class Hypergraph {

    public:

        /**
         * Gives the number of vertices in this hypergraph.
         *
         * @return The number of vertices.
         */
        [[nodiscard]] virtual int getNumberOfVertices() const = 0;

        /**
         * Gives the weights of the vertices in this hypergraph.
         *
         * @return The weights of the vertices, or nullptr if vertices are not weighted.
         */
        [[nodiscard]] virtual const int *getVertexWeights() const = 0;

        /**
         * Gives the number of hyperedges in this hypergraph.
         *
         * @return The number of hyperedges.
         */
        [[nodiscard]] virtual int getNumberOfHyperedges() const = 0;

        /**
         * Gives the {@code index}-th hyperedge in this hypergraph.
         *
         * @param index The index of the hyperedge to get.
         *
         * @return The {@code index}-th hyperedge.
         */
        [[nodiscard]] virtual const Panoramyx::Hyperedge *getHyperedge(int index) const = 0;

        /**
         * Gives the weights of the hyperedges in this hypergraph.
         *
         * @return The weights of the hyperedges, or nullptr if hyperedges are not weighted.
         */
        [[nodiscard]] virtual const int *getHyperedgeWeights() const = 0;

        /**
         * Gives the indices at which the vertices of each hyperedge start in the array
         * returned by getHyperedgeVertices().
         *
         * @return The indices of the vertices for each hyperedge.
         *
         * @see getHyperedgeVertices()
         */
        [[nodiscard]] virtual const long *getHyperedgeIndices() const = 0;

        /**
         * Gives the vertices of the hyperedges in the hypergraph.
         *
         * @return The vertices of the different hyperedges.
         *
         * @see getHyperedgeIndices()
         */
        [[nodiscard]] virtual const int *getHyperedgeVertices() const = 0;

    };

}

#endif
