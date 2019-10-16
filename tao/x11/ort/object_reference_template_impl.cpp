/**
 * @file    object_reference_template_impl.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 default ORT implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
// TAO dependencies
#include "tao/x11/objproxy.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/x11/portable_server/portableserver_proxies.h"

// TAOX11 dependencies
#include "tao/x11/ort/object_reference_template_impl.h"
#include "tao/x11/portable_server/portableserver_impl.h"

#include "tao/x11/exception_macros.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    ObjectReferenceFactory_impl::ObjectReferenceFactory_impl(
        TAO_PI::ObjectReferenceFactory *tao_orf)
      : tao_orf_ (tao_orf)
    {
    }

    IDL::traits<CORBA::Object>::ref_type
    ObjectReferenceFactory_impl::make_object (
        const std::string& repository_id,
        const PortableInterceptor::ObjectId& id)
    {
      IDL::traits<CORBA::Object>::ref_type rc;
      try
      {
        TAO_CORBA::OctetSeq tao_objid;
        seq_from_x11 (tao_objid, id);
        TAO_CORBA::Object_var tao_obj =
            this->tao_orf_->make_object (
                repository_id.c_str (),
                tao_objid);
        rc = CORBA::make_reference<CORBA::Object> (
            new Object_proxy (tao_obj._retn ()));
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    ObjectReferenceTemplate_impl::ObjectReferenceTemplate_impl(
        TAO_PI::ObjectReferenceTemplate *tao_ort)
      : tao_ort_ (tao_ort)
    {
    }

    PortableInterceptor::ServerId
    ObjectReferenceTemplate_impl::server_id ()
    {
      TAO_PI::ServerId_var tao_id = this->tao_ort_->server_id ();
      return TAO_CORBA::is_nil (tao_id.in ()) ? PortableInterceptor::ServerId () : tao_id.in ();
    }

    PortableInterceptor::ORBId
    ObjectReferenceTemplate_impl::orb_id ()
    {
      TAO_PI::ORBId_var tao_id = this->tao_ort_->orb_id ();
      return TAO_CORBA::is_nil (tao_id.in ()) ? PortableInterceptor::ORBId () : tao_id.in ();
    }

    PortableInterceptor::AdapterName
    ObjectReferenceTemplate_impl::adapter_name ()
    {
      TAO_PI::AdapterName_var tao_an = this->tao_ort_->adapter_name ();
      return seq_to_x11<PortableInterceptor::AdapterName> (tao_an.in ());
    }

    IDL::traits<CORBA::Object>::ref_type
    ObjectReferenceTemplate_impl::make_object (
        const std::string& repository_id,
        const PortableInterceptor::ObjectId& id)
    {
      IDL::traits<CORBA::Object>::ref_type rc;
      try
      {
        TAO_CORBA::OctetSeq tao_objid;
        seq_from_x11 (tao_objid, id);
        TAO_CORBA::Object_var tao_obj =
            this->tao_ort_->make_object (
                repository_id.c_str (),
                tao_objid);
        rc = CORBA::make_reference<CORBA::Object> (
            new Object_proxy (tao_obj._retn ()));
      }
      catch_tao_system_ex (_sx)
      return rc;
    }
  }
}
