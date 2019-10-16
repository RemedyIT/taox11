/**
 * @file    value_box_t.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 Valuebox template class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_VALUETYPE_VALUE_BOX_T_H_INCLUDED
#define TAOX11_VALUETYPE_VALUE_BOX_T_H_INCLUDED

#pragma once

#include /**/ "ace/config-all.h"

#include "tao/x11/valuetype/value_base.h"

namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {

    template <typename T>
    class Valuebox_T : public ValueBase
    {
    public:
      typedef CORBA::valuetype_traits<Valuebox_T<T>>     _traits_type;
      typedef CORBA::valuetype_reference<Valuebox_T<T>>  _ref_type;

      void swap(Valuebox_T<T>& r)
      {
        std::swap (this->value_, r.value_);
      }

    protected:
      typedef std::shared_ptr<Valuebox_T<T>>             _shared_ptr_type;

      Valuebox_T () = default;
      Valuebox_T (const Valuebox_T<T>&) = default;
      Valuebox_T (Valuebox_T<T>&&) = default;
      Valuebox_T (T v)
        : value_ (std::move(v))
      {}
      ~Valuebox_T () = default;

      _ref_type _lock ()
      { return _traits_type::narrow (this->ValueBase::_lock ()); }

      virtual void _obv_get_truncatable_repo_ids (std::vector<std::string const*>& _ids) const
      { _ids.push_back (std::addressof(this->_obv_repository_id ())); }

      virtual bool _obv_match_formal_type (ptrdiff_t ) const
      { return true; }

      virtual bool _obv_unmarshal_v (TAO_InputCDR &)
      { return true; }

      T value_;
    private:
      Valuebox_T<T>& operator =(const Valuebox_T<T>&) = delete;
      Valuebox_T<T>& operator =(Valuebox_T<T>&&) = delete;
    };
  } // namespace CORBA

} // namespace TAOX11_NAMESPACE

namespace std
{
  template <typename T>
  void swap (TAOX11_NAMESPACE::CORBA::Valuebox_T<T>& l,
             TAOX11_NAMESPACE::CORBA::Valuebox_T<T>& r)
  {
    l.swap (r);
  }
} // namespace std

#endif // TAOX11_VALUETYPE_VALUE_BOX_T_H_INCLUDED
