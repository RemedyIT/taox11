/**
 * @file    PolicyC.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 Policy client stub class.
 * Handcrafted after initial generation by RIDL.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
/*
 * ****  Code generated by the RIDL Compiler ****
 * RIDL has been developed by:
 *        Remedy IT Expertise BV
 *        The Netherlands
 *        https://www.remedy.nl
 */

#ifndef __RIDL_POLICYC_H_INCLUDED__
#define __RIDL_POLICYC_H_INCLUDED__

#include /**/ "ace/pre.h"

#include "tao/x11/stddef.h"
#include "tao/x11/basic_traits.h"
#include "tao/x11/corba.h"
#include "tao/x11/system_exception.h"
#include "tao/x11/object.h"
#include "tao/x11/user_exception.h"
#include "tao/x11/anytypecode/any.h"
#include "tao/x11/anytypecode/typecode_ref.h"
#include "tao/x11/taox11_export.h"

using namespace TAOX11_NAMESPACE;

// generated from c++/cli_hdr/include.erb
#include "tao/x11/UShortSeqC.h"

// generated from c++/cli_hdr/include.erb
#include "tao/x11/CurrentC.h"

// generated from c++/cli_hdr/include.erb
#include "tao/x11/Policy_ForwardC.h"

// generated from StubHeaderWriter#enter_module
namespace TAOX11_NAMESPACE {
  namespace CORBA {

    // generated from c++/cli_hdr/typedef.erb
    using PolicyErrorCode = int16_t;

    // generated from c++/anyop_hdr/typedef.erb
    extern TAOX11_AnyTypeCode_Export TAOX11_CORBA::typecode_reference const _tc_PolicyErrorCode;

    // generated from StubHeaderWriter#visit_const
    constexpr PolicyErrorCode BAD_POLICY = 0;

    // generated from StubHeaderWriter#visit_const
    constexpr PolicyErrorCode UNSUPPORTED_POLICY = 1;

    // generated from StubHeaderWriter#visit_const
    constexpr PolicyErrorCode BAD_POLICY_TYPE = 2;

    // generated from StubHeaderWriter#visit_const
    constexpr PolicyErrorCode BAD_POLICY_VALUE = 3;

    // generated from StubHeaderWriter#visit_const
    constexpr PolicyErrorCode UNSUPPORTED_POLICY_VALUE = 4;

    // generated from c++/cli_hdr/except_pre.erb
    class TAOX11_Export PolicyError final
     : public TAOX11_CORBA::UserException
    {
    public:
      const char* what() const noexcept override;

      void _raise () const override;

      void _tao_encode (TAO_OutputCDR &cdr) const override;

      void _tao_decode (TAO_InputCDR &cdr) override;

      /// Deep copy
      TAOX11_CORBA::Exception *_tao_duplicate () const override;

      // generated from c++/cli_hdr/except_post.erb
      PolicyError ();
      virtual ~PolicyError () throw ();
      PolicyError (const PolicyError&) = default;
      PolicyError (PolicyError&&) = default;
      explicit PolicyError (TAOX11_CORBA::PolicyErrorCode reason);
      PolicyError& operator= (const PolicyError& x);
      PolicyError& operator= (PolicyError&& x);

      void reason (const TAOX11_CORBA::PolicyErrorCode& _reason);
      void reason (TAOX11_CORBA::PolicyErrorCode&& _reason);
      const TAOX11_CORBA::PolicyErrorCode& reason () const;
      TAOX11_CORBA::PolicyErrorCode& reason ();
    protected:
      void _info (std::ostream& strm) const override;
    private:
      TAOX11_CORBA::PolicyErrorCode reason_;
    }; // PolicyError

    // generated from c++/anyop_hdr/typedef.erb
    extern TAOX11_AnyTypeCode_Export TAOX11_CORBA::typecode_reference const _tc_PolicyError;

