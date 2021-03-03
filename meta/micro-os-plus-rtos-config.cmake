#
# This file is part of the µOS++ distribution.
#   (https://github.com/micro-os-plus)
# Copyright (c) 2021 Liviu Ionescu
#
# This Source Code Form is subject to the terms of the MIT License.
# If a copy of the license was not distributed with this file, it can
# be obtained from https://opensource.org/licenses/MIT/.
#
# -----------------------------------------------------------------------------

# https://cmake.org/cmake/help/v3.18/
# https://cmake.org/cmake/help/v3.18/manual/cmake-packages.7.html#package-configuration-file

if(micro-os-plus-rtos-included)
  return()
endif()

set(micro-os-plus-rtos-included TRUE)

message(STATUS "Processing xPack ${PACKAGE_JSON_NAME}@${PACKAGE_JSON_VERSION}...")

# -----------------------------------------------------------------------------
# Dependencies.

find_package(micro-os-plus-utils-lists REQUIRED)
find_package(micro-os-plus-memory-allocators REQUIRED)
find_package(micro-os-plus-libs-cpp REQUIRED)
find_package(micro-os-plus-version REQUIRED)
# find_package(micro-os-plus-libs-cpp-estd REQUIRED)

# -----------------------------------------------------------------------------
# The current folder.

get_filename_component(xpack_current_folder ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)

# -----------------------------------------------------------------------------

if(NOT TARGET micro-os-plus-rtos-interface)

  add_library(micro-os-plus-rtos-interface INTERFACE EXCLUDE_FROM_ALL)

  # ---------------------------------------------------------------------------

  file(GLOB_RECURSE source_files CONFIGURE_DEPENDS "${xpack_current_folder}/src/*.c*")

  target_sources(
    micro-os-plus-rtos-interface

    INTERFACE
      ${source_files}
  )

  target_include_directories(
    micro-os-plus-rtos-interface

    INTERFACE
      ${xpack_current_folder}/include
  )

  target_link_libraries(
    micro-os-plus-rtos-interface

    INTERFACE
      micro-os-plus::utils-lists-static
      micro-os-plus::memory-allocators
      micro-os-plus::libs-cpp
      # micro-os-plus::libs-cpp-estd
      micro-os-plus::architecture
      micro-os-plus::version
  )

  # ---------------------------------------------------------------------------
  # Aliases.

  add_library(micro-os-plus::rtos ALIAS micro-os-plus-rtos-interface)
  message(STATUS "micro-os-plus::rtos")

endif()

# -----------------------------------------------------------------------------
