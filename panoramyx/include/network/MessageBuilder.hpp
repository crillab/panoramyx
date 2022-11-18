/**
* @date 11/10/22
* @file MessageBuilder.hpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#ifndef PANORAMYX_MESSAGEBUILDER_HPP
#define PANORAMYX_MESSAGEBUILDER_HPP

#include <string>
#include "NetworkTypes.hpp"
#include <cstring>
#include <iostream>

namespace Panoramyx {

/**
 * @class MessageBuilder
 *
 * @brief 
 * @file MessageBuilder.hpp
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @version 0.1.0
 */

    class MessageBuilder {
    private:
        Message* message;
        unsigned long size;
    public:
        MessageBuilder();
        MessageBuilder& forMethod(const std::string& methodName);
        MessageBuilder& from(int src);
        MessageBuilder& withTag(int tag);
        template<typename T>
        MessageBuilder& withParameter(T p) {
            message->nbParameters++;
            size+=sizeof(T);

            message = static_cast<Message *>(realloc(message, size));
            memcpy(message->parameters+message->size,&p,sizeof(p));
            message->size+=sizeof(T);
            return *this;
        }


        Message* build();
    };



} // Panoramyx

#endif //PANORAMYX_MESSAGEBUILDER_HPP
