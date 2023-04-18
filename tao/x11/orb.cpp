/**
 * @file    orb.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 core ORB class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
// TAO namespace dependencies
#include "tao/x11/objproxy.h"
#include "tao/x11/orbproxy.h"
#include "tao/ServicesC.h"

// X11 namespace dependencies
#include "tao/x11/orb.h"
#include "tao/x11/orb_constants.h"
#include "tao/x11/orb_registry.h"
#include "tao/x11/exception_macros.h"
#include "tao/x11/PolicyC.h"
#if !defined(CORBA_E_MICRO)
# include "tao/x11/valuefactory_manager_adapter.h"
# include "tao/x11/policyfactory_manager_adapter.h"
#endif
#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
# include "tao/x11/ifr_client_adapter.h"
# include "tao/x11/dynamic_adapter.h"
# if (TAO_HAS_MINIMUM_CORBA == 0)
#   include "tao/x11/nvlist_adapter.h"
#   include "tao/x11/typecode_factory_adapter.h"
# endif
#endif
#include "tao/x11/log.h"
#include "tao/x11/versionx11.h"

#if !defined (TAOX11_RELEASE_BUILDSTAMP)
# define TAOX11_RELEASE_BUILDSTAMP \
    __DATE__ " " __TIME__
#endif

#include "ace/OS_NS_sys_utsname.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// @name CDR streaming operator specializations for TAOX11_CORBA::ORB::InvalidName
//@{
TAO_CORBA::Boolean operator<< (TAO_OutputCDR &strm, const TAOX11_CORBA::ORB::InvalidName& ex)
{
  return (strm << ex._rep_id ());
}
TAO_CORBA::Boolean operator>> (TAO_InputCDR &, TAOX11_CORBA::ORB::InvalidName&)
{
  return true;
}
//@}

TAO_END_VERSIONED_NAMESPACE_DECL

namespace TAOX11_NAMESPACE
{
  /*  ORB traits method specialization
   *
   */
  namespace CORBA
  {
    void
    ORB::InvalidName::_info (std::ostream& user_exception_info) const
    {
      TAOX11_IDL::traits<ORB::InvalidName>::write_on(user_exception_info, *this);
    }

    void ORB::InvalidName::_raise () const
    {
      throw *this;
    }

    void ORB::InvalidName::_tao_encode (TAO_OutputCDR &cdr) const
    {
      if (!(cdr << *this))
      {
        throw TAO_CORBA::MARSHAL ();
      }
    }

    void ORB::InvalidName::_tao_decode (TAO_InputCDR &cdr)
    {
      if (!(cdr >> *this))
      {
        throw TAO_CORBA::MARSHAL ();
      }
    }

    TAOX11_NAMESPACE::CORBA::Exception *ORB::InvalidName::_tao_duplicate () const
    {
      TAOX11_NAMESPACE::CORBA::Exception * result = 0;
      ACE_NEW_NORETURN (result, InvalidName (*this));
      if (!result)
        throw TAO_CORBA::NO_MEMORY ();
      return result;
    }

    template<>
    TAOX11_Export object_traits<CORBA::ORB>::shared_ptr_type
    object_traits<CORBA::ORB>::lock_shared (
        CORBA::ORB* p)
    {
      return std::dynamic_pointer_cast<CORBA::ORB> (p->_lock_shared ());
    }

    ORB::ORB (ORB_Proxy_ptr op)
      : Object (),
        proxy_ (op),
        orb_registry_ ()
    {
      this->orb_registry_ = ORB_Registry::instance (true);
    }

    ORB::~ORB ()
    {
    }

    ORB_Proxy& ORB::proxy ()
    {
      return *this->proxy_;
    }

    std::string
    ORB::id ()
    {
      try {
         TAO_CORBA::String_var ORBid = this->proxy ()->id ();
         return ORBid.in ();
       }
       catch_tao_system_ex (_sx)
       return std::string ();
    }

    object_traits<Object>::ref_type
    ORB::string_to_object (const std::string& str)
    {
      try {
        TAO_CORBA::Object_var tmp = this->proxy ()->string_to_object(str.c_str ());
        if (!TAO_CORBA::is_nil (tmp))
          {
            return Object::_narrow (Object_proxy (TAO_CORBA::Object::_duplicate (tmp)));
         }
      }
      catch_tao_system_ex (_sx)
      return object_traits<Object>::ref_type ();
    }

    std::string
    ORB::object_to_string (object_traits<Object>::ref_type obj)
    {
      // When we are passed a null reference or a reference without a proxy we need
      // to throw a CORBA::MARSHAL error with minor code 4
      if (!obj || !obj->get_proxy ())
        {
          throw TAOX11_CORBA::MARSHAL (TAOX11_CORBA::OMGVMCID | 4, TAOX11_CORBA::COMPLETED_NO);
        }

      try {
        TAO_CORBA::String_var ior = this->proxy ()->object_to_string (obj->get_proxy ()->get ());
        return ior.in ();
      }
      catch_tao_system_ex (_sx)
      return std::string ();
    }
//
#if (TAO_HAS_MINIMUM_CORBA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

    void
    ORB::create_list (uint32_t count,
                      CORBA::object_reference<CORBA::NVList> &new_list)
    {
      std::shared_ptr<ORB_Registry> orbreg = ORB_Registry::instance ();
      if (orbreg)
      {
        NVListAdapter *adapter = orbreg->nvlist_adapter ();
        if (adapter)
        {
          adapter->create_list (count, new_list);
        }
      }
    }

    void
    ORB::create_operation_list(CORBA::object_reference<CORBA::OperationDef> opdef,
                               CORBA::object_reference<CORBA::NVList> &result)
    {
      IFRClientAdapter *adapter = ORB::ifr_client_adapter ();
      if (adapter)
      {
        adapter->create_operation_list (this->proxy (), opdef, result);
      }
    }
    // The following functions are not implemented in TAO - they just throw
    // CORBA::NO_IMPLEMENT.
    void
    ORB::get_default_context (object_reference<Context>& /*ctx*/)
    {
      throw CORBA::NO_IMPLEMENT (
                   TAO_CORBA::SystemException::_tao_minor_code (
                     0,
                     ENOTSUP),
                   CORBA::COMPLETED_NO);
    }

    void
    ORB::send_multiple_requests_oneway (const CORBA::RequestSeq & )
    {
      throw CORBA::NO_IMPLEMENT (
                   TAO_CORBA::SystemException::_tao_minor_code (
                     0,
                     ENOTSUP),
                   CORBA::COMPLETED_NO);
    }

    void
    ORB::send_multiple_requests_deferred (const CORBA::RequestSeq & )
    {
      throw CORBA::NO_IMPLEMENT (
                   TAO_CORBA::SystemException::_tao_minor_code (
                     0,
                     ENOTSUP),
                   CORBA::COMPLETED_NO);
    }

    void
    ORB::get_next_response (object_reference<CORBA::Request> & )
    {
      throw CORBA::NO_IMPLEMENT (
                   TAO_CORBA::SystemException::_tao_minor_code (
                     0,
                     ENOTSUP),
                   CORBA::COMPLETED_NO);
    }

    bool
    ORB::poll_next_response ()
    {
      throw CORBA::NO_IMPLEMENT (
                   TAO_CORBA::SystemException::_tao_minor_code (
                     0,
                     ENOTSUP),
                   CORBA::COMPLETED_NO);
    }

    bool
    ORB::get_service_information (
       CORBA::ServiceType,
       CORBA::ServiceInformation&)
    {
      throw CORBA::NO_IMPLEMENT (
                   TAO_CORBA::SystemException::_tao_minor_code (
                     0,
                     ENOTSUP),
                   CORBA::COMPLETED_NO);
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_struct_tc (
        const std::string &id,
        const std::string &name,
        const CORBA::StructMemberSeq &members)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_struct_tc(id, name, members);
      }
      return nullptr;
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_union_tc (
        const std::string &id,
        const std::string &name,
        CORBA::object_reference<CORBA::TypeCode> discriminator_type,
        const CORBA::UnionMemberSeq &members)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_union_tc(id, name, discriminator_type, members);
      }
      return nullptr;
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_enum_tc (
        const std::string &id,
        const std::string &name,
        const CORBA::EnumMemberSeq &members)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_enum_tc(id, name, members);
      }
      return nullptr;
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_alias_tc (
        const std::string &id,
        const std::string &name,
        CORBA::object_reference<CORBA::TypeCode> original_type)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_alias_tc(id, name, original_type);
      }
      return nullptr;
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_exception_tc (
        const std::string &id,
        const std::string &name,
        const CORBA::StructMemberSeq &members)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_exception_tc(id, name, members);
      }
      return nullptr;
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_interface_tc (
        const std::string &id,
        const std::string &name)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_interface_tc(id, name);
      }
      return nullptr;
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_string_tc (
        uint32_t bound)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_string_tc(bound);
      }
      return nullptr;
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_wstring_tc (
        uint32_t bound)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_wstring_tc(bound);
      }
      return nullptr;
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_fixed_tc (
        uint16_t digits,
        uint16_t scale)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_fixed_tc(digits, scale);
      }
      return nullptr;
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_sequence_tc (
        uint32_t bound,
        CORBA::object_reference<CORBA::TypeCode> element_type)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_sequence_tc(bound, element_type);
      }
      return nullptr;
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_array_tc (
        uint32_t length,
        CORBA::object_reference<CORBA::TypeCode> element_type)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_array_tc(length, element_type);
      }
      return nullptr;
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_value_tc (
        const std::string &id,
        const std::string &name,
        CORBA::ValueModifier type_modifier,
        CORBA::object_reference<CORBA::TypeCode> concrete_base,
        const CORBA::ValueMemberSeq &members)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_value_tc(id, name, type_modifier, concrete_base, members);
      }
      return nullptr;
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_value_box_tc (
        const std::string &id,
        const std::string &name,
        CORBA::object_reference<CORBA::TypeCode> boxed_type)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_value_box_tc(id, name, boxed_type);
      }
      return nullptr;
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_native_tc (
        const std::string &id,
        const std::string &name)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_native_tc(id, name);
      }
      return nullptr;
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_recursive_tc (
        const std::string &id)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_recursive_tc(id);
      }
      return nullptr;
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_abstract_interface_tc (
        const std::string &id,
        const std::string &name)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_abstract_interface_tc(id, name);
      }
      return nullptr;
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_local_interface_tc (
        const std::string &id,
        const std::string &name)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_local_interface_tc(id, name);
      }
      return nullptr;
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_component_tc (
        const std::string &id,
        const std::string &name)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_component_tc(id, name);
      }
      return nullptr;
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_home_tc (
        const std::string &id,
        const std::string &name)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_home_tc(id, name);
      }
      return nullptr;
    }

    CORBA::object_reference<CORBA::TypeCode>
    ORB::create_event_tc (
        const std::string &id,
        const std::string &name,
        CORBA::ValueModifier type_modifier,
        CORBA::object_reference<CORBA::TypeCode> concrete_base,
        const CORBA::ValueMemberSeq &members)
    {
      TypecodeFactoryAdapter *adapter = this->typecode_factory_adapter ();
      if (adapter)
      {
        return adapter->create_event_tc(id, name, type_modifier, concrete_base, members);
      }
      return nullptr;
    }

 #endif

    object_traits<Object>::ref_type
    ORB::resolve_initial_references (const ObjectId& name)
    {
      if (this->orb_registry_)
      {
        return this->orb_registry_->resolve_initial_reference (
            IDL::traits<ORB>::narrow (this->_lock()), name);
      }
      return {};
    }

    ORB::ObjectIdList
    ORB::list_initial_services ()
    {
      if (this->orb_registry_)
      {
        return this->orb_registry_->list_initial_references (
            IDL::traits<ORB>::narrow (this->_lock()));
      }
      return {};
    }

    bool
    ORB::work_pending ()
    {
      try {
        return this->proxy ()->work_pending ();
      }
      catch_tao_system_ex (_sx)
      return false;
    }

    void
    ORB::perform_work (ACE_Time_Value &tv)
    {
      try {
        this->proxy ()->perform_work (tv);
      }
      catch_tao_system_ex (_sx)
    }

    void
    ORB::perform_work ()
    {
      try {
        this->proxy ()->perform_work ();
      }
      catch_tao_system_ex (_sx)
    }

    void
    ORB::run ()
    {
      try {
        this->proxy ()->run ();
      }
      catch_tao_system_ex (_sx)
    }

    void
    ORB::run (ACE_Time_Value &tv)
    {
      try {
        this->proxy ()->run (tv);
      }
      catch_tao_system_ex (_sx)
    }

    void
    ORB::destroy ()
    {
      this->orb_registry_ = nullptr;
      try {
        this->proxy ()->destroy ();
      }
      catch_tao_system_ex (_sx)
    }

    void
    ORB::shutdown (bool wait)
    {
      this->orb_registry_ = nullptr;
      try {
        this->proxy ()->shutdown (wait);
      }
      catch_tao_system_ex (_sx)
    }

    ACE_Reactor*
    ORB::reactor ()
    {
      ACE_Reactor* reactor_ = nullptr;
      try {
        reactor_ = this->proxy ()->orb_core ()->reactor ();
      }
      catch_tao_system_ex (_sx)
      return reactor_;
    }

