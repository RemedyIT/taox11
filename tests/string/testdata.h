/**
 * @file    testdata.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 Application test data and utils
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

static const std::string stringArr[] = { "Abcdef", "Def", "0123456789", "4", "5", "6", "7", "8" };
std::vector<std::string> stringVec(stringArr,
    stringArr + sizeof(stringArr) / sizeof(stringArr[0]));
static const std::string stringOutArr[] =
    { "Abc", "Def", "Ghi", "0", "1", "0123456789", "0123456789", "0123456789" };
std::vector<std::string> stringOutVec(stringOutArr,
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

// Test if vector content is equal.
template<typename T>
bool eqv(const std::vector<T>& v1, std::vector<T> v2)
{
  if (v1.size() != v2.size())
  {
    TAOX11_TEST_DEBUG << " Difference in size " << v1.size() << " and " << v2.size() << " ";
    return false;
  }
  typename std::vector<T>::iterator first2 = v2.begin();
  for (const T &el : v1)
  {
    if (el != *first2)
    {
      TAOX11_TEST_DEBUG << "  "<< el << " <> " << *first2 << "  ";
      dump(v1);
      dump(v2);
      return false;
    }
    ++first2;
  }
  return true;
}

