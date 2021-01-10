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
 * The code provides an implementation of the CMSIS OS C API,
 * on top of the µOS++ C++ definitions.
 */

#include <micro-os-plus/rtos/os.h>

// ----------------------------------------------------------------------------

using namespace os;
using namespace os::rtos;

// ----------------------------------------------------------------------------

#include <micro-os-plus/legacy/cmsis_os.h>

// ----------------------------------------------------------------------------
//  ==== Kernel Control Functions ====

/**
 * @details
 * Initialise of the RTOS Kernel to allow peripheral setup and creation
 * of other RTOS objects with the functions:
 * - osThreadCreate : Create a thread function.
 * - osTimerCreate : Define attributes of the timer callback function.
 * - osMutexCreate : Define and initialise a mutex.
 * - osSemaphoreCreate : Define and initialise a semaphore.
 * - osPoolCreate : Define and initialise a fix-size memory pool.
 * - osMessageCreate : Define and initialise a message queue.
 * - osMailCreate : Define and initialise a mail queue with fix-size memory blocks.
 *
 * The RTOS scheduler does not start thread switching until the function
 * osKernelStart is called.
 *
 * @note In case that the RTOS Kernel starts thread execution with the
 * function main the function osKernelInitialize stops thread
 * switching. This allows you to setup the system to a defined
 * state before thread switching is resumed with osKernelStart.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osKernelInitialize (void)
{
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  scheduler::initialize ();
  return osOK;
}

/**
 * @details
 * Start the RTOS Kernel and begin thread switching.
 *
 * @note When the CMSIS-RTOS starts thread execution with the
 * function main this function resumes thread switching.
 * The main thread will continue executing after osKernelStart.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osKernelStart (void)
{
  scheduler::start ();
  // In µOS++ this does not return.
}

/**
 * @details
 * Identifies if the RTOS scheduler is started. For systems with the
 * option to start the main function as a thread this allows
 * you to identify that the RTOS scheduler is already running.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
int32_t
osKernelRunning (void)
{
  return scheduler::started () ? 1 : 0;
}

#if (defined (osFeature_SysTick)  &&  (osFeature_SysTick != 0))

/**
 * @details
 * Get the value of the Kernel SysTick timer for time comparison.
 * The value is a rolling 32-bit counter that is typically composed
 * of the kernel system interrupt timer value and an counter that
 * counts these interrupts.
 *
 * This function allows the implementation of timeout checks.
 * These are for example required when checking for a busy status
 * in a device or peripheral initialisation routine.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
uint32_t
osKernelSysTick (void)
{
  // Get the current SysTick timestamp down to cpu cycles.
  return static_cast<uint32_t> (hrclock.now ());
}

#endif    // System Timer available

// ----------------------------------------------------------------------------
//  ==== Thread Management ====

/**
 * @details
 * Find a free slot in the array of thread definitions and initialise
 * the thread.
 *
 * Start the thread function by adding it to the Active Threads list
 * and set it to state READY. The thread function receives the argument
 * pointer as function argument when the function is started. When the
 * priority of the created thread function is higher than the current
 * RUNNING thread, the created thread function starts instantly and
 * becomes the new RUNNING thread.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osThreadId
osThreadCreate (const osThreadDef_t* thread_def, void* args)
{
  if (interrupts::in_handler_mode ())
    {
      return nullptr;
    }

  if (thread_def == nullptr)
    {
      return nullptr;
    }

  thread::attributes attr;
  attr.th_priority = thread_def->tpriority;
  attr.th_stack_size_bytes = thread_def->stacksize;

  // Creating thread with invalid priority should fail (validator requirement).
  if (thread_def->tpriority >= osPriorityError)
    {
      return nullptr;
    }

  // Find a free slot in the tread definitions array.
  for (uint32_t i = 0; i < thread_def->instances; ++i)
    {
      thread* th = (thread*) &thread_def->data[i];
      if (th->state () == thread::state::undefined
          || th->state () == thread::state::destroyed)
        {
          if (attr.th_stack_size_bytes > 0)
            {
              attr.th_stack_address = &thread_def->stack[(i)
                  * ((thread_def->stacksize + sizeof(uint64_t) - 1)
                      / sizeof(uint64_t))];
            }
          new (th) thread (thread_def->name,
                           (thread::func_t) thread_def->pthread, args, attr);

          // No need to yield here, already done by constructor.
          return reinterpret_cast<osThreadId> (th);
        }
    }
  return nullptr;
}

/**
 * @details
 * Get the thread ID of the current running thread.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osThreadId
osThreadGetId (void)
{
  if (interrupts::in_handler_mode ())
    {
      return nullptr;
    }

  return reinterpret_cast<osThreadId> (&this_thread::thread ());
}

/**
 * @details
 * Remove the thread function from the active thread list. If the
 * thread is currently RUNNING the execution will stop.
 *
 * @note In case that osThreadTerminate terminates the currently
 * running task, the function never returns and other threads
 * that are in the READY state are started.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osThreadTerminate (osThreadId thread_id)
{
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (thread_id == nullptr)
    {
      return osErrorParameter;
    }

  thread::state_t state =
      (reinterpret_cast<rtos::thread&> (*thread_id)).state ();
  if (state == thread::state::undefined)
    {
      return osErrorResource;
    }

  if ((reinterpret_cast<thread*> (thread_id)) == &this_thread::thread ())
    {
      this_thread::exit ();
      /* NOTREACHED */
    }
  else
    {
      (reinterpret_cast<rtos::thread&> (*thread_id)).kill ();
    }

  return osOK;
}

