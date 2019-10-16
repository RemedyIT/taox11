/**
 * @file    portableserver_impl.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 POA implementation classes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_PORTABLESERVER_IMPL_H_INCLUDED
#define TAOX11_PORTABLESERVER_IMPL_H_INCLUDED

#include "tao/x11/portable_server/portableserver.h"

namespace TAOX11_NAMESPACE {

  class POA_proxy;
  typedef POA_proxy* POA_proxy_ptr;
  typedef std::unique_ptr<POA_proxy> POA_proxy_ref;

  class POAManager_proxy;
  typedef POAManager_proxy* POAManager_proxy_ptr;
  typedef std::unique_ptr<POAManager_proxy> POAManager_proxy_ref;

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
  class POAManagerFactory_proxy;
  typedef POAManagerFactory_proxy* POAManagerFactory_proxy_ptr;
  typedef std::unique_ptr<POAManagerFactory_proxy> POAManagerFactory_proxy_ref;
#endif

  class POA_Current_proxy;
  typedef POA_Current_proxy* POA_Current_proxy_ptr;
  typedef std::unique_ptr<POA_Current_proxy> POA_Current_proxy_ref;

  class Adapter_Activator_Callback;
  class Servant_Manager_Callback;

  namespace PortableServer
  {
    class POA_Current_impl;

    class TAOX11_PortableServer_Export POA_impl
      : public virtual IDL::traits<PortableServer::POA>::base_type
    {
    public:
      friend class TAOX11_NAMESPACE::PortableServer::Servant;
      friend struct CORBA::object_traits<POA>;
      friend class POA_Current_impl;
      friend class Adapter_Activator_Callback;
      friend class Servant_Manager_Callback;
      friend class Servant_Activator_Callback;
      friend class Servant_Locator_Callback;

      virtual void destroy (bool etherealize_objects,
                            bool wait_for_completion) override;

      virtual PortableServer::ObjectId
      activate_object (CORBA::servant_reference<PortableServer::Servant> servant) override;

      virtual void
      activate_object_with_id (const PortableServer::ObjectId & id,
                               CORBA::servant_reference<PortableServer::Servant> servant) override;

      virtual void
      deactivate_object (const PortableServer::ObjectId & oid) override;

      virtual IDL::traits<CORBA::Object>::ref_type
      id_to_reference (const PortableServer::ObjectId & oid) override;

      virtual IDL::traits<POAManager>::ref_type
      the_POAManager () override;

#if ! defined (CORBA_E_MICRO)
      // generated from c++/cli_hdr/operation.erb
      virtual IDL::traits<POA>::ref_type
      create_POA (const std::string& adapter_name,
          IDL::traits<PortableServer::POAManager>::ref_type a_POAManager,
          const TAOX11_NAMESPACE::CORBA::PolicyList& policies) override;

      // generated from c++/cli_hdr/operation.erb
      virtual IDL::traits<POA>::ref_type
      find_POA (const std::string& adapter_name,
          bool activate_it) override;
#endif

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
      // generated from c++/cli_hdr/operation.erb
      virtual IDL::traits<PortableServer::ThreadPolicy>::ref_type
      create_thread_policy (TAOX11_NAMESPACE::PortableServer::ThreadPolicyValue value) override;
#endif
#if  ! defined (CORBA_E_MICRO)
      // generated from c++/cli_hdr/operation.erb
      virtual IDL::traits<PortableServer::LifespanPolicy>::ref_type
      create_lifespan_policy (TAOX11_NAMESPACE::PortableServer::LifespanPolicyValue value) override;

      // generated from c++/cli_hdr/operation.erb
      virtual IDL::traits<PortableServer::IdUniquenessPolicy>::ref_type
      create_id_uniqueness_policy (TAOX11_NAMESPACE::PortableServer::IdUniquenessPolicyValue value) override;

      // generated from c++/cli_hdr/operation.erb
      virtual IDL::traits<PortableServer::IdAssignmentPolicy>::ref_type
      create_id_assignment_policy (TAOX11_NAMESPACE::PortableServer::IdAssignmentPolicyValue value) override;
#endif
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
      // generated from c++/cli_hdr/operation.erb
      virtual IDL::traits<PortableServer::ImplicitActivationPolicy>::ref_type
      create_implicit_activation_policy (TAOX11_NAMESPACE::PortableServer::ImplicitActivationPolicyValue value) override;

      // generated from c++/cli_hdr/operation.erb
      virtual IDL::traits<PortableServer::ServantRetentionPolicy>::ref_type
      create_servant_retention_policy (TAOX11_NAMESPACE::PortableServer::ServantRetentionPolicyValue value) override;

      // generated from c++/cli_hdr/operation.erb
      virtual IDL::traits<PortableServer::RequestProcessingPolicy>::ref_type
      create_request_processing_policy (TAOX11_NAMESPACE::PortableServer::RequestProcessingPolicyValue value) override;
#endif
      // generated from c++/cli_hdr/attribute.erb
      virtual std::string the_name() override;

      // generated from c++/cli_hdr/attribute.erb
      virtual IDL::traits<POA>::ref_type the_parent() override;
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
      // generated from c++/cli_hdr/attribute.erb
      virtual TAOX11_NAMESPACE::PortableServer::POAList
      the_children() override;
#endif
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
      // generated from c++/cli_hdr/attribute.erb
      virtual IDL::traits<POAManagerFactory>::ref_type
      the_POAManagerFactory() override;

      // generated from c++/cli_hdr/attribute.erb
      virtual IDL::traits<AdapterActivator>::ref_type
      the_activator() override;
      virtual void
      the_activator(IDL::traits<PortableServer::AdapterActivator>::ref_type _v) override;

      // generated from c++/cli_hdr/operation.erb
      virtual IDL::traits<ServantManager>::ref_type
      get_servant_manager () override;

      // generated from c++/cli_hdr/operation.erb
      virtual void
      set_servant_manager (IDL::traits<PortableServer::ServantManager>::ref_type imgr) override;

      // generated from c++/cli_hdr/operation.erb
      virtual CORBA::servant_reference<PortableServer::Servant>
      get_servant () override;

      // generated from c++/cli_hdr/operation.erb
      virtual void
      set_servant (CORBA::servant_reference<PortableServer::Servant> p_servant) override;
#endif
      // generated from c++/cli_hdr/operation.erb
      virtual IDL::traits<CORBA::Object>::ref_type
      create_reference (const TAOX11_NAMESPACE::CORBA::RepositoryId& intf) override;
#if !defined (CORBA_E_MICRO)
      // generated from c++/cli_hdr/operation.erb
      virtual IDL::traits<CORBA::Object>::ref_type
      create_reference_with_id (const TAOX11_NAMESPACE::PortableServer::ObjectId& oid,
                                const TAOX11_NAMESPACE::CORBA::RepositoryId& intf) override;
#endif
      // generated from c++/cli_hdr/operation.erb
      virtual TAOX11_NAMESPACE::PortableServer::ObjectId
      servant_to_id (CORBA::servant_reference<PortableServer::Servant> p_servant) override;

      // generated from c++/cli_hdr/operation.erb
      virtual IDL::traits<CORBA::Object>::ref_type
      servant_to_reference (CORBA::servant_reference<PortableServer::Servant> p_servant) override;

      // generated from c++/cli_hdr/operation.erb
      virtual CORBA::servant_reference<PortableServer::Servant>
      reference_to_servant (IDL::traits<CORBA::Object>::ref_type reference) override;

      // generated from c++/cli_hdr/operation.erb
      virtual TAOX11_NAMESPACE::PortableServer::ObjectId
      reference_to_id (IDL::traits<CORBA::Object>::ref_type reference) override;

      // generated from c++/cli_hdr/operation.erb
      virtual CORBA::servant_reference<PortableServer::Servant>
      id_to_servant (const TAOX11_NAMESPACE::PortableServer::ObjectId& oid) override;

      // generated from c++/cli_hdr/attribute.erb
      virtual TAOX11_NAMESPACE::CORBA::OctetSeq id() override;

      POA_proxy& _proxy ();

    protected:
      virtual ~POA_impl () = default;;

    private:
      template <typename _Tp1, typename, typename ...Args>
      friend CORBA::object_reference<_Tp1> CORBA::make_reference(Args&& ...args);

      explicit POA_impl (POA_proxy_ptr op);

      /** @name Illegal to be called. Deleted explicitly to let the compiler detect any violation */
      //@{
      POA_impl () = delete;
      POA_impl (const POA_impl&) = delete;
      POA_impl (POA_impl&&) = delete;
      POA_impl& operator= (const POA_impl&) = delete;
      POA_impl& operator= (POA_impl&&) = delete;
      //@}

    private:
      POA_proxy_ref proxy_;
    };

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
    class TAOX11_PortableServer_Export POAManagerFactory_impl;
