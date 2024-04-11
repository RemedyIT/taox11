/**
 * @file    object_fwd.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 forward decls CORBA core Object classes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_OBJECT_FWD_H_INCLUDED
#define TAOX11_OBJECT_FWD_H_INCLUDED

#include /**/ "ace/config-all.h"

#include "tao/x11/base/stddef.h"
#include "tao/x11/object_traits_t.h"
#include "tao/x11/base/idl_traits_t.h"

namespace TAOX11_NAMESPACE
{
  class Object_proxy;
  using Object_proxy_ptr = Object_proxy *;
  using Object_proxy_ref = std::unique_ptr<Object_proxy> ;

  namespace CORBA
  {
    // forwards
    class Object;
    class LocalObject;

    /// (Local)Object trait method specializations
    template<>
    TAOX11_Export object_traits<CORBA::Object>::shared_ptr_type
    object_traits<CORBA::Object>::lock_shared (CORBA::Object*);

    template<>
    TAOX11_Export object_traits<CORBA::Object>::ref_type
    object_traits<CORBA::Object>::narrow (
        object_traits<CORBA::Object>::ref_type);

    template<>
    TAOX11_Export object_traits<CORBA::LocalObject>::shared_ptr_type
    object_traits<CORBA::LocalObject>::lock_shared (
        CORBA::LocalObject*);
  } // namespace CORBA

  namespace IDL
  {
    template<>
    struct traits <CORBA::Object> :
      public IDL::common_byval_traits <CORBA::object_reference <CORBA::Object>>,
      public CORBA::object_traits <CORBA::Object>
    {
      using is_abstract = std::false_type;
      using is_local = std::false_type;

      template <typename OStrm_, typename Formatter = formatter<CORBA::Object, OStrm_>>
      static inline OStrm_& write_on(
          OStrm_& os_, in_type val_,
          Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }

      TAOX11_Export static std::string _dump (ref_type, const char*, bool = false);
    };

    template<>
    struct traits <CORBA::LocalObject> :
      public IDL::common_byval_traits <CORBA::object_reference <CORBA::LocalObject>>,
      public CORBA::object_traits <CORBA::LocalObject>
    {
      using is_abstract = std::false_type;
      using is_local = std::true_type;

      using base_type = CORBA::LocalObject;

      template <typename OStrm_, typename Formatter = formatter<CORBA::LocalObject, OStrm_>>
      static inline OStrm_& write_on(
          OStrm_& os_, in_type val_,
          Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };
  } // namespace IDL
} // namespace TAOX11_NAMESPACE

#endif // TAOX11_OBJECT_FWD_H_INCLUDED
