/**
 * @file    orb_registry.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 ORB registry class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_ORB_REGISTRY_H_INCLUDED
#define TAOX11_ORB_REGISTRY_H_INCLUDED

#include "tao/x11/stddef.h"
#include "tao/x11/taox11_export.h"
#include "tao/x11/object.h"
#include "tao/x11/orb.h"
#include "tao/x11/anyinsert_adapter.h"
#if !defined(CORBA_E_MICRO)
# include "tao/x11/valuefactory_manager_adapter.h"
# include "tao/x11/valuetype_adapter.h"
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
#include <map>
#include <mutex>

namespace TAOX11_NAMESPACE
{
  class Object_Loader; //forward

  /**
   * @class ORB_Registry
   *
   * @brief Class to register global ORB resources.
   *
   */
  class TAOX11_Export ORB_Registry
  {
  public:
    ~ORB_Registry () = default;

    AnyInsertAdapter *any_insert_adapter ();

#if !defined(CORBA_E_MICRO)
    ValueFactoryManagerAdapter *valuefactory_manager ();

    ValueTypeAdapter *value_type_adapter ();

    PolicyFactoryManagerAdapter *policy_manager ();
#endif

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
    IFRClientAdapter *ifr_client_adapter ();

    DynamicAdapter *dynamic_adapter ();

# if (TAO_HAS_MINIMUM_CORBA == 0)
    NVListAdapter *nvlist_adapter ();

    TypecodeFactoryAdapter *typecode_factory_adapter ();
# endif
#endif

    static IDL::traits<CORBA::Object>::ref_type resolve_tao_service (
        CORBA::object_reference<CORBA::ORB>, const std::string&);

  private:
    friend class CORBA::ORB;

    ORB_Registry() = default;

    IDL::traits<CORBA::Object>::ref_type
    resolve_initial_reference (
        CORBA::object_reference<CORBA::ORB>_type,
        const std::string&);

    CORBA::ORB::ObjectIdList
    list_initial_references (CORBA::object_reference<CORBA::ORB>);

    void register_user_service (std::string, IDL::traits<CORBA::Object>::ref_type);
    IDL::traits<CORBA::Object>::ref_type resolve_user_service (const std::string&);
    void unregister_user_service (const std::string&);

    Object_Loader *resolve_object_loader (const std::string&);

    AnyInsertAdapter *any_insert_adapter_ {};

#if !defined(CORBA_E_MICRO)
    ValueFactoryManagerAdapter *vf_manager_ {};

    ValueTypeAdapter *vt_adapter_ {};

    PolicyFactoryManagerAdapter *policy_manager_ {};
#endif

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
    IFRClientAdapter *ifr_client_adapter_ {};

    DynamicAdapter *dynamic_adapter_ {};

# if (TAO_HAS_MINIMUM_CORBA == 0)
    NVListAdapter *nvlist_adapter_ {};

    TypecodeFactoryAdapter *typecode_factory_adapter_ {};
# endif
#endif

    Object_Loader *typecode_factory_loader_ {};
    Object_Loader *codec_factory_loader_ {};
    Object_Loader *dynamicany_factory_loader_ {};

    typedef std::map<std::string, IDL::traits<CORBA::Object>::ref_type> user_services_map_t;
    user_services_map_t user_services_;

    static std::shared_ptr<ORB_Registry> instance (bool create=false);

    static std::mutex lock_;
    static std::weak_ptr<ORB_Registry> instance_;
  };
} // namespace TAOX11_NAMESPACE

#endif // TAOX11_ORB_REGISTRY_H_INCLUDED
