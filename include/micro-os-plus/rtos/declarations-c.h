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
 * The structures declared in this file are used both in the µOS++ C API
 * and in the CMSIS RTOS API.
 *
 * Since there is no method to automatically sync them with the C++
 * definitions, they must be manually adjusted to match them, otherwise
 * the validation checks in os-c-wrapper.cpp will fail.
 */

#ifndef MICRO_OS_PLUS_RTOS_DECLARATIONS_C_H_
#define MICRO_OS_PLUS_RTOS_DECLARATIONS_C_H_

// ----------------------------------------------------------------------------

#include <micro-os-plus/version.h>
#include <micro-os-plus/rtos/defines.h>

// ----------------------------------------------------------------------------

#if defined(HAVE_MICRO_OS_PLUS_CONFIG_H)
#include <micro-os-plus/config.h>
#endif // HAVE_MICRO_OS_PLUS_CONFIG_H

// ----------------------------------------------------------------------------

#include <micro-os-plus/rtos/port/declarations.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

  // ==========================================================================

  typedef struct micro_os_plus_internal_double_list_links_s
  {
    void* prev;
    void* next;
  } micro_os_plus_internal_double_list_links_t;

  typedef micro_os_plus_internal_double_list_links_t
      micro_os_plus_internal_threads_waiting_list_t;

  typedef struct micro_os_plus_internal_thread_children_list_s
  {
    micro_os_plus_internal_double_list_links_t links;
  } micro_os_plus_internal_thread_children_list_t;

  typedef struct micro_os_plus_internal_waiting_thread_node_s
  {
    micro_os_plus_internal_double_list_links_t links;
    void* thread;
  } micro_os_plus_internal_waiting_thread_node_t;

  typedef struct micro_os_plus_internal_clock_timestamps_list_s
  {
    micro_os_plus_internal_double_list_links_t links;
  } micro_os_plus_internal_clock_timestamps_list_t;

  /**
   * @addtogroup micro-os-plus-rtos-c-core
   * @{
   */

  /**
   * @brief Type of values returned by RTOS functions.
   *
   * @details
   * For error processing reasons, most µOS++ RTOS functions
   * return a numeric result, which, according to POSIX,
   * when the call was successful, must be `0`
   * (`micro_os_plus_ok`) or an error code defined in `<errno.h>` otherwise.
   *
   * @see micro_os_plus::rtos::result_t
   */
  typedef uint32_t micro_os_plus_result_t;

  /**
   * @brief Type of variables holding flags modes.
   *
   * @details
   * An unsigned type used to hold the mode bits passed to
   * functions returning flags.
   *
   * Both thread event flags and generic event flags use this definition.
   *
   * @see micro_os_plus::rtos::flags::mode_t
   */
  typedef uint32_t micro_os_plus_flags_mode_t;

  /**
   * @brief Type of variables holding flags masks.
   *
   * @details
   * An unsigned type large enough to store all the flags, usually
   * 32-bits wide.
   *
   * Both thread event flags and generic event flags use this definition.
   *
   * @see micro_os_plus::rtos::flags::mask_t
   */
  typedef uint32_t micro_os_plus_flags_mask_t;

  /**
   * @brief Bits used to specify the flags modes.
   *
   * @see micro_os_plus::rtos::flags::mode
   */
  enum
  {
    micro_os_plus_flags_mode_all = 1, //
    micro_os_plus_flags_mode_any = 2, //
    micro_os_plus_flags_mode_clear = 4, //
  };

  /**
   * @brief Special mask to represent any flag.
   */
#define micro_os_plus_flags_any 0

  /**
   * Special mask to represent all flags.
   */
