/**
* @date 12/10/22
* @file MpiNetworkCommunication.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_MPINETWORKCOMMUNICATION_HPP
#define PANORAMYX_MPINETWORKCOMMUNICATION_HPP

#include "INetworkCommunication.hpp"

namespace Panoramyx {

/**
 * @class MPINetworkCommunication
 *
 * @brief 
 * @file MpiNetworkCommunication.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class MPINetworkCommunication: public INetworkCommunication {
    public:

        static INetworkCommunication* getInstance();

        int getId() override;

        int nbProcesses() override;

        Message *receive(int tag, int src, unsigned long size) override;

        void send(Message *m, int dest) override;

        ~MPINetworkCommunication() override = default;
    private:
        int rank=-1;
        int worldSize=-1;
        static INetworkCommunication* instance;
    };

} // Panoramyx

#endif //PANORAMYX_MPINETWORKCOMMUNICATION_HPP
