/**
 * @file    system_exception.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 System exception classes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_SYSTEM_EXCEPTION_H_INCLUDED
#define TAOX11_SYSTEM_EXCEPTION_H_INCLUDED

#include "tao/x11/exception.h"

namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {
    /**
     * @enum CompletionStatus
     *
     * @brief Completion status for CORBA system exceptions
     *
     * Enumerations that denote how an operation was (not) completed
     * when a @c CORBA::SystemException is thrown.
     */
    enum CompletionStatus
    {
      COMPLETED_YES,     ///< Successful or exceptional completion.
      COMPLETED_NO,      ///< Didn't change any state; retry is OK.
      COMPLETED_MAYBE    ///< Can't say what happened; retry unsafe.
    };

    class TAOX11_Export SystemException : public Exception
    {
    public:
      // Make IDL::formatter our friend in order to let it use our _info operation
      template <typename T, typename Y> friend struct TAOX11_NAMESPACE::IDL::formatter;

      /// Destructor.
      ~SystemException () noexcept override = default;

      /// Get the minor status.
      uint32_t minor () const
      {
        return this->minor_;
      }

      /// Set the minor status.
      void minor (uint32_t m)
      {
        this->minor_ = m;
      }

      /// Get the completion status.
      CORBA::CompletionStatus completed () const
      {
        return this->completed_;
      }

      /// Set the operation completion status.
      void completed (CORBA::CompletionStatus c)
      {
        this->completed_ = c;
      }

      // = These are TAO-specific extensions.
      void _tao_encode (TAO_OutputCDR &cdr) const override;

      void _tao_decode (TAO_InputCDR &cdr) override;

      /// Deep copy
      CORBA::Exception *_tao_duplicate () const override;

      // = TAO X11 extension

      /// raise as a TAO CORBA::SystemException derivative
      virtual void _raise_tao () const = 0;

      /// Helper to create a minor status value.
      static uint32_t _tao_minor_code (u_int location, int errno_value);

    protected:
      SystemException () = default;
      SystemException (const SystemException &) = default;
      SystemException (SystemException &&) = default;
      SystemException & operator = (const SystemException &) = default;
      SystemException & operator = (SystemException &&) = default;

      SystemException (uint32_t code, CORBA::CompletionStatus completed);

      /// Constructor using a repository id.
      SystemException (const char *repository_id,
                       const char *local_name,
                       uint32_t code,
                       CORBA::CompletionStatus completed);

      void _info (std::ostream& strm) const override;

    private:
      /// Minor code.
      uint32_t minor_ {};

      /// Completion status.
      CORBA::CompletionStatus completed_ {CORBA::COMPLETED_NO};
    }; // SystemException

/*---------------------------------------------------------
 * CORBA standard system exceptions:
      UNKNOWN                 // the unknown exception
      BAD_PARAM               // an invalid parameter was passed
      NO_MEMORY               // memory allocation failure
      IMP_LIMIT               // violated implementation limit
      COMM_FAILURE            // communication failure
      INV_OBJREF              // invalid object reference
      OBJECT_NOT_EXIST        // no such object
      NO_PERMISSION           // no permission for operation
      INTERNAL                // ORB internal error
      MARSHAL                 // error marshaling param/result
      INITIALIZE              // ORB initialization failure
      NO_IMPLEMENT            // implementation unavailable
      BAD_TYPECODE            // bad typecode
      BAD_OPERATION           // invalid operation
      NO_RESOURCES            // out of resources for request
      NO_RESPONSE             // response not yet available
      PERSIST_STORE           // persistent storage failure
      BAD_INV_ORDER           // routine invocations out of order
      TRANSIENT               // transient error, try again later
      FREE_MEM                // cannot free memory
      INV_IDENT               // invalid identifier syntax
      INV_FLAG                // invalid flag was specified
      INTF_REPOS              // interface repository unavailable
      BAD_CONTEXT             // error processing context object
      OBJ_ADAPTER             // object adapter failure
      DATA_CONVERSION         // data conversion error
      INV_POLICY              // invalid policies present
      REBIND                  // rebind needed
      TIMEOUT                 // operation timed out
      TRANSACTION_UNAVAILABLE // no transaction
      TRANSACTION_MODE        // invalid transaction mode
      TRANSACTION_REQUIRED    // operation needs transaction
      TRANSACTION_ROLLEDBACK  // operation was a no-op
      INVALID_TRANSACTION     // invalid TP context passed
      CODESET_INCOMPATIBLE    // incompatible code set
      BAD_QOS                 // bad quality of service
      INVALID_ACTIVITY
      ACTIVITY_COMPLETED
      ACTIVITY_REQUIRED
      THREAD_CANCELLED
 */

