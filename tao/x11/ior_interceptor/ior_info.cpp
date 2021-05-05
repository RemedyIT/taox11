/**
 * @file    ior_info.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 PI IORInfo implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

// TAO namespace dependencies
#include "tao/x11/tao_corba.h"
#include "tao/ObjRefTemplate/Default_ORTC.h"
#include "tao/ObjRefTemplate/ObjectReferenceTemplate.h"
#include "tao/x11/objproxy.h"
#include "tao/x11/ior_interceptor/tao_ior_interceptor.h"

// X11 namespace dependencies
#include "tao/x11/ort/object_reference_template.h"
#include "tao/x11/ort/object_reference_template_impl.h"
#include "tao/x11/ior_interceptor/ior_info.h"

#include "tao/x11/exception_macros.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    IORInfo::IORInfo (
        TAO_PI::IORInfo_ptr iorinf)
      : iorinf_ (iorinf)
    {
      this->set_proxy (new Object_proxy (iorinf));
    }

    IDL::traits<CORBA::Policy>::ref_type
    IORInfo::get_effective_policy (
        CORBA::PolicyType type)
    {
      IDL::traits<CORBA::Policy>::ref_type rc {};
      try
      {
        TAO_CORBA::Policy_var tao_pol = this->iorinf_->get_effective_policy (type);
        rc = CORBA::make_reference<TAOX11_NAMESPACE::CORBA::Policy> (
              new Object_proxy (TAO_CORBA::Policy::_duplicate(tao_pol.in())));
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    /**
     * Conversion helper for IOP::TaggedComponent
     */
    namespace {
      const TAO_IOP::TaggedComponent& tagged_component_from_x11 (
          const IOP::TaggedComponent& x11_tc,
          TAO_IOP::TaggedComponent& tao_tc)
      {
        tao_tc.tag = x11_tc.tag ();
        seq_from_x11 (tao_tc.component_data, x11_tc.component_data ());
        return tao_tc;
      }
    }

    void
    IORInfo::add_ior_component (
        const IOP::TaggedComponent& a_component)
    {
      try
      {
        TAO_IOP::TaggedComponent tao_tc;
        this->iorinf_->add_ior_component (
            tagged_component_from_x11 (a_component, tao_tc));
      }
      catch_tao_system_ex (_sx)
    }

    void
    IORInfo::add_ior_component_to_profile (
        const IOP::TaggedComponent& a_component,
        IOP::ProfileId profile_id)
    {
      try
      {
        TAO_IOP::TaggedComponent tao_tc;
        this->iorinf_->add_ior_component_to_profile (
            tagged_component_from_x11 (a_component, tao_tc),
            profile_id);
      }
      catch_tao_system_ex (_sx)
    }

    PortableInterceptor::AdapterManagerId
    IORInfo::manager_id ()
    {
      PortableInterceptor::AdapterManagerId rc {};
      try
      {
        TAO_PI::AdapterManagerId_var tao_id = this->iorinf_->manager_id ();
        if (!TAO_CORBA::is_nil (tao_id.in ()))
          rc = tao_id.in ();
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    PortableInterceptor::AdapterState
    IORInfo::state ()
    {
      PortableInterceptor::AdapterState rc {};
      try
      {
        rc = this->iorinf_->state ();
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    IDL::traits<PortableInterceptor::ObjectReferenceTemplate>::ref_type
    IORInfo::adapter_template ()
    {
      IDL::traits<PortableInterceptor::ObjectReferenceTemplate>::ref_type rc {};
      try
      {
        rc = CORBA::make_reference<PortableInterceptor_impl::ObjectReferenceTemplate_impl> (
            this->iorinf_->adapter_template ());
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    IDL::traits<PortableInterceptor::ObjectReferenceFactory>::ref_type
    IORInfo::current_factory ()
    {
      IDL::traits<PortableInterceptor::ObjectReferenceFactory>::ref_type rc {};
      try
      {
        rc = CORBA::make_reference<PortableInterceptor_impl::ObjectReferenceFactory_impl> (
            this->iorinf_->current_factory ());
      }
      catch_tao_system_ex (_sx)
      return rc;
    }

    /**
     * TAO proxy for X11 ObjectReferenceFactory
     */
    namespace
    {
      /**
       * @class ObjectReferenceTemplate
       *
       * @brief Implementation of the PortableInterceptor::ObjectReferenceTemplate
       *        interface. This is a default implementation created to be returned
       *        by the IORInfo when the user requests the ORT or ORF.
       */
      class ObjectReferenceFactory_proxy final
        : public virtual OBV_TAO_Default_ORT::ObjectReferenceTemplate,
          public virtual TAO_CORBA::DefaultValueRefCountBase
      {
      public:
        /// Constructor
        explicit ObjectReferenceFactory_proxy (
            IDL::traits<PortableInterceptor::ObjectReferenceFactory>::ref_type x11_orf)
          : x11_orf_ (std::move (x11_orf))
        {}
        TAO_CORBA::ValueBase *_copy_value () override
        {
          TAO_CORBA::ValueBase *ret_val {};
          ACE_NEW_THROW_EX (
            ret_val,
            ObjectReferenceFactory_proxy (
              this->x11_orf_),
            TAO_CORBA::NO_MEMORY ()
          );
          return ret_val;
        }

        /**
         * NOT USED
         */
        char * server_id () override { return nullptr; }

        char * orb_id () override { return nullptr; }

        TAO_PI::AdapterName * adapter_name () override { return nullptr; }

        /**
         * Methods required by the
         * PortableInterceptor::ObjectReferenceFactory ValueType.
         */
        TAO_CORBA::Object_ptr make_object (
          const char * repository_id,
          const TAO_PI::ObjectId & id) override
        {
          TAO_CORBA::Object_var rc;
          try
          {
            IDL::traits<CORBA::Object>::ref_type x11_obj =
                this->x11_orf_->make_object (
                    (repository_id ? repository_id : std::string ()),
                    seq_to_x11<PortableInterceptor::ObjectId> (id));
            rc = TAO_CORBA::Object::_duplicate (x11_obj->get_proxy ()->get ());
          }
          catch (const CORBA::SystemException& sx)
          {
            sx._raise_tao ();
          }
          return rc._retn ();
        }

      protected:
        ~ObjectReferenceFactory_proxy () override = default;

      private:
        IDL::traits<PortableInterceptor::ObjectReferenceFactory>::ref_type x11_orf_;

        ObjectReferenceFactory_proxy () = delete;
        ObjectReferenceFactory_proxy (const ObjectReferenceFactory_proxy&) = delete;
        ObjectReferenceFactory_proxy (ObjectReferenceFactory_proxy&&) = delete;
        ObjectReferenceFactory_proxy& operator =(const ObjectReferenceFactory_proxy&) = delete;
        ObjectReferenceFactory_proxy& operator =(ObjectReferenceFactory_proxy&&) = delete;
      };
    }

    void
    IORInfo::current_factory (
        IDL::traits<PortableInterceptor::ObjectReferenceFactory>::ref_type x11_orf)
    {
      try
      {
        ObjectReferenceFactory_proxy *tao_orf = nullptr;
        ACE_NEW_THROW_EX (
          tao_orf,
          ObjectReferenceFactory_proxy (
            x11_orf),
          CORBA::NO_MEMORY ()
        );
        this->iorinf_->current_factory (tao_orf);
      }
      catch_tao_system_ex (_sx)
    }

  } // namespace PortableInterceptor_impl

  namespace CORBA
  {
    // IORInfo
    template<>
    object_traits<PortableInterceptor::IORInfo>::ref_type
    object_traits<PortableInterceptor::IORInfo>::narrow (
       object_traits<CORBA::Object>::ref_type objref)
    {
      if (objref)
      {
        if (objref->_is_local ())
        {
          return ref_type::_narrow (std::move(objref));
        }

        try
        {
          TAO_PI::IORInfo_ptr const riptr =
              TAO_PI::IORInfo::_narrow (objref->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (riptr))
          {
            return CORBA::make_reference<PortableInterceptor_impl::IORInfo> (riptr);
          }
        }
        catch_tao_system_ex(_ex)
      }

      return {};
    }
  } // namespace CORBA
} // namespace TAOX11_NAMESPACE
