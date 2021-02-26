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

message(STATUS "Including micro-os-plus-rtos...")

# -----------------------------------------------------------------------------
# Dependencies.

find_package(micro-os-plus-utils-lists REQUIRED)
find_package(micro-os-plus-memory-allocators REQUIRED)
find_package(micro-os-plus-libs-cpp REQUIRED)
# find_package(micro-os-plus-libs-cpp-estd REQUIRED)

# -----------------------------------------------------------------------------
# The current folder.

get_filename_component(xpack_current_folder ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)

# -----------------------------------------------------------------------------

if(NOT TARGET micro-os-plus-rtos-interface)

  add_library(micro-os-plus-rtos-interface INTERFACE EXCLUDE_FROM_ALL)

  # ---------------------------------------------------------------------------

  target_sources(
    micro-os-plus-rtos-interface

    INTERFACE
      ${xpack_current_folder}/src/libc/stdlib/malloc.cpp
      ${xpack_current_folder}/src/rtos/internal/os-flags.cpp
      ${xpack_current_folder}/src/rtos/internal/os-lists.cpp
      ${xpack_current_folder}/src/rtos/os-clocks.cpp
      ${xpack_current_folder}/src/rtos/os-condvar.cpp
      ${xpack_current_folder}/src/rtos/os-core.cpp
      ${xpack_current_folder}/src/rtos/os-c-wrapper.cpp
      ${xpack_current_folder}/src/rtos/os-evflags.cpp
      ${xpack_current_folder}/src/rtos/os-idle.cpp
      ${xpack_current_folder}/src/rtos/os-main.cpp
      ${xpack_current_folder}/src/rtos/os-memory.cpp
      ${xpack_current_folder}/src/rtos/os-mempool.cpp
      ${xpack_current_folder}/src/rtos/os-mqueue.cpp
      ${xpack_current_folder}/src/rtos/os-mutex.cpp
      ${xpack_current_folder}/src/rtos/os-semaphore.cpp
      ${xpack_current_folder}/src/rtos/os-terminate.cpp
      ${xpack_current_folder}/src/rtos/os-thread.cpp
      ${xpack_current_folder}/src/rtos/os-timer.cpp
      ${xpack_current_folder}/src/startup/initialise-free-store.cpp
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
  )

  # ---------------------------------------------------------------------------
  # Aliases.

  add_library(micro-os-plus::rtos ALIAS micro-os-plus-rtos-interface)
  message(STATUS "micro-os-plus::rtos")

endif()

# -----------------------------------------------------------------------------
