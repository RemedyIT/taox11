/**
 * @file    object.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 core Object classes
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/objproxy.h"
#include "tao/x11/orbproxy.h"

#include "tao/x11/object.h"
#include "tao/x11/orb.h"
#include "tao/x11/exception_macros.h"
#include "tao/x11/PolicyC.h"
#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
# include "tao/x11/dynamic_adapter.h"
# include "tao/x11/ifr_client_adapter.h"
#endif

namespace TAOX11_NAMESPACE
{

  namespace IDL
  {
    std::string traits<CORBA::Object>::_dump (
        traits<CORBA::Object>::ref_type obj,
        const char* objtype,
        bool amic)
    {
      std::string objstr = amic ? "CORBA::amic_traits<" : "IDL::traits<";
      objstr += objtype;
      objstr += ">::ref_type{";
      if (!obj)
      {
        objstr += "nil";
      }
      else if (obj->_is_local ())
      {
        objstr += "local:";
        // dirty trick, I know
        objstr += std::to_string ((uint64_t)obj.operator -> ());
      }
      else
      {
        objstr += "object:";
        objstr += obj->_repository_id();
      }
      objstr += '}';
      return objstr;
    }
  } // namespace IDL

  namespace CORBA
  {
    /*  Object and LocalObject traits method specializations
     *
     */
    template<>
    TAOX11_Export object_traits<CORBA::Object>::shared_ptr_type
    object_traits<CORBA::Object>::lock_shared (
        CORBA::Object* p)
    {
      return p->_lock_shared ();
    }

    template<>
    object_traits<CORBA::Object>::ref_type
    object_traits<CORBA::Object>::narrow (
        object_traits<CORBA::Object>::ref_type oref)
    {
      return oref;
    }

    template<>
    object_traits<CORBA::LocalObject>::shared_ptr_type
    object_traits<CORBA::LocalObject>::lock_shared (
        CORBA::LocalObject* p)
    {
      return std::dynamic_pointer_cast<CORBA::LocalObject> (p->_lock_shared ());
    }

    /*  Object and LocalObject implementation
     *
     */
    Object::~Object ()
    {
    }

    Object::_shared_ptr_type Object::_lock_shared ()
    {
      Object::_shared_ptr_type objref = this->self_ref_.lock ();
      if (!objref)
      {
        objref = Object::_shared_ptr_type (this, Object::dtor ());
        this->self_ref_ = objref;
      }
      return objref;
    }

    bool Object::_is_a (const std::string& local_type_id)
    {
      if (this->proxy_)
      {
        try {
          return (*this->proxy_)->_is_a (local_type_id.c_str ());
        }
        catch_tao_system_ex (_sx)
      }
      return false;
    }

    bool Object::_is_equivalent (object_traits<Object>::ref_type other_obj)
    {
      bool rc = false;
      if (other_obj == nullptr)
        {
          rc = false;
        }
      else if ((other_obj.operator ->()) == this)
        {
          rc = true;
        }
      else
        {
          TAO_CORBA::Object * tmp = other_obj->get_proxy()->get();
          rc = (*this->proxy_)->_is_equivalent (tmp);
        }
      return rc;
    }

    uint32_t
    Object::_hash (uint32_t maximum)
    {
      return (*this->proxy_)->_hash (maximum);
    }

    bool
    Object::_is_local()
    {
      return false;
    }

    object_traits<TAOX11_NAMESPACE::CORBA::ORB>::ref_type
    Object::_get_orb ()
    {
      TAO_CORBA::ORB_var _orb = (*this->proxy_)->_get_orb ();
      if (!TAO_CORBA::is_nil (_orb))
        {
          ORB_Proxy_ptr opp = new ORB_Proxy (_orb._retn ());
          return make_reference<TAOX11_NAMESPACE::CORBA::ORB> (opp);
        }
      return object_traits<TAOX11_NAMESPACE::CORBA::ORB>::ref_type();
    }