#endif

    class TAOX11_PortableServer_Export POAManager_impl final
      : public virtual IDL::traits<PortableServer::POAManager>::base_type
    {
    public:
      friend class TAOX11_NAMESPACE::PortableServer::POA_impl;
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
      friend class TAOX11_NAMESPACE::PortableServer::POAManagerFactory_impl;
#endif
      friend struct CORBA::object_traits<POAManager>;

      virtual void activate () override;
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
      virtual void hold_requests (bool wait_for_completion) override;
      virtual void discard_requests (bool wait_for_completion) override;
      virtual void deactivate (bool etherealize_objects,
                               bool wait_for_completion) override;
#endif
      virtual State get_state () override;

      virtual std::string get_id () override;

      POAManager_proxy& _proxy ();

    protected:
      virtual ~POAManager_impl () = default;

    private:
      template <typename _Tp1, typename, typename ...Args>
      friend CORBA::object_reference<_Tp1> CORBA::make_reference(Args&& ...args);

      explicit POAManager_impl (POAManager_proxy_ptr op);

      /** @name Illegal to be called. Deleted explicitly to let the compiler detect any violation */
      //@{
      POAManager_impl () = delete;
      POAManager_impl (const POAManager_impl&) = delete;
      POAManager_impl (POAManager_impl&&) = delete;
      POAManager_impl& operator= (const POAManager_impl&) = delete;
      POAManager_impl& operator= (POAManager_impl&&) = delete;
      //@}

    private:
      POAManager_proxy_ref proxy_;
    };

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
    class TAOX11_PortableServer_Export POAManagerFactory_impl final
      : public virtual IDL::traits<PortableServer::POAManagerFactory>::base_type
    {
    public:
      friend class TAOX11_NAMESPACE::PortableServer::POA_impl;
      friend struct CORBA::object_traits<POAManagerFactory>;

      virtual IDL::traits<PortableServer::POAManager>::ref_type
      create_POAManager (const std::string& id,
                         const TAOX11_NAMESPACE::CORBA::PolicyList& policies) override;

      virtual POAManagerSeq list () override;

      virtual IDL::traits<PortableServer::POAManager>::ref_type
      find (const std::string& id) override;

      POAManagerFactory_proxy& _proxy ();

    protected:
      virtual ~POAManagerFactory_impl () = default;

    private:
      template <typename _Tp1, typename, typename ...Args>
      friend CORBA::object_reference<_Tp1> CORBA::make_reference(Args&& ...args);

      explicit POAManagerFactory_impl (POAManagerFactory_proxy_ptr op);

      /** @name Illegal to be called. Deleted explicitly to let the compiler detect any violation */
      //@{
      POAManagerFactory_impl () = delete;
      POAManagerFactory_impl (const POAManagerFactory_impl&) = delete;
      POAManagerFactory_impl (POAManagerFactory_impl&&) = delete;
      POAManagerFactory_impl& operator= (const POAManagerFactory_impl&) = delete;
      POAManagerFactory_impl& operator= (POAManagerFactory_impl&&) = delete;
      //@}

    private:
      POAManagerFactory_proxy_ref proxy_;
    };
