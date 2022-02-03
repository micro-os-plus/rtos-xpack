/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

#ifndef MICRO_OS_PLUS_RTOS_SCHEDULER_H_
#define MICRO_OS_PLUS_RTOS_SCHEDULER_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include <micro-os-plus/rtos/declarations.h>
#include <micro-os-plus/rtos/clocks.h>

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
       * @brief Variable set to true after the scheduler is started.
       */
      extern bool is_started_;

#if !defined(MICRO_OS_PLUS_USE_RTOS_PORT_SCHEDULER)
      extern bool is_preemptive_;
      extern thread* volatile current_thread_;
      extern internal::ready_threads_list ready_threads_list_;
#endif // !defined(MICRO_OS_PLUS_USE_RTOS_PORT_SCHEDULER)

      extern internal::terminated_threads_list terminated_threads_list_;

      /**
       * @endcond
       */

      /**
       * @brief Initialise the RTOS scheduler.
       * @par Parameters
       *  None.
       * @retval result::ok The scheduler was initialised.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       */
      result_t
      initialize (void);

      /**
       * @brief Start the RTOS scheduler.
       * @par Parameters
       *  None.
       * @par Returns
       *  Nothing.
       */
      [[noreturn]] void
      start (void);

      /**
       * @brief Check if the scheduler was started.
       * @par Parameters
       *  None.
       * @retval true The scheduler was started.
       * @retval false The scheduler was not started.
       */
      bool
      started (void);

      /**
       * @brief Lock the scheduler.
       * @par Parameters
       *  None.
       * @return The previous state of the scheduler lock.
       */
      state_t
      lock (void);

      /**
       * @brief Unlock the scheduler.
       * @par Parameters
       *  None.
       * @return The previous state of the scheduler lock.
       */
      state_t
      unlock (void);

      /**
       * @brief Lock/unlock the scheduler.
       * @param [in] state The new state of the scheduler lock.
       * @return The previous state of the scheduler lock.
       */
      state_t
      locked (state_t state);

      /**
       * @brief Check if the scheduler is locked.
       * @par Parameters
       *  None.
       * @retval true The scheduler is locked.
       * @retval false The scheduler is switching threads (not locked).
       */
      bool
      locked (void);

      /**
       * @brief Check if the scheduler is in preemptive mode.
       * @par Parameters
       *  None
       * @retval true The scheduler is in preemptive mode.
       * @retval false The scheduler is not in preemptive mode.
       */
      bool
      preemptive (void);

      /**
       * @brief Set the scheduler preemptive mode.
       * @param [in] state The new state of the scheduler preemptive mode.
       * @return The previous state of the preemptive mode.
       */
      bool
      preemptive (bool state);

      // ----------------------------------------------------------------------

      /**
       * @cond ignore
       */

      void
      internal_switch_threads (void);

      /**
       * @endcond
       */

      // ======================================================================
      /**
       * @brief Scheduler critical section
       * [RAII](https://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization)
       * helper.
       * @headerfile os.h <micro-os-plus/rtos.h>
       */
      class critical_section
      {
      public:
        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Enter a critical section.
         * @par Parameters
         *  None.
         */
        critical_section ();

        /**
         * @cond ignore
         */

        // The rule of five.
        critical_section (const critical_section&) = delete;
        critical_section (critical_section&&) = delete;
        critical_section&
        operator= (const critical_section&)
            = delete;
        critical_section&
        operator= (critical_section&&)
            = delete;

        /**
         * @endcond
         */

        /**
         * @brief Exit a critical section.
         */
        ~critical_section ();

        /**
         * @}
         */

      protected:
        /**
         * @name Private Member Variables
         * @{
         */

        /**
         * @cond ignore
         */

        /**
         * @brief Variable to store the initial scheduler state.
         */
        const state_t state_;

        /**
         * @endcond
         */

        /**
         * @}
         */
      };

      // ======================================================================

      /**
       * @brief Scheduler uncritical section
       * [RAII](https://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization)
       * helper.
       * @headerfile os.h <micro-os-plus/rtos.h>
       */
      class uncritical_section
      {
      public:
        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Enter a critical section.
         * @par Parameters
         *  None.
         */
        uncritical_section ();

        /**
         * @cond ignore
         */

        // The rule of five.
        uncritical_section (const uncritical_section&) = delete;
        uncritical_section (uncritical_section&&) = delete;
        uncritical_section&
        operator= (const uncritical_section&)
            = delete;
        uncritical_section&
        operator= (uncritical_section&&)
            = delete;

        /**
         * @endcond
         */

        /**
         * @brief Exit a critical section.
         */
        ~uncritical_section ();

        /**
         * @}
         */

      protected:
        /**
         * @name Private Member Variables
         * @{
         */

        /**
         * @cond ignore
         */

        /**
         * @brief Variable to store the initial scheduler state.
         */
        const state_t state_;

        /**
         * @endcond
         */

        /**
         * @}
         */
      };

      // ======================================================================

      /**
       * @brief %Scheduler standard locker.
       * @headerfile os.h <micro-os-plus/rtos.h>
       */
      class lockable
      {
      public:
        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a lockable object instance.
         * @par Parameters
         *  None.
         */
        constexpr lockable ();

        /**
         * @cond ignore
         */

        // The rule of five.
        lockable (const lockable&) = delete;
        lockable (lockable&&) = delete;
        lockable&
        operator= (const lockable&)
            = delete;
        lockable&
        operator= (lockable&&)
            = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the lockable object instance.
         */
        ~lockable ();

        /**
         * @}
         */

      public:
        /**
         * @name Public Member Functions
         * @{
         */

        /**
         * @brief %Lock the scheduler.
         * @par Parameters
         *  None.
         * @par Returns
         *  Nothing.
         */
        void
        lock (void);

        /**
         * @brief Try to lock the scheduler.
         * @par Parameters
         *  None.
         * @retval true The scheduler was locked.
         */
        bool
        try_lock (void);

        /**
         * @brief Unlock the scheduler.
         * @par Parameters
         *  None.
         * @par Returns
         *  Nothing.
         */
        void
        unlock (void);

        /**
         * @}
         */

      protected:
        /**
         * @name Private Member Variables
         * @{
         */

        /**
         * @cond ignore
         */

        /**
         * @brief Variable to store the initial scheduler state.
         */
        state_t state_ = 0;

        /**
         * @endcond
         */

        /**
         * @}
         */
      };

      // ----------------------------------------------------------------------

      /**
       * @brief Scheduler statistics.
       */
      namespace statistics
      {
#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

        /**
         * @brief Get the total number of context switches.
         * @return Integer with the total number of context switches since
         * scheduler start.
         */
        rtos::statistics::counter_t
        context_switches (void);

        /**
         * @cond ignore
         */

        extern rtos::statistics::counter_t context_switches_;

        /**
         * @endcond
         */

#endif // defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

        /**
         * @brief Get the total duration of all threads.
         * @return Integer with the number of CPU cycles, possibly
         * divided by some prescaller.
         */
        rtos::statistics::duration_t
        cpu_cycles (void);

        /**
         * @cond ignore
         */

        extern clock::timestamp_t switch_timestamp_;
        extern rtos::statistics::duration_t cpu_cycles_;

        /**
         * @endcond
         */

#endif // defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

      } // namespace statistics
    } // namespace scheduler

    namespace interrupts
    {
      /**
       * @brief Check if the CPU is in handler mode.
       * @par Parameters
       *  None.
       * @retval true Execution is in an exception handler context.
       * @retval false Execution is in a thread context.
       */
      bool
      in_handler_mode (void);

      // ======================================================================

      // TODO: define all levels of critical sections
      // (kernel, real-time(level), complete)

      // TODO: make template, parameter IRQ level

      /**
       * @brief Interrupts critical section
       * [RAII](https://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization)
       * helper.
       * @headerfile os.h <micro-os-plus/rtos.h>
       */
      class critical_section
      {
      public:
        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Enter an interrupts critical section.
         * @par Parameters
         *  None.
         */
        critical_section ();

        /**
         * @cond ignore
         */

        // The rule of five.
        critical_section (const critical_section&) = delete;
        critical_section (critical_section&&) = delete;
        critical_section&
        operator= (const critical_section&)
            = delete;
        critical_section&
        operator= (critical_section&&)
            = delete;

        /**
         * @endcond
         */

        /**
         * @brief Exit the interrupts critical section.
         */
        ~critical_section ();

        /**
         * @}
         */

      public:
        /**
         * @name Public Member Functions
         * @{
         */

        /**
         * @brief Enter an interrupts critical section.
         * @par Parameters
         *  None.
         * @return The previous value of the interrupts priorities register.
         */
        static state_t
        enter (void);

        /**
         * @brief Exit the interrupts critical section.
         * @param state The value to restore the interrupts priorities
         * register.
         */
        static void
        exit (state_t state);

        /**
         * @}
         */

      protected:
        /**
         * @name Private Member Variables
         * @{
         */

        /**
         * @cond ignore
         */

        /**
         * @brief Variable to store the interrupts priorities register.
         */
        const state_t state_;

        /**
         * @endcond
         */

        /**
         * @}
         */
      };

      // ======================================================================

      /**
       * @brief Interrupts critical section
       * [RAII](https://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization)
       * helper.
       * @headerfile os.h <micro-os-plus/rtos.h>
       */
      class uncritical_section
      {
      public:
        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Enter an interrupts uncritical section.
         * @par Parameters
         *  None.
         */
        uncritical_section ();

        /**
         * @cond ignore
         */

        // The rule of five.
        uncritical_section (const uncritical_section&) = delete;
        uncritical_section (uncritical_section&&) = delete;
        uncritical_section&
        operator= (const uncritical_section&)
            = delete;
        uncritical_section&
        operator= (uncritical_section&&)
            = delete;

        /**
         * @endcond
         */

        /**
         * @brief Exit the interrupts uncritical section.
         */
        ~uncritical_section ();

        /**
         * @}
         */

      public:
        /**
         * @name Public Member Functions
         * @{
         */

        /**
         * @brief Enter interrupts uncritical section.
         * @par Parameters
         *  None.
         * @return The previous value of the interrupts priority register.
         */
        static state_t
        enter (void);

        /**
         * @brief Exit interrupts uncritical section.
         * @param state The value to restore the interrupts priority register.
         * @par Returns
         *  Nothing.
         */
        static void
        exit (state_t state);

        /**
         * @}
         */

      protected:
        /**
         * @name Private Member Variables
         * @{
         */

        /**
         * @cond ignore
         */

        /**
         * @brief Variable to store the interrupts priorities register.
         */
        const state_t state_;

        /**
         * @endcond
         */

        /**
         * @}
         */
      };

      // ======================================================================

      /**
       * @brief Interrupts standard locker.
       * @headerfile os.h <micro-os-plus/rtos.h>
       */
      class lockable
      {
      public:
        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct an interrupts lock.
         * @par Parameters
         *  None.
         */
        constexpr lockable ();

        /**
         * @brief Destruct the interrupts lock.
         */
        ~lockable ();

        /**
         * @cond ignore
         */

        // The rule of five.
        lockable (const lockable&) = delete;
        lockable (lockable&&) = delete;
        lockable&
        operator= (const lockable&)
            = delete;
        lockable&
        operator= (lockable&&)
            = delete;

        /**
         * @endcond
         */

        /**
         * @}
         */

      public:
        /**
         * @name Public Member Functions
         * @{
         */

        /**
         * @brief Lock the interrupts.
         * @par Parameters
         *  None.
         * @par Returns
         *  Nothing.
         */
        void
        lock (void);

        /**
         * @brief Try to lock the interrupts.
         * @par Parameters
         *  None.
         * @retval true The interrupts were locked.
         */
        bool
        try_lock (void);

        /**
         * @brief Unlock the interrupts.
         * @par Parameters
         *  None.
         * @par Returns
         *  Nothing.
         */
        void
        unlock (void);

        /**
         * @}
         */

      protected:
        /**
         * @name Private Member Variables
         * @{
         */

        /**
         * @cond ignore
         */

        /**
         * @brief Variable to store the interrupts priorities register.
         */
        state_t state_;

        /**
         * @endcond
         */

        /**
         * @}
         */
      };

    } // namespace interrupts
  } // namespace rtos
} // namespace micro_os_plus

