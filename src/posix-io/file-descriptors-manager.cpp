/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
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

#include <micro-os-plus/posix-io/file-descriptors-manager.h>
#include <micro-os-plus/posix-io/io.h>
#include <micro-os-plus/posix-io/socket.h>

#include <micro-os-plus/diag/trace.h>

#include <cerrno>
#include <cassert>
#include <cstddef>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    /**
     * @cond ignore
     */

    std::size_t file_descriptors_manager::size__;

    io** file_descriptors_manager::descriptors_array__;

    /**
     * @endcond
     */

    // ========================================================================
    file_descriptors_manager::file_descriptors_manager (std::size_t size)
    {
      trace::printf ("file_descriptors_manager::%s(%d)=%p\n", __func__, size,
                     this);

      assert(size > 0);

      size__ = size + reserved__; // Add space for standard files.
      descriptors_array__ = new class io*[size__];

      for (std::size_t i = 0; i < file_descriptors_manager::size (); ++i)
        {
          descriptors_array__[i] = nullptr;
        }
    }

    file_descriptors_manager::~file_descriptors_manager ()
    {
      trace::printf ("file_descriptors_manager::%s(%) @%p\n", __func__, this);

      delete[] descriptors_array__;
      size__ = 0;
    }

    // ------------------------------------------------------------------------

    io*
    file_descriptors_manager::io (int fildes)
    {
      // Check if valid descriptor or buffer not yet initialised
      if ((fildes < 0) || (static_cast<std::size_t> (fildes) >= size__)
          || (descriptors_array__ == nullptr))
        {
          return nullptr;
        }
      return descriptors_array__[fildes];
    }

    bool
    file_descriptors_manager::valid (int fildes)
    {
      if ((fildes < 0) || (static_cast<std::size_t> (fildes) >= size__))
        {
          return false;
        }
      return true;
    }

    int
    file_descriptors_manager::allocate (class io* io)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_DESCRIPTORS_MANAGER)
      trace::printf ("file_descriptors_manager::%s(%p)\n", __func__, io);
#endif

      if (io->file_descriptor () >= 0)
        {
          // Already allocated
          errno = EBUSY;
          return -1;
        }

      for (std::size_t i = reserved__; i < size__; ++i)
        {
          if (descriptors_array__[i] == nullptr)
            {
              descriptors_array__[i] = io;
              io->file_descriptor (static_cast<int> (i));
#if defined(OS_TRACE_POSIX_IO_FILE_DESCRIPTORS_MANAGER)
              trace::printf ("file_descriptors_manager::%s(%p) fd=%d\n",
                             __func__, io, i);
#endif
              return static_cast<int> (i);
            }
        }

      // Too many files open in system.
      errno = ENFILE;
      return -1;
    }

    int
    file_descriptors_manager::assign (file_descriptor_t fildes, class io* io)
    {
      if ((fildes < 0) || (static_cast<std::size_t> (fildes) >= size__))
        {
          errno = EBADF;
          return -1;
        }

      if (io->file_descriptor () >= 0)
        {
          // Already allocated
          errno = EBUSY;
          return -1;
        }

      descriptors_array__[fildes] = io;
      io->file_descriptor (fildes);
      return fildes;
    }

    int
    file_descriptors_manager::deallocate (int fildes)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_DESCRIPTORS_MANAGER)
      trace::printf ("file_descriptors_manager::%s(%d)\n", __func__, fildes);
#endif

      if ((fildes < 0) || (static_cast<std::size_t> (fildes) >= size__))
        {
          errno = EBADF;
          return -1;
        }

      descriptors_array__[fildes]->clear_file_descriptor ();
      descriptors_array__[fildes] = nullptr;
      return 0;
    }

    class socket*
    file_descriptors_manager::socket (int fildes)
    {
      assert((fildes >= 0) && (static_cast<std::size_t> (fildes) < size__));
      auto* const io = descriptors_array__[fildes];
      if (io->get_type () != io::type::socket)
        {
          return nullptr;
        }
      return reinterpret_cast<class socket*> (io);
    }

    size_t
    file_descriptors_manager::used (void)
    {
      std::size_t count = reserved__;
      for (std::size_t i = reserved__; i < file_descriptors_manager::size ();
          ++i)
        {
          if (descriptors_array__[i] != nullptr)
            {
              ++count;
            }
        }
      return count;
    }

  // ========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

