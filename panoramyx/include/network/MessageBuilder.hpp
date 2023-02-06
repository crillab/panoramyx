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
 * @file MessageBuilder.hpp
 * @brief Builds messages to be sent through the network.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_MESSAGEBUILDER_HPP
#define PANORAMYX_MESSAGEBUILDER_HPP

#include <cstring>
#include <string>

#include "Message.hpp"

namespace Panoramyx {

    /**
     * The MessageBuilder makes easier the building of messages to be sent
     * through the network.
     */
    class MessageBuilder {

    private:

        /**
         * The message that is being built.
         */
        Message *message;

        /**
         * The (full) size of the message that is being built.
         */
        unsigned long size;

    public:

        /**
         * Creates a new MessageBuilder.
         */
        MessageBuilder();

        /**
         * Specifies the name of the message that is being built.
         *
         * @param name The name of the message.
         *
         * @return This message builder.
         */
        MessageBuilder &named(const std::string &name);

        /**
         * Specifies the source of the message that is being built.
         *
         * @param src The source of the message.
         *
         * @return This message builder.
         */
        MessageBuilder &from(int src);

        /**
         * Specifies the tag of the message that is being built, identifying the kind
         * of which the message is.
         *
         * @param tag The tag of the message.
         *
         * @return This message builder.
         */
        MessageBuilder &withTag(int tag);

        /**
         * Adds a parameter to the message that is being built.
         *
         * @tparam T The type of the parameter to add.
         * @param tag The tag of the message.
         *
         * @return This message builder.
         */
        template<typename T>
        MessageBuilder &withParameter(T param) {
            message->nbParameters++;
            size += sizeof(T);
            message = static_cast<Message *>(realloc(message, size));
            memcpy(message->parameters + message->size, &param, sizeof(param));
            message->size += sizeof(T);
            return *this;
        }
        MessageBuilder &withParameter(std::string param);
        MessageBuilder &withParameter(Universe::BigInteger param);
        /**
         * Builds the message.
         *
         * @return The message to send.
         */
        Message *build();

    };

}

#endif
