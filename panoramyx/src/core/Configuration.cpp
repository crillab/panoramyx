/**
* @date 11/10/22
* @file Configuration.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include "../../include/core/Configuration.hpp"

namespace Panoramyx {

/**
@class Configuration
@brief Definition of the class Configuration. 
@file Configuration.cpp
*/

    std::string &Configuration::operator[](const std::string &key) {
        return map[key];
    }

} // Panoramyx