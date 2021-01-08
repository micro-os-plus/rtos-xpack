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

#ifndef CMSIS_PLUS_POSIX_IO_C_POSIX_SYSCALLS_ALIASES_STANDARD_H_
#define CMSIS_PLUS_POSIX_IO_C_POSIX_SYSCALLS_ALIASES_STANDARD_H_

#ifdef __cplusplus
extern "C"
{
#endif

  // --------------------------------------------------------------------------

  /**
   * @ingroup cmsis-plus-posix-io-c
   * @{
   */

  // For embedded environment that use POSIX system calls, redefine
  // all functions without the '__posix_' prefix.
  int __attribute__((weak, alias ("__posix_accept")))
  accept (int socket, struct sockaddr* address, socklen_t* address_len);

  int __attribute__((weak, alias ("__posix_bind")))
  bind (int socket, const struct sockaddr* address, socklen_t address_len);

  int __attribute__((weak, alias ("__posix_chdir")))
  chdir (const char* path);

  int __attribute__((weak, alias ("__posix_chmod")))
  chmod (const char* path, mode_t mode);

  int __attribute__((weak, alias ("__posix_chown")))
  chown (const char* path, uid_t owner, gid_t group);

  clock_t __attribute__((weak, alias ("__posix_clock")))
  clock (void);

  int __attribute__((weak, alias ("__posix_close")))
  close (int fildes);

  int __attribute__((weak, alias ("__posix_closedir")))
  closedir (DIR* dirp);

  int __attribute__((weak, alias ("__posix_connect")))
  connect (int socket, const struct sockaddr* address, socklen_t address_len);

  int __attribute__((weak, alias ("__posix_execve")))
  execve (const char* path, char* const argv[], char* const envp[]);

  int __attribute__((weak, alias ("__posix_fcntl")))
  fcntl (int fildes, int cmd, ...);

  pid_t __attribute__((weak, alias ("__posix_fork")))
  fork (void);

  int __attribute__((weak, alias ("__posix_fstat")))
  fstat (int fildes, struct stat* buf);

  int __attribute__((weak, alias ("__posix_fstatvfs")))
  fstatvfs (int fildes, struct statvfs* buf);

  int __attribute__((weak, alias ("__posix_ftruncate")))
  ftruncate (int fildes, off_t length);

  int __attribute__((weak, alias ("__posix_fsync")))
  fsync (int fildes);

  char*
  __attribute__((weak, alias ("__posix_getcwd")))
  getcwd (char* buf, size_t size);

  int __attribute__((weak, alias ("__posix_getpeername")))
  getpeername (int socket, struct sockaddr* address, socklen_t* address_len);

  pid_t __attribute__((weak, alias ("__posix_getpid")))
  getpid (void);

  int __attribute__((weak, alias ("__posix_getsockname")))
  getsockname (int socket, struct sockaddr* address, socklen_t* address_len);

  int __attribute__((weak, alias ("__posix_getsockopt")))
  getsockopt (int socket, int level, int option_name, void* option_value,
              socklen_t* option_len);

  int __attribute__((weak, alias ("__posix_gettimeofday")))
  gettimeofday (struct timeval* ptimeval, void* ptimezone);

  int __attribute__((weak, alias ("__posix_ioctl")))
  ioctl (int fildes, int request, ...);

  int __attribute__((weak, alias ("__posix_isatty")))
  isatty (int fildes);

  int __attribute__((weak, alias ("__posix_kill")))
  kill (pid_t pid, int sig);

  int __attribute__((weak, alias ("__posix_link")))
  link (const char* existing, const char* _new);

  int __attribute__((weak, alias ("__posix_listen")))
  listen (int socket, int backlog);

  off_t __attribute__((weak, alias ("__posix_lseek")))
  lseek (int fildes, off_t offset, int whence);

  int __attribute__((weak, alias ("__posix_mkdir")))
  mkdir (const char* path, mode_t mode);

  int __attribute__((weak, alias ("__posix_open")))
  open (const char* path, int oflag, ...);

  /**
   * @brief Open directory.
   * @param dirname [in] Directory name.
   * @return Pointer to DIR object.
   */
  DIR*
  __attribute__((weak, alias ("__posix_opendir")))
  opendir (const char* dirname);

  int __attribute__((weak, alias ("__posix_raise")))
  raise (int sig);

  ssize_t __attribute__((weak, alias ("__posix_read")))
  read (int fildes, void* buf, size_t nbyte);

  struct dirent*
  __attribute__((weak, alias ("__posix_readdir")))
  readdir (DIR* dirp);

  int __attribute__((weak, alias ("__posix_readdir_r")))
  readdir_r (DIR* dirp, struct dirent* entry, struct dirent** result);

  ssize_t __attribute__((weak, alias ("__posix_readlink")))
  readlink (const char* path, char* buf, size_t bufsize);

  ssize_t __attribute__((weak, alias ("__posix_recv")))
  recv (int socket, void* buffer, size_t length, int flags);

  ssize_t __attribute__((weak, alias ("__posix_recvfrom")))
  recvfrom (int socket, void* buffer, size_t length, int flags,
            struct sockaddr* address, socklen_t* address_len);

  ssize_t __attribute__((weak, alias ("__posix_recvmsg")))
  recvmsg (int socket, struct msghdr* message, int flags);

  int __attribute__((weak, alias ("__posix_rename")))
  rename (const char* oldfn, const char* newfn);

  void __attribute__((weak, alias ("__posix_rewinddir")))
  rewinddir (DIR* dirp);

  int __attribute__((weak, alias ("__posix_rmdir")))
  rmdir (const char* path);

  int __attribute__((weak, alias ("__posix_select")))
  select (int nfds, fd_set* readfds, fd_set* writefds, fd_set* errorfds,
          struct timeval* timeout);

  ssize_t __attribute__((weak, alias ("__posix_send")))
  send (int socket, const void* buffer, size_t length, int flags);

  ssize_t __attribute__((weak, alias ("__posix_sendmsg")))
  sendmsg (int socket, const struct msghdr* message, int flags);

  ssize_t __attribute__((weak, alias ("__posix_sendto")))
  sendto (int socket, const void* message, size_t length, int flags,
          const struct sockaddr* dest_addr, socklen_t dest_len);

  int __attribute__((weak, alias ("__posix_setsockopt")))
  setsockopt (int socket, int level, int option_name, const void* option_value,
              socklen_t option_len);

  int __attribute__((weak, alias ("__posix_shutdown")))
  shutdown (int socket, int how);

  int __attribute__((weak, alias ("__posix_sockatmark")))
  sockatmark (int socket);

  int __attribute__((weak, alias ("__posix_socket")))
  socket (int domain, int type, int protocol);

#if 0
  int __attribute__((weak, alias ("__posix_socketpair")))
  socketpair (int domain, int type, int protocol, int socket_vector[2]);
#endif

  int __attribute__((weak, alias ("__posix_stat")))
  stat (const char* path, struct stat* buf);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"

  int __attribute__((weak, alias ("__posix_statvfs")))
  statvfs (const char* path, struct statvfs* buf);

#pragma GCC diagnostic pop

  void __attribute__((weak, alias ("__posix_sync")))
  sync (void);

  int __attribute__((weak, alias ("__posix_symlink")))
  symlink (const char* existing, const char* _new);

  int __attribute__((weak, alias ("__posix_system")))
  system (const char *command);

  int __attribute__((weak, alias ("__posix_symlink")))
  tcdrain (int fildes);

  int __attribute__((weak, alias ("__posix_tcflush")))
  tcflush (int fildes, int queue_selector);

  int __attribute__((weak, alias ("__posix_tcgetattr")))
  tcgetattr (int fildes, struct termios *termios_p);

  int __attribute__((weak, alias ("__posix_tcsendbreak")))
  tcsendbreak (int fildes, int duration);

  int __attribute__((weak, alias ("__posix_tcsetattr")))
  tcsetattr (int fildes, int optional_actions, const struct termios *termios_p);

  clock_t __attribute__((weak, alias ("__posix_times")))
  times (struct tms* buf);

  int __attribute__((weak, alias ("__posix_truncate")))
  truncate (const char* path, off_t length);

  int __attribute__((weak, alias ("__posix_unlink")))
  unlink (const char* name);

  int __attribute__((weak, alias ("__posix_utime")))
  utime (const char* path, const struct utimbuf* times);

  pid_t __attribute__((weak, alias ("__posix_wait")))
  wait (int* stat_loc);

  ssize_t __attribute__((weak, alias ("__posix_write")))
  write (int fildes, const void* buf, size_t nbyte);

  ssize_t __attribute__((weak, alias ("__posix_writev")))
  writev (int fildes, const struct iovec* iov, int iovcnt);

/**
 * @}
 */

// --------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif /* CMSIS_PLUS_POSIX_IO_C_POSIX_SYSCALLS_ALIASES_STANDARD_H_ */

// ----------------------------------------------------------------------------

