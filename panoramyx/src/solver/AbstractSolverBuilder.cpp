/**
* @date 06/02/23
* @file AbstractSolverBuilder.cpp
* @brief 
* @author Thibault Falque
* @author Romain Wallon 
* @license This project is released under the GNU LGPL3 License.
*/

#include "../../include/solver/AbstractSolverBuilder.hpp"
#include "../../../libs/autis/libs/universe/libs/easy-jni/easyjni/JavaVirtualMachineBuilder.h"
#include "../../../libs/autis/libs/universe/libs/easy-jni/easyjni/JavaVirtualMachineRegistry.h"

using namespace easyjni;

void Panoramyx::AbstractSolverBuilder::buildJVM() {
    if (!jars.empty()) {
        JavaVirtualMachineBuilder builder = JavaVirtualMachineBuilder();

        for (auto &j: jars) {
            builder.addToClasspath(j);
        }

        for (auto &j: javaOptions) {
            builder.addOption(j);
        }

        builder.setVersion(JNI_VERSION_10);

        auto jvm = builder.buildJavaVirtualMachine();
        JavaVirtualMachineRegistry::set(jvm);
    }
}
