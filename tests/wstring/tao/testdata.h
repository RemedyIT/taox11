/**
 * @file    testdata.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 Application test data and utils
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

static const std::wstring stringArr[] = { L"Abc", L"Def", L"0123456789", L"2",
  L"3", L"4", L"5", L"6" };
static const std::wstring stringOutArr[] = { L"Abc", L"Def", L"Ghi", L"0",
  L"1", L"0123456789", L"0123456789", L"0123456789" };

CORBA::WStringSeq * Array2Seq(const std::wstring arr[])
{
  CORBA::WStringSeq * seq = new CORBA::WStringSeq();
  try
  {
    seq->length(8);
    for (int ix = 0; ix < 8; ix++)
    {
      const std::wstring ccp = arr[ix];
      (*seq)[ix] = CORBA::wstring_dup(ccp.c_str());
    }
  }
  catch (...)
  {
    std::cout << "Error while converting array to sequence." << std::endl;
  }
  return seq;
}

// Test if wstring sequence content is equal.
bool eqv(const CORBA::WStringSeq& v1, CORBA::WStringSeq& v2)
{
  //if (v1.length() != v2.lenght())
  //  return false;
  for (CORBA::WStringSeq::size_type ix = 0; ix < v1.length(); ix++)
  {
    std::wstring s1 = v1[ix].in();
    std::wstring s2 = v2[ix].in();

    if (s1.compare(s2) != 0)
    {
      std::wcout << "  " << s1 << " <> " << s2 << "  ";
      return false;
    }
  }
  return true;
}