#if !defined(CORBA_E_MICRO)
    PolicyFactoryManagerAdapter*
    ORB::policy_manager ()
    {
      if (this->orb_registry_)
      {
        return this->orb_registry_->policy_manager ();
      }
      return nullptr;
    }


    IDL::traits<Policy>::ref_type
    ORB::create_policy (const PolicyType policy_type,
                        const Any& val)
    {
      TAOX11_LOG_TRACE ("ORB::create_policy");

      try {
        return this->policy_manager ()->create_policy (
          this->proxy(),
          policy_type,
          val);
      }
      catch (const TAO_CORBA::PolicyError &ex_) {
        throw PolicyError (ex_.reason);
      }
      catch_tao_system_ex (_sx)
      return nullptr;
    }

    // Value factory operations
    CORBA::ValueFactory
    ORB::register_value_factory (
              const std::string &repository_id,
              CORBA::ValueFactory factory)
    {
      try {
        return this->valuefactory_manager ()->register_value_factory (
            this->proxy (),
            repository_id,
            factory);
      }
      catch_tao_system_ex (_sx)
      return nullptr;
    }

    CORBA::ValueFactory
    ORB::lookup_value_factory (
        const std::string &repository_id)
    {
      try {
        return this->valuefactory_manager ()->lookup_value_factory (
            this->proxy (),
            repository_id);
      }
      catch_tao_system_ex (_sx)
      return nullptr;
    }

    void
    ORB::unregister_value_factory (
        const std::string& repository_id)
    {
      try {
        this->valuefactory_manager ()->unregister_value_factory (
            this->proxy (),
            repository_id);
      }
      catch_tao_system_ex (_sx)
    }

    ValueFactoryManagerAdapter*
    ORB::valuefactory_manager ()
    {
      if (this->orb_registry_)
      {
        return this->orb_registry_->valuefactory_manager ();
      }
      return nullptr;
    }