#define micro_os_plus_flags_all 0xFFFFFFFF

  // --------------------------------------------------------------------------

  /**
   * @brief Type of variables holding scheduler state codes.
   *
   * @details
   * Usually a boolean telling if the scheduler is
   * locked or not, but for recursive locks it might also be a
   * numeric counter.
   *
   * @see micro_os_plus::rtos::scheduler::state_t
   */
  typedef micro_os_plus_port_scheduler_state_t micro_os_plus_scheduler_state_t;

  /**
   * @brief Type of variables holding interrupts priority values.
   *
   * @details
   * Usually an integer large enough to hold the CPU register
   * where the interrupt priorities are stored.
   *
   * Used to temporarily store the CPU register
   * during critical sections.
   *
   * @see micro_os_plus::rtos::interrupts::state_t
   */
  typedef micro_os_plus_port_interrupts_state_t
      micro_os_plus_interrupts_state_t;

  // --------------------------------------------------------------------------

  // Define clock types based on port definitions.

  /**
   * @brief Type of variables holding clock time stamps.
   *
   * @details
   * A numeric type intended to store a clock timestamp, either in ticks
   * cycles or seconds.
   *
   * @see micro_os_plus::rtos::clock::timestamp_t
   */
  typedef micro_os_plus_port_clock_timestamp_t micro_os_plus_clock_timestamp_t;

  /**
   * @brief Type of variables holding clock durations.
   *
   * @details
   * A numeric type intended to store a clock duration, either in ticks
   * cycles, or seconds.
   *
   * @see micro_os_plus::rtos::clock::duration_t
   */
  typedef micro_os_plus_port_clock_duration_t micro_os_plus_clock_duration_t;

  /**
   * @brief Type of variables holding clock offsets.
   *
   * @details
   * A numeric type intended to store a clock offset
   * (difference to epoch), either in ticks
   * or in seconds.
   *
   * @see micro_os_plus::rtos::clock::duration_t
   */
  typedef micro_os_plus_port_clock_offset_t micro_os_plus_clock_offset_t;

  // --------------------------------------------------------------------------

  /**
   * @brief Generic iterator, implemented as a pointer.
   *
   * @details
   * To simplify things, the C implementation of iterators
   * includes a single pointer to a C++ object instance. Internally,
   * the functions
   * used to iterate must cast this pointer properly, but this
   * should be transparent for the user.
   */
  typedef void* micro_os_plus_iterator_t;

  // --------------------------------------------------------------------------

  /**
   * @brief Type of variables holding context switches counters.
   *
   * @see micro_os_plus::rtos::statistics::counter_t
   */
  typedef uint64_t micro_os_plus_statistics_counter_t;

  /**
   * @brief Type of variables holding durations in CPU cycles.
   *
   * @see micro_os_plus::rtos::statistics::duration_t
   */
  typedef uint64_t micro_os_plus_statistics_duration_t;

  /**
   * @}
   */

  /**
   * @brief Internal event flags.
   *
   * @see micro_os_plus::rtos::internal::event_flags
   */
  typedef struct micro_os_plus_internal_event_flags_s
  {
    micro_os_plus_flags_mask_t flags_mask;
  } micro_os_plus_internal_event_flags_t;

  // ==========================================================================
