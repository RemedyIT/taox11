/**
 * @file    std_condition_variable.h
 * @author  Martin Corino
 *
 * @brief   Adds 'std::condition_variable' for MingW platforms.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_STD_CONDITION_VAR_T_H_INCLUDED
#define TAOX11_STD_CONDITION_VAR_T_H_INCLUDED

#ifndef TAOX11_STDEXT_H_INCLUDED
#pragma message ("warning: This file should not be included by user code directly. Please remove the include from your code")
#endif

#if defined(__MINGW32__) && !defined(_GLIBCXX_HAS_GTHREADS)

#include <ace/Synch_Traits.h>
#include <ace/Synch.h>
#include <ace/Guard_T.h>
#include <ace/Time_Value_T.h>
#if defined(ACE_WIN32) || \
    (defined (_POSIX_MONOTONIC_CLOCK) && !defined (ACE_LACKS_MONOTONIC_TIME)) || \
    defined (ACE_HAS_CLOCK_GETTIME_MONOTONIC)
# include "ace/Monotonic_Time_Policy.h"
# define STD_CV_TIME_POLICY  ACE_Monotonic_Time_Policy
#else
# include "ace/Time_Policy.h"
# define STD_CV_TIME_POLICY  ACE_System_Time_Policy
#endif
#include <errno.h>
#include <system_error>
#include <chrono>
#include <mutex>

namespace std
{
  /// cv_status
  enum class cv_status { no_timeout, timeout };

  class condition_variable
  {
  public:
    typedef ACE_SYNCH::CONDITION*   native_handle_type;

    condition_variable() noexcept;
    ~condition_variable() noexcept;

    void
    notify_one() noexcept
    { this->cv_.signal (); }

    void
    notify_all() noexcept
    { this->cv_.broadcast (); }

    void
    wait(unique_lock<mutex>& __lock) noexcept
    {
      this->cv_.wait (
          *const_cast<mutex*> (__lock.mutex ())->native_handle ());
    }

    template<typename _Predicate>
    void
    wait(unique_lock<mutex>& __lock, _Predicate __p)
    {
      while (!__p())
        wait(__lock);
    }

    template<typename _Clock, typename _Duration>
    cv_status
    wait_until(
        unique_lock<mutex>& __lock,
        const chrono::time_point<_Clock, _Duration>& __atime)
    {
      const typename _Clock::time_point __c_entry = _Clock::now();
      const auto __rtime =
          chrono::duration_cast<chrono::microseconds> (
              __atime - __c_entry);
      return __wait_until_impl(__lock, __rtime);
    }

    template<typename _Clock, typename _Duration, typename _Predicate>
    bool
    wait_until(
        unique_lock<mutex>& __lock,
        const chrono::time_point<_Clock, _Duration>& __atime,
        _Predicate __p)
    {
      while (!__p())
        if (wait_until(__lock, __atime) == cv_status::timeout)
          return __p();
      return true;
    }

    template<typename _Rep, typename _Period>
    cv_status
    wait_for(
        unique_lock<mutex>& __lock,
        const chrono::duration<_Rep, _Period>& __rtime)
    { return __wait_until_impl(__lock, __rtime); }

    template<typename _Rep, typename _Period, typename _Predicate>
    bool
    wait_for(
        unique_lock<mutex>& __lock,
        const chrono::duration<_Rep, _Period>& __rtime,
        _Predicate __p)
    {
      while (!__p())
        if (__wait_until_impl(__lock, __rtime) == cv_status::timeout)
          return __p();
      return true;
    }

    native_handle_type
    native_handle()
    { return &this->cv_; }

  private:
    template<typename _Rep, typename _Period>
    cv_status
    __wait_until_impl(
        unique_lock<mutex>& __lock,
        const chrono::duration<_Rep, _Period>& __rtime)
    {
      ACE_Time_Value_T<STD_CV_TIME_POLICY> __atv {
        ACE_Time_Value_T<STD_CV_TIME_POLICY> ({ __rtime }).to_absolute_time ()
      };

      if (this->cv_.wait (
            *const_cast<mutex*> (__lock.mutex ())->native_handle (),
            &__atv) < 0)
      {
        if (errno == ETIME)
          return cv_status::timeout;
        throw std::system_error (errno, std::generic_category());
      }
      else
      {
        return cv_status::no_timeout;
      }
    }

    condition_variable(const condition_variable&) = delete;
    condition_variable& operator=(const condition_variable&) = delete;

    ACE_SYNCH::MUTEX  mutex_;
    ACE_SYNCH::CONDITION cv_ { mutex_ };
  };
}; // std

#endif /* __MINGW32__ && !_GLIBCXX_HAS_GTHREADS */

#endif /* TAOX11_STD_CONDITION_VAR_T_H_INCLUDED */