    // generated from c++/cli_hdr/except_pre.erb
    class TAOX11_Export InvalidPolicies final
     : public TAOX11_CORBA::UserException
    {
    public:
      const char* what() const noexcept override;

      void _raise () const override;

      void _tao_encode (TAO_OutputCDR &cdr) const override;

      void _tao_decode (TAO_InputCDR &cdr) override;

      /// Deep copy
      TAOX11_CORBA::Exception *_tao_duplicate () const override;

      // generated from c++/cli_hdr/except_post.erb
      InvalidPolicies ();
      virtual ~InvalidPolicies () throw ();
      InvalidPolicies (const InvalidPolicies&) = default;
      InvalidPolicies (InvalidPolicies&&) = default;
      explicit InvalidPolicies (TAOX11_CORBA::UShortSeq indices);
      InvalidPolicies& operator= (const InvalidPolicies& x);
      InvalidPolicies& operator= (InvalidPolicies&& x);

      void indices (const TAOX11_CORBA::UShortSeq& _indices);
      void indices (TAOX11_CORBA::UShortSeq&& _indices);
      const TAOX11_CORBA::UShortSeq& indices () const;
      TAOX11_CORBA::UShortSeq& indices ();

    protected:
      void _info (std::ostream& strm) const override;
    private:
      TAOX11_CORBA::UShortSeq indices_;
    }; // InvalidPolicies

    // generated from c++/anyop_hdr/typedef.erb
    extern TAOX11_AnyTypeCode_Export TAOX11_CORBA::typecode_reference const _tc_InvalidPolicies;

    // generated from StubHeaderWriter#enter_interface

     // generated from c++/cli_hdr/interface_fwd.erb
#if !defined (_INTF_TAOX11_NAMESPACE__CORBA__POLICY_FWD_)
#define _INTF_TAOX11_NAMESPACE__CORBA__POLICY_FWD_
    class TAOX11_Export Policy;
    class TAOX11_Export Policy_proxy;
    using Policy_proxy_ptr = Policy_proxy*;
    namespace POA
    {
      class TAOX11_Export Policy;
    }
#endif // !_INTF_TAOX11_NAMESPACE__CORBA__POLICY_FWD_
     // generated from CxxWriterBase#at_global_scope
  } // namespace TAOX11_NAMESPACE
} // namespace CORBA
 // entering CxxWriterBase#at_global_scope

 // generated from c++/cli_hdr/interface_object_traits.erb
