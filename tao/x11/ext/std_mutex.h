/**
 * @file    std_mutex.h
 * @author  Martin Corino
 *
 * @brief   Adds 'std::mutex' and 'std::recursive_mutex' for MingW platforms.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_STD_MUTEX_T_H_INCLUDED
#define TAOX11_STD_MUTEX_T_H_INCLUDED

#ifndef TAOX11_STDEXT_H_INCLUDED
#pragma message ("warning: This file should not be included by user code directly. Please remove the include from your code")
#endif

#if defined(__MINGW32__) && !defined(_GLIBCXX_HAS_GTHREADS)

#include <ace/Synch_Traits.h>
#include <ace/Synch.h>
#include <ace/Guard_T.h>
#include <errno.h>
#include <system_error>

namespace std
{
  class mutex
  {
  public:
    typedef ACE_SYNCH::MUTEX*      native_handle_type;

    mutex () = default;
    ~mutex() = default;

    void
    lock()
    {
      if (this->mutex_.acquire () != 0)
        throw std::system_error ();
    }

    bool
    try_lock() noexcept
    {
      return (this->mutex_.tryacquire () == 0);
    }

    void
    unlock()
    {
      if (this->mutex_.release () != 0)
        throw std::system_error ();
    }

    native_handle_type
    native_handle()
    { return &this->mutex_; }

  private:
    ACE_SYNCH::MUTEX  mutex_;
    mutex(const mutex&) = delete;
    mutex& operator=(const mutex&) = delete;

  };

  class recursive_mutex
  {
  public:
    typedef ACE_SYNCH::RECURSIVE_MUTEX*      native_handle_type;

    recursive_mutex () = default;
    ~recursive_mutex() = default;

    void
    lock()
    {
      if (this->mutex_.acquire () != 0)
        throw std::system_error ();
    }

    bool
    try_lock() noexcept
    {
      return (this->mutex_.tryacquire () == 0);
    }

    void
    unlock()
    {
      if (this->mutex_.release () != 0)
        throw std::system_error ();
    }

    native_handle_type
    native_handle()
    { return &this->mutex_; }

  private:
    ACE_SYNCH::RECURSIVE_MUTEX  mutex_;
    recursive_mutex(const recursive_mutex&) = delete;
    recursive_mutex& operator=(const recursive_mutex&) = delete;
  };
}; // std

#endif /* __MINGW32__ && !_GLIBCXX_HAS_GTHREADS */

#endif /* TAOX11_STD_MUTEX_T_H_INCLUDED */
