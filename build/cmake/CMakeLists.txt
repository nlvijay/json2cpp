#
#  Copyright (C) 2017 National Institute For Space Research (INPE) - Brazil.
#
#  This file is part of Earth Observation Web Services (JSON2CPP).
#
#  JSON2CPP is free software: you can
#  redistribute it and/or modify it under the terms of the
#  GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License,
#  or (at your option) any later version.
#
#  JSON2CPP is distributed in the hope that
#  it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with JSON2CPP. See LICENSE. If not, write to
#  e-sensing team at <esensing-team@dpi.inpe.br>.
#
#  Description: Main CMakeLists.txt of JSON2CPP build system.
#
#  Author: Gilberto Ribeiro de Queiroz
#

#
# Warning: JSON2CPP requires at least CMake 2.8
#
cmake_minimum_required(VERSION 2.8)

#
# Project codename: json2cpp
#
project(json2cpp)

#
# Variables for the system versioning
#
set(JSON2CPP_VERSION_MAJOR 0)
set(JSON2CPP_VERSION_MINOR 1)
set(JSON2CPP_VERSION_PATCH 0)
set(JSON2CPP_VERSION_STATUS "alpha1")
set(JSON2CPP_VERSION_NUMBER "0x000100")

if(JSON2CPP_VERSION_STATUS)
    set(JSON2CPP_VERSION_STRING "${JSON2CPP_VERSION_MAJOR}.${JSON2CPP_VERSION_MINOR}.${JSON2CPP_VERSION_PATCH}-${JSON2CPP_VERSION_STATUS}")
else()
    set(JSON2CPP_VERSION_STRING "${JSON2CPP_VERSION_MAJOR}.${JSON2CPP_VERSION_MINOR}.${JSON2CPP_VERSION_PATCH}-${JSON2CPP_VERSION_STATUS}")
endif()

if(JSON2CPP_BUILD_NUMBER)
    set(JSON2CPP_VERSION_STRING "${JSON2CPP_VERSION_STRING}-build-${JSON2CPP_BUILD_NUMBER}")
endif()

message(STATUS "JSON2CPP: build version = '${JSON2CPP_VERSION_STRING}'")

#
# Find the JSON2CPP absolute paths
#
get_filename_component(JSON2CPP_ABSOLUTE_ROOT_DIR "../../" ABSOLUTE)

message(STATUS "JSON2CPP: codebase path in: '${JSON2CPP_ABSOLUTE_ROOT_DIR}'")

#
# Common definitions due to the Boost Libraries
#
add_definitions(-DBOOST_ALL_NO_LIB -DBOOST_FILESYSTEM_VERSION=3 -DBOOST_UBLAS_TYPE_CHECK=0 -DBOOST_LOG_DYN_LINK)

#
# Set maximum instantiation depth for template classes to 32
#
if(APPLE)
    add_definitions(-ftemplate-depth-256)
endif()

#
# Compiler options: warning and C++ 11
#
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER MATCHES ".*clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")
endif()

#
# Add source tree directory to search for CMake modules
#
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules")

# Build tree directory will be needed in all projects.
include_directories(${PROJECT_BINARY_DIR})

message(STATUS "JSON2CPP: adding general include directory: '${PROJECT_BINARY_DIR}'")

#
# Set output directory for libraries and executables.
#
if(CMAKE_GENERATOR STREQUAL Xcode)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    message(STATUS "JSON2CPP: setting output directory to: '${CMAKE_BINARY_DIR}'")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    message(STATUS "JSON2CPP: setting output directory to: '${CMAKE_BINARY_DIR}/bin'")
endif()

#
# Enable CTest
#
enable_testing()
message(STATUS "JSON2CPP: CTest enabled!")

#
# Include CMake's support for a macro that provides an option dependent on other options.
#
include(CMakeDependentOption)

#
# Search for 3rd-party libraries.
#

find_package(RapidJSON REQUIRED)

