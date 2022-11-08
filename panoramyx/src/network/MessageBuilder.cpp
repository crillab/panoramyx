/**
* @date 11/10/22
* @file MessageBuilder.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/


#include <cstring>
#include "../../include/network/MessageBuilder.hpp"

namespace Panoramyx {

/**
@class MessageBuilder
@brief Definition of the class MessageBuilder. 
@file MessageBuilder.cpp
*/

    MessageBuilder::MessageBuilder() : size(sizeof(Message)) {
        message = static_cast<Message *>(malloc(sizeof(Message)));
        message->size = 0;
        message->nbParameters = 0;
    }

    MessageBuilder &MessageBuilder::forMethod(const std::string &methodName) {
        strncpy(message->methodName, methodName.c_str(), sizeof(message->methodName));
        return *this;
    }

    Message *MessageBuilder::build() {
        return message;
    }

    MessageBuilder &MessageBuilder::withTag(int tag) {
        message->tag = tag;
        return *this;
    }
} // Panoramyx