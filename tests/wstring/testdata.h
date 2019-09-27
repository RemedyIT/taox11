/**
 * @file    testdata.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 Application test data and utils
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */


static const std::wstring stringArr[] = { L"Abc", L"Def", L"0123456789", L"2",
  L"3", L"4", L"5", L"6" };
std::vector<std::wstring>
    stringVec(stringArr, stringArr + sizeof(stringArr) / sizeof(stringArr[0]));
static const std::wstring stringOutArr[] = { L"Abc", L"Def", L"Ghi", L"0", L"1", L"0123456789",
    L"0123456789", L"0123456789" };
std::vector<std::wstring> stringOutVec(stringOutArr,
    stringOutArr + sizeof(stringOutArr) / sizeof(stringOutArr[0]));

// Dump vector
template<typename T>
void dump(const std::vector<T>& v)
{
  TAOX11_TEST_DEBUG << "[ ";
  for (const T &el : v)
  {
    TAOX11_TEST_DEBUG << (el);
    TAOX11_TEST_DEBUG << ", ";
  }
  TAOX11_TEST_DEBUG << " ]";
}

template<>
void dump(const std::vector<std::wstring>& v)
{
  TAOX11_TEST_DEBUG << "[ ";
  for (const std::wstring &el : v)
  {
    TAOX11_TEST_DEBUG_W << (el.c_str());
    TAOX11_TEST_DEBUG << ", ";
  }
  TAOX11_TEST_DEBUG << " ]";
}

// Test if vector content is equal.
template<typename T>
bool eqv(const std::vector<T>& v1, std::vector<T> v2)
{
  typename std::vector<T>::iterator first2 = v2.begin();
  for (const T &el : v1)
  {
    if (el != *first2)
    {
      dump(v1);
      dump(v2);
      return false;
    }
    ++first2;
  }
  return true;
}