#if (TAO_HAS_MINIMUM_CORBA == 0)
    bool Object::_non_existent ()
    {
      if (this->proxy_)
      {
        try {
          return (*this->proxy_)->_non_existent ();
        }
        catch_tao_system_ex (_sx)
      }
      return true;
    }

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)

    /// Get info about the object from the Interface Repository.
    object_reference<TAOX11_CORBA::InterfaceDef> Object::_get_interface ()
    {
      IFRClientAdapter *adapter = ORB::ifr_client_adapter ();
      if (adapter)
      {
        return adapter->get_interface_remote (this->_lock ());
      }
      return nullptr;
    }

    /// Get info about the object from the Interface Repository.
    object_reference<TAOX11_CORBA::Object> Object::_get_component ()
    {
      if (this->proxy_)
      {
        try {
          TAO_CORBA::Object_var tmp = (*this->proxy_)->_get_component ();
          if (!TAO_CORBA::is_nil (tmp))
          {
            return Object::_narrow (Object_proxy (TAO_CORBA::Object::_duplicate (tmp)));
          }
        }
        catch_tao_system_ex (_sx)
      }
      return object_reference<TAOX11_CORBA::Object> ();
    }
#endif

    /// Get the repository id.
    std::string Object::_repository_id ()
    {
      if (this->proxy_)
      {
        try {
          TAO_CORBA::String_var _id = (*this->proxy_)->_repository_id ();
          return _id.in ();
        }
        catch_tao_system_ex (_sx)
      }
      return {};
    }
#endif

    std::string Object::_interface_repository_id () const
    {
      if (this->proxy_)
      {
        return (*this->proxy_)->_interface_repository_id ();
      }
      return {};
    }

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
    void Object::_create_request (
        object_reference<Context> ctx,
        const std::string& operation,
        object_reference<NVList> arg_list,
        object_reference<NamedValue> result,
        object_reference<Request>& request,
        Flags req_flags)
    {
      if (ctx)
      {
        // context is not supported; should be nil
        throw CORBA::NO_IMPLEMENT ();
      }
      DynamicAdapter *adapter = DynamicAdapter::dynamic_adapter ();
      if (adapter)
      {
        adapter->create_request (
            (*this->proxy_),
            operation,
            arg_list,
            result,
            nullptr,
            request,
            req_flags);
      }
    }

    void Object::_create_request (
        object_reference<Context> ctx,
        const std::string& operation,
        object_reference<NVList> arg_list,
        object_reference<NamedValue> result,
        const ExceptionList& exclist,
        const ContextList&, // ignored
        object_reference<Request>& request,
        Flags req_flags)
    {
      if (ctx)
      {
        // context is not supported; should be nil
        throw CORBA::NO_IMPLEMENT ();
      }
      DynamicAdapter *adapter = DynamicAdapter::dynamic_adapter ();
      if (adapter)
      {
        adapter->create_request (
            (*this->proxy_),
            operation,
            arg_list,
            result,
            &exclist,
            request,
            req_flags);
      }
    }

    object_reference<Request> Object::_request (
        const std::string& operation)
    {
      DynamicAdapter *adapter = DynamicAdapter::dynamic_adapter ();
      if (adapter)
      {
        return adapter->request (
            (*this->proxy_),
            operation);
      }
      return nullptr;
    }
#endif

