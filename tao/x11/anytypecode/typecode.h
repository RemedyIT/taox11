/**
 * @file    typecode.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 TypeCode class
 *
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_TYPECODE_H
#define TAOX11_TYPECODE_H

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/x11/anytypecode/typecode_ref.h"
#include "tao/x11/anytypecode/any.h"
#include "tao/x11/object.h"
#include "tao/x11/user_exception.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// forward decls TAO CDR streams
class TAO_OutputCDR;
class TAO_InputCDR;

TAO_END_VERSIONED_NAMESPACE_DECL

namespace TAOX11_NAMESPACE
{

  namespace CORBA
  {
    using Visibility = int16_t; // forward
    using ValueModifier = int16_t; // forward
    enum class TCKind : uint32_t; // forward

    class TypeCode_impl;

    // abstract base class
    class TAOX11_AnyTypeCode_Export TypeCode
      : public LocalObject
    {
    public:

      class TAOX11_AnyTypeCode_Export Bounds final
        : public TAOX11_NAMESPACE::CORBA::UserException
      {
      public:
        void _raise () const override;

        void _tao_encode (TAO_OutputCDR &cdr) const override;

        void _tao_decode (TAO_InputCDR &cdr) override;

        /// Deep copy
        TAOX11_NAMESPACE::CORBA::Exception *_tao_duplicate () const override;

        Bounds ();
        ~Bounds () noexcept override = default;
        Bounds (const Bounds&) = default;
        Bounds (Bounds&&) = default;
        Bounds& operator= (const Bounds& x);
        Bounds& operator= (Bounds&& x);
      protected:
        void _info (std::ostream&) const override;
      }; // Bounds

      static TAOX11_NAMESPACE::CORBA::typecode_reference const _tc_Bounds;

      class TAOX11_AnyTypeCode_Export BadKind final
        : public TAOX11_NAMESPACE::CORBA::UserException
      {
        public:
          void _raise () const override;

          void _tao_encode (TAO_OutputCDR &cdr) const override;

          void _tao_decode (TAO_InputCDR &cdr) override;

          /// Deep copy
          TAOX11_NAMESPACE::CORBA::Exception *_tao_duplicate () const override;

          BadKind ();
          ~BadKind () noexcept override = default;
          BadKind (const BadKind&) = default;
          BadKind (BadKind&&) = default;
          BadKind& operator= (const BadKind& x);
          BadKind& operator= (BadKind&& x);
        protected:
          void _info (std::ostream&) const override;
      }; // BadKind

      static TAOX11_NAMESPACE::CORBA::typecode_reference const _tc_BadKind;

      using _traits_type = typecode_traits;
      using _ref_type = typecode_reference;

      virtual bool equal (typecode_reference tc) const = 0;

      virtual bool equivalent (typecode_reference tc) const = 0;

      /// The kind of @c TypeCode.
      virtual TAOX11_CORBA::TCKind kind () const = 0;

      virtual typecode_reference get_compact_typecode () const = 0;

      virtual std::string id () const = 0;

      virtual std::string name () const = 0;

      /// The type member count.
      /**
       * This method is valid for the following kinds of @c TypeCodes:
       *
       * @li @c tk_struct
       * @li @c tk_union
       * @li @c tk_enum
       * @li @c tk_value
       * @li @c tk_except
       * @li @c tk_event
       */
      virtual uint32_t member_count () const = 0;

      /// The type member name.
      /**
       * This method is valid for the following kinds of @c TypeCodes:
       *
       * @li @c tk_struct
       * @li @c tk_union
       * @li @c tk_enum
       * @li @c tk_value
       * @li @c tk_except
       * @li @c tk_event
       *
       * @note CORBA::TypeCode::member_name() does not follow the usual
       *       parameter passing rules defined by the C++ mapping.  This
       *       behavior is required by the C++ mapping.  In particular,
       *       ownership is maintained by the @c TypeCode.
       */
      virtual std::string member_name (uint32_t index) const = 0;

      /// The type member @c TypeCode.
      /**
       * This method is valid for the following kinds of @c TypeCodes:
       *
       * @li @c tk_struct
       * @li @c tk_union
       * @li @c tk_value
       * @li @c tk_except
       * @li @c tk_event
       */
      virtual typecode_reference member_type (uint32_t index) const = 0;

      /// The union member label.
      /**
       * This method is valid for the following kinds of @c TypeCodes:
       *
       * @li @c tk_union
       */
      virtual CORBA::Any member_label (uint32_t index) const = 0;

      /// The type of all non-default union member labels.
      /**
       * This method is valid for the following kinds of @c TypeCodes:
       *
       * @li @c tk_union
       */
      virtual typecode_reference discriminator_type () const = 0;

      /// The index of the default union member.
      /**
       * This method is valid for the following kinds of @c TypeCodes:
       *
       * @li @c tk_union
       */
      virtual int32_t default_index () const = 0;

      /// The length of the type.
      /**
       * This method is valid for the following kinds of @c TypeCodes:
       *
       * @li @c tk_string
       * @li @c tk_wstring
       * @li @c tk_sequence
       * @li @c tk_array
       */
      virtual uint32_t length () const = 0;

      /// The underlying content type.
      /**
       * This method is valid for the following kinds of @c TypeCodes:
       *
       * @li @c tk_sequence
       * @li @c tk_array
       * @li @c tk_value_box
       * @li @c tk_alias
       */
      virtual typecode_reference content_type () const = 0;

      /// The number of significant digits.
      /**
       * This method is valid for the following kinds of @c TypeCodes:
       *
       * @li @c tk_fixed
       */
      virtual uint16_t fixed_digits () const = 0;

      /// The scale factor.
      /**
       * This method is valid for the following kinds of @c TypeCodes:
       *
       * @li @c tk_fixed
       */
      virtual uint16_t fixed_scale () const = 0;

      /// The @c Visibility of the @c valuetype or @c eventtype member
      /// corresponding to index @a index.
      /**
       * This method is valid for the following kinds of @c TypeCodes:
       *
       * @li @c tk_value
       * @li @c tk_event
       */
      virtual Visibility member_visibility (uint32_t index) const = 0;

      /// The @c ValueModifier of the @c valuetype of @c eventtype
      /// represented by this @c TypeCode.
      /**
       * This method is valid for the following kinds of @c TypeCodes:
       *
       * @li @c tk_value
       * @li @c tk_event
       */
      virtual ValueModifier type_modifier () const = 0;

      /// The @c TypeCode corresponding to the concrete base
      /// @c valuetype or @c eventtype.
      /**
       * This method is valid for the following kinds of @c TypeCodes:
       *
       * @li @c tk_value
       * @li @c tk_event
       *
       * @return @c TypeCode corresponding to the concrete base
       *         @c valuetype or @c eventtype.
       *         @c CORBA::TypeCode::_nil() if no concrete base exists.
       */
      //TypeCode_ptr concrete_base_type () const;
      virtual typecode_reference concrete_base_type () const = 0;

      //@}
    protected:
      friend class TypeCode_impl;

      /// Constructor.
      TypeCode () = default;

      /// Destructor.
      /**
       * Protected destructor to enforce proper memory management
       * through the reference counting mechanism.
       */
      ~TypeCode () = default;

      virtual bool _tc_marshal (TAO_OutputCDR&) = 0;
      virtual bool _tc_unmarshal (TAO_InputCDR&) = 0;

      virtual TypeCode_impl* _impl () = 0;

    private:
      // Prevent copying and assignment.
      TypeCode (TypeCode const &) = delete;
      TypeCode& operator= (TypeCode const &) = delete;
    };
  } // namespace CORBA
} // namespace TAOX11_NAMESPACE

