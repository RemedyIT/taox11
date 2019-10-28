/**
 * @file    logstream_t.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 logger stream buffer
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_LOGGER_LOGSTREAM_T_CPP
#define TAOX11_LOGGER_LOGSTREAM_T_CPP

#include "tao/x11/logger/logstream_t.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Log_Msg.h"
#include "ace/Log_Record.h"

// taox11_ios_init
//
// There is reported to be a bug in the Dinkumware
// implementation of iostreams where calling
// basic_ios::init() multiple times for the same
// basic_ios instance creates a memory leak as a result
// of the ios' locale being allocated more than
// once, each time overwriting the old pointer.
//
// This usually happens in multiple inheritance
// schemes involving a derived IOS typename and a
// derived IO stream typename (i, o or io).
//
// The fix for Dinkumware is to not call init() in the
// derived IOS typename but call init() only for each
// stream object - in the istream, ostream or
// iostream constructor.
//
// Other stream implementations, however, require that
// init() is called in the derived IOS constructor.
// So we replace each call to init() with the
// ace_ios_init macro defined below.


#if !defined(TAOX11_IOS_INIT_FIX)
  // Microsoft Visual Studio with Dinkumware STL (but not STLport)
# if defined(_MSC_VER) && (!defined(_STLP_MSVC) || defined(_STLP_NO_OWN_IOSTREAMS))
#   define TAOX11_IOS_INIT_FIX 1
    // QNX with Dinkumware but not GNU C++ Library
# elif defined(__QNX__) && !defined(__GLIBCPP__)
#   define TAOX11_IOS_INIT_FIX 1
# endif
#endif

#if defined(TAOX11_IOS_INIT_FIX)
# define taox11_ios_init(buf)
#else
# define taox11_ios_init(buf) this->init(buf)
#endif


namespace x11_logger
{

  template <typename CH, typename TR>
  log_buffer_t<CH, TR>::log_buffer_t (
      typename log_buffer_t<CH, TR>::logprio_type prio,
      typename std::basic_ios<CH, TR>::openmode mode)
    : priority_ (prio),
      mode_ (mode)
    {
      this->reset_buffers ();
    }

  template <typename CH, typename TR>
  typename log_buffer_t<CH, TR>::int_type
  log_buffer_t<CH, TR>::overflow (int_type c)
    {
      if (!(this->mode_ & ios_type::out)) return char_traits::eof ();

      if (c != char_traits::eof ())
      {
        *this->pptr () = char_traits::to_char_type (c);
        this->pbump (1);
      }
      if (this->flush_buffer () == std::streamsize (-1)) return char_traits::eof ();

      return c;
    }

  template <typename CH, typename TR>
  int
  log_buffer_t<CH, TR>::sync ()
    {
      if (this->pptr () && this->pptr () > this->pbase ())
      {
        if (this->flush_buffer () == -1) return -1;
      }

      this->log_record ();

      return 0;
    }

  template <typename CH, typename TR>
  void
  log_buffer_t<CH, TR>::set_mode (
      typename std::basic_ios<CH, TR>::openmode mode)
    {
      this->mode_ = mode;
    }

  template <typename CH, typename TR>
  typename std::basic_ios<CH, TR>::openmode
  log_buffer_t<CH, TR>::get_mode () const
    {
      return this->mode_;
    }

  template <typename CH, typename TR>
  void
  log_buffer_t<CH, TR>::reset_buffers()
    {
      this->setg (this->_buffer () + 4,
                  this->_buffer () + 4,
                  this->_buffer () + 4);
      this->setp (this->_buffer (),
                  this->_buffer ()+ (BUFFER_SIZE - 1));
    }

  template <typename CH, typename TR>
  int
  log_buffer_t<CH, TR>::flush_buffer ()
    {
      int const n = int (this->pptr () - this->pbase ());

      std::streamoff n_out = n;
      // make sure never to log more than MAXLOGMSG bytes in 1 log record
      std::streamoff const len = this->logmsg_.tellp ();
      if ((len + n_out) > MAXLOGMSG)
      {
          // determine max room left and put that part in buffer
          n_out = MAXLOGMSG - len;
          this->logmsg_.write (this->pbase (), n_out);

          this->log_record (); // flush full buffer to logging backend

          n_out = n - n_out; // determine part of logging data that's left
      }

      this->logmsg_.write (this->pbase (), n_out);

      this->pbump (-n);
      return n;
    }

  template <typename CH, typename TR>
  void
  log_buffer_t<CH, TR>::log_record ()
  {
    // Specialized for char and wchar_t (see log.h/cpp)
  }

  template <typename CH, typename TR>
  log_ios_t<CH, TR>::log_ios_t (logprio_type prio, openmode mode)
    : streambuf_ (prio, mode)
    {
      taox11_ios_init (std::addressof(this->streambuf_));
    }

  template <typename CH, typename TR>
  log_ios_t<CH, TR>::~log_ios_t ()
    {
    }

  template <typename CH, typename TR>
  typename log_ios_t<CH, TR>::buffer_type*
  log_ios_t<CH, TR>::rdbuf ()
    {
      return &this->streambuf_;
    }

  template <typename CH, typename TR>
  const typename log_ios_t<CH, TR>::buffer_type&
  log_ios_t<CH, TR>::stream () const
    {
      return this->streambuf_;
    }

  template <typename CH, typename TR>
  log_ostream_t<CH, TR>::log_ostream_t(logprio_type prio)
    : log_ios_t<CH, TR> (prio, std::ios::out),
      std::basic_ostream<CH, TR> (log_ios_t<CH, TR>::rdbuf ())
    {
    }

  template <typename CH, typename TR>
  log_ostream_t<CH, TR>::~log_ostream_t()
    {
    }

} /* x11_logger */

#endif /* TAOX11_LOGGER_LOGSTREAM_T_CPP */