/**
 * @details
 * Pass control to the next thread that is in state READY.
 * If there is no other thread in the state READY, the current
 * thread continues execution and no thread switching occurs.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osThreadYield (void)
{
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  this_thread::yield ();

  return osOK;
}

/**
 * @details
 * Change the priority of a running thread.
 * There is no need for an extra yield after it, if the priorities
 * require, there is an implicit reschedule.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osThreadSetPriority (osThreadId thread_id, osPriority priority)
{
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (thread_id == nullptr)
    {
      return osErrorParameter;
    }

  thread::state_t state =
      (reinterpret_cast<rtos::thread&> (*thread_id)).state ();
  if (state == thread::state::undefined || state >= thread::state::destroyed)
    {
      return osErrorResource;
    }

  if (priority < osPriorityIdle || priority >= osPriorityError)
    {
      return osErrorValue;
    }

  // Call C++ mutator.
  thread::priority_t prio = static_cast<thread::priority_t> (priority);
  result_t res =
      ((reinterpret_cast<rtos::thread&> (*thread_id)).priority (prio));

  // A mandatory yield is needed here, must be done
  // by the implementation.

  if (res == result::ok)
    {
      return osOK;
    }
  else if (res == EINVAL)
    {
      return osErrorValue;
    }
  else
    {
      return osErrorOS;
    }
}

/**
 * @details
 * Get the priority of an active thread. In case of a failure the value
 * osPriorityError is returned.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osPriority
osThreadGetPriority (osThreadId thread_id)
{
  if (interrupts::in_handler_mode ())
    {
      return osPriorityError;
    }

  if (thread_id == nullptr)
    {
      return osPriorityError;
    }

  // Call C++ accessor.
  thread::priority_t prio =
      (reinterpret_cast<rtos::thread&> (*thread_id)).priority ();
  return static_cast<osPriority> (prio);
}

// ----------------------------------------------------------------------------
//  ==== Generic Wait Functions ====

/**
 * @details
 * Wait for a specified time period in _millisec_.
 *
 * The millisec value specifies the number of timer ticks and is therefore
 * an upper bound. The exact time delay depends on the actual time elapsed
 * since the last timer tick.
 *
 * For a value of 1, the system waits until the next timer tick occurs.
 * That means that the actual time delay may be up to one timer tick less.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osDelay (uint32_t millisec)
{
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  result_t res = sysclock.sleep_for (
      clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)));

  if (res == ETIMEDOUT)
    {
      return osEventTimeout;
    }
  else
    {
      return osErrorOS;
    }
}

#if (defined (osFeature_Wait)  &&  (osFeature_Wait != 0))

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

/**
 * @details
 * Wait for any event of the type signal, message, mail for a specified
 * time period in millisec. While the system waits, the thread that is
 * calling this function is put into the state WAITING. When millisec
 * is set to osWaitForever, the function will wait for an infinite time
 * until an event occurs.
 *
 * The osWait function puts a thread into the state WAITING and waits
 * for any of the following events:
 *
 * - a signal sent to that thread explicitly
 * - a message from a message object that is registered to that thread
 * - a mail from a mail object that is registered to that thread
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osEvent
osWait (uint32_t millisec)
{
  osEvent event;

  if (interrupts::in_handler_mode ())
    {
      event.status = osErrorISR;
      return event;
    }

  result_t res = sysclock.wait_for (
      clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)));

  // TODO: return events
  if (res == ETIMEDOUT)
    {
      event.status = osEventTimeout;
    }
  else
    {
      event.status = osErrorOS;
    }

  return event;
}

#pragma GCC diagnostic pop

#endif  // Generic Wait available

// ----------------------------------------------------------------------------
//  ==== Timer Management Functions ====

/**
 * @details
 * Create a one-shot or periodic timer and associate it with a callback
 * function argument. The timer is initially stopped and must be started with
 * `osTimerStart()`.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osTimerId
osTimerCreate (const osTimerDef_t* timer_def, os_timer_type type, void* args)
{
  if (interrupts::in_handler_mode ())
    {
      return nullptr;
    }

  if (timer_def == nullptr)
    {
      return nullptr;
    }

  timer::attributes attr;
  attr.tm_type = (timer::type_t) type;

  new ((void*) timer_def->data) timer (timer_def->name,
                                       (timer::func_t) timer_def->ptimer,
                                       (timer::func_args_t) args, attr);

  return reinterpret_cast<osTimerId> (timer_def->data);
}

/**
 * @details
 * Start or restart the timer.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osTimerStart (osTimerId timer_id, uint32_t millisec)
{
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (timer_id == nullptr)
    {
      return osErrorParameter;
    }

  result_t res = (reinterpret_cast<rtos::timer&> (*timer_id)).start (
      clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)));

  if (res == result::ok)
    {
      return osOK;
    }
  else
    {
      return osErrorOS;
    }
}

/**
 * @details
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osTimerStop (osTimerId timer_id)
{
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (timer_id == nullptr)
    {
      return osErrorParameter;
    }

  result_t res = (reinterpret_cast<rtos::timer&> (*timer_id)).stop ();
  if (res == result::ok)
    {
      return osOK;
    }
  else if (res == EAGAIN)
    {
      return osErrorResource;
    }
  else
    {
      return osErrorOS;
    }
}

/**
 * @details
 * Delete the timer object that was created by @ref osTimerCreate.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osTimerDelete (osTimerId timer_id)
{
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (timer_id == nullptr)
    {
      return osErrorParameter;
    }

  (reinterpret_cast<rtos::timer&> (*timer_id)).~timer ();
  return osOK;
}

// ----------------------------------------------------------------------------
//  ==== Signal Management ====

/**
 * @details
 * Set the signal flags of an active thread.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
int32_t
osSignalSet (osThreadId thread_id, int32_t signals)
{
  if (thread_id == nullptr)
    {
      return (int32_t) 0x80000000;
    }

  if (signals == (int32_t) 0x80000000)
    {
      return (int32_t) 0x80000000;
    }

  flags::mask_t osig;
  ((thread*) (thread_id))->flags_raise ((flags::mask_t) signals, &osig);
  return (int32_t) osig;
}

/**
 * @details
 * Clear the signal flags of an active thread.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
int32_t
osSignalClear (osThreadId thread_id, int32_t signals)
{
  if (thread_id == nullptr)
    {
      return (int32_t) 0x80000000;
    }

  if (interrupts::in_handler_mode () || (signals == 0))
    {
      return (int32_t) 0x80000000;
    }

  flags::mask_t sig;

#if defined(OS_INCLUDE_RTOS_THREAD_PUBLIC_FLAGS_CLEAR)
  ((thread*) (thread_id))->flags_clear ((flags::mask_t) signals, &sig);
#else
  assert(((thread* ) (thread_id)) == &this_thread::thread ());
  // IGNORE THREAD ID!
  this_thread::flags_clear ((flags::mask_t) signals, &sig);
#endif
  return (int32_t) sig;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

/**
 * @details
 * Suspend the execution of the current RUNNING thread until all
 * specified signal flags with the parameter signals are set.
 * When the parameter signals is 0 the current RUNNING thread
 * is suspended until any signal is set. When these signal flags are
 * already set, the function returns instantly. Otherwise the thread
 * is put into the state WAITING. Signal flags that are reported as
 * event are automatically cleared.
 *
 * The argument millisec specifies how long the system waits for
 * the specified signal flags. While the system waits the tread
 * calling this function is put into the state WAITING. The timeout
 * value can have the following values:
 *
 * - when millisec is 0, the function returns instantly.
 * - when millisec is set to osWaitForever the function will wait
 * an infinite time until a specified signal is set.
 * - all other values specify a time in millisecond for a timeout.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osEvent
osSignalWait (int32_t signals, uint32_t millisec)
{
  osEvent event;

  if (interrupts::in_handler_mode ())
    {
      event.status = osErrorISR;
      return event;
    }

  if ((uint32_t) signals & 0x80000000)
    {
      event.status = osErrorValue;
      return event;
    }

  result_t res;
  if (millisec == osWaitForever)
    {
      res = this_thread::flags_wait ((flags::mask_t) signals,
                                     (flags::mask_t*) &event.value.signals);
    }
  else if (millisec == 0)
    {
      res = this_thread::flags_try_wait ((flags::mask_t) signals,
                                         (flags::mask_t*) &event.value.signals);
    }
  else
    {
      res = this_thread::flags_timed_wait (
          (flags::mask_t) signals,
          clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)),
          (flags::mask_t*) &event.value.signals);
    }

  if (res == result::ok)
    {
      event.status = osEventSignal;
    }
  else if (res == EWOULDBLOCK)
    {
      event.status = osOK; // Only for try_flags_wait().
    }
  else if (res == ETIMEDOUT)
    {
      event.status = osEventTimeout; // Only for timed_flags_wait().
    }
  else if (res == EPERM)
    {
      event.status = osErrorISR;
    }
  else if (res == EINVAL)
    {
      event.status = osErrorValue;
    }
  else
    {
      event.status = osErrorOS;
    }

  return event;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

//  ==== Mutex Management ====

/**
 * @details
 * Create and initialise a mutex object instance.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osMutexId
osMutexCreate (const osMutexDef_t* mutex_def)
{
  if (interrupts::in_handler_mode ())
    {
      return nullptr;
    }

  if (mutex_def == nullptr)
    {
      return nullptr;
    }

  mutex::attributes attr;
  attr.mx_type = mutex::type::recursive;
  attr.mx_protocol = mutex::protocol::inherit;

  new ((void*) mutex_def->data) mutex (mutex_def->name, attr);

  return reinterpret_cast<osMutexId> (mutex_def->data);
}

/**
 * @details
 * Wait until a mutex becomes available. If no other thread has
 * obtained the mutex, the function instantly returns and blocks
 * the mutex object.
 *
 * The argument millisec specifies how long the system waits for
 * a mutex. While the system waits the thread that is calling this
 * function is put into the state WAITING. The millisec timeout can
 * have the following values:
 *
 * - when millisec is 0, the function returns instantly.
 * - when millisec is set to osWaitForever the function will
 * wait for an infinite time until the mutex becomes available.
 * - all other values specify a time in millisecond for a timeout.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osMutexWait (osMutexId mutex_id, uint32_t millisec)
{
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (mutex_id == nullptr)
    {
      return osErrorParameter;
    }

  result_t ret;
  if (millisec == osWaitForever)
    {
      ret = (reinterpret_cast<rtos::mutex&> (*mutex_id)).lock ();
      // osErrorResource:
    }
  else if (millisec == 0)
    {
      ret = (reinterpret_cast<rtos::mutex&> (*mutex_id)).try_lock ();
    }
  else
    {
      ret = (reinterpret_cast<rtos::mutex&> (*mutex_id)).timed_lock (
          clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)));
      // osErrorTimeoutResource:
    }

  if (ret == result::ok)
    {
      // osOK: the mutex has been obtained.
      return osOK;
    }
  else if (ret == EWOULDBLOCK)
    {
      // The mutex could not be obtained when no timeout was specified.
      // Only for try_lock().
      return osErrorResource;
    }
  else if (ret == ENOTRECOVERABLE)
    {
      // The mutex could not be obtained when no timeout was specified.
      // Only for lock().
      return osErrorResource;
    }
  else if (ret == ETIMEDOUT)
    {
      // The mutex could not be obtained in the given time.
      // Only for timed_lock().
      return osErrorTimeoutResource;
    }
  else
    {
      return osErrorOS;
    }

  /* NOTREACHED */
}

