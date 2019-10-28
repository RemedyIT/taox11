/**
 * @file    any_unknown_type.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 CORBA Any implementation class for
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/AnyTypeCode/Marshal.h"
#include "tao/AnyTypeCode/Null_RefCount_Policy.h"
#include "tao/AnyTypeCode/Value_TypeCode_Static.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"

#include "tao/x11/anytypecode/any_unknown_type.h"
#include "tao/x11/anytypecode/any.h"
#include "tao/x11/anytypecode/typecode_impl.h"
#include "tao/x11/anytypecode/typecode_constants.h"
#include "tao/x11/system_exception.h"
#include "tao/x11/exception_macros.h"
#include "tao/x11/stub_arg_traits.h"
#include "tao/x11/valuetype_adapter.h"

#include "tao/CDR.h"

#include "ace/OS_NS_string.h"

namespace TAOX11_NAMESPACE
{
#if !defined(CORBA_E_MICRO)

  /**
   * @class Marshal_Abstract
   *
   * @brief Marshal_Abstract
   *
   * marshal an abstract reference
   * defined here since this functionality is missing from TAO
   */
  class Marshal_Abstract
  {
  public:
    /// skip operation
    static TAO::traverse_status skip (TAO_CORBA::TypeCode_ptr tc,
                                      TAO_InputCDR *context);

    /// append operation
    static TAO::traverse_status append (TAO_CORBA::TypeCode_ptr tc,
                                        TAO_InputCDR *src,
                                        TAO_OutputCDR *dest);
  };

  TAO::traverse_status
  Marshal_Abstract::skip (TAO_CORBA::TypeCode_ptr, TAO_InputCDR *stream)
  {
    // First check for discriminator
    TAO_CORBA::Boolean discriminator;
    stream->read_boolean (discriminator);

    // object or value?
    if (discriminator)
    {
      // object
      TAO_Marshal_ObjRef marshal;
      return marshal.skip (nullptr, stream);
    }
    else
    {
      // value; use ValueTypeAdapter to read value from stream thereby advancing pointers
      ValueTypeAdapter * const adapter = ValueTypeAdapter::value_type_adapter ();
      if (adapter)
      {
        CORBA::valuetype_reference<CORBA::ValueBase> val;
        if (adapter->stream_to_value (*stream, val))
        {
          return TAO::TRAVERSE_CONTINUE;
        }
      }

      return TAO::TRAVERSE_STOP;
    }
  }

  TAO::traverse_status
  Marshal_Abstract::append (TAO_CORBA::TypeCode_ptr,
                            TAO_InputCDR *src,
                            TAO_OutputCDR *dest)
  {
    // First check for discriminator
    TAO_CORBA::Boolean discriminator;
    src->read_boolean (discriminator);

    // object or value?
    if (discriminator)
    {
      // object
      TAO_Marshal_ObjRef marshal;
      return marshal.append (nullptr, src, dest);
    }
    else
    {
      // value; use ValueTypeAdapter to read value from source stream
      // and write to destination stream
      ValueTypeAdapter * const adapter = ValueTypeAdapter::value_type_adapter ();
      if (adapter)
      {
        CORBA::valuetype_reference<CORBA::ValueBase> val;
        if (adapter->stream_to_value (*src, val) &&
            adapter->value_to_stream (*dest, val))
        {
          return TAO::TRAVERSE_CONTINUE;
        }
      }

      return TAO::TRAVERSE_STOP;
    }
  }

