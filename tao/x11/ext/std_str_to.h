/**
 * @file    std_str_to.h
 * @author  Martin Corino
 *
 * @brief   Adds 'std::strtoXX' and 'std::wcstoXX' functionality for ANDROID builds
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_STD_STR_TO_H_INCLUDED
#define TAOX11_STD_STR_TO_H_INCLUDED

#ifndef TAOX11_STDEXT_H_INCLUDED
#pragma message ("warning: This file should not be included by user code directly. Please remove the include from your code")
#endif

#if defined(__ANDROID_API__)
# define TAOX11_LACKS_STD_STR_TO_SUPPORT
#endif /* defined (__ANDROID_API__) */

#if defined(TAOX11_LACKS_STD_STR_TO_SUPPORT)

#include <stdlib.h>

#ifdef strtoll
# undef strtoll
#endif
#ifdef strtoull
# undef strtoull
#endif
#ifdef strtof
# undef strtof
#endif
#ifdef strtold
# undef strtold
#endif

#ifdef wcstoll
# undef wcstoll
#endif
#ifdef wcstoull
# undef wcstoull
#endif
#ifdef wcstof
# undef wcstof
#endif
#ifdef wcstold
# undef wcstold
#endif

namespace std
{
  inline long long
  strtoll (const char *str, char **str_end, int base)
  {
    return ::strtoll (str, str_end, base);
  }
  inline unsigned long long
  strtoull (const char *str, char **str_end, int base)
  {
    return ::strtoull (str, str_end, base);
  }
  inline float
  strtof (const char *str, char **str_end)
  {
    return ::strtof (str, str_end);
  }
  inline long double
  strtold (const char *str, char **str_end)
  {
    return ::strtold (str, str_end);
  }

  inline long long
  wcstoll (const wchar_t *str, wchar_t **str_end, int base)
  {
    return ::wcstoll (str, str_end, base);
  }
  inline unsigned long long
  wcstoull (const wchar_t *str, wchar_t **str_end, int base)
  {
    return ::wcstoull (str, str_end, base);
  }
  inline float
  wcstof (const wchar_t *str, wchar_t **str_end)
  {
    return ::wcstof (str, str_end);
  }
  inline long double
  wcstold (const wchar_t *str, wchar_t **str_end)
  {
    return ::wcstold (str, str_end);
  }
}; // std
#endif /* TAOX11_LACKS_STD_STR_TO_SUPPORT */

#endif /* TAOX11_STD_STR_TO_H_INCLUDED */
