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

#include <micro-os-plus/rtos.h>
#include <micro-os-plus/estd/memory_resource>

// ----------------------------------------------------------------------------

using namespace os;

// ----------------------------------------------------------------------------

extern rtos::thread* os_main_thread;

void
os_terminate_goodbye (void)
{
#if defined(TRACE)

  trace::printf ("\n");

#if !defined(OS_EXCLUDE_DYNAMIC_MEMORY_ALLOCATIONS)

  // Application memory.
  estd::pmr::get_default_resource ()->trace_print_statistics ();

#if defined(OS_INTEGER_RTOS_DYNAMIC_MEMORY_SIZE_BYTES)
  rtos::memory::get_default_resource ()->trace_print_statistics ();
#endif // defined(OS_INTEGER_RTOS_DYNAMIC_MEMORY_SIZE_BYTES)

#endif // !defined(OS_EXCLUDE_DYNAMIC_MEMORY_ALLOCATIONS)

  class rtos::thread::stack& st = os_main_thread->stack ();

  trace::printf ("Main thread stack: %u/%u bytes used\n",
                 st.size () - st.available (), st.size ());

#if defined(OS_HAS_INTERRUPTS_STACK)
  trace::printf ("Interrupts stack: %u/%u bytes used\n",
                 rtos::interrupts::stack ()->size ()
                     - rtos::interrupts::stack ()->available (),
                 rtos::interrupts::stack ()->size ());
#endif // defined(OS_HAS_INTERRUPTS_STACK)

  trace::printf ("\nHasta la Vista!\n");

#endif // defined(TRACE)
}
// ----------------------------------------------------------------------------