#define MICRO_OS_PLUS_THREAD_PRIO_SHIFT (4)

  /**
   * @addtogroup micro-os-plus-rtos-c-thread
   * @{
   */

  /**
   * @brief Thread priorities; intermediate values are also possible.
   *
   * @see micro_os_plus::rtos::thread::state
   */
  enum
  {
    // Ordered, with **none** as the first and **error** as the last.
    micro_os_plus_thread_priority_none = 0, // not defined
    micro_os_plus_thread_priority_idle
    = (1 << MICRO_OS_PLUS_THREAD_PRIO_SHIFT),
    micro_os_plus_thread_priority_lowest
    = (2 << MICRO_OS_PLUS_THREAD_PRIO_SHIFT), // lowest
    micro_os_plus_thread_priority_low = (2 << MICRO_OS_PLUS_THREAD_PRIO_SHIFT),
    micro_os_plus_thread_priority_below_normal
    = (4 << MICRO_OS_PLUS_THREAD_PRIO_SHIFT),
    micro_os_plus_thread_priority_normal
    = (6 << MICRO_OS_PLUS_THREAD_PRIO_SHIFT), // default
    micro_os_plus_thread_priority_above_normal
    = (8 << MICRO_OS_PLUS_THREAD_PRIO_SHIFT),
    micro_os_plus_thread_priority_high
    = (10 << MICRO_OS_PLUS_THREAD_PRIO_SHIFT),
    micro_os_plus_thread_priority_realtime
    = (12 << MICRO_OS_PLUS_THREAD_PRIO_SHIFT),
    micro_os_plus_thread_priority_highest
    = (((13 + 1) << MICRO_OS_PLUS_THREAD_PRIO_SHIFT) - 1),
    micro_os_plus_thread_priority_isr
    = (((14 + 1) << MICRO_OS_PLUS_THREAD_PRIO_SHIFT) - 1),
    micro_os_plus_thread_priority_error
    = (((15 + 1) << MICRO_OS_PLUS_THREAD_PRIO_SHIFT) - 1)
  };

  /**
   * @brief An enumeration with all possible thread states.
   *
   * @see micro_os_plus::rtos::thread::state
   */
  enum
  {
    /**
     * @brief Used to catch uninitialised threads.
     */
    micro_os_plus_thread_state_undefined = 0,
    /**
     * @brief Present in the READY list and competing for CPU.
     */
    micro_os_plus_thread_state_ready = 1,
    /**
     * @brief Has the CPU and runs.
     */
    micro_os_plus_thread_state_running = 2,
    /**
     * @brief Not present in the READY list, waiting for an event.
     */
    micro_os_plus_thread_state_suspended = 3,
    /**
     * @brief No longer usable, but resources not yet released.
     */
    micro_os_plus_thread_state_terminated = 4,
    /**
     * @brief Terminated and resources (like stack) released.
     */
    micro_os_plus_thread_state_destroyed = 5
  };

  /**
   * @brief Type of thread function arguments.
   * @details
   * Useful to cast other similar types
   * to silence possible compiler warnings.
   *
   * @see micro_os_plus::rtos::thread::func_args_t
   */
  typedef void* micro_os_plus_thread_func_args_t;

  /**
   * @brief Type of thread function.
   *
   * @details
   * Useful to cast other similar types
   * to silence possible compiler warnings.
   *
   * @see micro_os_plus::rtos::thread::func_t
   */
  typedef void* (*micro_os_plus_thread_func_t) (
      micro_os_plus_thread_func_args_t args);

  /**
   * @brief Type of variables holding thread states.
   *
   * @see micro_os_plus::rtos::thread::state_t
   */
  typedef uint8_t micro_os_plus_thread_state_t;

  /**
   * @brief Type of variables holding thread priorities.
   *
   * @details
   * A numeric type used to hold thread priorities, affecting the thread
   * behaviour, like scheduling and thread wakeup due to events;
   * usually an unsigned 8-bits type.
   *
   * Higher values represent higher priorities.
   *
   * @see micro_os_plus::rtos::thread::priority_t
   */
  typedef uint8_t micro_os_plus_thread_priority_t;

  // --------------------------------------------------------------------------

  /**
   * @brief Type of variables holding stack words.
   *
   * @details
   * A numeric type intended to store a stack word
   * as stored by push instructions.
   *
   * @see micro_os_plus::rtos::stack::element_t
   */
  typedef micro_os_plus_port_thread_stack_element_t
      micro_os_plus_thread_stack_element_t;

  /**
   * @brief Type of variables holding aligned stack elements.
   *
   * @details
   * A numeric type intended to be used for stack allocations.
   *
   * @see micro_os_plus::rtos::stack::allocation_element_t
   */
  typedef micro_os_plus_port_thread_stack_allocation_element_t
      micro_os_plus_thread_stack_allocation_element_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  /**
   * @brief Thread stack.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * The members of this structure are hidden and should not
   * be accessed directly, but through associated functions.
   *
   * @see micro_os_plus::rtos::thread::stack
   */
  typedef struct micro_os_plus_thread_stack_s
  {
    /**
     * @cond ignore
     */

    void* stack_addr;
    size_t stack_size_bytes;

    /**
     * @endcond
     */

  } micro_os_plus_thread_stack_t;

  /**
   * @brief Thread context.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * The members of this structure are hidden and should not
   * be accessed directly, but through associated functions.
   *
   * @see micro_os_plus::rtos::thread::context
   */
  typedef struct micro_os_plus_thread_context_s
  {
    /**
     * @cond ignore
     */

    micro_os_plus_thread_stack_t stack;
#if !defined(MICRO_OS_PLUS_USE_RTOS_PORT_SCHEDULER)
    micro_os_plus_port_thread_context_t port;
#endif

    /**
     * @endcond
     */

  } micro_os_plus_thread_context_t;

#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) \
    || defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

  /**
   * @brief Thread statistics.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * The members of this structure are hidden and should not
   * be accessed directly, but through associated functions.
   *
   * @see micro_os_plus::rtos::thread::statistics
   */
  typedef struct micro_os_plus_thread_statistics_s
  {
    /**
     * @cond ignore
     */

#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)
    micro_os_plus_statistics_counter_t context_switches;
#endif // defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)
    micro_os_plus_statistics_duration_t cpu_cycles;
#endif // defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

    /**
     * @endcond
     */

  } micro_os_plus_thread_statistics_t;

