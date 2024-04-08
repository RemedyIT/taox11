/**
 * @file    basic_traits.h
 * @author  Martin Corino
 *
 * @brief   Traits for basic types
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_BASIC_TRAITS_H_INCLUDED
#define TAOX11_BASIC_TRAITS_H_INCLUDED

#include "tao/x11/idl_traits_t.h"

namespace TAOX11_NAMESPACE
{
  namespace IDL
  {
    /// type traits for basic types

    template<>
    struct traits <bool> : public common_byval_traits<bool>
    {
      template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
      static inline OStrm_& write_on(OStrm_& os_, in_type val_, Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };
    template<>
    struct traits <char> : public common_byval_traits<char>
    {
      template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
      static inline OStrm_& write_on(OStrm_& os_, in_type val_, Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };
    template<>
    struct traits <wchar_t> : public common_byval_traits<wchar_t>
    {
      template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
      static inline OStrm_& write_on(OStrm_& os_, in_type val_, Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };
    template<>
    struct traits <int8_t> : public common_byval_traits<int8_t>
    {
      template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
      static inline OStrm_& write_on(OStrm_& os_, in_type val_, Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };
    template<>
    struct traits <uint8_t> : public common_byval_traits<uint8_t>
    {
      template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
      static inline OStrm_& write_on(OStrm_& os_, in_type val_, Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };
    template<>
    struct traits <int16_t> : public common_byval_traits<int16_t>
    {
      template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
      static inline OStrm_& write_on(OStrm_& os_, in_type val_, Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };
    template<>
    struct traits <uint16_t> : public common_byval_traits<uint16_t>
    {
      template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
      static inline OStrm_& write_on(OStrm_& os_, in_type val_, Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };
    template<>
    struct traits <int32_t> : public common_byval_traits<int32_t>
    {
      template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
      static inline OStrm_& write_on(OStrm_& os_, in_type val_, Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };
    template<>
    struct traits <uint32_t> : public common_byval_traits<uint32_t>
    {
      template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
      static inline OStrm_& write_on(OStrm_& os_, in_type val_, Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };
    template<>
    struct traits <int64_t> : public common_byval_traits<int64_t>
    {
      template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
      static inline OStrm_& write_on(OStrm_& os_, in_type val_, Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };
    template<>
    struct traits <uint64_t> : public common_byval_traits<uint64_t>
    {
      template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
      static inline OStrm_& write_on(OStrm_& os_, in_type val_, Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };
    template<>
    struct traits <float> : public common_byval_traits<float>
    {
      template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
      static inline OStrm_& write_on(OStrm_& os_, in_type val_, Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };
    template<>
    struct traits <double> : public common_byval_traits<double>
    {
      template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
      static inline OStrm_& write_on(OStrm_& os_, in_type val_, Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };
    template<>
    struct traits <long double> : public common_byval_traits<long double>
    {
      template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
      static inline OStrm_& write_on(OStrm_& os_, in_type val_, Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };
    template<>
    struct traits <std::string>
      : public common_traits<std::string>
    {
      using element_traits = IDL::traits<char>;
      using is_bounded = std::false_type;

      template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
      static inline OStrm_& write_on(OStrm_& os_, in_type val_, Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };
    template<>
    struct traits <std::wstring>
      : public common_traits<std::wstring>
    {
      using element_traits = IDL::traits<wchar_t>;
      using is_bounded = std::false_type;

      template <typename OStrm_, typename Formatter = formatter<value_type, OStrm_>>
      static inline OStrm_& write_on(OStrm_& os_, in_type val_, Formatter fmt_ = Formatter ())
      {
        return fmt_ (os_, val_);
      }

      template <typename Formatter = std::false_type>
      static inline __Writer<Formatter> write (in_type val) { return {val} ; }
    };

    template <typename OStrm_, typename Formatter>
    inline OStrm_& operator <<(OStrm_& os, IDL::traits<bool>::__Writer<Formatter> w)
    {
      using writer_t = IDL::traits<bool>::__Writer<Formatter>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<bool, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<bool>::write_on (os, w.val_, formatter_t ());
    }
    template <typename OStrm_, typename Formatter>
    inline OStrm_& operator <<(OStrm_& os, IDL::traits<char>::__Writer<Formatter> w)
    {
      using writer_t = IDL::traits<char>::__Writer<Formatter>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<char, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<char>::write_on (os, w.val_, formatter_t ());
    }
    template <typename OStrm_, typename Formatter>
    inline OStrm_& operator <<(OStrm_& os, IDL::traits<wchar_t>::__Writer<Formatter> w)
    {
      using writer_t = IDL::traits<wchar_t>::__Writer<Formatter>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<wchar_t, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<wchar_t>::write_on (os, w.val_, formatter_t ());
    }
    template <typename OStrm_, typename Formatter>
    inline OStrm_& operator <<(OStrm_& os, IDL::traits<int8_t>::__Writer<Formatter> w)
    {
      using writer_t = IDL::traits<int8_t>::__Writer<Formatter>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<int8_t, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<int8_t>::write_on (os, w.val_, formatter_t ());
    }
    template <typename OStrm_, typename Formatter>
    inline OStrm_& operator <<(OStrm_& os, IDL::traits<uint8_t>::__Writer<Formatter> w)
    {
      using writer_t = IDL::traits<uint8_t>::__Writer<Formatter>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<uint8_t, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<uint8_t>::write_on (os, w.val_, formatter_t ());
    }
    template <typename OStrm_, typename Formatter>
    inline OStrm_& operator <<(OStrm_& os, IDL::traits<int16_t>::__Writer<Formatter> w)
    {
      using writer_t =  IDL::traits<int16_t>::__Writer<Formatter>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<int16_t, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<int16_t>::write_on (os, w.val_, formatter_t ());
    }
    template <typename OStrm_, typename Formatter>
    inline OStrm_& operator <<(OStrm_& os, IDL::traits<uint16_t>::__Writer<Formatter> w)
    {
      using writer_t = IDL::traits<uint16_t>::__Writer<Formatter>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<uint16_t, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<uint16_t>::write_on (os, w.val_, formatter_t ());
    }
    template <typename OStrm_, typename Formatter>
    inline OStrm_& operator <<(OStrm_& os, IDL::traits<int32_t>::__Writer<Formatter> w)
    {
      using writer_t = IDL::traits<int32_t>::__Writer<Formatter>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<int32_t, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<int32_t>::write_on (os, w.val_, formatter_t ());
    }
    template <typename OStrm_, typename Formatter>
    inline OStrm_& operator <<(OStrm_& os, IDL::traits<uint32_t>::__Writer<Formatter> w)
    {
      using writer_t = IDL::traits<uint32_t>::__Writer<Formatter>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<uint32_t, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<uint32_t>::write_on (os, w.val_, formatter_t ());
    }
    template <typename OStrm_, typename Formatter>
    inline OStrm_& operator <<(OStrm_& os, IDL::traits<int64_t>::__Writer<Formatter> w)
    {
      using writer_t = IDL::traits<int64_t>::__Writer<Formatter>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<int64_t, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<int64_t>::write_on (os, w.val_, formatter_t ());
    }
    template <typename OStrm_, typename Formatter>
    inline OStrm_& operator <<(OStrm_& os, IDL::traits<uint64_t>::__Writer<Formatter> w)
    {
      using writer_t = IDL::traits<uint64_t>::__Writer<Formatter>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<uint64_t, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<uint64_t>::write_on (os, w.val_, formatter_t ());
    }
    template <typename OStrm_, typename Formatter>
    inline OStrm_& operator <<(OStrm_& os, IDL::traits<float>::__Writer<Formatter> w)
    {
      using writer_t = IDL::traits<float>::__Writer<Formatter>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<float, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<float>::write_on (os, w.val_, formatter_t ());
    }
    template <typename OStrm_, typename Formatter>
    inline OStrm_& operator <<(OStrm_& os, IDL::traits<double>::__Writer<Formatter> w)
    {
      using writer_t = IDL::traits<double>::__Writer<Formatter>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<double, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<double>::write_on (os, w.val_, formatter_t ());
    }
    template <typename OStrm_, typename Formatter>
    inline OStrm_& operator <<(OStrm_& os, IDL::traits<long double>::__Writer<Formatter> w)
    {
      using writer_t = IDL::traits<long double>::__Writer<Formatter>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<long double, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<long double>::write_on (os, w.val_, formatter_t ());
    }
    template <typename OStrm_, typename Formatter>
    inline OStrm_& operator <<(OStrm_& os, IDL::traits<std::string>::__Writer<Formatter> w)
    {
      using writer_t = IDL::traits<std::string>::__Writer<Formatter>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<std::string, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<std::string>::write_on (os, w.val_, formatter_t ());
    }
    template <typename OStrm_, typename Formatter>
    inline OStrm_& operator <<(OStrm_& os, IDL::traits<std::wstring>::__Writer<Formatter> w)
    {
      using writer_t = IDL::traits<std::wstring>::__Writer<Formatter>;
      using formatter_t = typename std::conditional<
                          std::is_same<
                            typename writer_t::formatter_t,
                            std::false_type>::value,
                          formatter<std::wstring, OStrm_>,
                          typename writer_t::formatter_t>::type;
      return IDL::traits<std::wstring>::write_on (os, w.val_, formatter_t ());
    }

  } // namespace IDL
} // namespace TAOX11_NAMESPACE

#endif
