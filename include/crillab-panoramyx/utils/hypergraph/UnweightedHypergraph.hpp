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
 * @file UnweightedHypergraph.hpp
 * @brief Provides the representation of an unweighted hypergraph.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_UNWEIGHTEDHYPERGRAPH_HPP
#define PANORAMYX_UNWEIGHTEDHYPERGRAPH_HPP

#include "Hyperedge.hpp"
#include "HypergraphBuilder.hpp"
#include "UnweightedHyperedge.hpp"

namespace Panoramyx {

    /**
     * The UnweightedHypergraph represents an unweighted hyperedge in an hypergraph.
     */
    class UnweightedHypergraph : public Panoramyx::Hypergraph {

    private:

        /**
         * The number of vertices in this hypergraph.
         */
        int numberOfVertices;

        /**
         * The number of hyperedges in this hypergraph.
         */
        int numberOfHyperedges;

        /**
         * The indices at which the vertices of each hyperedge start in hyperedgeVertices.
         */
        long *hyperedgeIndices;

        /**
         * The vertices of the different hyperedges.
         */
        int *hyperedgeVertices;

    public:

        /**
         * Creates a new UnweightedHypergraph.
         *
         * @param builder The builder to create the hypergraph from.
         */
        UnweightedHypergraph(Panoramyx::HypergraphBuilder *builder);

        /**
         * Destroys this UnweightedHypergraph.
         */
        ~UnweightedHypergraph() = default;

        /**
         * Gives the number of vertices in this hypergraph.
         *
         * @return The number of vertices.
         */
        [[nodiscard]] int getNumberOfVertices() const override;

        /**
         * Gives the weights of the vertices in this hypergraph.
         *
         * @return The weights of the vertices, or nullptr if vertices are not weighted.
         */
        [[nodiscard]] const int *getVertexWeights() const override;

        /**
         * Gives the number of hyperedges in this hypergraph.
         *
         * @return The number of hyperedges.
         */
        [[nodiscard]] int getNumberOfHyperedges() const override;

        /**
         * Gives the {@code index}-th hyperedge in this hypergraph.
         *
         * @param index The index of the hyperedge to get.
         *
         * @return The {@code index}-th hyperedge.
         */
        [[nodiscard]] const Panoramyx::Hyperedge *getHyperedge(int index) const;

        /**
         * Gives the weights of the hyperedges in this hypergraph.
         *
         * @return The weights of the hyperedges, or nullptr if hyperedges are not weighted.
         */
        [[nodiscard]] const int *getHyperedgeWeights() const override;

        /**
         * Gives the indices at which the vertices of each hyperedge start in the array
         * returned by getHyperedgeVertices().
         *
         * @return The indices of the vertices for each hyperedge.
         *
         * @see getHyperedgeVertices()
         */
        [[nodiscard]] const long *getHyperedgeIndices() const override;

        /**
         * Gives the vertices of the hyperedges in the hypergraph.
         *
         * @return The vertices of the different hyperedges.
         *
         * @see getHyperedgeIndices()
         */
        [[nodiscard]] const int *getHyperedgeVertices() const override;

    };

}

#endif
