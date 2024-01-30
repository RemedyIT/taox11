/**
 * @file    object_traits_t.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 Object traits template
 *
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_OBJECT_TRAITS_T_H_INCLUDED
#define TAOX11_OBJECT_TRAITS_T_H_INCLUDED

#include /**/ "ace/config-all.h"

#include "tao/x11/stddef.h"
#include "tao/x11/system_exception.h"

namespace TAOX11_NAMESPACE
{
  class Object_proxy;

  namespace CORBA
  {
    template <typename T>
    class abstractbase_reference; // forward
    template <typename T>
    class weak_abstractbase_reference; // forward

    class TAOX11_Export Object;

    template <typename T>
    class object_reference;

    template <typename T, typename = typename
      std::enable_if<std::is_base_of<CORBA::Object, T>::value>::type, typename ...Args>
    constexpr object_reference<T> make_reference(Args&& ...args);

    template <typename T>
    class weak_object_reference;

    template <typename T>
    struct object_traits
    {
      using stub_type =  T;
      using ref_type = object_reference<T>;
      using weak_ref_type = weak_object_reference<T>;

      static ref_type narrow (object_reference<Object> obj);

      using shared_ptr_type = std::shared_ptr<T>;

      static shared_ptr_type lock_shared (stub_type*);

      template <typename TInst,
          typename = typename
            std::enable_if<std::is_base_of<T, TInst>::value>::type,
          typename ...Args>
      static inline constexpr object_reference<T> make_reference(Args&& ...args)
      {
        return TAOX11_CORBA::make_reference<TInst>(std::forward<Args> (args)...);
      }
    };

    template <typename T>
    class object_reference final
    {
    public:
      using value_type = T;
      using ptr_type =  T*;
      using traits_type = object_traits<T>;

      object_reference (std::nullptr_t = nullptr)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<typename _Tp1::ptr_type, T*>::value>::type>
      object_reference (const _Tp1& oref)
        : stub_ (oref.stub_)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<typename _Tp1::ptr_type, T*>::value>::type>
      object_reference (_Tp1&& oref)
        : stub_ (std::move (oref.stub_))
      {}

      ~object_reference () = default;

      inline T* operator-> ()
      {
        if (stub_ != nullptr) return stub_.get ();
        else throw TAOX11_NAMESPACE::CORBA::INV_OBJREF ();
      }
      inline const T* operator-> () const
      {
        if (stub_ != nullptr) return stub_.get ();
        else throw TAOX11_NAMESPACE::CORBA::INV_OBJREF ();
      }
      explicit operator bool() const // never throws
      { return stub_ == nullptr ? false : true; }
      bool operator ==(std::nullptr_t) const // never throws
      { return stub_ == nullptr ? true : false; }
      bool operator !=(std::nullptr_t) const // never throws
      { return stub_ == nullptr ? false : true; }

      void reset ()
      { this->stub_.reset (); }

      void swap (object_reference<T>& oref) noexcept(true)
      { this->stub_.swap (oref.stub_); }

      void operator=(std::nullptr_t)
      { this->stub_.reset (); }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<typename _Tp1::ptr_type, T*>::value>::type>
      void operator=(const _Tp1& oref)
      { this->stub_ = oref.stub_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<typename _Tp1::ptr_type, T*>::value>::type>
      void operator=(_Tp1&& oref)
      { this->stub_ = std::move (oref.stub_); }

      weak_object_reference<T> weak_reference () const
      { return weak_object_reference<T> (*this); }

    protected:
      using shared_ptr_type = std::shared_ptr<T>;

      template <typename _Tp1> friend class object_reference;
      template <typename _Tp1> friend struct object_traits;
      friend class weak_object_reference<T>;
      template <typename _Tp1> friend class abstractbase_reference;
      template <typename _Tp1> friend class weak_abstractbase_reference;
      template <typename _Tp1, typename, typename ...Args>
      friend constexpr object_reference<_Tp1> make_reference(Args&& ...args);
      friend class Object;

      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      explicit object_reference (_Tp1* t)
        : stub_ (object_traits<T>::lock_shared (t))
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_base_of<_Tp1, T>::value>::type>
      explicit object_reference (const std::shared_ptr<_Tp1>& sp)
        : stub_ (std::dynamic_pointer_cast<T> (sp))
      {}
      explicit object_reference (const shared_ptr_type& sp)
        : stub_ (sp)
      {}

      template<typename _Tp1, typename = typename
        std::enable_if<std::is_base_of<typename _Tp1::value_type, T>::value>::type>
      static object_reference<T> _narrow(_Tp1 sp)
      { return object_reference<T> (std::dynamic_pointer_cast<T> (sp.stub_)); }

    private:
      shared_ptr_type stub_;
    };

    template <typename T>
    class weak_object_reference final
    {
    public:
      weak_object_reference () = default;
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      weak_object_reference (const weak_object_reference<_Tp1>& woref)
        : stub_ (woref.stub_)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      weak_object_reference (weak_object_reference<_Tp1>&& woref)
        : stub_ (std::move(woref.stub_))
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      weak_object_reference (const object_reference<_Tp1>& oref)
        : stub_ (oref.stub_)
      {}

      ~weak_object_reference () = default;

      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator= (const weak_object_reference<_Tp1>& woref)
      { this->stub_ = woref.stub_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator= (weak_object_reference<_Tp1>&& woref)
      { this->stub_ = std::move(woref.stub_); }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator= (const object_reference<_Tp1>& oref)
      { this->stub_ = oref.stub_; }

      bool expired () const
      { return this->stub_.expired (); }

      void reset ()
      { this->stub_.reset (); }

      void swap (weak_object_reference<T>& r) noexcept(true)
      { this->stub_.swap (r.stub_); }

      object_reference<T> lock () const
      { return object_reference<T> (this->stub_.lock ()); }

    protected:
      using weak_ptr_type = std::weak_ptr<T>;

      template <typename _Tp1> friend class weak_abstractbase_reference;

    private:
      weak_ptr_type stub_;
    };

    /**
     * Assume default behavior of Object classes.
     * Specializations will take care of the problems with
     * incomplete types because of IDL forward declarations.
     */
    template <typename T>
    inline typename object_traits<T>::shared_ptr_type
    object_traits<T>::lock_shared (stub_type* p)
    {
      return std::dynamic_pointer_cast<T> (p->_lock_shared ());
    }

    /**
     * By default just try to dynamically cast the shared_ptr.
     * Specializations can/will improve on this for remote object refs
     * and special local objects.
     */
    template <typename T>
    inline typename object_traits<T>::ref_type
    object_traits<T>::narrow (object_reference<Object> obj)
    {
      return ref_type (std::dynamic_pointer_cast<T> (obj.stub_));
    }

    template <typename T, typename, typename ...Args>
    inline constexpr object_reference<T> make_reference(Args&& ...args)
    {
      return object_reference<T> (new T (std::forward<Args> (args)...));
    }
  } // namespace CORBA

} // namespace TAOX11_NAMESPACE

namespace std
{
  template <typename T>
  inline void swap (TAOX11_NAMESPACE::CORBA::object_reference<T>& l,
                    TAOX11_NAMESPACE::CORBA::object_reference<T>& r) noexcept(true)
  {
    l.swap (r);
  }

  template <typename T>
  inline void swap (TAOX11_NAMESPACE::CORBA::weak_object_reference<T>& l,
                    TAOX11_NAMESPACE::CORBA::weak_object_reference<T>& r) noexcept(true)
  {
    l.swap (r);
  }
} // namespace std

#endif // TAOX11_OBJECT_TRAITS_T_H_INCLUDED
