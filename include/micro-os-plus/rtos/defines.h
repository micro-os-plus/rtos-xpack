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

#ifndef MICRO_OS_PLUS_DEFINES_H_
#define MICRO_OS_PLUS_DEFINES_H_

// ----------------------------------------------------------------------------

/**
 * @ingroup micro-os-plus-app-config-info
 * @{
 */

/**
 * @brief Tell the world that µOS++ is in use.
 * @details
 * Macro to inform the application build that µOS++
 * is in use.
 */
#define MICRO_OS_PLUS_USE_MICRO_OS_PLUS

/**
 * @def MICRO_OS_PLUS_IS_CROSS_BUILD
 * @brief Tell the world that this is a cross build.
 * @details
 * Currently only macOS and GNU/Linux are identified as native builds,
 * all the other are considered cross builds.
 */

/**
 * @def MICRO_OS_PLUS_HAS_STD_THREADS
 * @brief Tell the world that `std::tread` is available.
 * @details
 * For cross builds, standard thread definitions (available in the
 * `micro_os_plus::estd::` namespace) are also aliased or redefined in the
 * `std::` namespace.
 */

#if (!(defined(__APPLE__) || defined(__linux__) || defined(__unix__)))
#define MICRO_OS_PLUS_IS_CROSS_BUILD
#if !defined(_GLIBCXX_HAS_GTHREADS)
#define MICRO_OS_PLUS_HAS_STD_THREADS
#endif
#endif

/**
 * @}
 */

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_DEFINES_H_

// ----------------------------------------------------------------------------