/**
 * @details
 * Release a mutex that was obtained with osMutexWait. Other
 * threads that currently wait for the same mutex will be now
 * put into the state READY.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osMutexRelease (osMutexId mutex_id)
{
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (mutex_id == nullptr)
    {
      return osErrorParameter;
    }

  result_t res;
  res = (reinterpret_cast<rtos::mutex&> (*mutex_id)).unlock ();

  if (res == result::ok)
    {
      return osOK;
    }
  else if ((res == EPERM) || (res == ENOTRECOVERABLE))
    {
      return osErrorResource;
    }
  else
    {
      return osErrorOS;
    }
}

/**
 * @details
 * Delete a mutex object instance. The function releases internal memory
 * obtained for mutex handling. After this call the mutex_id is no
 * longer valid and cannot be used. The mutex may be created again
 * using the function osMutexCreate.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osMutexDelete (osMutexId mutex_id)
{
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (mutex_id == nullptr)
    {
      return osErrorParameter;
    }

  (reinterpret_cast<rtos::mutex&> (*mutex_id)).~mutex ();
  return osOK;
}

// ----------------------------------------------------------------------------

//  ==== Semaphore Management Functions ====

#if (defined (osFeature_Semaphore)  &&  (osFeature_Semaphore != 0))

/**
 * @details
 * Create and initialise a semaphore object that is used to manage
 * access to shared resources. The parameter count specifies
 * the number of available resources. The count value 1 creates
 * a binary semaphore.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osSemaphoreId
osSemaphoreCreate (const osSemaphoreDef_t* semaphore_def, int32_t count)
{
  if (interrupts::in_handler_mode ())
    {
      return nullptr;
    }

  if (semaphore_def == nullptr)
    {
      return nullptr;
    }

  semaphore::attributes attr;
  attr.sm_initial_value = (semaphore::count_t) count;
  // The logic is very strange, the CMSIS expects both the max-count to be the
  // same as count, and also to accept a count of 0, which leads to
  // useless semaphores. We patch this behaviour in the wrapper, the main
  // object uses a more realistic max_value.
  attr.sm_max_value = (semaphore::count_t) (
      count == 0 ? osFeature_Semaphore : count);

  new ((void*) semaphore_def->data) semaphore (semaphore_def->name, attr);

  return reinterpret_cast<osSemaphoreId> (semaphore_def->data);
}

/**
 * @details
 * Wait until a semaphore token becomes available. When no semaphore
 * token is available, the function waits for the time specified with
 * the parameter millisec.
 *
 * The argument millisec specifies how long the system waits for a
 * semaphore token to become available. While the system waits the
 * thread that is calling this function is put into the state WAITING.
 * The millisec timeout can have the following values:
 *
 * - when millisec is 0, the function returns instantly.
 * - when millisec is set to osWaitForever the function will wait
 * for an infinite time until the semaphore token becomes available.
 * - all other values specify a time in millisecond for a timeout.
 *
 * The return value indicates the number of available tokens (the
 * semaphore count value). If 0 is returned, then no semaphore was
 * available.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
int32_t
osSemaphoreWait (osSemaphoreId semaphore_id, uint32_t millisec)
{
  if (interrupts::in_handler_mode ())
    {
      return -1;
    }

  if (semaphore_id == nullptr)
    {
      return -1;
    }

  result_t res;
  if (millisec == osWaitForever)
    {
      res = (reinterpret_cast<rtos::semaphore&> (*semaphore_id)).wait ();
    }
  else if (millisec == 0)
    {
      res = (reinterpret_cast<rtos::semaphore&> (*semaphore_id)).try_wait ();
      if (res == EWOULDBLOCK)
        {
          return 0;
        }
    }
  else
    {
      res = (reinterpret_cast<rtos::semaphore&> (*semaphore_id)).timed_wait (
          clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)));
      if (res == ETIMEDOUT)
        {
          return 0;
        }
    }

  if (res == 0)
    {
      int count =
          (int32_t) (reinterpret_cast<rtos::semaphore&> (*semaphore_id)).value ();
      return count + 1;
    }
  else
    {
      return -1;
    }
}

/**
 * @details
 * Release a semaphore token. This increments the count of
 * available semaphore tokens.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
osStatus
osSemaphoreRelease (osSemaphoreId semaphore_id)
{
  if (semaphore_id == nullptr)
    {
      return osErrorParameter;
    }

  if ((reinterpret_cast<rtos::semaphore&> (*semaphore_id)).initial_value ()
      == 0)
    {
      return osErrorResource;
    }

  result_t res = (reinterpret_cast<rtos::semaphore&> (*semaphore_id)).post ();

  if (res == result::ok)
    {
      return osOK;
    }
  else if (res == EAGAIN)
    {
      return osErrorResource;
    }
  else
    {
      return osErrorOS;
    }
}

/**
 * @details
 * Delete a semaphore object instance. The function releases internal memory
 * obtained for semaphore handling. After this call the semaphore_id
 * is no longer valid and cannot be used. The semaphore may be created
 * again using the function osSemaphoreCreate.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osStatus
osSemaphoreDelete (osSemaphoreId semaphore_id)
{
  if (interrupts::in_handler_mode ())
    {
      return osErrorISR;
    }

  if (semaphore_id == nullptr)
    {
      return osErrorParameter;
    }

  (reinterpret_cast<rtos::semaphore&> (*semaphore_id)).~semaphore ();
  return osOK;
}

#endif /* Semaphore available */

