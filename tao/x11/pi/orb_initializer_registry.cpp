/**
 * @file    orb_initializer_registry.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 ORBInitializer registry
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "tao/x11/tao_corba.h"
#include "tao/x11/pi/tao_pi.h"
#include "tao/ORBInitializer_Registry.h"

#include "tao/x11/pi/orb_initializer_registry.h"
#include "tao/x11/pi/orb_init_info.h"

#include "tao/x11/user_exception_proxy_out.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableInterceptor_impl
  {
    // ORBInitializer_proxy
    class ORBInitializer_proxy final :
        public virtual TAO_PI::ORBInitializer,
        public virtual TAO_CORBA::LocalObject
    {
    public:
      explicit ORBInitializer_proxy (
          IDL::traits<PortableInterceptor::ORBInitializer>::ref_type init)
        : init_ (std::move (init))
      {}
      virtual ~ORBInitializer_proxy () = default;

      /// The pre-initialization hook.
      virtual void pre_init (
          TAO_PI::ORBInitInfo_ptr info) override;

      /// The post-initialization hook.
      virtual void post_init (
          TAO_PI::ORBInitInfo_ptr info) override;

    private:
      IDL::traits<PortableInterceptor_impl::ORBInitInfo>::ref_type to_x11_ (
          TAO_PI::ORBInitInfo_ptr info)
      {
        TAO_PI::ORBInitInfo_3_1_ptr const orbiiptr =
            TAO_PI::ORBInitInfo_3_1::_narrow (info);
        if (!TAO_CORBA::is_nil (orbiiptr))
        {
          return CORBA::make_reference<PortableInterceptor_impl::ORBInitInfo> (orbiiptr);
        }
        return {};
      }

      IDL::traits<PortableInterceptor::ORBInitializer>::ref_type  init_;

      ORBInitializer_proxy () = delete;
      ORBInitializer_proxy (const ORBInitializer_proxy&) = delete;
      ORBInitializer_proxy (ORBInitializer_proxy&&) = delete;
      ORBInitializer_proxy& operator= (const ORBInitializer_proxy&) = delete;
      ORBInitializer_proxy& operator= (ORBInitializer_proxy&&) = delete;
    };

    void ORBInitializer_proxy::pre_init (
        TAO_PI::ORBInitInfo_ptr info)
    {
      try
      {
        this->init_->pre_init (this->to_x11_ (info));
      }
      catch (const CORBA::SystemException& sx)
      {
        sx._raise_tao ();
      }
      catch (const CORBA::UserException& ux)
      {
        throw UserExceptionProxy::out (ux);
      }
    }

    /// The post-initialization hook.
    void ORBInitializer_proxy::post_init (
        TAO_PI::ORBInitInfo_ptr info)
    {
      try
      {
        this->init_->post_init (this->to_x11_ (info));
      }
      catch (const CORBA::SystemException& sx)
      {
        sx._raise_tao ();
      }
      catch (const CORBA::UserException& ux)
      {
        throw UserExceptionProxy::out (ux);
      }
    }
  }

  namespace PortableInterceptor
  {
    /// Register an ORBInitializer with the global ORBInitializer
    /// table.
    void
    register_orb_initializer (IDL::traits<ORBInitializer>::ref_type init)
    {
      TAO_PI::ORBInitializer_ptr temp_initializer =
          TAO_PI::ORBInitializer::_nil ();
      ACE_NEW_THROW_EX (temp_initializer,
                        PortableInterceptor_impl::ORBInitializer_proxy (init),
                        CORBA::NO_MEMORY (
                          TAO_CORBA::SystemException::_tao_minor_code (
                            0,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      TAO_PI::ORBInitializer_var initializer = temp_initializer;
      TAO_PI::register_orb_initializer (initializer.in ());
    }
  }
}
