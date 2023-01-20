/**
* @date 11/10/22
* @file NetworkTypes.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_NETWORKTYPES_HPP
#define PANORAMYX_NETWORKTYPES_HPP
namespace Panoramyx{
    struct Message{
        char methodName[4];
        int src;
        int nbParameters;
        unsigned long size;
        int tag;
        char parameters[];

        template<typename T>
        inline T read(int i=0) const {
            return *((const T*)(parameters+i));
        }
    };



}
#endif //PANORAMYX_NETWORKTYPES_HPP