#if (TAO_HAS_CORBA_MESSAGING == 1)

    object_traits<TAOX11_NAMESPACE::CORBA::Policy>::ref_type
    Object::_get_client_policy (const TAOX11_NAMESPACE::CORBA::PolicyType& /*policy_type*/)
    {
      throw CORBA::NO_IMPLEMENT (TAO_CORBA::OMGVMCID | 8, CORBA::COMPLETED_NO);
    }

    object_traits<TAOX11_NAMESPACE::CORBA::Policy>::ref_type
    Object::_get_policy (const TAOX11_NAMESPACE::CORBA::PolicyType& policy_type)
    {
      try
      {
        TAO_CORBA::Policy_var _tmp = (*this->proxy_)->_get_policy (policy_type);
        if (!TAO_CORBA::is_nil (_tmp.in()))
          {
            return make_reference<TAOX11_NAMESPACE::CORBA::Policy> (
                new TAOX11_NAMESPACE::Object_proxy (
                    TAO_CORBA::Policy::_duplicate(_tmp.in())));
          }
      }
      catch_tao_system_ex (_sx)
      return make_reference<TAOX11_NAMESPACE::CORBA::Policy> ();
    }

    object_traits<Object>::ref_type
    Object::_set_policy_overrides (
          const TAOX11_NAMESPACE::CORBA::PolicyList& policies,
          const TAOX11_NAMESPACE::CORBA::SetOverrideType& set_add)
    {
      try {
        TAO_CORBA::PolicyList _pols;
        _pols.length (TAO_CORBA::ULong (policies.size ()));
        TAO_CORBA::ULong n (0);
        for (object_reference<TAOX11_NAMESPACE::CORBA::Policy> _pol : policies)
          {
            _pols[n++] =
                TAO_CORBA::Policy::_narrow (_pol->get_proxy ()->get ());
          }
        TAO_CORBA::Object_var tmp = (*this->proxy_)->_set_policy_overrides (
                            _pols,
                            static_cast<TAO_CORBA::SetOverrideType>(set_add));
        if (!TAO_CORBA::is_nil (tmp))
          {
            return Object::_narrow (Object_proxy (
                                        TAO_CORBA::Object::_duplicate (tmp)));
          }
      }
      catch_tao_system_ex (_sx)

      return object_traits<Object>::ref_type ();
    }

    TAOX11_NAMESPACE::CORBA::PolicyList
    Object::_get_policy_overrides (
          const TAOX11_NAMESPACE::CORBA::PolicyTypeSeq & types)
    {
      try {
        TAO_CORBA::PolicyTypeSeq _type_seq_tmp;
        _type_seq_tmp.length(ACE_Utils::truncate_cast<TAO_CORBA::ULong> (types.size()));
        for(TAO_CORBA::ULong l = 0; l < _type_seq_tmp.length(); l++)
          {
            _type_seq_tmp[l] = types[l];
          }

        TAO_CORBA::PolicyList_var _list_tmp = (*this->proxy_)->_get_policy_overrides (
                             _type_seq_tmp);

        PolicyList _pols;
         for (TAO_CORBA::ULong l = 0; l < _list_tmp->length(); l++)
           {
              object_reference<Policy> pol =
                  make_reference<TAOX11_NAMESPACE::CORBA::Policy> (
                      new TAOX11_NAMESPACE::Object_proxy (
                          TAO_CORBA::Policy::_duplicate(_list_tmp[l])));
              _pols.push_back(pol);
           }
        return _pols;
      }
      catch_tao_system_ex (_sx)

      return TAOX11_NAMESPACE::CORBA::PolicyList();
    }

    bool
    Object::_validate_connection (
        TAOX11_NAMESPACE::CORBA::PolicyList& inconsistent_policies)
    {
      TAO_CORBA::PolicyList_var _pols;
      bool const rc = (*this->proxy_)->_validate_connection (_pols.out());
      for (TAO_CORBA::ULong l = 0; l < _pols->length(); l++)
          {
            object_reference<Policy> pol = make_reference<Policy> (
                  new TAOX11_NAMESPACE::Object_proxy (
                      TAO_CORBA::Policy::_duplicate(_pols[l])));
             inconsistent_policies.push_back(pol);
          }

      return rc;
    }

#endif

    TAOX11_NAMESPACE::CORBA::DomainManagerList
    Object::_get_domain_managers()
    {
      throw CORBA::NO_IMPLEMENT (TAO_CORBA::OMGVMCID | 8, CORBA::COMPLETED_NO);
    }

    Object::Object (Object_proxy_ptr op)
      : proxy_ (op)
    {
    }

    Object_proxy_ptr Object::get_proxy () const
    {
      return this->proxy_.get();
    }

    void Object::set_proxy (Object_proxy_ptr op)
    {
      this->proxy_.reset (op);
    }

    object_traits<Object>::ref_type
    Object::_narrow (Object_proxy&& op)
    {
      return op ? make_reference<Object> (new Object_proxy (std::move (op))) : nullptr;
    }

    LocalObject::~LocalObject ()
    {
    }

    bool LocalObject::_is_a (const std::string& local_type_id)
    {
      return (local_type_id == "IDL:omg.org/CORBA/LocalObject:1.0"
              || local_type_id == "IDL:omg.org/CORBA/Object:1.0");
    }

    bool LocalObject::_is_equivalent (object_traits<Object>::ref_type other_obj)
    {
      bool rc = false;
      if (!other_obj)
        {
          rc = false;
        }
      else if ((other_obj.operator ->()) == this)
        {
          rc = true;
        }
      return rc;
    }

    uint32_t
    LocalObject::_hash (uint32_t maximum)
    {
      // Note that we reinterpret_cast to an "ptrdiff_t" instead of
      // an uint32_t since we need to first cast to an integer large
      // enough to hold an address to avoid compile-time warnings on some
      // 64-bit platforms.
      uint32_t const hash =
        static_cast<uint32_t> (reinterpret_cast<ptrdiff_t> (this));

      return hash % maximum;
    }

    bool
    LocalObject::_is_local()
    {
      return true;
    }