if(RapidJSON_FOUND)
    message(STATUS "JSON2CPP: RapidJSON Library found!")
    include_directories(${RAPIDJSON_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "JSON2CPP: could not find required RapidJSON Library. Please, refer to the JSON2CPP build documentation!")
endif()

#
# Define installation directories
#
set(JSON2CPP_INSTALL_LIB_DIR lib)
set(JSON2CPP_INSTALL_BIN_DIR bin)
set(JSON2CPP_INSTALL_INCLUDE_DIR include)
set(JSON2CPP_INSTALL_SHARE_DIR share)

if(WIN32 AND NOT CYGWIN)
  set(JSON2CPP_INSTALL_CMAKE_DIR cmake)
else()
  set(JSON2CPP_INSTALL_CMAKE_DIR lib/cmake/json2cpp-${JSON2CPP_VERSION_STRING})
endif()

#
# Make relative paths absolute: needed to redistribute CMake files
#
foreach(p LIB BIN INCLUDE SHARE CMAKE)
    set(var JSON2CPP_INSTALL_${p}_DIR)

    if(NOT IS_ABSOLUTE "${${var}}")
        set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
    endif()

    #message(STATUS "JSON2CPP: var = ${${var}}")
endforeach()

#
# Configuration of general source code files that must be installed
# if devel component is installed.
#
#configure_file(${JSON2CPP_ABSOLUTE_ROOT_DIR}/src/json2cpp/build_config.hpp.in
#               ${PROJECT_BINARY_DIR}/build_config.hpp @ONLY)

#configure_file(${JSON2CPP_ABSOLUTE_ROOT_DIR}/src/json2cpp/version.hpp.in
#               ${PROJECT_BINARY_DIR}/json2cpp_version.hpp @ONLY)

#install(FILES ${PROJECT_BINARY_DIR}/build_config.hpp
#              ${PROJECT_BINARY_DIR}/json2cpp_version.hpp
#        DESTINATION "${JSON2CPP_INSTALL_INCLUDE_DIR}"
#        COMPONENT devel)

#
# Modules to build
#
file(GLOB JSON2CPP_SRC_FILES ${JSON2CPP_ABSOLUTE_ROOT_DIR}/src/json2cpp/*.cpp)
file(GLOB JSON2CPP_HDR_FILES ${JSON2CPP_ABSOLUTE_ROOT_DIR}/src/json2cpp/*.hpp)

source_group("Source Files"  FILES ${JSON2CPP_SRC_FILES})
source_group("Header Files"  FILES ${JSON2CPP_HDR_FILES})

add_executable(json2cpp ${JSON2CPP_SRC_FILES} ${JSON2CPP_HDR_FILES})

#install(TARGETS json2cpp
#        EXPORT json2cpp-targets
#        RUNTIME DESTINATION ${JSON2CPP_INSTALL_BIN_DIR} COMPONENT runtime
#        LIBRARY DESTINATION ${JSON2CPP_INSTALL_LIB_DIR} COMPONENT runtime
#        ARCHIVE DESTINATION ${JSON2CPP_INSTALL_LIB_DIR} COMPONENT devel)

#install(FILES ${JSON2CPP_HDR_FILES}
#        DESTINATION "${JSON2CPP_INSTALL_INCLUDE_DIR}/app_server"
#        COMPONENT devel)

#
# Export all targets information to the build-tree.
# Tip:
#   * Another way would be the following command: export(TARGETS ${JSON2CPP_EXPORT_TARGETS} FILE "${PROJECT_BINARY_DIR}/json2cpp-targets.cmake")
#   * In this case we would need a list such as: set(JSON2CPP_EXPORT_TARGETS "")
#                                                list(APPEND JSON2CPP_EXPORT_TARGETS "json2cpp_core")
#
#export(EXPORT json2cpp-targets FILE "${PROJECT_BINARY_DIR}/json2cpp-targets.cmake")

#
# Register JSON2CPP' current build directory in the CMake user 
# package registry. This way the find_package command may consider
# this build directory while searching for the package json2cpp.
#
#export(PACKAGE json2cpp)

#
# find_package(json2cpp) command will rely on this archive in order to
# find out JSON2CPP version.
#
#configure_file(${PROJECT_SOURCE_DIR}/json2cpp-config-version.cmake.in
#               ${PROJECT_BINARY_DIR}/json2cpp-config-version.cmake @ONLY)

#
# find_package(json2cpp) will rely on this file when cross compiling
# in the build tree. Note that JSON2CPP_CONF_INCLUDE_DIRS will point
# to codebase and build-tree.
#
#set(JSON2CPP_CONF_INCLUDE_DIRS "${JSON2CPP_ABSOLUTE_ROOT_DIR}/src" "${PROJECT_BINARY_DIR}")
#message(STATUS "JSON2CPP: JSON2CPP_CONF_INCLUDE_DIRS = ${JSON2CPP_CONF_INCLUDE_DIRS}")

#configure_file(${PROJECT_SOURCE_DIR}/json2cpp-config.cmake.in
#               ${PROJECT_BINARY_DIR}/json2cpp-config.cmake @ONLY)

#
# find_package(json2cpp) will rely on this file when cross compiling
# in the install tree.
#
#file(RELATIVE_PATH JSON2CPP_REL_INCLUDE_DIR "${JSON2CPP_INSTALL_CMAKE_DIR}" "${JSON2CPP_INSTALL_INCLUDE_DIR}")

#message(STATUS "JSON2CPP: JSON2CPP_REL_INCLUDE_DIR = ${JSON2CPP_REL_INCLUDE_DIR}")

#set(JSON2CPP_CONF_INCLUDE_DIRS "\${JSON2CPP_CMAKE_DIR}/${JSON2CPP_REL_INCLUDE_DIR}")
#message(STATUS "JSON2CPP: JSON2CPP_CONF_INCLUDE_DIRS = ${JSON2CPP_CONF_INCLUDE_DIRS}")

#configure_file(${PROJECT_SOURCE_DIR}/json2cpp-config.cmake.in
#               ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/json2cpp-config.cmake @ONLY)

#
# Install CMake support files
#
#install(FILES ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/json2cpp-config.cmake
#              ${PROJECT_BINARY_DIR}/json2cpp-config-version.cmake
#        DESTINATION "${JSON2CPP_INSTALL_CMAKE_DIR}"
#        COMPONENT devel)

#
# Export all targets information to the install-tree
#
#install(EXPORT json2cpp-targets
#        DESTINATION "${JSON2CPP_INSTALL_CMAKE_DIR}"
#        FILE json2cpp-targets.cmake
#        COMPONENT devel)

