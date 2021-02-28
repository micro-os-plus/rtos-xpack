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
 * The code provides an implementation of the C API for the µOS++ RTOS.
 */

#include <micro-os-plus/rtos.h>
#include <micro-os-plus/rtos/c-api.h>

// ----------------------------------------------------------------------------

using namespace micro_os_plus;
using namespace micro_os_plus::rtos;

// ----------------------------------------------------------------------------

// Validate C typedefs sizes & alignment.
static_assert (sizeof (micro_os_plus_result_t) == sizeof (result_t),
               "adjust size of micro_os_plus_result_t");
static_assert (alignof (micro_os_plus_result_t) == alignof (result_t),
               "adjust align of micro_os_plus_result_t");

static_assert (sizeof (micro_os_plus_flags_mode_t) == sizeof (flags::mode_t),
               "adjust size of micro_os_plus_flags_mode_t");
static_assert (alignof (micro_os_plus_flags_mode_t) == alignof (flags::mode_t),
               "adjust align of micro_os_plus_flags_mode_t");

static_assert (sizeof (micro_os_plus_flags_mask_t) == sizeof (flags::mask_t),
               "adjust size of micro_os_plus_flags_mask_t");
static_assert (alignof (micro_os_plus_flags_mask_t) == alignof (flags::mask_t),
               "adjust align of micro_os_plus_flags_mask_t");

static_assert (sizeof (micro_os_plus_scheduler_state_t)
                   == sizeof (scheduler::state_t),
               "adjust size of micro_os_plus_scheduler_state_t");
static_assert (alignof (micro_os_plus_scheduler_state_t)
                   == alignof (scheduler::state_t),
               "adjust align of micro_os_plus_scheduler_state_t");

static_assert (sizeof (micro_os_plus_interrupts_state_t)
                   == sizeof (interrupts::state_t),
               "adjust size of micro_os_plus_interrupts_state_t");
static_assert (alignof (micro_os_plus_interrupts_state_t)
                   == alignof (interrupts::state_t),
               "adjust align of micro_os_plus_interrupts_state_t");

static_assert (sizeof (micro_os_plus_clock_timestamp_t)
                   == sizeof (clock::timestamp_t),
               "adjust size of micro_os_plus_port_clock_timestamp_t");
static_assert (alignof (micro_os_plus_clock_timestamp_t)
                   == alignof (clock::timestamp_t),
               "adjust align of micro_os_plus_port_clock_timestamp_t");

static_assert (sizeof (micro_os_plus_clock_duration_t)
                   == sizeof (clock::duration_t),
               "adjust size of micro_os_plus_port_clock_duration_t");
static_assert (alignof (micro_os_plus_clock_duration_t)
                   == alignof (clock::duration_t),
               "adjust align of micro_os_plus_port_clock_duration_t");

static_assert (sizeof (micro_os_plus_clock_offset_t)
                   == sizeof (clock::offset_t),
               "adjust size of micro_os_plus_port_clock_offset_t");
static_assert (alignof (micro_os_plus_clock_offset_t)
                   == alignof (clock::offset_t),
               "adjust align of micro_os_plus_port_clock_offset_t");

static_assert (sizeof (micro_os_plus_statistics_counter_t)
                   == sizeof (statistics::counter_t),
               "adjust size of micro_os_plus_statistics_counter_t");
static_assert (alignof (micro_os_plus_statistics_counter_t)
                   == alignof (statistics::counter_t),
               "adjust align of micro_os_plus_statistics_counter_t");

static_assert (sizeof (micro_os_plus_statistics_duration_t)
                   == sizeof (statistics::duration_t),
               "adjust size of micro_os_plus_statistics_duration_t");
static_assert (alignof (micro_os_plus_statistics_duration_t)
                   == alignof (statistics::duration_t),
               "adjust align of micro_os_plus_statistics_duration_t");

static_assert (sizeof (micro_os_plus_thread_func_args_t)
                   == sizeof (thread::func_args_t),
               "adjust size of micro_os_plus_thread_func_args_t");
static_assert (alignof (micro_os_plus_thread_func_args_t)
                   == alignof (thread::func_args_t),
               "adjust align of micro_os_plus_thread_func_args_t");

static_assert (sizeof (micro_os_plus_thread_func_t) == sizeof (thread::func_t),
               "adjust size of micro_os_plus_thread_func_t");
static_assert (alignof (micro_os_plus_thread_func_t)
                   == alignof (thread::func_t),
               "adjust align of micro_os_plus_thread_func_t");

static_assert (sizeof (micro_os_plus_thread_state_t)
                   == sizeof (thread::state_t),
               "adjust size of micro_os_plus_thread_state_t");
static_assert (alignof (micro_os_plus_thread_state_t)
                   == alignof (thread::state_t),
               "adjust align of micro_os_plus_thread_state_t");

static_assert (sizeof (micro_os_plus_thread_priority_t)
                   == sizeof (thread::priority_t),
               "adjust size of micro_os_plus_thread_priority_t");
static_assert (alignof (micro_os_plus_thread_priority_t)
                   == alignof (thread::priority_t),
               "adjust align of micro_os_plus_thread_priority_t");

static_assert (sizeof (micro_os_plus_timer_func_args_t)
                   == sizeof (timer::func_args_t),
               "adjust size of micro_os_plus_timer_func_args_t");
static_assert (alignof (micro_os_plus_timer_func_args_t)
                   == alignof (timer::func_args_t),
               "adjust align of micro_os_plus_timer_func_args_t");

static_assert (sizeof (micro_os_plus_timer_func_t) == sizeof (timer::func_t),
               "adjust size of micro_os_plus_timer_func_t");
static_assert (alignof (micro_os_plus_timer_func_t) == alignof (timer::func_t),
               "adjust align of micro_os_plus_timer_func_t");

static_assert (sizeof (micro_os_plus_timer_type_t) == sizeof (timer::type_t),
               "adjust size of micro_os_plus_timer_type_t");
static_assert (alignof (micro_os_plus_timer_type_t) == alignof (timer::type_t),
               "adjust align of micro_os_plus_timer_type_t");

static_assert (sizeof (micro_os_plus_timer_state_t) == sizeof (timer::state_t),
               "adjust size of micro_os_plus_timer_state_t");
static_assert (alignof (micro_os_plus_timer_state_t)
                   == alignof (timer::state_t),
               "adjust align of micro_os_plus_timer_state_t");

static_assert (sizeof (micro_os_plus_mutex_count_t) == sizeof (mutex::count_t),
               "adjust size of micro_os_plus_mutex_count_t");
static_assert (alignof (micro_os_plus_mutex_count_t)
                   == alignof (mutex::count_t),
               "adjust align of micro_os_plus_mutex_count_t");

static_assert (sizeof (micro_os_plus_mutex_type_t) == sizeof (mutex::type_t),
               "adjust size of micro_os_plus_mutex_type_t");
static_assert (alignof (micro_os_plus_mutex_type_t) == alignof (mutex::type_t),
               "adjust align of micro_os_plus_mutex_type_t");

static_assert (sizeof (micro_os_plus_mutex_protocol_t)
                   == sizeof (mutex::protocol_t),
               "adjust size of micro_os_plus_mutex_protocol_t");
static_assert (alignof (micro_os_plus_mutex_protocol_t)
                   == alignof (mutex::protocol_t),
               "adjust align of micro_os_plus_mutex_protocol_t");

static_assert (sizeof (micro_os_plus_mutex_robustness_t)
                   == sizeof (mutex::robustness_t),
               "adjust size of micro_os_plus_mutex_robustness_t");
static_assert (alignof (micro_os_plus_mutex_robustness_t)
                   == alignof (mutex::robustness_t),
               "adjust align of micro_os_plus_mutex_robustness_t");

static_assert (sizeof (micro_os_plus_semaphore_count_t)
                   == sizeof (semaphore::count_t),
               "adjust size of micro_os_plus_semaphore_count_t");
static_assert (alignof (micro_os_plus_semaphore_count_t)
                   == alignof (semaphore::count_t),
               "adjust align of micro_os_plus_semaphore_count_t");

static_assert (sizeof (micro_os_plus_mempool_size_t)
                   == sizeof (memory_pool::size_t),
               "adjust size of micro_os_plus_mempool_size_t");
static_assert (alignof (micro_os_plus_mempool_size_t)
                   == alignof (memory_pool::size_t),
               "adjust align of micro_os_plus_mempool_size_t");

static_assert (sizeof (micro_os_plus_mqueue_size_t)
                   == sizeof (message_queue::size_t),
               "adjust size of micro_os_plus_mqueue_size_t");
static_assert (alignof (micro_os_plus_mqueue_size_t)
                   == alignof (message_queue::size_t),
               "adjust align of micro_os_plus_mqueue_size_t");

static_assert (sizeof (micro_os_plus_mqueue_msg_size_t)
                   == sizeof (message_queue::msg_size_t),
               "adjust size of micro_os_plus_mqueue_msg_size_t");
static_assert (alignof (micro_os_plus_mqueue_msg_size_t)
                   == alignof (message_queue::msg_size_t),
               "adjust align of micro_os_plus_mqueue_msg_size_t");

static_assert (sizeof (micro_os_plus_mqueue_index_t)
                   == sizeof (message_queue::index_t),
               "adjust size of micro_os_plus_mqueue_index_t");
static_assert (alignof (micro_os_plus_mqueue_index_t)
                   == alignof (message_queue::index_t),
               "adjust align of micro_os_plus_mqueue_index_t");

static_assert (sizeof (micro_os_plus_mqueue_priority_t)
                   == sizeof (message_queue::priority_t),
               "adjust size of micro_os_plus_mqueue_priority_t");
static_assert (alignof (micro_os_plus_mqueue_priority_t)
                   == alignof (message_queue::priority_t),
               "adjust align of micro_os_plus_mqueue_priority_t");

// ----------------------------------------------------------------------------

// Validate C enumeration values

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wenum-compare"

static_assert (micro_os_plus_thread_priority_idle == thread::priority::idle,
               "adjust micro_os_plus_thread_priority_idle");
static_assert (micro_os_plus_thread_priority_low == thread::priority::low,
               "adjust micro_os_plus_thread_priority_low");
static_assert (micro_os_plus_thread_priority_below_normal
                   == thread::priority::below_normal,
               "adjust micro_os_plus_thread_priority_below_normal");
static_assert (micro_os_plus_thread_priority_normal
                   == thread::priority::normal,
               "adjust micro_os_plus_thread_priority_normal");
static_assert (micro_os_plus_thread_priority_above_normal
                   == thread::priority::above_normal,
               "adjust micro_os_plus_thread_priority_above_normal");
static_assert (micro_os_plus_thread_priority_high == thread::priority::high,
               "adjust micro_os_plus_thread_priority_high");
