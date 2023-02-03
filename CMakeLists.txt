# Configuring CMake.
cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Declaring the project.
project(panoramyx VERSION 0.1.0)
set(PROJECT_VENDOR "Falque Thibault" "Wallon Romain")
set(PROJECT_CONTACT "falque@cril.fr" "wallon@cril.fr")
set(PROJECT_URL "http://github.com/crillab/panoramyx")
set(PROJECT_DESCRIPTION "PANORAMYx: Programming pArallel coNstraint sOlveRs mAde aMazingly easY")


# configure GCC/MSVC/Clang  command

if (NOT MSVC)
    message(STATUS "Not msvc")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wextra ")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused ")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmaybe-uninitialized")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-qual")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Woverloaded-virtual")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wredundant-decls")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winit-self")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
        #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnoexcept")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPARANOID ")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")
    endif ()

    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++1y")
    endif ()

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pthread -g3")

    if (CMAKE_SYSTEM_NAME MATCHES "Darwin" AND CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "arm64")
        # M1 doesn't seem to support -mtune=native -march=native
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g3 ")
    else ()
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -mtune=native -march=native")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -mtune=native -march=native -g3 ")
    endif ()

    # disable warnings for the register keyword which is deprecated but used in the python 2.7 bindings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

elseif (MSVC)
    string(REGEX REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REGEX REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REGEX REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    # Force to always compile with W4
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif ()
    # raise warnings as errors
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")


    # https://msdn.microsoft.com/en-us/library/ms173499.aspx
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")

    # disable all warnings to see whether we can now build in below 1h on appveyor
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w")
    ### disable verbose warnings:
    # warning C4589: Constructor of abstract class '...' ignores initializer for
    # virtual base class '...' (false positive warnings)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4589")
    # warning C4505: 'function' : unreferenced local function has been removed
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4505")
    # warning C4127: conditional expression is constant
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4127")
    # warning C4458: declaration of '...' hides class member
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4458")
    # warning C4459: declaration of '...' hides global declaration
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4459")
    # warning C4702: unreachable code
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4702")
    # warning C4250: ABC inherits XYZ via dominance
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4250")
    # warning C4503: decorated name length exceeded, name was truncated
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4503")
    # disable lots of warnings about "unsecure" C runtime function
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    # disable "The POSIX name for this item is deprecated. Instead, use the ISO C
    # and C++ conformant name.", Nope. We will not.
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    # disable lots of warnings about "unsecure" STL functions
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    # windef.h bizzarly defines min and max as macros, unless this is defined.
    add_definitions(-DNOMINMAX)
endif ()

message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")


# Update and configure submodule dependencies

find_package(Git QUIET)
# if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
#     # Update submodules as needed
#     option(GIT_SUBMODULE "Check submodules during build" ON)
#     if (GIT_SUBMODULE)
#         message(STATUS "Submodule update")
#         execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive --remote
#                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#                 RESULT_VARIABLE GIT_SUBMOD_RESULT)
#         if (NOT GIT_SUBMOD_RESULT EQUAL "0")
#             message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
#         endif ()
#     endif ()
# endif ()

add_subdirectory(libs)

# find MPI
find_package(MPI REQUIRED)
message(STATUS ${MPI_CXX_HEADER_DIR})
include_directories(${MPI_CXX_HEADER_DIR})

# Configuring the use of JNI.
find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

# add a target to generate API documentation with Doxygen
add_subdirectory(doc)
set(LIBRARY_NAME panoramyx)

file(GLOB_RECURSE SOURCES
        ${PROJECT_SOURCE_DIR}/${LIBRARY_NAME}/**.cpp)


file(GLOB_RECURSE HEADERS
        ${PROJECT_SOURCE_DIR}/${LIBRARY_NAME}/**.hpp
        )

# Adding the library to the build targets.

include_directories(libs/loguru)
add_library(${LIBRARY_NAME} ${SOURCES} ${HEADERS} libs/loguru/loguru.cpp libs/loguru/loguru.hpp)
set_target_properties(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${LIBRARY_NAME} autis exception universe easyjni ${MPI_CXX_LIBRARIES} argparse)


add_executable(mainpano panoramyx/src/main.cpp)
target_link_libraries(mainpano ${LIBRARY_NAME})


if (WIN32)
    install(TARGETS ${LIBRARY_NAME}
            RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
else ()
    install(TARGETS ${LIBRARY_NAME}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif ()


