/**
 * @file    abstractbase_traits_t.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 AbstractBase traits template
 *
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_ABSTRACTBASE_TRAITS_T_H_INCLUDED
#define TAOX11_ABSTRACTBASE_TRAITS_T_H_INCLUDED

#include /**/ "ace/config-all.h"

#include "tao/x11/stddef.h"
#include "tao/x11/system_exception.h"
#include "tao/x11/object_traits_t.h"
#include "tao/x11/valuetype/valuetype_traits_t.h"

namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {
    class AbstractBase;

    template <typename T>
    class abstractbase_reference;

    template <typename T>
    class weak_abstractbase_reference;

    template <typename T>
    struct abstractbase_traits
    {
      typedef T                               abs_type;
      typedef abstractbase_reference<T>       ref_type;
      typedef weak_abstractbase_reference<T>  weak_ref_type;

      static ref_type narrow (abstractbase_reference<CORBA::AbstractBase>);
    };

    template <typename T>
    class abstractbase_reference final
    {
    public:
      typedef T                       value_type;
      typedef T*                      ptr_type;
      typedef abstractbase_traits<T>  traits_type;

      abstractbase_reference (std::nullptr_t = nullptr)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      abstractbase_reference (const abstractbase_reference<_Tp1> &aref)
        : abs_ (aref.abs_)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      abstractbase_reference (abstractbase_reference<_Tp1> &&aref)
        : abs_ (std::move (aref.abs_))
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      abstractbase_reference (const object_reference<_Tp1> &oref)
        : abs_ (oref.stub_)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      abstractbase_reference (const valuetype_reference<_Tp1> &vtref)
        : abs_ (vtref.vtp_)
      {}

      ~abstractbase_reference () = default;

      inline T* operator-> ()
      {
        if (abs_ != nullptr) return abs_.get ();
        else throw TAOX11_NAMESPACE::CORBA::INV_OBJREF ();
      }
      inline const T* operator-> () const
      {
        if (abs_ != nullptr) return abs_.get ();
        else throw TAOX11_NAMESPACE::CORBA::INV_OBJREF ();
      }
      explicit operator bool() const // never throws
      { return abs_ == nullptr ? false : true; }
      bool operator ==(std::nullptr_t) const // never throws
      { return abs_ == nullptr ? true : false; }
      bool operator !=(std::nullptr_t) const // never throws
      { return abs_ == nullptr ? false : true; }

      void reset ()
      { this->abs_.reset (); }

      void swap (abstractbase_reference<T>& oref) noexcept(true)
      { this->abs_.swap (oref.abs_); }

      void operator=(std::nullptr_t)
      { this->abs_.reset (); }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator=(const abstractbase_reference<_Tp1>& aref)
      { this->abs_ = aref.abs_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator=(abstractbase_reference<_Tp1>&& aref)
      { this->abs_ = std::move (aref.abs_); }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator=(const object_reference<_Tp1>& oref)
      { this->abs_ = oref.stub_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator=(object_reference<_Tp1>&& oref)
      { this->abs_ = std::move (oref.stub_); }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator=(const valuetype_reference<_Tp1>& vtref)
      { this->abs_ = vtref.vtp_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator=(valuetype_reference<_Tp1>&& vtref)
      { this->abs_ = std::move (vtref.vtp_); }

      weak_abstractbase_reference<T> weak_reference () const
      { return weak_abstractbase_reference<T> (*this); }

    protected:
      typedef std::shared_ptr<T>    shared_ptr_type;

      template <typename _Tp1> friend class abstractbase_reference;
      template <typename _Tp1> friend struct abstractbase_traits;
      friend class weak_abstractbase_reference<T>;
      friend class AbstractBase;

      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      explicit abstractbase_reference (_Tp1* t)
        : abs_ (std::dynamic_pointer_cast<T> (t->_lock_shared ()))
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_base_of<_Tp1, T>::value>::type>
      explicit abstractbase_reference (const std::shared_ptr<_Tp1>& sp)
        : abs_ (std::dynamic_pointer_cast<T> (sp))
      {}
      explicit abstractbase_reference (const shared_ptr_type& sp)
        : abs_ (sp)
      {}

      template<typename _Tp1, typename = typename
        std::enable_if<std::is_base_of<typename _Tp1::value_type, T>::value>::type>
      static abstractbase_reference<T> _narrow(_Tp1 sp)
      { return abstractbase_reference<T> (std::dynamic_pointer_cast<T> (sp.abs_)); }

    private:
      shared_ptr_type abs_;
    };

    template <typename T>
    class weak_abstractbase_reference final
    {
    public:
      weak_abstractbase_reference ()
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      weak_abstractbase_reference (const weak_abstractbase_reference<_Tp1>& woref)
        : abs_ (woref.abs_)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      weak_abstractbase_reference (const abstractbase_reference<_Tp1>& oref)
        : abs_ (oref.abs_)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      weak_abstractbase_reference (const weak_object_reference<_Tp1>& woref)
        : abs_ (woref.stub_)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      weak_abstractbase_reference (const object_reference<_Tp1>& oref)
        : abs_ (oref.stub_)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      weak_abstractbase_reference (const weak_valuetype_reference<_Tp1>& wvref)
        : abs_ (wvref.vtp_)
      {}
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      weak_abstractbase_reference (const valuetype_reference<_Tp1>& vtref)
        : abs_ (vtref.vtp_)
      {}

      ~weak_abstractbase_reference () = default;

      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator= (const weak_abstractbase_reference<_Tp1>& woref)
      { this->abs_ = woref.abs_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator= (const abstractbase_reference<_Tp1>& oref)
      { this->abs_ = oref.abs_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator= (const weak_object_reference<_Tp1>& woref)
      { this->abs_ = woref.stub_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator= (const object_reference<_Tp1>& oref)
      { this->abs_ = oref.stub_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator= (const weak_valuetype_reference<_Tp1>& wvref)
      { this->abs_ = wvref.vtp_; }
      template<typename _Tp1, typename = typename
        std::enable_if<std::is_convertible<_Tp1*, T*>::value>::type>
      void operator= (const valuetype_reference<_Tp1>& vtref)
      { this->abs_ = vtref.vtp_; }

      bool expired () const
      { return this->abs_.expired (); }

      void reset ()
      { this->abs_.reset (); }

      void swap (weak_abstractbase_reference<T>& r) noexcept(true)
      { this->abs_.swap (r.abs_); }

      abstractbase_reference<T> lock () const
      { return abstractbase_reference<T> (this->abs_.lock ()); }

    protected:
      typedef std::weak_ptr<T>  weak_ptr_type;

    private:
      weak_ptr_type abs_;
    };

    template <typename T>
    inline typename abstractbase_traits<T>::ref_type
    abstractbase_traits<T>::narrow (
        abstractbase_reference<CORBA::AbstractBase>)
    {
      return nullptr;
    }
  } // namespace CORBA
} // namespace TAOX11_NAMESPACE

namespace std
{
  template <typename T>
  inline void swap (TAOX11_NAMESPACE::CORBA::abstractbase_reference<T>& l,
                    TAOX11_NAMESPACE::CORBA::abstractbase_reference<T>& r) noexcept(true)
  {
    l.swap (r);
  }

  template <typename T>
  inline void swap (TAOX11_NAMESPACE::CORBA::weak_abstractbase_reference<T>& l,
                    TAOX11_NAMESPACE::CORBA::weak_abstractbase_reference<T>& r) noexcept(true)
  {
    l.swap (r);
  }
} // namespace std

#endif // TAOX11_ABSTRACTBASE_TRAITS_T_H_INCLUDED
