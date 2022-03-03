/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus/)
 * Copyright (c) 2016 Liviu Ionescu.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

#ifndef MICRO_OS_PLUS_RTOS_TYPES_H_
#define MICRO_OS_PLUS_RTOS_TYPES_H_

// ----------------------------------------------------------------------------
// This file is included at the end of declarations.h.

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_INCLUDE_CONFIG_H)
#include <micro-os-plus/config.h>
#endif // MICRO_OS_PLUS_INCLUDE_CONFIG_H

// ----------------------------------------------------------------------------

#include <memory>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

namespace micro_os_plus
{
  namespace rtos
  {
    namespace memory
    {

#if !defined(MICRO_OS_PLUS_INCLUDE_RTOS_CUSTOM_ALLOCATOR) \
    || defined(__DOXYGEN__)

      /**
       * @brief Type of allocator used by the system objects.
       *  Must be stateless.
       * @ingroup micro-os-plus-rtos-memres
       * @tparam T Type of elements to be allocated.
       */
      template <typename T>
      using allocator = allocator_stateless_default_resource<T>;

#endif

    } // namespace memory
  } // namespace rtos
} // namespace micro_os_plus

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_CUSTOM_TYPES)
#include <micro-os-plus/rtos/custom-types.h>
#endif

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif // __cplusplus

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_RTOS_TYPES_H_

// ----------------------------------------------------------------------------
