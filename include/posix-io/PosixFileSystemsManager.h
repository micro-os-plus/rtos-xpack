/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * µOS++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * µOS++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef POSIX_FILE_SYSTEMS_MANAGER_H_
#define POSIX_FILE_SYSTEMS_MANAGER_H_

// ----------------------------------------------------------------------------

namespace os
{
  class PosixFileSystem;

  class PosixFileSystemsManager
  {
  public:

    PosixFileSystemsManager ();

    ~PosixFileSystemsManager ();

    // ------------------------------------------------------------------------

    static PosixFileSystem*
    getFileSystem (const char** path);

    static PosixFileSystem*
    getFileSystem (const char** path1, const char** path2);

    static int
    setRoot (PosixFileSystem* fs);

    static int
    mount (PosixFileSystem* fs, const char* path);

    static int
    umount (const char* path);

  };

} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* POSIX_FILE_SYSTEMS_MANAGER_H_ */
