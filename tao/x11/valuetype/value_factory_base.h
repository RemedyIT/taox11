/**
 * @file    value_factory_base.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 ValueFactory base class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_VALUETYPE_VALUE_FACTORY_BASE_H_INCLUDED
#define TAOX11_VALUETYPE_VALUE_FACTORY_BASE_H_INCLUDED

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/x11/valuetype/taox11_valuetype_export.h"
#include "tao/x11/valuetype/value_base.h"
#include "tao/x11/valuetype/abstract_base.h"
#include "tao/x11/object.h"

namespace TAOX11_NAMESPACE
{

  namespace CORBA
  {
    class ValueFactoryBase;

    template<>
    TAOX11_Valuetype_Export object_traits<CORBA::ValueFactoryBase>::shared_ptr_type
    object_traits<CORBA::ValueFactoryBase>::lock_shared (
        CORBA::ValueFactoryBase*);
  } // namespace CORBA

  namespace IDL
  {
    template<>
    struct traits <CORBA::ValueFactoryBase> :
      public IDL::common_byval_traits <CORBA::object_reference <CORBA::ValueFactoryBase>>,
      public CORBA::object_traits <CORBA::ValueFactoryBase>
    {
      typedef std::true_type is_local;
      typedef std::false_type is_abstract;

      template <typename OStrm_, typename Formatter = formatter<CORBA::ValueFactoryBase, OStrm_>>
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

  namespace CORBA
  {
    class TAOX11_Valuetype_Export ValueFactoryBase
      : public virtual CORBA::LocalObject
    {
    protected:
      ValueFactoryBase () = default;
       virtual ~ValueFactoryBase ()= default;

      friend struct object_traits<ValueFactoryBase>;

    private:
      ValueFactoryBase (const ValueFactoryBase&) = delete;
      ValueFactoryBase& operator =(const ValueFactoryBase&) = delete;
      ValueFactoryBase& operator =(ValueFactoryBase&&) = delete;

    public:
      typedef IDL::traits<ValueFactoryBase>           _traits_type;
      typedef IDL::traits<ValueFactoryBase>::ref_type _ref_type;

      virtual const std::string& _interface_repository_id () const;

      virtual CORBA::valuetype_reference<CORBA::ValueBase> create_for_unmarshal () = 0;

      virtual const std::string& _obv_repository_id () const;
    };

  } // namespace CORBA

  namespace IDL
  {
    template <typename OStrm_>
    struct formatter< CORBA::ValueFactoryBase, OStrm_>
    {
      inline OStrm_& operator ()(
          OStrm_& os_,
          IDL::traits<CORBA::ValueFactoryBase>::in_type o_)
      {
        os_ << "IDL::traits<CORBA::ValueFactoryBase>::ref_type {local object:"
            << o_.operator -> ()<< "}";
        return os_;
      }
    };

    template <typename OStrm_, typename Fmt>
    inline OStrm_& operator <<(
        OStrm_& os,
        IDL::traits<CORBA::ValueFactoryBase>::__Writer<Fmt> w)
    {
      typedef IDL::traits<CORBA::ValueFactoryBase>::__Writer<Fmt> writer_t;
      typedef typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<CORBA::ValueFactoryBase, OStrm_>,
                          typename writer_t::formatter_t>::type formatter_t;
      return IDL::traits<CORBA::ValueFactoryBase>::write_on (
          os, w.val_,
          formatter_t ());
    }
  } // namespace IDL
} // namespace TAOX11_NAMESPACE

#endif // TAOX11_VALUETYPE_VALUE_FACTORY_BASE_H_INCLUDED
