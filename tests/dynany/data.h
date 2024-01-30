/**
 * @file    data.h
 * @author  Marijke Hengstmengel
 *
 * @brief   Header file for the class containing test constants
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/basic_traits.h"
#include "tao/x11/orb.h"
#include "tao/x11/object.h"


class Data
{
public:
  Data (IDL::traits<CORBA::ORB>::ref_type orb);
  ~Data () = default;

  bool        m_bool1 {true},        m_bool2 {false};
  uint8_t     m_octet1 {8},          m_octet2 {0};
  char        m_char1 {'z'},         m_char2 {'\0'};
  int16_t     m_short1 {-5},         m_short2 {0};
  int32_t     m_long1 {-123456},     m_long2 {0},  m_long3 {101};
  uint16_t    m_ushort1 {5},         m_ushort2 {0};
  uint32_t    m_ulong1 {123456},     m_ulong2 {0};
  float       m_float1 {0.142857f},  m_float2 {0.0f};
  double      m_double1 {3.14159},   m_double2 {0.0};
  int64_t     m_longlong1 {654321},  m_longlong2 {0};
  uint64_t    m_ulonglong1 {654321}, m_ulonglong2 {0};
  std::string        m_string1 {"upchuck"};
  std::string        m_string2 {""};
  IDL::traits<CORBA::TypeCode>::ref_type   m_typecode1 {CORBA::_tc_long},  m_typecode2 {CORBA::_tc_null};
  wchar_t           m_wchar1 {L'A'},m_wchar2 {'\0'};
  IDL::traits<CORBA::Object>::ref_type    m_objref1,    m_objref2;
  CORBA::ShortSeq   m_shortseq1 {0,1,2};
  CORBA::ShortSeq   m_shortseq2;
  CORBA::UShortSeq  m_ushortseq1 {0,1,2};
  CORBA::UShortSeq  m_ushortseq2;
  long double     m_longdouble1 {88888888888.8888}, m_longdouble2 {99999999999.9999};

  std::string const labels[24] ={"type boolean","type octet","type char", "type short", "type long",
                                 "type ushort", "type ulong","type float", "type double","type longlong",
                                 "type ulonglong","type string","type typecode","type wchar","type any",
                                 "type objref","type short sequence", "type ushort sequence", "type longdouble", "type enum",
                                 "typedef'd enum in struct", "type ValueBox", "type Value", "type abstract"};

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
};
