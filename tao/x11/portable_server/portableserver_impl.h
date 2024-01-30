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
  using POA_proxy_ptr = POA_proxy*;
  using POA_proxy_ref = std::unique_ptr<POA_proxy>;

  class POAManager_proxy;
  using POAManager_proxy_ptr = POAManager_proxy*;
  using POAManager_proxy_ref = std::unique_ptr<POAManager_proxy>;

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
  class POAManagerFactory_proxy;
  using POAManagerFactory_proxy_ptr = POAManagerFactory_proxy*;
  using POAManagerFactory_proxy_ref = std::unique_ptr<POAManagerFactory_proxy>;
#endif

  class POA_Current_proxy;
  using POA_Current_proxy_ptr = POA_Current_proxy*;
  using POA_Current_proxy_ref = std::unique_ptr<POA_Current_proxy>;

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

      void destroy (bool etherealize_objects,
                    bool wait_for_completion) override;

      PortableServer::ObjectId
      activate_object (CORBA::servant_reference<PortableServer::Servant> servant) override;

      void
      activate_object_with_id (const PortableServer::ObjectId & id,
                               CORBA::servant_reference<PortableServer::Servant> servant) override;

      void
      deactivate_object (const PortableServer::ObjectId & oid) override;

      IDL::traits<CORBA::Object>::ref_type
      id_to_reference (const PortableServer::ObjectId & oid) override;

      IDL::traits<POAManager>::ref_type
      the_POAManager () override;

#if ! defined (CORBA_E_MICRO)
      IDL::traits<POA>::ref_type
      create_POA (const std::string& adapter_name,
          IDL::traits<PortableServer::POAManager>::ref_type a_POAManager,
          const TAOX11_NAMESPACE::CORBA::PolicyList& policies) override;

      IDL::traits<POA>::ref_type
      find_POA (const std::string& adapter_name,
          bool activate_it) override;
#endif

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
      IDL::traits<PortableServer::ThreadPolicy>::ref_type
      create_thread_policy (TAOX11_NAMESPACE::PortableServer::ThreadPolicyValue value) override;
#endif
#if  ! defined (CORBA_E_MICRO)
      IDL::traits<PortableServer::LifespanPolicy>::ref_type
      create_lifespan_policy (TAOX11_NAMESPACE::PortableServer::LifespanPolicyValue value) override;

      IDL::traits<PortableServer::IdUniquenessPolicy>::ref_type
      create_id_uniqueness_policy (TAOX11_NAMESPACE::PortableServer::IdUniquenessPolicyValue value) override;

      IDL::traits<PortableServer::IdAssignmentPolicy>::ref_type
      create_id_assignment_policy (TAOX11_NAMESPACE::PortableServer::IdAssignmentPolicyValue value) override;
#endif
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
      IDL::traits<PortableServer::ImplicitActivationPolicy>::ref_type
      create_implicit_activation_policy (TAOX11_NAMESPACE::PortableServer::ImplicitActivationPolicyValue value) override;

      IDL::traits<PortableServer::ServantRetentionPolicy>::ref_type
      create_servant_retention_policy (TAOX11_NAMESPACE::PortableServer::ServantRetentionPolicyValue value) override;

      IDL::traits<PortableServer::RequestProcessingPolicy>::ref_type
      create_request_processing_policy (TAOX11_NAMESPACE::PortableServer::RequestProcessingPolicyValue value) override;
#endif
      std::string the_name() override;

      IDL::traits<POA>::ref_type the_parent() override;
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
      TAOX11_NAMESPACE::PortableServer::POAList
      the_children() override;
#endif
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
      IDL::traits<POAManagerFactory>::ref_type
      the_POAManagerFactory() override;

      IDL::traits<AdapterActivator>::ref_type
      the_activator() override;
      void
      the_activator(IDL::traits<PortableServer::AdapterActivator>::ref_type _v) override;

      IDL::traits<ServantManager>::ref_type
      get_servant_manager () override;

      void
      set_servant_manager (IDL::traits<PortableServer::ServantManager>::ref_type imgr) override;

      CORBA::servant_reference<PortableServer::Servant>
      get_servant () override;

      void
      set_servant (CORBA::servant_reference<PortableServer::Servant> p_servant) override;
#endif
      IDL::traits<CORBA::Object>::ref_type
      create_reference (const TAOX11_NAMESPACE::CORBA::RepositoryId& intf) override;
#if !defined (CORBA_E_MICRO)
      IDL::traits<CORBA::Object>::ref_type
      create_reference_with_id (const TAOX11_NAMESPACE::PortableServer::ObjectId& oid,
                                const TAOX11_NAMESPACE::CORBA::RepositoryId& intf) override;
#endif
      TAOX11_NAMESPACE::PortableServer::ObjectId
      servant_to_id (CORBA::servant_reference<PortableServer::Servant> p_servant) override;

      IDL::traits<CORBA::Object>::ref_type
      servant_to_reference (CORBA::servant_reference<PortableServer::Servant> p_servant) override;

      CORBA::servant_reference<PortableServer::Servant>
      reference_to_servant (IDL::traits<CORBA::Object>::ref_type reference) override;

      TAOX11_NAMESPACE::PortableServer::ObjectId
      reference_to_id (IDL::traits<CORBA::Object>::ref_type reference) override;

      CORBA::servant_reference<PortableServer::Servant>
      id_to_servant (const TAOX11_NAMESPACE::PortableServer::ObjectId& oid) override;

      TAOX11_NAMESPACE::CORBA::OctetSeq id() override;

      POA_proxy& _proxy ();

    protected:
      virtual ~POA_impl () = default;

    private:
      template <typename _Tp1, typename, typename ...Args>
      friend constexpr CORBA::object_reference<_Tp1> CORBA::make_reference(Args&& ...args);

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

      void activate () override;
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO) && (TAO_HAS_MINIMUM_POA == 0)
      void hold_requests (bool wait_for_completion) override;
      void discard_requests (bool wait_for_completion) override;
      void deactivate (bool etherealize_objects,
                               bool wait_for_completion) override;
#endif
      State get_state () override;

      std::string get_id () override;

      POAManager_proxy& _proxy ();

    protected:
      virtual ~POAManager_impl () = default;

    private:
      template <typename _Tp1, typename, typename ...Args>
      friend constexpr CORBA::object_reference<_Tp1> CORBA::make_reference(Args&& ...args);

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

      IDL::traits<PortableServer::POAManager>::ref_type
      create_POAManager (const std::string& id,
                         const TAOX11_NAMESPACE::CORBA::PolicyList& policies) override;

      POAManagerSeq list () override;

      IDL::traits<PortableServer::POAManager>::ref_type
      find (const std::string& id) override;

      POAManagerFactory_proxy& _proxy ();

    protected:
      virtual ~POAManagerFactory_impl () = default;

    private:
      template <typename _Tp1, typename, typename ...Args>
      friend constexpr CORBA::object_reference<_Tp1> CORBA::make_reference(Args&& ...args);

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

      IDL::traits<PortableServer::POA>::ref_type get_POA () override;

      PortableServer::ObjectId get_object_id () override;

      IDL::traits<CORBA::Object>::ref_type get_reference () override;

      CORBA::servant_reference<PortableServer::Servant> get_servant () override;

      POA_Current_proxy& _proxy ();

    protected:
      virtual ~POA_Current_impl () = default;

    private:
      template <typename _Tp1, typename, typename ...Args>
      friend constexpr CORBA::object_reference<_Tp1> CORBA::make_reference(Args&& ...args);

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