// ===== Inline & template implementations ====================================

namespace micro_os_plus
{
  namespace rtos
  {
    namespace scheduler
    {
      /**
       * @details
       * Check if the scheduler was started, i.e. if scheduler::start()
       * was called.
       *
       * @note Can be invoked from Interrupt Service Routines.
       */
      inline bool
      started (void)
      {
        return is_started_;
      }

      /**
       * @details
       * Check if the scheduler preemption is enabled.
       *
       * @note Can be invoked from Interrupt Service Routines.
       */
      inline bool
      preemptive (void)
      {
#if !defined(MICRO_OS_PLUS_USE_RTOS_PORT_SCHEDULER)
        return is_preemptive_;
#else
        return port::scheduler::preemptive ();
#endif
      }

      /**
       * @details
       * Check if the scheduler is locked on the current thread or
       * is switching threads from the ready list.
       *
       * @note Can be invoked from Interrupt Service Routines.
       */
      inline bool
      locked (void)
      {
        return port::scheduler::locked ();
      }

      /**
       * @details
       * Set the scheduler lock state to locked and
       * return the previous state.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline state_t
      lock (void)
      {
        return port::scheduler::lock ();
      }

      /**
       * @details
       * Set the scheduler lock state to unlocked and
       * return the previous state.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline state_t
      unlock (void)
      {
        return port::scheduler::unlock ();
      }

      /**
       * @details
       * Set the scheduler lock state based on the parameter and
       * return the previous state.
       *
       * This allows to implement scheduler critical sections, where
       * the scheduler is disabled and context switches are not
       * performed.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline state_t
      locked (state_t state)
      {
        return port::scheduler::locked (state);
      }

      /**
       * @details
       * Lock the scheduler and remember the initial scheduler state.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline critical_section::critical_section () : state_ (lock ())
      {
#if defined(MICRO_OS_PLUS_TRACE_RTOS_SCHEDULER)
        trace::printf (" {c ");
#endif
      }

      /**
       * @details
       * Restore the initial scheduler state and possibly unlock
       * the scheduler.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline critical_section::~critical_section ()
      {
#if defined(MICRO_OS_PLUS_TRACE_RTOS_SCHEDULER)
        trace::printf (" c} ");
#endif
        locked (state_);
      }

      /**
       * @details
       * Lock the scheduler and remember the initial scheduler state.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline uncritical_section::uncritical_section () : state_ (unlock ())
      {
#if defined(MICRO_OS_PLUS_TRACE_RTOS_SCHEDULER)
        trace::printf (" {u ");
#endif
      }

      /**
       * @details
       * Restore the initial scheduler state and possibly unlock
       * the scheduler.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline uncritical_section::~uncritical_section ()
      {
#if defined(MICRO_OS_PLUS_TRACE_RTOS_SCHEDULER)
        trace::printf (" u} ");
#endif
        locked (state_);
      }

      /**
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      constexpr lockable::lockable () : state_ (port::scheduler::state::init)
      {
        ;
      }

      /**
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline lockable::~lockable ()
      {
        ;
      }

      /**
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline void
      lockable::lock (void)
      {
        state_ = scheduler::lock ();
      }

      /**
       * @details
       * Somehow redundant, since the lock will always succeed;
       * but used to meet the lockableable requirements.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline bool
      lockable::try_lock (void)
      {
        state_ = scheduler::lock ();
        return true;
      }

      /**
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline void
      lockable::unlock (void)
      {
        scheduler::locked (state_);
      }

      namespace statistics
      {
#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

        /**
         * @details
         * Each time the scheduler performs a context switch, it increments
         * the thread counter and the scheduler total counter.
         *
         * This value can be used together with the corresponding
         * thread function, to compute percentages.
         *
         * @note This function is available only when
         * @ref MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES
         * is defined.
         *
         * @warning Cannot be invoked from Interrupt Service Routines.
         */
        inline rtos::statistics::counter_t
        context_switches (void)
        {
          return context_switches_;
        }

#endif // defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

