/**
 * @file    log_base.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 logger stream implementations
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "ace/ace_wchar.h"
#include "ace/ACE.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Singleton.h"
#if defined(__MINGW32__)
# if defined (ACE_HAS_THREADS)
#  include "ace/Thread_Mutex.h"
# else
#  include "ace/Null_Mutex.h"
# endif
# include "ace/Synch_Traits.h"
#endif
#include <algorithm>
#include <array>
#include <vector>
#include <queue>
#include <iomanip>
#include <iostream>
#include <cctype>
#include <cstring>

#include "tao/x11/logger/x11_logger_export.h"

#ifndef X11_EXTERN_
# define X11_EXTERN_
#endif
#ifndef X11_EXPORT_
# define X11_EXPORT_ X11_Logger_Export
#endif

#include "tao/x11/logger/log_base.h"

namespace x11_logger
{
  // Translation tables

  static std::array<std::basic_string<ACE_TCHAR>, 11> priority_names_ =
  {
    {
       ACE_TEXT ("NOT USED"),
       ACE_TEXT ("LP_TRACE"),
       ACE_TEXT ("LP_DEBUG"),
       ACE_TEXT ("LP_INFO"),
       ACE_TEXT ("NOT USED"),
       ACE_TEXT ("LP_WARNING"),
       ACE_TEXT ("NOT USED"),
       ACE_TEXT ("LP_ERROR"),
       ACE_TEXT ("LP_CRITICAL"),
       ACE_TEXT ("NOT USED"),
       ACE_TEXT ("LP_PANIC")
     }
  };

  struct priority_mapping_t
  {
    std::string prio_name_;
    uint32_t prio_mask_;
  };
  static std::array<priority_mapping_t, 9> priority_map_ =
  {
    {
      {"TRACE", LP_TRACE},
      {"DEBUG", LP_DEBUG},
      {"INFO", LP_INFO},
      {"WARNING", LP_WARNING},
      {"ERROR", LP_ERROR},
      {"CRITICAL", LP_CRITICAL},
      {"PANIC", LP_PANIC},
      {"ALL_ERROR", LP_ALL_ERROR},
      {"ALL", LP_ALL}
    }
  };

  struct verbosity_mapping_t
  {
    std::string v_name_;
    uint32_t v_mask_;
  };
  static std::array<verbosity_mapping_t, 9> verbosity_map_ =
  {
    {
      {"THREAD_ID", V_THREAD_ID},
      {"THREAD", V_THREAD_ID},
      {"PRIO", V_PRIO},
      {"TIME", V_TIME},
      {"DATETIME", V_DATETIME},
      {"PROCESS_ID", V_PROCESS_ID},
      {"PROCESS", V_PROCESS_ID},
      {"CATEGORY", V_CATEGORY},
      {"ALL", V_ALL}
    }
  };

  // Special guard class using the ACE log framework lock
  class ACE_Log_Guard
  {
  public:
    ACE_Log_Guard (ACE_Log_Msg* ace_log)
      : logger_ (ace_log)
    {
      this->logger_->acquire ();
    }
    ~ACE_Log_Guard ()
    {
      this->logger_->release ();
    }
  private:
    ACE_Log_Msg* logger_;
  };

  inline std::basic_string<ACE_TCHAR> priority_name (uint32_t prop)
  {
    uint32_t log {};

    for (; prop > 1; ++log)
      prop >>= 1;

    return priority_names_[log];
  }

  // Logging helpers
  static std::basic_string<ACE_TCHAR>
  log_record_helper (Log_Module const * _lm, uint32_t priority, ACE_Log_Record& lr)
  {
    std::basic_stringstream<ACE_TCHAR> logbuf(ACE_TEXT (""));

    uint32_t vmask = _lm->verbosity_mask();

    if (ACE_BIT_ENABLED(vmask, X11_Verbose::V_CATEGORY))
    {
      logbuf << std::left << std::setw(11) << ACE_TEXT_CHAR_TO_TCHAR (_lm->category ()) << ACE_TEXT (" ");
    }

    if (ACE_BIT_ENABLED(vmask, X11_Verbose::V_PRIO))
    {
      const std::basic_string<ACE_TCHAR> name =
        ACE_TEXT ("[") + priority_name(priority) + ACE_TEXT ("]");
      // setw to strlen LM_CRITICAL + 2 brackets.
      logbuf << std::left << std::setw (13) << name << ACE_TEXT (" - ");
    }

    if (ACE_BIT_ENABLED(vmask, X11_Verbose::V_PROCESS_ID))
    {
      logbuf <<  ACE_TEXT ("(") << lr.pid () ;
      if (ACE_BIT_ENABLED(vmask, X11_Verbose::V_THREAD_ID))
      {
        logbuf << ACE_TEXT ("|") << std::this_thread::get_id ();
      }
      logbuf << ACE_TEXT (") - ");
    }
    else if (ACE_BIT_ENABLED(vmask, X11_Verbose::V_THREAD_ID))
    {
      logbuf << ACE_TEXT ("(") << std::this_thread::get_id () << ACE_TEXT (") - ");
    }

    if (ACE_BIT_ENABLED(vmask, X11_Verbose::V_TIME))
    {
      logbuf << x11_logger::now  << ACE_TEXT (" - ");
    }

    if (ACE_BIT_ENABLED(vmask, X11_Verbose::V_DATETIME))
    {
      logbuf << x11_logger::date  << ACE_TEXT (" - ");
    }
    return logbuf.str();
  }

  static void
  log_failure_message (Log_Module const * _lm, const ACE_TCHAR* message)
  {
    // get ACE logger from TSS
    ACE_Log_Msg* ace_log = ACE_Log_Msg::instance ();

    ACE_Log_Record lr (LM_ERROR, ACE_OS::gettimeofday (), ace_log->getpid ());
    std::basic_stringstream<ACE_TCHAR> logbuf;
    logbuf << log_record_helper (_lm, LM_ERROR, lr);
    logbuf << message;
    lr.msg_data (logbuf.str ().c_str ());

    ace_log->log (lr);
  }

  // String helpers

  inline char up_char (char ch)
  {
    return std::use_facet< std::ctype< char > >(std::locale() ).toupper(ch);
  }

  void trim(std::string& str)
  {
    std::string::size_type pos1 = str.find_first_not_of(' ');
    std::string::size_type pos2 = str.find_last_not_of(' ');
    str = str.substr(pos1 == std::string::npos ? 0 : pos1,
                     pos2 == std::string::npos ? str.length() - 1 : pos2 - pos1 + 1);
  }

  /*
   * Log_Module class
   */
  Log_Module* Log_Module::getInstance()
  {
    static Log_Module instance_;

    return &instance_;
  }

  Log_Module::Log_Module()
  : category_ ("X11"),
    priority_mask_ (LP_PANIC),
    verbosity_mask_ (0),
    output_mask_ (OS_STDERR),
    file_max_size_ (0),
    file_count_ (0),
    file_flags_ (0),
    file_stream_ (nullptr)
  {
    // set default
    this->verbosity_mask_ =
        x11_logger::V_CATEGORY |
        x11_logger::V_PRIO |
        x11_logger::V_PROCESS_ID |
        x11_logger::V_THREAD_ID |
        x11_logger::V_TIME ;

    // check env vars for priorities and verbose options, overwrite default if env var is set
    get_env_var_prio_mask (true);
    get_env_var_verbose_mask (true);
    get_env_var_output_mask ();
  }

  Log_Module::Log_Module(
      std::string category,
      uint32_t priority_mask,
      uint32_t verbosity_mask)
   : category_ (std::move (category)),
     priority_mask_ (priority_mask),
     verbosity_mask_ (verbosity_mask)
  {
    // check env vars for priorities and verbose options, overwrite default if env var is set
    get_env_var_prio_mask ();
    get_env_var_verbose_mask ();
    get_env_var_output_mask ();
  }

  Log_Module::~Log_Module()
  {
    if (this->file_stream_)
    {
      // avoid using ACE_OS::fopen as we did not use ACE_OS::fopen
      ::fclose (this->file_stream_);
    }
  }

  /// Get the current output mask.
  uint32_t Log_Module::output_mask () const
  {
    ACE_Log_Guard __guard (ACE_Log_Msg::instance ());

    if (this->output_mask_ || this == Log_Module::getInstance ())
      return this->output_mask_;
    else
      return Log_Module::getInstance ()->output_mask ();
  }

  /// Set the output mask, returns original mask.
  uint32_t Log_Module::output_mask (uint32_t mask)
  {
    ACE_Log_Guard __guard (ACE_Log_Msg::instance ());

    uint32_t const org_mask = this->output_mask_;
    this->output_mask_ = mask;
    return org_mask;
  }

  /// Set logfile path specifications
  void Log_Module::set_logfile_path (std::string base_path)
  {
    ACE_Log_Guard __guard (ACE_Log_Msg::instance ());

    this->set_logfile_path_i (base_path);

    // setting this also sets the file output flag
    this->output_mask_ |= OS_FILE;
  }

  void Log_Module::set_logfile_path_i (std::string base_path)
  {
    ACE_stat st;

    if (ACE_OS::stat (base_path.c_str (), &st) == 0 && S_ISDIR(st.st_mode))
    {
      // only log folder specified -> append category as base filename
      this->file_base_ = base_path;
      this->file_base_.append (1, ACE_DIRECTORY_SEPARATOR_CHAR_A);
      this->file_base_.append (this->category_);
    }
    else
    {
      // does the path end with a dir sep?
      if (base_path.back () == ACE_DIRECTORY_SEPARATOR_CHAR_A)
      {
        // assume valid folder name; append category as base filename
        this->file_base_ = base_path;
        this->file_base_.append (this->category_);
      }
      else if (base_path.find (ACE_DIRECTORY_SEPARATOR_CHAR_A) != std::string::npos)
      {
        // assume relative or absolute path with base filename
        this->file_base_ = base_path;
      }
      else
      {
        char pathbuf[PATH_MAX +1];
        // assume just file basename to be stored in current working dir
        this->file_base_ = ACE_OS::getcwd (pathbuf, PATH_MAX);
        this->file_base_.append (1, ACE_DIRECTORY_SEPARATOR_CHAR_A);
        this->file_base_.append (base_path);
      }
    }
    // clear any previous built full file name
    this->file_path_.clear ();
  }

  /// Set logfile creation specifications
  void Log_Module::set_logfile_specs (uint32_t maxsize,
                                      uint32_t count,
                                      uint32_t flags)
  {
    ACE_Log_Guard __guard (ACE_Log_Msg::instance ());

    this->set_logfile_specs_i (maxsize, count, flags);

    // setting this also sets the file output flag
    this->output_mask_ |= OS_FILE;
  }

  void Log_Module::set_logfile_specs_i (uint32_t maxsize,
                                        uint32_t count,
                                        uint32_t flags)
  {
    this->file_max_size_ = maxsize;
    this->file_count_ = count;
    this->file_flags_ = flags;
  }

  /// Get logfile base path
  bool Log_Module::get_logfile_path (std::string& path) const
  {
    ACE_Log_Guard __guard (ACE_Log_Msg::instance ());

    bool rc = true;
    // if we have our own outputmask defined OR we're the base module
    if (this->output_mask_ || this == Log_Module::getInstance ())
    {
      // return our setting
      if (ACE_BIT_ENABLED (this->output_mask_, OS_FILE))
        path = this->file_base_;
      else
        rc = false;
    }
    else
      // return base setting
      rc = Log_Module::getInstance ()->get_logfile_path (path);

    return rc;
  }

  /// Get logfile creation specifications
  bool Log_Module::get_logfile_specs (uint32_t &maxsize,
                                      uint32_t &count,
                                      uint32_t &flags)
  {
    ACE_Log_Guard __guard (ACE_Log_Msg::instance ());

    bool rc = false;
    // if we have our own outputmask defined OR we're the base module
    if (this->output_mask_ || this == Log_Module::getInstance ())
    {
      // return our setting
      if (ACE_BIT_ENABLED (this->output_mask_, OS_FILE))
      {
        maxsize = this->file_max_size_;
        count = this->file_count_;
        flags = this->file_flags_;
        rc = true;
      }
    }
    else
      // return base setting
      rc = Log_Module::getInstance ()->get_logfile_specs (maxsize, count, flags);

    return rc;
  }

  /// Get the current priority mask.
  uint32_t Log_Module::priority_mask () const
  {
    ACE_Log_Guard __guard (ACE_Log_Msg::instance ());

    return this->priority_mask_;
  }

  /// Get the current verbosity mask.
  uint32_t Log_Module::verbosity_mask () const
  {
    ACE_Log_Guard __guard (ACE_Log_Msg::instance ());

    return this->verbosity_mask_;
  }

  /// Get the category name
  const std::string& Log_Module::category () const
  {
    ACE_Log_Guard __guard (ACE_Log_Msg::instance ());

    return this->category_;
  }

  /// Set the priority mask, returns original mask.
  uint32_t Log_Module::priority_mask (uint32_t new_mask)
  {
    ACE_Log_Guard __guard (ACE_Log_Msg::instance ());

    uint32_t old_mask;
    old_mask = this->priority_mask_;
    this->priority_mask_ = new_mask;
    return old_mask;
  }

  /// Set the verbosity mask, returns original mask.
  uint32_t Log_Module::verbosity_mask (uint32_t new_mask)
  {
    ACE_Log_Guard __guard (ACE_Log_Msg::instance ());

    uint32_t old_mask;
    old_mask = this->verbosity_mask_;
    this->verbosity_mask_ = new_mask;
    return old_mask;
  }

  // Set the priority mask with value of environment variable category + "_LOG_MASK"
  // if category + "_LOG_MASK" is set.
  // Set the priority mask with value of global environment variable X11_LOG_MASK
  // if X11_LOG_MASK is set and category + "_LOG_MASK" isn't set.
  // If no environment variable set, priority_mask_ doesn't change.
  void Log_Module::get_env_var_prio_mask(bool global)
  {
    const char * _env {};
    uint32_t mask {};
    std::vector<std::string> _tokens;

    // check for category specific settings if this is not global module
    if (!global)
    {
      std::string category_log_mask = this->category_ + "_LOG_MASK";
      std::transform(category_log_mask.begin(), category_log_mask.end(), category_log_mask.begin(), up_char);
      _env = std::getenv (category_log_mask.c_str ());
      if (_env)
      {
        std::istringstream  _env_strm;
        // handle quoted values
        if (*_env == '"' || *_env == '\'')
        {
          // skip leading quote
          ++_env;
          // if not empty
          if (*_env)
            // copy all the rest but (assumed) trailing quote
            _env_strm.str (std::string (_env, std::strlen (_env)-1));
          // else leave stream empty
        }
        else
        {
          _env_strm.str (_env);
        }
        for (std::string _env_value;
             std::getline(_env_strm, _env_value, '|');
             _tokens.push_back(_env_value));
      }
    }

    // if no tokens, no category specific environment variable is set or
    // is set but has no value.
    if (_tokens.empty ())
    {
      _env = std::getenv ("X11_LOG_MASK");
      if (_env)
      {
        std::istringstream  _env_strm;
        // handle quoted values
        if (*_env == '"' || *_env == '\'')
        {
          // skip leading quote
          ++_env;
          // if not empty
          if (*_env)
          {
            // copy all the rest but (assumed) trailing quote
            _env_strm.str (std::string (_env, std::strlen (_env)-1));
          }
          // else leave stream empty
        }
        else
        {
          _env_strm.str (_env);
        }
        for (std::string _env_value;
             std::getline(_env_strm, _env_value, '|');
             _tokens.push_back(_env_value));
      }
    }

    if (!_tokens.empty ())
    {
      for (std::string& _token : _tokens)
      {
        if (!_token.empty())
        {
          std::transform(_token.begin(), _token.end(), _token.begin(), up_char);
          trim (_token);

          for (priority_mapping_t& _pm : priority_map_)
          {
            if (_token == _pm.prio_name_)
            {
              mask |= _pm.prio_mask_;
              break;
            }
          }
        }
      }

      this->priority_mask_ = mask;
    }
  }

  // Set the verbosity mask with value of environment variable category + "_VERBOSE"
  // if category + "_VERBOSE" is set.
  // Set the priority mask with value of global environment variable X11_VERBOSE
  // if X11_VERBOSE is set and category + "_VERBOSE" isn't set.
  // If no environment variable set, verbosity_mask_ doesn't change.
  void Log_Module::get_env_var_verbose_mask(bool global)
  {
    const char * _env {};
    uint32_t mask {};
    std::vector<std::string> _tokens;

    // check for category specific settings if this is not global module
    if (!global)
    {
      std::string verbose_mask = this->category_ + "_VERBOSE";
      std::transform(verbose_mask.begin(), verbose_mask.end(), verbose_mask.begin(), up_char);
      _env = std::getenv (verbose_mask.c_str ());
      if (_env)
      {
        std::istringstream  _env_strm;
        // handle quoted values
        if (*_env == '"' || *_env == '\'')
        {
          // skip leading quote
          ++_env;
          // if not empty
          if (*_env)
            // copy all the rest but (assumed) trailing quote
            _env_strm.str (std::string (_env, std::strlen (_env)-1));
          // else leave stream empty
        }
        else
        {
          _env_strm.str (_env);
        }
        for (std::string _env_value;
             std::getline(_env_strm, _env_value, '|');
             _tokens.push_back(_env_value));
      }
    }

    // if no tokens, no category specific environment variable is set or
    // is set but has no value.
    if (_tokens.empty ())
    {
      _env = std::getenv ("X11_VERBOSE");
      if (_env)
      {
        std::istringstream _env_strm;
        // handle quoted values
        if (*_env == '"' || *_env == '\'')
        {
          // skip leading quote
          ++_env;
          // if not empty
          if (*_env)
          {
            // copy all the rest but (assumed) trailing quote
            _env_strm.str (std::string (_env, std::strlen (_env)-1));
          }
          // else leave stream empty
        }
        else
        {
          _env_strm.str (_env);
        }
        for (std::string _env_value;
             std::getline(_env_strm, _env_value, '|');
             _tokens.push_back(_env_value));
      }
    }

    if (!_tokens.empty ())
    {
      for (std::string& _token : _tokens)
      {
        std::transform(_token.begin(), _token.end(), _token.begin(), up_char);
        trim (_token);

        for (verbosity_mapping_t& _vm : verbosity_map_)
        {
          if (_token == _vm.v_name_)
          {
            mask |= _vm.v_mask_;
            break;
          }
        }
      }

      this->verbosity_mask_ = mask;
    }
  }

  // Set the output mask with value of environment variable <category>+"_LOG_OUTPUT"
  // if <category>+"_LOG_OUTPUT is set.
  // If no environment variable set, output_mask_ doesn't change.
  // environment variable value :== a sequence of 'STDERR' | 'SYSLOG' | 'IPC' | 'FILE[=<args>]' | 'CUSTOM'
  //      separated by '|'
  // 'FILE' allows optional args as FILE=<base>,<size>,<count>,<flags>
  // base  := either a file basename (no extension)
  //          or a path to an existing folder
  //          or the combination of both
  // size  := the maximum size of a single logfile
  // count := the number of rolling logfiles
  // flags := 'P' (files per process) and or
  //          'T' (truncate existing files)
  // Arguments can be missing or empty.
  // Outputmask environment variable does *not* fall back on global
  // variable as the log module will reuse the base log modules values
  // at run time.
  void Log_Module::get_env_var_output_mask ()
  {
    const char * _env {};
    uint32_t omask {};
    std::vector<std::string> _tokens;

    // check for category specific settings
    std::string s_omask = this->category_ + "_LOG_OUTPUT";
    std::transform(s_omask.begin(), s_omask.end(), s_omask.begin(), up_char);
    _env = std::getenv (s_omask.c_str ());
    if (_env)
    {
      std::istringstream  _env_strm(_env);
      // handle quoted values
      if (*_env == '"' || *_env == '\'')
      {
        // skip leading quote
        ++_env;
        // if not empty
        if (*_env)
        {
          // copy all the rest but (assumed) trailing quote
          _env_strm.str (std::string (_env, std::strlen (_env)-1));
        }
        // else leave stream empty
      }
      else
      {
        _env_strm.str (_env);
      }
      for (std::string _env_value;
           std::getline(_env_strm, _env_value, '|');
           _tokens.push_back(_env_value));
    }

    if (!_tokens.empty ())
    {
      for (std::string& _token : _tokens)
      {
        trim (_token);

        std::string::iterator _end = _token.end ();
        std::string::size_type pos;
        if ((pos = _token.find ('=')) != std::string::npos)
        {
          _end = _token.begin () + pos;
        }
        std::transform(_token.begin(), _end, _token.begin(), up_char);

        if (_token == "STDERR")
          omask |= OS_STDERR;
        else if (_token == "SYSLOG")
          omask |= OS_SYSLOG;
        else if (_token == "IPC")
          omask |= OS_IPC;
        else if (_token == "CUSTOM")
          omask |= OS_CUSTOM;
        else if (_token.compare (0, 4, "FILE") == 0)
        {
          omask |= OS_FILE;
          // check for arguments
          if ((pos = _token.find ('=')) != std::string::npos)
          {
            // split arguments
            std::queue<std::string> _args;
            std::istringstream  _arg_strm(_token.substr (pos+1));
            for (std::string _arg;
                 std::getline(_arg_strm, _arg, ',');
                 _args.push(_arg));
            // check for file base path
            if (!_args.empty ())
            {
              if (!_args.front ().empty ())
                this->set_logfile_path_i (_args.front ());
              _args.pop ();
            }
            // check for max size, rolling count and/or flags
            if (!_args.empty ())
            {
              uint32_t sz {};
              uint32_t cnt {};
              uint32_t flags {};
              if (!_args.front ().empty ())
                sz = ACE_OS::strtoul (_args.front ().c_str (), 0, 10);
              _args.pop ();
              if (!_args.empty ())
              {
                if (!_args.front ().empty ())
                  cnt = ACE_OS::strtoul (_args.front ().c_str (), 0, 10);
                _args.pop ();
                if (!_args.empty ())
                {
                  if (!_args.front ().empty ())
                  {
                    if (_args.front ().find_first_of ("pP") != std::string::npos)
                      flags |= FF_PROCESS;
                    if (_args.front ().find_first_of ("tT") != std::string::npos)
                      flags |= FF_TRUNCATE;
                  }
                }
              }
              this->set_logfile_specs_i (sz, cnt, flags);
            }
          }
        }
      }

      this->output_mask_ = omask;
    }
  }

  // Get current logfile stream
  // (handles rolling versions)
  FILE* Log_Module::get_logfile_stream (const ACE_Log_Record& lr)
  {
    // do we have our own outputmask defined?
    if (this->output_mask_)
    {
      // do we have a file output defined?
      if (ACE_BIT_ENABLED (this->output_mask_, OS_FILE))
      {
        // make sure the full filename is built
        this->build_file_path (lr.pid ());

        // do we already have a stream opened?
        if (this->file_stream_)
        {
          // check if we have a maximum size specified
          if (this->file_max_size_ > 0)
          {
            // determine current file size
            ACE_stat st;
            if (ACE_OS::fstat (ACE_OS::fileno (this->file_stream_), &st))
            {
              // report stat failure and return nullptr
              std::basic_stringstream<ACE_TCHAR> msgbuf;
              msgbuf << ACE_TEXT ("fstat() failed for logfile : ");
              msgbuf << ACE_TEXT_CHAR_TO_TCHAR (this->file_path_.c_str ());
              log_failure_message (this, msgbuf.str ().c_str ());
              return nullptr;
            }

            // check the current filesize plus the new logrecord
            if (this->file_max_size_ < static_cast<uint32_t> (st.st_size + lr.length ()))
            {
              // we need to roll; start by closing the current stream
              if (ACE_OS::fclose (this->file_stream_))
              {
                // report failure and return null
                std::basic_stringstream<ACE_TCHAR> msgbuf;
                msgbuf << ACE_TEXT ("fclose() failed for logfile : ");
                msgbuf << ACE_TEXT_CHAR_TO_TCHAR (this->file_path_.c_str ());
                log_failure_message (this, msgbuf.str ().c_str ());
                return nullptr;
              }
              this->file_stream_ = nullptr;

              // do we have versions or just a max size?
              if (this->file_count_ > 1)
              {
                // roll versions from last to first
                for (uint32_t n = this->file_count_-1; n>0 ;--n)
                {
                  // check for existence of version to roll to
                  std::stringstream fnbuf;
                  fnbuf << this->file_path_ << '.' << n;
                  std::string fnxt = fnbuf.str ();
                  // check if file exists
                  if (ACE_OS::access (fnxt.c_str (), F_OK) == 0)
                  {
                    // unlink outdated logfile
                    ACE_OS::unlink (fnxt.c_str ());
                  }
                  // check for existence of version to keep
                  fnbuf.str (this->file_path_);
                  if (n > 1) // version postfix?
                  {
                    fnbuf << '.' << n-1;
                  }
                  std::string fcur = fnbuf.str ();
                  // check if file exists
                  if (ACE_OS::access (fcur.c_str (), F_OK) == 0)
                  {
                    // rename to next version
                    if (ACE_OS::rename (fcur.c_str (), fnxt.c_str ()))
                    {
                      // report error and return null
                      std::basic_stringstream<ACE_TCHAR> msgbuf;
                      msgbuf << ACE_TEXT ("rename() failed for logfile : ");
                      msgbuf << ACE_TEXT_CHAR_TO_TCHAR (fcur.c_str ());
                      log_failure_message (this, msgbuf.str ().c_str ());
                      return nullptr;
                    }
                  }
                }
              }
              else
              {
                // just remove maxed out file
                if (ACE_OS::unlink (this->file_path_.c_str ()))
                {
                  // report error and return null
                  std::basic_stringstream<ACE_TCHAR> msgbuf;
                  msgbuf << ACE_TEXT ("unlink() failed for logfile : ");
                  msgbuf << ACE_TEXT_CHAR_TO_TCHAR (this->file_path_.c_str ());
                  log_failure_message (this, msgbuf.str ().c_str ());
                  return nullptr;
                }
              }
            }
          }
          // if we haven't rolled we can return the stream
          if (this->file_stream_)
          {
            return this->file_stream_;
          }
        }

        // we need to open a stream
        // avoid using ACE_OS::fopen as this may require ObjectManager initialization
        // which may not have happened yet
        this->file_stream_ =
            ::fopen (this->file_path_.c_str (),
                     ACE_BIT_ENABLED (this->file_flags_, FF_TRUNCATE) ? "w" : "a");
        if (!this->file_stream_)
        {
          // report open failure
          std::basic_stringstream<ACE_TCHAR> msgbuf;
          msgbuf << ACE_TEXT ("fopen() failed for logfile : ");
          msgbuf << ACE_TEXT_CHAR_TO_TCHAR (this->file_path_.c_str ());
          log_failure_message (this, msgbuf.str ().c_str ());
        }

        // ok, return stream
        return this->file_stream_;
      }
    }
    else
    { // if not; ask the base log module (if that isn't us)
      if (this != Log_Module::getInstance ())
      {
        return Log_Module::getInstance ()->get_logfile_stream (lr);
      }
    }
    return nullptr;
  }

  // Build full log filename according
  // to logfile base path and creation specs
  void Log_Module::build_file_path (long pid)
  {
    // initialize full file name if not yet done
    if (this->file_path_.empty ())
    {
      std::stringstream fnbuf;
      // set the base
      fnbuf << (this->file_base_.empty () ? this->category_ : this->file_base_);
      // do we need to add the PID?
      if (ACE_BIT_ENABLED (this->file_flags_, FF_PROCESS))
      {
        fnbuf << '-' << pid;
      }
      // append extension
      fnbuf << ".log";
      // store full name
      this->file_path_ = fnbuf.str ();
    }
  }

  struct log_streams
  {
    log_ostream_t<char> trace_    {LM_TRACE};
    log_ostream_t<char> debug_    {LM_DEBUG};
    log_ostream_t<char> info_     {LM_INFO};
    log_ostream_t<char> warning_  {LM_WARNING};
    log_ostream_t<char> error_    {LM_ERROR};
    log_ostream_t<char> critical_ {LM_CRITICAL};
    log_ostream_t<char> panic_    {LM_EMERGENCY};

    log_ostream_t<wchar_t> trace_w_    {LM_TRACE};
    log_ostream_t<wchar_t> debug_w_    {LM_DEBUG};
    log_ostream_t<wchar_t> info_w_     {LM_INFO};
    log_ostream_t<wchar_t> warning_w_  {LM_WARNING};
    log_ostream_t<wchar_t> error_w_    {LM_ERROR};
    log_ostream_t<wchar_t> critical_w_ {LM_CRITICAL};
    log_ostream_t<wchar_t> panic_w_    {LM_EMERGENCY};

    uint32_t indent_ {1};

    inline log_streams () = default;
  };

  /**
   * Log_Module, specific for the test framework.
   */
  TAOX11_Test_Log_Module* TAOX11_Test_Log_Module::getInstance()
  {
    static TAOX11_Test_Log_Module instance_;

    return &instance_;
  }

  TAOX11_Test_Log_Module::TAOX11_Test_Log_Module ()
    : x11_logger::Log_Module ("TESTX11",
                              x11_logger::X11_LogMask::LP_ALL,
                              x11_logger::X11_Verbose::V_PRIO|
                              x11_logger::X11_Verbose::V_TIME)
  {
  }

  TAOX11_Test_Log_Module::~TAOX11_Test_Log_Module ()
  {
  }

  /**
   * Helpers
   */
