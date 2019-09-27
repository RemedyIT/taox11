/**
 * @file    client.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 CodecFactory Test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "testC.h"
#include "tao/x11/codecfactory/codecfactory.h"

#include "ace/CDR_Base.h"
#include "testlib/taox11_testlog.h"

std::wstring
gen_wstring (uint32_t max_length)
{
  std::wstring ret (L"");
  uint32_t i = 0;
  int8_t x = 30;
  // Generate some random characters
  while (i < max_length)
  {
    wchar_t wc = ++x;
    if (wc)
    {
      ret += wc;
      i++;
    }
  }
  return ret;
}

bool
verify_data (Test::Bar *original, Test::Bar *extracted)
{
  if (!original || !extracted)
    return false;

  TAOX11_TEST_DEBUG << "Original" << std::endl;
  TAOX11_TEST_DEBUG << "--------" << std::endl;
  TAOX11_TEST_DEBUG << original->A () << std::endl;
  TAOX11_TEST_DEBUG << original->B () << std::endl;
  TAOX11_TEST_DEBUG << original->C () << std::endl;
  TAOX11_TEST_DEBUG << original->D () << std::endl;
  TAOX11_TEST_DEBUG << original->E () << std::endl;
  TAOX11_TEST_DEBUG << std::endl;
  TAOX11_TEST_DEBUG << "Extracted" << std::endl;
  TAOX11_TEST_DEBUG << "---------" << std::endl;
  TAOX11_TEST_DEBUG << extracted->A () << std::endl;
  TAOX11_TEST_DEBUG << extracted->B () << std::endl;
  TAOX11_TEST_DEBUG << extracted->C () << std::endl;
  TAOX11_TEST_DEBUG << extracted->D () << std::endl;
  TAOX11_TEST_DEBUG << extracted->E () << std::endl;
  TAOX11_TEST_DEBUG << std::endl;

  return !(original->A () != extracted->A () ||
    original->B () != extracted->B () ||
    original->C () != extracted->C () ||
    original->D () != extracted->D () ||
    original->E () != extracted->E ());
}

bool
test_codec (IDL::traits<IOP::Codec>::ref_type  codec)
{
  // Test values to be placed in the test structure.
  int32_t A = 1010;
  int32_t B = -3427;
  uint64_t C = 2001;
  char D[] = "I'm Batman.";
  std::wstring E = gen_wstring (25);

  // Create the structure to be encoded...
  Test::Bar value;
  value.A (A);
  value.B (B);
  value.C (C);
  value.D (D);
  value.E (E);

  // ... and put it in an Any
  CORBA::Any data;
  data <<= value;

  CORBA::OctetSeq encoded_data;
  CORBA::Any      decoded_data;
  Test::Bar extracted_value;

  // Encode the structure into an octet sequence using the CDR
  // enscapsulation Codec.

  TAOX11_TEST_DEBUG << "Testing CDR encapsulation Codec encode()/decode()"
    << std::endl;
  TAOX11_TEST_DEBUG << "================================================="
    << std::endl;

  // Start out with the encode() method, i.e. the one that
  // includes the TypeCode in the CDR encapsulation.
  encoded_data = codec->encode (data);

  if ((reinterpret_cast<ptrdiff_t> (encoded_data.data ())
          % ACE_CDR::MAX_ALIGNMENT) == 0)
  {
    TAOX11_TEST_DEBUG << std::endl;
    TAOX11_TEST_DEBUG << "Data for decoding are already aligned "
      "on MAX_ALIGNMENT." << std::endl;
    TAOX11_TEST_DEBUG << std::endl;
  }

  // Extract the data from the octet sequence.
  decoded_data = codec->decode (encoded_data);

  if (!(decoded_data >>= extracted_value))
  {
    TAOX11_TEST_ERROR << "ERROR: Unable to extract decoded data "
      << "from Any" << std::endl;
    return false;
  }
  // Verify that the extracted data matches the data that was
  // originally encoded into the octet sequence.
  if (!verify_data (&value, std::addressof(extracted_value)))
  {
    TAOX11_TEST_ERROR << "ERROR: Data extracted using "
      << "IOP::Codec::decode() does not match original data."
      << std::endl;
    return false;
  }

  TAOX11_TEST_DEBUG
    << "Testing CDR encapsulation Codec encode_value()/decode_value()"
    << std::endl;
  TAOX11_TEST_DEBUG
    << "============================================================="
    << std::endl;

  // Now use the encode_value() method, i.e. the one that does
  // *not* include the TypeCode in the CDR encapsulation.
  encoded_data = codec->encode_value (data);

  if ((reinterpret_cast<ptrdiff_t> (encoded_data.data ())
          % ACE_CDR::MAX_ALIGNMENT) == 0)
  {
    TAOX11_TEST_WARNING << std::endl;
    TAOX11_TEST_WARNING << "WARNING: Data to be decoded is already aligned "
      "on MAX_ALIGNMENT." << std::endl;
    TAOX11_TEST_WARNING << std::endl;
  }

  // Extract the data from the octet sequence.
  decoded_data = codec->decode_value (encoded_data, Test::_tc_Bar);

  if (!(decoded_data >>= extracted_value))
  {
    TAOX11_TEST_ERROR << "ERROR: Unable to extract decoded data from Any"
      << std::endl;
    return false;
  }

  // Verify that the extracted data matches the data that was
  // originally encoded into the octet seq  uence.
  if (!verify_data (std::addressof(value), &extracted_value))
  {
    TAOX11_TEST_ERROR << "ERROR: Data extracted using IOP::Codec::decode_value() "
      << "does not match original data" << std::endl;
    return false;
  }

  return true;
}

uint16_t
test_narrow (IDL::traits<CORBA::Object>::ref_type object)
{
  uint16_t retval = 0;

  TAOX11_TEST_DEBUG
    << "Testing the IOP::Codec narrow functionality" << std::endl;
  TAOX11_TEST_DEBUG
    << "========================================================================"
    << std::endl;

  try
  {
    IDL::traits<IOP::Codec>::ref_type tmp =
      IDL::traits<IOP::Codec>::narrow (nullptr);
    if (tmp != nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR (test_narrow): Narrowing a nullptr doesn't "
        << "result in a nullptr." << std::endl;
      ++retval;
    }
    else
    {
      TAOX11_TEST_DEBUG << "OK (test_narrow): Narrowing a nullptr returned a "
        << "nullptr." << std::endl;
    }
  }
  catch (const CORBA::Exception &ex)
  {
    TAOX11_TEST_ERROR << "ERROR (test_narrow): Unexpected CORBA exception "
      << "whilst narrowing a nullptr : " << ex << std::endl;
    ++retval;
  }

  try
  {
    IDL::traits<IOP::Codec>::ref_type tmp =
      IDL::traits<IOP::Codec>::narrow (object);
    if (tmp == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR (test_narrow): Narrowing a valid object "
        << "returned a nullptr." << std::endl;
      ++retval;
    }
    else
    {
      TAOX11_TEST_DEBUG << "OK (test_narrow): Narrowing a valid object returned a "
        << "valid codec." << std::endl;
    }
  }
  catch (const CORBA::Exception &ex)
  {
    TAOX11_TEST_ERROR << "ERROR (test_narrow): Unexpected CORBA exception "
      << "whilst narrowing an valid object: " << ex << std::endl;
    ++retval;
  }
  TAOX11_TEST_DEBUG << std::endl;
  return retval;
}

uint16_t
test_factory_exceptions (IDL::traits<IOP::CodecFactory>::ref_type codec_factory)
{
  uint16_t retval = 0;

  TAOX11_TEST_DEBUG
    << "Testing the IOP::Codec::UnknownEncoding exception create_codec()/"
    << "create_codec_with_codesets()"
    << std::endl;
  TAOX11_TEST_DEBUG
    << "========================================================================"
    << std::endl;

  // Test UnknownEncoding for 'create_codec'
  IOP::Encoding encoding;
  encoding.format (1);
  encoding.major_version (1);
  encoding.minor_version (0);
  try
  {
    codec_factory->create_codec (encoding);
    TAOX11_TEST_ERROR << "ERROR: No UnknownEncoding exception caught when "
      "supplying a wrong format (1) to 'create_codec'." << std::endl;
    ++retval;
  }
  catch (const IOP::CodecFactory::UnknownEncoding &ex)
  {
    TAOX11_TEST_DEBUG << "OK (create_codec): Caught the correct exception: "
      << ex << std::endl;
  }

  // Test UnknownEncoding for 'create_codec_with_codesets'
  IOP::Encoding_1_2 encoding_1_2;
  encoding_1_2.format (1);
  encoding_1_2.major_version (1);
  encoding_1_2.minor_version (2);
  encoding_1_2.char_codeset (0x00010001U);
  encoding_1_2.wchar_codeset (0x00010109U);
  try
  {
    codec_factory->create_codec_with_codesets (encoding_1_2);
    TAOX11_TEST_ERROR << "ERROR: No UnknownEncoding exception caught when "
      "supplying a wrong format (1) to 'create_codec_with_codesets'."
      << std::endl;
    ++retval;
  }
  catch (const IOP::CodecFactory::UnknownEncoding &ex)
  {
    TAOX11_TEST_DEBUG << "OK (create_codec_with_codesets): Caught the correct "
      << "exception: " << ex << std::endl;
  }

  // Test UnsupportedCodeset for 'create_codec_with_codesets'
  IOP::Encoding_1_2 encoding_1_2_1;
  encoding_1_2_1.format (IOP::ENCODING_CDR_ENCAPS);
  encoding_1_2_1.major_version (1);
  encoding_1_2_1.minor_version (2);
  encoding_1_2_1.char_codeset (0x00020001U);
  encoding_1_2_1.wchar_codeset (0x00010109U);
  try
  {
    codec_factory->create_codec_with_codesets (encoding_1_2_1);
    TAOX11_TEST_ERROR << "ERROR: No UnknownEncoding exception caught when "
      "supplying a wrong format (1) to 'create_codec_with_codesets'."
      << std::endl;
    ++retval;
  }
  catch (const IOP::CodecFactory::UnsupportedCodeset &ex)
  {
    if (ex.codeset () != 0x00020001U)
    {
      TAOX11_TEST_ERROR << "ERROR: Unexpected codeset received. Expected <"
        << 0x00020001U << "> - received <" << ex.codeset () << ">"
        << std::endl;
      retval++;
    }
    TAOX11_TEST_DEBUG << "OK (create_codec_with_codesets): Caught the correct "
      << "exception: " << ex << std::endl;
  }

  // Test UnsupportedCodeset for 'create_codec_with_codesets'
  IOP::Encoding_1_2 encoding_1_2_2;
  encoding_1_2_2.format (IOP::ENCODING_CDR_ENCAPS);
  encoding_1_2_2.major_version (1);
  encoding_1_2_2.minor_version (2);
  encoding_1_2_2.char_codeset (0x00010001U);
  encoding_1_2_2.wchar_codeset (0x00020109U);
  try
  {
    codec_factory->create_codec_with_codesets (encoding_1_2_2);
    TAOX11_TEST_ERROR << "ERROR: No UnknownEncoding exception caught when "
      "supplying a wrong format (1) to 'create_codec_with_codesets'." << std::endl;
    ++retval;
  }
  catch (const IOP::CodecFactory::UnsupportedCodeset &ex)
  {
    if (ex.codeset () != 0x00020109U)
    {
      TAOX11_TEST_ERROR << "ERROR: Unexpected codeset received. Expected <"
        << 0x00020109U << "> - received <" << ex.codeset () << ">"
        << std::endl;
      retval++;
    }
    TAOX11_TEST_DEBUG << "OK (create_codec_with_codesets): Caught the correct "
      << "exception: " << ex << std::endl;
  }
  TAOX11_TEST_DEBUG << std::endl;
  return retval;
}

uint16_t
test_invalidtypeforencoding (IDL::traits<IOP::CodecFactory>::ref_type codec_factory)
{
  uint16_t retval = 0;

  IOP::Encoding encoding;
  encoding.format (IOP::ENCODING_CDR_ENCAPS);
  encoding.major_version (1);
  encoding.minor_version (0);

  // Obtain the CDR encapsulation Codec.
  IDL::traits<IOP::Codec>::ref_type codec =
    codec_factory->create_codec (encoding);

  std::wstring tmp (L"I'm Batman");
  CORBA::Any data;
  data <<= tmp;

  TAOX11_TEST_DEBUG
    << "Testing the IOP::Codec::FormatMismatch exception encode()/encode_value()"
    << std::endl;
  TAOX11_TEST_DEBUG
    << "========================================================================"
    << std::endl;

  try
  {
    codec->encode (data);
    TAOX11_TEST_ERROR << "ERROR: No InvalidTypeForEncoding exception caught when "
      "wstring is supplied to 'encode' on an 1.0 codec." << std::endl;
    ++retval;
  }
  catch (const IOP::Codec::InvalidTypeForEncoding &ex)
  {
    TAOX11_TEST_DEBUG << "OK (encode): Caught the correct exception: "
      << ex << std::endl;
  }

  try
  {
    codec->encode_value (data);
    TAOX11_TEST_ERROR << "ERROR: No InvalidTypeForEncoding exception caught when "
      "wstring is supplied to 'encode_value' on an 1.0 codec." << std::endl;
    ++retval;
  }
  catch (const IOP::Codec::InvalidTypeForEncoding &ex)
  {
    TAOX11_TEST_DEBUG << "OK (encode_value): Caught the correct exception: "
      << ex << std::endl;
  }
  TAOX11_TEST_DEBUG << std::endl;
  return retval;
}

uint16_t
test_formatmismatch (IDL::traits<IOP::Codec>::ref_type codec)
{
  uint16_t retval = 0;
  CORBA::OctetSeq test;

  TAOX11_TEST_DEBUG
    << "Testing the IOP::Codec::FormatMismatch exception decode()/decode_value()"
    << std::endl;
  TAOX11_TEST_DEBUG
    << "========================================================================"
    << std::endl;

  try
  {
    CORBA::Any decoded_data = codec->decode (test);
    TAOX11_TEST_ERROR << "ERROR: No FormatMismatch exception caught when an "
      "empty OctetSeq is supplied to 'decode_value'." << std::endl;
    ++retval;
  }
  catch (const IOP::Codec::FormatMismatch &ex)
  {
    TAOX11_TEST_DEBUG << "OK (decode_value): Caught the correct exception: "
      << ex << std::endl;
  }

  try
  {
    CORBA::Any decoded_data = codec->decode_value (test, Test::_tc_Foo);
    TAOX11_TEST_ERROR << "ERROR: No FormatMismatch exception caught when an "
      "empty OctetSeq is supplied to 'decode'." << std::endl;
    ++retval;
  }
  catch (const IOP::Codec::FormatMismatch &ex)
  {
    TAOX11_TEST_DEBUG << "OK (decode): Caught the correct exception: "
      << ex << std::endl;
  }
  TAOX11_TEST_DEBUG << std::endl;
  return retval;
}
int
main (int argc, char *argv[])
{
  uint16_t retval = 0;
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv, "my_orb");

      IDL::traits<IOP::CodecFactory>::ref_type tmp =
        IDL::traits<IOP::CodecFactory>::narrow (nullptr);

      if (tmp != nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<IOP::CodecFactory>::narrow "
          << "does not return a nullptr when a nullptr is supplied"
          << std::endl;
        ++retval;
      }

      IDL::traits<CORBA::Object>::ref_type obj =
        orb->resolve_initial_references ("CodecFactory");

      IDL::traits<IOP::CodecFactory>::ref_type codec_factory =
        IDL::traits<IOP::CodecFactory>::narrow (obj);

      if (codec_factory == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: Unable to retrieve the CodecFactory."
          << std::endl;
        return 1;
      }

      retval += test_factory_exceptions (codec_factory);

      // Set up a structure that contains information necessary to
      // create a GIOP 1.1 CDR encapsulation Codec.
      IOP::Encoding_1_2 encoding_1_2;
      encoding_1_2.format (IOP::ENCODING_CDR_ENCAPS);
      encoding_1_2.major_version (1);
      encoding_1_2.minor_version (2);
      encoding_1_2.char_codeset (0x00010001U);
      encoding_1_2.wchar_codeset (0x00010109U);

      // Obtain the CDR encapsulation Codec.
      IDL::traits<IOP::Codec>::ref_type codec_1_2 =
        codec_factory->create_codec_with_codesets (encoding_1_2);

      if (codec_1_2 == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: Retrieved Codec12 seems nil"
          << std::endl;
        ++retval;
      }
      else
      {
        retval += test_narrow (codec_1_2);
        if (!test_codec (codec_1_2))
        {
          ++retval;
        }
      }

      // Set up a structure that contains information necessary to
      // create a GIOP 1.1 CDR encapsulation Codec.
      IOP::Encoding encoding;
      encoding.format (IOP::ENCODING_CDR_ENCAPS);
      encoding.major_version (1);
      encoding.minor_version (1);

      // Obtain the CDR encapsulation Codec.
      IDL::traits<IOP::Codec>::ref_type codec =
        codec_factory->create_codec (encoding);
      if (codec != nullptr)
      {
        if (!test_codec (codec))
          ++retval;
      }
      else
      {
        TAOX11_TEST_ERROR << "ERROR: Retrieved Codec seems nil"
          << std::endl;
        ++retval;
      }

      // The IOP::Codec::TypeMismatch exception is not implemented in TAO.
      // Therefor we're unable to test this here.

      retval += test_invalidtypeforencoding (codec_factory);
      retval += test_formatmismatch (codec);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "ERROR: Caught unexpected exception : "
        << ex << std::endl;
      return 1;
    }

  return retval;
}