#endif

#if (TAO_HAS_MINIMUM_CORBA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
    TypecodeFactoryAdapter*
    ORB::typecode_factory_adapter ()
    {
      if (this->orb_registry_)
      {
        return this->orb_registry_->typecode_factory_adapter ();
      }
      return nullptr;
    }
#endif

#if !defined(CORBA_E_MICRO)
    /// Register an object reference with the ORB.
    void
    ORB::register_initial_reference (const std::string& id,
                                     object_traits<Object>::ref_type obj)
    {
      if (!obj)
        {
          throw TAOX11_CORBA::BAD_PARAM (TAOX11_CORBA::OMGVMCID | 27, TAOX11_CORBA::COMPLETED_NO);
        }

      // register native X11 (local object) services in the native X11 registry
      if (obj->_is_local ())
      {
        this->orb_registry_->register_user_service (id, obj);
      }
      else
      {
        // in case the service uses a remote reference register with TAO ORB so the TAO environment
        // could use it too
        try {
          this->proxy ()->register_initial_reference (id.c_str(), obj->get_proxy ()->get ());
        }
        catch (const TAO_CORBA::ORB::InvalidName&) {
          throw ORB::InvalidName ();
        }
        catch_tao_system_ex (_sx)
      }
    }

    /// Unregister an object reference with the ORB (TAO/TAOX11 specific).
    object_traits<Object>::ref_type
    ORB::unregister_initial_reference (
        const std::string& id)
    {
      object_traits<Object>::ref_type result =
          this->orb_registry_->resolve_user_service (id);
      if (result)
      {
        this->orb_registry_->unregister_user_service (id);
      }
      else
      {
        try {
          TAO_CORBA::Object_var tmp = this->proxy ()->unregister_initial_reference (id.c_str());
          if (!TAO_CORBA::is_nil (tmp))
            {
              return Object::_narrow (Object_proxy (TAO_CORBA::Object::_duplicate (tmp)));
            }
          throw ORB::InvalidName ();
        }
        catch (const TAO_CORBA::ORB::InvalidName&) {
          throw ORB::InvalidName ();
        }
        catch_tao_system_ex (_sx)
      }
      return result;
    }
