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
#include <micro-os-plus/diag/trace.h>
#include <micro-os-plus/rtos/internal/event-flags.h>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

namespace micro_os_plus
{
  namespace rtos
  {
    namespace internal
    {
      // ----------------------------------------------------------------------

      result_t
      event_flags::raise (flags::mask_t mask, flags::mask_t* oflags)
      {
        micro_os_plus_assert_err (mask != 0, EINVAL);

        assert (port::interrupts::is_priority_valid ());

        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          if (oflags != nullptr)
            {
              *oflags = flags_mask_;
            }

          flags_mask_ = flags_mask_ | mask; // Volatile set bits.

          // ----- Exit critical section --------------------------------------
        }
        return result::ok;
      }

      bool
      event_flags::check_raised (flags::mask_t mask, flags::mask_t* oflags,
                                 flags::mode_t mode)
      {
        if (mask == flags::any)
          {
            // Any flag will do it.
            if (flags_mask_ != 0)
              {
                if (oflags != nullptr)
                  {
                    *oflags = flags_mask_;
                  }

                if (mode & flags::mode::clear)
                  {
                    // Clear them all.
                    flags_mask_ = 0;
                  }
                return true;
              }
          }
        else if (((((mode & flags::mode::all) != 0))
                  && ((flags_mask_ & mask) == mask))
                 || (((mode & flags::mode::any) != 0)
                     && ((flags_mask_ & mask) != 0)))
          {
            if (oflags != nullptr)
              {
                *oflags = (flags_mask_ & mask);
              }

            if (mode & flags::mode::clear)
              {
                // Clear desired flags.
                flags_mask_ = flags_mask_ & ~mask; // Volatile clear bits.
              }
            return true;
          }

        return false;
      }

      flags::mask_t
      event_flags::get (flags::mask_t mask, flags::mode_t mode)
      {
        flags::mask_t ret;
        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          if (mask == 0)
            {
              // Return the entire mask.
              ret = flags_mask_;
            }
          else
            {
              ret = flags_mask_ & mask;
              if ((mode & flags::mode::clear) != 0)
                {
                  // Clear the selected bits; leave the rest untouched.
                  flags_mask_ = flags_mask_ & ~mask; // Volatile clear bits.
                }
            }
          // ----- Exit critical section --------------------------------------
        }

        // Return the selected bits.
        return ret;
      }

      result_t
      event_flags::clear (flags::mask_t mask, flags::mask_t* oflags)
      {
        micro_os_plus_assert_err (mask != 0, EINVAL);

        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          if (oflags != nullptr)
            {
              *oflags = flags_mask_;
            }

          // Clear the selected bits; leave the rest untouched.
          flags_mask_ = flags_mask_ & ~mask; // Volatile clear bits.

          // ----- Exit critical section --------------------------------------
        }

        return result::ok;
      }

      // --------------------------------------------------------------------------

    } // namespace internal
  } // namespace rtos
} // namespace micro_os_plus

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