// ----------------------------------------------------------------------------
//  ==== Memory Pool Management Functions ====

#if (defined (osFeature_Pool)  &&  (osFeature_Pool != 0))

/**
 * @details
 * Create and initialize a memory pool object instance.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osPoolId
osPoolCreate (const osPoolDef_t* pool_def)
{
  if (interrupts::in_handler_mode ())
    {
      return nullptr;
    }

  if (pool_def == nullptr)
    {
      return nullptr;
    }

  memory_pool::attributes attr;
  attr.mp_pool_address = pool_def->pool;
  attr.mp_pool_size_bytes = pool_def->pool_sz;

  new ((void*) pool_def->data) memory_pool (pool_def->name,
                                            (std::size_t) pool_def->items,
                                            (std::size_t) pool_def->item_sz,
                                            attr);

  return reinterpret_cast<osPoolId> (pool_def->data);
}

/**
 * @details
 * Allocate a memory block from the memory pool.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
void*
osPoolAlloc (osPoolId pool_id)
{
  if (pool_id == nullptr)
    {
      return nullptr;
    }

  return (reinterpret_cast<memory_pool&> (*pool_id)).try_alloc ();
}

/**
 * @details
 * Allocate a memory block from a memory pool and set memory block to zero.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
void*
osPoolCAlloc (osPoolId pool_id)
{
  if (pool_id == nullptr)
    {
      return nullptr;
    }

  void* ret;
  ret = (reinterpret_cast<memory_pool&> (*pool_id)).try_alloc ();
  if (ret != nullptr)
    {
      memset (ret, 0,
              (reinterpret_cast<memory_pool&> (*pool_id)).block_size ());
    }

  return ret;
}

/**
 * @details
 * Return an allocated memory block back to the memory pool.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
osStatus
osPoolFree (osPoolId pool_id, void* block)
{
  if (pool_id == nullptr)
    {
      return osErrorParameter;
    }

  if (block == nullptr)
    {
      return osErrorParameter;
    }

  result_t res;
  res = (reinterpret_cast<memory_pool&> (*pool_id)).free (block);

  if (res == result::ok)
    {
      return osOK;
    }
  else if (res == EINVAL)
    {
      return osErrorValue;
    }
  else
    {
      return osErrorOS;
    }
}

#endif /* Memory Pool Management available */

