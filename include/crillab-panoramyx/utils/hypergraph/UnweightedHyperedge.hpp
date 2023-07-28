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
 * @file UnweightedHyperedge.hpp
 * @brief Provides the representation of an unweighted hyperedge.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_UNWEIGHTEDHYPEREDGE_HPP
#define PANORAMYX_UNWEIGHTEDHYPEREDGE_HPP

#include "Hyperedge.hpp"

namespace Panoramyx {

    /**
     * The UnweightedHyperedge represents an unweighted hyperedge in an hypergraph.
     */
    class UnweightedHyperedge : public Panoramyx::Hyperedge {

    private:

        /**
         * The number of vertices in this hyperedge.
         */
        int nbVertices;

        /**
         * The vertices joined by this hyperedge.
         */
        int *vertices;

        /**
         * Creates a new UnweightedHyperedge.
         *
         * @param nbVertices The number of vertices in this hyperedge.
         * @param vertices The vertices joined by the hyperedge.
         */
        UnweightedHyperedge(int nbVertices, int *vertices);

    public:

        /**
         * Destroys this UnweightedHyperedge.
         */
        virtual ~UnweightedHyperedge();

        /**
         * Creates a new UnweightedHyperedge.
         *
         * @param nbVertices The number of vertices in this hyperedge.
         * @param vertices The vertices joined by the hyperedge.
         *
         * @return The created hyperedge.
         */
        static UnweightedHyperedge *joining(int nbVertices, int *vertices);

        /**
         * Gives the size of this hyperedge, measured in number of vertices it joins.
         *
         * @return The size of this hyperedge.
         */
        [[nodiscard]] int size() const override;

        /**
         * Gives the vertices that are joined by this hyperedge in the hypergraph.
         *
         * @return The vertices joined by this hyperedge.
         */
        [[nodiscard]] const int *getVertices() const override;

    };

}

#endif
