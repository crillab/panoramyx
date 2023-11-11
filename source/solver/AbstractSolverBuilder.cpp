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
 * @file AbstractSolverBuilder.cpp
 * @brief The parent class of all the solvers builders.
 *
 * @author Thibault Falque
 * @author Romain Wallon
 *
 * @copyright Copyright (c) 2022-2023 - Univ Artois & CNRS & Exakis Nelite.
 * @license This project is released under the GNU LGPL3 License.
 */

#include <crillab-easyjni/JavaVirtualMachineBuilder.h>
#include <crillab-easyjni/JavaVirtualMachineRegistry.h>

#include <crillab-panoramyx/solver/AbstractSolverBuilder.hpp>

using namespace std;

using namespace easyjni;
using namespace Panoramyx;

AbstractSolverBuilder *AbstractSolverBuilder::withJar(const string &jar) {
    this->jars.emplace_back(jar);
    return this;
}

AbstractSolverBuilder *AbstractSolverBuilder::withJars(const vector<string> &jars) {
    this->jars.insert(this->jars.end(), jars.begin(), jars.end());
    return this;
}

AbstractSolverBuilder *AbstractSolverBuilder::withJavaOption(const string &javaOption) {
    this->javaOptions.emplace_back(javaOption);
    return this;
}

AbstractSolverBuilder *AbstractSolverBuilder::withJavaOptions(const vector<string> &javaOptions) {
    this->javaOptions.insert(this->javaOptions.end(), javaOptions.begin(), javaOptions.end());
    return this;
}

AbstractSolverBuilder *AbstractSolverBuilder::withNetworkCommunicator(INetworkCommunication *networkCommunication) {
    this->networkCommunication = networkCommunication;
    return this;
}

void AbstractSolverBuilder::buildJVM() {
    if (!jars.empty()) {
        JavaVirtualMachineBuilder builder = JavaVirtualMachineBuilder();

        for (auto &jar: jars) {
            builder.addToClasspath(jar);
        }

        for (auto &option: javaOptions) {
            builder.addOption(option);
        }

        builder.setVersion(JNI_VERSION_10);

        auto jvm = builder.buildJavaVirtualMachine();
        JavaVirtualMachineRegistry::set(jvm);
    }
}