#endif

  /**
   * @brief Thread attributes.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * Initialise this structure with `micro_os_plus_thread_attributes_init()`,
   * and then set any of the individual members directly.
   *
   * @see micro_os_plus::rtos::thread::attributes
   */
  typedef struct micro_os_plus_thread_attributes_s
  {
    /**
     * @brief Address of the clock to use for timeouts.
     *
     * @details
     * It may be `micro_os_plus_clock_get_sysclock()`,
     * `micro_os_plus_clock_get_rtclock()`, or any other user object derived
     * from class `clock`.
     *
     * If `NULL`, the default is `sysclock`.
     */
    void* clock;

    /**
     * @brief Address of the user defined storage for the thread stack.
     *
     * @details
     * If `NULL`, the default is to dynamically allocate the stack.
     */
    void* stack_address;

    /**
     * @brief Size of the user defined storage for the thread
     *  stack, in bytes.
     *
     * @details
     * If 0, the default is `micro_os_plus_thread_stack_get_default_size()`.
     *
     * A convenient and explicit variant to this attribute
     * is to call `micro_os_plus_thread_stack_set_default_size ()` just before
     * creating the thread. However mind setting this from different
     * threads at the same time.
     */
    size_t stack_size_bytes;

    /**
     * @brief Thread initial priority.
     *
     * @details
     * If 0, the default is `micro_os_plus_thread_priority_normal`.
     *
     * A convenient and explicit variant to this attribute
     * is to call `micro_os_plus_thread_set_priority()` at the beginning of the
     * thread function.
     */
    micro_os_plus_thread_priority_t priority;

  } micro_os_plus_thread_attributes_t;

  /**
   * @brief Thread object storage.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * This C structure has the same size as the C++
   * `micro_os_plus::rtos::thread` object and must be initialised with
   * `micro_os_plus_thread_create()`.
   *
   * Later on a pointer to it can be used both in C and C++
   * to refer to the thread object instance.
   *
   * The members of this structure are hidden and should not
   * be used directly, but only through specific functions.
   *
   * @see micro_os_plus::rtos::thread
   */
  typedef struct micro_os_plus_thread_s
  {
    /**
     * @cond ignore
     */

    void* vtbl;
    const char* name;
    int errno_; // Prevent the macro to expand (for example with a prefix).
    micro_os_plus_internal_waiting_thread_node_t ready_node;
    micro_os_plus_thread_func_t func;
    micro_os_plus_thread_func_args_t func_args;
    void* func_result_;
    void* parent;
    micro_os_plus_internal_double_list_links_t child_links;
    micro_os_plus_internal_thread_children_list_t children;
    micro_os_plus_internal_double_list_links_t mutexes;
    void* joiner;
    void* waiting_node;
    void* clock_node;
    void* clock;
    void* allocator;
    void* allocted_stack_address;
    size_t acquired_mutexes;
    size_t allocated_stack_size_elements;
    micro_os_plus_thread_state_t state;
    micro_os_plus_thread_priority_t priority_assigned;
    micro_os_plus_thread_priority_t priority_inherited;
    bool interrupted;
    micro_os_plus_internal_event_flags_t event_flags;
#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_CUSTOM_THREAD_USER_STORAGE)
    micro_os_plus_thread_user_storage_t user_storage; //
#endif // defined(MICRO_OS_PLUS_INCLUDE_RTOS_CUSTOM_THREAD_USER_STORAGE)

#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) \
    || defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)
    micro_os_plus_thread_statistics_t statistics;
#endif // defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

#if defined(MICRO_OS_PLUS_USE_RTOS_PORT_SCHEDULER)
    micro_os_plus_thread_port_data_t port;
#endif
    micro_os_plus_thread_context_t context;

    /**
     * @endcond
     */

  } micro_os_plus_thread_t;

#pragma GCC diagnostic pop

  /**
   * @}
   */

  // ==========================================================================
  /**
   * @addtogroup micro-os-plus-rtos-c-clock
   * @{
   */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  /**
   * @brief Clock object storage.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * This C structure has the same size as the C++ `micro_os_plus::rtos::clock`
   * object.
   *
   * The members of this structure are hidden and should not
   * be used directly, but only through specific functions.
   *
   * @see micro_os_plus::rtos::clock
   */
  typedef struct micro_os_plus_clock_s
  {
    /**
     * @cond ignore
     */

    void* vtbl;
    const char* name;
    micro_os_plus_internal_clock_timestamps_list_t steady_list;
    micro_os_plus_clock_duration_t sleep_count;
    micro_os_plus_clock_timestamp_t steady_count;

    /**
     * @endcond
     */

  } micro_os_plus_clock_t;

#pragma GCC diagnostic pop

  /**
   * @}
   */

  // ==========================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct micro_os_plus_clock_node_s
  {
    void* next;
    void* prev;
    void* list;
    micro_os_plus_clock_timestamp_t timestamp;
    void* timer;
  } micro_os_plus_internal_clock_timer_node_t;