        /**
         * @details
         * For Cortex-M ports, this value is usually derived from
         * the clock that drives the SysTick, which most of the times
         * is the system clock.
         *
         * This value can be used together with the corresponding
         * thread function, to compute percentages.
         *
         * @note This function is available only when
         * @ref MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES
         * is defined.
         *
         * @warning Cannot be invoked from Interrupt Service Routines.
         */
        inline rtos::statistics::duration_t
        cpu_cycles (void)
        {
          return cpu_cycles_;
        }

#endif // defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

      } // namespace statistics

    } // namespace scheduler

    // ========================================================================

    namespace interrupts
    {
      /**
       * @note Can be invoked from Interrupt Service Routines (obviously).
       */
      inline __attribute__ ((always_inline)) bool
      in_handler_mode (void)
      {
        return port::interrupts::in_handler_mode ();
      }

      /**
       * @note Can be invoked from Interrupt Service Routines.
       */
      inline __attribute__ ((always_inline))
      critical_section::critical_section ()
          : state_ (enter ())
      {
        ;
      }

      /**
       * @note Can be invoked from Interrupt Service Routines.
       */
      inline __attribute__ ((always_inline))
      critical_section::~critical_section ()
      {
        exit (state_);
      }

      /**
       * @note Can be invoked from Interrupt Service Routines.
       */
      inline __attribute__ ((always_inline)) state_t
      critical_section::enter (void)
      {
        return port::interrupts::critical_section::enter ();
      }

