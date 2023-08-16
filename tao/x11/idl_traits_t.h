/**
 * @file    idl_traits_t.h
 * @author  Martin Corino
 *
 * @brief   Traits template for IDL generated types
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_IDL_TRAITS_T_H_INCLUDED
#define TAOX11_IDL_TRAITS_T_H_INCLUDED

#include "tao/x11/stddef.h"
#include "tao/x11/bounded_vector_t.h"
#include "tao/x11/bounded_map_t.h"

namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {
    class LocalObject; // forward
    template <typename T>
    struct object_traits; // forward
    template <typename T>
    class object_reference; // forward
    template <typename T>
    class abstractbase_reference; // forward
    class ValueBase; // forward
    template <typename T>
    class valuetype_reference; // forward
    template <typename T>
    struct valuetype_traits; // forward
  } // namespace CORBA

  namespace IDL
  {
    template <typename T>
    struct traits
    {
      using _traits_type = typename std::conditional<
                          std::is_base_of<CORBA::LocalObject, T>::value,
                          CORBA::object_traits<T>,
                          typename std::conditional<
                                  std::is_base_of<CORBA::ValueBase, T>::value,
                                  CORBA::valuetype_traits<T>,
                                  void>::type>::type;

      using ref_type = typename _traits_type::ref_type;
      using weak_ref_type = typename _traits_type::weak_ref_type;

      template <typename _Tp>
      inline static ref_type narrow (_Tp obj)
      {
        return _traits_type::narrow (obj);
      }
    };

    template <typename T>
    struct common_traits
    {
      /// common traits
      using value_type = T;
      using in_type = const T&;
      using out_type = T&;
      using inout_type = T&;

      template <typename Formatter>
      struct __Writer { using formatter_t = Formatter; in_type val_; };
    };

    template <typename T>
    struct common_byval_traits
    {
      /// common traits
      using value_type = T;
      using in_type = T;
      using out_type = T&;
      using inout_type = T&;

      template <typename Formatter>
      struct __Writer { using formatter_t = Formatter ; in_type val_; };
    };

    template <typename T, typename OStrm_>
    struct formatter
    {
      inline OStrm_& operator ()(
          OStrm_& os_,
          typename IDL::traits<T>::in_type val_)
      { os_ << val_; return os_; }
    };

    template <typename OStrm_>
    struct formatter<bool, OStrm_>
    {
      inline OStrm_& operator ()(
          OStrm_& os_,
          bool val_)
      { os_ << (val_ ? "true" : "false"); return os_; }
    };

    template <>
    struct formatter<bool, std::basic_ostream<wchar_t>>
    {
      inline std::basic_ostream<wchar_t>& operator ()(
          std::basic_ostream<wchar_t>& os_,
          bool val_)
      { return os_ << (val_ ? L"true" : L"false"); }
    };

    template <typename OStrm_>
    struct formatter<int8_t, OStrm_>
    {
      inline OStrm_& operator ()(OStrm_& os_, int8_t val_)
      { os_ << std::hex << static_cast<int16_t> (val_) << std::dec; return os_;}
    };

    template <typename OStrm_>
    struct formatter<uint8_t, OStrm_>
    {
      inline OStrm_& operator ()(OStrm_& os_, uint8_t val_)
      { os_ << std::hex << static_cast<uint16_t> (val_) << std::dec; return os_;}
    };

    template <typename OStrm_>
    struct formatter<char, OStrm_>
    {
      inline OStrm_& operator ()(OStrm_& os_, char val_)
      { os_ << '\'' << val_ << '\''; return os_; }
    };

    template <>
    struct formatter<char, std::basic_ostream<wchar_t>>
    {
      inline std::basic_ostream<wchar_t>& operator ()(std::basic_ostream<wchar_t>& os_, char val_)
      { return os_ << L'\''
            << std::use_facet<std::ctype<wchar_t>> (os_.getloc ()).widen (val_)
            << L'\'';
      }
    };

    template <typename OStrm_>
    struct formatter<wchar_t, OStrm_>
    {
      inline OStrm_& operator ()(OStrm_& os_, wchar_t val_)
      { os_ << L'\'' << val_ << L'\''; return os_; }
    };

    template <>
    struct formatter<wchar_t, std::basic_ostream<char>>
    {
      inline std::basic_ostream<char>& operator ()(std::basic_ostream<char>& os_, wchar_t val_)
      { return os_ << "L'"
                   << std::use_facet<std::ctype<wchar_t>> (os_.getloc ()).narrow (val_, '?')
                   << '\'';
      }
    };

    template <typename OStrm_>
    struct formatter<std::string, OStrm_>
    {
      inline OStrm_& operator ()(OStrm_& os_, std::string val_)
      { os_ << '"' << val_ << '"'; return os_; }
    };

    template <>
    struct formatter<std::string, std::basic_ostream<wchar_t>>
    {
      inline std::basic_ostream<wchar_t>& operator ()(std::basic_ostream<wchar_t>& os_, std::string val_)
      { return os_ << '"' << ACE_Ascii_To_Wide (val_.c_str ()).wchar_rep () << '"'; }
    };

    template <typename OStrm_>
    struct formatter<std::wstring, OStrm_>
    {
      inline OStrm_& operator ()(OStrm_& os_, std::string val_)
      { os_ << L'"' << val_ << '"'; return os_; }
    };

    template <>
    struct formatter<std::wstring, std::basic_ostream<char>>
    {
      inline std::basic_ostream<char>& operator ()(std::basic_ostream<char>& os_, std::wstring val_)
      { return os_ << "L\"" << ACE_Wide_To_Ascii (val_.c_str ()).char_rep () << '"'; }
    };

    template <typename T, typename OStrm_>
    struct formatter<std::vector<T>, OStrm_>
    {
      using traits = IDL::traits<std::vector<T>>;
      using elem_traits = typename traits::element_traits;

      template <typename T_ELEM>
      struct element_formatter
      {
        OStrm_& os_;
        element_formatter (OStrm_& os)
          : os_ (os) {}

        inline OStrm_& operator ()(typename elem_traits::in_type el_)
        { elem_traits::write_on (this->os_, el_) << ','; return os_;}
      };

      inline OStrm_& operator ()(OStrm_& os_, typename traits::in_type val_)
      {
        if (val_.empty ())
        {
          os_ << "<>";
        }
        else
        {
          os_ << '<';
          std::for_each (val_.cbegin (),
                         --val_.cend (),
                         element_formatter<typename elem_traits::value_type> (os_));
          elem_traits::write_on (os_, val_.back ());
          os_ << '>';
        }
        return os_;
      }
    };

    template <typename T, typename OStrm_, const uint32_t B>
    struct formatter<IDL::bounded_vector<T, B>, OStrm_>
    {
      using traits = IDL::traits<IDL::bounded_vector<T, B>>;
      using elem_traits = typename traits::element_traits;

      template <typename T_ELEM>
      struct element_formatter
      {
        OStrm_& os_;
        element_formatter (OStrm_& os)
          : os_ (os) {}

        inline OStrm_& operator ()(typename elem_traits::in_type el_)
        { elem_traits::write_on (this->os_, el_) << ','; return os_; }
      };

      inline OStrm_& operator ()(OStrm_& os_, typename traits::in_type val_)
      {
        if (val_.empty ())
        {
          os_ << "<>";
        }
        else
        {
          os_ << '<';
          std::for_each (val_.cbegin (),
                         --val_.cend (),
                         element_formatter<typename elem_traits::value_type> (os_));
          elem_traits::write_on (os_, val_.back ());
          os_ << '>';
        }
        return os_;
      }
    };

    template <typename X, typename Y, typename OStrm_>
    struct formatter<std::map<X,Y>, OStrm_>
    {
      using traits = IDL::traits<std::map<X,Y>>;
      using elem_traits = typename std::map<X,Y>::value_type;
      using key_traits = IDL::traits<X>;
      using value_traits = IDL::traits<Y>;

      template <typename T_ELEM>
      struct element_formatter
      {
        OStrm_& os_;
        element_formatter (OStrm_& os)
          : os_ (os) {}

        inline OStrm_& operator ()(elem_traits el_)
        { this->os_ << "{"; key_traits::write_on (this->os_, el_.first) << ','; return value_traits::write_on (this->os_, el_.second) << "},"; }
      };

      inline OStrm_& operator ()(OStrm_& os_, typename traits::in_type val_)
      {
        if (val_.empty ())
        {
          os_ << "{}";
        }
        else
        {
          os_ << '{';
          std::for_each (val_.cbegin (),
                         --val_.cend (),
                         element_formatter<elem_traits> (os_));
          os_ << "{";
          key_traits::write_on (os_, (--val_.cend ())->first) << ",";
          value_traits::write_on (os_, (--val_.cend ())->second);
          os_ << "}}";
        }
        return os_;
      }
    };

    template <typename X, typename Y, typename OStrm_, const uint32_t B>
    struct formatter<IDL::bounded_map<X, Y, B>, OStrm_>
    {
      using traits = IDL::traits<IDL::bounded_map<X, Y, B>>;
      using elem_traits = typename IDL::bounded_map<X,Y,B>::value_type;
      using key_traits = IDL::traits<X>;
      using value_traits = IDL::traits<Y>;

      template <typename T_ELEM>
      struct element_formatter
      {
        OStrm_& os_;
        element_formatter (OStrm_& os)
          : os_ (os) {}

        inline OStrm_& operator ()(elem_traits el_)
        { this->os_ << "{"; return key_traits::write_on (this->os_, el_.first) << ','; return value_traits::write_on (this->os_, el_.second) << "},"; }
      };

      inline OStrm_& operator ()(OStrm_& os_, typename traits::in_type val_)
      {
        if (val_.empty ())
        {
          os_ << "{}";
        }
        else
        {
          os_ << '{';
          std::for_each (val_.cbegin (),
                         --val_.cend (),
                         element_formatter<elem_traits> (os_));
          os_ << "{";
          key_traits::write_on (os_, (--val_.cend ())->first) << ",";
          value_traits::write_on (os_, (--val_.cend ())->second);
          os_ << "}}";
        }
        return os_;
      }
    };

    template <typename T>
    struct __value_type
    { using value_type = T; };
    template <typename T>
    struct __value_type<CORBA::object_reference<T>>
    { using value_type = typename CORBA::object_reference<T>:: value_type; };
    template <typename T>
    struct __value_type<CORBA::abstractbase_reference<T>>
    { using value_type = typename CORBA::abstractbase_reference<T>:: value_type; };
    template <typename T>
    struct __value_type<CORBA::valuetype_reference<T>>
    { using value_type = typename CORBA::valuetype_reference<T>:: value_type; };

    template <typename T, typename OStrm_, std::size_t N>
    struct formatter<std::array<T, N>, OStrm_>
    {
      using elem_formatter = formatter<typename __value_type<T>::value_type, OStrm_>;

      struct element_formatter
      {
        OStrm_& os_;
        element_formatter (OStrm_& os)
          : os_ (os) {}

        inline OStrm_& operator ()(const T& el_)
        { elem_formatter () (this->os_, el_) << ','; return os_; }
      };

      inline OStrm_& operator ()(OStrm_& os_, const std::array<T, N>& val_)
      {
        if (val_.empty ())
        {
          os_ << "[]";
        }
        else
        {
          os_ << '[';
          std::for_each (val_.cbegin (),
                         val_.cend () - 1,
                         element_formatter (os_));
          elem_formatter () (os_, val_.back());
          os_ << ']';
        }
        return os_;
      }
    };
  } // namespace IDL
} // namespace TAOX11_NAMESPACE

#endif // TAOX11_IDL_TRAITS_T_H_INCLUDED
