/**
 * @file    std_make_unique_t.h
 * @author  Marcel Smit
 *
 * @brief   Adds 'make_unique' functionality for MINGW32 and ANDROID builds
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_MAKE_UNIQUE_H_INCLUDED
#define TAOX11_MAKE_UNIQUE_H_INCLUDED

#ifndef TAOX11_STDEXT_H_INCLUDED
#pragma message ("warning: This file should not be included by user code directly. Please remove the include from your code")
#endif

/**
 * For compilers that lack std::make_unique we provide an implementation
 */
#if (defined __GNUC__)
// g++ has a std::make_unique at the moment C++14 is enabled
# if __cplusplus == 201103L
#  define TAOX11_LACKS_STD_MAKE_UNIQUE
# endif
#elif defined (ACE_WIN32_VC14)
// Visual Studio 2015 has a std::make_unique implementation
#else
# define TAOX11_LACKS_STD_MAKE_UNIQUE
#endif

#if defined(TAOX11_LACKS_STD_MAKE_UNIQUE)

// for std::unique_ptr
#include <memory>

namespace std
{
  template<class T,
           typename = typename
             std::enable_if<!(std::is_array<T>::value && std::extent<T>::value == 0)>::type,
           typename ...Args> inline
  unique_ptr<T> make_unique (Args&& ...args)
  {
    return unique_ptr<T> (new T (std::forward<Args> (args)...));
  }

  template<class T,
           typename = typename
             std::enable_if<std::is_array<T>::value && std::extent<T>::value == 0>::type>
  inline
  unique_ptr<T> make_unique (std::size_t size)
  {
    return unique_ptr<T>(new typename std::remove_extent<T>::type[size]());
  }
}

#endif /* TAOX11_LACKS_STD_MAKE_UNIQUE */

#endif /* TAOX11_MAKE_UNIQUE_H_INCLUDED */
