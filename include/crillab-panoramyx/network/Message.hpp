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
 * @file Message.hpp
 * @brief Defines a structure that can be sent through the network.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_MESSAGE_HPP
#define PANORAMYX_MESSAGE_HPP

#include <cstring>

#define PANO_ANY_SOURCE (-1)

#define PANO_ANY_TAG (-1)
#define PANO_TAG_RESPONSE 1
#define PANO_TAG_SOLVE 2
#define PANO_TAG_CONFIG 4

#define PANO_DEFAULT_MESSAGE_SIZE 1024
#define PANO_NUMBER_MAX_CHAR 20
#define PANO_VARIABLE_NAME_MAX_CHAR 20

#define PANO_MESSAGE_CONSTRAINT_SET_IGNORED "igr"
#define PANO_MESSAGE_CONSTRAINT_IS_IGNORED "ig?"
#define PANO_MESSAGE_CONSTRAINT_SCORE "scr"

#define PANO_MESSAGE_INDEX "idx"
#define PANO_MESSAGE_RESET "rst"
#define PANO_MESSAGE_N_VARIABLES "nv"
#define PANO_MESSAGE_GET_VARIABLES_MAPPING "vmp"
#define PANO_MESSAGE_GET_AUXILIARY_VARIABLES "aux"
#define PANO_MESSAGE_GET_CONSTRAINTS "ctr"
#define PANO_MESSAGE_DECISION_VARIABLES "dec"
#define PANO_MESSAGE_VALUE_HEURISTIC_STATIC "vhs"
#define PANO_MESSAGE_N_CONSTRAINTS "nc"
#define PANO_MESSAGE_IS_OPTIMIZATION "op?"
#define PANO_MESSAGE_IS_MINIMIZATION "min"
#define PANO_MESSAGE_SET_TIMEOUT "t"
#define PANO_MESSAGE_SET_TIMEOUT_MS "tm"
#define PANO_MESSAGE_SET_VERBOSITY "v"
#define PANO_MESSAGE_ADD_SEARCH_LISTENER "adl"
#define PANO_MESSAGE_SET_LOG_FILE "log"
#define PANO_MESSAGE_SET_LOG_STREAM "lgs"
#define PANO_MESSAGE_LOAD_INSTANCE "lod"
#define PANO_MESSAGE_SOLVE "s"
#define PANO_MESSAGE_SOLVE_FILENAME "sf"
#define PANO_MESSAGE_SOLVE_ASSUMPTIONS "sa"
#define PANO_MESSAGE_INTERRUPT "i"
#define PANO_MESSAGE_SOLUTION "sol"
#define PANO_MESSAGE_MAP_SOLUTION "map"
#define PANO_MESSAGE_CHECK_SOLUTION "chk"
#define PANO_MESSAGE_CHECK_SOLUTION_ASSIGNMENT "cka"
#define PANO_MESSAGE_END_SEARCH "end"

#define PANO_MESSAGE_SATISFIABLE "sat"
#define PANO_MESSAGE_UNSATISFIABLE "ust"
#define PANO_MESSAGE_OPTIMUM_FOUND "opt"
#define PANO_MESSAGE_UNSUPPORTED "usp"
#define PANO_MESSAGE_UNKNOWN "unk"

#define PANO_MESSAGE_LOWER_BOUND "low"
#define PANO_MESSAGE_UPPER_BOUND "upp"
#define PANO_MESSAGE_LOWER_UPPER_BOUND "lub"
#define PANO_MESSAGE_GET_CURRENT_BOUND "cur"
#define PANO_MESSAGE_GET_LOWER_BOUND "lb?"
#define PANO_MESSAGE_GET_UPPER_BOUND "ub?"
#define PANO_MESSAGE_NEW_BOUND_FOUND "bnd"

#define IS(str) (str)
#define NAME_OF(msg, n) (strncmp((msg)->name, (n), sizeof((msg)->name)) == 0)

namespace Panoramyx {

    /**
     * The Message is a structure that may be sent in the network to exchange
     * information between processes.
     */
    struct Message {

        /**
         * Reads a value from this message.
         *
         * @tparam T The type of the value to read.
         *
         * @param index The byte index at which to read the value in the message.
         *
         * @return The read value.
         */
        template<typename T>
        inline T read(int index = 0) const {
            return *((const T *) (parameters + index));
        }

        /**
         * The tag identifying of which kind this message is.
         */
        int tag;

        /**
         * The name of this message.
         */
        char name[4];

        /**
         * The source of the message (i.e., the identifier of the communicator that has sent it).
         */
        int src;

        /**
         * The number of parameters sent in the message.
         */
        int nbParameters;

        /**
         * The size of the parameters of this message.
         */
        unsigned long size;

        /**
         * The parameters of this message.
         * This array has the size stored in the size field.
         */
        char parameters[];

    };

}

#endif
