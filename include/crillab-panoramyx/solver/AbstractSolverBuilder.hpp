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

#ifndef PANORAMYX_ABSTRACTBUILDERSOLVER_HPP
#define PANORAMYX_ABSTRACTBUILDERSOLVER_HPP

#include <vector>
#include <string>
#include "../network/INetworkCommunication.hpp"
#include "../core/Configuration.hpp"
#include "AbstractParallelSolver.hpp"

namespace Panoramyx{


class AbstractSolverBuilder{
protected:
    INetworkCommunication* networkCommunication;
    std::vector<std::string> jars;
    std::vector<std::string> javaOptions;

    void buildJVM();

public:
    AbstractSolverBuilder*withJars(const std::vector<std::string> &jars){
        this->jars.insert(this->jars.end(),jars.begin(),jars.end());
        return this;
    }
    AbstractSolverBuilder*withJavaOption(const std::vector<std::string> &javaOptions){
        this->javaOptions.insert(this->javaOptions.end(),javaOptions.begin(),javaOptions.end());
        return this;
    }
    AbstractSolverBuilder*withNetworkCommunicator(INetworkCommunication* networkCommunication) {
        this->networkCommunication=networkCommunication;
        return this;
    }



    virtual AbstractParallelSolver* build()=0;
};

}
#endif