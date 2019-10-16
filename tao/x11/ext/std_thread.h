/**
 * @file    std_thread.h
 * @author  Martin Corino
 *
 * @brief   Adds 'std::thread' and 'std::this_thread' for MingW platforms.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_STD_THREAD_H_INCLUDED
#define TAOX11_STD_THREAD_H_INCLUDED

#ifndef TAOX11_STDEXT_H_INCLUDED
#pragma message ("warning: This file should not be included by user code directly. Please remove the include from your code")
#endif

#if defined(__MINGW32__) && !defined(_GLIBCXX_HAS_GTHREADS)

#include "ace/OS_NS_Thread.h"
#include "ace/Time_Value.h"
#include <functional>
#include <memory>
#include <chrono>
#include <system_error>
#include <cerrno>

#define _STD_THREAD_INVALID_HANDLE 0

namespace std
{
  class thread
  {
  public:
    class id
    {
    public:
      id () noexcept {}

      explicit
      id (ACE_thread_t an_id)
        : id_(an_id) {}

    private:
      friend class thread;
      friend class hash<thread::id>;
      void clear () { this->id_ = 0;}

      friend bool
      operator==(thread::id __x, thread::id __y) noexcept
      {
        return __x.id_ == __y.id_;
      }

      friend bool
      operator<(thread::id __x, thread::id __y) noexcept
      {
        return __x.id_ < __y.id_;
      }

      template<class _CharT, class _Traits>
      friend basic_ostream<_CharT, _Traits>&
      operator<<(basic_ostream<_CharT, _Traits>& __out, thread::id __id);

      ACE_thread_t id_ {};
    };

  protected:
    ACE_hthread_t handle_ {_STD_THREAD_INVALID_HANDLE};
    id threadId_;

  public:
    typedef ACE_hthread_t native_handle_type;

    id get_id () const noexcept
    { return this->threadId_; }

    native_handle_type native_handle () const
    { return this->handle_; }

    thread () noexcept = default;

    thread (thread&& other) noexcept
      : handle_ (other.handle_)
      , threadId_ (other.threadId_)
    {
        other.handle_ = _STD_THREAD_INVALID_HANDLE;
        other.threadId_.clear ();
    }

    thread (thread&) = delete;
    thread (const thread&) = delete;
    thread (const thread&&) = delete;

    template<class Function, class... Args>
    explicit thread (Function&& f, Args&&... args)
    {
      typedef decltype(std::bind(f, args...)) Call;
      std::unique_ptr<Call> call =
          std::make_unique<Call> (std::bind(f, args...));
      if (ACE_OS::thr_create (
            threadfunc<Call>,
            (void*)call.get (),
            THR_NEW_LWP | THR_JOINABLE,
            &this->threadId_.id_,
            &this->handle_) != 0)
      {
        throw std::system_error(errno, std::generic_category());
      }
      call.release ();
    }

    template <class Call>
    static unsigned int __stdcall threadfunc(void* arg)
    {
      std::unique_ptr<Call> upCall (static_cast<Call*>(arg));
      (*upCall) ();
      return (unsigned long)0;
    }

    bool joinable () const
    { return this->handle_ != _STD_THREAD_INVALID_HANDLE; }

    void join ()
    {
      if (get_id () == id (ACE_OS::thr_self ()))
        throw std::system_error (EDEADLK, std::generic_category());
      if (this->handle_ == _STD_THREAD_INVALID_HANDLE)
        throw std::system_error (ESRCH, std::generic_category());
      if (!joinable ())
        throw std::system_error (EINVAL, std::generic_category());
      ACE_THR_FUNC_RETURN status;
      if (ACE_OS::thr_join (this->handle_, &status) != 0)
        throw std::system_error ();
      this->handle_ = _STD_THREAD_INVALID_HANDLE;
      this->threadId_.clear ();
    }

    ~thread()
    {
      if (joinable())
        std::terminate();
    }

    thread& operator= (const thread&) = delete;

    thread& operator= (thread&& other) noexcept
    {
      if (joinable())
        std::terminate();
      swap (std::forward<thread> (other));
      return *this;
    }

    void swap (thread&& other) noexcept
    {
      std::swap (this->handle_, other.handle_);
      std::swap (this->threadId_.id_, other.threadId_.id_);
    }

    static unsigned int hardware_concurrency () noexcept
    {
      static int ncpus = -1;
      if (ncpus == -1)
      {
        SYSTEM_INFO sysinfo;
        ::GetSystemInfo (std::addressof(sysinfo));
        ncpus = sysinfo.dwNumberOfProcessors;
      }
      return ncpus;
    }

    void detach ()
    {
      if (!joinable ())
        throw std::system_error (EINVAL, std::generic_category());
      if (this->handle_ != _STD_THREAD_INVALID_HANDLE)
      {
        ::CloseHandle (this->handle_);
        this->handle_ = _STD_THREAD_INVALID_HANDLE;
      }
      this->threadId_.clear ();
    }
  };

  inline void
  swap(thread& __x, thread& __y) noexcept
  { __x.swap(std::move (__y)); }

  inline bool
  operator!=(thread::id __x, thread::id __y) noexcept
  { return !(__x == __y); }

  inline bool
  operator<=(thread::id __x, thread::id __y) noexcept
  { return !(__y < __x); }

  inline bool
  operator>(thread::id __x, thread::id __y) noexcept
  { return __y < __x; }

  inline bool
  operator>=(thread::id __x, thread::id __y) noexcept
  { return !(__x < __y); }

  /// std::hash specialization for thread::id.
  template<>
  struct hash<thread::id>
    : public __hash_base<size_t, thread::id>
  {
    size_t
    operator()(const thread::id& __id) const noexcept
    { return std::_Hash_impl::hash(std::addressof(__id.id_), sizeof (__id.id_)); }
  };

  template<class _CharT, class _Traits>
  inline basic_ostream<_CharT, _Traits>&
  operator<<(basic_ostream<_CharT, _Traits>& __out, thread::id __id)
  {
    if (__id == thread::id())
      return __out << "thread::id of a non-executing thread";
    else
      return __out << __id.id_;
  }

  namespace this_thread
  {
    inline thread::id get_id ()
    { return thread::id (ACE_OS::thr_self ()); }

    inline void yield ()
    { ACE_OS::thr_yield (); }

    template< class Rep, class Period >
    void sleep_for( const std::chrono::duration<Rep,Period>& sleep_duration)
    {
      ACE_OS::sleep (ACE_Time_Value (sleep_duration));
    }
    template <class Clock, class Duration>
    void sleep_until(const std::chrono::time_point<Clock,Duration>& sleep_time)
    {
        sleep_for(sleep_time-Clock::now());
    }
  }

}; // std
#endif /* __MINGW32__ && !_GLIBCXX_HAS_GTHREADS */

#endif /* TAOX11_STD_THREAD_H_INCLUDED */
