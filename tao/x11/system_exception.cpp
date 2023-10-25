/**
 * @file    system_exception.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 System exception classes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/tao_corba.h"
#include "tao/SystemException.h"
#include "tao/x11/system_exception.h"

#include "tao/x11/system_exception_x11.h"
#include "ace/OS_Memory.h"

#include "tao/x11/anyinsert_adapter.h"

namespace TAOX11_NAMESPACE {

  std::unique_ptr<TAOX11_CORBA::SystemException>
  create_system_exception (const std::string& id)
  {
    // use TAO to create system exception from id
    std::unique_ptr<TAO_CORBA::SystemException> sysex (
        TAO_TAO::create_system_exception (id.c_str ()));
    if (sysex)
    {
      try
      {
        // raise TAO exception as X11 version
        _raise_x11 (*sysex);
      }
      catch (const TAOX11_CORBA::SystemException& sysex_x11)
      {
        // create duplicate by deep copy
        TAOX11_CORBA::Exception * ex_x11 = sysex_x11._tao_duplicate ();
        if (ex_x11)
        {
          // cast to TAOX11 system exception and return safely
          return std::unique_ptr<TAOX11_CORBA::SystemException> (
              dynamic_cast<TAOX11_CORBA::SystemException*> (ex_x11));
        }
      }
    }
    return nullptr;
  }

  CORBA::SystemException::SystemException (const char *repository_id,
                                           const char *local_name,
                                           uint32_t code,
                                           CORBA::CompletionStatus completed)
    : CORBA::Exception (repository_id,
                        local_name),
      minor_ (code),
      completed_ (completed)
  {
  }

  CORBA::SystemException::SystemException (uint32_t code,
                                           CORBA::CompletionStatus completed)
    : minor_ (code),
      completed_ (completed)
  {
  }

  void CORBA::SystemException::_info (std::ostream& strm) const
  {
    CORBA::Exception::_info(strm);
    strm << " { Minor code : " << this->minor();
    strm << " , Completed : " << this->completed ();
    strm << " }";
  }

  void
  CORBA::SystemException::_tao_encode (TAO_OutputCDR &cdr) const
  {
    if (cdr.write_string (this->_name ())
        && cdr.write_ulong (this->minor ())
        && cdr.write_ulong (this->completed ()))
      {
        return;
      }

    throw ::CORBA::MARSHAL ();
  }

  void
  CORBA::SystemException::_tao_decode (TAO_InputCDR &cdr)
  {
    // The string is read by the caller, to determine the exact type of
    // the exception.  We just decode the fields...
    // cdr.read_string (this->id ());
    TAO_CORBA::ULong tmp;

    if (cdr.read_ulong (this->minor_)
        && cdr.read_ulong (tmp))
      {
        this->completed_ = CORBA::CompletionStatus (tmp);
        return;
      }

    throw ::CORBA::MARSHAL ();
  }

  CORBA::Exception *
  CORBA::SystemException::_tao_duplicate () const
  {
    return {};
  }

  uint32_t
  CORBA::SystemException::_tao_minor_code (u_int location, int errno_value)
  {
    return TAO_CORBA::SystemException::_tao_minor_code (location, errno_value);
  }

#if defined (THREAD_CANCELLED)
#undef THREAD_CANCELLED
#endif /* THREAD_CANCELLED */

  // Concrete SystemException constructors and destructor
  // define the template
#define TAOX11_SYSTEM_EXCEPTION(name) \
  CORBA::name ::name () \
    :  CORBA::SystemException ("IDL:omg.org/CORBA/" #name ":1.0", \
                               #name, \
                               0, \
                               CORBA::COMPLETED_NO) \
  { \
  } \
  \
  CORBA::name ::name (uint32_t code, CORBA::CompletionStatus completed) \
    : CORBA::SystemException ("IDL:omg.org/CORBA/" #name ":1.0", \
                              #name, \
                              code, \
                              completed) \
  { \
  } \
  void TAOX11_NAMESPACE::CORBA::name::_any_insert (CORBA::Any &_any) const \
  { \
    AnyInsertAdapter *adapter = AnyInsertAdapter::any_insert_adapter (); \
    if (adapter) \
     { \
       adapter->insert_into_any (std::addressof(_any), *this); \
     } \
  } \
  void TAOX11_NAMESPACE::CORBA::name::_any_insert (CORBA::Any &_any) \
  { \
    AnyInsertAdapter *adapter = AnyInsertAdapter::any_insert_adapter (); \
    if (adapter) \
     { \
       adapter->insert_into_any (std::addressof(_any), *this); \
     } \
  } \

  // expand the list
  TAOX11_STANDARD_SYSTEM_EXCEPTION_LIST
  // undefine the template
#undef TAOX11_SYSTEM_EXCEPTION

  // what()

#define TAOX11_SYSTEM_EXCEPTION(name) \
  const char* \
  CORBA::name ::what () const noexcept \
  { \
    return "CORBA::" #name; \
  }

  // expand the list
  TAOX11_STANDARD_SYSTEM_EXCEPTION_LIST
  // undefine the template
#undef TAOX11_SYSTEM_EXCEPTION

  // _raise()

#define TAOX11_SYSTEM_EXCEPTION(name) \
  void \
  CORBA::name ::_raise () const \
  { \
    throw *this; \
  }

  // expand the list
  TAOX11_STANDARD_SYSTEM_EXCEPTION_LIST
  // undefine the template
#undef TAOX11_SYSTEM_EXCEPTION

  // _raise_tao()

#define TAOX11_SYSTEM_EXCEPTION(_nam_) \
  void \
  CORBA::_nam_ ::_raise_tao () const \
  { \
    TAO_CORBA::_nam_ * result = new (std::nothrow) \
      TAO_CORBA::_nam_ (TAO_CORBA::ULong (this->minor ()), \
      TAO_CORBA::CompletionStatus (this->completed ())); \
    if (!result) \
    { \
      throw TAO_CORBA::NO_MEMORY (); \
    } \
    else \
    { \
      std::unique_ptr < TAO_CORBA::_nam_ > x_safe (result); \
      throw *result; \
    } \
  }

  // expand the list
  TAOX11_STANDARD_SYSTEM_EXCEPTION_LIST
  // undefine the template
#undef TAOX11_SYSTEM_EXCEPTION

  // _tao_duplicate()

#define TAOX11_SYSTEM_EXCEPTION(name) \
  CORBA::Exception * \
  CORBA::name ::_tao_duplicate () const \
  { \
    return new (std::nothrow) CORBA::name (*this); \
  }

  // expand the list
  TAOX11_STANDARD_SYSTEM_EXCEPTION_LIST
  // undefine the template
#undef TAOX11_SYSTEM_EXCEPTION

#define TAOX11_SYSTEM_EXCEPTION(name) \
  catch (const TAO_CORBA::name & _tao_sx) { \
    throw TAOX11_NAMESPACE::CORBA::name ((uint32_t)_tao_sx.minor (), \
                                 (TAOX11_NAMESPACE::CORBA::CompletionStatus)_tao_sx.completed ()); \
  }

  void _raise_x11 (const TAO_CORBA::SystemException& _sx)
  {
    try {
      _sx._raise ();
    }
    TAOX11_STANDARD_SYSTEM_EXCEPTION_LIST
    catch (...) {
      throw TAOX11_NAMESPACE::CORBA::UNKNOWN ();
    }
  }

  // undefine the template
#undef TAOX11_SYSTEM_EXCEPTION

} // namespace TAOX11_NAMESPACE
