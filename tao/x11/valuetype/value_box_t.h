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
      using _traits_type = CORBA::valuetype_traits<Valuebox_T<T>>;
      using _ref_type = CORBA::valuetype_reference<Valuebox_T<T>>;

      void swap(Valuebox_T<T>& r)
      {
        std::swap (this->value_, r.value_);
      }

    protected:
      using _shared_ptr_type = std::shared_ptr<Valuebox_T<T>>;

      Valuebox_T () = default;
      Valuebox_T (const Valuebox_T<T>&) = default;
      Valuebox_T (Valuebox_T<T>&&) = default;
      Valuebox_T (T v)
        : value_ (std::move(v))
      {}
      ~Valuebox_T () = default;
      Valuebox_T<T>& operator =(const Valuebox_T<T>&) = delete;
      Valuebox_T<T>& operator =(Valuebox_T<T>&&) = delete;

      _ref_type _lock ()
      { return _traits_type::narrow (this->ValueBase::_lock ()); }

      void _obv_get_truncatable_repo_ids (std::vector<std::string const*>& _ids) const override
      { _ids.push_back (std::addressof(this->_obv_repository_id ())); }

      bool _obv_match_formal_type (ptrdiff_t ) const override
      { return true; }

      bool _obv_unmarshal_v (TAO_InputCDR &) override
      { return true; }

      T value_;
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
