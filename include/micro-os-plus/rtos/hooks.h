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

#ifndef MICRO_OS_PLUS_RTOS_HOOKS_H_
#define MICRO_OS_PLUS_RTOS_HOOKS_H_

// ----------------------------------------------------------------------------

#include <stddef.h>
#include <stdbool.h>

// ----------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

  /**
   * @addtogroup micro-os-plus-app-hooks
   * @{
   */

  /**
   * @name Startup Routines
   * @{
   */

  /**
   * @brief The standard C application entry point.
   * @par Parameters
   *  None.
   * @par Returns
   *  Does not return.
   */
  void __attribute__ ((noreturn)) _start (void);

  /**
   * @brief Create the idle thread.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_startup_create_thread_idle (void);

  /**
   * @}
   */

  /**
   * @name Hooks
   * @{
   */

  /**
   * @brief Hook to enter a power saving mode.
   * @par Parameters
   *  None.
   * @retval true The hook entered a power saving mode.
   * @retval false The hook did not enter a power saving mode.
   */
  bool
  micro_os_plus_rtos_idle_enter_power_saving_mode_hook (void);

  /**
   * @brief Hook to handle out of memory in the application free store.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_rtos_application_out_of_memory_hook (void);

  /**
   * @brief Hook to handle out of memory in the RTOS dynamic memory.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   */
  void
  micro_os_plus_rtos_system_out_of_memory_hook (void);

  /**
   * @}
   */

  /**
   * @}
   */

#if defined(__cplusplus)
}
#endif // __cplusplus

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_RTOS_HOOKS_H_

// ----------------------------------------------------------------------------
