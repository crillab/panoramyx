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
        char methodName[32];
        int nbParameters;
        int size;
        int tag;
        char parameters[];
    };

}
#endif //PANORAMYX_NETWORKTYPES_HPP