#pragma GCC diagnostic pop

  /**
   * @addtogroup micro-os-plus-rtos-c-timer
   * @{
   */

  /**
   * @brief An enumeration with the timer types.
   */
  enum
  {
    micro_os_plus_timer_once = 0, //
    micro_os_plus_timer_periodic = 1 //
  };

  /**
   * @brief Type of timer function arguments.
   *
   * @details
   * Useful to cast other similar types
   * to silence possible compiler warnings.
   *
   * @see micro_os_plus::rtos::timer::func_args_t
   */
  typedef void* micro_os_plus_timer_func_args_t;

  /**
   * @brief Type of timer function.
   *
   * @details
   * Useful to cast other similar types
   * to silence possible compiler warnings.
   *
   * @see micro_os_plus::rtos::timer::func_t
   */
  typedef void (*micro_os_plus_timer_func_t) (
      micro_os_plus_timer_func_args_t args);

  /**
   * @brief Type of variables holding timer types.
   *
   * @see micro_os_plus::rtos::timer::type_t
   */
  typedef uint8_t micro_os_plus_timer_type_t;

  /**
   * @brief Type of variables holding timer states.
   *
   * @see micro_os_plus::rtos::timer::state_t
   */
  typedef uint8_t micro_os_plus_timer_state_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  /**
   * @brief Timer attributes.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * Initialise this structure with `micro_os_plus_timer_attributes_init()` and
   * then set any of the individual members directly.
   *
   * @see micro_os_plus::rtos::timer::attributes
   */
  typedef struct micro_os_plus_timer_attributes_s
  {
    /**
     * @brief Pointer to clock object instance.
     */
    void* clock;

    /**
     * @brief Timer type.
     */
    micro_os_plus_timer_type_t timer_type;

  } micro_os_plus_timer_attributes_t;

  /**
   * @brief Timer object storage.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * This C structure has the same size as the C++ `micro_os_plus::rtos::timer`
   * object and must be initialised with `micro_os_plus_timer_create()`.
   *
   * Later on a pointer to it can be used both in C and C++
   * to refer to the timer object instance.
   *
   * The members of this structure are hidden and should not
   * be used directly, but only through specific functions.
   *
   * @see micro_os_plus::rtos::timer
   */
  typedef struct micro_os_plus_timer_s
  {
    /**
     * @cond ignore
     */

    const char* name;
    micro_os_plus_timer_func_t func;
    micro_os_plus_timer_func_args_t func_args;
#if !defined(MICRO_OS_PLUS_USE_RTOS_PORT_TIMER)
    void* clock;
    micro_os_plus_internal_clock_timer_node_t clock_node;
    micro_os_plus_clock_duration_t period;
#endif
#if defined(MICRO_OS_PLUS_USE_RTOS_PORT_TIMER)
    micro_os_plus_timer_port_data_t port_;
#endif
    micro_os_plus_timer_type_t type;
    micro_os_plus_timer_state_t state;

    /**
     * @endcond
     */

  } micro_os_plus_timer_t;

#pragma GCC diagnostic pop

  /**
   * @}
   */

  // ==========================================================================
  typedef int16_t micro_os_plus_mutex_count_t;
  typedef uint8_t micro_os_plus_mutex_type_t;
  typedef uint8_t micro_os_plus_mutex_protocol_t;
  typedef uint8_t micro_os_plus_mutex_robustness_t;

  /**
   * @addtogroup micro-os-plus-rtos-c-mutex
   * @{
   */

  /**
   * @brief An enumeration with mutex protocols.
   *
   * @see micro_os_plus::rtos::mutex::protocol
   */
  enum
  {
    /**
     * @brief Priority and scheduling not affected by mutex ownership.
     */
    micro_os_plus_mutex_protocol_none = 0,

    /**
     * @brief Inherit priority from highest priority thread.
     */
    micro_os_plus_mutex_protocol_inherit = 1,

    /**
     * @brief Execute at the highest priority.
     */
    micro_os_plus_mutex_protocol_protect = 2,

    /**
     * @brief Default mutex protocol.
     */
    micro_os_plus_mutex_protocol_default
    = micro_os_plus_mutex_protocol_inherit,
  };

  /**
   * @brief An enumeration with mutex robustness.
   *
   * @see micro_os_plus::rtos::mutex::robustness
   */
  enum
  {
    /**
     * @brief Normal robustness.
     */
    micro_os_plus_mutex_robustness_stalled = 0,

    /**
     * @brief Enhanced robustness at thread termination.
     */
    micro_os_plus_mutex_robustness_robust = 1,

    /**
     * @brief Default mutex robustness.
     */
    micro_os_plus_mutex_robustness_default
    = micro_os_plus_mutex_robustness_stalled,
  };

  /**
   * @brief An enumeration with mutex types.
   *
   * @see micro_os_plus::rtos::mutex::type
   */
  enum
  {
    /**
     * @brief Normal mutex behaviour.
     */
    micro_os_plus_mutex_type_normal = 0,

    /**
     * @brief Check mutex behaviour.
     */
    micro_os_plus_mutex_type_errorcheck = 1,

    /**
     * @brief Recursive mutex behaviour.
     */
    micro_os_plus_mutex_type_recursive = 2,

    /**
     * @brief Default mutex type.
     */
    micro_os_plus_mutex_type_default = micro_os_plus_mutex_type_normal,
  };

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  /**
   * @brief Mutex attributes.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * Initialise this structure with `micro_os_plus_mutex_attributes_init()` and
   * then set any of the individual members directly.
   *
   * @see micro_os_plus::rtos::mutex::attributes
   */
  typedef struct micro_os_plus_mutex_attributes_s
  {
    /**
     * @brief Pointer to clock object instance.
     */
    void* clock;

    /**
     * @brief Mutex priority ceiling.
     */
    micro_os_plus_thread_priority_t priority_ceiling;

    /**
     * @brief Mutex protocol.
     */
    micro_os_plus_mutex_protocol_t protocol;

    /**
     * @brief Mutex robustness.
     */
    micro_os_plus_mutex_robustness_t robustness;

    /**
     * @brief Mutex type.
     */
    micro_os_plus_mutex_type_t type;

    /**
     * @brief Recursive mutex max count.
     */
    micro_os_plus_mutex_count_t max_count;

  } micro_os_plus_mutex_attributes_t;

  /**
   * @brief Mutex object storage.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * This C structure has the same size as the C++ `micro_os_plus::rtos::mutex`
   * object and must be initialised with `micro_os_plus_mutex_create()`.
   *
   * Later on a pointer to it can be used both in C and C++
   * to refer to the mutex object instance.
   *
   * The members of this structure are hidden and should not
   * be used directly, but only through specific functions.
   *
   * @see micro_os_plus::rtos::mutex
   */
  typedef struct micro_os_plus_mutex_s
  {
    /**
     * @cond ignore
     */

    const char* name;
    void* owner;
#if !defined(MICRO_OS_PLUS_USE_RTOS_PORT_MUTEX)
    micro_os_plus_internal_threads_waiting_list_t list;
    void* clock;
#endif
    micro_os_plus_internal_double_list_links_t owner_links;
#if defined(MICRO_OS_PLUS_USE_RTOS_PORT_MUTEX)
    micro_os_plus_mutex_port_data_t port;
#endif
    micro_os_plus_mutex_count_t count;
    micro_os_plus_thread_priority_t initial_priority_ceiling;
    micro_os_plus_thread_priority_t priority_ceiling;
    micro_os_plus_thread_priority_t bosted_priority;
    bool owner_dead;
    bool consistent;
    bool recoverable;
    micro_os_plus_mutex_type_t type;
    micro_os_plus_mutex_protocol_t protocol;
    micro_os_plus_mutex_robustness_t robustness;
    micro_os_plus_mutex_count_t max_count;

    /**
     * @endcond
     */

  } micro_os_plus_mutex_t;

