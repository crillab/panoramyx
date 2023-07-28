cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    crillab-panoramyx
    VERSION 0.1.0
    DESCRIPTION "Programming pArallel coNstraint sOlveRs mAde aMazingly easY"
    HOMEPAGE_URL "https://github.com/crillab/panoramyx/"
    LANGUAGES CXX
)
set(PROJECT_VENDOR "Falque Thibault" "Wallon Romain")
set(PROJECT_CONTACT "falque@cril.fr" "wallon@cril.fr")


include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)


# ---- Declare library ----
# find MPI

find_package(MPI REQUIRED)
include_directories(${MPI_CXX_HEADER_DIR})
find_package(crillab-autis REQUIRED)
find_package(argparse REQUIRED)
find_package(kahypar REQUIRED)
find_package(loguru REQUIRED)
find_package(crillab-universe REQUIRED)
find_package(crillab-easyjni REQUIRED)
find_package(crillab-except REQUIRED)
find_package(xcsp3-cpp-parser REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(Iconv REQUIRED)
find_package(JNI REQUIRED)

file(GLOB_RECURSE SOURCES
        ${PROJECT_SOURCE_DIR}/source/**.cpp)

file(GLOB_RECURSE HEADERS
        ${PROJECT_SOURCE_DIR}/include/**.hpp
        )

add_library(
    crillab-panoramyx_crillab-panoramyx
    ${SOURCES} ${HEADERS}
)
add_library(crillab-panoramyx::crillab-panoramyx ALIAS crillab-panoramyx_crillab-panoramyx)
target_link_libraries(crillab-panoramyx_crillab-panoramyx crillab-autis::crillab-autis loguru ${MPI_CXX_LIBRARIES})
if(WIN32)
target_compile_options(crillab-panoramyx_crillab-panoramyx PRIVATE /MT)
endif()

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(crillab-panoramyx_crillab-panoramyx PUBLIC CRILLAB_PANORAMYX_STATIC_DEFINE)
endif()

set_target_properties(
    crillab-panoramyx_crillab-panoramyx PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME crillab-panoramyx
    OUTPUT_NAME crillab-panoramyx
)

target_include_directories(
    crillab-panoramyx_crillab-panoramyx ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    crillab-panoramyx_crillab-panoramyx SYSTEM
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(crillab-panoramyx_crillab-panoramyx PUBLIC cxx_std_20)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

add_executable(mainpano example/main.cpp)
target_link_libraries(mainpano crillab-panoramyx_crillab-panoramyx)
if(WIN32)
target_compile_options(mainpano PRIVATE /MT)
endif()
# ---- Developer mode ----

if(NOT crillab-panoramyx_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of crillab-panoramyx"
  )
endif()



include(cmake/dev-mode.cmake)


