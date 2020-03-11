/**
 * @file    exception.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 core exception classes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_EXCEPTION_H_INCLUDED
#define TAOX11_EXCEPTION_H_INCLUDED

#include "tao/x11/stddef.h"
#include /**/ "tao/Versioned_Namespace.h"
#include "tao/x11/anytypecode/taox11_anytypecode_export.h"
#include "tao/x11/idl_traits_t.h"

#include <sstream>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// forward declared TAO CDR streams
class TAO_OutputCDR;
class TAO_InputCDR;

TAO_END_VERSIONED_NAMESPACE_DECL

namespace TAOX11_NAMESPACE {
  namespace CORBA {
    class Exception;
  }
}

namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {

    class Any; // forward

    class TAOX11_Export Exception : public std::exception
    {
    public:
      /// Destructor.
      virtual ~Exception () throw () = default;

      const char* what() const noexcept override;

      /// Throw the exception (when using the standard mapping).
      virtual void _raise () const = 0;

      /// Return the repository ID of the Exception.
      virtual const char *_rep_id () const;

      /// Return the name of the Exception.
      virtual const char *_name () const;

      // = These are TAO-specific extensions.

      virtual void _tao_encode (TAO_OutputCDR &cdr) const = 0;

      virtual void _tao_decode (TAO_InputCDR &cdr) = 0;

      /// Deep copy
      virtual CORBA::Exception *_tao_duplicate () const = 0;

      /// TAOX11 implementation detail
      virtual void _any_insert (CORBA::Any &) const = 0;
      virtual void _any_insert (CORBA::Any &) = 0;

    protected:
      template <typename T, typename OStrm_> friend struct IDL::formatter;

      Exception () = default;
      Exception (const Exception &) = default;
      Exception (Exception &&) = default;
      Exception & operator = (const Exception &) = default;
      Exception & operator = (Exception &&) = default;

      /// Construct from a repository id.
      Exception (const char *repository_id, const char *local_name)
        : id_ (repository_id), name_ (local_name)
      {
      }
      /// Add the information of the exception to a stream.
      virtual void _info (std::ostream &strm) const = 0;

    private:
      /// Repository Id
      const char *id_ {};

      /// Local name.
      const char *name_ {};
    }; // Exception

  } // namespace CORBA

  namespace IDL
  {
    // IDL traits
    template<>
    struct traits<CORBA::Exception>
      : public common_traits<CORBA::Exception>
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
    struct formatter< CORBA::Exception, OStrm_>
    {
      inline OStrm_& operator ()(
          OStrm_& os_,
          IDL::traits<CORBA::Exception>::in_type x_)
      {
        x_._info (os_);
        return os_;
      }
    };

    template <typename OStrm_, typename Fmt>
    inline OStrm_& operator <<(
        OStrm_& os,
        IDL::traits<CORBA::Exception>::__Writer<Fmt> w)
    {
      typedef IDL::traits<CORBA::Exception>::__Writer<Fmt> writer_t;
      typedef typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<CORBA::Exception, OStrm_>,
                          typename writer_t::formatter_t>::type formatter_t;
      return IDL::traits<CORBA::Exception>::write_on (
          os, w.val_,
          formatter_t ());
    }
  } // namespace IDL

  // Any inserters (implemented in anytypecode library)
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, const CORBA::Exception &);
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, CORBA::Exception&&);

} // namespace TAOX11_NAMESPACE

inline std::ostream& operator <<(std::ostream& os_, const TAOX11_NAMESPACE::CORBA::Exception& x_)
{
  return TAOX11_NAMESPACE::IDL::traits<TAOX11_CORBA::Exception>::write_on(os_, x_);
}

#endif // TAOX11_EXCEPTION_H_INCLUDED
