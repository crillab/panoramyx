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
 * @file HypergraphBuilder.hpp
 * @brief Provides an easy way of building hypergraphs.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_HYPERGRAPHBUILDER_HPP
#define PANORAMYX_HYPERGRAPHBUILDER_HPP

#include <cstring>

#include "Hypergraph.hpp"
#include "UnweightedHyperedge.hpp"

namespace Panoramyx {

    /**
     * The HypergraphBuilder provides an easy and natural interface for building
     * hypergraphs.
     */
    class HypergraphBuilder {

    private:

        /**
         * The number of vertices in the hypergraph.
         */
        int numberOfVertices;

        /**
         * The current available number of vertex indices.
         */
        int nbVerticesIndex;

        /**
         * The weights of the vertices.
         * If vertices are not weighted, this array remains nullptr.
         */
        int *vertexWeights;

        /**
         * The number of hyperedges in the hypergraph.
         */
        int numberOfHyperedges;

        /**
         * The weights of the hyperedges.
         * If hyperedges are not weighted, this array remains nullptr.
         */
        int *hyperedgeWeights;

        /**
         * The indices at which the vertices of each hyperedge start in
         * hyperedgeVertices.
         */
        long *hyperedgeIndices;

        /**
         * The vertices of the different hyperedges.
         */
        int *hyperedgeVertices;

        /**
         * The index of the current hyperedge.
         */
        int hyperedgeIndex;

        /**
         * The index at which the vertices of the current hyperedge start in
         * hyperedgeVertices.
         */
        int hyperedgeVerticesIndex;

        /**
         * Creates a new HypergraphBuilder.
         *
         * @param nbVertices The number of vertices in the hypergraph.
         * @param nbHyperedges The number of hyperedges in the hypergraph.
         */
        HypergraphBuilder(int nbVertices, int nbHyperedges);

    public:

        /**
         * Creates a new HypergraphBuilder.
         *
         * @param nbVertices The number of vertices in the hypergraph.
         * @param nbHyperedges The number of hyperedges in the hypergraph.
         *
         * @return The created HypergraphBuilder.
         */
        static Panoramyx::HypergraphBuilder *createHypergraph(int nbVertices, int nbHyperedges);

        /**
         * Gives the number of vertices in the hypergraph.
         *
         * @return The number of vertices.
         */
        int getNumberOfVertices();

        /**
         * Sets the weight of a vertex in the hypergraph.
         *
         * @param vertex The vertex to set the weight of.
         * @param weight The weight of the vertex.
         *
         * @return This builder.
         */
        Panoramyx::HypergraphBuilder *withVertexWeight(int vertex, int weight);

        /**
         * Gives the number of hyperedges in the hypergraph.
         *
         * @return The number of hyperedges.
         */
        int getNumberOfHyperedges();

        /**
         * Gives the indices at which the vertices of each hyperedge start in the array
         * returned by {@link #getHyperedgeVertices()}.
         *
         * @return The indices of the vertices for each hyperedge.
         *
         * @see #getHyperedgeVertices()
         */
        long *getHyperedgeIndices();

        /**
         * Gives the vertices of the hyperedges in the hypergraph.
         *
         * @return The vertices of the different hyperedges.
         *
         * @see #getHyperedgeIndices()
         */
        int *getHyperedgeVertices();

        /**
         * Adds an unweighted hyperedge to the hypergraph.
         *
         * @param hyperedge The hyperedge to add.
         *
         * @return This builder.
         */
        Panoramyx::HypergraphBuilder *withHyperedge(Panoramyx::UnweightedHyperedge *hyperedge);

        /**
         * Appends the vertices of the given hyperedge to {@link #hyperedgeVertices}.
         *
         * @param hyperedge The hyperedge to append the vertices of.
         */
        void appendVertices(Panoramyx::Hyperedge *hyperedge);

        /**
         * Appends a vertex to hyperedgeVertices.
         * The size of the array is doubled if there is not enough room to add the vertex.
         *
         * @param vertex The vertex to append.
         */
        void appendVertex(int vertex);

        /**
         * Creates the hypergraph built by this builder.
         *
         * @return The built hypergraph.
         */
        Hypergraph *build();

    };

}

#endif
