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
 * @file NetworkCommunicationFactory.hpp
 * @brief Provides a factory for implementations of INetworkCommunication.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_NETWORKCOMMUNICATIONFACTORY_HPP
#define PANORAMYX_NETWORKCOMMUNICATIONFACTORY_HPP

#include "INetworkCommunication.hpp"

namespace Panoramyx {

    /**
     * The NetworkCommunicationFactory allows to instantiate implementations of INetworkCommunication.
     */
    class NetworkCommunicationFactory {

    private:

        /**
         * The pointer to the number of arguments given to the main function.
         */
        int *argc;

        /**
         * The pointer to the array of arguments given to the main function.
         */
        char ***argv;

    public:

        /**
         * Creates a new NetworkCommunicationFactory.
         *
         * @param argc The pointer to the number of arguments given to the main function.
         * @param argv The pointer to the array of arguments given to the main function.
         */
        NetworkCommunicationFactory(int* argc,char***argv);

        /**
         * Creates an instance of MPINetworkCommunication.
         *
         * @return The created instance.
         */
        INetworkCommunication *createMPINetworkCommunication();

        /**
         * Creates an instance of ThreadCommunication.
         *
         * @param nbThreads The number of threads to run.
         *
         * @return The created instance.
         */
        INetworkCommunication *createThreadCommunication(int nbThreads);

    };

}

#endif
