/**
 * @file    valuetype_traits_t.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 ValueType traits template
 *
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_VALUETYPE_VALUETYPE_TRAITS_T_H_INCLUDED
#define TAOX11_VALUETYPE_VALUETYPE_TRAITS_T_H_INCLUDED

#include /**/ "ace/config-all.h"

#include "tao/x11/stddef.h"
#include "tao/x11/system_exception.h"
#include "tao/x11/valuetype/taox11_valuetype_export.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableServer
  {
    class Servant; //forward
  }

  namespace CORBA
  {
    template <typename T>
    class servant_reference; // forward
    template <typename T>
    class weak_servant_reference; // forward

    template <typename T>
    class abstractbase_reference; // forward
    template <typename T>
    class weak_abstractbase_reference; // forward

    class TAOX11_Valuetype_Export ValueBase;

    template <typename T>
    class valuetype_reference;

    template <typename T,
              typename = typename
      std::enable_if<std::is_base_of<typename
        std::conditional<std::is_base_of<PortableServer::Servant, T>::value, void, CORBA::ValueBase>::type, T>::value>::type,
              typename ...Args>
    constexpr valuetype_reference<T> make_reference(Args&& ...args);

    template <typename T>
    class weak_valuetype_reference;

    template <typename T>
    struct valuetype_traits
    {
      using ref_type = valuetype_reference<T>;
      using weak_ref_type = weak_valuetype_reference<T>;

      template <typename _Tp, typename = typename
          std::enable_if<std::is_base_of<ValueBase, typename _Tp::value_type>::value>::type>
      inline static ref_type narrow (_Tp vt)
      {
        return ref_type::_narrow (vt);
      }

      template <typename TInst = T, typename ...Args>
      inline static constexpr valuetype_reference<T> make_reference(Args&& ...args)
      {
        return TAOX11_CORBA::make_reference<TInst> (std::forward<Args> (args)...);
      }
    };

    template <typename T>
    class valuetype_reference final
    {
    public:
      using value_type = T;
      using ptr_type = T*;
      using traits_type = valuetype_traits<T>;

      valuetype_reference (std::nullptr_t = nullptr)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      valuetype_reference (const valuetype_reference<_Tp1>& vref)
        : vtp_ (vref.vtp_)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      valuetype_reference (valuetype_reference<_Tp1> &&vref)
        : vtp_ (std::move (vref.vtp_))
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      valuetype_reference (const servant_reference<_Tp1>& srvref)
        : vtp_ (srvref.srvt_)
      {}

      ~valuetype_reference () = default;

      inline T* operator-> ()
      {
        if (vtp_ != nullptr) return vtp_.get ();
        else throw TAOX11_NAMESPACE::CORBA::INV_OBJREF ();
      }
      inline const T* operator-> () const
      {
        if (vtp_ != nullptr) return vtp_.get ();
        else throw TAOX11_NAMESPACE::CORBA::INV_OBJREF ();
      }
      explicit operator bool() const // never throws
      { return vtp_ == nullptr ? false : true; }
      bool operator ==(std::nullptr_t) const // never throws
      { return vtp_ == nullptr ? true : false; }
      bool operator !=(std::nullptr_t) const // never throws
      { return vtp_ == nullptr ? false : true; }

      bool operator ==(valuetype_reference<T>& vref) const // never throws
      { return vtp_ == vref.vtp_ ? true : false; }
      bool operator !=(valuetype_reference<T>& vref) const // never throws
      { return vtp_ != vref.vtp_ ? true : false; }

      void reset ()
      { this->vtp_.reset (); }

      void swap (valuetype_reference<T>& vref) noexcept(true)
      { this->vtp_.swap (vref.vtp_); }

      void operator=(std::nullptr_t)
      { this->vtp_.reset (); }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator=(const valuetype_reference<_Tp1>& vref)
      { this->vtp_ = vref.vtp_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator=(valuetype_reference<_Tp1>&& vref)
      { this->vtp_ = std::move (vref.vtp_); }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator=(const servant_reference<_Tp1>& srvref)
      { this->vtp_ = srvref.srvt_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator=(servant_reference<_Tp1>&& srvref)
      { this->vtp_ = std::move (srvref.srvt_); }

      weak_valuetype_reference<T> weak_reference () const
      { return weak_valuetype_reference<T> (*this); }

    protected:
      using shared_ptr_type = std::shared_ptr<T>;

      template <typename _Tp1> friend class valuetype_reference;
      template <typename _Tp1> friend struct valuetype_traits;
      friend class weak_valuetype_reference<T>;
      template <typename _Tp1> friend class servant_reference;
      template <typename _Tp1> friend class weak_servant_reference;
      template <typename _Tp1> friend class abstractbase_reference;
      template <typename _Tp1> friend class weak_abstractbase_reference;
      template <typename _Tp1, typename, typename ...Args>
      friend constexpr valuetype_reference<_Tp1> make_reference(Args&& ...args);
      friend class ValueBase;

      template<typename _Tp1, typename = typename
        std::enable_if<std::is_base_of<_Tp1, T>::value>::type>
      explicit valuetype_reference (const std::shared_ptr<_Tp1>& sp)
        : vtp_ (std::dynamic_pointer_cast<T> (sp))
      {}
      template <typename _Tp1, typename = typename
          std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      explicit valuetype_reference (_Tp1* t)
        : vtp_ (std::dynamic_pointer_cast<T> (t->_lock_shared ()))
      {}
      explicit valuetype_reference (const shared_ptr_type& sp)
        : vtp_ (sp)
      {}

      template<typename _Tp1, typename = typename
        std::enable_if<std::is_base_of<typename _Tp1::value_type, T>::value>::type>
      static valuetype_reference<T> _narrow(_Tp1 vtref)
      { return valuetype_reference<T> (std::dynamic_pointer_cast<T> (vtref.vtp_)); }

    private:
      shared_ptr_type vtp_;
    };

    template <typename T>
    class weak_valuetype_reference final
    {
    public:
      weak_valuetype_reference () = default;
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      weak_valuetype_reference (const weak_valuetype_reference<_Tp1>& wref)
        : vtp_ (wref.vtp_)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      weak_valuetype_reference (const valuetype_reference<_Tp1>& ref)
        : vtp_ (ref.vtp_)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      weak_valuetype_reference (const weak_servant_reference<_Tp1>& wsref)
        : vtp_ (wsref.srvt_)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      weak_valuetype_reference (const servant_reference<_Tp1>& srvref)
        : vtp_ (srvref.srvt_)
      {}

      ~weak_valuetype_reference () = default;

      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator= (const weak_valuetype_reference<_Tp1>& wref)
      { this->vtp_ = wref.vtp_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator= (const valuetype_reference<_Tp1>& ref)
      { this->vtp_ = ref.vtp_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator= (const weak_servant_reference<_Tp1>& wsref)
      { this->vtp_ = wsref.srvt_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator= (const servant_reference<_Tp1>& srvref)
      { this->vtp_ = srvref.srvt_; }

      bool expired () const
      { return this->vtp_.expired (); }

      void reset ()
      { this->vtp_.reset (); }

      void swap (weak_valuetype_reference<T>& r) noexcept(true)
      { this->vtp_.swap (r.vtp_); }

      valuetype_reference<T> lock () const
      { return valuetype_reference<T> (this->vtp_.lock ()); }

    protected:
      using weak_ptr_type = std::weak_ptr<T>;

      template <typename _Tp1> friend class weak_servant_reference;
      template <typename _Tp1> friend class weak_abstractbase_reference;

    private:
      weak_ptr_type vtp_;
    };

    template <typename T, typename, typename ...Args>
    inline constexpr valuetype_reference<T> make_reference(Args&& ...args)
    {
      return valuetype_reference<T> (new T (std::forward<Args> (args)...));
    }
  } // namespace CORBA
} // namespace TAOX11_NAMESPACE

namespace std
{
  template <typename T>
  inline void swap (TAOX11_NAMESPACE::CORBA::valuetype_reference<T>& l,
                    TAOX11_NAMESPACE::CORBA::valuetype_reference<T>& r) noexcept(true)
  {
    l.swap (r);
  }

  template <typename T>
  inline void swap (TAOX11_NAMESPACE::CORBA::weak_valuetype_reference<T>& l,
                    TAOX11_NAMESPACE::CORBA::weak_valuetype_reference<T>& r) noexcept(true)
  {
    l.swap (r);
  }
} // namespace std

#endif // TAOX11_VALUETYPE_VALUETYPE_TRAITS_T_H_INCLUDED
