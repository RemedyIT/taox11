/**
 * @file    policy_manager_impl.cpp
 * @author  Marcel Smit
 *
 * @brief   x11 CORBA Policy framework
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/objproxy.h"
#include "tao/x11/Policy_ManagerC.h"
#include "tao/x11/exception_macros.h"

namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {
    class TAOX11_Export Policy_Manager_impl final
      : public ::TAOX11_NAMESPACE::IDL::traits<PolicyManager>::base_type
    {
    public:
      Policy_Manager_impl (TAO_CORBA::PolicyManager_ptr polptr);
      ~Policy_Manager_impl () override = default;

      ::TAOX11_NAMESPACE::CORBA::PolicyList
      get_policy_overrides (
        const ::TAOX11_NAMESPACE::CORBA::PolicyTypeSeq& ts) override;

      void set_policy_overrides (
        const ::TAOX11_NAMESPACE::CORBA::PolicyList& policies,
        ::TAOX11_NAMESPACE::CORBA::SetOverrideType set_add) override;

    private:
      Policy_Manager_impl (const Policy_Manager_impl&) = delete;
      Policy_Manager_impl (Policy_Manager_impl&&) = delete;
      Policy_Manager_impl& operator= (const Policy_Manager_impl&) = delete;
      Policy_Manager_impl& operator= (Policy_Manager_impl&&) = delete;

      TAO_CORBA::PolicyManager_ptr pol_;
    };

    Policy_Manager_impl::Policy_Manager_impl (TAO_CORBA::PolicyManager_ptr polptr)
      : pol_ (polptr)
    {
      this->set_proxy (new Object_proxy (polptr));
    }


    ::TAOX11_NAMESPACE::CORBA::PolicyList
    Policy_Manager_impl::get_policy_overrides (
      const ::TAOX11_NAMESPACE::CORBA::PolicyTypeSeq& ts)
    {
      try
      {
        TAO_CORBA::PolicyTypeSeq _type_seq_tmp;
        _type_seq_tmp.length(ACE_Utils::truncate_cast<TAO_CORBA::ULong> (ts.size ()));
        for(TAO_CORBA::ULong l = 0; l < _type_seq_tmp.length(); l++)
        {
          _type_seq_tmp[l] = ts[l];
        }

        TAO_CORBA::PolicyList_var _list_tmp =
          this->pol_->get_policy_overrides (_type_seq_tmp);

        PolicyList _pols;
        for (TAO_CORBA::ULong l = 0; l < _list_tmp->length(); l++)
        {
          object_reference<Policy> pol =
            make_reference<TAOX11_NAMESPACE::CORBA::Policy> (
              new TAOX11_NAMESPACE::Object_proxy (
                TAO_CORBA::Policy::_duplicate (_list_tmp[l])));
          _pols.push_back(pol);
        }
        return _pols;
      }
      catch_tao_system_ex (_sx)

      return TAOX11_NAMESPACE::CORBA::PolicyList();
    }

    void
    Policy_Manager_impl::set_policy_overrides (
      const ::TAOX11_NAMESPACE::CORBA::PolicyList& policies,
      ::TAOX11_NAMESPACE::CORBA::SetOverrideType set_add)
    {
      try
      {
        TAO_CORBA::PolicyList _pols;
        _pols.length (TAO_CORBA::ULong (policies.size ()));
        TAO_CORBA::ULong n (0);
        for (const object_reference<TAOX11_NAMESPACE::CORBA::Policy>& _pol : policies)
        {
          _pols[n++] = TAO_CORBA::Policy::_narrow (
            const_cast <TAO_CORBA::Object_ptr>(_pol->get_proxy ()->get ()));
        }
        this->pol_->set_policy_overrides (_pols,
          static_cast<TAO_CORBA::SetOverrideType> (set_add));
      }
      catch_tao_system_ex (_sx)
    }

    template<>
    object_traits< ::TAOX11_NAMESPACE::CORBA::PolicyManager>::ref_type
    object_traits< ::TAOX11_NAMESPACE::CORBA::PolicyManager>::narrow (
       object_traits<TAOX11_NAMESPACE::CORBA::Object>::ref_type objref)
    {
      if (objref)
      {
        if (objref->_is_local ())
        {
          return ref_type::_narrow (std::move(objref));
        }

        try
        {
          TAO_CORBA::PolicyManager_ptr polptr =
            TAO_CORBA::PolicyManager::_narrow (objref->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (polptr))
          {
            return CORBA::make_reference<Policy_Manager_impl> (polptr);
          }
        }
        catch_tao_system_ex(_ex)
      }

      return nullptr;
    }
  } // namespace CORBA
} // namespace TAOX11_NAMESPACE
