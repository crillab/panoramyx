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
 * @file UnweightedHypergraph.cpp
 * @brief Provides the representation of an unweighted hypergraph.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <crillab-panoramyx/utils/hypergraph/UnweightedHypergraph.hpp>

using namespace Panoramyx;

UnweightedHypergraph::UnweightedHypergraph(HypergraphBuilder *builder) :
        numberOfVertices(builder->getNumberOfVertices()),
        numberOfHyperedges(builder->getNumberOfHyperedges()),
        hyperedgeIndices(builder->getHyperedgeIndices()),
        hyperedgeVertices(builder->getHyperedgeVertices()) {
    // Nothing to do: everything is already initialized.
}

int UnweightedHypergraph::getNumberOfVertices() const {
    return numberOfVertices;
}

const int *UnweightedHypergraph::getVertexWeights() const {
    return nullptr;
}

int UnweightedHypergraph::getNumberOfHyperedges() const {
    return numberOfVertices;
}

const Hyperedge *UnweightedHypergraph::getHyperedge(int index) const {
    int begin = (int) hyperedgeIndices[index];
    int end = (int) hyperedgeIndices[index + 1];
    int nbVertices = end - begin;
    int *vertices = new int[nbVertices];

    // Copying the vertices, considering that vertices are currently shifted.
    for (int i = 0; i < nbVertices; i++) {
        vertices[i] = hyperedgeVertices[begin + i] + 1;
    }

    return UnweightedHyperedge::joining(nbVertices, vertices);
}

const int *UnweightedHypergraph::getHyperedgeWeights() const {
    return nullptr;
}

const long *UnweightedHypergraph::getHyperedgeIndices() const {
    return hyperedgeIndices;
}

const int *UnweightedHypergraph::getHyperedgeVertices() const {
    return hyperedgeVertices;
}