#define TAOX11_STANDARD_SYSTEM_EXCEPTION_LIST \
    TAOX11_SYSTEM_EXCEPTION (UNKNOWN) \
    TAOX11_SYSTEM_EXCEPTION (BAD_PARAM) \
    TAOX11_SYSTEM_EXCEPTION (NO_MEMORY) \
    TAOX11_SYSTEM_EXCEPTION (IMP_LIMIT) \
    TAOX11_SYSTEM_EXCEPTION (COMM_FAILURE) \
    TAOX11_SYSTEM_EXCEPTION (INV_OBJREF) \
    TAOX11_SYSTEM_EXCEPTION (OBJECT_NOT_EXIST) \
    TAOX11_SYSTEM_EXCEPTION (NO_PERMISSION) \
    TAOX11_SYSTEM_EXCEPTION (INTERNAL) \
    TAOX11_SYSTEM_EXCEPTION (MARSHAL) \
    TAOX11_SYSTEM_EXCEPTION (INITIALIZE) \
    TAOX11_SYSTEM_EXCEPTION (NO_IMPLEMENT) \
    TAOX11_SYSTEM_EXCEPTION (BAD_TYPECODE) \
    TAOX11_SYSTEM_EXCEPTION (BAD_OPERATION) \
    TAOX11_SYSTEM_EXCEPTION (NO_RESOURCES) \
    TAOX11_SYSTEM_EXCEPTION (NO_RESPONSE) \
    TAOX11_SYSTEM_EXCEPTION (PERSIST_STORE) \
    TAOX11_SYSTEM_EXCEPTION (BAD_INV_ORDER) \
    TAOX11_SYSTEM_EXCEPTION (TRANSIENT) \
    TAOX11_SYSTEM_EXCEPTION (FREE_MEM) \
    TAOX11_SYSTEM_EXCEPTION (INV_IDENT) \
    TAOX11_SYSTEM_EXCEPTION (INV_FLAG) \
    TAOX11_SYSTEM_EXCEPTION (INTF_REPOS) \
    TAOX11_SYSTEM_EXCEPTION (BAD_CONTEXT) \
    TAOX11_SYSTEM_EXCEPTION (OBJ_ADAPTER) \
    TAOX11_SYSTEM_EXCEPTION (DATA_CONVERSION) \
    TAOX11_SYSTEM_EXCEPTION (INV_POLICY) \
    TAOX11_SYSTEM_EXCEPTION (REBIND) \
    TAOX11_SYSTEM_EXCEPTION (TIMEOUT) \
    TAOX11_SYSTEM_EXCEPTION (TRANSACTION_UNAVAILABLE) \
    TAOX11_SYSTEM_EXCEPTION (TRANSACTION_MODE) \
    TAOX11_SYSTEM_EXCEPTION (TRANSACTION_REQUIRED) \
    TAOX11_SYSTEM_EXCEPTION (TRANSACTION_ROLLEDBACK) \
    TAOX11_SYSTEM_EXCEPTION (INVALID_TRANSACTION) \
    TAOX11_SYSTEM_EXCEPTION (CODESET_INCOMPATIBLE) \
    TAOX11_SYSTEM_EXCEPTION (BAD_QOS) \
    TAOX11_SYSTEM_EXCEPTION (INVALID_ACTIVITY) \
    TAOX11_SYSTEM_EXCEPTION (ACTIVITY_COMPLETED) \
    TAOX11_SYSTEM_EXCEPTION (ACTIVITY_REQUIRED) \
    TAOX11_SYSTEM_EXCEPTION (THREAD_CANCELLED)


  // Declarations for all of the CORBA standard exceptions.
  //
  // @@ - shouldn't have a default minor code, at least for code that's
  // inside the ORB.  All minor codes should be symbolically catalogued.

