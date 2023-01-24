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
 * @file Configuration.hpp
 * @brief Provides an object for configuring Panoramyx.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_CONFIGURATION_HPP
#define PANORAMYX_CONFIGURATION_HPP

#include <string>
#include <unordered_map>

namespace Panoramyx {

    /**
     * The Configuration provides an object for configuring Panoramyx.
     */
    class Configuration {

    private:

        /**
         * The map storing the configuration.
         */
        std::unordered_map<std::string, std::string> map;

    public:

        /**
         * Gives the configured value for the given key.
         *
         * @param key The key to get the value of.
         *
         * @return The configured value for the given key.
         */
        std::string &operator[](const std::string &key);

    };

}

#endif
