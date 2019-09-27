/**
 * @file    std_to_string.h
 * @author  Martin Corino
 *
 * @brief   Adds 'std::to_string' and 'std::stoX' functionality for MINGW32 and ANDROID builds
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_TO_STRING_H_INCLUDED
#define TAOX11_TO_STRING_H_INCLUDED

#ifndef TAOX11_STDEXT_H_INCLUDED
#pragma message ("warning: This file should not be included by user code directly. Please remove the include from your code")
#endif

#if defined(__MINGW32__) && !defined(__MINGW64__)
# if (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 8))
#   define TAOX11_LACKS_STD_TO_STRING
# endif /* __GNUC__ < 4.8 */
#endif /* defined(__MINGW32__) && !defined(__MINGW64__) */
#if defined(__ANDROID_API__)
# define TAOX11_LACKS_STD_TO_STRING
#endif /* defined (__ANDROID_API__) */

#if defined(TAOX11_LACKS_STD_TO_STRING)

#include <sstream>
#include <stdexcept>

namespace std
{
  template <typename T>
  string
  to_string(T __val)
  {
    std::stringstream str {};
    str << __val;
    return str.str ();
  }

  template <typename T>
  wstring
  to_wstring(T __val)
  {
    std::wstringstream str {};
    str << __val;
    return str.str ();
  }

  inline long
  stol (const string& __str, size_t* __idx = 0, int __base = 10)
  {
    char*__s_end {};
    long l = std::strtol (__str.c_str (), &__s_end, __base);
    if (errno == ERANGE) throw std::out_of_range ("stol");
    if (l == 0 && __s_end == __str.c_str ()) throw std::invalid_argument ("stol");
    if (__idx) *__idx = (__s_end - __str.c_str ()) ;
    return l;
  }

  inline int
  stoi (const string& __str, size_t* __idx = 0, int __base = 10)
  {
    return (int)stol (__str, __idx, __base);
  }

  inline long long
  stoll (const string& __str, size_t* __idx = 0, int __base = 10)
  {
    char* __s_end {};
    long long ll = std::strtoll (__str.c_str (), &__s_end, __base);
    if (errno == ERANGE) throw std::out_of_range ("stoll");
    if (ll == 0 && __s_end == __str.c_str ()) throw std::invalid_argument ("stoll");
    if (__idx) *__idx = (__s_end - __str.c_str ()) ;
    return ll;
  }

  inline unsigned long
  stoul (const string& __str, size_t* __idx = 0, int __base = 10)
  {
    char* __s_end {};
    unsigned long ul = std::strtoul (__str.c_str (), &__s_end, __base);
    if (errno == ERANGE) throw std::out_of_range ("stoul");
    if (ul == 0 && __s_end == __str.c_str ()) throw std::invalid_argument ("stoul");
    if (__idx) *__idx = (__s_end - __str.c_str ()) ;
    return ul;
  }

  inline unsigned long long
  stoull (const string& __str, size_t* __idx = 0, int __base = 10)
  {
    char* __s_end {};
    unsigned long long ull = std::strtoull (__str.c_str (), &__s_end, __base);
    if (errno == ERANGE) throw std::out_of_range ("stoull");
    if (ull == 0 && __s_end == __str.c_str ()) throw std::invalid_argument ("stoull");
    if (__idx) *__idx = (__s_end - __str.c_str ()) ;
    return ull;
  }

  inline long
  stol (const wstring& __str, size_t* __idx = 0, int __base = 10)
  {
    wchar_t* __s_end {};
    long l = std::wcstol (__str.c_str (), &__s_end, __base);
    if (errno == ERANGE) throw std::out_of_range ("stol");
    if (l == 0 && __s_end == __str.c_str ()) throw std::invalid_argument ("stol");
    if (__idx) *__idx = (__s_end - __str.c_str ()) ;
    return l;
  }

  inline int
  stoi (const wstring& __str, size_t* __idx = 0, int __base = 10)
  {
    return (int)stol (__str, __idx, __base);
  }

