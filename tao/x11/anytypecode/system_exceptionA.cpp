/**
 * @file    system_exceptionA.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 CORBA System exception Any ops
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tao/x11/tao_corba.h"
#include "tao/x11/corba.h"
#include "tao/x11/system_exception.h"
#include "tao/x11/system_exception_x11.h"
#include "tao/x11/anytypecode/any.h"
#include "tao/x11/anytypecode/typecode_constants.h"
#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/anytypecode/any_dual_impl_t.h"

#if defined (THREAD_CANCELLED)
#undef THREAD_CANCELLED
#endif /* THREAD_CANCELLED */

namespace TAOX11_NAMESPACE
{
// Any extraction for various system exceptions
#define TAOX11_SYSTEM_EXCEPTION(name) \
  catch (const TAOX11_CORBA::name & x11_sysex) \
  { \
    TAOX11_CORBA::name * x11_sx_dup = \
        dynamic_cast< TAOX11_CORBA::name *> (x11_sysex._tao_duplicate ()); \
    std::unique_ptr< TAOX11_CORBA::name> x11_safe_sx (x11_sx_dup); \
    any >>= *x11_sx_dup; \
    x11_safe_sx.release (); \
    return std::unique_ptr< CORBA::SystemException> (x11_sx_dup); \
  }

  std::unique_ptr<CORBA::SystemException>
  extract_system_exception_from_any (const TAOX11_CORBA::Any& any)
  {
    std::unique_ptr<TAO_CORBA::SystemException> sysex (
        TAO_TAO::create_system_exception (any.type ()->id ().c_str ()));
    if (sysex)
    {
      try
      {
        // raise TAO exception as X11 version
        _raise_x11 (*sysex);
      }
      TAOX11_STANDARD_SYSTEM_EXCEPTION_LIST
    }
    return nullptr;
  }

#undef TAOX11_SYSTEM_EXCEPTION

// marshaling specializations for system exceptions
#define TAOX11_SYSTEM_EXCEPTION(name) \
  template <> \
  class CDR_Marshal_Policy<IDL::traits< CORBA::name>> \
  { \
  public: \
    static bool demarshal_value (TAO_InputCDR& cdr, IDL::traits< CORBA::name>::value_type& value) \
    { \
      try { \
        value._tao_decode (cdr); \
        return true; \
      } \
      catch (const CORBA::Exception&) { \
      } \
      return false; \
    } \
    static bool marshal_value (TAO_OutputCDR& cdr, const IDL::traits< CORBA::name>::value_type& value) \
    { \
      try { \
        value._tao_encode (cdr); \
        return true; \
      } \
      catch (const CORBA::Exception&) { \
      } \
      return false; \
    } \
  }; \

  TAOX11_STANDARD_SYSTEM_EXCEPTION_LIST

#undef TAOX11_SYSTEM_EXCEPTION

// Any ops for system exceptions

#define TAOX11_SYSTEM_EXCEPTION(name) \
  void \
  operator<<= (CORBA::Any &any, const CORBA::name &ex) \
  { \
    Any_Dual_Impl_T<IDL::traits< CORBA::name>, \
                    CDR_Marshal_Policy>::insert_copy ( \
        any, \
        CORBA::_tc_ ## name, \
        ex \
      ); \
  }

  TAOX11_STANDARD_SYSTEM_EXCEPTION_LIST

#undef TAOX11_SYSTEM_EXCEPTION

#define TAOX11_SYSTEM_EXCEPTION(name) \
  void \
  operator<<= (CORBA::Any &any, CORBA::name &&ex) \
  { \
    Any_Dual_Impl_T<IDL::traits< CORBA::name>, \
                    CDR_Marshal_Policy>::insert ( \
        any, \
        CORBA::_tc_ ## name, \
        std::move (ex) \
      ); \
  }

  TAOX11_STANDARD_SYSTEM_EXCEPTION_LIST

#undef TAOX11_SYSTEM_EXCEPTION

#define TAOX11_SYSTEM_EXCEPTION(name) \
  bool \
  operator>>= (const CORBA::Any &any, CORBA::name &ex) \
  { \
    return Any_Dual_Impl_T<IDL::traits< CORBA::name>, \
                           CDR_Marshal_Policy>::extract ( \
              any, \
              CORBA::_tc_ ## name, \
              ex \
            ); \
  }

  TAOX11_STANDARD_SYSTEM_EXCEPTION_LIST

#undef TAOX11_SYSTEM_EXCEPTION

} // namespace TAOX11_NAMESPACE