#if (TAO_HAS_CORBA_MESSAGING == 1)

    object_traits<TAOX11_NAMESPACE::CORBA::Policy>::ref_type
    LocalObject::_get_client_policy (const TAOX11_NAMESPACE::CORBA::PolicyType& /*policy_type*/)
    {
      throw CORBA::NO_IMPLEMENT (TAO_CORBA::OMGVMCID | 8, CORBA::COMPLETED_NO);
    }

    object_traits<TAOX11_NAMESPACE::CORBA::Policy>::ref_type
    LocalObject::_get_policy (const TAOX11_NAMESPACE::CORBA::PolicyType& /*policy_type*/)
    {
      throw CORBA::NO_IMPLEMENT (TAO_CORBA::OMGVMCID | 8, CORBA::COMPLETED_NO);
    }

    object_traits<Object>::ref_type
    LocalObject::_set_policy_overrides (
          const TAOX11_NAMESPACE::CORBA::PolicyList & /*policies*/,
          const TAOX11_NAMESPACE::CORBA::SetOverrideType& /*set_add*/)
    {
      throw CORBA::NO_IMPLEMENT (TAO_CORBA::OMGVMCID | 8, CORBA::COMPLETED_NO);
    }

    TAOX11_NAMESPACE::CORBA::PolicyList
    LocalObject::_get_policy_overrides (
          const TAOX11_NAMESPACE::CORBA::PolicyTypeSeq & /*types*/)
    {
      throw CORBA::NO_IMPLEMENT (TAO_CORBA::OMGVMCID | 8, CORBA::COMPLETED_NO);
    }

    bool
    LocalObject::_validate_connection (
        TAOX11_NAMESPACE::CORBA::PolicyList& /*inconsistent_policies*/)
    {
      throw CORBA::NO_IMPLEMENT (TAO_CORBA::OMGVMCID | 8, CORBA::COMPLETED_NO);
    }

#endif

    object_traits<TAOX11_NAMESPACE::CORBA::ORB>::ref_type
    LocalObject::_get_orb ()
    {
      throw CORBA::NO_IMPLEMENT (TAO_CORBA::OMGVMCID | 8, CORBA::COMPLETED_NO);
    }

    TAOX11_NAMESPACE::CORBA::DomainManagerList
    LocalObject::_get_domain_managers()
    {
      throw CORBA::NO_IMPLEMENT (TAO_CORBA::OMGVMCID | 8, CORBA::COMPLETED_NO);
    }

#if (TAO_HAS_MINIMUM_CORBA == 0)
    bool LocalObject::_non_existent ()
    {
      return false;
    }

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)

    /// Get info about the object from the Interface Repository.
    object_traits<InterfaceDef>::ref_type LocalObject::_get_interface ()
    {
      throw CORBA::NO_IMPLEMENT (TAO_CORBA::OMGVMCID | 8, CORBA::COMPLETED_NO);
    }

    /// Get info about the object from the Interface Repository.
    object_traits<Object>::ref_type LocalObject::_get_component ()
    {
      throw CORBA::NO_IMPLEMENT (TAO_CORBA::OMGVMCID | 8, CORBA::COMPLETED_NO);
    }
#endif

    /// Get the repository id.
    std::string LocalObject::_repository_id ()
    {
      throw CORBA::NO_IMPLEMENT (TAO_CORBA::OMGVMCID | 8, CORBA::COMPLETED_NO);
    }
#endif

#if ! defined (CORBA_E_COMPACT) && ! defined (CORBA_E_MICRO)
    void LocalObject::_create_request (
        object_reference<Context>,
        const std::string&,
        object_reference<NVList>,
        object_reference<NamedValue>,
        object_reference<Request>&,
        Flags)
    {
      throw CORBA::NO_IMPLEMENT (TAO_CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO);
    }

    void LocalObject::_create_request (
        object_reference<Context>,
        const std::string&,
        object_reference<NVList>,
        object_reference<NamedValue>,
        const ExceptionList&,
        const ContextList&,
        object_reference<Request>&,
        Flags)
    {
      throw CORBA::NO_IMPLEMENT (TAO_CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO);
    }

    object_reference<Request> LocalObject::_request (
        const std::string&)
    {
      throw CORBA::NO_IMPLEMENT (TAO_CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO);
    }
#endif

  } // namespace CORBA
} // namespace TAOX11_NAMESPACE