namespace TAOX11_NAMESPACE
{
  namespace IDL
  {
#if !defined (_INTF_FMT_TAOX11_NAMESPACE__CORBA__TYPECODE_TRAITS_DECL_)
#define _INTF_FMT_TAOX11_NAMESPACE__CORBA__TYPECODE_TRAITS_DECL_
    template <typename OStrm_>
    struct formatter<CORBA::TypeCode, OStrm_>
    {
      OStrm_& operator ()(
          OStrm_&,
          IDL::traits<CORBA::TypeCode>::in_type);
    };

    template <typename OStrm_, typename Fmt>
    OStrm_& operator <<(
        OStrm_&,
        IDL::traits<CORBA::TypeCode>::__Writer<Fmt>);
#endif // !_INTF_FMT_TAOX11_NAMESPACE_CORBA_TYPECODE_TRAITS_DECL_

    template <typename OStrm_>
    inline OStrm_& formatter<CORBA::TypeCode, OStrm_>::operator ()(
        OStrm_& os_,
        IDL::traits<CORBA::TypeCode>::in_type tc)
    {
      return os_ << IDL::traits<TAOX11_CORBA::Object>::_dump (
                        tc,
                        "CORBA::TypeCode");
    }

    template <typename OStrm_, typename Fmt>
    inline OStrm_& operator <<(
        OStrm_& os,
        IDL::traits<CORBA::TypeCode>::__Writer<Fmt> w)
    {
      using writer_t = IDL::traits<CORBA::TypeCode>::__Writer<Fmt>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<CORBA::TypeCode, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<CORBA::TypeCode>::write_on (
          os, w.val_,
          formatter_t ());
    }

