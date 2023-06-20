/**
 * PANORAMYX - Programming pArallel coNstraint sOlveRs mAde aMazingly easY.
 * Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 * If not, see {@link http://www.gnu.org/licenses}.
 */

/**
 * @file MessageBuilder.cpp
 * @brief Builds messages to be sent through the network.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <cstring>
#include <iostream>

#include "../../../libs/autis/libs/universe/universe/include/core/UniverseType.hpp"

#include "../../include/network/MessageBuilder.hpp"

using namespace std;

using namespace Panoramyx;
using namespace Universe;

MessageBuilder::MessageBuilder() :
        size(sizeof(Message)) {
    message = static_cast<Message *>(malloc(sizeof(Message)));
    message->nbParameters = 0;
    message->size = 0;
}

MessageBuilder &MessageBuilder::named(const string &name) {
    strncpy(message->name, name.c_str(), sizeof(message->name));
    return *this;
}

MessageBuilder &MessageBuilder::from(int src) {
    message->src = src;
    return *this;
}

MessageBuilder &MessageBuilder::withTag(int tag) {
    message->tag = tag;
    return *this;
}

MessageBuilder &MessageBuilder::withParameter(string p) {
    message->nbParameters++;
    size += p.size() + 1;
    message = static_cast<Message *>(realloc(message, size));
    memcpy(message->parameters + message->size, p.c_str(), p.size() + 1);
    message->parameters[message->size+p.size()]='\0';
    message->size += p.size() + 1;
    return *this;
}

MessageBuilder &MessageBuilder::withParameter(BigInteger param) {
    this->withParameter(Universe::toString(param));
    return *this;
}

Message *MessageBuilder::build() {
    return message;
}
