/**
* @date 11/10/22
* @file Configuration.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_CONFIGURATION_HPP
#define PANORAMYX_CONFIGURATION_HPP

#include <string>
#include <unordered_map>

namespace Panoramyx {

/**
 * @class Configuration
 *
 * @brief 
 * @file Configuration.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class Configuration {
    public:
        std::string& operator[](const std::string& key);

    private:
        std::unordered_map<std::string,std::string> map;
    };

} // Panoramyx

#endif //PANORAMYX_CONFIGURATION_HPP