#endif

    class TAOX11_PortableServer_Export POA_Current_impl final
      : public virtual IDL::traits<PortableServer::Current>::base_type
    {
    public:
      friend struct CORBA::object_traits<Current>;

      // generated from c++/cli_hdr/operation.erb
      virtual IDL::traits<PortableServer::POA>::ref_type
      get_POA () override;

      // generated from c++/cli_hdr/operation.erb
      virtual PortableServer::ObjectId get_object_id () override;

      // generated from c++/cli_hdr/operation.erb
      virtual IDL::traits<CORBA::Object>::ref_type
      get_reference () override;

      // generated from c++/cli_hdr/operation.erb
      virtual CORBA::servant_reference<PortableServer::Servant>
      get_servant () override;

      POA_Current_proxy& _proxy ();

    protected:
      virtual ~POA_Current_impl () = default;

    private:
      template <typename _Tp1, typename, typename ...Args>
      friend CORBA::object_reference<_Tp1> CORBA::make_reference(Args&& ...args);

      explicit POA_Current_impl (POA_Current_proxy_ptr op);

      /** @name Illegal to be called. Deleted explicitly to let the compiler detect any violation */
      //@{
      POA_Current_impl () = delete;
      POA_Current_impl (const POA_Current_impl&) = delete;
      POA_Current_impl (POA_Current_impl&&) = delete;
      POA_Current_impl& operator= (const POA_Current_impl&) = delete;
      POA_Current_impl& operator= (POA_Current_impl&&) = delete;
      //@}

    private:
      POA_Current_proxy_ref proxy_;
    };

  } // namespace PortableServer
} // namespace TAOX11_NAMESPACE

using namespace TAOX11_NAMESPACE;

#endif /* TAOX11_PORTABLESERVER_IMPL_H_INCLUDED */
