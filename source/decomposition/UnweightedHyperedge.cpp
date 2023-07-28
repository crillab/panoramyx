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
 * @file UnweightedHyperedge.cpp
 * @brief Provides the representation of an unweighted hyperedge.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <crillab-panoramyx/utils/hypergraph/UnweightedHyperedge.hpp>

using namespace Panoramyx;

UnweightedHyperedge::UnweightedHyperedge(int nbVertices, int *vertices) :
        nbVertices(nbVertices),
        vertices(vertices) {
    // Nothing to do: everything is already initialized.
}

UnweightedHyperedge::~UnweightedHyperedge() {
    delete[] vertices;
}

UnweightedHyperedge *UnweightedHyperedge::joining(int nbVertices, int *vertices) {
    return new UnweightedHyperedge(nbVertices, vertices);
}

int UnweightedHyperedge::size() const {
    return nbVertices;
}

const int *UnweightedHyperedge::getVertices() const {
    return vertices;
}
