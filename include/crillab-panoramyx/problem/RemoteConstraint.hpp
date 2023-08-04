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
 * @file RemoteConstraint.hpp
 * @brief Provides the representation of a Universe constraint that is defined in a remote solver.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_REMOTECONSTRAINT_HPP
#define PANORAMYX_REMOTECONSTRAINT_HPP

#include <mutex>
#include <vector>

#include <crillab-universe/core/problem/IUniverseConstraint.hpp>

#include "../network/INetworkCommunication.hpp"

namespace Panoramyx {

    /**
     * The RemoteConstraint provides the representation of a Universe constraint that is
     * defined in a remote solver.
     */
    class RemoteConstraint : public Universe::IUniverseConstraint {

    private:

        /**
         * The communicator used to communicate with the remote solver.
         */
        Panoramyx::INetworkCommunication *communicator;

        /**
         * The mutex guaranteeing that two messages will not interfere.
         */
        std::mutex &mutex;

        /**
         * The rank of the solver that owns this constraint.
         */
        int solverRank;

        /**
         * The index of this constraint in the solver.
         */
        int constraintIndex;

    public:

        /**
         * Creates a new RemoteConstraint.
         *
         * @param communicator The communicator used to communicate with the remote solver.
         * @param solverRank The rank of the solver that owns this constraint.
         * @param constraintIndex The index of this constraint in the solver.
         */
        RemoteConstraint(INetworkCommunication *communicator, std::mutex &mutex, int solverRank, int constraintIndex);

        /**
         * Destroys this RemoteConstraint.
         */
        ~RemoteConstraint() override = default;

        /**
         * Gives the scope of this constraint, i.e., the variables that appear in
         * this constraint.
         *
         * @return The variables involved in this constraint.
         */
        const std::vector<Universe::IUniverseVariable *> &scope() override;

        /**
         * Sets whether this constraint must be ignored by the solver.
         *
         * @param ignored Whether the constraint should be ignored.
         */
        void setIgnored(bool ignored) override;

        /**
         * Checks whether this constraint must be ignored by the solver.
         *
         * @return Whether the constraint should be ignored.
         */
        const bool isIgnored() const override;

        /**
         * Gives the score of this constraint, as computed by the solver.
         *
         * @return The score of the constraint.
         */
        const double getScore() const override;

    };

}

#endif
