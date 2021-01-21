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

message(STATUS "Including micro-os-plus-rtos...")

# -----------------------------------------------------------------------------

function(target_sources_micro_os_plus_rtos target)

  get_filename_component(xpack_root_folder ${CMAKE_CURRENT_FUNCTION_LIST_DIR} DIRECTORY)

  target_sources(
    ${target}

    PRIVATE
      ${xpack_root_folder}/src/libc/stdlib/malloc.cpp
      ${xpack_root_folder}/src/rtos/internal/os-flags.cpp
      ${xpack_root_folder}/src/rtos/internal/os-lists.cpp
      ${xpack_root_folder}/src/rtos/os-clocks.cpp
      ${xpack_root_folder}/src/rtos/os-condvar.cpp
      ${xpack_root_folder}/src/rtos/os-core.cpp
      ${xpack_root_folder}/src/rtos/os-c-wrapper.cpp
      ${xpack_root_folder}/src/rtos/os-evflags.cpp
      ${xpack_root_folder}/src/rtos/os-idle.cpp
      ${xpack_root_folder}/src/rtos/os-main.cpp
      ${xpack_root_folder}/src/rtos/os-memory.cpp
      ${xpack_root_folder}/src/rtos/os-mempool.cpp
      ${xpack_root_folder}/src/rtos/os-mqueue.cpp
      ${xpack_root_folder}/src/rtos/os-mutex.cpp
      ${xpack_root_folder}/src/rtos/os-semaphore.cpp
      ${xpack_root_folder}/src/rtos/os-terminate.cpp
      ${xpack_root_folder}/src/rtos/os-thread.cpp
      ${xpack_root_folder}/src/rtos/os-timer.cpp
      ${xpack_root_folder}/src/startup/initialise-free-store.cpp
      # ${xpack_root_folder}/src/startup/initialise-interrupts-stack.cpp
  )

endfunction()

# -----------------------------------------------------------------------------

function(target_include_directories_micro_os_plus_rtos target)

  get_filename_component(xpack_root_folder ${CMAKE_CURRENT_FUNCTION_LIST_DIR} DIRECTORY)

  target_include_directories(
    ${target}

    PUBLIC
      ${xpack_root_folder}/include
  )

endfunction()

# -----------------------------------------------------------------------------
