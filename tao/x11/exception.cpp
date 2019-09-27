/**
 * @file    exception.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 core exception classes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tao/x11/exception.h"
#include <iostream>

namespace TAOX11_NAMESPACE {
  namespace CORBA {
    const char* Exception::what() const noexcept
    {
      return  this->name_;
    }

    void
    Exception::_info (std::ostream& strm) const
    {
      strm << this->name_;
    }

    const char *Exception::_rep_id () const
    {
      return this->id_;
    }

    const char *Exception::_name () const
    {
      return this->name_;
    }
  } // namespace CORBA
} // namespace TAOX11_NAMESPACE