#endif

    AnyInsertAdapter *ORB::any_insert_adapter ()
    {
      std::shared_ptr<ORB_Registry> orbreg = ORB_Registry::instance ();
      if (orbreg)
      {
        return orbreg->any_insert_adapter ();
      }
      return nullptr;
    }

#if !defined(CORBA_E_MICRO)
    ValueTypeAdapter* ORB::value_type_adapter ()
    {
      std::shared_ptr<ORB_Registry> orbreg = ORB_Registry::instance ();
      if (orbreg)
      {
        return orbreg->value_type_adapter ();
      }
      return nullptr;
    }
#endif

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
      IFRClientAdapter* ORB::ifr_client_adapter ()
      {
        std::shared_ptr<ORB_Registry> orbreg = ORB_Registry::instance ();
        if (orbreg)
        {
          return orbreg->ifr_client_adapter ();
        }
        return nullptr;
      }

      DynamicAdapter* ORB::dynamic_adapter ()
      {
        std::shared_ptr<ORB_Registry> orbreg = ORB_Registry::instance ();
        if (orbreg)
        {
          return orbreg->dynamic_adapter ();
        }
        return nullptr;
      }
#endif

    object_traits<ORB>::ref_type ORB::init (int & argc, char* argv[], const std::string& orb_name)
    {
      TAOX11_LOG_INFO ("TAOX11 " << (uint32_t)TAOX11_MAJOR_VERSION << '.'
                                 << (uint32_t)TAOX11_MINOR_VERSION << '.'
                                 << (uint32_t)TAOX11_MICRO_VERSION
                                 << " (c) Remedy IT");
      ACE_utsname uname;
      int const result = ACE_OS::uname (std::addressof(uname));
      if (result != -1)
      {
        TAOX11_LOG_INFO ("TAOX11 machine: " << uname.nodename << ", " << uname.machine);
        TAOX11_LOG_INFO ("TAOX11 platform: " << uname.sysname << ", " <<  uname.release << ", " << uname.version);
      }
      TAOX11_LOG_INFO ("TAOX11 compiler: " << ACE::compiler_name() << " version "
                                           << ACE::compiler_major_version() << "." << ACE::compiler_minor_version () << "." << ACE::compiler_beta_version ());
      TAOX11_LOG_INFO ("TAOX11 buildstamp: " << TAOX11_RELEASE_BUILDSTAMP);

      try {
        TAO_CORBA::ORB_var _orb;

        _orb = TAO_CORBA::ORB_init (argc, argv, orb_name.empty () ? 0 : orb_name.c_str ());

        if (!TAO_CORBA::is_nil (_orb.in ()))
        {
          ORB_Proxy_ptr opp = new ORB_Proxy (_orb._retn ());
          return make_reference<ORB> (opp);
        }
      }
      catch_tao_system_ex (_sx)
      return {};
    }

    object_traits<ORB>::ref_type ORB_init (int &argc, char * argv[], const std::string& orb_name)
    {
      return ORB::init (argc, argv, orb_name);
    }
  } // namespace CORBA
} // namespace TAOX11_NAMESPACE

