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
 * @file Hyperedge.hpp
 * @brief The interface defining an hyperedge from an hypergraph.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_HYPEREDGE_HPP
#define PANORAMYX_HYPEREDGE_HPP

namespace Panoramyx {

    /**
     * The Hyperedge defines the contract for representing hyperedges in an hypergraph.
     */
    class Hyperedge {

    public:

        /**
         * Destroys this Hyperedge.
         */
        virtual ~Hyperedge() = default;

        /**
         * Gives the size of this hyperedge, measured in number of vertices it joins.
         *
         * @return The size of this hyperedge.
         */
        [[nodiscard]] virtual int size() const = 0;

        /**
         * Gives the vertices that are joined by this hyperedge in the hypergraph.
         *
         * @return The vertices joined by this hyperedge.
         */
        [[nodiscard]] virtual const int *getVertices() const = 0;

    };

}

#endif
