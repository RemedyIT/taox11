/**
 * @file    upcall_command.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 upcall command base class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tao/x11/portable_server/upcall_command.h"
#include "tao/x11/system_exception.h"
#include "tao/x11/user_exception_proxy_out.h"

namespace TAOX11_NAMESPACE
{
  Upcall_Command::Upcall_Command (std::function<void()> execute_function) :
    execute_function_ (std::move(execute_function))
  {
  }

  /// calls _execute() catching/wrapping any user exceptions.
  void Upcall_Command::execute ()
  {
    try {
      this->execute_function_();
    }
    catch (const TAOX11_NAMESPACE::CORBA::UserException& _ex) {
      throw UserExceptionProxy::out (_ex);
    }
    catch (const TAOX11_NAMESPACE::CORBA::SystemException& _ex) {
      // reraise as TAO system exception
      _ex._raise_tao ();
    }
  }
} // namespace TAOX11_NAMESPACE