  inline long long
  stoll (const wstring& __str, size_t* __idx = 0, int __base = 10)
  {
    wchar_t* __s_end {};
    long long ll = std::wcstoll (__str.c_str (), &__s_end, __base);
    if (errno == ERANGE) throw std::out_of_range ("stoll");
    if (ll == 0 && __s_end == __str.c_str ()) throw std::invalid_argument ("stoll");
    if (__idx) *__idx = (__s_end - __str.c_str ()) ;
    return ll;
  }

  inline unsigned long
  stoul (const wstring& __str, size_t* __idx = 0, int __base = 10)
  {
    wchar_t* __s_end {};
    unsigned long ul = std::wcstoul (__str.c_str (), &__s_end, __base);
    if (errno == ERANGE) throw std::out_of_range ("stoul");
    if (ul == 0 && __s_end == __str.c_str ()) throw std::invalid_argument ("stoul");
    if (__idx) *__idx = (__s_end - __str.c_str ()) ;
    return ul;
  }

  inline unsigned long long
  stoull (const wstring& __str, size_t* __idx = 0, int __base = 10)
  {
    wchar_t* __s_end {};
    unsigned long long ull = std::wcstoull (__str.c_str (), &__s_end, __base);
    if (errno == ERANGE) throw std::out_of_range ("stoull");
    if (ull == 0 && __s_end == __str.c_str ()) throw std::invalid_argument ("stoull");
    if (__idx) *__idx = (__s_end - __str.c_str ()) ;
    return ull;
  }

  inline float
  stof (const string& __str, size_t* __idx = 0)
  {
    char* __s_end {};
    float f = std::strtof (__str.c_str (), &__s_end);
    if (errno == ERANGE) throw std::out_of_range ("stof");
    if (f == 0 && __s_end == __str.c_str ()) throw std::invalid_argument ("stof");
    if (__idx) *__idx = (__s_end - __str.c_str ()) ;
    return f;
  }

  inline double
  stod (const string& __str, size_t* __idx = 0)
  {
    char* __s_end {};
    double d = std::strtod (__str.c_str (), &__s_end);
    if (errno == ERANGE) throw std::out_of_range ("stod");
    if (d == 0 && __s_end == __str.c_str ()) throw std::invalid_argument ("stod");
    if (__idx) *__idx = (__s_end - __str.c_str ()) ;
    return d;
  }

  inline long double
  stold (const string& __str, size_t* __idx = 0)
  {
    char* __s_end {};
    long double ld = std::strtold (__str.c_str (), &__s_end);
    if (errno == ERANGE) throw std::out_of_range ("stold");
    if (ld == 0 && __s_end == __str.c_str ()) throw std::invalid_argument ("stold");
    if (__idx) *__idx = (__s_end - __str.c_str ()) ;
    return ld;
  }

  inline float
  stof (const wstring& __str, size_t* __idx = 0)
  {
    wchar_t* __s_end {};
    float f = std::wcstof (__str.c_str (), &__s_end);
    if (errno == ERANGE) throw std::out_of_range ("stof");
    if (f == 0 && __s_end == __str.c_str ()) throw std::invalid_argument ("stof");
    if (__idx) *__idx = (__s_end - __str.c_str ()) ;
    return f;
  }

  inline double
  stod (const wstring& __str, size_t* __idx = 0)
  {
    wchar_t* __s_end {};
    double d = std::wcstod (__str.c_str (), &__s_end);
    if (errno == ERANGE) throw std::out_of_range ("stod");
    if (d == 0 && __s_end == __str.c_str ()) throw std::invalid_argument ("stod");
    if (__idx) *__idx = (__s_end - __str.c_str ()) ;
    return d;
  }

  inline long double
  stold (const wstring& __str, size_t* __idx = 0)
  {
    wchar_t* __s_end {};
    long double ld = std::wcstold (__str.c_str (), &__s_end);
    if (errno == ERANGE) throw std::out_of_range ("stold");
    if (ld == 0 && __s_end == __str.c_str ()) throw std::invalid_argument ("stold");
    if (__idx) *__idx = (__s_end - __str.c_str ()) ;
    return ld;
  }
}; // std
#endif /* TAOX11_LACKS_STD_TO_STRING */

#endif /* TAOX11_TO_STRING_H_INCLUDED */
