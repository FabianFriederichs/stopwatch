cmake_minimum_required(VERSION 3.23)
project(stopwatch LANGUAGES CXX VERSION 0.1.0)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
# main target
add_library(stopwatch INTERFACE)
# create header file set
target_sources(stopwatch
    INTERFACE
        FILE_SET public_headers
        TYPE HEADERS
        BASE_DIRS 
            "${CMAKE_CURRENT_SOURCE_DIR}/include"
        FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/include/stopwatch/stopwatch.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/stopwatch/common.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/stopwatch/cpu_clock.hpp"
)
# install interface headers
target_include_directories(stopwatch
    INTERFACE
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
# compile settings
target_compile_features(stopwatch
    INTERFACE
        cxx_std_17
)
# export target
install(TARGETS stopwatch
    EXPORT stopwatchTargets
    FILE_SET public_headers
)
# install export
install(EXPORT stopwatchTargets
    FILE stopwatchTargets.cmake
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/stopwatch"
    NAMESPACE stopwatch::
)
# config file
set(package_file_config_template_string "
@PACKAGE_INIT@
include(\"\${CMAKE_CURRENT_LIST_DIR}/@PROJECT_NAME@Targets.cmake\")
")
# write config file template to build directory
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/stopwatchConfig.cmake.in" "${package_file_config_template_string}")
# configure config file
configure_package_config_file(
    "${CMAKE_CURRENT_BINARY_DIR}/stopwatchConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/stopwatchConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/stopwatch"
    PATH_VARS
        CMAKE_INSTALL_INCLUDEDIR
        CMAKE_INSTALL_LIBDIR
)
# package version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/stopwatchConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT
)
# install config files
install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/stopwatchConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/stopwatchConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/stopwatch"
)
# packaging
set(PACKAGE_NAME "${PROJECT_NAME}")
set(PACKAGE_VENDOR "TU Braunschweig")
set(PACKAGE_AUTHOR "Fabian Friederichs")
# enable packaging generators based on platform
if(WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
    # add environment variable to NSIS installer
    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "WriteRegStr HKLM 'SYSTEM\\\\CurrentControlSet\\\\Control\\\\Session Manager\\\\Environment' 'stopwatch_DIR' '\$INSTDIR'")
    set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "DeleteRegValue HKLM 'SYSTEM\\\\CurrentControlSet\\\\Control\\\\Session Manager\\\\Environment' 'stopwatch_DIR'")
elseif(UNIX)
    set(CPACK_GENERATOR "TGZ;DEB;RPM")
endif()
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/CPackOptions.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/CPackOptions.cmake"
    @ONLY
)
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/package")
set(CPACK_PROJECT_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/CPackOptions.cmake")
include(CPack)
