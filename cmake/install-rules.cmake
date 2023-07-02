if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/crillab-panoramyx-${PROJECT_VERSION}"
      CACHE PATH ""
  )
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package crillab-panoramyx)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT crillab-panoramyx_Development
)

install(
    TARGETS crillab-panoramyx_crillab-panoramyx
    EXPORT crillab-panoramyxTargets
    RUNTIME #
    COMPONENT crillab-panoramyx_Runtime
    LIBRARY #
    COMPONENT crillab-panoramyx_Runtime
    NAMELINK_COMPONENT crillab-panoramyx_Development
    ARCHIVE #
    COMPONENT crillab-panoramyx_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    crillab-panoramyx_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE PATH "CMake package config location relative to the install prefix"
)
mark_as_advanced(crillab-panoramyx_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${crillab-panoramyx_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT crillab-panoramyx_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${crillab-panoramyx_INSTALL_CMAKEDIR}"
    COMPONENT crillab-panoramyx_Development
)

install(
    EXPORT crillab-panoramyxTargets
    NAMESPACE crillab-panoramyx::
    DESTINATION "${crillab-panoramyx_INSTALL_CMAKEDIR}"
    COMPONENT crillab-panoramyx_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
