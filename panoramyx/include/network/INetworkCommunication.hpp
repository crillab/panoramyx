/**
* @date 12/10/22
* @file INetworkCommunication.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_INETWORKCOMMUNICATION_HPP
#define PANORAMYX_INETWORKCOMMUNICATION_HPP

#include "NetworkTypes.hpp"

namespace Panoramyx {

/**
 * @class INetworkCommunication
 *
 * @brief 
 * @file INetworkCommunication.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class INetworkCommunication {
    public:
        virtual int getId() = 0;

        virtual int nbProcesses() = 0;

        virtual Message *receive(int tag, int src, int size) = 0;

        virtual void send(Message *m, int dest) = 0;

        virtual ~INetworkCommunication() = default;
    };

} // Panoramyx

#endif //PANORAMYX_INETWORKCOMMUNICATION_HPP