#endif /* !CORBA_E_MICRO */

  Unknown_IDL_Type::LOCK const
  Unknown_IDL_Type::lock_i ()
  {
    static LOCK base_lock_ (new ACE_Lock_Adapter<TAO_SYNCH_MUTEX>());
    return base_lock_;
  }

  Unknown_IDL_Type::Unknown_IDL_Type (CORBA::typecode_reference tc,
                                      TAO_InputCDR &cdr)
    : Any_Impl (tc, true)
    , lock_ (lock_i ())
    , cdr_ (static_cast<ACE_Message_Block*>(nullptr), lock_.get ())
  {
    try
      {
        this->_tao_decode (cdr);
      }
    catch (CORBA::Exception const &)
      {
        if (TAO_debug_level > 0)
          {
            TAOX11_LOG_DEBUG ("TAOX11 (%P|%t) %N:%l "
              << "silent marshaling exception "
              << "in Unknown_IDL_Type::Unknown_IDL_Type");
          }
      }
  }

  Unknown_IDL_Type::Unknown_IDL_Type (CORBA::typecode_reference tc)
    : Any_Impl (tc, true)
    , lock_ (lock_i ())
    , cdr_ (static_cast<ACE_Message_Block*>(nullptr), lock_.get ())
  {
  }

  Unknown_IDL_Type::~Unknown_IDL_Type ()
  {
  }

  bool
  Unknown_IDL_Type::marshal_value (TAO_OutputCDR & cdr)
  {
    try
      {
        // We don't want the rd_ptr to move, in case we are shared by
        // another Any, so we use this to copy the state, not the buffer.
        TAO_InputCDR for_reading (this->cdr_);

        TAO::traverse_status const status =
#if !defined(CORBA_E_MICRO)
          this->type_->kind () == CORBA::TCKind::tk_abstract_interface ?
              Marshal_Abstract::append (TC_helper::get_tao_tc (this->type_),
                                        &for_reading,
                                        &cdr) :
#endif
              TAO_Marshal_Object::perform_append (TC_helper::get_tao_tc(this->type_),
                                                  &for_reading,
                                                  &cdr);

        if (status != TAO::TRAVERSE_CONTINUE)
          {
            return false;
          }
      }
    catch (TAO_CORBA::Exception const &)
      {
        return false;
      }
    catch (CORBA::Exception const &)
      {
        return false;
      }

    return true;
  }

  TAO_InputCDR &
  Unknown_IDL_Type::_tao_get_cdr ()
  {
    return this->cdr_;
  }

  int
  Unknown_IDL_Type::_tao_byte_order () const
  {
    return this->cdr_.byte_order ();
  }
  void
  Unknown_IDL_Type::_tao_decode (TAO_InputCDR & cdr)
  {
    try {
      // @@ (JP) The following code depends on the fact that
      //         TAO_InputCDR does not contain chained message blocks,
      //         otherwise <begin> and <end> could be part of
      //         different buffers!

      // This will be the start of a new message block.
      char const * const begin = cdr.rd_ptr ();

      // Skip over the next argument.
      TAO::traverse_status const status =
#if !defined(CORBA_E_MICRO)
        this->type_->kind () == CORBA::TCKind::tk_abstract_interface ?
            Marshal_Abstract::skip (TC_helper::get_tao_tc (this->type_), &cdr) :
#endif
            TAO_Marshal_Object::perform_skip (TC_helper::get_tao_tc (this->type_), &cdr);

      if (status != TAO::TRAVERSE_CONTINUE)
        {
          throw CORBA::MARSHAL ();
        }

      // This will be the end of the new message block.
      char const * const end = cdr.rd_ptr ();

      // The ACE_CDR::mb_align() call can shift the rd_ptr by up to
      // ACE_CDR::MAX_ALIGNMENT - 1 bytes. Similarly, the offset adjustment
      // can move the rd_ptr by up to the same amount. We accommodate
      // this by including 2 * ACE_CDR::MAX_ALIGNMENT bytes of additional
      // space in the message block.
      size_t const size = end - begin;

      ACE_Message_Block new_mb (size + 2 * ACE_CDR::MAX_ALIGNMENT);

      ACE_CDR::mb_align (std::addressof(new_mb));
      ptrdiff_t offset = ptrdiff_t (begin) % ACE_CDR::MAX_ALIGNMENT;

      if (offset < 0)
        {
          offset += ACE_CDR::MAX_ALIGNMENT;
        }

      new_mb.rd_ptr (offset);
      new_mb.wr_ptr (offset + size);

      ACE_OS::memcpy (new_mb.rd_ptr (), begin, size);

      this->cdr_.reset (&new_mb, cdr.byte_order ());
      this->cdr_.char_translator (cdr.char_translator ());
      this->cdr_.wchar_translator (cdr.wchar_translator ());

      this->cdr_.set_repo_id_map (cdr.get_repo_id_map ());
      this->cdr_.set_codebase_url_map (cdr.get_codebase_url_map ());
      this->cdr_.set_value_map (cdr.get_value_map ());

      // Take over the GIOP version, the input cdr can have a different
      // version then our current GIOP version.
      ACE_CDR::Octet major_version;
      ACE_CDR::Octet minor_version;
      cdr.get_version (major_version, minor_version);
      this->cdr_.set_version (major_version, minor_version);
    }
    catch_tao_system_ex(_ex)
  }

  bool
  Unknown_IDL_Type::to_object (IDL::traits<CORBA::Object>::ref_type & obj) const
  {
    try
      {
        CORBA::TCKind const kind = TC_helper::unaliased_kind (this->type_);

        if (kind != CORBA::TCKind::tk_objref)
          {
            return false;
          }

        // We don't want the rd_ptr to move, in case we are shared by
        // another Any, so we use this to copy the state, not the buffer.
        TAO_InputCDR for_reading (this->cdr_);

        return for_reading >> obj;
      }
    catch (TAO_CORBA::Exception const &)
      {
      }
    catch (CORBA::Exception const &)
      {
      }

    return false;
  }

  bool
  Unknown_IDL_Type::to_value (CORBA::valuetype_reference<CORBA::ValueBase> & val) const
  {
#if !defined(CORBA_E_MICRO)
    try
      {
        CORBA::TCKind const kind = TC_helper::unaliased_kind (this->type_);

        if (kind != CORBA::TCKind::tk_value)
          {
            return false;
          }

        // We don't want the rd_ptr to move, in case we are shared by
        // another Any, so we use this to copy the state, not the buffer.
        TAO_InputCDR for_reading (this->cdr_);

        ValueTypeAdapter * const adapter = ValueTypeAdapter::value_type_adapter ();

        return adapter->stream_to_value (for_reading, val);
      }
    catch (TAO_CORBA::Exception const &)
      {
      }
    catch (CORBA::Exception const &)
      {
      }
#else
    X11_UNUSED_ARG (val);
#endif

    return false;
  }

  bool
  Unknown_IDL_Type::to_abstract_base (CORBA::abstractbase_reference<CORBA::AbstractBase> & obj) const
  {
#if !defined(CORBA_E_MICRO)
    try
      {
        CORBA::TCKind const kind = TC_helper::unaliased_kind (this->type_);

        if (kind != CORBA::TCKind::tk_abstract_interface)
          {
            return false;
          }

        // We don't want the rd_ptr to move, in case we are shared by
        // another Any, so we use this to copy the state, not the buffer.
        TAO_InputCDR for_reading (this->cdr_);

        ValueTypeAdapter * const adapter = ValueTypeAdapter::value_type_adapter ();

        return adapter->stream_to_abstract_base (for_reading, obj);
      }
    catch (TAO_CORBA::Exception const &)
      {
      }
    catch (CORBA::Exception const &)
      {
      }
#else
    X11_UNUSED_ARG (obj);
#endif
    return false;
  }

} // namespace TAOX11_NAMESPACE

