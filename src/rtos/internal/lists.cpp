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

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_INCLUDE_CONFIG_H)
#include <micro-os-plus/config.h>
#endif // MICRO_OS_PLUS_INCLUDE_CONFIG_H

#include <micro-os-plus/rtos.h>

#include <micro-os-plus/diag/trace.h>

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
      // ======================================================================

      void
      thread_children_list::link (thread& thread)
      {
        // Add thread intrusive node at the end of the list.
        insert_after (thread.child_links_,
                      const_cast<utils::static_double_list_links*> (tail ()));
      }

      // ======================================================================

      void
      ready_threads_list::link (waiting_thread_node& node)
      {
        if (head_.previous () == nullptr)
          {
            // If this is the first time, initialise the list to empty.
            clear ();
          }

        thread::priority_t prio = node.thread_->priority ();

        waiting_thread_node* after = static_cast<waiting_thread_node*> (
            const_cast<utils::static_double_list_links*> (tail ()));

        if (empty ())
          {
            // Insert at the end of the list.
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS)
            trace::printf ("ready %s() empty +%u\n", __func__, prio);
#endif
          }
        else if (prio <= after->thread_->priority ())
          {
            // Insert at the end of the list.
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS)
            trace::printf ("ready %s() back %u +%u \n", __func__,
                           after->thread_->priority (), prio);
#endif
          }
        else if (prio > head ()->thread_->priority ())
          {
            // Insert at the beginning of the list.
            after = static_cast<waiting_thread_node*> (&head_);
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS)
            trace::printf ("ready %s() front +%u %u \n", __func__, prio,
                           head ()->thread_->priority ());
#endif
          }
        else
          {
            // Insert in the middle of the list.
            // The loop is guaranteed to terminate, and not hit the head.
            // The weight is relatively small, priority() is not heavy.
            while (prio > after->thread_->priority ())
              {
                after = static_cast<waiting_thread_node*> (after->previous ());
              }
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS)
            trace::printf ("ready %s() middle %u +%u \n", __func__,
                           after->thread_->priority (), prio);
#endif
          }

        insert_after (node, after);

        node.thread_->state_ = thread::state::ready;
      }

      /**
       * @details
       * Must be called in a critical section.
       */
      thread*
      ready_threads_list::unlink_head (void)
      {
        assert (!empty ());

        thread* th = head ()->thread_;

#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS)
        trace::printf ("ready %s() %p %s\n", __func__, th, th->name ());
#endif

        const_cast<waiting_thread_node*> (head ())->unlink ();

        assert (th != nullptr);

        // Unlinking is immediately followed by a context switch,
        // so in order to guarantee that the thread is marked as
        // running, it is saver to do it here.

        th->state_ = thread::state::running;
        return th;
      }

      // ======================================================================

      /**
       * @class waiting_threads_list
       * @details
       * There are at least two strategies:
       * - keep the list ordered by priorities and have the top node
       *  easily accessible the head
       * - preserve the insertion order and perform a full list traversal
       *  to determine the top node.
       *
       * The first strategy requires a partial list traverse with each
       * insert, to find the place to insert the node, but makes
       * retrieving the top priority node trivial, by a single access
       * to the list head.
       *
       * The second strategy might minimise the overall processing
       * time, but always requires a full list traversal to determine
       * the top priority node.
       *
       * On the other hand, typical waiting lists contain only one
       * element, and in this case there is no distinction. Mutex
       * objects occasionally might have two entries (and rarely more).
       * Condition variables might also have several waiting threads,
       * the number is usually small. In these cases, the distinction
       * between the two strategies is also minimal.
       *
       * In the rare cases when the waiting list is large, the first
       * strategy favours top node retrieval, possibly
       * improving the response time, and is thus preferred.
       */

      /**
       * @details
       * Based on priority, the node is inserted
       * - at the end of the list,
       * - at the beginning of the list,
       * - in the middle of the list, which
       * requires a partial list traversal (done from the end).
       *
       * To satisfy the circular double linked list requirements,
       * an empty list still contains the head node with references
       * to itself.
       */
      void
      waiting_threads_list::link (waiting_thread_node& node)
      {
        thread::priority_t prio = node.thread_->priority ();

        waiting_thread_node* after = static_cast<waiting_thread_node*> (
            const_cast<utils::static_double_list_links*> (tail ()));

        if (empty ())
          {
            // Insert at the end of the list.
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS)
            trace::printf ("wait %s() empty +%u\n", __func__, prio);
#endif
          }
        else if (prio <= after->thread_->priority ())
          {
            // Insert at the end of the list.
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS)
            trace::printf ("wait %s() back %u +%u \n", __func__,
                           after->thread_->priority (), prio);
