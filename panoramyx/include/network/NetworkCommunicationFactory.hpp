/**
* @date 06/02/23
* @file NetworkCommunicationFactory.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_NETWORKCOMMUNICATIONFACTORY_HPP
#define PANORAMYX_NETWORKCOMMUNICATIONFACTORY_HPP

#include "INetworkCommunication.hpp"

namespace Panoramyx {

/**
 * @class NetworkCommunicationFactory
 *
 * @brief 
 * @file NetworkCommunicationFactory.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class NetworkCommunicationFactory {
    private:
        int* argc;
        char*** argv;
    public:
        NetworkCommunicationFactory(int* argc,char***argv);
        INetworkCommunication* createMPINetworkCommunication();
    };

} // Panoramyx

#endif //PANORAMYX_NETWORKCOMMUNICATIONFACTORY_HPP