#define TAOX11_SYSTEM_EXCEPTION(name) \
    class TAOX11_Export name final : public SystemException \
    { \
    public: \
      ~name () noexcept override = default; \
      name (); \
      name (uint32_t code, CORBA::CompletionStatus completed); \
      name (const name &) = default; \
      name (name &&) = default; \
      name & operator = (const name &) = default; \
      name & operator = (name &&) = default; \
      const char* what() const noexcept override; \
      void _raise () const override; \
      CORBA::Exception *_tao_duplicate () const override; \
      void _raise_tao () const override; \
      void _any_insert (CORBA::Any &) const override; \
      void _any_insert (CORBA::Any &) override; \
    };

    TAOX11_STANDARD_SYSTEM_EXCEPTION_LIST

#undef TAOX11_SYSTEM_EXCEPTION

  } // namespace CORBA

  namespace IDL
  {
#define TAOX11_SYSTEM_EXCEPTION(name) \
    template <> \
    struct traits<CORBA::name> \
      : public common_traits<CORBA::name> \
    { \
      template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>> \
      static inline OStrm_& write_on( \
          OStrm_& os_, in_type val_, \
          Formatter fmt_ = Formatter ()) \
      { \
        return fmt_ (os_, val_); \
      } \
      \
      template <typename Formatter = std::false_type> \
      static inline __Writer<Formatter> write (in_type val) { return {val} ; } \
    }; \
    \
    template <typename OStrm_> \
    struct formatter< CORBA::name, OStrm_> \
    { \
      inline OStrm_& operator ()( \
          OStrm_& os_, \
          IDL::traits<CORBA::name>::in_type x_) \
      { \
        x_._info (os_); \
        return os_; \
      } \
    }; \
    \
    template <typename OStrm_, typename Fmt> \
    inline OStrm_& operator <<( \
        OStrm_& os, \
        IDL::traits<CORBA::name>::__Writer<Fmt> w) \
    { \
      using writer_t = IDL::traits<CORBA::name>::__Writer<Fmt>; \
      using formatter_t = typename std::conditional< \
                          std::is_same< \
                            typename writer_t::formatter_t, \
                            std::false_type>::value, \
                          formatter<CORBA::name, OStrm_>, \
                          typename writer_t::formatter_t>::type; \
      return IDL::traits<CORBA::name>::write_on ( \
          os, w.val_, \
          formatter_t ()); \
    }

    TAOX11_STANDARD_SYSTEM_EXCEPTION_LIST

#undef TAOX11_SYSTEM_EXCEPTION

  } // namespace CORBA

#define TAOX11_SYSTEM_EXCEPTION(name) \
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, const CORBA::name &); \
  TAOX11_AnyTypeCode_Export void operator<<= (CORBA::Any &, CORBA::name &&); \
  TAOX11_AnyTypeCode_Export bool operator>>= (const CORBA::Any &, CORBA::name &); \

  TAOX11_STANDARD_SYSTEM_EXCEPTION_LIST

#undef TAOX11_SYSTEM_EXCEPTION

  TAOX11_Export
  std::unique_ptr<CORBA::SystemException>
  create_system_exception (const std::string&);

  TAOX11_AnyTypeCode_Export
  std::unique_ptr<CORBA::SystemException>
  extract_system_exception_from_any (const CORBA::Any&);

} // namespace TAOX11_NAMESPACE

inline std::ostream& operator<< (
    std::ostream& strm,
    TAOX11_NAMESPACE::CORBA::CompletionStatus status)
{
  switch (status)
  {
    case TAOX11_NAMESPACE::CORBA::COMPLETED_YES: return strm << "YES"; break;
    case TAOX11_NAMESPACE::CORBA::COMPLETED_NO: return strm << "NO"; break;
    case TAOX11_NAMESPACE::CORBA::COMPLETED_MAYBE: return strm << "MAYBE"; break;
    default: return strm << "UNKNOWN"; break;
  }
}

#endif // TAOX11_SYSTEM_EXCEPTION_H_INCLUDED
