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

#ifndef MICRO_OS_PLUS_RTOS_H_
#define MICRO_OS_PLUS_RTOS_H_

// ----------------------------------------------------------------------------

/*
 * The initial µOS++ RTOS API was inspired by CMSIS RTOS API v1.x,
 * Copyright (c) 2013 ARM LIMITED.
 */

/**
 * @file os.h
 * @brief Single file µOS++ RTOS definitions.
 * @details
 * The code was originally inspired by ARM CMSIS `<cmsis_os.h>` file, v1.02,
 * and tries to remain functionally close to the CMSIS RTOS specifications.
 *
 * References are to C++ Standard ISO/IEC 14882:2011(E)
 * Third edition (2011-09-01).
 *
 * Major improvements:
 * - no more macros required to define objects
 * - allow static memory allocations for all objects
 * - very close to POSIX ([IEEE Std 1003.1, 2013
 * Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html))
 * - specifically designed to facilitate the implementation of
 *   C++ standard thread library (ISO/IEC 14882:2011)
 * - standard POSIX errors definitions used
 * - improved usability, by defining both simple (using defaults)
 *   and complex (using attributes) object constructors (feature
 *   inspired by POSIX threads attributes)
 * - improved readability with explicit three-fold waiting functions
 *   (for example: lock(), try-lock(), timed-lock(), similar to POSIX threads)
 * - POSIX condition variable added
 * - versatile clocks added (Systick_clock, Realtime_clock)
 * - all object can wait using either Systick or Realtime_clock
 *
 * Differences from POSIX:
 * - timeouts are relative, while in POSIX are absolute. Rationale:
 * although internally it may be easier to compare absolute dates,
 * it looked more convenient for the user to enter short durations
 * instead of very large absolute times.
 */

/*
 * TODO:
 * - make Thread virtual, to allow create_hook/delete_hook functionality
 * - make most classes virtual, to allow post_hook functionality
 * - event timestamps: add derived classes that capture the event timestamp
 * - add object type in base class
 * - add Wait_list in base class
 *
 * Notes:
 * - the try_wait(), try_flags_wait() names are probably not very inspired,
 * but at least they are consistent in POSIX.
 */

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include <micro-os-plus/rtos/declarations.h>
#include <micro-os-plus/diag/trace.h>
#include <micro-os-plus/rtos/scheduler.h>

// Includes a reference to critical sections.
#include <micro-os-plus/rtos/memory.h>

#include <micro-os-plus/rtos/thread.h>
#include <micro-os-plus/rtos/clocks.h>
#include <micro-os-plus/rtos/timer.h>
#include <micro-os-plus/rtos/mutex.h>
#include <micro-os-plus/rtos/condition-variable.h>
#include <micro-os-plus/rtos/semaphore.h>
#include <micro-os-plus/rtos/memory-pool.h>
#include <micro-os-plus/rtos/message-queue.h>
#include <micro-os-plus/rtos/event-flags.h>

#include <micro-os-plus/rtos/hooks.h>
#if (!(defined(__APPLE__) || defined(__linux__) || defined(__unix__)))
#include <micro-os-plus/startup/hooks.h>
#endif // Unix

// More or less at the end, when all other definitions are available.
#include <micro-os-plus/rtos/inlines.h>
#include <micro-os-plus/rtos/port/inlines.h>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

namespace micro_os_plus
{
  namespace rtos
  {
    namespace scheduler
    {
      /**
       * @cond ignore
       */

      /**
       * @brief Head of the list with all top level and detached threads.
       */
      extern thread::threads_list top_threads_list_;

      /**
       * @endcond
       */

      /**
       * @brief Get the children threads.
       * @param [in] th Pointer to thread or nullptr.
       * @return Reference to children list.
       */
      thread::threads_list&
      children_threads (thread* th);

    } // namespace scheduler

    // ------------------------------------------------------------------------

    namespace interrupts
    {
#if defined(MICRO_OS_PLUS_HAS_INTERRUPTS_STACK) || defined(__DOXYGEN__)

      /**
       * @brief Get the interrupts stack
       * @ingroup micro-os-plus-rtos-core
       * @par Parameters
       *  None.
       * @return Pointer to stack object instance.
       */
      class thread::stack*
      stack (void);

#endif // defined(MICRO_OS_PLUS_HAS_INTERRUPTS_STACK)
    } // namespace interrupts

    // ------------------------------------------------------------------------
    /**
     * @brief  Create an object that is owned by a `shared_ptr` and is
     *  allocated using the RTOS system allocator.
     * @ingroup micro-os-plus-rtos-memres
     * @param  arguments  Arguments for the _T_ object's constructor.
     * @return A shared_ptr that owns the newly created object.
     * @throw * An exception may be thrown from `allocate()` or
     *          from the constructor of _T_.
     */
    template <typename T, typename... Args>
    inline typename std::enable_if<!std::is_array<T>::value,
                                   std::shared_ptr<T>>::type
    make_shared (Args&&... arguments)
    {
      // -Wno-psabi to disble the ABI warning.
      typedef typename std::remove_const<T>::type T_nc;
      return std::allocate_shared<T> (memory::allocator<T_nc> (),
                                      std::forward<Args> (arguments)...);
    }

  } // namespace rtos
} // namespace micro_os_plus

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif // __cplusplus

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_RTOS_H_

// ----------------------------------------------------------------------------
