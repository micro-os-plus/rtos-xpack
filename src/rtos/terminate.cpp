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

#include <micro-os-plus/rtos.h>
#include <micro-os-plus/estd/memory_resource>
#include <micro-os-plus/startup/hooks.h>

// ----------------------------------------------------------------------------

using namespace micro_os_plus;

// ----------------------------------------------------------------------------

extern rtos::thread* micro_os_plus_main_thread;

void
micro_os_plus_terminate_goodbye (void)
{
#if defined(MICRO_OS_PLUS_TRACE)

  trace::printf ("\n");

#if !defined(MICRO_OS_PLUS_EXCLUDE_DYNAMIC_MEMORY_ALLOCATIONS)

  // Application memory.
  estd::pmr::get_default_resource ()->trace_print_statistics ();

#if defined(MICRO_OS_PLUS_INTEGER_RTOS_DYNAMIC_MEMORY_SIZE_BYTES)
  rtos::memory::get_default_resource ()->trace_print_statistics ();
#endif // defined(MICRO_OS_PLUS_INTEGER_RTOS_DYNAMIC_MEMORY_SIZE_BYTES)

#endif // !defined(MICRO_OS_PLUS_EXCLUDE_DYNAMIC_MEMORY_ALLOCATIONS)

  class rtos::thread::stack& st = micro_os_plus_main_thread->stack ();

  trace::printf ("Main thread stack: %u/%u bytes used\n",
                 st.size () - st.available (), st.size ());

#if defined(MICRO_OS_PLUS_HAS_INTERRUPTS_STACK)
  trace::printf ("Interrupts stack: %u/%u bytes used\n",
                 rtos::interrupts::stack ()->size ()
                     - rtos::interrupts::stack ()->available (),
                 rtos::interrupts::stack ()->size ());
#endif // defined(MICRO_OS_PLUS_HAS_INTERRUPTS_STACK)

  trace::printf ("\nHasta la Vista!\n");

#endif // defined(MICRO_OS_PLUS_TRACE)
}
// ----------------------------------------------------------------------------