static_assert (micro_os_plus_thread_priority_realtime
                   == thread::priority::realtime,
               "adjust micro_os_plus_thread_priority_realtime");
static_assert (micro_os_plus_thread_priority_error == thread::priority::error,
               "adjust micro_os_plus_thread_priority_error");

static_assert (micro_os_plus_flags_mode_all == flags::mode::all,
               "adjust micro_os_plus_flags_mode_all");
static_assert (micro_os_plus_flags_mode_any == flags::mode::any,
               "adjust micro_os_plus_flags_mode_any");
static_assert (micro_os_plus_flags_mode_clear == flags::mode::clear,
               "adjust micro_os_plus_flags_mode_clear");

static_assert (micro_os_plus_thread_state_undefined
                   == thread::state::undefined,
               "adjust micro_os_plus_thread_state_undefined");
static_assert (micro_os_plus_thread_state_ready == thread::state::ready,
               "adjust micro_os_plus_thread_state_ready");
static_assert (micro_os_plus_thread_state_running == thread::state::running,
               "adjust micro_os_plus_thread_state_running");
static_assert (micro_os_plus_thread_state_suspended
                   == thread::state::suspended,
               "adjust micro_os_plus_thread_state_suspended");
static_assert (micro_os_plus_thread_state_terminated
                   == thread::state::terminated,
               "adjust micro_os_plus_thread_state_terminated");
static_assert (micro_os_plus_thread_state_destroyed
                   == thread::state::destroyed,
               "adjust micro_os_plus_thread_state_destroyed");

static_assert (micro_os_plus_timer_once == timer::run::once,
               "adjust micro_os_plus_timer_once");
static_assert (micro_os_plus_timer_periodic == timer::run::periodic,
               "adjust micro_os_plus_timer_periodic");

static_assert (micro_os_plus_mutex_protocol_none == mutex::protocol::none,
               "adjust micro_os_plus_mutex_protocol_none");
static_assert (micro_os_plus_mutex_protocol_inherit
                   == mutex::protocol::inherit,
               "adjust micro_os_plus_mutex_protocol_inherit");
static_assert (micro_os_plus_mutex_protocol_protect
                   == mutex::protocol::protect,
               "adjust micro_os_plus_mutex_protocol_protect");

static_assert (micro_os_plus_mutex_robustness_stalled
                   == mutex::robustness::stalled,
               "adjust micro_os_plus_mutex_robustness_stalled");
static_assert (micro_os_plus_mutex_robustness_robust
                   == mutex::robustness::robust,
               "adjust micro_os_plus_mutex_robustness_robust");

static_assert (micro_os_plus_mutex_type_normal == mutex::type::normal,
               "adjust micro_os_plus_mutex_type_normal");
static_assert (micro_os_plus_mutex_type_errorcheck == mutex::type::errorcheck,
               "adjust micro_os_plus_mutex_type_errorcheck");
static_assert (micro_os_plus_mutex_type_recursive == mutex::type::recursive,
               "adjust micro_os_plus_mutex_type_recursive");
static_assert (micro_os_plus_mutex_type_default == mutex::type::default_,
               "adjust micro_os_plus_mutex_type_default");

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

// Validate C structs sizes (should match the C++ objects sizes).
// Validate offset of individual members (if needed, validate member size).

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"

static_assert (sizeof (rtos::clock) == sizeof (micro_os_plus_clock_t),
               "adjust micro_os_plus_clock_t size");

static_assert (sizeof (rtos::thread) == sizeof (micro_os_plus_thread_t),
               "adjust micro_os_plus_thread_t size");
static_assert (sizeof (rtos::thread::attributes)
                   == sizeof (micro_os_plus_thread_attributes_t),
               "adjust micro_os_plus_thread_attributes_t size");
static_assert (offsetof (rtos::thread::attributes, th_stack_address)
                   == offsetof (micro_os_plus_thread_attributes_t,
                                th_stack_address),
               "adjust micro_os_plus_thread_attributes_t members");
static_assert (offsetof (rtos::thread::attributes, th_stack_size_bytes)
                   == offsetof (micro_os_plus_thread_attributes_t,
                                th_stack_size_bytes),
               "adjust micro_os_plus_thread_attributes_t members");
static_assert (offsetof (rtos::thread::attributes, th_priority)
                   == offsetof (micro_os_plus_thread_attributes_t,
                                th_priority),
               "adjust micro_os_plus_thread_attributes_t members");

static_assert (sizeof (rtos::timer) == sizeof (micro_os_plus_timer_t),
               "adjust size of micro_os_plus_timer_t");
static_assert (sizeof (rtos::timer::attributes)
                   == sizeof (micro_os_plus_timer_attributes_t),
               "adjust size of micro_os_plus_timer_attributes_t");
static_assert (offsetof (rtos::timer::attributes, tm_type)
                   == offsetof (micro_os_plus_timer_attributes_t, tm_type),
               "adjust micro_os_plus_timer_attributes_t members");

static_assert (sizeof (rtos::mutex) == sizeof (micro_os_plus_mutex_t),
               "adjust size of micro_os_plus_mutex_t");
static_assert (sizeof (rtos::mutex::attributes)
                   == sizeof (micro_os_plus_mutex_attributes_t),
               "adjust size of micro_os_plus_mutex_attributes_t");
static_assert (offsetof (rtos::mutex::attributes, mx_priority_ceiling)
                   == offsetof (micro_os_plus_mutex_attributes_t,
                                mx_priority_ceiling),
               "adjust micro_os_plus_mutex_attributes_t members");
static_assert (offsetof (rtos::mutex::attributes, mx_protocol)
                   == offsetof (micro_os_plus_mutex_attributes_t, mx_protocol),
               "adjust micro_os_plus_mutex_attributes_t members");
static_assert (offsetof (rtos::mutex::attributes, mx_robustness)
                   == offsetof (micro_os_plus_mutex_attributes_t,
                                mx_robustness),
               "adjust micro_os_plus_mutex_attributes_t members");
static_assert (offsetof (rtos::mutex::attributes, mx_type)
                   == offsetof (micro_os_plus_mutex_attributes_t, mx_type),
               "adjust micro_os_plus_mutex_attributes_t members");
static_assert (offsetof (rtos::mutex::attributes, mx_max_count)
                   == offsetof (micro_os_plus_mutex_attributes_t,
                                mx_max_count),
               "adjust micro_os_plus_mutex_attributes_t members");

static_assert (sizeof (rtos::condition_variable)
                   == sizeof (micro_os_plus_condition_variable_t),
               "adjust size of micro_os_plus_condition_variable_t");
static_assert (sizeof (rtos::condition_variable::attributes)
                   == sizeof (micro_os_plus_condition_variable_attributes_t),
               "adjust size of micro_os_plus_condition_variable_attributes_t");

static_assert (sizeof (rtos::semaphore) == sizeof (micro_os_plus_semaphore_t),
               "adjust size of micro_os_plus_semaphore_t");
static_assert (sizeof (rtos::semaphore::attributes)
                   == sizeof (micro_os_plus_semaphore_attributes_t),
               "adjust size of micro_os_plus_semaphore_attributes_t");
static_assert (offsetof (rtos::semaphore::attributes, semaphore_initial_value)
                   == offsetof (micro_os_plus_semaphore_attributes_t,
                                semaphore_initial_value),
               "adjust micro_os_plus_semaphore_attributes_t members");
static_assert (offsetof (rtos::semaphore::attributes, semaphore_max_value)
                   == offsetof (micro_os_plus_semaphore_attributes_t,
                                semaphore_max_value),
               "adjust micro_os_plus_semaphore_attributes_t members");

static_assert (sizeof (rtos::memory_pool) == sizeof (micro_os_plus_mempool_t),
               "adjust size of micro_os_plus_mempool_t");
static_assert (sizeof (rtos::memory_pool::attributes)
                   == sizeof (micro_os_plus_mempool_attributes_t),
               "adjust size of micro_os_plus_mempool_attributes_t");
static_assert (offsetof (rtos::memory_pool::attributes, mp_pool_address)
                   == offsetof (micro_os_plus_mempool_attributes_t,
                                mp_pool_address),
               "adjust micro_os_plus_mempool_attributes_t members");
static_assert (offsetof (rtos::memory_pool::attributes, mp_pool_size_bytes)
                   == offsetof (micro_os_plus_mempool_attributes_t,
                                mp_pool_size_bytes),
               "adjust micro_os_plus_mempool_attributes_t members");

static_assert (sizeof (rtos::message_queue) == sizeof (micro_os_plus_mqueue_t),
               "adjust size of micro_os_plus_mqueue_t");
static_assert (sizeof (rtos::message_queue::attributes)
                   == sizeof (micro_os_plus_mqueue_attributes_t),
               "adjust size of micro_os_plus_mqueue_attributes_t");
static_assert (offsetof (rtos::message_queue::attributes, mq_queue_address)
                   == offsetof (micro_os_plus_mqueue_attributes_t,
                                mq_queue_addr),
               "adjust micro_os_plus_mqueue_attributes_t members");
static_assert (offsetof (rtos::message_queue::attributes, mq_queue_size_bytes)
                   == offsetof (micro_os_plus_mqueue_attributes_t,
                                mq_queue_size_bytes),
               "adjust micro_os_plus_mqueue_attributes_t members");

static_assert (sizeof (rtos::event_flags)
                   == sizeof (micro_os_plus_event_flags_t),
               "adjust size of micro_os_plus_event_flags_t");
static_assert (sizeof (rtos::event_flags::attributes)
                   == sizeof (micro_os_plus_event_flags_attributes_t),
               "adjust size of micro_os_plus_event_flags_attributes_t");

static_assert (sizeof (class thread::stack)
                   == sizeof (micro_os_plus_thread_stack_t),
               "adjust size of micro_os_plus_thread_stack_t");
static_assert (sizeof (class thread::context)
                   == sizeof (micro_os_plus_thread_context_t),
               "adjust size of micro_os_plus_thread_context_t");

#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) \
    || defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)
static_assert (sizeof (class thread::statistics)
                   == sizeof (micro_os_plus_thread_statistics_t),
               "adjust size of micro_os_plus_thread_statistics_t");

static_assert (sizeof (internal::timer_node)
                   == sizeof (micro_os_plus_internal_clock_timer_node_t),
               "adjust size of micro_os_plus_internal_clock_timer_node_t");

#pragma GCC diagnostic pop
#endif
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::scheduler::initialize()
 */
