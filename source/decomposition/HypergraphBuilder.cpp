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
 * @file HypergraphBuilder.cpp
 * @brief Provides an easy way of building hypergraphs.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <cstring>

#include <crillab-panoramyx/utils/hypergraph/Hypergraph.hpp>
#include <crillab-panoramyx/utils/hypergraph/UnweightedHyperedge.hpp>
#include <crillab-panoramyx/utils/hypergraph/UnweightedHypergraph.hpp>
#include <crillab-panoramyx/utils/hypergraph/HypergraphBuilder.hpp>

using namespace Panoramyx;

HypergraphBuilder::HypergraphBuilder(int nbVertices, int nbHyperedges) :
        numberOfVertices(nbVertices),
        nbVerticesIndex(nbHyperedges << 1),
        vertexWeights(nullptr),
        numberOfHyperedges(nbHyperedges),
        hyperedgeWeights(nullptr),
        hyperedgeIndices(new long[nbHyperedges + 1]),
        hyperedgeVertices(new int[nbHyperedges << 1]),
        hyperedgeIndex(0),
        hyperedgeVerticesIndex(0) {
    // Nothing to do: everything is already initialized.
}

HypergraphBuilder *HypergraphBuilder::createHypergraph(int nbVertices, int nbHyperedges) {
    return new HypergraphBuilder(nbVertices, nbHyperedges);
}

int HypergraphBuilder::getNumberOfVertices() {
    return numberOfVertices;
}

HypergraphBuilder *HypergraphBuilder::withVertexWeight(int vertex, int weight) {
    if (vertexWeights == nullptr) {
        // This is the first vertex for which a weight is specified.
        vertexWeights = new int[numberOfVertices];
    }

    vertexWeights[vertex - 1] = weight;
    return this;
}

int HypergraphBuilder::getNumberOfHyperedges() {
    return numberOfHyperedges;
}

long *HypergraphBuilder::getHyperedgeIndices() {
    return hyperedgeIndices;
}

int *HypergraphBuilder::getHyperedgeVertices() {
    return hyperedgeVertices;
}

HypergraphBuilder *HypergraphBuilder::withHyperedge(UnweightedHyperedge *hyperedge) {
    appendVertices(hyperedge);
    return this;
}

void HypergraphBuilder::appendVertices(Hyperedge *hyperedge) {
    hyperedgeIndices[hyperedgeIndex++] = hyperedgeVerticesIndex;
    for (int i = 0; i < hyperedge->size(); i++) {
        appendVertex(hyperedge->getVertices()[i]);
    }
}

void HypergraphBuilder::appendVertex(int vertex) {
    if (nbVerticesIndex == hyperedgeVerticesIndex) {
        int *tmp = new int[nbVerticesIndex << 1];
        // There is not enough room for adding a vertex.
        memcpy(tmp, hyperedgeVertices, (hyperedgeVerticesIndex) * sizeof(int));
        hyperedgeVertices = tmp;
        nbVerticesIndex <<= 1;
    }

    hyperedgeVertices[hyperedgeVerticesIndex++] = vertex - 1;
}

Hypergraph *HypergraphBuilder::build() {
    // Terminating the vertices.
    hyperedgeIndices[hyperedgeIndex] = hyperedgeVerticesIndex;
    int *tmp = new int[hyperedgeVerticesIndex];
    memcpy(tmp, hyperedgeVertices, hyperedgeVerticesIndex * sizeof(int));
    delete[] hyperedgeVertices;
    hyperedgeVertices = tmp;

    // Creating the hypergraph.
    return new UnweightedHypergraph(this);
}
