/**
 * @file    orb.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 core ORB class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_ORB_H_INCLUDED
#define TAOX11_ORB_H_INCLUDED

#include "tao/x11/object.h"
#include "tao/x11/user_exception.h"
#include "tao/x11/ServicesC.h"
#include <chrono>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Reactor;
ACE_END_VERSIONED_NAMESPACE_DECL

namespace TAOX11_NAMESPACE
{
  // forwards
  class ORB_Registry;
  class AnyInsertAdapter;
#if !defined(CORBA_E_MICRO)
  class ValueTypeAdapter;
  class PolicyFactoryManagerAdapter;
  class ValueFactoryManagerAdapter;
# if !defined (CORBA_E_COMPACT)
  class IFRClientAdapter;
  class DynamicAdapter;
#   if (TAO_HAS_MINIMUM_CORBA == 0)
  class NVListAdapter;
  class TypecodeFactoryAdapter;
#   endif
# endif
#endif

  class ORB_Proxy;
  using ORB_Proxy_ptr = ORB_Proxy*;
  using ORB_Proxy_ref = std::unique_ptr<ORB_Proxy>;

  namespace CORBA
  {
    // forward
    class ORB;
    class Any;
    class ValueFactoryBase;
    using ValueFactory = object_reference<ValueFactoryBase>;

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
    // forwards
    class InterfaceDef;
    class OperationDef;
    class NVList;
# if (TAO_HAS_MINIMUM_CORBA == 0)
    class Context;
    class Request;
    using RequestSeq = std::vector<object_reference<Request>>;
    using EnumMemberSeq = std::vector<Identifier>;
    class StructMember;
    using StructMemberSeq = std::vector<StructMember>;
    class UnionMember;
    using UnionMemberSeq = std::vector<UnionMember>;
    class ValueMember;
    using ValueMemberSeq = std::vector<ValueMember>;
    using ValueModifier = int16_t;
# endif
#endif

    /// ORB trait method specialization
    template<>
    TAOX11_Export object_traits<CORBA::ORB>::shared_ptr_type
    object_traits<CORBA::ORB>::lock_shared (
        CORBA::ORB*);
  } // namespace CORBA

  namespace IDL
  {
    /// ORB traits
    template<>
    struct traits <CORBA::ORB> :
      public IDL::common_byval_traits <CORBA::object_reference <CORBA::ORB>>,
      public CORBA::object_traits <CORBA::ORB>
    {
      template <typename OStrm_, typename Formatter = formatter<CORBA::ORB, OStrm_>>
      static inline OStrm_& write_on(
          OStrm_& os_, in_type val_,
          Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };
  } // namespace IDL

  namespace CORBA
  {
    class TAOX11_Export ORB
      : public virtual CORBA::LocalObject
    {
    public:
      /// @name Member types
      //@{
      using _traits_type = IDL::traits<ORB>;
      /// Strong reference type
      using _ref_type = _traits_type::ref_type;
      //@}

      using ObjectId = std::string;
      using ObjectIdList = std::vector<ObjectId>;

      friend struct TAOX11_CORBA::object_traits<ORB>;
      friend class Object;

      class InvalidName final
       : public CORBA::UserException
      {
      public:
        void _raise () const override;

        void _tao_encode (TAO_OutputCDR &cdr) const override;

        void _tao_decode (TAO_InputCDR &cdr) override;

        /// Deep copy
        CORBA::Exception *_tao_duplicate () const override;

        InvalidName ();
        ~InvalidName () noexcept override = default;
        InvalidName (const InvalidName&) = default;
        InvalidName (InvalidName&&) = default;
        InvalidName& operator= (const InvalidName& x) = default;
        InvalidName& operator= (InvalidName&& x)  = default;

      protected:
        void _info (std::ostream&) const override;
      }; // InvalidName

      std::string id ();

      IDL::traits<CORBA::Object>::ref_type string_to_object (const std::string& str);

      std::string object_to_string (IDL::traits<CORBA::Object>::ref_type obj);

#if (TAO_HAS_MINIMUM_CORBA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
      void create_list (
          uint32_t count,
          CORBA::object_reference<CORBA::NVList> &new_list);

      void create_operation_list(
          CORBA::object_reference<CORBA::OperationDef>,
          CORBA::object_reference<CORBA::NVList>&);

      void get_default_context (object_reference<Context> &ctx);

      void send_multiple_requests_oneway (const CORBA::RequestSeq &req);
      void send_multiple_requests_deferred (const CORBA::RequestSeq &req);
      void get_next_response (object_reference<CORBA::Request> &req);

      bool poll_next_response ();

      bool get_service_information (
          CORBA::ServiceType service_type,
          CORBA::ServiceInformation &service_information);

      /// The ORB TypeCode creation functions.

      CORBA::object_reference<CORBA::TypeCode> create_struct_tc (
          const std::string &id,
          const std::string &name,
          const CORBA::StructMemberSeq &members);

      CORBA::object_reference<CORBA::TypeCode> create_union_tc (
          const std::string &id,
          const std::string &name,
          CORBA::object_reference<CORBA::TypeCode> discriminator_type,
          const CORBA::UnionMemberSeq &members);

      CORBA::object_reference<CORBA::TypeCode> create_enum_tc (
          const std::string &id,
          const std::string &name,
          const CORBA::EnumMemberSeq &members);

      CORBA::object_reference<CORBA::TypeCode> create_alias_tc (
          const std::string &id,
          const std::string &name,
          CORBA::object_reference<CORBA::TypeCode> original_type);

      CORBA::object_reference<CORBA::TypeCode> create_exception_tc (
          const std::string &id,
          const std::string &name,
          const CORBA::StructMemberSeq &members);

      CORBA::object_reference<CORBA::TypeCode> create_interface_tc (
          const std::string &id,
          const std::string &name);

      CORBA::object_reference<CORBA::TypeCode> create_string_tc (
          uint32_t bound);

      CORBA::object_reference<CORBA::TypeCode> create_wstring_tc (
          uint32_t bound);

      CORBA::object_reference<CORBA::TypeCode> create_fixed_tc (
          uint16_t digits,
          uint16_t scale);

      CORBA::object_reference<CORBA::TypeCode> create_sequence_tc (
          uint32_t bound,
          CORBA::object_reference<CORBA::TypeCode> element_type);

      CORBA::object_reference<CORBA::TypeCode> create_array_tc (
          uint32_t length,
          CORBA::object_reference<CORBA::TypeCode> element_type);

      CORBA::object_reference<CORBA::TypeCode> create_value_tc (
          const std::string &id,
          const std::string &name,
          CORBA::ValueModifier type_modifier,
          CORBA::object_reference<CORBA::TypeCode> concrete_base,
          const CORBA::ValueMemberSeq &members);

      CORBA::object_reference<CORBA::TypeCode> create_value_box_tc (
          const std::string &id,
          const std::string &name,
          CORBA::object_reference<CORBA::TypeCode> boxed_type);

      CORBA::object_reference<CORBA::TypeCode> create_native_tc (
          const std::string &id,
          const std::string &name);

      CORBA::object_reference<CORBA::TypeCode> create_recursive_tc (
          const std::string &id);

      CORBA::object_reference<CORBA::TypeCode> create_abstract_interface_tc (
          const std::string &id,
          const std::string &name);

      CORBA::object_reference<CORBA::TypeCode> create_local_interface_tc (
          const std::string &id,
          const std::string &name);

      CORBA::object_reference<CORBA::TypeCode> create_component_tc (
          const std::string &id,
          const std::string &name);

      CORBA::object_reference<CORBA::TypeCode> create_home_tc (
          const std::string &id,
          const std::string &name);

      CORBA::object_reference<CORBA::TypeCode> create_event_tc (
          const std::string &id,
          const std::string &name,
          CORBA::ValueModifier type_modifier,
          CORBA::object_reference<CORBA::TypeCode> concrete_base,
          const CORBA::ValueMemberSeq &members);
#endif

      ObjectIdList list_initial_services ();
      IDL::traits<CORBA::Object>::ref_type resolve_initial_references (
          const ObjectId& name);


      bool work_pending ();
      void perform_work ();
      void perform_work (ACE_Time_Value &tv);
      template< class Rep, class Period >
      inline void perform_work (std::chrono::duration<Rep, Period>& time)
      {
        ACE_Time_Value tv (time);
        this->perform_work (tv);
        time << tv;
      }
      void run ();
      void run (ACE_Time_Value &tv);
      template< class Rep, class Period >
      inline void run (std::chrono::duration<Rep, Period>& time)
      {
        ACE_Time_Value tv (time);
        this->run (tv);
        time << tv;
      }

      void destroy ();
      void shutdown (bool wait = false);

      /// Return the ACE reactor that is related to this ORB
      ACE_Reactor* reactor ();

#if !defined(CORBA_E_MICRO)
      IDL::traits<Policy>::ref_type create_policy (
          const PolicyType policy_type,
          const Any &val);

      // Value factory operations
      CORBA::ValueFactory
      register_value_factory (
            const std::string &repository_id,
            CORBA::ValueFactory factory);

      void
      unregister_value_factory (
          const std::string& repository_id);

      CORBA::ValueFactory
      lookup_value_factory (
          const std::string &repository_id);

      /// Register an object reference with the ORB.
      void register_initial_reference (
          const std::string& id,
          IDL::traits<CORBA::Object>::ref_type obj);

     /// Unregister an object reference with the ORB (TAO/TAOX11 specific).
      object_traits<Object>::ref_type unregister_initial_reference (
         const std::string& id);
#endif

      static AnyInsertAdapter *any_insert_adapter ();

#if !defined(CORBA_E_MICRO)
      static ValueTypeAdapter* value_type_adapter ();

# if !defined (CORBA_E_COMPACT)
      static IFRClientAdapter* ifr_client_adapter ();

      static DynamicAdapter* dynamic_adapter ();
# endif
#endif

      static object_traits<ORB>::ref_type init(
          int& argc,
          char* argv[],
          const std::string& orb_name = std::string ());

    protected:
      virtual ~ORB ();

#if !defined(CORBA_E_MICRO)
      ValueFactoryManagerAdapter *valuefactory_manager ();

      PolicyFactoryManagerAdapter *policy_manager ();

# if (TAO_HAS_MINIMUM_CORBA == 0) && !defined (CORBA_E_COMPACT)
      TypecodeFactoryAdapter *typecode_factory_adapter ();
# endif
#endif

      ORB_Proxy& proxy ();

    private:
      friend ORB_Registry;

      template <typename _Tp1, typename, typename ...Args>
      friend constexpr object_reference<_Tp1> make_reference(Args&& ...args);

      explicit ORB (ORB_Proxy_ptr op);

      ORB_Proxy_ref proxy_;

      std::shared_ptr<ORB_Registry> orb_registry_;
    }; // ORB

    TAOX11_Export IDL::traits<ORB>::ref_type ORB_init (
        int &argc,
        char * argv[],
        const std::string& orb_name = std::string ());

    inline ORB::InvalidName::InvalidName ()
      : CORBA::UserException ("IDL:omg.org/CORBA/ORB/InvalidName:1.0", "InvalidName")
    {
    }

  } // namespace CORBA

  namespace IDL
  {
    template<>
    struct traits < CORBA::ORB::InvalidName>
      : IDL::common_traits<CORBA::ORB::InvalidName>
    {
      template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
      static inline OStrm_& write_on(
          OStrm_& os_, in_type val_,
          Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };

    template <typename OStrm_>
    struct formatter< CORBA::ORB::InvalidName, OStrm_>
    {
      inline OStrm_& operator ()(
          OStrm_& os_,
          IDL::traits<CORBA::ORB::InvalidName>::in_type)
      {
        return os_ << "CORBA::ORB::InvalidName";
      }
    };

    template <typename OStrm_, typename Fmt>
    inline OStrm_& operator <<(
        OStrm_& os,
        IDL::traits<CORBA::ORB::InvalidName>::__Writer<Fmt> w)
    {
      using writer_t = IDL::traits<CORBA::ORB::InvalidName>::__Writer<Fmt>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<CORBA::ORB::InvalidName, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<CORBA::ORB::InvalidName>::write_on (
          os, w.val_,
          formatter_t ());
    }

    template <typename OStrm_>
    struct formatter< CORBA::ORB, OStrm_>
    {
      inline OStrm_& operator ()(
          OStrm_& os_,
          IDL::traits<CORBA::ORB>::in_type o_)
      {
        return os_ << "IDL::traits<CORBA::ORB>::ref_type {orb:"
            << o_->id ()<< "}";
      }
    };

    template <typename OStrm_, typename Fmt>
    inline OStrm_& operator <<(
        OStrm_& os,
        IDL::traits<CORBA::ORB>::__Writer<Fmt> w)
    {
      using writer_t = IDL::traits<CORBA::ORB>::__Writer<Fmt>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<CORBA::ORB, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<CORBA::ORB>::write_on (
          os, w.val_,
          formatter_t ());
    }
  } // namespace IDL
} // namespace TAOX11_NAMESPACE

#endif // TAOX11_ORB_H_INCLUDED