#if defined(__MINGW32__)
  //FUZZ: disable check_for_ACE_Thread_Mutex
  static thread_local log_streams __x11_loggers;
# if defined (ACE_HAS_THREADS)
# define  __x11_loggers \
    (*ACE_TSS_Singleton<log_streams, ACE_Thread_Mutex>::instance ())
# else
  static log_streams     __x11_loggers_var;
  static log_streams    &__x11_loggers = __x11_loggers_var;
# endif
  //FUZZ: enable check_for_ACE_Thread_Mutex
#else
  static thread_local log_streams __x11_loggers;
#endif

  log_ostream_t<char>& trace ()
  {
    return __x11_loggers.trace_;
  }
  log_ostream_t<char>& debug ()
  {
    return __x11_loggers.debug_;
  }
  log_ostream_t<char>& info ()
  {
    return __x11_loggers.info_;
  }
  log_ostream_t<char>& warning ()
  {
    return __x11_loggers.warning_;
  }
  log_ostream_t<char>& error ()
  {
    return __x11_loggers.error_;
  }
  log_ostream_t<char>& critical ()
  {
    return __x11_loggers.critical_;
  }
  log_ostream_t<char>& panic ()
  {
    return __x11_loggers.panic_;
  }

  log_ostream_t<wchar_t>& trace_w ()
  {
    return __x11_loggers.trace_w_;
  }
  log_ostream_t<wchar_t>& debug_w ()
  {
    return __x11_loggers.debug_w_;
  }
  log_ostream_t<wchar_t>& info_w ()
  {
    return __x11_loggers.info_w_;
  }
  log_ostream_t<wchar_t>& warning_w ()
  {
    return __x11_loggers.warning_w_;
  }
  log_ostream_t<wchar_t>& error_w ()
  {
    return __x11_loggers.error_w_;
  }
  log_ostream_t<wchar_t>& critical_w ()
  {
    return __x11_loggers.critical_w_;
  }
  log_ostream_t<wchar_t>& panic_w ()
  {
    return __x11_loggers.panic_w_;
  }

  uint32_t log_indent_inc ()
  {
    return __x11_loggers.indent_++;
  }
  uint32_t log_indent_dec ()
  {
    if (__x11_loggers.indent_ > 0)
      return --__x11_loggers.indent_;
    return 0;
  }

  // Flush log msg to logging backend/sink
  template <>
  void
  log_buffer_t<wchar_t>::log_record ()
  {
    // get ACE logger from TSS
    ACE_Log_Msg* ace_log = ACE_Log_Msg::instance ();

    // set the guard (the log framework lock is recursive)
    ACE_Log_Guard __guard (ace_log);

    // create a log record
    ACE_Log_Record lr (this->priority_, ACE_OS::gettimeofday () , ace_log->getpid ());

    if (this->lm_typeref_ == nullptr)
    {
      x11_logger::Log_Type_T<x11_logger::Log_Module> base_logmod;
      this->lm_typeref_ = base_logmod.get_reference ();
    }
    Log_Module * lm {};
    if (*this->lm_typeref_)
    {
      lm = (*this->lm_typeref_)->get_module ();
    }

    if (lm)
    {
      // lazy evaluation of priority mask
      if ((lm->priority_mask () & this->priority_) == this->priority_)
      {
        // build log record
        std::basic_stringstream<ACE_TCHAR> logbuf;
        logbuf << log_record_helper (lm, this->priority_, lr);
        logbuf << ACE_TEXT_WCHAR_TO_TCHAR (this->logmsg_.str ().c_str ());
        lr.msg_data (logbuf.str ().c_str ());

        // backup the ACE logger flags
        u_long const ace_log_flags  = ace_log->flags ();
        u_long lm_log_flags {};

        // reset logger flags
        ace_log->clr_flags (ACE_Log_Msg::STDERR|
                            ACE_Log_Msg::SYSLOG|
                            ACE_Log_Msg::LOGGER|
                            ACE_Log_Msg::CUSTOM);

        // get log module outputmask
        uint32_t const lm_omask = lm->output_mask ();
        // determine logger flags to set
        if (ACE_BIT_ENABLED (lm_omask, OS_STDERR))
          lm_log_flags |= ACE_Log_Msg::STDERR;
        if (ACE_BIT_ENABLED (lm_omask, OS_SYSLOG))
          lm_log_flags |= ACE_Log_Msg::SYSLOG;
        if (ACE_BIT_ENABLED (lm_omask, OS_IPC))
          lm_log_flags |= ACE_Log_Msg::LOGGER;
        if (ACE_BIT_ENABLED (lm_omask, OS_CUSTOM))
          lm_log_flags |= ACE_Log_Msg::CUSTOM;

        // update logger
        ace_log->set_flags(lm_log_flags);

        // send log record to standard backends
        ace_log->log (lr);

        // do we need to do file logging?
        if (ACE_BIT_ENABLED (lm_omask, OS_FILE))
        {
          // get output stream
          FILE *fout = lm->get_logfile_stream (lr);
          // print log record on stream
          lr.print (ace_log->local_host (), ace_log_flags, fout);
        }

        // reset logger
        ace_log->clr_flags(lm_log_flags);
        // restore logger flags
        ace_log->set_flags(ace_log_flags);
      }
    }
    else
    {
      // build log record
      lr.msg_data (ACE_TEXT_WCHAR_TO_TCHAR (this->logmsg_.str ().c_str ()));
      // send log record to standard backends
      ace_log->log (lr);
    }

    // reset log msg buffer
    this->logmsg_.str (std::wstring());
  }

  // flush log msg to logging backend/sink
  template <>
  void
  log_buffer_t<char>::log_record ()
  {
    // get ACE logger from TSS
    ACE_Log_Msg* ace_log = ACE_Log_Msg::instance ();

    // set the guard (the log framework lock is recursive)
    ACE_Log_Guard __guard (ace_log);

    // flush log msg to logging backend
    ACE_Log_Record lr (this->priority_, ACE_OS::gettimeofday () , ace_log->getpid ());

    if (this->lm_typeref_ == nullptr)
    {
      x11_logger::Log_Type_T<x11_logger::Log_Module> base_logmod;
      this->lm_typeref_ = base_logmod.get_reference ();
    }
    Log_Module * lm {};
    if (*this->lm_typeref_)
    {
      lm = (*this->lm_typeref_)->get_module ();
    }

    if (lm)
    {
      // lazy evaluation of priority mask
      if ((lm->priority_mask () & this->priority_) == this->priority_)
      {
        // build log record
        std::basic_stringstream<ACE_TCHAR> logbuf;
        logbuf << log_record_helper (lm, this->priority_, lr);
        logbuf << ACE_TEXT_CHAR_TO_TCHAR (this->logmsg_.str ().c_str ());
        lr.msg_data (logbuf.str ().c_str ());

        // backup the ACE logger flags
        u_long const ace_log_flags  = ace_log->flags ();
        u_long lm_log_flags = 0;

        // reset logger flags
        ace_log->clr_flags (ACE_Log_Msg::STDERR|
                            ACE_Log_Msg::SYSLOG|
                            ACE_Log_Msg::LOGGER|
                            ACE_Log_Msg::CUSTOM);

        // get log module outputmask
        uint32_t const lm_omask = lm->output_mask ();
        // determine logger flags to set
        if (ACE_BIT_ENABLED (lm_omask, OS_STDERR))
          lm_log_flags |= ACE_Log_Msg::STDERR;
        if (ACE_BIT_ENABLED (lm_omask, OS_SYSLOG))
          lm_log_flags |= ACE_Log_Msg::SYSLOG;
        if (ACE_BIT_ENABLED (lm_omask, OS_IPC))
          lm_log_flags |= ACE_Log_Msg::LOGGER;
        if (ACE_BIT_ENABLED (lm_omask, OS_CUSTOM))
          lm_log_flags |= ACE_Log_Msg::CUSTOM;

        // update logger
        ace_log->set_flags(lm_log_flags);

        // send log record to standard backends
        ace_log->log (lr);

        // do we need to do file logging?
        if (ACE_BIT_ENABLED (lm_omask, OS_FILE))
        {
          // get output stream
          FILE *fout = lm->get_logfile_stream (lr);
          // print log record on stream
          lr.print (ace_log->local_host (), ace_log_flags, fout);
        }

        // reset logger
        ace_log->clr_flags(lm_log_flags);
        // restore logger flags
        ace_log->set_flags(ace_log_flags);
      }
    }
    else
    {
      // build log record
      lr.msg_data (ACE_TEXT_CHAR_TO_TCHAR (this->logmsg_.str ().c_str ()));
      // send log record to standard backends
      ace_log->log (lr);
    }

    // reset log
    this->logmsg_.str (std::string());
  }

  std::ostream&
  operator<< (std::ostream& _os, __Timestamp _ts)
  {
    ACE_TCHAR day_and_time[27];
    const ACE_TCHAR *s {};
    if (!_ts.with_date_)
    {
      // Print just the time
      s = ACE::timestamp (day_and_time,
                          sizeof (day_and_time) / sizeof (ACE_TCHAR),
                          true);
    }
    else
    {
      // Print time and date
      ACE::timestamp (day_and_time,
                      sizeof (day_and_time) / sizeof (ACE_TCHAR));
      s = day_and_time;
    }
    return (_os << ACE_TEXT_ALWAYS_CHAR (s));
  }

  std::wostream&
  operator<< (std::wostream& _wos, __Timestamp _ts)
  {
    ACE_TCHAR day_and_time[27];
    const ACE_TCHAR *s {};
    if (!_ts.with_date_)
    {
      // Print just the time
      s = ACE::timestamp (day_and_time,
                          sizeof (day_and_time) / sizeof (ACE_TCHAR),
                          true);
    }
    else
    {
      // Print time and date
      ACE::timestamp (day_and_time,
                      sizeof (day_and_time) / sizeof (ACE_TCHAR));
      s = day_and_time;
    }
    return (_wos << ACE_TEXT_ALWAYS_WCHAR (s));
  }

  std::ostream&
  operator<< (std::ostream& os, __HexPrint hp)
  {
    os << "Buffer length = " << hp.len_ << "\n";
    uint32_t const max = (hp.len_ / 16) + ((hp.len_ % 16) > 0 ? 1 : 0);
    for (uint32_t l=0; l<max ;++l)
    {
      uint32_t off = l*16;
      uint32_t c=0;
      for (; c<16 && (off+c)<hp.len_ ;++c)
      {
        uint8_t b = (uint8_t)hp.buf_[off+c];
        os << std::hex << std::setw(2) << (uint16_t)b << ' ';
      }
      for (; c<16 ;++c)
        os << "   ";
      os << ' ' << std::dec;
      for (uint32_t c=0; c<16 && (off+c)<hp.len_ ;++c)
      {
        char ch = hp.buf_[off+c];
        os << (std::isprint (ch) ? ch : '.');
      }
      os << "\n";
    }
    return os;
  }

} /* x11_logger */
