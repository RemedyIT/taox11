/**
 * @file    servant_traits_t.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 servant traits templates
 *
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_SERVANT_TRAITS_T_H_INCLUDED
#define TAOX11_SERVANT_TRAITS_T_H_INCLUDED

#include /**/ "ace/config-all.h"

#include "tao/x11/stddef.h"
#include "tao/x11/system_exception.h"

namespace TAOX11_NAMESPACE
{
  namespace PortableServer
  {
    class Servant; //forward
  }

  namespace CORBA
  {
    template <typename T>
    class valuetype_reference; //forward
    template <typename T>
    class weak_valuetype_reference; //forward

    template <typename T>
    class servant_reference;

    template <typename T, typename = typename
      std::enable_if<std::is_base_of<PortableServer::Servant, T>::value>::type, typename ...Args>
    constexpr servant_reference<T> make_reference(Args&& ...args);

    template <typename T>
    class weak_servant_reference;

    template <typename T>
    struct servant_traits
    {
      using ref_type = servant_reference<T>;
      using weak_ref_type = weak_servant_reference<T>;

      template <typename _Tp, typename = typename
          std::enable_if<std::is_base_of<PortableServer::Servant, typename _Tp::value_type>::value>::type>
      inline static ref_type narrow (_Tp vt)
      {
        return ref_type::_narrow (std::move(vt));
      }
    };

    template <typename T>
    class servant_reference final
    {
    public:
      using value_type = T;
      using ptr_type =  T*;
      using shared_ptr_type = std::shared_ptr<T>;
      using traits_type = servant_traits<T>;

      servant_reference (std::nullptr_t = nullptr)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      servant_reference (const servant_reference<_Tp1>& sref)
        : srvt_ (sref.srvt_)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      servant_reference (servant_reference<_Tp1> &&sref)
        : srvt_ (std::move (sref.srvt_))
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      servant_reference (const valuetype_reference<_Tp1>& vtref)
        : srvt_ (vtref.vtp_)
      {}

      inline T* operator-> ()
      {
        if (srvt_ != nullptr) return srvt_.get ();
        else throw TAOX11_NAMESPACE::CORBA::INV_OBJREF ();
      }
      inline const T* operator-> () const
      {
        if (srvt_ != nullptr) return srvt_.get ();
        else throw TAOX11_NAMESPACE::CORBA::INV_OBJREF ();
      }
      explicit operator bool() const // never throws
      { return srvt_ == nullptr ? false : true; }
      bool operator ==(std::nullptr_t) const // never throws
      { return srvt_ == nullptr ? true : false; }
      bool operator !=(std::nullptr_t) const // never throws
      { return srvt_ == nullptr ? false : true; }

      void reset ()
      { this->srvt_.reset (); }

      void swap (servant_reference<T>& sr) noexcept(true)
      { this->srvt_.swap (sr.srvt_); }

      void operator=(std::nullptr_t)
      { this->srvt_.reset (); }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator=(const servant_reference<_Tp1>& sref)
      { this->srvt_ = sref.srvt_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator=(servant_reference<_Tp1>&& sref)
      { this->srvt_ = std::move (sref.srvt_); }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator=(const valuetype_reference<_Tp1>& vtref)
      { this->srvt_ = vtref.vtp_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator=(valuetype_reference<_Tp1>&& vtref)
      { this->srvt_ = std::move (vtref.vtp_); }

      weak_servant_reference<T> weak_reference () const
      { return weak_servant_reference<T> (*this); }

    protected:
      template <typename _Tp1> friend class servant_reference;
      template <typename _Tp1> friend struct servant_traits;
      friend class weak_servant_reference<T>;
      template <typename _Tp1> friend class valuetype_reference;
      template <typename _Tp1> friend class weak_valuetype_reference;
      template <typename _Tp1, typename, typename ...Args>
      friend constexpr servant_reference<_Tp1> make_reference(Args&& ...args);
      friend class PortableServer::Servant;

      template <typename _Tp1>
      explicit servant_reference (_Tp1* t)
        : srvt_ (std::dynamic_pointer_cast<T> (t->_lock_shared ()))
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_base_of<_Tp1, T>::value>::type>
      explicit servant_reference (const std::shared_ptr<_Tp1>& sp)
        : srvt_ (std::dynamic_pointer_cast<T> (sp))
      {}
       explicit servant_reference (const shared_ptr_type& sp)
        : srvt_ (sp)
      {}

      template<typename _Tp1, typename = typename
        std::enable_if<std::is_base_of<typename _Tp1::value_type, T>::value>::type>
      static servant_reference<T> _narrow(_Tp1 sp)
      { return servant_reference<T> (std::dynamic_pointer_cast<T> (sp.srvt_)); }

    private:
      shared_ptr_type srvt_;
    };

    template <typename T>
    class weak_servant_reference final
    {
    public:
      weak_servant_reference () = default;
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      weak_servant_reference (const weak_servant_reference<_Tp1>& woref)
        : srvt_ (woref.srvt_)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      weak_servant_reference (weak_servant_reference<_Tp1>&& woref)
        : srvt_ (std::move(woref.srvt_))
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      weak_servant_reference (const servant_reference<_Tp1>& oref)
        : srvt_ (oref.srvt_)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      weak_servant_reference (const weak_valuetype_reference<_Tp1>& wvref)
        : srvt_ (wvref.vtp_)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      weak_servant_reference (const valuetype_reference<_Tp1>& vtref)
        : srvt_ (vtref.vtp_)
      {}

      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator= (const weak_servant_reference<_Tp1>& woref)
      { this->srvt_ = woref.srvt_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator= (weak_servant_reference<_Tp1>&& woref)
      { this->srvt_ = std::move(woref.srvt_); }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator= (const servant_reference<_Tp1>& oref)
      { this->srvt_ = oref.srvt_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator= (const weak_valuetype_reference<_Tp1>& wvref)
      { this->srvt_ = wvref.vtp_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator= (const valuetype_reference<_Tp1>& vtref)
      { this->srvt_ = vtref.vtp_; }

      bool expired () const
      { return this->srvt_.expired (); }

      void reset ()
      { this->srvt_.reset (); }

      void swap (weak_servant_reference<T>& wsr) noexcept(true)
      { this->srvt_.swap (wsr.srvt_); }

      servant_reference<T> lock () const
      { return servant_reference<T> (this->srvt_.lock ()); }

    protected:
      using weak_ptr_type = std::weak_ptr<T>;

      template <typename _Tp1> friend class weak_valuetype_reference;

    private:
      weak_ptr_type srvt_;
    };

    template <typename T, typename, typename ...Args>
    inline constexpr servant_reference<T> make_reference(Args&& ...args)
    {
      return servant_reference<T> (new T (std::forward<Args> (args)...));
    }
  } // namespace CORBA

} // namespace TAOX11_NAMESPACE

namespace std
{
  template <typename T>
  inline void swap (TAOX11_NAMESPACE::CORBA::servant_reference<T>& l,
                    TAOX11_NAMESPACE::CORBA::servant_reference<T>& r) noexcept(true)
  {
    l.swap (r);
  }

  template <typename T>
  inline void swap (TAOX11_NAMESPACE::CORBA::weak_servant_reference<T>& l,
                    TAOX11_NAMESPACE::CORBA::weak_servant_reference<T>& r) noexcept(true)
  {
    l.swap (r);
  }
} // namespace std

#endif // TAOX11_SERVANT_TRAITS_T_H_INCLUDED