#if !defined (_INTF_TAOX11_NAMESPACE__CORBA__POLICY_TRAITS_DECL_)
#define _INTF_TAOX11_NAMESPACE__CORBA__POLICY_TRAITS_DECL_
namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {
    template<>
    TAOX11_Export object_traits< TAOX11_CORBA::Policy>::shared_ptr_type
    object_traits< TAOX11_CORBA::Policy>::lock_shared (
        TAOX11_CORBA::Policy*);
    template<>
    TAOX11_Export object_traits< TAOX11_CORBA::Policy>::ref_type object_traits< TAOX11_CORBA::Policy>::narrow (
        object_traits< TAOX11_CORBA::Object>::ref_type);
  } // namespace CORBA

  namespace IDL
  {
    template<>
    struct traits < TAOX11_CORBA::Policy> :
      public IDL::common_byval_traits <IDL::traits< TAOX11_CORBA::Policy>::ref_type>,
      public CORBA::object_traits < TAOX11_CORBA::Policy>
    {
      static constexpr bool local = false;
      static constexpr bool abstract = false;

      template <typename OStrm_, typename Formatter = formatter<TAOX11_CORBA::Policy, OStrm_>>
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
#endif // !_INTF_TAOX11_NAMESPACE__CORBA__POLICY_TRAITS_DECL_

 // leaving CxxWriterBase#at_global_scope
namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {

    // generated from c++/cli_hdr/interface_pre.erb
    class TAOX11_Export Policy
      : public virtual TAOX11_CORBA::Object
    {
    public:
      friend struct TAOX11_CORBA::object_traits<Policy>;

      using _traits_type = IDL::traits<Policy>;
      using _ref_type = _traits_type::ref_type;

      virtual const std::string& _interface_repository_id () const;

      // generated from c++/cli_hdr/attribute.erb
      virtual TAOX11_CORBA::PolicyType policy_type();

      // generated from c++/cli_hdr/operation.erb
      virtual object_traits<Policy>::ref_type copy ();

      // generated from c++/cli_hdr/operation.erb
      virtual void destroy ();

      static TAOX11_CORBA::object_traits<Policy>::ref_type _narrow (
          TAOX11_CORBA::object_traits< TAOX11_CORBA::Object>::ref_type);

    protected:
      template <typename _Tp1, typename, typename ...Args>
      friend object_reference<_Tp1> make_reference(Args&& ...args);

      Policy ();
      explicit Policy (TAOX11_NAMESPACE::Object_proxy_ptr op);
      ~Policy () = default;

      virtual object_traits<Policy>::ref_type copy_i ();

    private:
      Policy(const Policy&) = delete;
      Policy& operator=(const Policy&) = delete;
    }; // Policy

    // generated from c++/anyop_hdr/typedef.erb
    extern TAOX11_AnyTypeCode_Export TAOX11_CORBA::typecode_reference const _tc_Policy;

  } // namespace CORBA
} // namespace TAOX11_NAMESPACE

 // generated from StubHeaderTraitsWriter#pre_visit
namespace TAOX11_NAMESPACE {
  namespace IDL {

#if !defined (_INTF_FMT_TAOX11_NAMESPACE__CORBA__POLICY_TRAITS_DECL_)
#define _INTF_FMT_TAOX11_NAMESPACE__CORBA__POLICY_TRAITS_DECL_
    template <typename OStrm_>
    struct formatter< CORBA::Policy, OStrm_>
    {
      OStrm_& operator ()(
          OStrm_&,
          IDL::traits<CORBA::Policy>::in_type);
    };

    template <typename OStrm_, typename Fmt>
    inline OStrm_& operator <<(
        OStrm_&,
        IDL::traits<CORBA::Policy>::__Writer<Fmt>);
#endif // !_INTF_FMT_TAOX11_NAMESPACE__CORBA__POLICY_TRAITS_DECL_

     // generated from c++/cli_hdr/struct_idl_traits.erb
#if !defined (_STRUCT_TAOX11_NAMESPACE__CORBA__POLICYERROR_TRAITS_)
#define _STRUCT_TAOX11_NAMESPACE__CORBA__POLICYERROR_TRAITS_
     template<>
     struct traits < TAOX11_CORBA::PolicyError>
       : IDL::common_traits< TAOX11_CORBA::PolicyError>
     {
       template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
       static inline OStrm_& write_on(
           OStrm_& os_, in_type val_,
           Formatter fmt_ = Formatter ())
       {
         return fmt_ (os_, val_);
       }

       template <typename Formatter = std::false_type>
       static inline __Writer<Formatter> write (in_type val) { return {val} ; }
     };

     template <typename OStrm_>
     struct formatter< TAOX11_CORBA::PolicyError, OStrm_>
     {
       inline OStrm_& operator ()(
           OStrm_& os_,
           IDL::traits<TAOX11_CORBA::PolicyError>::in_type)
       {
         return os_ << "CORBA::PolicyError";
       }
     };

     template <typename OStrm_, typename Fmt>
     inline OStrm_& operator <<(
         OStrm_& os,
         IDL::traits<TAOX11_CORBA::PolicyError>::__Writer<Fmt> w)
     {
       using writer_t = IDL::traits<TAOX11_CORBA::PolicyError>::__Writer<Fmt>;
       using formatter_t = typename std::conditional<
                           std::is_same<
                             typename writer_t::formatter_t,
                             std::false_type>::value,
                           formatter<TAOX11_CORBA::PolicyError, OStrm_>,
                           typename writer_t::formatter_t>::type;
       return IDL::traits<TAOX11_CORBA::PolicyError>::write_on (
           os, w.val_,
           formatter_t ());
     }
#endif // _STRUCT_TAOX11_NAMESPACE__CORBA__POLICYERROR_TRAITS_

   // generated from c++/cli_hdr/struct_idl_traits.erb
#if !defined (_STRUCT_TAOX11_NAMESPACE__CORBA__INVALIDPOLICIES_TRAITS_)
#define _STRUCT_TAOX11_NAMESPACE__CORBA__INVALIDPOLICIES_TRAITS_
     template<>
     struct traits < TAOX11_CORBA::InvalidPolicies>
       : IDL::common_traits< TAOX11_CORBA::InvalidPolicies>
     {
       template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
       static inline OStrm_& write_on(
           OStrm_& os_, in_type val_,
           Formatter fmt_ = Formatter ())
       {
         return fmt_ (os_, val_);
       }

       template <typename Formatter = std::false_type>
       static inline __Writer<Formatter> write (in_type val) { return {val} ; }
     };

     template <typename OStrm_>
     struct formatter< TAOX11_CORBA::InvalidPolicies, OStrm_>
     {
       inline OStrm_& operator ()(
           OStrm_& os_,
           IDL::traits<TAOX11_CORBA::InvalidPolicies>::in_type)
       {
         return os_ << "CORBA::InvalidPolicies";
       }
     };

     template <typename OStrm_, typename Fmt>
     inline OStrm_& operator <<(
         OStrm_& os,
         IDL::traits<TAOX11_CORBA::InvalidPolicies>::__Writer<Fmt> w)
     {
       using writer_t = IDL::traits<TAOX11_CORBA::InvalidPolicies>::__Writer<Fmt>;
       using formatter_t = typename std::conditional<
                           std::is_same<
                             typename writer_t::formatter_t,
                             std::false_type>::value,
                           formatter<TAOX11_CORBA::InvalidPolicies, OStrm_>,
                           typename writer_t::formatter_t>::type;
       return IDL::traits<TAOX11_CORBA::InvalidPolicies>::write_on (
           os, w.val_,
           formatter_t ());
     }
#endif // _STRUCT_TAOX11_NAMESPACE__CORBA__INVALIDPOLICIES_TRAITS_
  } // namespace IDL
} // namespace TAOX11_NAMESPACE

// generated from StubHeaderIDLTraitsDefWriter#pre_visit
namespace TAOX11_NAMESPACE {
  namespace IDL {

   template <typename OStrm_>
   inline OStrm_&
   formatter< CORBA::Policy, OStrm_>::operator ()(
       OStrm_& os_,
       IDL::traits<CORBA::Policy>::in_type o_)
   {
     return os_ << IDL::traits< TAOX11_NAMESPACE::CORBA::Object>::_dump (o_, "CORBA::Policy");
   }

   template <typename OStrm_, typename Fmt>
   inline OStrm_& operator <<(
       OStrm_& os,
       IDL::traits<CORBA::Policy>::__Writer<Fmt> w)
   {
     using writer_t = IDL::traits<CORBA::Policy>::__Writer<Fmt>;
     using formatter_t = typename std::conditional<
                         std::is_same<
                           typename writer_t::formatter_t,
                           std::false_type>::value,
                         formatter<CORBA::Policy, OStrm_>,
                         typename writer_t::formatter_t>::type;
     return IDL::traits<CORBA::Policy>::write_on (
         os, w.val_,
         formatter_t ());
   }

  } // namespace IDL
} // namespace TAOX11_NAMESPACE

namespace TAOX11_NAMESPACE
{
   // generated from c++/cli_hdr/anyop.erb
#if !defined (__TAOX11_ANYOP_TAOX11_NAMESPACE__CORBA__POLICYERROR_DECL__)
#define __TAOX11_ANYOP_TAOX11_NAMESPACE__CORBA__POLICYERROR_DECL__
  TAOX11_AnyTypeCode_Export void operator<<= (TAOX11_CORBA::Any &, const TAOX11_CORBA::PolicyError&);
  TAOX11_AnyTypeCode_Export void operator<<= (TAOX11_CORBA::Any &, TAOX11_CORBA::PolicyError&&);
  TAOX11_AnyTypeCode_Export bool operator>>= (const TAOX11_CORBA::Any &, TAOX11_CORBA::PolicyError&);
#endif

#if !defined (__TAOX11_ANYOP_TAOX11_NAMESPACE__CORBA__INVALIDPOLICIES_DECL__)
#define __TAOX11_ANYOP_TAOX11_NAMESPACE__CORBA__INVALIDPOLICIES_DECL__
  TAOX11_AnyTypeCode_Export void operator<<= (TAOX11_CORBA::Any &, const TAOX11_CORBA::InvalidPolicies&);
  TAOX11_AnyTypeCode_Export void operator<<= (TAOX11_CORBA::Any &, TAOX11_CORBA::InvalidPolicies&&);
  TAOX11_AnyTypeCode_Export bool operator>>= (const TAOX11_CORBA::Any &, TAOX11_CORBA::InvalidPolicies&);
#endif

   // generated from c++/cli_hdr/anyop.erb
#if !defined (__TAOX11_ANYOP_TAOX11_NAMESPACE__CORBA__POLICY_DECL__)
#define __TAOX11_ANYOP_TAOX11_NAMESPACE__CORBA__POLICY_DECL__
  TAOX11_AnyTypeCode_Export void operator<<= (TAOX11_CORBA::Any &, IDL::traits< TAOX11_CORBA::Policy>::ref_type);
  TAOX11_AnyTypeCode_Export bool operator>>= (const TAOX11_CORBA::Any &, IDL::traits< TAOX11_CORBA::Policy>::_ref_type&);
#endif

} // namespace TAOX11_NAMESPACE

// generated from StubHeaderStdWriter#pre_visit
namespace std {
} // namespace std

// generated from c++/cli_inl/except_inl.erb
inline TAOX11_CORBA::PolicyError::PolicyError ()
  : TAOX11_CORBA::UserException ("IDL:CORBA/PolicyError:1.0", "TAOX11_CORBA::PolicyError")
  , reason_ (0)
{
}
inline TAOX11_CORBA::PolicyError::PolicyError (PolicyErrorCode reason)
  : TAOX11_CORBA::UserException ("IDL:CORBA/PolicyError:1.0", "TAOX11_CORBA::PolicyError")
  , reason_ (std::move (reason))
{
}

inline void TAOX11_CORBA::PolicyError::reason (const PolicyErrorCode& _reason) { this->reason_ = _reason; }
inline void TAOX11_CORBA::PolicyError::reason (PolicyErrorCode&& _reason) { this->reason_ = std::move (_reason); }
inline const TAOX11_CORBA::PolicyErrorCode& TAOX11_CORBA::PolicyError::reason () const { return this->reason_; }
inline TAOX11_CORBA::PolicyErrorCode& TAOX11_CORBA::PolicyError::reason () { return this->reason_; }

inline TAOX11_CORBA::PolicyError& TAOX11_CORBA::PolicyError::operator= (const PolicyError& x)
{
  if (this != &x)
  {
    this->UserException::operator = (x);
    this->reason_ = x.reason_;
  }
  return *this;
}
inline TAOX11_CORBA::PolicyError& TAOX11_CORBA::PolicyError::operator= (TAOX11_CORBA::PolicyError&& x)
{
  if (this != &x)
  {
    this->UserException::operator = (x);
    this->reason_ = std::move (x.reason_);
  }
  return *this;
}


// generated from c++/cli_inl/except_inl.erb
inline TAOX11_CORBA::InvalidPolicies::InvalidPolicies ()
  : TAOX11_CORBA::UserException ("IDL:CORBA/InvalidPolicies:1.0", "TAOX11_CORBA::InvalidPolicies")
  , indices_ ()
{
}
inline TAOX11_CORBA::InvalidPolicies::InvalidPolicies (TAOX11_CORBA::UShortSeq indices)
  : TAOX11_CORBA::UserException ("IDL:CORBA/InvalidPolicies:1.0", "TAOX11_CORBA::InvalidPolicies")
  , indices_ (std::move (indices))
{
}

inline void TAOX11_CORBA::InvalidPolicies::indices (const TAOX11_CORBA::UShortSeq& _indices) { this->indices_ = _indices; }
inline void TAOX11_CORBA::InvalidPolicies::indices (TAOX11_CORBA::UShortSeq&& _indices) { this->indices_ = std::move (_indices); }
inline const TAOX11_CORBA::UShortSeq& TAOX11_CORBA::InvalidPolicies::indices () const { return this->indices_; }
inline TAOX11_CORBA::UShortSeq& TAOX11_CORBA::InvalidPolicies::indices () { return this->indices_; }

inline TAOX11_CORBA::InvalidPolicies& TAOX11_CORBA::InvalidPolicies::operator= (const TAOX11_CORBA::InvalidPolicies& x)
{
  if (this != &x)
  {
    this->UserException::operator = (x);
    this->indices_ = x.indices_;
  }
  return *this;
}
inline TAOX11_CORBA::InvalidPolicies& TAOX11_CORBA::InvalidPolicies::operator= (TAOX11_CORBA::InvalidPolicies&& x)
{
  if (this != &x)
  {
    this->UserException::operator = (x);
    this->indices_ = std::move (x.indices_);
  }
  return *this;
}

// generated from c++/cli_hdr/interface_os.erb
TAOX11_Export std::ostream& operator<< (
    std::ostream& strm,
    IDL::traits< TAOX11_CORBA::Policy>::ref_type);

#if defined (__TAOX11_INCLUDE_STUB_PROXY__)
#include "PolicyP.h"
#endif

#include /**/ "ace/post.h"

#endif // __RIDL_POLICYC_H_INCLUDED__

// -*- END -*-
