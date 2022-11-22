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

#define PANO_TAG_RESPONSE 1
#define PANO_TAG_SOLVE 2
#define PANO_TAG_CONFIG 4
#define PANO_ANY_TAG MPI_ANY_TAG


#define PANO_DEFAULT_MESSAGE_SIZE 1024
#define PANO_DEFAULT_RAB 512

#define PANO_MESSAGE_LOAD "lod"
#define PANO_MESSAGE_SOLVE "s"
#define PANO_MESSAGE_SOLVE_FILENAME "sf"
#define PANO_MESSAGE_SOLVE_ASSUMPTIONS "sa"
#define PANO_MESSAGE_INTERRUPT "i"
#define PANO_MESSAGE_SET_VERBOSITY "v"
#define PANO_MESSAGE_SET_TIMEOUT "t"
#define PANO_MESSAGE_SET_TIMEOUT_MS "tm"
#define PANO_MESSAGE_SET_LOG_FILE "log"
#define PANO_MESSAGE_N_VARIABLES "nv"
#define PANO_MESSAGE_N_CONSTRAINTS "nc"
#define PANO_MESSAGE_END_SEARCH "end"
#define PANO_MESSAGE_SOLUTION "sol"
#define PANO_MESSAGE_RESET "rst"

#define PANO_MESSAGE_SATISFIABLE "sat"
#define PANO_MESSAGE_UNSATISFIABLE "ust"
#define PANO_MESSAGE_UNSUPPORTED "usp"
#define PANO_MESSAGE_UNKNOWN "unk"
#define PANO_MESSAGE_OPTIMUM_FOUND "opt"


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

        virtual Message *receive(int tag, int src, unsigned long size) = 0;

        virtual void send(Message *m, int dest) = 0;

        virtual ~INetworkCommunication() = default;
    };

} // Panoramyx

#endif //PANORAMYX_INETWORKCOMMUNICATION_HPP
