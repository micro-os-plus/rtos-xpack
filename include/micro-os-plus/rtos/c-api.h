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

/*
 * The code was originally inspired by ARM CMSIS `cmsis_os.h` file, v1.02,
 * and tries to remain functionally close to the CMSIS RTOS specifications.
 */

#ifndef MICRO_OS_PLUS_RTOS_C_API_H_
#define MICRO_OS_PLUS_RTOS_C_API_H_

// ----------------------------------------------------------------------------

// Include the µOS++ C API structures declarations.
#include <micro-os-plus/rtos/declarations-c.h>

#include <micro-os-plus/rtos/hooks.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * @addtogroup micro-os-plus-rtos-c-core
   * @{
   */

  // --------------------------------------------------------------------------
  /**
   * @name Return Codes
   * @{
   */

  /**
   * @brief Return codes, in addition to POSIX codes.
   */
  enum
  {
    /**
     * @brief Function completed; no error or event occurred.
     */
    micro_os_plus_ok = 0,
  };

  /**
   * @}
   */

  // --------------------------------------------------------------------------
  /**
   * @name Main Thread Function
   * @{
   */

  /**
   * @brief Application entry point, running on the main thread context.
   * @ingroup micro-os-plus-rtos-c
   * @headerfile os.h <micro-os-plus/rtos/c-api.h>
   * @param argc Count of arguments.
   * @param argv Array of string arguments.
   * @retval 0 The program terminated normally.
   * @retval 1 The program terminated with an error.
   *
   * @details
   * If the application does not define a main() function but defines
   * os_main(),
   * the µOS++ RTOS will automatically provide a main() function
   * that starts the main thread and calls os_main() within this context.
   *
   * The returned value is used in semihosted tests, to inform the
   * host on the result of the test.
   */
  int
  os_main (int argc, char* argv[]);

  /**
   * @}
   */

  // --------------------------------------------------------------------------
  /**
   * @name Scheduler Functions
   * @{
   */

  /**
   * @brief Initialise the RTOS scheduler.
   * @par Parameters
   *  None.
   * @retval micro_os_plus_ok The scheduler was initialised.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   */
  micro_os_plus_result_t
  micro_os_plus_scheduler_initialize (void);

  /**
   * @brief Start the RTOS scheduler.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   */
  void __attribute__ ((noreturn)) micro_os_plus_scheduler_start (void);

  /**
   * @brief Check if the scheduler was started.
   * @par Parameters
   *  None.
   * @retval true The scheduler was started.
   * @retval false The scheduler was not started.
   */
  bool
  micro_os_plus_scheduler_is_started (void);

  /**
   * @brief Lock the scheduler.
   * @par Parameters
   *  None.
   * @return The previous state of the scheduler lock.
   */
  micro_os_plus_scheduler_state_t
  micro_os_plus_scheduler_lock (void);

  /**
   * @brief Unlock the scheduler.
   * @par Parameters
   *  None.
   * @return The previous state of the scheduler lock.
   */
  micro_os_plus_scheduler_state_t
  micro_os_plus_scheduler_unlock (void);

  /**
   * @brief Lock/unlock the scheduler.
   * @param [in] state The new state of the scheduler lock.
   * @return The previous state of the scheduler lock.
   */
  micro_os_plus_scheduler_state_t
  micro_os_plus_scheduler_set_locked (micro_os_plus_scheduler_state_t state);

  /**
   * @brief Check if the scheduler is locked.
   * @par Parameters
   *  None.
   * @retval true The scheduler is locked.
   * @retval false The scheduler is switching threads (not locked).
   */
  bool
  micro_os_plus_scheduler_is_locked (void);

  /**
   * @brief Check if the scheduler is in preemptive mode.
   * @par Parameters
   *  None.
   * @retval true The scheduler is in preemptive mode.
   * @retval false The scheduler is not in preemptive mode.
   */
  bool
  micro_os_plus_scheduler_is_preemptive (void);

  /**
   * @brief Set the scheduler preemptive mode.
   * @param [in] state The new state of the scheduler preemptive mode.
   * @return The previous state of the preemptive mode.
   */
  bool
  micro_os_plus_scheduler_set_preemptive (bool state);

  /**
   * @}
   */

  // --------------------------------------------------------------------------
  /**
   * @name Scheduler Statistics Functions
   * @{
   */

#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

  /**
   * @brief Get the total number of context switches.
   * @return Integer with the total number of context switches since
   *  scheduler start.
   */
  micro_os_plus_statistics_counter_t
  micro_os_plus_scheduler_stat_get_context_switches (void);

#endif // defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

  /**
   * @brief Get the total duration of all threads.
   * @return Integer with the number of CPU cycles, possibly
   * divided by some prescaller.
   */
  micro_os_plus_statistics_duration_t
  micro_os_plus_scheduler_stat_get_cpu_cycles (void);

#endif // defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

  /**
   * @}
   */

  // --------------------------------------------------------------------------
  /**
   * @name Interrupts Functions
   * @{
   */

  /**
   * @brief Check if the CPU is in handler mode.
   * @par Parameters
   *  None.
   * @retval true Execution is in an exception handler context.
   * @retval false Execution is in a thread context.
   */
  bool
  micro_os_plus_irq_in_handler_mode (void);

  /**
   * @brief Enter an interrupts critical section.
   * @par Parameters
   *  None.
   * @return The previous value of the interrupts priority register.
   */
  micro_os_plus_irq_state_t
  micro_os_plus_irq_critical_enter (void);

  /**
   * @brief Exit the interrupts critical section.
   * @param state The value to restore the interrupts priority register.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_irq_critical_exit (micro_os_plus_irq_state_t state);

  // --------------------------------------------------------------------------

  /**
   * @brief Enter an interrupts uncritical section.
   * @par Parameters
   *  None.
   * @return The previous value of the interrupts priority register.
   */
  micro_os_plus_irq_state_t
  micro_os_plus_irq_uncritical_enter (void);

  /**
   * @brief Exit the interrupts uncritical section.
   * @param state The value to restore the interrupts priority register.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_irq_uncritical_exit (micro_os_plus_irq_state_t state);

#if defined(MICRO_OS_PLUS_HAS_INTERRUPTS_STACK) || defined(__DOXYGEN__)

  /**
   * @brief Get the interrupts stack.
   * @par Parameters
   *  None.
   * @return A pointer to the interrupts stack object instance.
   */
  micro_os_plus_thread_stack_t*
  micro_os_plus_irq_get_stack (void);

#endif

  /**
   * @}
   */

  /**
   * @}
   */

  // --------------------------------------------------------------------------
  /**
   * @addtogroup micro-os-plus-rtos-c-thread
   * @{
   */

  /**
   * @name Current Thread Functions
   * @{
   */

  /**
   * @brief Get the current running thread.
   * @par Parameters
   *  None.
   * @return Pointer to the current running thread object instance.
   */
  micro_os_plus_thread_t*
  micro_os_plus_this_thread (void);

  /**
   * @brief Suspend the current running thread to wait for an event.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_this_thread_suspend (void);

  /**
   * @brief Terminate the current running thread.
   * @param [in] exit_ptr Pointer to object to return. (Optional, may be NULL).
   * @par Returns
   *  Nothing.
   */
  void __attribute__ ((noreturn))
  micro_os_plus_this_thread_exit (void* exit_ptr);

  /**
   * @brief Wait for thread event flags.
   * @param [in] mask The expected flags (OR-ed bit-mask);
   *  may be zero.
   * @param [out] oflags Pointer where to store the current flags;
   *  may be `NULL`.
   * @param [in] mode Mode bits to select if either all or any flags
   *  are expected, and if the flags should be cleared.
   * @retval micro_os_plus_ok All expected flags are raised.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval EINVAL The mask is outside of the permitted range.
   * @retval EINTR The operation was interrupted.
   * @retval ENOTRECOVERABLE Wait failed.
   */
  micro_os_plus_result_t
  micro_os_plus_this_thread_flags_wait (micro_os_plus_flags_mask_t mask,
                                        micro_os_plus_flags_mask_t* oflags,
                                        micro_os_plus_flags_mode_t mode);

  /**
   * @brief Try to wait for thread event flags.
   * @param [in] mask The expected flags (OR-ed bit-mask);
   *  may be zero.
   * @param [out] oflags Pointer where to store the current flags;
   *  may be `NULL`.
   * @param [in] mode Mode bits to select if either all or any flags
   *  are expected, and if the flags should be cleared.
   * @retval micro_os_plus_ok All expected flags are raised.
   * @retval EINVAL The mask is outside of the permitted range.
   * @retval EWOULDBLOCK The expected condition did not occur.
   * @retval ENOTRECOVERABLE Wait failed.
   */
  micro_os_plus_result_t
  micro_os_plus_this_thread_flags_try_wait (micro_os_plus_flags_mask_t mask,
                                            micro_os_plus_flags_mask_t* oflags,
                                            micro_os_plus_flags_mode_t mode);

  /**
   * @brief Timed wait for thread event flags.
   * @param [in] mask The expected flags (OR-ed bit-mask);
   *  may be zero.
   * @param [out] oflags Pointer where to store the current flags;
   *  may be `NULL`.
   * @param [in] mode Mode bits to select if either all or any flags
   *  are expected, and if the flags should be cleared.
   * @param [in] timeout Timeout to wait, in clock units (ticks or seconds).
   * @retval micro_os_plus_ok All expected flags are raised.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval ETIMEDOUT The expected condition did not occur during the
   *  entire timeout duration.
   * @retval EINVAL The mask is outside of the permitted range.
   * @retval EINTR The operation was interrupted.
   * @retval ENOTRECOVERABLE Wait failed.
   */
  micro_os_plus_result_t
  micro_os_plus_this_thread_flags_timed_wait (
      micro_os_plus_flags_mask_t mask, micro_os_plus_clock_duration_t timeout,
      micro_os_plus_flags_mask_t* oflags, micro_os_plus_flags_mode_t mode);

  /**
   * @brief Clear thread event flags.
   * @param [in] mask The OR-ed flags to clear.
   * @param [out] oflags Optional pointer where to store the
   *  previous flags; may be `NULL`.
   * @retval micro_os_plus_ok The flags were cleared.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval EINVAL The mask is zero.
   */
  micro_os_plus_result_t
  micro_os_plus_this_thread_flags_clear (micro_os_plus_flags_mask_t mask,
                                         micro_os_plus_flags_mask_t* oflags);

  /**
   * @brief Get/clear thread event flags.
   * @param [in] mask The OR-ed flags to get/clear; may be zero.
   * @param [in] mode Mode bits to select if the flags should be
   *  cleared (the other bits are ignored).
   * @retval flags The selected bits from the current thread
   *  event flags mask.
   * @retval sig::all Cannot be invoked from an Interrupt Service Routines.
   */
  micro_os_plus_flags_mask_t
  micro_os_plus_this_thread_flags_get (micro_os_plus_flags_mask_t mask,
                                       micro_os_plus_flags_mode_t mode);

  /**
   * @}
   */

  // --------------------------------------------------------------------------
  /**
   * @name Thread Attribute Functions
   * @{
   */

  /**
   * @brief Initialise the thread attributes.
   * @param [in] attr Pointer to thread attributes object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_thread_attributes_init (
      micro_os_plus_thread_attributes_t* attr);

  /**
   * @}
   */

  /**
   * @name Thread Creation Functions
   * @{
   */

  /**
   * @brief Construct a statically allocated thread object instance.
   * @param [in] thread Pointer to thread object instance storage.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] func Pointer to thread function.
   * @param [in] args Pointer to thread function arguments (may be NULL).
   * @param [in] attr Pointer to attributes (may be NULL)
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_thread_construct (
      micro_os_plus_thread_t* thread, const char* name,
      micro_os_plus_thread_func_t func,
      const micro_os_plus_thread_func_args_t args,
      const micro_os_plus_thread_attributes_t* attr);

  /**
   * @brief Destruct the statically allocated thread object instance.
   * @param [in] thread Pointer to thread object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_thread_destruct (micro_os_plus_thread_t* thread);

  /**
   * @brief Allocate a thread object instance and construct it.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] func Pointer to thread function.
   * @param [in] args Pointer to thread function arguments (may be NULL).
   * @param [in] attr Pointer to attributes (may be NULL)
   * @return Pointer to new thread object instance.
   */
  micro_os_plus_thread_t*
  micro_os_plus_thread_new (const char* name, micro_os_plus_thread_func_t func,
                            const micro_os_plus_thread_func_args_t args,
                            const micro_os_plus_thread_attributes_t* attr);

  /**
   * @brief Destruct the thread object instance and deallocate it.
   * @param [in] thread Pointer to dynamically allocated thread object
   * instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_thread_delete (micro_os_plus_thread_t* thread);

  /**
   * @}
   */

  /**
   * @name Thread Functions
   * @{
   */

  /**
   * @brief Get the thread name.
   * @param [in] thread Pointer to thread object instance.
   * @return Null terminated string.
   */
  const char*
  micro_os_plus_thread_get_name (micro_os_plus_thread_t* thread);

  /**
   * @brief Get the thread current scheduling priority.
   * @param [in] thread Pointer to thread object instance.
   * @return The thread priority.
   */
  micro_os_plus_thread_priority_t
  micro_os_plus_thread_get_priority (micro_os_plus_thread_t* thread);

  /**
   * @brief Set the thread dynamic scheduling priority.
   * @param [in] thread Pointer to thread object instance.
   * @param [in] prio New priority.
   * @retval micro_os_plus_ok The priority was set.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval EINVAL The value of prio is invalid for the
   *  scheduling policy of the specified thread.
   */
  micro_os_plus_result_t
  micro_os_plus_thread_set_priority (micro_os_plus_thread_t* thread,
                                     micro_os_plus_thread_priority_t prio);

  /**
   * @brief Wait for thread termination.
   * @param [in] thread Pointer to terminating thread object instance.
   * @param [in] exit_ptr Pointer to thread exit value. (may be NULL).
   * @retval micro_os_plus_ok The thread was terminated.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   */
  micro_os_plus_result_t
  micro_os_plus_thread_join (micro_os_plus_thread_t* thread, void** exit_ptr);

  /**
   * @brief Resume the thread.
   * @param [in] thread Pointer to thread object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_thread_resume (micro_os_plus_thread_t* thread);

  /**
   * @brief Raise thread event flags.
   * @param [in] thread Pointer to thread object instance.
   * @param [in] mask The OR-ed flags to raise.
   * @param [out] oflags Optional pointer where to store the
   *  previous flags; may be `NULL`.
   * @retval micro_os_plus_ok The flags were raised.
   * @retval EINVAL The mask is zero.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   */
  micro_os_plus_result_t
  micro_os_plus_thread_flags_raise (micro_os_plus_thread_t* thread,
                                    micro_os_plus_flags_mask_t mask,
                                    micro_os_plus_flags_mask_t* oflags);

  /**
   * @brief Get the thread scheduler state.
   * @param [in] thread Pointer to thread object instance.
   * @return Thread scheduler state.
   */
  micro_os_plus_thread_state_t
  micro_os_plus_thread_get_state (micro_os_plus_thread_t* thread);

#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_CUSTOM_THREAD_USER_STORAGE) \
    || defined(__DOXYGEN__)

  /**
   * @brief Get the thread user storage.
   * @param [in] thread Pointer to thread object instance.
   * @return The address of the thread user storage.
   */
  micro_os_plus_thread_user_storage_t*
  micro_os_plus_thread_get_user_storage (micro_os_plus_thread_t* thread);

#endif // defined(MICRO_OS_PLUS_INCLUDE_RTOS_CUSTOM_THREAD_USER_STORAGE)

  /**
   * @brief Get the thread context stack.
   * @param [in] thread Pointer to thread object instance.
   * @return A pointer to the context stack object instance.
   */
  micro_os_plus_thread_stack_t*
  micro_os_plus_thread_get_stack (micro_os_plus_thread_t* thread);

  /**
   * @}
   */

  // --------------------------------------------------------------------------
  /**
   * @name Thread Stack Functions
   * @{
   */

  /**
   * @brief Get the default stack size.
   * @par Parameters
   *  None.
   * @return  The default stack size in bytes.
   */
  size_t
  micro_os_plus_thread_stack_get_default_size (void);

  /**
   * @brief Set the default stack size.
   * @param [in] size_bytes Default stack size in bytes.
   * @return  The previous value of the default stack size in bytes.
   */
  size_t
  micro_os_plus_thread_stack_set_default_size (size_t size_bytes);

  /**
   * @brief Get the min stack size.
   * @par Parameters
   *  None.
   * @return  The min stack size in bytes.
   */
  size_t
  micro_os_plus_thread_stack_get_min_size (void);

  /**
   * @brief Set the min stack size.
   * @param [in] size_bytes Minimum stack size in bytes.
   * @return  The previous value of the min stack size in bytes.
   */
  size_t
  micro_os_plus_thread_stack_set_min_size (size_t size_bytes);

  /**
   * @brief Get the stack lowest reserved address.
   * @param [in] stack Pointer to stack object instance.
   * @return  The address of the stack reserved area.
   */
  micro_os_plus_thread_stack_element_t*
  micro_os_plus_thread_stack_get_bottom (micro_os_plus_thread_stack_t* stack);

  /**
   * @brief Get the top stack address.
   * @param [in] stack Pointer to stack object instance.
   * @return The address after the last stack element.
   */
  micro_os_plus_thread_stack_element_t*
  micro_os_plus_thread_stack_get_top (micro_os_plus_thread_stack_t* stack);

  /**
   * @brief Get the stack size.
   * @param [in] stack Pointer to stack object instance.
   * @return  The stack size in bytes.
   */
  size_t
  micro_os_plus_thread_stack_get_size (micro_os_plus_thread_stack_t* stack);

  /**
   * @brief Compute how much available stack remains.
   * @param [in] stack Pointer to stack object instance.
   * @return Number of available bytes.
   */
  size_t
  micro_os_plus_thread_stack_get_available (
      micro_os_plus_thread_stack_t* stack);

  /**
   * @brief Check if bottom magic word is still there.
   * @param [in] stack Pointer to stack object instance.
   * @retval true  The magic word is still there.
   * @retval false  The magic word was overwritten.
   */
  bool
  micro_os_plus_thread_stack_check_bottom_magic (
      micro_os_plus_thread_stack_t* stack);

  /**
   * @brief Check if top magic word is still there.
   * @param [in] stack Pointer to stack object instance.
   * @retval true  The magic word is still there.
   * @retval false  The magic word was overwritten.
   */
  bool
  micro_os_plus_thread_stack_check_top_magic (
      micro_os_plus_thread_stack_t* stack);

  /**
   * @}
   */

  // --------------------------------------------------------------------------
  /**
   * @name Thread Statistics Functions
   * @{
   */

#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

  /**
   * @brief Get the number of thread context switches.
   * @return A long integer with the number of times the thread
   * was scheduled for execution.
   */
  micro_os_plus_statistics_counter_t
  micro_os_plus_thread_stat_get_context_switches (
      micro_os_plus_thread_t* thread);

#endif // defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

  /**
   * @brief Get the thread execution time.
   * @return A long integer with the accumulated number of CPU cycles,
   * possibly divided by some prescaller.
   */
  micro_os_plus_statistics_duration_t
  micro_os_plus_thread_stat_get_cpu_cycles (micro_os_plus_thread_t* thread);

#endif // defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

  /**
   * @}
   */

  // --------------------------------------------------------------------------
  /**
   * @name Thread Children Iterator Functions
   * @{
   */

  /**
   * @brief Get the beginning of the list of children threads.
   * @param [in] thread Pointer to thread object or NULL for the
   * list of top threads.
   * @return An iterator positioned at the list begin.
   */
  micro_os_plus_iterator_t
  micro_os_plus_children_threads_iter_begin (micro_os_plus_thread_t* thread);

  /**
   * @brief Get the end of the list of children threads.
   * @param [in] thread Pointer to thread object or NULL for the
   * list of top threads.
   * @return An iterator positioned at the list end.
   */
  micro_os_plus_iterator_t
  micro_os_plus_children_threads_iter_end (micro_os_plus_thread_t* thread);

  /**
   * @brief Get the thread from the current iterator position.
   * @param [in] iterator An active iterator.
   * @return The pointer to the thread object instance.
   */
  micro_os_plus_thread_t*
  micro_os_plus_children_threads_iter_get (micro_os_plus_iterator_t iterator);

  /**
   * @brief Advance the iterator to the next position.
   * @param [in] iterator An active iterator.
   * @return An iterator positioned at the next list element.
   */
  micro_os_plus_iterator_t
  micro_os_plus_children_threads_iter_next (micro_os_plus_iterator_t iterator);

  /**
   * @}
   */

  /**
   * @}
   */

  // --------------------------------------------------------------------------
  /**
   * @addtogroup micro-os-plus-rtos-c-clock
   * @{
   */

  /**
   * @name Clock Functions
   * @{
   */

  /**
   * @brief Get the clock name.
   * @param [in] clock Pointer to clock object instance.
   * @return Null terminated string.
   */
  const char*
  micro_os_plus_clock_get_name (micro_os_plus_clock_t* clock);

  /**
   * @brief Tell the current time, possibly adjusted for epoch.
   * @param [in] clock Pointer to clock object instance.
   * @return The clock current timestamp (time units from startup
   * plus the epoch offset).
   */
  micro_os_plus_clock_timestamp_t
  micro_os_plus_clock_now (micro_os_plus_clock_t* clock);

  /**
   * @brief Tell the current time since startup.
   * @param [in] clock Pointer to clock object instance.
   * @return The clock current timestamp (time units from startup).
   */
  micro_os_plus_clock_timestamp_t
  micro_os_plus_clock_steady_now (micro_os_plus_clock_t* clock);

  /**
   * @brief Sleep for a relative duration.
   * @param [in] clock Pointer to clock object instance.
   * @param [in] duration The number of clock units
   *  (ticks or seconds) to sleep.
   * @retval ETIMEDOUT The sleep lasted the entire duration.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval EINTR The sleep was interrupted.
   */
  micro_os_plus_result_t
  micro_os_plus_clock_sleep_for (micro_os_plus_clock_t* clock,
                                 micro_os_plus_clock_duration_t duration);

  /**
   * @brief Sleep until an absolute timestamp.
   * @param [in] clock Pointer to clock object instance.
   * @param [in] timestamp The absolute moment in time, in clock units.
   * @retval ETIMEDOUT The sleep lasted the entire duration.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval EINTR The sleep was interrupted.
   */
  micro_os_plus_result_t
  micro_os_plus_clock_sleep_until (micro_os_plus_clock_t* clock,
                                   micro_os_plus_clock_timestamp_t timestamp);

  /**
   * @brief Timed wait for an event.
   * @param [in] clock Pointer to clock object instance.
   * @param [in] timeout The timeout in clock units.
   * @retval micro_os_plus_ok An event occurred before the timeout.
   * @retval ETIMEDOUT The wait lasted the entire duration.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval EINTR The sleep was interrupted.
   */
  micro_os_plus_result_t
  micro_os_plus_clock_wait_for (micro_os_plus_clock_t* clock,
                                micro_os_plus_clock_duration_t timeout);

  /**
   * @brief Get adjustment offset.
   * @param [in] clock Pointer to clock object instance.
   * @return Integer value representing the offset to epoch.
   */
  micro_os_plus_clock_offset_t
  micro_os_plus_clock_get_offset (micro_os_plus_clock_t* clock);

  /**
   * @brief Set adjustment offset.
   * @param [in] clock Pointer to clock object instance.
   * @param [in] offset Integer representing the offset to epoch (positive).
   * @return Integer value representing the previous offset to epoch.
   */
  micro_os_plus_clock_offset_t
  micro_os_plus_clock_set_offset (micro_os_plus_clock_t* clock,
                                  micro_os_plus_clock_offset_t offset);

  /**
   * @brief Get `sysclock` (the system clock).
   * @return The address of the clock_systick instance.
   */
  micro_os_plus_clock_t*
  micro_os_plus_clock_get_sysclock (void);

  /**
   * @brief Get `rtclock` (the real-time clock).
   * @return The address of the clock_rtc instance.
   */
  micro_os_plus_clock_t*
  micro_os_plus_clock_get_rtclock (void);

  /**
   * @brief Get `hrclock` (the high resolution clock).
   * @return The address of the clock_highres instance.
   */
  micro_os_plus_clock_t*
  micro_os_plus_clock_get_hrclock (void);

  // --------------------------------------------------------------------------

  /**
   * @brief Tell the current time since startup.
   * @par Parameters
   *  None.
   * @return The number of SysTick input clocks since startup.
   */
  micro_os_plus_clock_timestamp_t
  micro_os_plus_sysclock_now (void);

  /**
   * @brief Sleep for a relative duration.
   * @param [in] duration The number of clock units
   *  (ticks or seconds) to sleep.
   * @retval ETIMEDOUT The sleep lasted the entire duration.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval EINTR The sleep was interrupted.
   */
  micro_os_plus_result_t
  micro_os_plus_sysclock_sleep_for (micro_os_plus_clock_duration_t duration);

  /**
   * @brief Sleep until an absolute timestamp.
   * @param [in] timestamp The absolute moment in time, in clock units.
   * @retval ETIMEDOUT The sleep lasted the entire duration.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval EINTR The sleep was interrupted.
   */
  micro_os_plus_result_t
  micro_os_plus_sysclock_sleep_until (
      micro_os_plus_clock_timestamp_t timestamp);

  /**
   * @brief Timed wait for an event.
   * @param [in] timeout The timeout in clock units.
   * @retval micro_os_plus_ok An event occurred before the timeout.
   * @retval ETIMEDOUT The wait lasted the entire duration.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval EINTR The sleep was interrupted.
   */
  micro_os_plus_result_t
  micro_os_plus_sysclock_wait_for (micro_os_plus_clock_duration_t timeout);

#pragma GCC diagnostic push
#if defined(__cplusplus)
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

  /**
   * @brief Convert microseconds to ticks.
   * @param [in] microsec The number of microseconds.
   * @return The number of ticks.
   */
  inline __attribute__ ((always_inline)) micro_os_plus_clock_duration_t
  micro_os_plus_sysclock_ticks_cast (uint32_t microsec)
  {
    return (micro_os_plus_clock_duration_t) (
        (((microsec) * ((uint32_t)MICRO_OS_PLUS_INTEGER_SYSTICK_FREQUENCY_HZ))
         + (uint32_t)1000000ul - 1)
        / (uint32_t)1000000ul);
  }

  /**
   * @brief Convert microseconds to ticks.
   * @param [in] microsec The number of microseconds.
   * @return The number of ticks.
   */
  inline __attribute__ ((always_inline)) micro_os_plus_clock_duration_t
  micro_os_plus_sysclock_ticks_cast_long (uint64_t microsec)
  {
    return (micro_os_plus_clock_duration_t) (
        (((microsec) * ((uint64_t)MICRO_OS_PLUS_INTEGER_SYSTICK_FREQUENCY_HZ))
         + (uint64_t)1000000ul - 1)
        / (uint64_t)1000000ul);
  }

#pragma GCC diagnostic pop

  /**
   * @}
   */

  /**
   * @}
   */

  // --------------------------------------------------------------------------
  /**
   * @addtogroup micro-os-plus-rtos-c-timer
   * @{
   */

  /**
   * @name Timer Attribute Functions
   * @{
   */

  /**
   * @brief Initialise the single shot timer attributes.
   * @param [in] attr Pointer to timer attributes object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_timer_attributes_init (micro_os_plus_timer_attributes_t* attr);

  /**
   * @brief Initialise the periodic timer attributes.
   * @param [in] attr Pointer to timer attributes object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_timer_attributes_periodic_init (
      micro_os_plus_timer_attributes_t* attr);

  /**
   * @brief Get a periodic timer attributes object instance.
   * @return Pointer to timer attributes object instance.
   */
  const micro_os_plus_timer_attributes_t*
  micro_os_plus_timer_attributes_get_periodic (void);

  /**
   * @}
   */

  /**
   * @name Timer Creation Functions
   * @{
   */

  /**
   * @brief Construct a statically allocated timer object instance.
   * @param [in] timer Pointer to timer object instance storage.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] function Pointer to timer function.
   * @param [in] args Pointer to timer function arguments (may be NULL).
   * @param [in] attr Pointer to attributes (may be NULL).
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_timer_construct (micro_os_plus_timer_t* timer,
                                 const char* name,
                                 micro_os_plus_timer_func_t function,
                                 micro_os_plus_timer_func_args_t args,
                                 const micro_os_plus_timer_attributes_t* attr);

  /**
   * @brief Destruct the statically allocated timer object instance.
   * @param [in] timer Pointer to timer object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_timer_destruct (micro_os_plus_timer_t* timer);

  /**
   * @brief Allocate a timer object instance and construct it.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] function Pointer to timer function.
   * @param [in] args Pointer to timer function arguments (may be NULL).
   * @param [in] attr Pointer to attributes (may be NULL).
   * @return Pointer to new timer object instance.
   */
  micro_os_plus_timer_t*
  micro_os_plus_timer_new (const char* name,
                           micro_os_plus_timer_func_t function,
                           micro_os_plus_timer_func_args_t args,
                           const micro_os_plus_timer_attributes_t* attr);

  /**
   * @brief Destruct the timer object instance and deallocate it.
   * @param [in] timer Pointer to dynamically allocated timer object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_timer_delete (micro_os_plus_timer_t* timer);

  /**
   * @}
   */

  /**
   * @name Timer Functions
   * @{
   */

  /**
   * @brief Get the timer name.
   * @param [in] timer Pointer to timer object instance.
   * @return Null terminated string.
   */
  const char*
  micro_os_plus_timer_get_name (micro_os_plus_timer_t* timer);

  /**
   * @brief Start or restart the timer.
   * @param [in] timer Pointer to timer object instance.
   * @param [in] period Timer period, in clock units (ticks or seconds).
   * @retval micro_os_plus_ok The timer has been started or restarted.
   * @retval ENOTRECOVERABLE Timer could not be started.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   */
  micro_os_plus_result_t
  micro_os_plus_timer_start (micro_os_plus_timer_t* timer,
                             micro_os_plus_clock_duration_t period);

  /**
   * @brief Stop the timer.
   * @param [in] timer Pointer to timer object instance.
   * @retval micro_os_plus_ok The timer has been stopped.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval EAGAIN The timer is not yet started.
   * @retval ENOTRECOVERABLE Timer could not be stopped.
   */
  micro_os_plus_result_t
  micro_os_plus_timer_stop (micro_os_plus_timer_t* timer);

  /**
   * @}
   */

  /**
   * @}
   */

  // --------------------------------------------------------------------------
  /**
   * @addtogroup micro-os-plus-rtos-c-mutex
   * @{
   */

  /**
   * @name Mutex Attributes Functions
   * @{
   */

  /**
   * @brief Initialise the mutex attributes.
   * @param [in] attr Pointer to mutex attributes object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_mutex_attributes_init (micro_os_plus_mutex_attributes_t* attr);

  /**
   * @brief Initialise the recursive mutex attributes.
   * @param [in] attr Pointer to mutex attributes object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_mutex_attributes_recursive_init (
      micro_os_plus_mutex_attributes_t* attr);

  /**
   * @brief Get a recursive mutex attributes object instance.
   * @return Pointer to mutex attributes object instance.
   */
  const micro_os_plus_mutex_attributes_t*
  micro_os_plus_mutex_attributes_get_recursive (void);

  /**
   * @}
   */

  /**
   * @name Mutex Creation Functions
   * @{
   */

  /**
   * @brief Construct a statically allocated mutex object instance.
   * @param [in] mutex Pointer to mutex object instance storage.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] attr Pointer to attributes (may be NULL).
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_mutex_construct (micro_os_plus_mutex_t* mutex,
                                 const char* name,
                                 const micro_os_plus_mutex_attributes_t* attr);

  /**
   * @brief Construct a statically allocated recursive mutex object instance.
   * @param [in] mutex Pointer to mutex object instance storage.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] attr Pointer to attributes (may be NULL).
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_mutex_recursive_construct (
      micro_os_plus_mutex_t* mutex, const char* name,
      const micro_os_plus_mutex_attributes_t* attr);

  /**
   * @brief Destruct the statically allocated mutex object instance.
   * @param [in] mutex Pointer to mutex object instance storage.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_mutex_destruct (micro_os_plus_mutex_t* mutex);

  /**
   * @brief Allocate a mutex object instance and construct it.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] attr Pointer to attributes (may be NULL).
   * @return Pointer to new mutex object instance.
   */
  micro_os_plus_mutex_t*
  micro_os_plus_mutex_new (const char* name,
                           const micro_os_plus_mutex_attributes_t* attr);

  /**
   * @brief Allocated a recursive mutex object instance and construct it.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] attr Pointer to attributes (may be NULL).
   * @return Pointer to new recursive mutex object instance.
   */
  micro_os_plus_mutex_t*
  micro_os_plus_mutex_recursive_new (
      const char* name, const micro_os_plus_mutex_attributes_t* attr);

  /**
   * @brief Destruct the mutex object instance and deallocate it.
   * @param [in] mutex Pointer to dynamically allocated mutex object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_mutex_delete (micro_os_plus_mutex_t* mutex);

  /**
   * @}
   */

  /**
   * @name Mutex Functions
   * @{
   */

  /**
   * @brief Get the mutex name.
   * @param [in] mutex Pointer to mutex object instance.
   * @return Null terminated string.
   */
  const char*
  micro_os_plus_mutex_get_name (micro_os_plus_mutex_t* mutex);

  /**
   * @brief Lock/acquire the mutex.
   * @param [in] mutex Pointer to mutex object instance.
   * @retval micro_os_plus_ok The mutex was locked.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval ENOTRECOVERABLE The state protected by the mutex is
   *  not recoverable..
   * @retval EAGAIN The mutex could not be acquired because the maximum
   *  number of recursive locks for mutex has been exceeded.
   * @retval EINVAL The mutex was created with the protocol
   *  attribute having the value `micro_os_plus_mutex_protocol_protect` and the
   *  calling thread's priority is higher than the mutex's
   *  current priority ceiling.
   * @retval EOWNERDEAD The mutex is a robust mutex and the process
   *  containing the previous owning thread terminated while holding
   *  the mutex lock. The mutex lock shall be acquired by the calling
   *  thread and it is up to the new owner to make the state consistent.
   * @retval EDEADLK The mutex type is `micro_os_plus_mutex_type_errorcheck`
   * and the current thread already owns the mutex.
   */
  micro_os_plus_result_t
  micro_os_plus_mutex_lock (micro_os_plus_mutex_t* mutex);

  /**
   * @brief Try to lock/acquire the mutex.
   * @param [in] mutex Pointer to mutex object instance.
   * @retval micro_os_plus_ok The mutex was locked.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval ENOTRECOVERABLE The state protected by the mutex is
   *  not recoverable..
   * @retval EAGAIN The mutex could not be acquired because the maximum
   *  number of recursive locks for mutex has been exceeded.
   * @retval EINVAL The mutex was created with the protocol
   *  attribute having the value `micro_os_plus_mutex_protocol_protect` and the
   *  calling thread's priority is higher than the mutex's
   *  current priority ceiling.
   * @retval EOWNERDEAD The mutex is a robust mutex and the process
   *  containing the previous owning thread terminated while holding
   *  the mutex lock. The mutex lock shall be acquired by the calling
   *  thread and it is up to the new owner to make the state consistent.
   * @retval EDEADLK The mutex type is `micro_os_plus_mutex_type_errorcheck`
   * and the current thread already owns the mutex.
   * @retval EWOULDBLOCK The mutex could not be acquired because it was
   *  already locked.
   */
  micro_os_plus_result_t
  micro_os_plus_mutex_try_lock (micro_os_plus_mutex_t* mutex);

  /**
   * @brief Timed attempt to lock/acquire the mutex.
   * @param [in] mutex Pointer to mutex object instance.
   * @param [in] timeout Timeout to wait, in clock units (ticks or seconds).
   * @retval micro_os_plus_ok The mutex was locked.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval ETIMEDOUT The mutex could not be locked before the
   *  specified timeout expired.
   * @retval ENOTRECOVERABLE The state protected by the mutex
   *  is not recoverable.
   * @retval EAGAIN The mutex could not be acquired because the
   *  maximum number of recursive locks for mutex has been exceeded.
   * @retval EDEADLK The mutex type is `micro_os_plus_mutex_type_errorcheck`
   *  and the current thread already owns the mutex.
   * @retval EINVAL The process or thread would have blocked, and
   *  the timeout parameter is invalid.
   * @retval EOWNERDEAD The mutex is a robust mutex and the process
   *  containing the previous owning thread terminated while holding
   *  the mutex lock. The mutex lock shall be acquired by the
   *  calling thread and it is up to the new owner to make the
   *  state consistent.
   */
  micro_os_plus_result_t
  micro_os_plus_mutex_timed_lock (micro_os_plus_mutex_t* mutex,
                                  micro_os_plus_clock_duration_t timeout);

  /**
   * @brief Unlock/release the mutex.
   * @param [in] mutex Pointer to mutex object instance.
   * @retval micro_os_plus_ok The mutex was unlocked.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routine;
   *  the mutex type is `micro_os_plus_mutex_type_errorcheck` or
   *  `micro_os_plus_mutex_type_recursive`, or the mutex is a robust mutex,
   *  and the current thread does not own the mutex.
   * @retval ENOTRECOVERABLE The mutex was not unlocked.
   */
  micro_os_plus_result_t
  micro_os_plus_mutex_unlock (micro_os_plus_mutex_t* mutex);

  /**
   * @brief Get the priority ceiling of a mutex.
   * @param [in] mutex Pointer to mutex object instance.
   * @return The priority ceiling.
   */
  micro_os_plus_thread_priority_t
  micro_os_plus_mutex_get_priority_ceiling (micro_os_plus_mutex_t* mutex);

  /**
   * @brief Change the priority ceiling of a mutex.
   * @param [in] mutex Pointer to mutex object instance.
   * @param [in] priority_ceiling new priority.
   * @param [out] old_priority_ceiling pointer to location where to
   *  store the previous priority; may be `NULL`.
   * @retval micro_os_plus_ok The priority was changed.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   */
  micro_os_plus_result_t
  micro_os_plus_mutex_set_priority_ceiling (
      micro_os_plus_mutex_t* mutex,
      micro_os_plus_thread_priority_t priority_ceiling,
      micro_os_plus_thread_priority_t* old_priority_ceiling);

  /**
   * @brief Mark mutex as consistent.
   * @param [in] mutex Pointer to mutex object instance.
   * @retval micro_os_plus_ok The mutex was marked as consistent.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval EINVAL The mutex object referenced by mutex is not robust
   *  or does not protect an inconsistent state.
   */
  micro_os_plus_result_t
  micro_os_plus_mutex_mark_consistent (micro_os_plus_mutex_t* mutex);

  /**
   * @brief Get the thread that owns the mutex.
   * @param [in] mutex Pointer to mutex object instance.
   * @return Pointer to thread or `NULL` if not owned.
   */
  micro_os_plus_thread_t*
  micro_os_plus_mutex_get_owner (micro_os_plus_mutex_t* mutex);

  /**
   * @brief Get the mutex type.
   * @param [in] mutex Pointer to mutex object instance.
   * @return An integer encoding the @ref micro_os_plus::rtos::mutex::type.
   */
  micro_os_plus_mutex_type_t
  micro_os_plus_mutex_get_type (micro_os_plus_mutex_t* mutex);

  /**
   * @brief Get the mutex protocol.
   * @param [in] mutex Pointer to mutex object instance.
   * @return An integer encoding the @ref micro_os_plus::rtos::mutex::protocol.
   */
  micro_os_plus_mutex_protocol_t
  micro_os_plus_mutex_get_protocol (micro_os_plus_mutex_t* mutex);

  /**
   * @brief Get the mutex robustness.
   * @param [in] mutex Pointer to mutex object instance.
   * @return An integer encoding the @ref
   * micro_os_plus::rtos::mutex::robustness.
   */
  micro_os_plus_mutex_robustness_t
  micro_os_plus_mutex_get_robustness (micro_os_plus_mutex_t* mutex);

  /**
   * @brief Reset the mutex.
   * @param [in] mutex Pointer to mutex object instance.
   * @retval micro_os_plus_ok The mutex was reset.
   */
  micro_os_plus_result_t
  micro_os_plus_mutex_reset (micro_os_plus_mutex_t* mutex);

  /**
   * @}
   */

  /**
   * @}
   */

  // --------------------------------------------------------------------------
  /**
   * @addtogroup micro-os-plus-rtos-c-condvar
   * @{
   */

  /**
   * @name Condition Variable Attributes Functions
   * @{
   */

  /**
   * @brief Initialise the condition variable attributes.
   * @param [in] attr Pointer to condition variable attributes object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_condvar_attributes_init (
      micro_os_plus_condvar_attributes_t* attr);

  /**
   * @}
   */

  /**
   * @name Condition Variable Creation Functions
   * @{
   */

  /**
   * @brief Construct a statically allocated condition variable
   *  object instance.
   * @param [in] condvar Pointer to condition variable object instance storage.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] attr Pointer to attributes (may be NULL).
   * @par Errors
   *  The constructor shall fail if:
   *  - `EAGAIN` - The system lacked the necessary resources
   *  (other than memory) to create the condition variable.
   *  - `ENOMEM` - Insufficient memory exists to initialise
   *  the condition variable.
   * @par
   *  The constructor shall not fail with an error code of `EINTR`.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_condvar_construct (
      micro_os_plus_condvar_t* condvar, const char* name,
      const micro_os_plus_condvar_attributes_t* attr);

  /**
   * @brief Destruct the statically allocated condition variable
   *  object instance.
   * @param [in] condvar Pointer to condition variable object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_condvar_destruct (micro_os_plus_condvar_t* condvar);

  /**
   * @brief Allocate a condition variable object instance and construct it.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] attr Pointer to attributes (may be NULL).
   * @par Errors
   *  The constructor shall fail if:
   *  - `EAGAIN` - The system lacked the necessary resources
   *  (other than memory) to create the condition variable.
   *  - `ENOMEM` - Insufficient memory exists to initialise
   *  the condition variable.
   * @par
   *  The constructor shall not fail with an error code of `EINTR`.
   * @return Pointer to new condition variable object instance.
   */
  micro_os_plus_condvar_t*
  micro_os_plus_condvar_new (const char* name,
                             const micro_os_plus_condvar_attributes_t* attr);

  /**
   * @brief Destruct the condition variable object instance and deallocate it.
   * @param [in] condvar Pointer to dynamically allocated condition
   *  variable object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_condvar_delete (micro_os_plus_condvar_t* condvar);

  /**
   * @}
   */

  /**
   * @name Condition Variable Functions
   * @{
   */

  /**
   * @brief Get the condition variable name.
   * @param [in] condvar Pointer to condition variable object instance.
   * @return Null terminated string.
   */
  const char*
  micro_os_plus_condvar_get_name (micro_os_plus_condvar_t* condvar);

  /**
   * @brief Notify one thread waiting for a condition variable.
   * @param [in] condvar Pointer to condition variable object instance.
   * @retval micro_os_plus_ok The thread was signalled.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @par Errors
   *  The function shall not fail with an error code of `EINTR`.
   */
  micro_os_plus_result_t
  micro_os_plus_condvar_signal (micro_os_plus_condvar_t* condvar);

  /**
   * @brief Notify all threads waiting for a condition variable.
   * @param [in] condvar Pointer to condition variable object instance.
   * @retval micro_os_plus_ok All waiting threads signalled.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @par Errors
   *  The function shall not fail with an error code of `EINTR`.
   */
  micro_os_plus_result_t
  micro_os_plus_condvar_broadcast (micro_os_plus_condvar_t* condvar);

  /**
   * @brief Wait for a condition variable to be notified.
   * @param [in] condvar Pointer to condition variable object instance.
   * @param [in] mutex Pointer to the associated mutex.
   * @retval micro_os_plus_ok The condition change was signalled.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines,
   *  or the mutex type is `mutex::type::errorcheck` or the mutex
   *  is a robust mutex, and the current thread does not own the mutex.
   * @retval ENOTRECOVERABLE The state protected by the mutex is
   *  not recoverable.
   * @retval EOWNERDEAD The mutex is a robust mutex and the
   *  process containing the previous owning thread terminated
   *  while holding the mutex lock. The mutex lock shall be
   *  acquired by the calling thread and it is up to the new
   *  owner to make the state consistent.
   * @par Errors
   *  The function shall not fail with an error code of `EINTR`.
   */
  micro_os_plus_result_t
  micro_os_plus_condvar_wait (micro_os_plus_condvar_t* condvar,
                              micro_os_plus_mutex_t* mutex);

  /**
   * @brief Timed wait for a condition variable to be notified.
   * @param [in] condvar Pointer to condition variable object instance.
   * @param [in] mutex Pointer to the associated mutex.
   * @param [in] timeout Timeout to wait.
   * @retval micro_os_plus_ok The condition change was signalled.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines,
   *  or the mutex type is `mutex::type::errorcheck` or the mutex
   *  is a robust mutex, and the current thread does not own the mutex.
   * @retval ENOTRECOVERABLE The state protected by the mutex is
   *  not recoverable.
   * @retval EOWNERDEAD The mutex is a robust mutex and the
   *  process containing the previous owning thread terminated
   *  while holding the mutex lock. The mutex lock shall be
   *  acquired by the calling thread and it is up to the new
   *  owner to make the state consistent.
   * @retval ETIMEDOUT The timeout has passed.
   * @par Errors
   *  The function shall not fail with an error code of `EINTR`.
   */
  micro_os_plus_result_t
  micro_os_plus_condvar_timed_wait (micro_os_plus_condvar_t* condvar,
                                    micro_os_plus_mutex_t* mutex,
                                    micro_os_plus_clock_duration_t timeout);

  /**
   * @}
   */

  /**
   * @}
   */

  // --------------------------------------------------------------------------
  /**
   * @addtogroup micro-os-plus-rtos-c-semaphore
   * @{
   */

  /**
   * @name Semaphore Attributes Functions
   * @{
   */

  /**
   * @brief Initialise the counting semaphore attributes.
   * @param [in] attr Pointer to semaphore attributes object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_semaphore_attributes_init (
      micro_os_plus_semaphore_attributes_t* attr);

  /**
   * @brief Initialise the binary semaphore attributes.
   * @param [in] attr Pointer to semaphore attributes object instance.
   * @param [in] initial_value Initial count value.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_semaphore_attributes_binary_init (
      micro_os_plus_semaphore_attributes_t* attr,
      const micro_os_plus_semaphore_count_t initial_value);

  /**
   * @brief Initialise the counting semaphore attributes.
   * @param [in] attr Pointer to semaphore attributes object instance.
   * @param [in] max_value Maximum count value.
   * @param [in] initial_value Initial count value.
   */
  void
  micro_os_plus_semaphore_attributes_counting_init (
      micro_os_plus_semaphore_attributes_t* attr,
      const micro_os_plus_semaphore_count_t max_value,
      const micro_os_plus_semaphore_count_t initial_value);

  /**
   * @brief Get a binary semaphore attributes object instance.
   * @return Pointer to semaphore attributes object instance.
   */
  const micro_os_plus_semaphore_attributes_t*
  micro_os_plus_semaphore_attributes_get_binary (void);

  /**
   * @}
   */

  /**
   * @name Semaphore Creation Functions
   * @{
   */

  /**
   * @brief Construct a statically allocated semaphore object instance.
   * @param [in] semaphore Pointer to semaphore object instance storage.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] attr Pointer to attributes (may be NULL).
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_semaphore_construct (
      micro_os_plus_semaphore_t* semaphore, const char* name,
      const micro_os_plus_semaphore_attributes_t* attr);

  /**
   * @brief Construct a statically allocated binary semaphore object instance.
   * @param [in] semaphore Pointer to semaphore object instance storage.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] initial_value Initial count value.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_semaphore_binary_construct (
      micro_os_plus_semaphore_t* semaphore, const char* name,
      const micro_os_plus_semaphore_count_t initial_value);

  /**
   * @brief Construct a statically allocated counting semaphore
   *  object instance.
   * @param [in] semaphore Pointer to semaphore object instance storage.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] max_value Maximum count value.
   * @param [in] initial_value Initial count value.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_semaphore_counting_construct (
      micro_os_plus_semaphore_t* semaphore, const char* name,
      const micro_os_plus_semaphore_count_t max_value,
      const micro_os_plus_semaphore_count_t initial_value);

  /**
   * @brief Destruct the statically allocated semaphore object instance.
   * @param [in] semaphore Pointer to semaphore object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_semaphore_destruct (micro_os_plus_semaphore_t* semaphore);

  /**
   * @brief Allocated a semaphore object instance and construct it.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] attr Pointer to attributes (may be NULL).
   * @return Pointer to new semaphore object instance.
   */
  micro_os_plus_semaphore_t*
  micro_os_plus_semaphore_new (
      const char* name, const micro_os_plus_semaphore_attributes_t* attr);

  /**
   * @brief Allocate a binary semaphore object instance and construct it.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] initial_value Initial count value.
   * @return Pointer to new semaphore object instance.
   */
  micro_os_plus_semaphore_t*
  micro_os_plus_semaphore_binary_new (
      const char* name, const micro_os_plus_semaphore_count_t initial_value);

  /**
   * @brief Allocate a counting semaphore object instance and construct it.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] max_value Maximum count value.
   * @param [in] initial_value Initial count value.
   * @return Pointer to new semaphore object instance.
   */
  micro_os_plus_semaphore_t*
  micro_os_plus_semaphore_counting_new (
      const char* name, const micro_os_plus_semaphore_count_t max_value,
      const micro_os_plus_semaphore_count_t initial_value);

  /**
   * @brief Destruct the semaphore object instance.
   * @param [in] semaphore Pointer to semaphore object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_semaphore_delete (micro_os_plus_semaphore_t* semaphore);

  /**
   * @}
   */

  /**
   * @name Semaphore Functions
   * @{
   */

  /**
   * @brief Get the semaphore name.
   * @param [in] semaphore Pointer to semaphore object instance.
   * @return Null terminated string.
   */
  const char*
  micro_os_plus_semaphore_get_name (micro_os_plus_semaphore_t* semaphore);

  /**
   * @brief Post (unlock) the semaphore.
   * @param [in] semaphore Pointer to semaphore object instance.
   * @retval micro_os_plus_ok The semaphore was posted.
   * @retval EAGAIN The maximum count value was exceeded.
   * @retval ENOTRECOVERABLE The semaphore could not be posted
   *  (extension to POSIX).
   */
  micro_os_plus_result_t
  micro_os_plus_semaphore_post (micro_os_plus_semaphore_t* semaphore);

  /**
   * @brief Lock the semaphore, possibly waiting.
   * @param [in] semaphore Pointer to semaphore object instance.
   * @retval micro_os_plus_ok The calling process successfully
   *  performed the semaphore lock operation.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval ENOTRECOVERABLE Semaphore wait failed (extension to POSIX).
   * @retval EDEADLK A deadlock condition was detected.
   * @retval EINTR The operation was interrupted.
   */
  micro_os_plus_result_t
  micro_os_plus_semaphore_wait (micro_os_plus_semaphore_t* semaphore);

  /**
   * @brief Try to lock the semaphore.
   * @param [in] semaphore Pointer to semaphore object instance.
   * @retval micro_os_plus_ok The calling process successfully
   *  performed the semaphore lock operation.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval EWOULDBLOCK The semaphore was already locked.
   * @retval ENOTRECOVERABLE Semaphore wait failed (extension to POSIX).
   * @retval EDEADLK A deadlock condition was detected.
   * @retval EINTR The operation was interrupted.
   */
  micro_os_plus_result_t
  micro_os_plus_semaphore_try_wait (micro_os_plus_semaphore_t* semaphore);

  /**
   * @brief Timed wait to lock the semaphore.
   * @param [in] semaphore Pointer to semaphore object instance.
   * @param [in] timeout Timeout to wait.
   * @retval micro_os_plus_ok The calling process successfully
   *  performed the semaphore lock operation.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval EINVAL Invalid timeout (POSIX limits the timeout
   *  to 1000 million ns)
   * @retval ETIMEDOUT The semaphore could not be locked before
   *  the specified timeout expired.
   * @retval ENOTRECOVERABLE Semaphore wait failed (extension to POSIX).
   * @retval EDEADLK A deadlock condition was detected.
   * @retval EINTR The operation was interrupted.
   */
  micro_os_plus_result_t
  micro_os_plus_semaphore_timed_wait (micro_os_plus_semaphore_t* semaphore,
                                      micro_os_plus_clock_duration_t timeout);

  /**
   * @brief Get the semaphore count value.
   * @param [in] semaphore Pointer to semaphore object instance.
   * @return The semaphore count value.
   */
  micro_os_plus_semaphore_count_t
  micro_os_plus_semaphore_get_value (micro_os_plus_semaphore_t* semaphore);

  /**
   * @brief Reset the semaphore.
   * @param [in] semaphore Pointer to semaphore object instance.
   * @retval micro_os_plus_ok The semaphore was reset.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   */
  micro_os_plus_result_t
  micro_os_plus_semaphore_reset (micro_os_plus_semaphore_t* semaphore);

  /**
   * @brief Get the semaphore initial count value.
   * @param [in] semaphore Pointer to semaphore object instance.
   * @return The numeric value set from attributes.
   */
  micro_os_plus_semaphore_count_t
  micro_os_plus_semaphore_get_initial_value (
      micro_os_plus_semaphore_t* semaphore);

  /**
   * @brief Get the semaphore maximum count value.
   * @param [in] semaphore Pointer to semaphore object instance.
   * @return The numeric value set from attributes.
   */
  micro_os_plus_semaphore_count_t
  micro_os_plus_semaphore_get_max_value (micro_os_plus_semaphore_t* semaphore);

  /**
   * @}
   */

  /**
   * @}
   */

  // --------------------------------------------------------------------------
  /**
   * @addtogroup micro-os-plus-rtos-c-mempool
   * @{
   */

  /**
   * @name Memory Pool Attributes Functions
   * @{
   */

  /**
   * @brief Initialise the memory pool attributes.
   * @param [in] attr Pointer to memory pool attributes object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_mempool_attributes_init (
      micro_os_plus_mempool_attributes_t* attr);

  /**
   * @}
   */

  /**
   * @name Memory Pool Creation Functions
   * @{
   */

  /**
   * @brief Construct a statically allocated memory pool object instance.
   * @param [in] mempool Pointer to memory pool object instance storage.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] blocks The maximum number of items in the pool.
   * @param [in] block_size_bytes The size of an item, in bytes.
   * @param [in] attr Pointer to attributes (may be NULL).
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_mempool_construct (
      micro_os_plus_mempool_t* mempool, const char* name, size_t blocks,
      size_t block_size_bytes, const micro_os_plus_mempool_attributes_t* attr);

  /**
   * @brief Destruct the statically allocated memory pool object instance.
   * @param [in] mempool Pointer to memory pool object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_mempool_destruct (micro_os_plus_mempool_t* mempool);

  /**
   * @brief Allocate a memory pool object instance and construct it.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] blocks The maximum number of items in the pool.
   * @param [in] block_size_bytes The size of an item, in bytes.
   * @param [in] attr Pointer to attributes (may be NULL).
   * @return Pointer to new memory pool object instance.
   */
  micro_os_plus_mempool_t*
  micro_os_plus_mempool_new (const char* name, size_t blocks,
                             size_t block_size_bytes,
                             const micro_os_plus_mempool_attributes_t* attr);

  /**
   * @brief Destruct the memory pool object instance and deallocate it.
   * @param [in] mempool Pointer to dynamically allocated memory pool
   *  object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_mempool_delete (micro_os_plus_mempool_t* mempool);

  /**
   * @}
   */

  /**
   * @name Memory Pool Functions
   * @{
   */

  /**
   * @brief Get the memory pool name.
   * @param [in] mempool Pointer to memory pool object instance.
   * @return Null terminated string.
   */
  const char*
  micro_os_plus_mempool_get_name (micro_os_plus_mempool_t* mempool);

  /**
   * @brief Allocate a memory block.
   * @param [in] mempool Pointer to memory pool object instance.
   * @return Pointer to memory block, or `NULL` if interrupted.
   */
  void*
  micro_os_plus_mempool_alloc (micro_os_plus_mempool_t* mempool);

  /**
   * @brief Try to allocate a memory block.
   * @param [in] mempool Pointer to memory pool object instance.
   * @return Pointer to memory block, or `NULL` if no memory available.
   */
  void*
  micro_os_plus_mempool_try_alloc (micro_os_plus_mempool_t* mempool);

  /**
   * @brief Allocate a memory block with timeout.
   * @param [in] mempool Pointer to memory pool object instance.
   * @param [in] timeout Timeout to wait, in clock units (ticks or seconds).
   * @return Pointer to memory block, or `NULL` if timeout.
   */
  void*
  micro_os_plus_mempool_timed_alloc (micro_os_plus_mempool_t* mempool,
                                     micro_os_plus_clock_duration_t timeout);

  /**
   * @brief Free the memory block.
   * @param [in] mempool Pointer to memory pool object instance.
   * @param [in] block Pointer to memory block to free.
   * @retval micro_os_plus_ok The memory block was released.
   * @retval EINVAL The block does not belong to the memory pool.
   */
  micro_os_plus_result_t
  micro_os_plus_mempool_free (micro_os_plus_mempool_t* mempool, void* block);

  /**
   * @brief Get memory pool capacity.
   * @param [in] mempool Pointer to memory pool object instance.
   * @return The max number of blocks in the pool.
   */
  size_t
  micro_os_plus_mempool_get_capacity (micro_os_plus_mempool_t* mempool);

  /**
   * @brief Get blocks count.
   * @param [in] mempool Pointer to memory pool object instance.
   * @return The number of blocks used from the queue.
   */
  size_t
  micro_os_plus_mempool_get_count (micro_os_plus_mempool_t* mempool);

  /**
   * @brief Get block size.
   * @param [in] mempool Pointer to memory pool object instance.
   * @return The block size, in bytes.
   */
  size_t
  micro_os_plus_mempool_get_block_size (micro_os_plus_mempool_t* mempool);

  /**
   * @brief Check if the memory pool is empty.
   * @param [in] mempool Pointer to memory pool object instance.
   * @retval true The memory pool has no allocated blocks.
   * @retval false The memory pool has allocated blocks.
   */
  bool
  micro_os_plus_mempool_is_empty (micro_os_plus_mempool_t* mempool);

  /**
   * @brief Check if the memory pool is full.
   * @param [in] mempool Pointer to memory pool object instance.
   * @retval true All memory blocks are allocated.
   * @retval false There are still memory blocks that can be allocated.
   */
  bool
  micro_os_plus_mempool_is_full (micro_os_plus_mempool_t* mempool);

  /**
   * @brief Reset the memory pool.
   * @param [in] mempool Pointer to memory pool object instance.
   * @retval micro_os_plus_ok The memory pool was reset.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   */
  micro_os_plus_result_t
  micro_os_plus_mempool_reset (micro_os_plus_mempool_t* mempool);

  /**
   * @brief Get the pool storage address.
   * @param [in] mempool Pointer to memory pool object instance.
   * @return Pointer to storage.
   */
  void*
  micro_os_plus_mempool_get_pool (micro_os_plus_mempool_t* mempool);

  /**
   * @}
   */

  /**
   * @}
   */

  // --------------------------------------------------------------------------
  /**
   * @addtogroup micro-os-plus-rtos-c-mqueue
   * @{
   */

  /**
   * @name Message Queue Attributes Functions
   * @{
   */

  /**
   * @brief Initialise the message queue attributes.
   * @param [in] attr Pointer to message queue attributes object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_mqueue_attributes_init (
      micro_os_plus_mqueue_attributes_t* attr);

  /**
   * @}
   */

  /**
   * @name Message Queue Creation Functions
   * @{
   */

  /**
   * @brief Construct a statically allocated message queue object instance.
   * @param [in] mqueue Pointer to message queue object instance storage.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] msgs The number of messages.
   * @param [in] msg_size_bytes The message size, in bytes.
   * @param [in] attr Pointer to attributes (may be NULL).
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_mqueue_construct (
      micro_os_plus_mqueue_t* mqueue, const char* name, size_t msgs,
      size_t msg_size_bytes, const micro_os_plus_mqueue_attributes_t* attr);

  /**
   * @brief Destruct the statically allocated message queue object instance.
   * @param [in] mqueue Pointer to message queue object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_mqueue_destruct (micro_os_plus_mqueue_t* mqueue);

  /**
   * @brief Allocate a message queue object instance and construct it.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] msgs The number of messages.
   * @param [in] msg_size_bytes The message size, in bytes.
   * @param [in] attr Pointer to attributes (may be NULL).
   * @return Pointer to new message queue object instance.
   */
  micro_os_plus_mqueue_t*
  micro_os_plus_mqueue_new (const char* name, size_t msgs,
                            size_t msg_size_bytes,
                            const micro_os_plus_mqueue_attributes_t* attr);

  /**
   * @brief Destruct the message queue object instance and deallocate it.
   * @param [in] mqueue Pointer to dynamically allocated message queue
   *  object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_mqueue_delete (micro_os_plus_mqueue_t* mqueue);

  /**
   * @}
   */

  /**
   * @name Message Queue Functions
   * @{
   */

  /**
   * @brief Get the message queue name.
   * @param [in] mqueue Pointer to message queue object instance.
   * @return Null terminated string.
   */
  const char*
  micro_os_plus_mqueue_get_name (micro_os_plus_mqueue_t* mqueue);

  /**
   * @brief Send a message to the queue.
   * @param [in] mqueue Pointer to message queue object instance.
   * @param [in] msg The address of the message to enqueue.
   * @param [in] nbytes The length of the message. Must be not
   *  higher than the value used when creating the queue.
   * @param [in] mprio The message priority. Enter 0 if priorities are not
   * used.
   * @retval micro_os_plus_ok The message was enqueued.
   * @retval EINVAL A parameter is invalid or outside of a permitted range.
   * @retval EMSGSIZE The specified message length, nbytes,
   *  exceeds the message size attribute of the message queue.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval ENOTRECOVERABLE The message could not be enqueue
   *  (extension to POSIX).
   * @retval EINTR The operation was interrupted.
   */
  micro_os_plus_result_t
  micro_os_plus_mqueue_send (micro_os_plus_mqueue_t* mqueue, const void* msg,
                             size_t nbytes,
                             micro_os_plus_mqueue_priority_t mprio);

  /**
   * @brief Try to send a message to the queue.
   * @param [in] mqueue Pointer to message queue object instance.
   * @param [in] msg The address of the message to enqueue.
   * @param [in] nbytes The length of the message. Must be not
   *  higher than the value used when creating the queue.
   * @param [in] mprio The message priority. Enter 0 if priorities are not
   * used.
   * @retval micro_os_plus_ok The message was enqueued.
   * @retval EWOULDBLOCK The specified message queue is full.
   * @retval EINVAL A parameter is invalid or outside of a permitted range.
   * @retval EMSGSIZE The specified message length, nbytes,
   *  exceeds the message size attribute of the message queue.
   * @retval ENOTRECOVERABLE The message could not be enqueue
   *  (extension to POSIX).
   */
  micro_os_plus_result_t
  micro_os_plus_mqueue_try_send (micro_os_plus_mqueue_t* mqueue,
                                 const void* msg, size_t nbytes,
                                 micro_os_plus_mqueue_priority_t mprio);

  /**
   * @brief Send a message to the queue with timeout.
   * @param [in] mqueue Pointer to message queue object instance.
   * @param [in] msg The address of the message to enqueue.
   * @param [in] nbytes The length of the message. Must be not
   *  higher than the value used when creating the queue.
   * @param [in] timeout The timeout duration.
   * @param [in] mprio The message priority. Enter 0 if priorities are not
   * used.
   * @retval micro_os_plus_ok The message was enqueued.
   * @retval EINVAL A parameter is invalid or outside of a permitted range.
   * @retval EMSGSIZE The specified message length, nbytes,
   *  exceeds the message size attribute of the message queue.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval ETIMEDOUT The timeout expired before the message
   *  could be added to the queue.
   * @retval ENOTRECOVERABLE The message could not be enqueue
   *  (extension to POSIX).
   * @retval EINTR The operation was interrupted.
   */
  micro_os_plus_result_t
  micro_os_plus_mqueue_timed_send (micro_os_plus_mqueue_t* mqueue,
                                   const void* msg, size_t nbytes,
                                   micro_os_plus_clock_duration_t timeout,
                                   micro_os_plus_mqueue_priority_t mprio);

  /**
   * @brief Receive a message from the queue.
   * @param [in] mqueue Pointer to message queue object instance.
   * @param [out] msg The address where to store the dequeued message.
   * @param [in] nbytes The size of the destination buffer. Must
   *  be lower than the value used when creating the queue.
   * @param [out] mprio The address where to store the message
   *  priority. Enter `NULL` if priorities are not used.
   * @retval micro_os_plus_ok The message was received.
   * @retval EINVAL A parameter is invalid or outside of a permitted range.
   * @retval EMSGSIZE The specified message length, nbytes, is
   *  greater than the message size attribute of the message queue.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval ENOTRECOVERABLE The message could not be dequeued
   *  (extension to POSIX).
   * @retval EBADMSG The implementation has detected a data corruption
   *  problem with the message.
   * @retval EINTR The operation was interrupted.
   */
  micro_os_plus_result_t
  micro_os_plus_mqueue_receive (micro_os_plus_mqueue_t* mqueue, void* msg,
                                size_t nbytes,
                                micro_os_plus_mqueue_priority_t* mprio);

  /**
   * @brief Try to receive a message from the queue.
   * @param [in] mqueue Pointer to message queue object instance.
   * @param [out] msg The address where to store the dequeued message.
   * @param [in] nbytes The size of the destination buffer. Must
   *  be lower than the value used when creating the queue.
   * @param [out] mprio The address where to store the message
   *  priority. Enter `NULL` if priorities are not used.
   * @retval micro_os_plus_ok The message was received.
   * @retval EINVAL A parameter is invalid or outside of a permitted range.
   * @retval EMSGSIZE The specified message length, nbytes, is
   *  greater than the message size attribute of the message queue.
   * @retval ENOTRECOVERABLE The message could not be dequeued
   *  (extension to POSIX).
   * @retval EBADMSG The implementation has detected a data corruption
   *  problem with the message.
   * @retval EWOULDBLOCK The specified message queue is empty.
   */
  micro_os_plus_result_t
  micro_os_plus_mqueue_try_receive (micro_os_plus_mqueue_t* mqueue, void* msg,
                                    size_t nbytes,
                                    micro_os_plus_mqueue_priority_t* mprio);

  /**
   * @brief Receive a message from the queue with timeout.
   * @param [in] mqueue Pointer to message queue object instance.
   * @param [out] msg The address where to store the dequeued message.
   * @param [in] nbytes The size of the destination buffer. Must
   *  be lower than the value used when creating the queue.
   * @param [in] timeout The timeout duration.
   * @param [out] mprio The address where to store the message
   *  priority. Enter `NULL` if priorities are not used.
   * @retval micro_os_plus_ok The message was received.
   * @retval EINVAL A parameter is invalid or outside of a permitted range.
   * @retval EMSGSIZE The specified message length, nbytes, is
   *  greater than the message size attribute of the message queue.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval ENOTRECOVERABLE The message could not be dequeued
   *  (extension to POSIX).
   * @retval EBADMSG The implementation has detected a data corruption
   *  problem with the message.
   * @retval EINTR The operation was interrupted.
   * @retval ETIMEDOUT No message arrived on the queue before the
   *  specified timeout expired.
   */
  micro_os_plus_result_t
  micro_os_plus_mqueue_timed_receive (micro_os_plus_mqueue_t* mqueue,
                                      void* msg, size_t nbytes,
                                      micro_os_plus_clock_duration_t timeout,
                                      micro_os_plus_mqueue_priority_t* mprio);

  /**
   * @brief Get queue capacity.
   * @param [in] mqueue Pointer to message queue object instance.
   * @return The max number of messages that can be queued.
   */
  size_t
  micro_os_plus_mqueue_get_capacity (micro_os_plus_mqueue_t* mqueue);

  /**
   * @brief Get queue length.
   * @param [in] mqueue Pointer to message queue object instance.
   * @return The number of messages in the queue.
   */
  size_t
  micro_os_plus_mqueue_get_length (micro_os_plus_mqueue_t* mqueue);

  /**
   * @brief Get message size.
   * @param [in] mqueue Pointer to message queue object instance.
   * @return The message size, in bytes.
   */
  size_t
  micro_os_plus_mqueue_get_msg_size (micro_os_plus_mqueue_t* mqueue);

  /**
   * @brief Check if the queue is empty.
   * @param [in] mqueue Pointer to message queue object instance.
   * @retval true The queue has no messages.
   * @retval false The queue has some messages.
   */
  bool
  micro_os_plus_mqueue_is_empty (micro_os_plus_mqueue_t* mqueue);

  /**
   * @brief Check if the queue is full.
   * @param [in] mqueue Pointer to message queue object instance.
   * @retval true The queue is full.
   * @retval false The queue is not full.
   */
  bool
  micro_os_plus_mqueue_is_full (micro_os_plus_mqueue_t* mqueue);

  /**
   * @brief Reset the message queue.
   * @param [in] mqueue Pointer to message queue object instance.
   * @retval micro_os_plus_ok The queue was reset.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   */
  micro_os_plus_result_t
  micro_os_plus_mqueue_reset (micro_os_plus_mqueue_t* mqueue);

  /**
   * @}
   */

  /**
   * @}
   */

  // --------------------------------------------------------------------------
  /**
   * @addtogroup micro-os-plus-rtos-c-evflag
   * @{
   */

  /**
   * @name Event Flags Attributes Functions
   * @{
   */

  /**
   * @brief Initialise the event flags attributes.
   * @param [in] attr Pointer to event flags attributes object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_event_flags_attributes_init (
      micro_os_plus_event_flags_attributes_t* attr);

  /**
   * @}
   */

  /**
   * @name Event Flags Creation Functions
   * @{
   */

  /**
   * @brief Construct a statically allocated event flags object instance.
   * @param [in] evflags Pointer to event flags object instance storage.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] attr Pointer to attributes (may be NULL).
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_event_flags_construct (
      micro_os_plus_event_flags_t* evflags, const char* name,
      const micro_os_plus_event_flags_attributes_t* attr);

  /**
   * @brief Destruct the statically allocated event flags object instance.
   * @param [in] evflags Pointer to event flags object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_event_flags_destruct (micro_os_plus_event_flags_t* evflags);

  /**
   * @brief Allocate an event flags object instance and construct it.
   * @param [in] name Pointer to name (may be NULL).
   * @param [in] attr Pointer to attributes (may be NULL).
   * @return Pointer to new event flags object instance.
   */
  micro_os_plus_event_flags_t*
  micro_os_plus_event_flags_new (
      const char* name, const micro_os_plus_event_flags_attributes_t* attr);

  /**
   * @brief Destruct the event flags object instance and deallocate it.
   * @param [in] evflags Pointer to dynamically allocated event flags
   *  object instance.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_event_flags_delete (micro_os_plus_event_flags_t* evflags);

  /**
   * @}
   */

  /**
   * @name Event Flags Functions
   * @{
   */

  /**
   * @brief Get the event flags name.
   * @param [in] evflags Pointer to event flags object instance.
   * @return Null terminated string.
   */
  const char*
  micro_os_plus_event_flags_get_name (micro_os_plus_event_flags_t* evflags);

  /**
   * @brief Wait for event flags.
   * @param [in] evflags Pointer to event flags object instance.
   * @param [in] mask The expected flags (OR-ed bit-mask);
   *  may be zero.
   * @param [out] oflags Pointer where to store the current flags;
   *  may be `NULL`.
   * @param [in] mode Mode bits to select if either all or any flags
   *  are expected, and if the flags should be cleared.
   * @retval micro_os_plus_ok All expected flags were raised.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval EINVAL The mask is outside of the permitted range.
   * @retval EINTR The operation was interrupted.
   * @retval ENOTRECOVERABLE Wait failed.
   */
  micro_os_plus_result_t
  micro_os_plus_event_flags_wait (micro_os_plus_event_flags_t* evflags,
                                  micro_os_plus_flags_mask_t mask,
                                  micro_os_plus_flags_mask_t* oflags,
                                  micro_os_plus_flags_mode_t mode);

  /**
   * @brief Try to wait for event flags.
   * @param [in] evflags Pointer to event flags object instance.
   * @param [in] mask The expected flags (OR-ed bit-mask);
   *  may be zero.
   * @param [out] oflags Pointer where to store the current flags;
   *  may be `NULL`.
   * @param [in] mode Mode bits to select if either all or any flags
   *  are expected, and if the flags should be cleared.
   * @retval micro_os_plus_ok All expected flags were raised.
   * @retval EINVAL The mask is outside of the permitted range.
   * @retval EWOULDBLOCK The expected condition did not occur.
   * @retval ENOTRECOVERABLE Wait failed.
   */
  micro_os_plus_result_t
  micro_os_plus_event_flags_try_wait (micro_os_plus_event_flags_t* evflags,
                                      micro_os_plus_flags_mask_t mask,
                                      micro_os_plus_flags_mask_t* oflags,
                                      micro_os_plus_flags_mode_t mode);

  /**
   * @brief Timed wait for event flags.
   * @param [in] evflags Pointer to event flags object instance.
   * @param [in] mask The expected flags (OR-ed bit-mask);
   *  may be zero.
   * @param [out] oflags Pointer where to store the current flags;
   *  may be `NULL`.
   * @param [in] mode Mode bits to select if either all or any flags
   *  are expected, and if the flags should be cleared.
   * @param [in] timeout Timeout to wait.
   * @retval micro_os_plus_ok All expected flags are raised.
   * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
   * @retval ETIMEDOUT The expected condition did not occur during the
   *  entire timeout duration.
   * @retval EINVAL The mask is outside of the permitted range.
   * @retval EINTR The operation was interrupted.
   * @retval ENOTRECOVERABLE Wait failed.
   */
  micro_os_plus_result_t
  micro_os_plus_event_flags_timed_wait (micro_os_plus_event_flags_t* evflags,
                                        micro_os_plus_flags_mask_t mask,
                                        micro_os_plus_clock_duration_t timeout,
                                        micro_os_plus_flags_mask_t* oflags,
                                        micro_os_plus_flags_mode_t mode);

  /**
   * @brief Raise event flags.
   * @param [in] evflags Pointer to event flags object instance.
   * @param [in] mask The OR-ed flags to raise.
   * @param [out] oflags Optional pointer where to store the
   *  new value of the flags; may be `NULL`.
   * @retval micro_os_plus_ok The flags were raised.
   * @retval EINVAL The mask is zero.
   * @retval ENOTRECOVERABLE Raise failed.
   */
  micro_os_plus_result_t
  micro_os_plus_event_flags_raise (micro_os_plus_event_flags_t* evflags,
                                   micro_os_plus_flags_mask_t mask,
                                   micro_os_plus_flags_mask_t* oflags);

  /**
   * @brief Clear event flags.
   * @param [in] evflags Pointer to event flags object instance.
   * @param [in] mask The OR-ed flags to clear. Zero means 'all'.
   * @param [out] oflags Optional pointer where to store the
   *  previous value of the flags; may be `NULL`.
   * @retval micro_os_plus_ok The flags were cleared.
   * @retval EINVAL The mask is zero.
   */
  micro_os_plus_result_t
  micro_os_plus_event_flags_clear (micro_os_plus_event_flags_t* evflags,
                                   micro_os_plus_flags_mask_t mask,
                                   micro_os_plus_flags_mask_t* oflags);

  /**
   * @brief Get (and possibly clear) event flags.
   * @param [in] evflags Pointer to event flags object instance.
   * @param [in] mask The OR-ed flags to get/clear; may be zero.
   * @param [in] mode Mode bits to select if the flags should be
   *  cleared (the other bits are ignored).
   * @return The selected bits from the flags mask.
   */
  micro_os_plus_flags_mask_t
  micro_os_plus_event_flags_get (micro_os_plus_event_flags_t* evflags,
                                 micro_os_plus_flags_mask_t mask,
                                 micro_os_plus_flags_mode_t mode);

  /**
   * @brief Check if there are threads waiting.
   * @param [in] evflags Pointer to event flags object instance.
   * @retval true There are threads waiting.
   * @retval false There are no threads waiting.
   */
  bool
  micro_os_plus_event_flags_are_waiting (micro_os_plus_event_flags_t* evflags);

  /**
   * @}
   */

  /**
   * @}
   */

  // --------------------------------------------------------------------------
  /**
   * @addtogroup micro-os-plus-rtos-c-memres
   * @{
   */

  /**
   * @name Memory Management Functions
   * @{
   */

  /**
   * @brief Get the application default memory resource (free store).
   * @return Pointer to memory resource object instance.
   */
  micro_os_plus_memory_t*
  micro_os_plus_memory_get_default (void);

  /**
   * @brief Allocate a block of memory.
   * @param memory Pointer to a memory resource object instance.
   * @param bytes Number of bytes to allocate.
   * @param alignment Integer (power of 2) with alignment constraints.
   */
  void*
  micro_os_plus_memory_allocate (micro_os_plus_memory_t* memory, size_t bytes,
                                 size_t alignment);

  /**
   * @brief Deallocate the previously allocated block of memory.
   * @param memory Pointer to a memory resource object instance.
   * @param addr Address of memory block to free.
   * @param bytes Number of bytes to deallocate (may be 0 if unknown).
   * @param alignment Integer (power of 2) with alignment constraints.
   */
  void
  micro_os_plus_memory_deallocate (micro_os_plus_memory_t* memory, void* addr,
                                   size_t bytes, size_t alignment);

  /**
   * @brief Reset the memory manager to the initial state.
   * @param memory Pointer to a memory resource object instance.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_memory_reset (micro_os_plus_memory_t* memory);

  /**
   * @brief Coalesce free blocks.
   * @param memory Pointer to a memory resource object instance.
   * @par Parameters
   *  None.
   * @retval true if the operation resulted in larger blocks.
   * @retval false if the operation was ineffective.
   */
  bool
  micro_os_plus_memory_coalesce (micro_os_plus_memory_t* memory);

  /**
   * @brief Get the total size of managed memory.
   * @param memory Pointer to a memory resource object instance.
   * @return Number of bytes.
   */
  size_t
  micro_os_plus_memory_get_total_bytes (micro_os_plus_memory_t* memory);

  /**
   * @brief Get the total size of allocated chunks.
   * @param memory Pointer to a memory resource object instance.
   * @return Number of bytes.
   */
  size_t
  micro_os_plus_memory_get_allocated_bytes (micro_os_plus_memory_t* memory);

  /**
   * @brief Get the total size of free chunks.
   * @param memory Pointer to a memory resource object instance.
   * @return Number of bytes.
   */
  size_t
  micro_os_plus_memory_get_free_bytes (micro_os_plus_memory_t* memory);

  /**
   * @brief Get the number of allocated chunks.
   * @param memory Pointer to a memory resource object instance.
   * @return Number of chunks.
   */
  size_t
  micro_os_plus_memory_get_allocated_chunks (micro_os_plus_memory_t* memory);

  /**
   * @brief Get the number of free chunks.
   * @param memory Pointer to a memory resource object instance.
   * @return Number of chunks.
   */
  size_t
  micro_os_plus_memory_get_free_chunks (micro_os_plus_memory_t* memory);

/**
 * @}
 */

/**
 * @}
 */

// --------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_RTOS_C_API_H_

// ----------------------------------------------------------------------------