// ----------------------------------------------------------------------------
//  ==== Message Queue Management Functions ====

#if (defined (osFeature_MessageQ)  &&  (osFeature_MessageQ != 0))

/**
 * @details
 * Create and initialise a message queue object instance.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osMessageQId
osMessageCreate (const osMessageQDef_t* queue_def,
                 osThreadId thread_id __attribute__((unused)))
{
  if (interrupts::in_handler_mode ())
    {
      return nullptr;
    }

  if (queue_def == nullptr)
    {
      return nullptr;
    }

  message_queue::attributes attr;
  attr.mq_queue_address = queue_def->queue;
  attr.mq_queue_size_bytes = queue_def->queue_sz;

  new ((void*) queue_def->data) message_queue (queue_def->name,
                                               (std::size_t) queue_def->items,
                                               (std::size_t) queue_def->item_sz,
                                               attr);

  return reinterpret_cast<osMessageQId> (queue_def->data);
}

/**
 * @details
 * Put the message info in a message queue specified by queue_id.
 *
 * When the message queue is full, the system retries for a specified
 * time with millisec. While the system retries the thread that is
 * calling this function is put into the state WAITING. The millisec
 * timeout can have the following values:
 *
 * - when millisec is 0, the function returns instantly.
 * - when millisec is set to osWaitForever the function will wait
 *  for an infinite time until a message queue slot becomes available.
 * - all other values specify a time in millisecond for a timeout.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
osStatus
osMessagePut (osMessageQId queue_id, uint32_t info, uint32_t millisec)
{
#pragma GCC diagnostic push
#if defined ( __clang__ )
#pragma clang diagnostic ignored "-Wint-to-pointer-cast"
#endif

  if (queue_id == nullptr)
    {
      return osErrorParameter;
    }

  result_t res;
  if (millisec == osWaitForever)
    {
      if (interrupts::in_handler_mode ())
        {
          return osErrorParameter;
        }
      res = (reinterpret_cast<message_queue&> (*queue_id)).send (
          (const char*) &info, sizeof(uint32_t), 0);
      // osOK, osErrorResource, osErrorParameter
    }
  else if (millisec == 0)
    {
      res = (reinterpret_cast<message_queue&> (*queue_id)).try_send (
          (const char*) &info, sizeof(uint32_t), 0);
      // osOK, osErrorResource, osErrorParameter
    }
  else
    {
      if (interrupts::in_handler_mode ())
        {
          return osErrorParameter;
        }
      res = (reinterpret_cast<message_queue&> (*queue_id)).timed_send (
          (const char*) &info, sizeof(uint32_t),
          clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)), 0);
      // osOK, osErrorTimeoutResource, osErrorParameter
    }

  if (res == result::ok)
    {
      // The message was put into the queue.
      return osOK;
    }
  else if (res == EWOULDBLOCK)
    {
      // No memory in the queue was available
      return osErrorResource;
    }
  else if (res == ETIMEDOUT)
    {
      // No memory in the queue was available during the given time limit.
      return osErrorTimeoutResource;
    }
  else if (res == EINVAL || res == EMSGSIZE)
    {
      // A parameter is invalid or outside of a permitted range.
      return osErrorParameter;
    }
  else
    {
      return osErrorOS;
    }

#pragma GCC diagnostic pop
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

/**
 * @details
 * Suspend the execution of the current RUNNING thread until a
 * message arrives. When a message is already in the queue,
 * the function returns instantly with the message information.
 *
 * The argument millisec specifies how long the system waits for
 * a message to become available. While the system waits the thread
 * that is calling this function is put into the state WAITING.
 * The millisec timeout value can have the following values:
 *
 * - when millisec is 0, the function returns instantly.
 * - when millisec is set to osWaitForever the function will
 * wait for an infinite time until a message arrives.
 * - all other values specify a time in millisecond for a timeout.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
osEvent
osMessageGet (osMessageQId queue_id, uint32_t millisec)
{
  osEvent event;
  result_t res;

  if (queue_id == nullptr)
    {
      event.status = osErrorParameter;
      return event;
    }

  if (millisec == osWaitForever)
    {
      if (interrupts::in_handler_mode ())
        {
          event.status = osErrorParameter;
          return event;
        }
      res = (reinterpret_cast<message_queue&> (*queue_id)).receive (
          (char*) &event.value.v, sizeof(uint32_t), nullptr);
      // result::event_message;
    }
  else if (millisec == 0)
    {
      res = (reinterpret_cast<message_queue&> (*queue_id)).try_receive (
          (char*) &event.value.v, sizeof(uint32_t), nullptr);
      // result::event_message when message;
      // result::ok when no meessage
    }
  else
    {
      if (interrupts::in_handler_mode ())
        {
          event.status = osErrorParameter;
          return event;
        }
      res = (reinterpret_cast<message_queue&> (*queue_id)).timed_receive (
          (char*) &event.value.v, sizeof(uint32_t),
          clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)), nullptr);
      // result::event_message when message;
      // result::event_timeout when timeout;
    }

  if (res == result::ok)
    {
      // Message received, value.p contains the pointer to message.
      event.status = osEventMessage;
    }
  else if (res == ETIMEDOUT)
    {
      // No message has arrived during the given timeout period.
      event.status = osEventTimeout;
    }
  else if (res == EINVAL || res == EMSGSIZE)
    {
      // A parameter is invalid or outside of a permitted range.
      event.status = osErrorParameter;
    }
  else if (res == EWOULDBLOCK)
    {
      // No message is available in the queue and no timeout was specified.
      event.status = osOK;
    }
  else
    {
      event.status = osErrorOS;
    }

  return event;
}

#pragma GCC diagnostic pop

#endif /* Message Queues available */