    template<>
    struct traits < CORBA::TypeCode::Bounds>
      : IDL::common_traits<CORBA::TypeCode::Bounds>
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
    struct formatter< CORBA::TypeCode::Bounds, OStrm_>
    {
      inline OStrm_& operator ()(
          OStrm_& os_,
          IDL::traits<CORBA::TypeCode::Bounds>::in_type)
      {
        os_ << "CORBA::TypeCode::Bounds";
        return os_;
      }
    };

    template <typename OStrm_, typename Fmt>
    inline OStrm_& operator <<(
        OStrm_& os,
        IDL::traits<CORBA::TypeCode::Bounds>::__Writer<Fmt> w)
    {
      using writer_t = IDL::traits<CORBA::TypeCode::Bounds>::__Writer<Fmt>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<CORBA::TypeCode::Bounds, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<CORBA::TypeCode::Bounds>::write_on (
          os, w.val_,
          formatter_t ());
    }

    template<>
    struct traits < CORBA::TypeCode::BadKind>
      : IDL::common_traits<CORBA::TypeCode::BadKind>
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
    struct formatter< CORBA::TypeCode::BadKind, OStrm_>
    {
      inline OStrm_& operator ()(
          OStrm_& os_,
          IDL::traits<CORBA::TypeCode::BadKind>::in_type)
      {
        os_ << "CORBA::TypeCode::BadKind";
        return os_;
      }
    };

    template <typename OStrm_, typename Fmt>
    inline OStrm_& operator <<(
        OStrm_& os,
        IDL::traits<CORBA::TypeCode::BadKind>::__Writer<Fmt> w)
    {
      using writer_t = IDL::traits<CORBA::TypeCode::BadKind>::__Writer<Fmt>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<CORBA::TypeCode::BadKind, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<CORBA::TypeCode::BadKind>::write_on (
          os, w.val_,
          formatter_t ());
    }
  } // namespace IDL

  TAOX11_AnyTypeCode_Export void operator<<= (TAOX11_CORBA::Any &, const ::TAOX11_NAMESPACE::CORBA::TypeCode::Bounds&);
  TAOX11_AnyTypeCode_Export void operator<<= (TAOX11_CORBA::Any &, ::TAOX11_NAMESPACE::CORBA::TypeCode::Bounds&&);
  TAOX11_AnyTypeCode_Export bool operator>>= (const TAOX11_CORBA::Any &, ::TAOX11_NAMESPACE::CORBA::TypeCode::Bounds&);

  TAOX11_AnyTypeCode_Export void operator<<= (TAOX11_CORBA::Any &, const ::TAOX11_NAMESPACE::CORBA::TypeCode::BadKind&);
  TAOX11_AnyTypeCode_Export void operator<<= (TAOX11_CORBA::Any &, ::TAOX11_NAMESPACE::CORBA::TypeCode::BadKind&&);
  TAOX11_AnyTypeCode_Export bool operator>>= (const TAOX11_CORBA::Any &, ::TAOX11_NAMESPACE::CORBA::TypeCode::BadKind&);

  inline CORBA::TypeCode::Bounds::Bounds ()
        : CORBA::UserException ("IDL:omg.org/CORBA/TypeCode/Bounds:1.0",
                                "Bounds")
  {}

  inline CORBA::TypeCode::Bounds&
    CORBA::TypeCode::Bounds::operator= (const CORBA::TypeCode::Bounds& x)
  {
    if (this != std::addressof(x))
      {
        this->UserException::operator = (x);
      }
    return *this;
  }

  inline CORBA::TypeCode::Bounds&
  CORBA::TypeCode::Bounds::operator= (CORBA::TypeCode::Bounds&& x)
  {
    if (this != std::addressof(x))
      {
        this->UserException::operator = (x);
      }
    return *this;
  }

  inline CORBA::TypeCode::BadKind::BadKind ()
      : CORBA::UserException ("IDL:omg.org/CORBA/TypeCode/BadKind:1.0",
                              "BadKind")
  {}

  inline CORBA::TypeCode::BadKind&
    CORBA::TypeCode::BadKind::operator= (const CORBA::TypeCode::BadKind& x)
  {
    if (this != std::addressof(x))
      {
        this->UserException::operator = (x);
      }
    return *this;
  }

  inline CORBA::TypeCode::BadKind&
  CORBA::TypeCode::BadKind::operator= (CORBA::TypeCode::BadKind&& x)
  {
    if (this != std::addressof(x))
      {
        this->UserException::operator = (x);
      }
    return *this;
  }
} // namespace TAOX11_NAMESPACE

#endif /* TAO_TYPECODE_H */
