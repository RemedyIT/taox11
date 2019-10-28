/**
 * @file    object_reference_template_impl.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 default ORT implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_OBJREF_TEMPLATE_IMPL_H_INCLUDED
#define TAOX11_OBJREF_TEMPLATE_IMPL_H_INCLUDED

// TAO dependencies
#include "tao/ObjRefTemplate/ObjectReferenceTemplate.h"

#define TAO_PI \
  TAO_VERSIONED_NAMESPACE_NAME::PortableInterceptor

#include "tao/x11/ort/taox11_ort_export.h"
#include "tao/x11/ort/default_ortC.h"
#include "tao/x11/portable_server/portableserver.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    class TAOX11_ObjRef_Template_Export ObjectReferenceFactory_impl final
      : public IDL::traits<PortableInterceptor_impl::ObjectReferenceFactory>::obv_type
    {
    public:
      explicit ObjectReferenceFactory_impl (TAO_PI::ObjectReferenceFactory *tao_orf);

      /**
       * @name PortableInterceptor::ObjectReferenceFactory Methods
       *
       * Methods required by the
       * PortableInterceptor::ObjectReferenceFactory ValueType.
       */
      //@{
      virtual
      IDL::traits<CORBA::Object>::ref_type
      make_object (
          const std::string& repository_id,
          const PortableInterceptor::ObjectId& id) override;
      //@}

    private:
      TAO_PI::ObjectReferenceFactory_var tao_orf_;

      ObjectReferenceFactory_impl () = delete;
      ObjectReferenceFactory_impl (const ObjectReferenceFactory_impl&) = delete;
      ObjectReferenceFactory_impl (ObjectReferenceFactory_impl&&) = delete;
      ObjectReferenceFactory_impl& operator= (const ObjectReferenceFactory_impl&) = delete;
      ObjectReferenceFactory_impl& operator= (ObjectReferenceFactory_impl&&) = delete;
    };

    class TAOX11_ObjRef_Template_Export ObjectReferenceTemplate_impl final
      : public virtual IDL::traits<PortableInterceptor_impl::ObjectReferenceTemplate>::obv_type
    {
    public:
      explicit ObjectReferenceTemplate_impl (TAO_PI::ObjectReferenceTemplate *tao_ort);

      /**
       * @name PortableInterceptor::ObjectReferenceTemplate Methods
       *
       * Methods required by the
       * PortableInterceptor::ObjectReferenceTemplate ValueType.
       */
      //@{
      virtual
      PortableInterceptor::ServerId
      server_id () override;

      virtual
      PortableInterceptor::ORBId
      orb_id () override;

      virtual
      PortableInterceptor::AdapterName
      adapter_name () override;
      //@}

      /**
       * @name PortableInterceptor::ObjectReferenceFactory Methods
       *
       * Methods required by the
       * PortableInterceptor::ObjectReferenceFactory ValueType.
       */
      //@{
      virtual
      IDL::traits<CORBA::Object>::ref_type
      make_object (
          const std::string& repository_id,
          const PortableInterceptor::ObjectId& id) override;
      //@}

    private:
      TAO_PI::ObjectReferenceTemplate_var tao_ort_;

      ObjectReferenceTemplate_impl () = delete;
      ObjectReferenceTemplate_impl (const ObjectReferenceTemplate_impl&) = delete;
      ObjectReferenceTemplate_impl (ObjectReferenceTemplate_impl&&) = delete;
      ObjectReferenceTemplate_impl& operator= (const ObjectReferenceTemplate_impl&) = delete;
      ObjectReferenceTemplate_impl& operator= (ObjectReferenceTemplate_impl&&) = delete;
    };
  }
}

#endif /* TAOX11_OBJREF_TEMPLATE_IMPL_H_INCLUDED */