#pragma GCC diagnostic pop

  /**
   * @}
   */

  // ==========================================================================
  /**
   * @addtogroup micro-os-plus-rtos-c-condvar
   * @{
   */

  /**
   * @brief Condition variable attributes.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * Initialise this structure with
   * `micro_os_plus_condition_variable_attributes_init()` and then set any of
   * the individual members directly.
   *
   * @see micro_os_plus::rtos::condition_variable::attributes
   */
  typedef struct micro_os_plus_condition_variable_attributes_s
  {
    /**
     * @brief Pointer to clock object instance.
     */
    void* clock;

  } micro_os_plus_condition_variable_attributes_t;

  /**
   * @brief Condition variable object storage.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * This C structure has the same size as the C++
   * @ref micro_os_plus::rtos::condition_variable
   * object and must be initialised with
   * micro_os_plus_condition_variable_create().
   *
   * Later on a pointer to it can be used both in C and C++
   * to refer to the timer object instance.
   *
   * The members of this structure are hidden and should not
   * be used directly, but only through specific functions.
   *
   * @see micro_os_plus::rtos::condition_variable
   */
  typedef struct micro_os_plus_condition_variable_s
  {
    /**
     * @cond ignore
     */

    const char* name;
#if !defined(MICRO_OS_PLUS_USE_RTOS_PORT_CONDITION_VARIABLE)
    micro_os_plus_internal_threads_waiting_list_t list;
    // void* clock;
#endif

    /**
     * @endcond
     */

  } micro_os_plus_condition_variable_t;

  /**
   * @}
   */

  // ==========================================================================
  /**
   * @addtogroup micro-os-plus-rtos-c-semaphore
   * @{
   */

  /**
   * @brief Type of variables holding semaphore counts.
   *
   * @see micro_os_plus::rtos::semaphore::count_t
   */
  typedef int16_t micro_os_plus_semaphore_count_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  /**
   * @brief Semaphore attributes.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * Initialise this structure with `micro_os_plus_semaphore_attributes_init()`
   * and then set any of the individual members directly.
   *
   * @see micro_os_plus::rtos::semaphore::attributes
   */
  typedef struct micro_os_plus_semaphore_attributes_s
  {
    /**
     * @brief Pointer to clock object instance.
     */
    void* clock;

    /**
     * @brief Semaphore max count value.
     */
    micro_os_plus_semaphore_count_t max_value;

    /**
     * @brief Semaphore initial count value.
     */
    micro_os_plus_semaphore_count_t initial_value;

  } micro_os_plus_semaphore_attributes_t;

  /**
   * @brief Semaphore object storage.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * This C structure has the same size as the C++
   * `micro_os_plus::rtos::semaphore` object and must be initialised with
   * `micro_os_plus_semaphore_create()`.
   *
   * Later on a pointer to it can be used both in C and C++
   * to refer to the semaphore object instance.
   *
   * The members of this structure are hidden and should not
   * be used directly, but only through specific functions.
   *
   * @see micro_os_plus::rtos::semaphore
   */
  typedef struct micro_os_plus_semaphore_s
  {
    /**
     * @cond ignore
     */

    const char* name;
#if !defined(MICRO_OS_PLUS_USE_RTOS_PORT_SEMAPHORE)
    micro_os_plus_internal_threads_waiting_list_t list;
    void* clock;
#endif
#if defined(MICRO_OS_PLUS_USE_RTOS_PORT_SEMAPHORE)
    micro_os_plus_semaphore_port_data_t port;
#endif
    micro_os_plus_semaphore_count_t initial_count;
    micro_os_plus_semaphore_count_t count;
    micro_os_plus_semaphore_count_t max_count;

    /**
     * @endcond
     */

  } micro_os_plus_semaphore_t;

