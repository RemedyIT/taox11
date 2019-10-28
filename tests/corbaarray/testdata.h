/**
 * @file    testdata.h
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TESTDATA_H_
#define TESTDATA_H_

#include "testlib/taox11_testlog.h"
#include <array>

const Test::boolArray _bool1 = { { true, false, true, false, true } };
const Test::boolArray _bool2 = { { false, false, false, true, false } };

const Test::charArray _char1 = { { 'A', '2', '3', '4', '5' } };
const Test::charArray _char2 = { { '2', '3', '4', '5', 'Z' } };

const Test::octetArray _octet1 = { { 1, 2, 3, 4, 5 } };
const Test::octetArray _octet2 = { { 3, 4, 5, 6, 77 } };

const Test::wcharArray _wchar1 = { { L'a', L'2', L'3', L'4', L'5' } };
const Test::wcharArray _wchar2 = { { L'2', L'3', L'4', L'5', L'z' } };

const Test::shortArray _short1 = { { 1, 2, 3, 4, 5 } };
const Test::shortArray _short2 = { { 3, 4, 5, 6, -77 } };

const Test::ushortArray _ushort1 = { { 1, 2, 3, 4, 5 } };
const Test::ushortArray _ushort2 = { { 3, 4, 5, 6, 77 } };

const Test::longArray _long1 = { { 1, 2, 3, 4, 5 } };
const Test::longArray _long2 = { { 3, 4, 5, 6, -77 } };

const Test::ulongArray _ulong1 = { { 1, 2, 3, 4, 5 } };
const Test::ulongArray _ulong2 = { { 3, 4, 5, 6, 77 } };

const Test::llongArray _llong1 = { { 1, 2, 3, 4, 5 } };
const Test::llongArray _llong2 = { { 3, 4, 5, 6, -77 } };

const Test::ullongArray _ullong1 = { { 1, 2, 3, 4, 5 } };
const Test::ullongArray _ullong2 = { { 3, 4, 5, 6, 77 } };

const Test::floatArray _float1 = { { 1, 2, 3, 4, 5 } };
const Test::floatArray _float2 = { { 3, 4, 5, 6, -77 } };

const Test::doubleArray _double1 = { { 1, 2, 3, 4, 5 } };
const Test::doubleArray _double2 = { { 3, 4, 5, 6, -77 } };

const Test::Hello::ldoubleArray _ldouble1 = { { 1, 2, 3, 4, 5 } };
const Test::Hello::ldoubleArray _ldouble2 = { { 3, 4, 5, 6, -77 } };

// Dump array
template<typename T, size_t N>
void dump(const std::array<T, N>& v)
{
  TAOX11_TEST_DEBUG << "[ ";
  for (const T &el : v)
  {
    TAOX11_TEST_DEBUG << (el);
    TAOX11_TEST_DEBUG << ", ";
  }
  TAOX11_TEST_DEBUG << " ]";
}

template<typename T, size_t N>
void fill(std::array<T, N>& arr, T start)
{
  for (T &i : arr) {
    i = start++;
  }
}

// Fill a 2 dimensional array.
template<typename T, size_t N, size_t M>
void fill(std::array<std::array<T, N>, M>& v1, T start)
{
  for (std::array< T, N> &a1 : v1)
  {
    fill(a1, start++);
  }
}

// Fill a 3 dimensional array.
template<typename T, size_t N, size_t M, size_t O>
void fill(std::array<std::array<std::array<T, N>, M>, O>& v1, T start)
{
  for (std::array<std::array< T, N>, M> &a1 : v1)
  {
    fill(a1, start++);
  }
}

template<typename T, size_t N>
auto fillnew(T start) -> std::array<T, N>&
{
  std::array<T, N> mya2;
  for (T &i : mya2) {
    i = start++;
  }
  return mya2;
}

template<typename T, size_t N>
auto fillnew(std::array<T, N>& dummy, T start) -> std::array<T, N>&
{
  // dummy[0] = start;
  std::array<T, N> mya2;
  for (T &i : mya2) {
    i = start++;
  }
  dummy = mya2;
  return mya2;
}

// Fill a _new_ 2 dimensional array.
template<typename T, size_t N, size_t M>
auto fillnew(std::array<std::array<T, N>, M> dummy, T start) -> std::array<std::array<T, N>, M>&
{
  // dummy[0][0] = start;
  std::array < std::array<T, N>, M > mya2;
  for (std::array< T, N> &a1 : mya2)
  {
    fillnew(a1, start++);
  }
  dummy = mya2;
  return mya2;
}

// Fill a _new_ 3 dimensional array.
template<typename T, size_t N, size_t M, size_t O>
auto fillnew(std::array<std::array<std::array<T, N>, M>, O> dummy, T start) -> std::array<std::array<std::array<T, N>, M>, O>&
{
  //dummy[0][0][0] = start;
  std::array < std::array<std::array<T, N>, M>, O > mya2;
  for (std::array<std::array<T, N>, M> &a1 : mya2)
  {
    fillnew(a1, start++);
  }
  dummy = mya2;
  return mya2;
}

/// bool specializations
//  (use std::array<>::fill () method instead of similar
//   fill as above to work around strange optimization bug
//   in MinGW64 GCC 4.8.0 on Win64)

// Fill a 2 dimensional array.
template<size_t N, size_t M>
void fill(std::array<std::array<bool, N>, M>& v1, bool start)
{
  for (std::array<bool, N> &a1 : v1)
  {
    a1.fill(start ? true : false);
  }
}

// Fill a 3 dimensional array.
template<size_t N, size_t M, size_t O>
void fill(std::array<std::array<std::array<bool, N>, M>, O>& v1, bool start)
{
  for (std::array<std::array<bool, N>, M> &a1 : v1)
  {
    fill(a1, start ? true : false);
  }
}

// Test if array content is equal.
template<typename T, size_t N>
bool eqArray(const std::array<T, N>& v1, const std::array<T, N>& v2)
{
  typename std::array<T, N>::const_iterator first2 = v2.begin();
  for (const T &el : v1) {
    if (el != *first2)
    {
      TAOX11_TEST_ERROR << "Error: "<< el << "<>" << *first2 << " ";
      TAOX11_TEST_ERROR << " Expected:"; dump(v1);
      TAOX11_TEST_ERROR << " found:"; dump(v2); TAOX11_TEST_ERROR << std::endl;
      return false;
    }
    ++first2;
  }
  return true;
}

// Test if array content is equal.
template<typename T, size_t N, size_t M>
auto eqArray(const std::array<std::array<T, N>, M>& v1,
    const std::array<std::array<T, N>, M>& v2) ->bool
{
  typename std::array<std::array<T, N>, M>::const_iterator first2 = v2.begin();
  for (const std::array< T, N> &a1 : v1)
  {
    if (!eqArray(a1, *first2)) return false;
    ++first2;
  }
  return true;
}

// Test if array content is equal.
template<typename T, size_t N, size_t M, size_t O>
auto eqArray(const std::array<std::array<std::array<T, N>, M>, O>& v1,
    const std::array<std::array<std::array<T, N>, M>, O>& v2) ->bool
{
  typename std::array<std::array<std::array<T, N>, M>, O>::const_iterator first2 =
      v2.begin();
  for (const std::array<std::array<T, N>, M> &a1 : v1)
  {
    if (!eqArray(a1, *first2)) return false;
    ++first2;
  }
  return true;
}

#endif /* TESTDATA_H_ */