// ----------------------------------------------------------------------------
//  ==== Mail Queue Management Functions ====

#if (defined (osFeature_MailQ)  &&  (osFeature_MailQ != 0))

/**
 * @details
 * Create and initialise a mail queue object instance.
 *
 * @warning Cannot be invoked from Interrupt Service Routines.
 */
osMailQId
osMailCreate (const osMailQDef_t* mail_def,
              osThreadId thread_id __attribute__((unused)))
{
  if (interrupts::in_handler_mode ())
    {
      return nullptr;
    }

  if (mail_def == nullptr)
    {
      return nullptr;
    }

  memory_pool::attributes pool_attr;
  pool_attr.mp_pool_address = mail_def->pool;
  pool_attr.mp_pool_size_bytes = mail_def->pool_sz;
  new ((void*) &mail_def->data->pool) memory_pool (
      mail_def->name, (std::size_t) mail_def->items,
      (std::size_t) mail_def->pool_item_sz, pool_attr);

  message_queue::attributes queue_attr;
  queue_attr.mq_queue_address = mail_def->queue;
  queue_attr.mq_queue_size_bytes = mail_def->queue_sz;
  new ((void*) &mail_def->data->queue) message_queue (
      mail_def->name, (std::size_t) mail_def->items,
      (std::size_t) mail_def->queue_item_sz, queue_attr);

  return (osMailQId) (mail_def->data);
}