#endif
          }
        else if (prio > head ()->thread_->priority ())
          {
            // Insert at the beginning of the list.
            after = static_cast<waiting_thread_node*> (&head_);
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS)
            trace::printf ("wait %s() front +%u %u \n", __func__, prio,
                           head ()->thread_->priority ());
#endif
          }
        else
          {
            // Insert in the middle of the list.
            // The loop is guaranteed to terminate, and not hit the head.
            // The weight is relatively small, priority() is not heavy.
            while (prio > after->thread_->priority ())
              {
                after = static_cast<waiting_thread_node*> (after->previous ());
              }
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS)
            trace::printf ("wait %s() middle %u +%u \n", __func__,
                           after->thread_->priority (), prio);
#endif
          }

        insert_after (node, after);
      }

      /**
       * @details
       * Atomically get the top thread from the list, remove the node
       * and wake-up the thread.
       */
      bool
      waiting_threads_list::resume_one (void)
      {
        thread* th;
        {
          // ----- Enter critical section -----------------------------------
          interrupts::critical_section ics;

          // If the list is empty, silently return.
          if (empty ())
            {
              return false;
            }

          // The top priority is to remove the entry from the list
          // so that subsequent wakeups to address different threads.
          th = head ()->thread_;
          const_cast<waiting_thread_node*> (head ())->unlink ();
          // ----- Exit critical section ------------------------------------
        }
        assert (th != nullptr);

        thread::state_t state = th->state ();
        if (state != thread::state::destroyed)
          {
            th->resume ();
          }
        else
          {
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS)
            trace::printf ("%s() gone \n", __func__);
#endif
          }

        return true;
      }

      void
      waiting_threads_list::resume_all (void)
      {
        while (resume_one ()) {
          // Loop while there are treads in the list.
        }
      }

      // ======================================================================

      timestamp_node::timestamp_node (clock::timestamp_t ts) : timestamp (ts)
      {
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS_CONSTRUCT)
        trace::printf ("%s() %p \n", __func__, this);
#endif
      }

      timestamp_node::~timestamp_node ()
      {
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS_CONSTRUCT)
        trace::printf ("%s() %p \n", __func__, this);
#endif
      }

      // ======================================================================

      timeout_thread_node::timeout_thread_node (clock::timestamp_t ts,
                                                rtos::thread& th)
          : timestamp_node{ ts }, //
            thread (th)
      {
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS_CONSTRUCT)
        trace::printf ("%s() %p \n", __func__, this);
#endif
      }

      timeout_thread_node::~timeout_thread_node ()
      {
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS_CONSTRUCT)
        trace::printf ("%s() %p \n", __func__, this);
#endif
      }

      // Must be called in a critical section.
      void
      timeout_thread_node::action (void)
      {
        rtos::thread* th = &this->thread;
        this->unlink ();

        thread::state_t state = th->state ();
        if (state != thread::state::destroyed)
          {
            th->resume ();
          }
      }

      // ======================================================================

#if !defined(MICRO_OS_PLUS_USE_RTOS_PORT_TIMER)

      timer_node::timer_node (clock::timestamp_t ts, timer& tm)
          : timestamp_node{ ts }, //
            tmr (tm)
      {
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS_CONSTRUCT)
        trace::printf ("%s() %p \n", __func__, this);
#endif
      }

      timer_node::~timer_node ()
      {
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS_CONSTRUCT)
        trace::printf ("%s() %p \n", __func__, this);
#endif
      }

      /**
       * @details
       * Remove the node from the list and perform the timer actions.
       */
      void
      timer_node::action (void)
      {
        this->unlink ();
        tmr.internal_interrupt_service_routine ();
      }