micro_os_plus_result_t
micro_os_plus_scheduler_initialize (void)
{
  return (micro_os_plus_result_t)scheduler::initialize ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::scheduler::start()
 */
void
micro_os_plus_scheduler_start (void)
{
  scheduler::start ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::scheduler::started()
 */
bool
micro_os_plus_scheduler_is_started (void)
{
  return scheduler::started ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::scheduler::lock()
 */
micro_os_plus_scheduler_state_t
micro_os_plus_scheduler_lock (void)
{
  return scheduler::lock ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::scheduler::unlock()
 */
micro_os_plus_scheduler_state_t
micro_os_plus_scheduler_unlock (void)
{
  return scheduler::unlock ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::scheduler::lock()
 */
micro_os_plus_scheduler_state_t
micro_os_plus_scheduler_set_locked (micro_os_plus_scheduler_state_t state)
{
  return scheduler::locked (state);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::scheduler::locked()
 */
bool
micro_os_plus_scheduler_is_locked (void)
{
  return scheduler::locked ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::scheduler::preemptive()
 */
bool
micro_os_plus_scheduler_is_preemptive (void)
{
  return scheduler::preemptive ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::scheduler::preemptive(bool)
 */
bool
micro_os_plus_scheduler_set_preemptive (bool state)
{
  return scheduler::preemptive (state);
}

#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::scheduler::statistics::context_switches()
 */
micro_os_plus_statistics_counter_t
micro_os_plus_scheduler_stat_get_context_switches (void)
{
  return static_cast<micro_os_plus_statistics_counter_t> (
      scheduler::statistics::context_switches ());
}

#endif // defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::scheduler::statistics::cpu_cycles()
 */
micro_os_plus_statistics_duration_t
micro_os_plus_scheduler_stat_get_cpu_cycles (void)
{
  return static_cast<micro_os_plus_statistics_duration_t> (
      scheduler::statistics::cpu_cycles ());
}

#endif // defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::interrupts::in_handler_mode()
 */
bool
micro_os_plus_interrupts_in_handler_mode (void)
{
  return interrupts::in_handler_mode ();
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::interrupts::critical_section::enter()
 */
micro_os_plus_interrupts_state_t
micro_os_plus_interrupts_critical_enter (void)
{
  return interrupts::critical_section::enter ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::interrupts::critical_section::exit()
 */
void
micro_os_plus_interrupts_critical_exit (micro_os_plus_interrupts_state_t state)
{
  interrupts::critical_section::exit (state);
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::interrupts::uncritical_section::enter()
 */
micro_os_plus_interrupts_state_t
micro_os_plus_interrupts_uncritical_enter (void)
{
  return interrupts::uncritical_section::enter ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::interrupts::uncritical_section::exit()
 */
void
micro_os_plus_interrupts_uncritical_exit (
    micro_os_plus_interrupts_state_t state)
{
  interrupts::uncritical_section::exit (state);
}

#if defined(MICRO_OS_PLUS_HAS_INTERRUPTS_STACK) || defined(__DOXYGEN__)

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::interrupts::stack()
 */

micro_os_plus_thread_stack_t*
micro_os_plus_interrupts_get_stack (void)
{
  return reinterpret_cast<micro_os_plus_thread_stack_t*> (
      rtos::interrupts::stack ());
}

#endif

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::this_thread::thread()
 */
micro_os_plus_thread_t*
micro_os_plus_this_thread (void)
{
  return (micro_os_plus_thread_t*)&this_thread::thread ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::this_thread::suspend()
 */
void
micro_os_plus_this_thread_suspend (void)
{
  return this_thread::suspend ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::this_thread::exit()
 */
void
micro_os_plus_this_thread_exit (void* exit_ptr)
{
  this_thread::exit (exit_ptr);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::this_thread::flags_wait()
 */
micro_os_plus_result_t
micro_os_plus_this_thread_flags_wait (micro_os_plus_flags_mask_t mask,
                                      micro_os_plus_flags_mask_t* oflags,
                                      micro_os_plus_flags_mode_t mode)
{
  return (micro_os_plus_result_t)this_thread::flags_wait (mask, oflags, mode);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::this_thread::flags_try_wait()
 */
micro_os_plus_result_t
micro_os_plus_this_thread_flags_try_wait (micro_os_plus_flags_mask_t mask,
                                          micro_os_plus_flags_mask_t* oflags,
                                          micro_os_plus_flags_mode_t mode)
{
  return (micro_os_plus_result_t)this_thread::flags_try_wait (mask, oflags,
                                                              mode);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::this_thread::flags_timed_wait()
 */
micro_os_plus_result_t
micro_os_plus_this_thread_flags_timed_wait (
    micro_os_plus_flags_mask_t mask, micro_os_plus_clock_duration_t timeout,
    micro_os_plus_flags_mask_t* oflags, micro_os_plus_flags_mode_t mode)
{
  return (micro_os_plus_result_t)this_thread::flags_timed_wait (mask, timeout,
                                                                oflags, mode);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::this_thread::flags_clear()
 */
micro_os_plus_result_t
micro_os_plus_this_thread_flags_clear (micro_os_plus_flags_mask_t mask,
                                       micro_os_plus_flags_mask_t* oflags)
{
  return (micro_os_plus_result_t)this_thread::flags_clear (mask, oflags);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::this_thread::flags_get()
 */
micro_os_plus_flags_mask_t
micro_os_plus_this_thread_flags_get (micro_os_plus_flags_mask_t mask,
                                     micro_os_plus_flags_mode_t mode)
{
  return (micro_os_plus_flags_mask_t)this_thread::flags_get (mask, mode);
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::attributes
 */
void
micro_os_plus_thread_attributes_init (micro_os_plus_thread_attributes_t* attr)
{
  assert (attr != nullptr);
  new (attr) thread::attributes ();
}

/**
 * @details
 *
 * @note Must be paired with `micro_os_plus_thread_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread
 */
void
micro_os_plus_thread_construct (micro_os_plus_thread_t* thread,
                                const char* name,
                                micro_os_plus_thread_func_t func,
                                const micro_os_plus_thread_func_args_t args,
                                const micro_os_plus_thread_attributes_t* attr)
{
  assert (thread != nullptr);
  if (attr == nullptr)
    {
      attr = (const micro_os_plus_thread_attributes_t*)&thread::initializer;
    }
  new (thread)
      rtos::thread (name, (thread::func_t)func, (thread::func_args_t)args,
                    (const thread::attributes&)*attr);
}

/**
 * @details
 *
 * @note Must be paired with `micro_os_plus_thread_construct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread
 */
void
micro_os_plus_thread_destruct (micro_os_plus_thread_t* thread)
{
  assert (thread != nullptr);
  (reinterpret_cast<rtos::thread&> (*thread)).~thread ();
}

/**
 * @details
 *
 * Dynamically allocate the thread object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new thread(...)`.
 * @note Must be paired with `micro_os_plus_thread_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread
 */
micro_os_plus_thread_t*
micro_os_plus_thread_new (const char* name, micro_os_plus_thread_func_t func,
                          const micro_os_plus_thread_func_args_t args,
                          const micro_os_plus_thread_attributes_t* attr)
{
  if (attr == nullptr)
    {
      attr = (const micro_os_plus_thread_attributes_t*)&thread::initializer;
    }
  return reinterpret_cast<micro_os_plus_thread_t*> (
      new rtos::thread (name, (thread::func_t)func, (thread::func_args_t)args,
                        (const thread::attributes&)*attr));
}

/**
 * @details
 *
 * Destruct the thread and deallocate the dynamically allocated
 * space using the RTOS system allocator.
 *
 * @note Equivalent of C++ `delete ptr_th`.
 * @note Must be paired with `micro_os_plus_thread_new()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread
 */
void
micro_os_plus_thread_delete (micro_os_plus_thread_t* thread)
{
  assert (thread != nullptr);
  delete reinterpret_cast<rtos::thread*> (thread);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::name()
 */
const char*
micro_os_plus_thread_get_name (micro_os_plus_thread_t* thread)
{
  assert (thread != nullptr);
  return (reinterpret_cast<rtos::thread&> (*thread)).name ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::priority()
 */
micro_os_plus_thread_priority_t
micro_os_plus_thread_get_priority (micro_os_plus_thread_t* thread)
{
  assert (thread != nullptr);
  return (micro_os_plus_thread_priority_t) (
             reinterpret_cast<rtos::thread&> (*thread))
      .priority ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::priority(priority_t)
 */
micro_os_plus_result_t
micro_os_plus_thread_set_priority (micro_os_plus_thread_t* thread,
                                   micro_os_plus_thread_priority_t prio)
{
  assert (thread != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<rtos::thread&> (*thread))
      .priority (prio);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::join()
 */
micro_os_plus_result_t
micro_os_plus_thread_join (micro_os_plus_thread_t* thread, void** exit_ptr)
{
  assert (thread != nullptr);
  return (micro_os_plus_result_t) reinterpret_cast<rtos::thread&> (*thread)
      .join (exit_ptr);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::resume()
 */
void
micro_os_plus_thread_resume (micro_os_plus_thread_t* thread)
{
  assert (thread != nullptr);
  return (reinterpret_cast<rtos::thread&> (*thread)).resume ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::flags_raise()
 */
micro_os_plus_result_t
micro_os_plus_thread_flags_raise (micro_os_plus_thread_t* thread,
                                  micro_os_plus_flags_mask_t mask,
                                  micro_os_plus_flags_mask_t* oflags)
{
  assert (thread != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<rtos::thread&> (*thread))
      .flags_raise (mask, oflags);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::state()
 */
micro_os_plus_thread_state_t
micro_os_plus_thread_get_state (micro_os_plus_thread_t* thread)
{
  assert (thread != nullptr);
  return reinterpret_cast<micro_os_plus_thread_state_t> (
      (reinterpret_cast<rtos::thread&> (*thread)).state ());
}

#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_CUSTOM_THREAD_USER_STORAGE) \
    || defined(__DOXYGEN__)

/**
 * @details
 *
 * @note
 *  Available only when `MICRO_OS_PLUS_INCLUDE_RTOS_CUSTOM_THREAD_USER_STORAGE`
 *  is defined.
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::user_storage()
 */
micro_os_plus_thread_user_storage_t*
micro_os_plus_thread_get_user_storage (micro_os_plus_thread_t* thread)
{
  assert (thread != nullptr);
  return (reinterpret_cast<rtos::thread&> (*thread)).user_storage ();
}

#endif // defined(MICRO_OS_PLUS_INCLUDE_RTOS_CUSTOM_THREAD_USER_STORAGE)

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::stack()
 */
micro_os_plus_thread_stack_t*
micro_os_plus_thread_get_stack (micro_os_plus_thread_t* thread)
{
  assert (thread != nullptr);
  return reinterpret_cast<micro_os_plus_thread_stack_t*> (
      &(reinterpret_cast<rtos::thread&> (*thread)).stack ());
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::stack::default_size()
 */
size_t
micro_os_plus_thread_stack_get_default_size (void)
{
  return thread::stack::default_size ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::stack::default_size(std::size_t)
 */
size_t
micro_os_plus_thread_stack_set_default_size (size_t size_bytes)
{
  return thread::stack::default_size (size_bytes);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::stack::min_size()
 */
size_t
micro_os_plus_thread_stack_get_min_size (void)
{
  return thread::stack::min_size ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::stack::min_size(std::size_t)
 */
size_t
micro_os_plus_thread_stack_set_min_size (size_t size_bytes)
{
  return thread::stack::min_size (size_bytes);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::stack::bottom()
 */
micro_os_plus_thread_stack_element_t*
micro_os_plus_thread_stack_get_bottom (micro_os_plus_thread_stack_t* stack)
{
  assert (stack != nullptr);
  return (reinterpret_cast<class rtos::thread::stack&> (*stack)).bottom ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::stack::top()
 */
micro_os_plus_thread_stack_element_t*
micro_os_plus_thread_stack_get_top (micro_os_plus_thread_stack_t* stack)
{
  assert (stack != nullptr);
  return (reinterpret_cast<class rtos::thread::stack&> (*stack)).top ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::stack::size()
 */
size_t
micro_os_plus_thread_stack_get_size (micro_os_plus_thread_stack_t* stack)
{
  assert (stack != nullptr);
  return (reinterpret_cast<class rtos::thread::stack&> (*stack)).size ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::stack::available()
 */
size_t
micro_os_plus_thread_stack_get_available (micro_os_plus_thread_stack_t* stack)
{
  assert (stack != nullptr);
  return (reinterpret_cast<class rtos::thread::stack&> (*stack)).available ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::stack::check_bottom_magic()
 */
bool
micro_os_plus_thread_stack_check_bottom_magic (
    micro_os_plus_thread_stack_t* stack)
{
  assert (stack != nullptr);
  return (reinterpret_cast<class rtos::thread::stack&> (*stack))
      .check_bottom_magic ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::stack::check_top_magic()
 */
bool
micro_os_plus_thread_stack_check_top_magic (
    micro_os_plus_thread_stack_t* stack)
{
  assert (stack != nullptr);
  return (reinterpret_cast<class rtos::thread::stack&> (*stack))
      .check_top_magic ();
}

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::statistics::context_switches()
 */
micro_os_plus_statistics_counter_t
micro_os_plus_thread_stat_get_context_switches (micro_os_plus_thread_t* thread)
{
  assert (thread != nullptr);
  return static_cast<micro_os_plus_statistics_counter_t> (
      (reinterpret_cast<rtos::thread&> (*thread))
          .statistics ()
          .context_switches ());
}

#endif // defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

#if defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::thread::statistics::cpu_cycles()
 */
micro_os_plus_statistics_duration_t
micro_os_plus_thread_stat_get_cpu_cycles (micro_os_plus_thread_t* thread)
{
  assert (thread != nullptr);
  return static_cast<micro_os_plus_statistics_duration_t> (
      (reinterpret_cast<rtos::thread&> (*thread)).statistics ().cpu_cycles ());
}

#endif // defined(MICRO_OS_PLUS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

// ----------------------------------------------------------------------------

/**
 * @details
 * Return the position of the first element in the list of
 * children threads of the given thread. If NULL, the position
 * first element in the list of top threads is returned.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
micro_os_plus_iterator_t
micro_os_plus_children_threads_iter_begin (micro_os_plus_thread_t* thread)
{
  return reinterpret_cast<micro_os_plus_iterator_t> (
      scheduler::children_threads (reinterpret_cast<rtos::thread*> (thread))
          .begin ()
          .get_iterator_pointer ());
}

/**
 * @details
 * Return the position after the last element in the list of
 * children threads of the given thread. If NULL, the position
 * after the last element in the list of top threads is returned.
 *
 * The iteration loop must be terminated when the current
 * iterator is equal to this position, before entering the loop body.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
micro_os_plus_iterator_t
micro_os_plus_children_threads_iter_end (micro_os_plus_thread_t* thread)
{
  return reinterpret_cast<micro_os_plus_iterator_t> (
      scheduler::children_threads (reinterpret_cast<rtos::thread*> (thread))
          .end ()
          .get_iterator_pointer ());
}

/**
 * @details
 * Calling it when the iterator is equal to the end position leads to
 * unpredictable results.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
micro_os_plus_thread_t*
micro_os_plus_children_threads_iter_get (micro_os_plus_iterator_t iterator)
{
  // Construct a local iterator object based on the pointer.
  thread::threads_list::iterator it{
    reinterpret_cast<utils::double_list_links*> (iterator)
  };
  return reinterpret_cast<micro_os_plus_thread_t*> (&(*it));
}

/**
 * @details
 * Calling it when the iterator is equal to the end position leads to
 * unpredictable results.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
micro_os_plus_iterator_t
micro_os_plus_children_threads_iter_next (micro_os_plus_iterator_t iterator)
{
  thread::threads_list::iterator it{
    reinterpret_cast<utils::double_list_links*> (iterator)
  };
  ++it;

  return reinterpret_cast<micro_os_plus_iterator_t> (
      it.get_iterator_pointer ());
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::clock::name()
 */
const char*
micro_os_plus_clock_get_name (micro_os_plus_clock_t* clock)
{
  assert (clock != nullptr);
  return (reinterpret_cast<rtos::clock&> (*clock)).name ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::clock::now()
 */
micro_os_plus_clock_timestamp_t
micro_os_plus_clock_now (micro_os_plus_clock_t* clock)
{
  assert (clock != nullptr);
  return (micro_os_plus_clock_timestamp_t) (
             reinterpret_cast<rtos::clock&> (*clock))
      .now ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::clock::steady_now()
 */
micro_os_plus_clock_timestamp_t
micro_os_plus_clock_steady_now (micro_os_plus_clock_t* clock)
{
  assert (clock != nullptr);
  return (micro_os_plus_clock_timestamp_t) (
             reinterpret_cast<rtos::clock&> (*clock))
      .steady_now ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::clock::sleep_for()
 */
micro_os_plus_result_t
micro_os_plus_clock_sleep_for (micro_os_plus_clock_t* clock,
                               micro_os_plus_clock_duration_t duration)
{
  assert (clock != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<rtos::clock&> (*clock))
      .sleep_for (duration);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::clock::sleep_until()
 */
micro_os_plus_result_t
micro_os_plus_clock_sleep_until (micro_os_plus_clock_t* clock,
                                 micro_os_plus_clock_timestamp_t timestamp)
{
  assert (clock != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<rtos::clock&> (*clock))
      .sleep_until (timestamp);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::clock::wait_for()
 */
micro_os_plus_result_t
micro_os_plus_clock_wait_for (micro_os_plus_clock_t* clock,
                              micro_os_plus_clock_duration_t timeout)
{
  assert (clock != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<rtos::clock&> (*clock))
      .wait_for (timeout);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::adjustable_clock::offset()
 */
micro_os_plus_clock_offset_t
micro_os_plus_clock_get_offset (micro_os_plus_clock_t* clock)
{
  assert (clock != nullptr);
  return (micro_os_plus_clock_offset_t) (
             reinterpret_cast<rtos::clock&> (*clock))
      .offset ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::adjustable_clock::offset(offset_t)
 */
micro_os_plus_clock_offset_t
micro_os_plus_clock_set_offset (micro_os_plus_clock_t* clock,
                                micro_os_plus_clock_offset_t offset)
{
  assert (clock != nullptr);
  assert (clock != nullptr);
  return (micro_os_plus_clock_offset_t) (
             reinterpret_cast<rtos::clock&> (*clock))
      .offset ((clock::offset_t)offset);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::sysclock
 */
micro_os_plus_clock_t*
micro_os_plus_clock_get_sysclock (void)
{
  return (micro_os_plus_clock_t*)&sysclock;
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::rtclock
 */
micro_os_plus_clock_t*
micro_os_plus_clock_get_rtclock (void)
{
  return (micro_os_plus_clock_t*)&rtclock;
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::hrclock
 */
micro_os_plus_clock_t*
micro_os_plus_clock_get_hrclock (void)
{
  return (micro_os_plus_clock_t*)&hrclock;
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::clock::now()
 */
micro_os_plus_clock_timestamp_t
micro_os_plus_sysclock_now (void)
{
  return (micro_os_plus_clock_timestamp_t)sysclock.now ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::clock::sleep_for()
 */
micro_os_plus_result_t
micro_os_plus_sysclock_sleep_for (micro_os_plus_clock_duration_t duration)
{
  return (micro_os_plus_result_t)sysclock.sleep_for (duration);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::clock::sleep_until()
 */
micro_os_plus_result_t
micro_os_plus_sysclock_sleep_until (micro_os_plus_clock_timestamp_t timestamp)
{
  return (micro_os_plus_result_t)sysclock.sleep_until (timestamp);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::clock::wait_for()
 */
micro_os_plus_result_t
micro_os_plus_sysclock_wait_for (micro_os_plus_clock_duration_t timeout)
{
  return (micro_os_plus_result_t)sysclock.wait_for (timeout);
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::timer::attributes
 */
void
micro_os_plus_timer_attributes_init (micro_os_plus_timer_attributes_t* attr)
{
  assert (attr != nullptr);
  new (attr) timer::attributes ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::timer::attributes_periodic
 */
void
micro_os_plus_timer_attributes_periodic_init (
    micro_os_plus_timer_attributes_t* attr)
{
  assert (attr != nullptr);
  new (attr) timer::attributes_periodic ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::timer::periodic_initializer
 */
const micro_os_plus_timer_attributes_t*
micro_os_plus_timer_attributes_get_periodic (void)
{
  return (const micro_os_plus_timer_attributes_t*)&timer::periodic_initializer;
}

/**
 * @details
 *
 * @note Must be paired with `micro_os_plus_timer_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::timer
 */
void
micro_os_plus_timer_construct (micro_os_plus_timer_t* timer, const char* name,
                               micro_os_plus_timer_func_t function,
                               micro_os_plus_timer_func_args_t args,
                               const micro_os_plus_timer_attributes_t* attr)
{
  assert (timer != nullptr);
  if (attr == nullptr)
    {
      attr = (const micro_os_plus_timer_attributes_t*)&timer::
          periodic_initializer;
    }
  new (timer)
      rtos::timer (name, (timer::func_t)function, (timer::func_args_t)args,
                   (const timer::attributes&)*attr);
}

/**
 * @details
 *
 * @note Must be paired with `micro_os_plus_timer_construct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::timer
 */
void
micro_os_plus_timer_destruct (micro_os_plus_timer_t* timer)
{
  assert (timer != nullptr);
  (reinterpret_cast<rtos::timer&> (*timer)).~timer ();
}

/**
 * @details
 *
 * Dynamically allocate the timer object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new timer(...)`.
 * @note Must be paired with `micro_os_plus_timer_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::timer
 */
micro_os_plus_timer_t*
micro_os_plus_timer_new (const char* name, micro_os_plus_timer_func_t function,
                         micro_os_plus_timer_func_args_t args,
                         const micro_os_plus_timer_attributes_t* attr)
{
  if (attr == nullptr)
    {
      attr = (const micro_os_plus_timer_attributes_t*)&timer::
          periodic_initializer;
    }
  return reinterpret_cast<micro_os_plus_timer_t*> (
      new rtos::timer (name, (timer::func_t)function, (timer::func_args_t)args,
                       (const timer::attributes&)*attr));
}

/**
 * @details
 *
 * Destruct the timer and deallocate the dynamically allocated
 * space using the RTOS system allocator.
 *
 * @note Equivalent of C++ `delete ptr_timer`.
 * @note Must be paired with `micro_os_plus_timer_new()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::timer
 */
void
micro_os_plus_timer_delete (micro_os_plus_timer_t* timer)
{
  assert (timer != nullptr);
  delete reinterpret_cast<rtos::timer*> (timer);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::timer::name()
 */
const char*
micro_os_plus_timer_get_name (micro_os_plus_timer_t* timer)
{
  assert (timer != nullptr);
  return (reinterpret_cast<rtos::timer&> (*timer)).name ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::timer::start()
 */
micro_os_plus_result_t
micro_os_plus_timer_start (micro_os_plus_timer_t* timer,
                           micro_os_plus_clock_duration_t period)
{
  assert (timer != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<rtos::timer&> (*timer))
      .start (period);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::timer::stop()
 */
micro_os_plus_result_t
micro_os_plus_timer_stop (micro_os_plus_timer_t* timer)
{
  assert (timer != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<rtos::timer&> (*timer))
      .stop ();
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex::attributes
 */
void
micro_os_plus_mutex_attributes_init (micro_os_plus_mutex_attributes_t* attr)
{
  assert (attr != nullptr);
  new (attr) mutex::attributes ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex::attributes_recursive
 */
void
micro_os_plus_mutex_attributes_recursive_init (
    micro_os_plus_mutex_attributes_t* attr)
{
  assert (attr != nullptr);
  new (attr) mutex::attributes_recursive ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex::initializer_recursive
 */
const micro_os_plus_mutex_attributes_t*
micro_os_plus_mutex_attributes_get_recursive (void)
{
  return (
      const micro_os_plus_mutex_attributes_t*)&mutex::initializer_recursive;
}

/**
 * @details
 *
 * @note Must be paired with `micro_os_plus_mutex_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex
 */
void
micro_os_plus_mutex_construct (micro_os_plus_mutex_t* mutex, const char* name,
                               const micro_os_plus_mutex_attributes_t* attr)
{
  assert (mutex != nullptr);
  if (attr == nullptr)
    {
      attr = (const micro_os_plus_mutex_attributes_t*)&mutex::
          initializer_normal;
    }
  new (mutex) rtos::mutex (name, (const mutex::attributes&)*attr);
}

/**
 * @details
 *
 * @note Must be paired with `micro_os_plus_mutex_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex_recursive
 */
void
micro_os_plus_mutex_recursive_construct (
    micro_os_plus_mutex_t* mutex, const char* name,
    const micro_os_plus_mutex_attributes_t* attr)
{
  assert (mutex != nullptr);
  if (attr == nullptr)
    {
      attr = (const micro_os_plus_mutex_attributes_t*)&mutex::
          initializer_recursive;
    }
  new (mutex) rtos::mutex_recursive (name, (const mutex::attributes&)*attr);
}

/**
 * @details
 *
 * @note Must be paired with `micro_os_plus_mutex_construct()` or
 *  `micro_os_plus_mutex_recursive_construct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex
 */
void
micro_os_plus_mutex_destruct (micro_os_plus_mutex_t* mutex)
{
  assert (mutex != nullptr);
  (reinterpret_cast<rtos::mutex&> (*mutex)).~mutex ();
}

/**
 * @details
 *
 * Dynamically allocate the mutex object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new mutex(...)`.
 * @note Must be paired with `micro_os_plus_mutex_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex
 */
micro_os_plus_mutex_t*
micro_os_plus_mutex_new (const char* name,
                         const micro_os_plus_mutex_attributes_t* attr)
{
  if (attr == nullptr)
    {
      attr = (const micro_os_plus_mutex_attributes_t*)&mutex::
          initializer_normal;
    }
  return reinterpret_cast<micro_os_plus_mutex_t*> (
      new rtos::mutex (name, (const mutex::attributes&)*attr));
}

/**
 * @details
 *
 * Dynamically allocate the recursive mutex object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new mutex_recursive(...)`.
 * @note Must be paired with `micro_os_plus_mutex_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex_recursive
 */
micro_os_plus_mutex_t*
micro_os_plus_mutex_recursive_new (
    const char* name, const micro_os_plus_mutex_attributes_t* attr)
{
  if (attr == nullptr)
    {
      attr = (const micro_os_plus_mutex_attributes_t*)&mutex::
          initializer_recursive;
    }
  return reinterpret_cast<micro_os_plus_mutex_t*> (
      new rtos::mutex_recursive (name, (const mutex::attributes&)*attr));
}

/**
 * @details
 *
 * Destruct the mutex and deallocate the dynamically allocated
 * space using the RTOS system allocator.
 *
 * @note Equivalent of C++ `delete ptr_mutex`.
 * @note Must be paired with `micro_os_plus_mutex_new()` or
 *  `micro_os_plus_mutex_recursive_new()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex
 */
void
micro_os_plus_mutex_delete (micro_os_plus_mutex_t* mutex)
{
  assert (mutex != nullptr);
  delete reinterpret_cast<rtos::mutex*> (mutex);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex::name()
 */
const char*
micro_os_plus_mutex_get_name (micro_os_plus_mutex_t* mutex)
{
  assert (mutex != nullptr);
  return (reinterpret_cast<rtos::mutex&> (*mutex)).name ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex::lock()
 */
micro_os_plus_result_t
micro_os_plus_mutex_lock (micro_os_plus_mutex_t* mutex)
{
  assert (mutex != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<rtos::mutex&> (*mutex))
      .lock ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex::try_lock()
 */
micro_os_plus_result_t
micro_os_plus_mutex_try_lock (micro_os_plus_mutex_t* mutex)
{
  assert (mutex != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<rtos::mutex&> (*mutex))
      .try_lock ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex::timed_lock()
 */
micro_os_plus_result_t
micro_os_plus_mutex_timed_lock (micro_os_plus_mutex_t* mutex,
                                micro_os_plus_clock_duration_t timeout)
{
  assert (mutex != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<rtos::mutex&> (*mutex))
      .timed_lock (timeout);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex::unlock()
 */
micro_os_plus_result_t
micro_os_plus_mutex_unlock (micro_os_plus_mutex_t* mutex)
{
  assert (mutex != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<rtos::mutex&> (*mutex))
      .unlock ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex::priority_ceiling()
 */
micro_os_plus_thread_priority_t
micro_os_plus_mutex_get_priority_ceiling (micro_os_plus_mutex_t* mutex)
{
  assert (mutex != nullptr);
  return (micro_os_plus_thread_priority_t) (
             reinterpret_cast<rtos::mutex&> (*mutex))
      .priority_ceiling ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex::priority_ceiling(thread::priority_t,
 * thread::priority_t*)
 */
micro_os_plus_result_t
micro_os_plus_mutex_set_priority_ceiling (
    micro_os_plus_mutex_t* mutex,
    micro_os_plus_thread_priority_t priority_ceiling,
    micro_os_plus_thread_priority_t* old_priority_ceiling)
{
  assert (mutex != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<rtos::mutex&> (*mutex))
      .priority_ceiling (priority_ceiling, old_priority_ceiling);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex::consistent()
 */
micro_os_plus_result_t
micro_os_plus_mutex_mark_consistent (micro_os_plus_mutex_t* mutex)
{
  assert (mutex != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<rtos::mutex&> (*mutex))
      .consistent ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex::owner()
 */
micro_os_plus_thread_t*
micro_os_plus_mutex_get_owner (micro_os_plus_mutex_t* mutex)
{
  assert (mutex != nullptr);
  return (micro_os_plus_thread_t*)(reinterpret_cast<rtos::mutex&> (*mutex))
      .owner ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex::type()
 */
micro_os_plus_mutex_type_t
micro_os_plus_mutex_get_type (micro_os_plus_mutex_t* mutex)
{
  assert (mutex != nullptr);
  return (reinterpret_cast<rtos::mutex&> (*mutex)).type ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex::protocol()
 */
micro_os_plus_mutex_protocol_t
micro_os_plus_mutex_get_protocol (micro_os_plus_mutex_t* mutex)
{
  assert (mutex != nullptr);
  return (reinterpret_cast<rtos::mutex&> (*mutex)).protocol ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex::robustness()
 */
micro_os_plus_mutex_robustness_t
micro_os_plus_mutex_get_robustness (micro_os_plus_mutex_t* mutex)
{
  assert (mutex != nullptr);
  return (reinterpret_cast<rtos::mutex&> (*mutex)).robustness ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::mutex::reset()
 */
micro_os_plus_result_t
micro_os_plus_mutex_reset (micro_os_plus_mutex_t* mutex)
{
  assert (mutex != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<rtos::mutex&> (*mutex))
      .reset ();
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::condition_variable::attributes
 */
void
micro_os_plus_condition_variable_attributes_init (
    micro_os_plus_condition_variable_attributes_t* attr)
{
  assert (attr != nullptr);
  new (attr) condition_variable::attributes ();
}

/**
 * @details
 *
 * @note Must be paired with `micro_os_plus_condition_variable_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::condition_variable
 */
void
micro_os_plus_condition_variable_construct (
    micro_os_plus_condition_variable_t* condvar, const char* name,
    const micro_os_plus_condition_variable_attributes_t* attr)
{
  assert (condvar != nullptr);
  if (attr == nullptr)
    {
      attr
          = (const micro_os_plus_condition_variable_attributes_t*)&condition_variable::
              initializer;
    }
  new (condvar)
      condition_variable (name, (const condition_variable::attributes&)*attr);
}

/**
 * @details
 *
 * @note Must be paired with `micro_os_plus_condition_variable_construct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::condition_variable
 */
void
micro_os_plus_condition_variable_destruct (
    micro_os_plus_condition_variable_t* condvar)
{
  assert (condvar != nullptr);
  (reinterpret_cast<condition_variable&> (*condvar)).~condition_variable ();
}

/**
 * @details
 *
 * Dynamically allocate the condition variable object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new condition_variable(...)`.
 * @note Must be paired with `micro_os_plus_condition_variable_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::condition_variable
 */
micro_os_plus_condition_variable_t*
micro_os_plus_condition_variable_new (
    const char* name,
    const micro_os_plus_condition_variable_attributes_t* attr)
{
  if (attr == nullptr)
    {
      attr
          = (const micro_os_plus_condition_variable_attributes_t*)&condition_variable::
              initializer;
    }
  return reinterpret_cast<micro_os_plus_condition_variable_t*> (
      new condition_variable (name,
                              (const condition_variable::attributes&)*attr));
}

/**
 * @details
 *
 * Destruct the condition variable and deallocate the dynamically allocated
 * space using the RTOS system allocator.
 *
 * @note Equivalent of C++ `delete ptr_condvar`.
 * @note Must be paired with `micro_os_plus_condition_variable_new()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::condition_variable
 */
void
micro_os_plus_condition_variable_delete (
    micro_os_plus_condition_variable_t* condvar)
{
  assert (condvar != nullptr);
  delete reinterpret_cast<condition_variable*> (condvar);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::condition_variable::name()
 */
const char*
micro_os_plus_condition_variable_get_name (
    micro_os_plus_condition_variable_t* condvar)
{
  assert (condvar != nullptr);
  return (reinterpret_cast<condition_variable&> (*condvar)).name ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::condition_variable::signal()
 */
micro_os_plus_result_t
micro_os_plus_condition_variable_signal (
    micro_os_plus_condition_variable_t* condvar)
{
  assert (condvar != nullptr);
  return (micro_os_plus_result_t) (
             reinterpret_cast<condition_variable&> (*condvar))
      .signal ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::condition_variable::broadcast()
 */
micro_os_plus_result_t
micro_os_plus_condition_variable_broadcast (
    micro_os_plus_condition_variable_t* condvar)
{
  assert (condvar != nullptr);
  return (micro_os_plus_result_t) (
             reinterpret_cast<condition_variable&> (*condvar))
      .broadcast ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::condition_variable::wait()
 */
micro_os_plus_result_t
micro_os_plus_condition_variable_wait (
    micro_os_plus_condition_variable_t* condvar, micro_os_plus_mutex_t* mutex)
{
  assert (condvar != nullptr);
  return (micro_os_plus_result_t) (
             reinterpret_cast<condition_variable&> (*condvar))
      .wait (reinterpret_cast<rtos::mutex&> (*mutex));
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::condition_variable::timed_wait()
 */
micro_os_plus_result_t
micro_os_plus_condition_variable_timed_wait (
    micro_os_plus_condition_variable_t* condvar, micro_os_plus_mutex_t* mutex,
    micro_os_plus_clock_duration_t timeout)
{
  assert (condvar != nullptr);
  return (micro_os_plus_result_t) (
             reinterpret_cast<condition_variable&> (*condvar))
      .timed_wait (reinterpret_cast<rtos::mutex&> (*mutex), timeout);
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore::attributes
 */
void
micro_os_plus_semaphore_attributes_init (
    micro_os_plus_semaphore_attributes_t* attr)
{
  assert (attr != nullptr);
  new (attr) semaphore::attributes{};
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore::attributes_binary
 */
void
micro_os_plus_semaphore_attributes_binary_init (
    micro_os_plus_semaphore_attributes_t* attr,
    const micro_os_plus_semaphore_count_t initial_value)
{
  assert (attr != nullptr);
  new (attr) semaphore::attributes_binary{ initial_value };
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore::attributes_counting
 */
void
micro_os_plus_semaphore_attributes_counting_init (
    micro_os_plus_semaphore_attributes_t* attr,
    const micro_os_plus_semaphore_count_t max_value,
    const micro_os_plus_semaphore_count_t initial_value)
{
  assert (attr != nullptr);
  new (attr) semaphore::attributes_counting{ max_value, initial_value };
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore::initializer_binary
 */
const micro_os_plus_semaphore_attributes_t*
micro_os_plus_semaphore_attributes_get_binary (void)
{
  return (const micro_os_plus_semaphore_attributes_t*)&semaphore::
      initializer_binary;
}

/**
 * @details
 *
 * @note Must be paired with `micro_os_plus_semaphore_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore
 */
void
micro_os_plus_semaphore_construct (
    micro_os_plus_semaphore_t* semaphore, const char* name,
    const micro_os_plus_semaphore_attributes_t* attr)
{
  assert (semaphore != nullptr);
  if (attr == nullptr)
    {
      attr = (const micro_os_plus_semaphore_attributes_t*)&semaphore::
          initializer_binary;
    }
  new (semaphore) rtos::semaphore{ name, (const semaphore::attributes&)*attr };
}

/**
 * @details
 *
 * @note Must be paired with `micro_os_plus_semaphore_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore_binary
 */
void
micro_os_plus_semaphore_binary_construct (
    micro_os_plus_semaphore_t* semaphore, const char* name,
    const micro_os_plus_semaphore_count_t initial_value)
{
  assert (semaphore != nullptr);
  new (semaphore) rtos::semaphore_binary{ name, initial_value };
}

/**
 * @details
 *
 * @note Must be paired with `micro_os_plus_semaphore_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore_counting
 */
void
micro_os_plus_semaphore_counting_construct (
    micro_os_plus_semaphore_t* semaphore, const char* name,
    const micro_os_plus_semaphore_count_t max_value,
    const micro_os_plus_semaphore_count_t initial_value)
{
  assert (semaphore != nullptr);
  new (semaphore) rtos::semaphore_counting{ name, max_value, initial_value };
}

/**
 * @details
 *
 * @note Must be paired with `micro_os_plus_semaphore_construct()` or
 * `micro_os_plus_semaphore_binary_construct()` or
 * `micro_os_plus_semaphore_counting_construct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore
 */
void
micro_os_plus_semaphore_destruct (micro_os_plus_semaphore_t* semaphore)
{
  assert (semaphore != nullptr);
  (reinterpret_cast<rtos::semaphore&> (*semaphore)).~semaphore ();
}

/**
 * @details
 *
 * Dynamically allocate the semaphore object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new semaphore(...)`.
 * @note Must be paired with `micro_os_plus_semaphore_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore
 */
micro_os_plus_semaphore_t*
micro_os_plus_semaphore_new (const char* name,
                             const micro_os_plus_semaphore_attributes_t* attr)
{
  if (attr == nullptr)
    {
      attr = (const micro_os_plus_semaphore_attributes_t*)&semaphore::
          initializer_binary;
    }
  return reinterpret_cast<micro_os_plus_semaphore_t*> (
      new rtos::semaphore{ name, (const semaphore::attributes&)*attr });
}

/**
 * @details
 *
 * Dynamically allocate the binary semaphore object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new semaphore_binary(...)`.
 * @note Must be paired with `micro_os_plus_semaphore_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore_binary
 */
micro_os_plus_semaphore_t*
micro_os_plus_semaphore_binary_new (
    const char* name, const micro_os_plus_semaphore_count_t initial_value)
{
  return reinterpret_cast<micro_os_plus_semaphore_t*> (
      new rtos::semaphore_binary{ name, initial_value });
}

/**
 * @details
 *
 * Dynamically allocate the counting semaphore object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new semaphore_counting(...)`.
 * @note Must be paired with `micro_os_plus_semaphore_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore_counting
 */
micro_os_plus_semaphore_t*
micro_os_plus_semaphore_counting_new (
    const char* name, const micro_os_plus_semaphore_count_t max_value,
    const micro_os_plus_semaphore_count_t initial_value)
{
  return reinterpret_cast<micro_os_plus_semaphore_t*> (
      new rtos::semaphore_counting{ name, max_value, initial_value });
}

/**
 * @details
 *
 * Destruct the semaphore and deallocate the dynamically allocated
 * space using the RTOS system allocator.
 *
 * @note Equivalent of C++ `delete ptr_semaphore`.
 * @note Must be paired with `micro_os_plus_semaphore_new()` or
 * `micro_os_plus_semaphore_binary_new()` or
 * `micro_os_plus_semaphore_counting_new()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore
 */
void
micro_os_plus_semaphore_delete (micro_os_plus_semaphore_t* semaphore)
{
  assert (semaphore != nullptr);
  delete reinterpret_cast<rtos::semaphore*> (semaphore);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore::name()
 */
const char*
micro_os_plus_semaphore_get_name (micro_os_plus_semaphore_t* semaphore)
{
  assert (semaphore != nullptr);
  return (reinterpret_cast<rtos::semaphore&> (*semaphore)).name ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore::post()
 */
micro_os_plus_result_t
micro_os_plus_semaphore_post (micro_os_plus_semaphore_t* semaphore)
{
  assert (semaphore != nullptr);
  return (micro_os_plus_result_t) (
             reinterpret_cast<rtos::semaphore&> (*semaphore))
      .post ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore::wait()
 */
micro_os_plus_result_t
micro_os_plus_semaphore_wait (micro_os_plus_semaphore_t* semaphore)
{
  assert (semaphore != nullptr);
  return (micro_os_plus_result_t) (
             reinterpret_cast<rtos::semaphore&> (*semaphore))
      .wait ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore::try_wait()
 */
micro_os_plus_result_t
micro_os_plus_semaphore_try_wait (micro_os_plus_semaphore_t* semaphore)
{
  assert (semaphore != nullptr);
  return (micro_os_plus_result_t) (
             reinterpret_cast<rtos::semaphore&> (*semaphore))
      .try_wait ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore::timed_wait()
 */
micro_os_plus_result_t
micro_os_plus_semaphore_timed_wait (micro_os_plus_semaphore_t* semaphore,
                                    micro_os_plus_clock_duration_t timeout)
{
  assert (semaphore != nullptr);
  return (micro_os_plus_result_t) (
             reinterpret_cast<rtos::semaphore&> (*semaphore))
      .timed_wait (timeout);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore::value()
 */
micro_os_plus_semaphore_count_t
micro_os_plus_semaphore_get_value (micro_os_plus_semaphore_t* semaphore)
{
  assert (semaphore != nullptr);
  return (micro_os_plus_semaphore_count_t) (
             reinterpret_cast<rtos::semaphore&> (*semaphore))
      .value ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore::reset()
 */
micro_os_plus_result_t
micro_os_plus_semaphore_reset (micro_os_plus_semaphore_t* semaphore)
{
  assert (semaphore != nullptr);
  return (micro_os_plus_result_t) (
             reinterpret_cast<rtos::semaphore&> (*semaphore))
      .reset ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore::initial_value()
 */
micro_os_plus_semaphore_count_t
micro_os_plus_semaphore_get_initial_value (
    micro_os_plus_semaphore_t* semaphore)
{
  assert (semaphore != nullptr);
  return (micro_os_plus_semaphore_count_t) (
             reinterpret_cast<rtos::semaphore&> (*semaphore))
      .initial_value ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::semaphore::max_value()
 */
micro_os_plus_semaphore_count_t
micro_os_plus_semaphore_get_max_value (micro_os_plus_semaphore_t* semaphore)
{
  assert (semaphore != nullptr);
  return (micro_os_plus_semaphore_count_t) (
             reinterpret_cast<rtos::semaphore&> (*semaphore))
      .max_value ();
}

// ----------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory_pool::attributes
 */
void
micro_os_plus_mempool_attributes_init (
    micro_os_plus_mempool_attributes_t* attr)
{
  assert (attr != nullptr);
  new (attr) memory_pool::attributes ();
}

/**
 * @details
 *
 * @note Must be paired with `micro_os_plus_mempool_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory_pool
 */
void
micro_os_plus_mempool_construct (
    micro_os_plus_mempool_t* mempool, const char* name, size_t blocks,
    size_t block_size_bytes, const micro_os_plus_mempool_attributes_t* attr)
{
  assert (mempool != nullptr);
  if (attr == nullptr)
    {
      attr = (const micro_os_plus_mempool_attributes_t*)&memory_pool::
          initializer;
    }
  new (mempool) memory_pool (name, blocks, block_size_bytes,
                             (const memory_pool::attributes&)*attr);
}

/**
 * @details
 *
 * @note Must be paired with `micro_os_plus_mempool_construct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory_pool
 */
void
micro_os_plus_mempool_destruct (micro_os_plus_mempool_t* mempool)
{
  assert (mempool != nullptr);
  (reinterpret_cast<memory_pool&> (*mempool)).~memory_pool ();
}

/**
 * @details
 *
 * Dynamically allocate the memory pool object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new memory_pool(...)`.
 * @note Must be paired with `micro_os_plus_mempool_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory_pool
 */
micro_os_plus_mempool_t*
micro_os_plus_mempool_new (const char* name, size_t blocks,
                           size_t block_size_bytes,
                           const micro_os_plus_mempool_attributes_t* attr)
{
  if (attr == nullptr)
    {
      attr = (const micro_os_plus_mempool_attributes_t*)&memory_pool::
          initializer;
    }
  return reinterpret_cast<micro_os_plus_mempool_t*> (new memory_pool (
      name, blocks, block_size_bytes, (const memory_pool::attributes&)*attr));
}

/**
 * @details
 *
 * Destruct the memory pool and deallocate the dynamically allocated
 * space using the RTOS system allocator.
 *
 * @note Equivalent of C++ `delete ptr_mempool`.
 * @note Must be paired with `micro_os_plus_mempool_new()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory_pool
 */
void
micro_os_plus_mempool_delete (micro_os_plus_mempool_t* mempool)
{
  assert (mempool != nullptr);
  delete reinterpret_cast<memory_pool*> (mempool);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory_pool::name()
 */
const char*
micro_os_plus_mempool_get_name (micro_os_plus_mempool_t* mempool)
{
  assert (mempool != nullptr);
  return (reinterpret_cast<memory_pool&> (*mempool)).name ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory_pool::alloc()
 */
void*
micro_os_plus_mempool_alloc (micro_os_plus_mempool_t* mempool)
{
  assert (mempool != nullptr);
  return (reinterpret_cast<memory_pool&> (*mempool)).alloc ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory_pool::try_alloc()
 */
void*
micro_os_plus_mempool_try_alloc (micro_os_plus_mempool_t* mempool)
{
  assert (mempool != nullptr);
  return (reinterpret_cast<memory_pool&> (*mempool)).try_alloc ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory_pool::timed_alloc()
 */
void*
micro_os_plus_mempool_timed_alloc (micro_os_plus_mempool_t* mempool,
                                   micro_os_plus_clock_duration_t timeout)
{
  assert (mempool != nullptr);
  return (reinterpret_cast<memory_pool&> (*mempool)).timed_alloc (timeout);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory_pool::free()
 */
micro_os_plus_result_t
micro_os_plus_mempool_free (micro_os_plus_mempool_t* mempool, void* block)
{
  assert (mempool != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<memory_pool&> (*mempool))
      .free (block);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory_pool::capacity()
 */
size_t
micro_os_plus_mempool_get_capacity (micro_os_plus_mempool_t* mempool)
{
  assert (mempool != nullptr);
  return (reinterpret_cast<memory_pool&> (*mempool)).capacity ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory_pool::count()
 */
size_t
micro_os_plus_mempool_get_count (micro_os_plus_mempool_t* mempool)
{
  assert (mempool != nullptr);
  return (reinterpret_cast<memory_pool&> (*mempool)).count ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory_pool::block_size()
 */
size_t
micro_os_plus_mempool_get_block_size (micro_os_plus_mempool_t* mempool)
{
  assert (mempool != nullptr);
  return (reinterpret_cast<memory_pool&> (*mempool)).block_size ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory_pool::empty()
 */
bool
micro_os_plus_mempool_is_empty (micro_os_plus_mempool_t* mempool)
{
  assert (mempool != nullptr);
  return (reinterpret_cast<memory_pool&> (*mempool)).empty ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory_pool::full()
 */
bool
micro_os_plus_mempool_is_full (micro_os_plus_mempool_t* mempool)
{
  assert (mempool != nullptr);
  return (reinterpret_cast<memory_pool&> (*mempool)).full ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory_pool::reset()
 */
micro_os_plus_result_t
micro_os_plus_mempool_reset (micro_os_plus_mempool_t* mempool)
{
  assert (mempool != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<memory_pool&> (*mempool))
      .reset ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory_pool::pool()
 */
void*
micro_os_plus_mempool_get_pool (micro_os_plus_mempool_t* mempool)
{
  assert (mempool != nullptr);
  return (void*)(reinterpret_cast<memory_pool&> (*mempool)).pool ();
}

// --------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::message_queue::attributes
 */
void
micro_os_plus_mqueue_attributes_init (micro_os_plus_mqueue_attributes_t* attr)
{
  assert (attr != nullptr);
  new (attr) message_queue::attributes ();
}

/**
 * @details
 *
 * @note Must be paired with `micro_os_plus_mqueue_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::message_queue
 */
void
micro_os_plus_mqueue_construct (micro_os_plus_mqueue_t* mqueue,
                                const char* name, size_t msgs,
                                size_t msg_size_bytes,
                                const micro_os_plus_mqueue_attributes_t* attr)
{
  assert (mqueue != nullptr);
  if (attr == nullptr)
    {
      attr = (const micro_os_plus_mqueue_attributes_t*)&message_queue::
          initializer;
    }
  new (mqueue) message_queue (name, msgs, msg_size_bytes,
                              (const message_queue::attributes&)*attr);
}

/**
 * @details
 *
 * @note Must be paired with `micro_os_plus_mqueue_construct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::message_queue
 */
void
micro_os_plus_mqueue_destruct (micro_os_plus_mqueue_t* mqueue)
{
  assert (mqueue != nullptr);
  (reinterpret_cast<message_queue&> (*mqueue)).~message_queue ();
}

/**
 * @details
 *
 * Dynamically allocate the message queue object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new message_queue(...)`.
 * @note Must be paired with `micro_os_plus_mqueue_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::message_queue
 */
micro_os_plus_mqueue_t*
micro_os_plus_mqueue_new (const char* name, size_t msgs, size_t msg_size_bytes,
                          const micro_os_plus_mqueue_attributes_t* attr)
{
  if (attr == nullptr)
    {
      attr = (const micro_os_plus_mqueue_attributes_t*)&message_queue::
          initializer;
    }
  return reinterpret_cast<micro_os_plus_mqueue_t*> (new message_queue (
      name, msgs, msg_size_bytes, (const message_queue::attributes&)*attr));
}

/**
 * @details
 *
 * Destruct the message queue and deallocate the dynamically allocated
 * space using the RTOS system allocator.
 *
 * @note Equivalent of C++ `delete ptr_mqueue`.
 * @note Must be paired with `micro_os_plus_mqueue_new()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::message_queue
 */
void
micro_os_plus_mqueue_delete (micro_os_plus_mqueue_t* mqueue)
{
  assert (mqueue != nullptr);
  delete reinterpret_cast<message_queue*> (mqueue);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::message_queue::name()
 */
const char*
micro_os_plus_mqueue_get_name (micro_os_plus_mqueue_t* mqueue)
{
  assert (mqueue != nullptr);
  return (reinterpret_cast<message_queue&> (*mqueue)).name ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::message_queue::send()
 */
micro_os_plus_result_t
micro_os_plus_mqueue_send (micro_os_plus_mqueue_t* mqueue, const void* msg,
                           size_t nbytes,
                           micro_os_plus_mqueue_priority_t mprio)
{
  assert (mqueue != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<message_queue&> (*mqueue))
      .send (msg, nbytes, mprio);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::message_queue::try_send()
 */
micro_os_plus_result_t
micro_os_plus_mqueue_try_send (micro_os_plus_mqueue_t* mqueue, const void* msg,
                               size_t nbytes,
                               micro_os_plus_mqueue_priority_t mprio)
{
  assert (mqueue != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<message_queue&> (*mqueue))
      .try_send (msg, nbytes, mprio);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::message_queue::timed_send()
 */
micro_os_plus_result_t
micro_os_plus_mqueue_timed_send (micro_os_plus_mqueue_t* mqueue,
                                 const void* msg, size_t nbytes,
                                 micro_os_plus_clock_duration_t timeout,
                                 micro_os_plus_mqueue_priority_t mprio)
{
  assert (mqueue != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<message_queue&> (*mqueue))
      .timed_send (msg, nbytes, timeout, mprio);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::message_queue::receive()
 */
micro_os_plus_result_t
micro_os_plus_mqueue_receive (micro_os_plus_mqueue_t* mqueue, void* msg,
                              size_t nbytes,
                              micro_os_plus_mqueue_priority_t* mprio)
{
  assert (mqueue != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<message_queue&> (*mqueue))
      .receive (msg, nbytes, mprio);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::message_queue::try_receive()
 */
micro_os_plus_result_t
micro_os_plus_mqueue_try_receive (micro_os_plus_mqueue_t* mqueue, void* msg,
                                  size_t nbytes,
                                  micro_os_plus_mqueue_priority_t* mprio)
{
  assert (mqueue != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<message_queue&> (*mqueue))
      .try_receive (msg, nbytes, mprio);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::message_queue::timed_receive()
 */
micro_os_plus_result_t
micro_os_plus_mqueue_timed_receive (micro_os_plus_mqueue_t* mqueue, void* msg,
                                    size_t nbytes,
                                    micro_os_plus_clock_duration_t timeout,
                                    micro_os_plus_mqueue_priority_t* mprio)
{
  assert (mqueue != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<message_queue&> (*mqueue))
      .timed_receive (msg, nbytes, timeout, mprio);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::message_queue::length()
 */
size_t
micro_os_plus_mqueue_get_length (micro_os_plus_mqueue_t* mqueue)
{
  assert (mqueue != nullptr);
  return (reinterpret_cast<message_queue&> (*mqueue)).length ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::message_queue::capacity()
 */
size_t
micro_os_plus_mqueue_get_capacity (micro_os_plus_mqueue_t* mqueue)
{
  assert (mqueue != nullptr);
  return (reinterpret_cast<message_queue&> (*mqueue)).capacity ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::message_queue::msg_size()
 */
size_t
micro_os_plus_mqueue_get_msg_size (micro_os_plus_mqueue_t* mqueue)
{
  assert (mqueue != nullptr);
  return (reinterpret_cast<message_queue&> (*mqueue)).msg_size ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::message_queue::empty()
 */
bool
micro_os_plus_mqueue_is_empty (micro_os_plus_mqueue_t* mqueue)
{
  assert (mqueue != nullptr);
  return (reinterpret_cast<message_queue&> (*mqueue)).empty ();
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::message_queue::full()
 */
bool
micro_os_plus_mqueue_is_full (micro_os_plus_mqueue_t* mqueue)
{
  assert (mqueue != nullptr);
  return (reinterpret_cast<message_queue&> (*mqueue)).full ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::message_queue::reset()
 */
micro_os_plus_result_t
micro_os_plus_mqueue_reset (micro_os_plus_mqueue_t* mqueue)
{
  assert (mqueue != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<message_queue&> (*mqueue))
      .reset ();
}

// --------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::event_flags::attributes
 */
void
micro_os_plus_event_flags_attributes_init (
    micro_os_plus_event_flags_attributes_t* attr)
{
  assert (attr != nullptr);
  new (attr) event_flags::attributes ();
}

/**
 * @details
 *
 * @note Must be paired with `micro_os_plus_event_flags_destruct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::event_flags
 */
void
micro_os_plus_event_flags_construct (
    micro_os_plus_event_flags_t* evflags, const char* name,
    const micro_os_plus_event_flags_attributes_t* attr)
{
  assert (evflags != nullptr);
  if (attr == nullptr)
    {
      attr = (const micro_os_plus_event_flags_attributes_t*)&event_flags::
          initializer;
    }
  new (evflags) event_flags (name, (const event_flags::attributes&)*attr);
}

/**
 * @details
 *
 * @note Must be paired with `micro_os_plus_event_flags_construct()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::event_flags
 */
void
micro_os_plus_event_flags_destruct (micro_os_plus_event_flags_t* evflags)
{
  assert (evflags != nullptr);
  (reinterpret_cast<event_flags&> (*evflags)).~event_flags ();
}

/**
 * @details
 *
 * Dynamically allocate the event flags object instance using the RTOS
 * system allocator and construct it.
 *
 * @note Equivalent of C++ `new event_flags(...)`.
 * @note Must be paired with `micro_os_plus_event_flags_delete()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::event_flags
 */
micro_os_plus_event_flags_t*
micro_os_plus_event_flags_new (
    const char* name, const micro_os_plus_event_flags_attributes_t* attr)
{
  if (attr == nullptr)
    {
      attr = (const micro_os_plus_event_flags_attributes_t*)&event_flags::
          initializer;
    }
  return reinterpret_cast<micro_os_plus_event_flags_t*> (
      new event_flags (name, (const event_flags::attributes&)*attr));
}

/**
 * @details
 *
 * Destruct the event flags and deallocate the dynamically allocated
 * space using the RTOS system allocator.
 *
 * @note Equivalent of C++ `delete ptr_evflags`.
 * @note Must be paired with `micro_os_plus_event_flags_new()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::event_flags
 */
void
micro_os_plus_event_flags_delete (micro_os_plus_event_flags_t* evflags)
{
  assert (evflags != nullptr);
  delete reinterpret_cast<event_flags*> (evflags);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::event_flags::name()
 */
const char*
micro_os_plus_event_flags_get_name (micro_os_plus_event_flags_t* evflags)
{
  assert (evflags != nullptr);
  return (reinterpret_cast<event_flags&> (*evflags)).name ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::event_flags::wait()
 */
micro_os_plus_result_t
micro_os_plus_event_flags_wait (micro_os_plus_event_flags_t* evflags,
                                micro_os_plus_flags_mask_t mask,
                                micro_os_plus_flags_mask_t* oflags,
                                micro_os_plus_flags_mode_t mode)
{
  assert (evflags != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<event_flags&> (*evflags))
      .wait (mask, oflags, mode);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::event_flags::try_wait()
 */
micro_os_plus_result_t
micro_os_plus_event_flags_try_wait (micro_os_plus_event_flags_t* evflags,
                                    micro_os_plus_flags_mask_t mask,
                                    micro_os_plus_flags_mask_t* oflags,
                                    micro_os_plus_flags_mode_t mode)
{
  assert (evflags != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<event_flags&> (*evflags))
      .try_wait (mask, oflags, mode);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::event_flags::timed_wait()
 */
micro_os_plus_result_t
micro_os_plus_event_flags_timed_wait (micro_os_plus_event_flags_t* evflags,
                                      micro_os_plus_flags_mask_t mask,
                                      micro_os_plus_clock_duration_t timeout,
                                      micro_os_plus_flags_mask_t* oflags,
                                      micro_os_plus_flags_mode_t mode)
{
  assert (evflags != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<event_flags&> (*evflags))
      .timed_wait (mask, timeout, oflags, mode);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::event_flags::raise()
 */
micro_os_plus_result_t
micro_os_plus_event_flags_raise (micro_os_plus_event_flags_t* evflags,
                                 micro_os_plus_flags_mask_t mask,
                                 micro_os_plus_flags_mask_t* oflags)
{
  assert (evflags != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<event_flags&> (*evflags))
      .raise (mask, oflags);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::event_flags::clear()
 */
micro_os_plus_result_t
micro_os_plus_event_flags_clear (micro_os_plus_event_flags_t* evflags,
                                 micro_os_plus_flags_mask_t mask,
                                 micro_os_plus_flags_mask_t* oflags)
{
  assert (evflags != nullptr);
  return (micro_os_plus_result_t) (reinterpret_cast<event_flags&> (*evflags))
      .clear (mask, oflags);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::event_flags::get()
 */
micro_os_plus_flags_mask_t
micro_os_plus_event_flags_get (micro_os_plus_event_flags_t* evflags,
                               micro_os_plus_flags_mask_t mask,
                               micro_os_plus_flags_mode_t mode)
{
  assert (evflags != nullptr);
  return (micro_os_plus_flags_mask_t) (
             reinterpret_cast<event_flags&> (*evflags))
      .get (mask, mode);
}

/**
 * @details
 *
 * @note Can be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::event_flags::raise()
 */
bool
micro_os_plus_event_flags_are_waiting (micro_os_plus_event_flags_t* evflags)
{
  assert (evflags != nullptr);
  return (reinterpret_cast<event_flags&> (*evflags)).waiting ();
}

// --------------------------------------------------------------------------

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory::get_default_resource()
 */
micro_os_plus_memory_t*
micro_os_plus_memory_get_default (void)
{
  return reinterpret_cast<micro_os_plus_memory_t*> (
      rtos::memory::get_default_resource ());
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 * @warning Not thread safe, use a scheduler critical section to protect it.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory::memory_resource::allocate()
 */
void*
micro_os_plus_memory_allocate (micro_os_plus_memory_t* memory, size_t bytes,
                               size_t alignment)
{
  assert (memory != nullptr);
  return (reinterpret_cast<rtos::memory::memory_resource&> (*memory))
      .allocate (bytes, alignment);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 * @warning Not thread safe, use a scheduler critical section to protect it.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory::memory_resource::deallocate()
 */
void
micro_os_plus_memory_deallocate (micro_os_plus_memory_t* memory, void* addr,
                                 size_t bytes, size_t alignment)
{
  assert (memory != nullptr);
  (reinterpret_cast<rtos::memory::memory_resource&> (*memory))
      .deallocate (addr, bytes, alignment);
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 * @warning Not thread safe, use a scheduler critical section to protect it.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory::memory_resource::reset()
 */
void
micro_os_plus_memory_reset (micro_os_plus_memory_t* memory)
{
  assert (memory != nullptr);
  (reinterpret_cast<rtos::memory::memory_resource&> (*memory)).reset ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 * @warning Not thread safe, use a scheduler critical section to protect it.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory::memory_resource::coalesce()
 */
bool
micro_os_plus_memory_coalesce (micro_os_plus_memory_t* memory)
{
  assert (memory != nullptr);
  return (reinterpret_cast<rtos::memory::memory_resource&> (*memory))
      .coalesce ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 * @warning Not thread safe, use a scheduler critical section to protect it.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory::memory_resource::total_bytes()
 */
size_t
micro_os_plus_memory_get_total_bytes (micro_os_plus_memory_t* memory)
{
  assert (memory != nullptr);
  return (reinterpret_cast<rtos::memory::memory_resource&> (*memory))
      .total_bytes ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 * @warning Not thread safe, use a scheduler critical section to protect it.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory::memory_resource::allocated_bytes()
 */
size_t
micro_os_plus_memory_get_allocated_bytes (micro_os_plus_memory_t* memory)
{
  assert (memory != nullptr);
  return (reinterpret_cast<rtos::memory::memory_resource&> (*memory))
      .allocated_bytes ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 * @warning Not thread safe, use a scheduler critical section to protect it.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory::memory_resource::free_bytes()
 */
size_t
micro_os_plus_memory_get_free_bytes (micro_os_plus_memory_t* memory)
{
  assert (memory != nullptr);
  return (reinterpret_cast<rtos::memory::memory_resource&> (*memory))
      .free_bytes ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 * @warning Not thread safe, use a scheduler critical section to protect it.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory::memory_resource::allocated_chunks()
 */
size_t
micro_os_plus_memory_get_allocated_chunks (micro_os_plus_memory_t* memory)
{
  assert (memory != nullptr);
  return (reinterpret_cast<rtos::memory::memory_resource&> (*memory))
      .allocated_chunks ();
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 * @warning Not thread safe, use a scheduler critical section to protect it.
 *
 * @par For the complete definition, see
 *  @ref micro_os_plus::rtos::memory::memory_resource::free_chunks()
 */
size_t
micro_os_plus_memory_get_free_chunks (micro_os_plus_memory_t* memory)
{
  assert (memory != nullptr);
  return (reinterpret_cast<rtos::memory::memory_resource&> (*memory))
      .free_chunks ();
}

// ----------------------------------------------------------------------------