#pragma GCC diagnostic pop

  /**
   * @}
   */

  // ==========================================================================
  typedef uint16_t micro_os_plus_memory_pool_size_t;

  /**
   * @addtogroup micro-os-plus-rtos-c-mempool
   * @{
   */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  /**
   * @brief Memory pool attributes.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * Initialise this structure with
   * `micro_os_plus_memory_pool_attributes_init()` and then set any of the
   * individual members directly.
   *
   * @see micro_os_plus::rtos::memory_pool::attributes
   */
  typedef struct micro_os_plus_memory_pool_attributes_s
  {
    /**
     * @brief Pointer to clock object instance.
     */
    void* clock;

    /**
     * @brief Pointer to user provided memory pool area.
     */
    void* arena_address;

    /**
     * @brief Size of user provided memory pool area, in bytes.
     */
    size_t arena_size_bytes;

  } micro_os_plus_memory_pool_attributes_t;

  /**
   * @brief Memory pool object storage.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * This C structure has the same size as the C++
   * `micro_os_plus::rtos::memory_pool` object and must be initialised with
   * `micro_os_plus_memory_pool_create()`.
   *
   * Later on a pointer to it can be used both in C and C++
   * to refer to the memory pool object instance.
   *
   * The members of this structure are hidden and should not
   * be used directly, but only through specific functions.
   *
   * @see micro_os_plus::rtos::memory_pool
   */
  typedef struct micro_os_plus_memory_pool_s
  {
    /**
     * @cond ignore
     */

    void* vtbl;
    const char* name;
#if !defined(MICRO_OS_PLUS_USE_RTOS_PORT_MEMORY_POOL)
    micro_os_plus_internal_threads_waiting_list_t list;
    void* clock;
#endif
    void* pool_arena_address;
    void* allocated_pool_arena_address;
    void* allocator;
#if defined(MICRO_OS_PLUS_USE_RTOS_PORT_MEMORY_POOL)
    micro_os_plus_memory_pool_port_data_t port;
#endif
    size_t pool_arena_size_bytes;
    size_t allocated_pool_size_elements_;
    micro_os_plus_memory_pool_size_t blocks;
    micro_os_plus_memory_pool_size_t block_size_bytes;
    micro_os_plus_memory_pool_size_t count;
    void* first;

    /**
     * @endcond
     */

  } micro_os_plus_memory_pool_t;

#pragma GCC diagnostic pop

  /**
   * @}
   */

  // ==========================================================================
#if defined(MICRO_OS_PLUS_BOOL_RTOS_MESSAGE_QUEUE_SIZE_16BITS)
  typedef uint16_t micro_os_plus_message_queue_size_t;
