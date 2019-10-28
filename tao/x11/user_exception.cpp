/**
 * @file    user_exception.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 User exception base class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/user_exception.h"

namespace TAOX11_NAMESPACE
{
  void
  CORBA::UserException::_info (std::ostream& strm) const
  {
    TAOX11_CORBA::Exception::_info(strm);
  }

  void CORBA::UserException::_tao_encode (TAO_OutputCDR &) const
  {
    // noop
  }
  void CORBA::UserException::_tao_decode (TAO_InputCDR &)
  {
    // noop
  }
  void CORBA::UserException::_any_insert (CORBA::Any &) const
  {
    // noop
  }
  void CORBA::UserException::_any_insert (CORBA::Any &)
  {
    // noop
  }
} // namespace TAOX11_NAMESPACE