#endif

      // ======================================================================

      /**
       * @details
       * The list is kept in ascending time stamp order.
       *
       * Based on time stamp, the node is inserted
       * - at the end of the list,
       * - at the beginning of the list,
       * - in the middle of the list, which
       * requires a partial list traversal (done from the end).
       *
       * To satisfy the circular double linked list requirements,
       * an empty list still contains the head node with references
       * to itself.
       */
      void
      clock_timestamps_list::link (timestamp_node& node)
      {
        clock::timestamp_t timestamp = node.timestamp;

        timeout_thread_node* after = static_cast<timeout_thread_node*> (
            const_cast<utils::static_double_list_links*> (tail ()));

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnull-dereference"
        if (empty ())
          {
            // Insert at the end of the list.
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS_CLOCKS)
            trace::printf ("clock %s() empty +%u\n", __func__,
                           static_cast<uint32_t> (timestamp));
#endif
          }
        else if (timestamp >= after->timestamp)
          {
            // Insert at the end of the list.
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS_CLOCKS)
            trace::printf ("clock %s() back %u +%u\n", __func__,
                           static_cast<uint32_t> (after->timestamp),
                           static_cast<uint32_t> (timestamp));
#endif
          }
        else if (timestamp < head ()->timestamp)
          {
            // Insert at the beginning of the list
            // and update the new head.
            after = static_cast<timeout_thread_node*> (&head_);
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS_CLOCKS)
            trace::printf ("clock %s() front +%u %u\n", __func__,
                           static_cast<uint32_t> (timestamp),
                           static_cast<uint32_t> (head ()->timestamp));
#endif
          }
        else
          {
            // Insert in the middle of the list.
            // The loop is guaranteed to terminate.
            while (timestamp < after->timestamp)
              {
                after = static_cast<timeout_thread_node*> (after->previous ());
              }
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS_CLOCKS)
            trace::printf ("clock %s() middle %u +%u\n", __func__,
                           static_cast<uint32_t> (after->timestamp),
                           static_cast<uint32_t> (timestamp));
#endif
          }
#pragma GCC diagnostic pop

        insert_after (node, after);
      }

      /**
       * @details
       * With the list ordered, check if the list head time stamp was
       * reached and run the node action.
       *
       * Repeat for all nodes that have overdue time stamps.
       */
      void
      clock_timestamps_list::check_timestamp (clock::timestamp_t now)
      {
        if (head_.next () == nullptr)
          {
            // This happens before the constructors are executed.
            return;
          }

        // Multiple threads can wait for the same time stamp, so
        // iterate until a node with future time stamp is identified.
        for (;;)
          {
            // ----- Enter critical section -----------------------------------
            interrupts::critical_section ics;

            if (empty ())
              {
                break;
              }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnull-dereference"
            clock::timestamp_t head_ts = head ()->timestamp;
#pragma GCC diagnostic pop
            if (now >= head_ts)
              {
#if defined(MICRO_OS_PLUS_TRACE_RTOS_LISTS_CLOCKS)
                trace::printf ("%s() %u \n", __func__,
                               static_cast<uint32_t> (sysclock.now ()));
#endif
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnull-dereference"
                const_cast<timestamp_node*> (head ())->action ();
#pragma GCC diagnostic pop
              }
            else
              {
                break;
              }
            // ----- Exit critical section ------------------------------------
          }
      }

      // ======================================================================

      void
      terminated_threads_list::link (waiting_thread_node& node)
      {
        if (head_.previous () == nullptr)
          {
            // If this is the first time, initialise the list to empty.
            clear ();
          }

        waiting_thread_node* after = static_cast<waiting_thread_node*> (
            const_cast<utils::static_double_list_links*> (tail ()));

#if defined(MICRO_OS_PLUS_TRACE_RTOS_THREAD)
        trace::printf ("terminated %s() %p %s\n", __func__, &node.thread_,
                       node.thread_->name ());
#endif

        node.thread_->state_ = thread::state::terminated;

        insert_after (node, after);
      }

      // ----------------------------------------------------------------------
    } // namespace internal
  } // namespace rtos
} // namespace micro_os_plus

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
