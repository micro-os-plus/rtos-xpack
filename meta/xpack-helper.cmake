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

function(target_sources_micro_os_plus_rtos target)

  get_filename_component(PARENT_DIR ${CMAKE_CURRENT_FUNCTION_LIST_DIR} DIRECTORY)

  target_sources(
    ${target}

    PRIVATE
      ${PARENT_DIR}/src/libc/stdlib/malloc.cpp
      ${PARENT_DIR}/src/rtos/internal/os-flags.cpp
      ${PARENT_DIR}/src/rtos/internal/os-lists.cpp
      ${PARENT_DIR}/src/rtos/os-clocks.cpp
      ${PARENT_DIR}/src/rtos/os-condvar.cpp
      ${PARENT_DIR}/src/rtos/os-core.cpp
      ${PARENT_DIR}/src/rtos/os-c-wrapper.cpp
      ${PARENT_DIR}/src/rtos/os-evflags.cpp
      ${PARENT_DIR}/src/rtos/os-idle.cpp
      ${PARENT_DIR}/src/rtos/os-main.cpp
      ${PARENT_DIR}/src/rtos/os-memory.cpp
      ${PARENT_DIR}/src/rtos/os-mempool.cpp
      ${PARENT_DIR}/src/rtos/os-mqueue.cpp
      ${PARENT_DIR}/src/rtos/os-mutex.cpp
      ${PARENT_DIR}/src/rtos/os-semaphore.cpp
      ${PARENT_DIR}/src/rtos/os-terminate.cpp
      ${PARENT_DIR}/src/rtos/os-thread.cpp
      ${PARENT_DIR}/src/rtos/os-timer.cpp
      ${PARENT_DIR}/src/startup/initialise-free-store.cpp
      ${PARENT_DIR}/src/startup/initialise-interrupts-stack.cpp
    )
endfunction()

function(target_include_directories_micro_os_plus_rtos target)

  get_filename_component(PARENT_DIR ${CMAKE_CURRENT_FUNCTION_LIST_DIR} DIRECTORY)

  target_include_directories(
    ${target}

    PUBLIC
      ${PARENT_DIR}/include
  )
endfunction()

# -----------------------------------------------------------------------------