#else
typedef uint8_t micro_os_plus_message_queue_size_t;
#endif

  typedef uint16_t micro_os_plus_message_queue_message_size_t;
  typedef micro_os_plus_message_queue_size_t
      micro_os_plus_message_queue_index_t;

  /**
   * @addtogroup micro-os-plus-rtos-c-mqueue
   * @{
   */

  /**
   * @brief Type of variables holding message queue priorities.
   *
   * @see micro_os_plus::rtos::message_queue::priority_t
   */
  typedef uint8_t micro_os_plus_message_queue_priority_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  /**
   * @brief Message queue attributes.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * Initialise this structure with
   * `micro_os_plus_message_queue_attributes_init()` and then set any of the
   * individual members directly.
   *
   * @see micro_os_plus::rtos::message_queue::attributes
   */
  typedef struct micro_os_plus_message_queue_attributes_s
  {
    /**
     * @brief Pointer to clock object instance.
     */
    void* clock;

    /**
     * @brief Pointer to user provided message queue area.
     */
    void* arena_address;

    /**
     * @brief Size of user provided message queue area, in bytes.
     */
    size_t arena_size_bytes;

  } micro_os_plus_message_queue_attributes_t;

  /**
   * @brief Message queue object storage.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * This C structure has the same size as the C++
   * `micro_os_plus::rtos::message_queue` object and must be initialised with
   * `micro_os_plus_message_queue_create()`.
   *
   * Later on a pointer to it can be used both in C and C++
   * to refer to the message queue object instance.
   *
   * The members of this structure are hidden and should not
   * be used directly, but only through specific functions.
   *
   * @see micro_os_plus::rtos::message_queue
   */
  typedef struct micro_os_plus_message_queue_s
  {
    /**
     * @cond ignore
     */

    void* vtbl;
    const char* name;
#if !defined(MICRO_OS_PLUS_USE_RTOS_PORT_MESSAGE_QUEUE)
    micro_os_plus_internal_threads_waiting_list_t send_list;
    micro_os_plus_internal_threads_waiting_list_t receive_list;
    void* clock;
    micro_os_plus_message_queue_index_t* prev_array;
    micro_os_plus_message_queue_index_t* next_array;
    micro_os_plus_message_queue_priority_t* priority_array;
    void* first_free;
#endif

    void* arena_address;
    void* allocated_arena_address;
    void* allocator;

#if defined(MICRO_OS_PLUS_USE_RTOS_PORT_MESSAGE_QUEUE)
    micro_os_plus_message_queue_port_data_t port;
#endif

    size_t arena_size_bytes;
    size_t allocated_arena_size_elements;

    micro_os_plus_message_queue_message_size_t msg_size_bytes;
    micro_os_plus_message_queue_size_t msgs;

    micro_os_plus_message_queue_size_t count;
#if !defined(MICRO_OS_PLUS_USE_RTOS_PORT_MESSAGE_QUEUE)
    micro_os_plus_message_queue_index_t head;
#endif

    /**
     * @endcond
     */

  } micro_os_plus_message_queue_t;

#pragma GCC diagnostic pop

  /**
   * @}
   */

  // ==========================================================================
  /**
   * @addtogroup micro-os-plus-rtos-c-evflag
   * @{
   */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  /**
   * @brief Event flags attributes.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * Initialise this structure with
   * `micro_os_plus_event_flags_attributes_init()` and then set any of the
   * individual members directly.
   *
   * @see micro_os_plus::rtos::event_flags::attributes
   */
  typedef struct micro_os_plus_event_flags_attributes_s
  {
    /**
     * @brief Pointer to clock object instance.
     */
    void* clock;

  } micro_os_plus_event_flags_attributes_t;

  /**
   * @brief Event flags object storage.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * This C structure has the same size as the C++
   * `micro_os_plus::rtos::event_flags` object and must be initialised with
   * `micro_os_plus_event_flags_create()`.
   *
   * Later on a pointer to it can be used both in C and C++
   * to refer to the event flags object instance.
   *
   * The members of this structure are hidden and should not
   * be used directly, but only through specific functions.
   *
   * @see micro_os_plus::rtos::event_flags
   */
  typedef struct micro_os_plus_event_flags_s
  {
    /**
     * @cond ignore
     */

    const char* name;
#if !defined(MICRO_OS_PLUS_USE_RTOS_PORT_EVENT_FLAGS)
    micro_os_plus_internal_threads_waiting_list_t list;
    void* clock;
#endif

#if defined(MICRO_OS_PLUS_USE_RTOS_PORT_EVENT_FLAGS)
    micro_os_plus_event_flags_port_data_t port_;
#endif

    micro_os_plus_internal_event_flags_t flags;

    /**
     * @endcond
     */

  } micro_os_plus_event_flags_t;

#pragma GCC diagnostic pop

  /**
   * @}
   */

  // ==========================================================================
  /**
   * @addtogroup micro-os-plus-rtos-c-clock
   * @{
   */

  /**
   * @name Clock handlers
   * @{
   */

  /**
   * @brief SysTick interrupt handler.
   */
  void
  micro_os_plus_systick_handler (void);

  /**
   * @brief RTC interrupt handler.
   */
  void
  micro_os_plus_rtc_handler (void);

  /**
   * @}
   */

  /**
   * @}
   */

  // ==========================================================================
  /**
   * @addtogroup micro-os-plus-rtos-c-memres
   * @{
   */

  /**
   * @brief Memory resource object storage.
   * @headerfile c-api.h <micro-os-plus/rtos/c-api.h>
   *
   * @details
   * A pointer to this structure can be used as a pointer to the
   * `micro_os_plus::rtos::memory::memory_resource` object.
   *
   * The members of this structure are hidden and should not
   * be used directly, but only through specific functions.
   *
   * @see micro_os_plus::rtos::memory::memory_resource
   */

  typedef struct micro_os_plus_memory_s
  {
    char dummy; // Content is not relevant.
  } micro_os_plus_memory_t;

/**
 * @}
 */

// ============================================================================
#ifdef __cplusplus
}
#endif

#endif // MICRO_OS_PLUS_RTOS_DECLARATIONS_C_H_

// ----------------------------------------------------------------------------
