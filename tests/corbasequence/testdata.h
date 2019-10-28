/**
 * @file    testdata.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 corba sequence test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
static const char CharArr[] = {'a', 'b', 'c', 'd'};
std::vector<char> CharVec (CharArr, CharArr + sizeof(CharArr) / sizeof(CharArr[0]) );
static const char CharOutArr[] = {'1', '2', '3', '4', '5'};
std::vector<char> CharOutVec (CharOutArr, CharOutArr + sizeof(CharOutArr) / sizeof(CharOutArr[0]) );

static const wchar_t WCharArr[] = {'a', 'b', 'c', 'd'};
std::vector<wchar_t> WCharVec (WCharArr, WCharArr + sizeof(WCharArr) / sizeof(WCharArr[0]) );
static const wchar_t WCharOutArr[] = {'1', '2', '3', '4', '5'};
std::vector<wchar_t> WCharOutVec (WCharOutArr, WCharOutArr + sizeof(WCharOutArr) / sizeof(WCharOutArr[0]) );

static const std::string StringArr[] = {"a", "bb", "ccc", "dddd"};
std::vector<std::string> StringVec (StringArr, StringArr + sizeof(StringArr) / sizeof(StringArr[0]) );
static const std::string StringOutArr[] = {"aaaa", "bbb", "cc", "d"};
std::vector<std::string> StringOutVec (StringOutArr, StringOutArr + sizeof(StringOutArr) / sizeof(StringOutArr[0]) );

static const std::wstring WStringArr[] = {L"a", L"bb", L"ccc", L"dddd"};
std::vector<std::wstring> WStringVec (WStringArr, WStringArr + sizeof(WStringArr) / sizeof(WStringArr[0]) );
static const std::wstring WStringOutArr[] = {L"aaaa", L"bbb", L"cc", L"d"};
std::vector<std::wstring> WStringOutVec (WStringOutArr, WStringOutArr + sizeof(WStringOutArr) / sizeof(WStringOutArr[0]) );

static const bool BooleanArr[] = {true, false, true, false, true, false, true, true, true, true, true, true, true, true};
std::vector<bool> BooleanVec (BooleanArr, BooleanArr + sizeof(BooleanArr) / sizeof(BooleanArr[0]) );
static const bool BooleanOutArr[] = {false, false, true, false, true, true, false, true, true, false};
std::vector<bool> BooleanOutVec (BooleanOutArr, BooleanOutArr + sizeof(BooleanOutArr) / sizeof(BooleanOutArr[0]) );

static const uint8_t OctetArr[] = {65, 66, 67, 68, 99, 100, 101, 102, 103, 45, 46, 47, 48, 49, 50, 51};
std::vector<uint8_t> OctetVec (OctetArr, OctetArr + sizeof(OctetArr) / sizeof(OctetArr[0]) );
static const uint8_t OctetOutArr[] = {77, 78, 65, 66, 33, 34, 107, 108, 99, 98, 97, 96};
std::vector<uint8_t> OctetOutVec (OctetOutArr, OctetOutArr + sizeof(OctetOutArr) / sizeof(OctetOutArr[0]) );

static const int16_t ShortArr[] = {-15, 16, 1, 2, 28, 29};
std::vector<int16_t> ShortVec (ShortArr, ShortArr + sizeof(ShortArr) / sizeof(ShortArr[0]) );
static const int16_t ShortOutArr[] = {-129, 65, 33, 17, 9, 5, 3, 0};
std::vector<int16_t> ShortOutVec (ShortOutArr, ShortOutArr + sizeof(ShortOutArr) / sizeof(ShortOutArr[0]) );

static const uint16_t UShortArr[] = {15, 16, 1, 2, 28, 29};
std::vector<uint16_t> UShortVec (UShortArr, UShortArr + sizeof(UShortArr) / sizeof(UShortArr[0]) );
static const uint16_t UShortOutArr[] = {129, 65, 33, 17, 9, 5, 3, 0};
std::vector<uint16_t> UShortOutVec (UShortOutArr, UShortOutArr + sizeof(UShortOutArr) / sizeof(UShortOutArr[0]) );

static const int32_t LongArr[] = {-15, 16, 1, 2, 28, 29};
std::vector<int32_t> LongVec (LongArr, LongArr + sizeof(LongArr) / sizeof(LongArr[0]) );
static const int32_t LongOutArr[] = {-129, 65, 33, 17, 9, 5, 3, 0};
std::vector<int32_t> LongOutVec (LongOutArr, LongOutArr + sizeof(LongOutArr) / sizeof(LongOutArr[0]) );

static const uint32_t ULongArr[] = {15, 16, 1, 2, 28, 29};
std::vector<uint32_t> ULongVec (ULongArr, ULongArr + sizeof(ULongArr) / sizeof(ULongArr[0]) );
static const uint32_t ULongOutArr[] = {129, 65, 33, 17, 9, 5, 3, 0};
std::vector<uint32_t> ULongOutVec (ULongOutArr, ULongOutArr + sizeof(ULongOutArr) / sizeof(ULongOutArr[0]) );

static const int64_t LongLongArr[] = {-15, 16, 1, 2, 28, 29};
std::vector<int64_t> LongLongVec (LongLongArr, LongLongArr + sizeof(LongLongArr) / sizeof(LongLongArr[0]) );
static const int64_t LongLongOutArr[] = {-129, 65, 33, 17, 9, 5, 3, 0};
std::vector<int64_t> LongLongOutVec (LongLongOutArr, LongLongOutArr + sizeof(LongLongOutArr) / sizeof(LongLongOutArr[0]) );

static const uint64_t ULongLongArr[] = {15, 16, 1, 2, 28, 29};
std::vector<uint64_t> ULongLongVec (ULongLongArr, ULongLongArr + sizeof(ULongLongArr) / sizeof(ULongLongArr[0]) );
static const uint64_t ULongLongOutArr[] = {129, 65, 33, 17, 9, 5, 3, 0};
std::vector<uint64_t> ULongLongOutVec (ULongLongOutArr, ULongLongOutArr + sizeof(ULongLongOutArr) / sizeof(ULongLongOutArr[0]) );

static const float FloatArr[] = {-15.516273F, 16.516273F, 1.516273F, 2.516273F, 28.516273F, 29.0F};
std::vector<float> FloatVec (FloatArr, FloatArr + sizeof(FloatArr) / sizeof(FloatArr[0]) );
static const float FloatOutArr[] = {-129.0F, 65.0F, 33.516273F, 17.516273F, 9.516273F, 5.516273F, 3.516273F, 0.0F};
std::vector<float> FloatOutVec (FloatOutArr, FloatOutArr + sizeof(FloatOutArr) / sizeof(FloatOutArr[0]) );

static const double DoubleArr[] = {-15.516273, 16.516273, 1.516273, 2.516273, 28.516273, 29};
std::vector<double> DoubleVec (DoubleArr, DoubleArr + sizeof(DoubleArr) / sizeof(DoubleArr[0]) );
static const double DoubleOutArr[] = {-129.0, 65.0, 33.516273, 17.516273, 9.516273, 5.516273, 3.516273, 0};
std::vector<double> DoubleOutVec (DoubleOutArr, DoubleOutArr + sizeof(DoubleOutArr) / sizeof(DoubleOutArr[0]) );

static const long double LongDoubleArr[] = {-15.516273, 16.516273, 1.516273, 2.516273, 28.516273, 29};
std::vector<long double> LongDoubleVec (LongDoubleArr, LongDoubleArr + sizeof(LongDoubleArr) / sizeof(LongDoubleArr[0]) );
static const long double LongDoubleOutArr[] = {-129.0, 65.0, 33.516273, 17.516273, 9.516273, 5.516273, 3.516273, 0};
std::vector<long double> LongDoubleOutVec (LongDoubleOutArr, LongDoubleOutArr + sizeof(LongDoubleOutArr) / sizeof(LongDoubleOutArr[0]) );

// Test if vector content is equal.
template<typename T>
bool
eqv (const std::vector<T>& v1, std::vector<T> v2)
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

