/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2018 Liviu Ionescu.
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

#include <micro-os-plus/posix-io/block-device-partition.h>

#include <micro-os-plus/diag/trace.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ========================================================================

    block_device_partition::block_device_partition (block_device_impl& impl,
                                                    const char* name) :
        block_device
          { impl, name }
    {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition::%s(\"%s\")=@%p\n", __func__,
                     name_, this);
#endif
    }

    block_device_partition::~block_device_partition ()
    {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition::%s() @%p %s\n", __func__, this,
                     name_);
#endif
    }

    // ------------------------------------------------------------------------

    void
    block_device_partition::configure (blknum_t offset, blknum_t nblocks)
    {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition::%s(%u,%u) @%p\n", __func__,
                     offset, nblocks, this);
#endif

      impl ().configure (offset, nblocks);
    }

    // ========================================================================

    block_device_partition_impl::block_device_partition_impl (
        block_device& parent) :
        parent_ (parent)
    {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition_impl::%s()=@%p\n", __func__, this);
#endif
    }

    block_device_partition_impl::~block_device_partition_impl ()
    {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition_impl::%s() @%p\n", __func__, this);
#endif
    }

    // ----------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    int
    block_device_partition_impl::do_vioctl (int request, std::va_list args)
    {
      errno = ENOSYS;
      return -1;
    }

#pragma GCC diagnostic pop

    void
    block_device_partition_impl::configure (blknum_t offset, blknum_t nblocks)
    {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition_impl::%s(%u,%u) @%p\n", __func__,
                     offset, nblocks, this);
#endif

      partition_offset_blocks_ = offset;
      assert(nblocks > 0);
      num_blocks_ = nblocks;

      // Inherit from parent.
      block_logical_size_bytes_ = parent_.block_logical_size_bytes ();
      block_physical_size_bytes_ = parent_.block_physical_size_bytes ();
    }

    int
    block_device_partition_impl::do_vopen (const char* path, int oflag,
                                           std::va_list args)
    {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition_impl::%s(%d) @%p\n", __func__,
                     oflag, this);
#endif

      return parent_.vopen (path, oflag, args);
    }

    ssize_t
    block_device_partition_impl::do_read_block (void* buf, blknum_t blknum,
                                                std::size_t nblocks)
    {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition_impl::%s(0x%X, %u, %u) @%p\n",
                     __func__, buf, blknum, nblocks, this);
#endif

      return parent_.read_block (buf, blknum + partition_offset_blocks_,
                                 nblocks);
    }

    ssize_t
    block_device_partition_impl::do_write_block (const void* buf,
                                                 blknum_t blknum,
                                                 std::size_t nblocks)
    {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition_impl::%s(0x%X, %u, %u) @%p\n",
                     __func__, buf, blknum, nblocks, this);
#endif

      return parent_.write_block (buf, blknum + partition_offset_blocks_,
                                  nblocks);
    }

    void
    block_device_partition_impl::do_sync (void)
    {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition_impl::%s() @%p\n", __func__, this);
#endif

      return parent_.sync ();
    }

    int
    block_device_partition_impl::do_close (void)
    {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition_impl::%s() @%p\n", __func__, this);
#endif

      return parent_.close ();
    }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