      /**
       * @note Can be invoked from Interrupt Service Routines.
       */
      inline __attribute__ ((always_inline)) void
      critical_section::exit (state_t state)
      {
        port::interrupts::critical_section::exit (state);
      }

      // ======================================================================

      /**
       * @note Can be invoked from Interrupt Service Routines.
       */
      inline __attribute__ ((always_inline))
      uncritical_section::uncritical_section ()
          : state_ (enter ())
      {
        ;
      }

      /**
       * @note Can be invoked from Interrupt Service Routines.
       */
      inline __attribute__ ((always_inline))
      uncritical_section::~uncritical_section ()
      {
        exit (state_);
      }

      /**
       * @note Can be invoked from Interrupt Service Routines.
       */
      inline __attribute__ ((always_inline)) state_t
      uncritical_section::enter (void)
      {
        return port::interrupts::uncritical_section::enter ();
      }

      /**
       * @note Can be invoked from Interrupt Service Routines.
       */
      inline __attribute__ ((always_inline)) void
      uncritical_section::exit (state_t state)
      {
        port::interrupts::uncritical_section::exit (state);
      }

      // ======================================================================

      /**
       * @note Can be invoked from Interrupt Service Routines.
       */
      constexpr lockable::lockable () : state_ (port::interrupts::state::init)
      {
        ;
      }

      /**
       * @note Can be invoked from Interrupt Service Routines.
       */
      inline __attribute__ ((always_inline)) lockable::~lockable ()
      {
        ;
      }

      /**
       * @note Can be invoked from Interrupt Service Routines.
       */
      inline __attribute__ ((always_inline)) void
      lockable::lock (void)
      {
        state_ = critical_section::enter ();
      }

      /**
       * @details
       * Somehow redundant, since the lock will always succeed;
       * but used to meet the Lockable requirements.
       *
       * @note Can be invoked from Interrupt Service Routines.
       */
      inline __attribute__ ((always_inline)) bool
      lockable::try_lock (void)
      {
        state_ = critical_section::enter ();
        return true;
      }

      /**
       * @note Can be invoked from Interrupt Service Routines.
       */
      inline __attribute__ ((always_inline)) void
      lockable::unlock (void)
      {
        critical_section::exit (state_);
      }

      // ======================================================================
    } // namespace interrupts

  } // namespace rtos
} // namespace micro_os_plus

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif // __cplusplus

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_RTOS_SCHEDULER_H_

// ----------------------------------------------------------------------------
