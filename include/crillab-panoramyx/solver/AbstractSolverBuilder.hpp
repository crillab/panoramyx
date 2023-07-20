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
 * @file AbstractSolverBuilder.hpp
 * @brief The parent class of all the solvers builders.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#ifndef PANORAMYX_ABSTRACTSOLVERBUILDER_HPP
#define PANORAMYX_ABSTRACTSOLVERBUILDER_HPP

#include <string>
#include <vector>

#include "AbstractParallelSolver.hpp"
#include "../core/Configuration.hpp"
#include "../network/INetworkCommunication.hpp"

namespace Panoramyx {

    /**
     * The AbstractSolverBuilder is the parent class of all the solvers builders.
     */
    class AbstractSolverBuilder {

    protected:

        /**
         * The network communication used to communicate between the solvers.
         */
        Panoramyx::INetworkCommunication *networkCommunication;

        /**
         * The options for the JVM to run (if some solvers are written in Java).
         */
        std::vector<std::string> javaOptions;

        /**
         * The path to the JAR files defining Java solvers (if any).
         */
        std::vector<std::string> jars;

    public:

        /**
         * Destroys this AbstractSolverBuilder.
         */
        virtual ~AbstractSolverBuilder() = default;

        /**
         * Adds an option for the JVM to run (if some solvers are written in Java).
         *
         * @param javaOption The Java option to add.
         *
         * @return This solver builder.
         */
        Panoramyx::AbstractSolverBuilder *withJavaOption(const std::string &javaOption);

        /**
         * Adds options for the JVM to run (if some solvers are written in Java).
         *
         * @param javaOptions The Java options to add.
         *
         * @return This solver builder.
         */
        Panoramyx::AbstractSolverBuilder *withJavaOptions(const std::vector<std::string> &javaOptions);

        /**
         * Adds the path of a JAR file defining Java solvers.
         *
         * @param jar The path to the JAR file to add.
         *
         * @return This solver builder.
         */
        Panoramyx::AbstractSolverBuilder *withJar(const std::string &jar);

        /**
         * Adds the paths of JAR files defining Java solvers.
         *
         * @param jars The paths to the JAR files to add.
         *
         * @return This solver builder.
         */
        Panoramyx::AbstractSolverBuilder *withJars(const std::vector<std::string> &jars);

        /**
         * Sets the network communication to use to communicate between the solvers.
         *
         * @param networkCommunication The network communication to use.
         *
         * @return This solver builder.
         */
        Panoramyx::AbstractSolverBuilder *withNetworkCommunicator(
                Panoramyx::INetworkCommunication *networkCommunication);

        /**
         * Builds the solver.
         *
         * @return The built solver.
         */
        virtual AbstractParallelSolver *build() = 0;

    protected:

        /**
         * Builds the JVM to run Java solvers (if any).
         */
        void buildJVM();

    };

}

#endif
