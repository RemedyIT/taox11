/**
 * @file    testdata.h
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/StringSeqC.h"

static const std::string stringArr[] = { "Abcdef", "Def", "0123456789", "4", "5",
    "6", "7", "8" };
static const std::string stringOutArr[] = { "Abc", "Def", "Ghi", "0", "1",
    "0123456789", "0123456789", "0123456789" };

CORBA::StringSeq * Array2Seq(const std::string arr[])
{
  CORBA::StringSeq * seq = new CORBA::StringSeq();
  CORBA::StringSeq::size_type length = 8;
  seq->length(length);
  for (CORBA::StringSeq::size_type ix = 0; ix < length; ix++)
  {
    const std::string ccp = arr[ix];
    (*seq)[ix] = CORBA::string_dup(ccp.c_str());
  }
  return seq;
}

// Test if string sequence content is equal.
bool eqv(const CORBA::StringSeq& v1, CORBA::StringSeq& v2)
{
  if (v1.length() != v2.length())
  {
    std::cerr << " Difference in length (StringSeq) " << v1.length() << " and " << v2.length() << " ";
    return false;
  }
  for (CORBA::StringSeq::size_type ix = 0; ix < v1.length(); ix++)
  {
    std::string s1 = v1[ix].in();
    std::string s2 = v2[ix].in();

    if (s1.compare(s2) != 0)
      {
        std::cout << "  " << s1 << " <> " << s2 << "  ";
        return false;
      }
  }
  return true;
}

