/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MICRO_OS_PLUS_OS_VERSIONS_H_
#define MICRO_OS_PLUS_OS_VERSIONS_H_

// ----------------------------------------------------------------------------

/**
 * @ingroup cmsis-plus-app-config-info
 * @{
 */

/**
 * @brief Tell the world that µOS++ is in use.
 * @details
 * Macro to inform the application build that µOS++
 * is in use.
 */
 #define OS_USE_MICRO_OS_PLUS

/**
 * @def OS_IS_CROSS_BUILD
 * @brief Tell the world that this is a cross build.
 * @details
 * Currently only macOS and GNU/Linux are identified as native builds,
 * all the other are considered cross builds.
 */

/**
 * @def OS_HAS_STD_THREADS
 * @brief Tell the world that `std::tread` is available.
 * @details
 * For cross builds, standard thread definitions (available in the
 * `os::estd::` namespace) are also aliased or redefined in the `std::`
 * namespace.
 */

#if (! (defined(__APPLE__) || defined(__linux__) || defined(__unix__)))
#define OS_IS_CROSS_BUILD
#if !defined(_GLIBCXX_HAS_GTHREADS)
#define OS_HAS_STD_THREADS
#endif
#endif

/**
 * @}
 */

// ----------------------------------------------------------------------------

#endif /* MICRO_OS_PLUS_OS_VERSIONS_H_ */

// ----------------------------------------------------------------------------