/**
 * @details
 * Allocate a memory block from the mail queue that is filled
 * with the mail information.
 *
 * The argument queue_id specifies a mail queue identifier that
 * is obtain with osMailCreate.
 *
 * The argument millisec specifies how long the system waits for
 * a mail slot to become available. While the system waits the
 * tread calling this function is put into the state WAITING.
 * The millisec timeout can have the following values:
 *
 * - when millisec is 0, the function returns instantly.
 * - when millisec is set to osWaitForever the function will
 * wait for an infinite time until a mail slot can be allocated.
 * - all other values specify a time in millisecond for a timeout.
 *
 * A NULL pointer is returned when no memory slot can be obtained
 * or queue specifies an illegal parameter.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
void*
osMailAlloc (osMailQId mail_id, uint32_t millisec)
{
  if (mail_id == nullptr)
    {
      return nullptr;
    }

  void* ret = nullptr;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
  if (millisec == osWaitForever)
    {
      if (interrupts::in_handler_mode ())
        {
          return nullptr;
        }
      ret = (reinterpret_cast<memory_pool&> (mail_id->pool)).alloc ();
    }
  else if (millisec == 0)
    {
      ret = (reinterpret_cast<memory_pool&> (mail_id->pool)).try_alloc ();
    }
  else
    {
      if (interrupts::in_handler_mode ())
        {
          return nullptr;
        }
      ret = (reinterpret_cast<memory_pool&> (mail_id->pool)).timed_alloc (
          clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)));
    }
#pragma GCC diagnostic pop
  return ret;
}

/**
 * @details
 * Allocate a memory block from the mail queue that is filled with
 * the mail information. The memory block returned is cleared.
 *
 * The argument queue_id specifies a mail queue identifier that is
 * obtain with osMailCreate.
 *
 * The argument millisec specifies how long the system waits for a
 * mail slot to become available. While the system waits the thread
 * that is calling this function is put into the state WAITING.
 * The millisec timeout can have the following values:
 *
 * - when millisec is 0, the function returns instantly.
 * - when millisec is set to osWaitForever the function will
 * wait for an infinite time until a mail slot can be allocated.
 * - all other values specify a time in millisecond for a timeout.
 *
 * A NULL pointer is returned when no memory block can be obtained
 * or queue specifies an illegal parameter.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
void*
osMailCAlloc (osMailQId mail_id, uint32_t millisec)
{
  void* ret = osMailAlloc (mail_id, millisec);
  if (ret != nullptr)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
      memset (ret, 0,
              (reinterpret_cast<memory_pool&> (mail_id->pool)).block_size ());
#pragma GCC diagnostic pop
    }
  return ret;
}

/**
 * @details
 * Put the memory block specified with mail into the mail queue
 * specified by queue.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
osStatus
osMailPut (osMailQId mail_id, void* mail)
{
  if (mail_id == nullptr)
    {
      return osErrorParameter;
    }
  if (mail == nullptr)
    {
      return osErrorValue;
    }

  // Validate pointer.
  memory_pool* pool = reinterpret_cast<memory_pool*> (&mail_id->pool);
  if (((char*) mail < (char*) (pool->pool ()))
      || (((char*) mail)
          >= ((char*) (pool->pool ()) + pool->capacity () * pool->block_size ())))
    {
      return osErrorValue;
    }

  result_t res;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
  res = (reinterpret_cast<message_queue&> (mail_id->queue)).try_send (
      (const char*) &mail, sizeof(void*), 0);
#pragma GCC diagnostic pop
  if (res == result::ok)
    {
      return osOK;
    }
  else
    {
      return osErrorOS;
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"

/**
 * @details
 * Suspend the execution of the current RUNNING thread until a mail
 * arrives. When a mail is already in the queue, the function returns
 * instantly with the mail information.
 *
 * The argument millisec specifies how long the system waits for a
 * mail to arrive. While the system waits the thread that is calling
 * this function is put into the state WAITING. The millisec timeout
 * can have the following values:
 *
 * - when millisec is 0, the function returns instantly.
 * - when millisec is set to osWaitForever the function will wait
 * for an infinite time until a mail arrives.
 * - all other values specify a time in millisecond for a timeout.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
osEvent
osMailGet (osMailQId mail_id, uint32_t millisec)
{
  osEvent event;
  result_t res;

  if (mail_id == nullptr)
    {
      event.status = osErrorParameter;
      return event;
    }

  if (millisec == osWaitForever)
    {
      if (interrupts::in_handler_mode ())
        {
          event.status = osErrorParameter;
          return event;
        }
      res = (reinterpret_cast<message_queue&> ((mail_id->queue))).receive (
          (char*) &event.value.p, sizeof(void*), nullptr);
      // osEventMail for ok,
    }
  else if (millisec == 0)
    {
      res = (reinterpret_cast<message_queue&> (mail_id->queue)).try_receive (
          (char*) &event.value.p, sizeof(void*), nullptr);
      // osEventMail for ok,
    }
  else
    {
      if (interrupts::in_handler_mode ())
        {
          event.status = osErrorParameter;
          return event;
        }
      res = (reinterpret_cast<message_queue&> (mail_id->queue)).timed_receive (
          (char*) &event.value.p, sizeof(void*),
          clock_systick::ticks_cast ((uint64_t) (millisec * 1000u)), nullptr);
      // osEventMail for ok, osEventTimeout
    }

  if (res == result::ok)
    {
      // Mail received, value.p contains the pointer to mail content.
      event.status = osEventMail;
    }
  else if (res == EWOULDBLOCK)
    {
      // No mail is available in the queue and no timeout was specified.
      event.status = osOK;
    }
  else if (res == EINVAL || res == EMSGSIZE)
    {
      // A parameter is invalid or outside of a permitted range.
      event.status = osErrorParameter;
    }
  else if (res == ETIMEDOUT)
    {
      // No mail has arrived during the given timeout period.
      event.status = osEventTimeout;
    }
  else
    {
      event.status = osErrorOS;
    }

  return event;
}

#pragma GCC diagnostic pop

/**
 * @details
 * Free the memory block specified by mail and return it to the mail queue.
 *
 * @note Can be invoked from Interrupt Service Routines.
 */
osStatus
osMailFree (osMailQId mail_id, void* mail)
{
  if (mail_id == nullptr)
    {
      return osErrorParameter;
    }
  if (mail == nullptr)
    {
      return osErrorValue;
    }

  return osPoolFree (&(mail_id->pool), mail);
}

#endif /* Mail Queues available */

// ----------------------------------------------------------------------------

#pragma GCC diagnostic pop
