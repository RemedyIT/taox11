/**
 * @file    testdata.h
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include <cmath>
#include <limits>

static const char charArr[] = { 'a', 'b', 'c', 'd' };
std::vector<char> charVec(charArr,
    charArr + sizeof(charArr) / sizeof(charArr[0]));
static const char charOutArr[] = { '1', '2', '3', '4', '5' };
std::vector<char> charOutVec(charOutArr,
    charOutArr + sizeof(charOutArr) / sizeof(charOutArr[0]));

static const wchar_t wcharArr[] = { 'a', 'b', 'c', 'd' };
std::vector<wchar_t> wchar_tVec(wcharArr,
    wcharArr + sizeof(wcharArr) / sizeof(wcharArr[0]));
static const wchar_t wcharOutArr[] = { '1', '2', '3', '4', '5' };
std::vector<wchar_t> wchar_tOutVec(wcharOutArr,
    wcharOutArr + sizeof(wcharOutArr) / sizeof(wcharOutArr[0]));

static const bool boolArr[] = { true, false, true, false, true, false, true,
    true, true, true, true, true, true, true };
std::vector<bool> boolVec(boolArr,
    boolArr + sizeof(boolArr) / sizeof(boolArr[0]));
static const bool boolOutArr[] = { false, false, true, false, true, true,
    false, true, true, false };
std::vector<bool> boolOutVec(boolOutArr,
    boolOutArr + sizeof(boolOutArr) / sizeof(boolOutArr[0]));

static const uint8_t OctetArr[] = { 65, 66, 67, 68, 99, 100, 101, 102, 103, 45,
    46, 47, 48, 49, 50, 51 };
std::vector<uint8_t> OctetVec(OctetArr,
    OctetArr + sizeof(OctetArr) / sizeof(OctetArr[0]));
static const uint8_t OctetOutArr[] = { 77, 78, 65, 66, 33, 34, 107, 108, 99,
    98, 97, 96 };
std::vector<uint8_t> OctetOutVec(OctetOutArr,
    OctetOutArr + sizeof(OctetOutArr) / sizeof(OctetOutArr[0]));

static const int16_t ShortArr[] = { -15, 16, 1, 2, 28, 29 };
std::vector<int16_t> ShortVec(ShortArr,
    ShortArr + sizeof(ShortArr) / sizeof(ShortArr[0]));
static const int16_t ShortOutArr[] = { -129, 65, 33, 17, 9, 5, 3, 0 };
std::vector<int16_t> ShortOutVec(ShortOutArr,
    ShortOutArr + sizeof(ShortOutArr) / sizeof(ShortOutArr[0]));

static const uint16_t UShortArr[] = { 15, 16, 1, 2, 28, 29 };
std::vector<uint16_t> UShortVec(UShortArr,
    UShortArr + sizeof(UShortArr) / sizeof(UShortArr[0]));
static const uint16_t UShortOutArr[] = { 129, 65, 33, 17, 9, 5, 3, 0 };
std::vector<uint16_t> UShortOutVec(UShortOutArr,
    UShortOutArr + sizeof(UShortOutArr) / sizeof(UShortOutArr[0]));

static const int32_t LongArr[] = { -15, 16, 1, 2, 28, 29 };
std::vector<int32_t> LongVec(LongArr,
    LongArr + sizeof(LongArr) / sizeof(LongArr[0]));
static const int32_t LongOutArr[] = { -129, 65, 33, 17, 9, 5, 3, 0 };
std::vector<int32_t> LongOutVec(LongOutArr,
    LongOutArr + sizeof(LongOutArr) / sizeof(LongOutArr[0]));

static const uint32_t ULongArr[] = { 15, 16, 1, 2, 28, 29 };
std::vector<uint32_t> ULongVec(ULongArr,
    ULongArr + sizeof(ULongArr) / sizeof(ULongArr[0]));
static const uint32_t ULongOutArr[] = { 129, 65, 33, 17, 9, 5, 3, 0 };
std::vector<uint32_t> ULongOutVec(ULongOutArr,
    ULongOutArr + sizeof(ULongOutArr) / sizeof(ULongOutArr[0]));

static const int64_t LongLongArr[] = { -15, 16, 1, 2, 28, 29 };
std::vector<int64_t> LongLongVec(LongLongArr,
    LongLongArr + sizeof(LongLongArr) / sizeof(LongLongArr[0]));
static const int64_t LongLongOutArr[] = { -129, 65, 33, 17, 9, 5, 3, 0 };
std::vector<int64_t> LongLongOutVec(LongLongOutArr,
    LongLongOutArr + sizeof(LongLongOutArr) / sizeof(LongLongOutArr[0]));

static const uint64_t ULongLongArr[] = { 15, 16, 1, 2, 28, 29 };
std::vector<uint64_t> ULongLongVec(ULongLongArr,
    ULongLongArr + sizeof(ULongLongArr) / sizeof(ULongLongArr[0]));
static const uint64_t ULongLongOutArr[] = { 129, 65, 33, 17, 9, 5, 3, 0 };
std::vector<uint64_t> ULongLongOutVec(ULongLongOutArr,
    ULongLongOutArr + sizeof(ULongLongOutArr) / sizeof(ULongLongOutArr[0]));

static const float floatArr[] = { -15.516273F, 16.516273F, 1.516273F, 2.516273F,
    28.516273F, 29.0F };
std::vector<float> floatVec(floatArr,
    floatArr + sizeof(floatArr) / sizeof(floatArr[0]));
static const float floatOutArr[] = { -129.0F, 65.0F, 33.516273F, 17.516273F,
    9.516273F, 5.516273F, 3.516273F, 0.0F };
std::vector<float> floatOutVec(floatOutArr,
    floatOutArr + sizeof(floatOutArr) / sizeof(floatOutArr[0]));

static const double doubleArr[] = { -15.516273, 16.516273, 1.516273, 2.516273,
    28.516273, 29 };
std::vector<double> doubleVec(doubleArr,
    doubleArr + sizeof(doubleArr) / sizeof(doubleArr[0]));
static const double doubleOutArr[] = { -129.0, 65.0, 33.516273, 17.516273,
    9.516273, 5.516273, 3.516273, 0 };
std::vector<double> doubleOutVec(doubleOutArr,
    doubleOutArr + sizeof(doubleOutArr) / sizeof(doubleOutArr[0]));

static const long double ldoubleArr[] = { -15.516273, 16.516273, 1.516273,
    2.516273, 28.516273, 29 };
std::vector<long double> ldoubleVec(ldoubleArr,
    ldoubleArr + sizeof(ldoubleArr) / sizeof(ldoubleArr[0]));
static const long double ldoubleOutArr[] = { -129.0, 65.0, 33.516273,
    17.516273, 9.516273, 5.516273, 3.516273, 0 };
std::vector<long double> ldoubleOutVec(ldoubleOutArr,
    ldoubleOutArr + sizeof(ldoubleOutArr) / sizeof(ldoubleOutArr[0]));

// Test if vector content is equal.
template<typename T>
bool eqv(const std::vector<T>& v1, std::vector<T> v2)
{
  typename std::vector<T>::iterator first2 = v2.begin();
  for (const T &el : v1)
  {
    if (el != *first2)
    {
      TAOX11_TEST_ERROR << "  "<< el << " <> " << *first2 << "  " << v1 << v2 << std::endl;
      return false;
    }
    ++first2;
  }
  return true;
}

// Test if vector content is equal.
template<>
bool eqv(const std::vector<long double>& v1, std::vector<long double> v2)
{
  std::vector<long double>::iterator first2 = v2.begin();
  for (const long double &el : v1)
  {
    if (std::fabs (el - *first2) > std::numeric_limits<long double>::epsilon ())
    {
      TAOX11_TEST_ERROR << "  "<< el << " <> " << *first2 << "  " << v1 << v2 << std::endl;
      return false;
    }
    ++first2;
  }
  return true;
}
