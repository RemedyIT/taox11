#--------------------------------------------------------------------
# @file    cxx_type.rb
# @author  Martin Corino
#
# @brief   IDL language mapping configuration
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL
  # define language mapping specific type customizations

  class Type
    # IDL typename

    def idltype_name(_scope = nil)
      "#{typename.split('::').last.downcase}"
    end

    def idltype_unescaped_name(scope = nil)
      idltype_name(scope)
    end

    # user space type names

    def cxx_type(scope = nil)
      self.respond_to?(:node) ? ((scope && (scope == node || scope == node.enclosure)) ? node.cxxname : ('::' + node.scoped_cxxname)) : 'void'
    end

    def cxx_in_type(scope = nil)
      (is_pod? || is_reference?) ? cxx_member_type(scope) : "const #{cxx_member_type(scope)}&"
    end

    def cxx_out_type(scope = nil)
      "#{cxx_type(scope)}&"
    end
    alias :cxx_inout_type :cxx_out_type

    def cxx_move_type(scope = nil)
      "#{cxx_type(scope)}&&"
    end

    def cxx_byval_type(scope = nil)
      "#{cxx_type(scope)}"
    end

    def cxx_arg_type(scope = nil)
      cxx_type(scope)
    end

    def cxx_return_type(scope = nil)
      cxx_type(scope)
    end

    def cxx_member_type(scope = nil)
      cxx_type(scope)
    end

    def cxx_member_type_name
      self.respond_to?(:node) ? cxx_type(node.enclosure) : cxx_type
    end

    def cdr_to_type(scope = nil)
      cxx_out_type(scope)
    end

    def cdr_from_type(scope = nil)
      cxx_in_type(scope)
    end

    def resolved_cxx_type(scope = nil)
      cxx_type(scope)
    end

    def resolved_cxx_member_type(scope = nil)
      resolved_cxx_type(scope)
    end

    # type names following interface proxy scopes

    def proxy_cxxtype(scope = nil)
      self.respond_to?(:node) ? ((scope && (scope == node || scope == node.enclosure)) ? node.proxy_cxxname : ('::' + node.scoped_proxy_cxxname)) : 'void'
    end

    def proxy_in_cxxtype(scope = nil)
      "const #{proxy_cxxtype(scope)}&"
    end

    def proxy_out_cxxtype(scope = nil)
      "#{proxy_cxxtype(scope)}&"
    end
    alias :proxy_inout_cxxtype :proxy_out_cxxtype

    def proxy_move_cxxtype(scope = nil)
      "#{proxy_cxxtype(scope)}&&"
    end

    # typecode name methods

    def cxx_typecode
      self.respond_to?(:node) ? "_tc_#{node.cxxname}" : ''
    end

    def scoped_cxx_typecode
      if self.respond_to?(:node)
        ((node.enclosure && !node.enclosure.scopes.empty?) ?
            "#{node.enclosure.scoped_cxxname}::#{cxx_typecode}" :
            cxx_typecode)
      else
        ''
      end
    end

    def is_standard_type?
      false
    end

    # any insertion/extraction operators arg typename
    def cxx_anyop_arg_typename(scope = nil)
      cxx_type(scope)
    end

    def is_pod?
      false
    end

    def is_reference?
      false
    end

    # convert a type specific value to code
    def value_to_s(v, _scope = nil)
      v.to_s
    end

    # formatter for CDR in streaming value expressions
    # i.e. <input cdr> << <value expression>
    def cdr_to_fmt
      '%s'
    end

    # formatter for CDR out streaming value expressions
    # i.e. <input cdr> >> <value expression>
    def cdr_from_fmt
      '%s'
    end

    # formatter for ostream operator expressions
    def os_fmt
      '%s'
    end

    # Construct for triggering zero initialization
    def zero_initializer
      '{}'
    end

    # define cxxtype methods for 'primitive' types
    {
      Char => 'char',
      WChar => 'wchar_t',
      Octet => 'uint8_t',
      TinyShort => 'int8_t',
      UTinyShort => 'uint8_t',
      Short => 'int16_t',
      UShort => 'uint16_t',
      Long => 'int32_t',
      ULong => 'uint32_t',
      LongLong => 'int64_t',
      ULongLong => 'uint64_t',
      Float => 'float',
      Double => 'double',
      LongDouble => 'long double',
      Fixed => 'void*',
      Boolean => 'bool',
    }.each do |_idltype, _cxxtype|
      _idltype.class_eval %Q{
        def cxx_type(scope = nil)
          '#{_cxxtype}'
        end
        def proxy_cxxtype(scope = nil)
          cxx_type
        end
        def cxx_typecode
          "_tc_#{_idltype.name.split('::').last.downcase}"
        end
        def scoped_cxx_typecode
          "TAOX11_NAMESPACE::CORBA::\#{cxx_typecode}"
        end
        def is_standard_type?
          true
        end
      }
    end

    class Void
      def value_to_s(_v, _scope = nil)
        ''
      end

      def cxx_typecode
        '_tc_null'
      end

      def scoped_cxx_typecode
        "TAOX11_NAMESPACE::CORBA::#{cxx_typecode}"
      end

      def is_standard_type?
        true
      end
    end

    # for integer type classes
    # (Octet,(U)TinyShort,(U)Short,(U)Long,(U)LongLong)
    class Integer
      def is_pod?
        true
      end
    end

    class TinyShort
      def value_to_s(v, _scope = nil)
        v.to_s
      end

      def idltype_name(_scope = nil)
        'int8'
      end

      def cdr_to_fmt
        "ACE_InputCDR::to_int8 (#{super})"
      end

      def cdr_from_fmt
        "ACE_OutputCDR::from_int8 (#{super})"
      end
    end

    class Long
      def value_to_s(v, _scope = nil)
        # prevent integer overflow warnings on platforms with 32bit longs
        v == self.min ? "(#{v + 1}-1)" : v.to_s
      end
    end

    class LongLong
      def value_to_s(v, _scope = nil)
        "#{v}LL"
      end

      def idltype_name(_scope = nil)
        'long long'
      end
    end

    class UTinyShort
      def value_to_s(v, _scope = nil)
        v.to_s
      end

      def idltype_name(_scope = nil)
        'uint8'
      end

      def cdr_to_fmt
        "ACE_InputCDR::to_uint8 (#{super})"
      end

      def cdr_from_fmt
        "ACE_OutputCDR::from_uint8 (#{super})"
      end
    end

    class UShort
      def value_to_s(v, _scope = nil)
        "#{v}U"
      end

      def idltype_name(_scope = nil)
        'unsigned short'
      end
    end

    class ULong
      def value_to_s(v, _scope = nil)
        "#{v}UL"
      end

      def idltype_name(_scope = nil)
        'unsigned long'
      end
    end

    class ULongLong
      def value_to_s(v, _scope = nil)
        "#{v}ULL"
      end

      def idltype_name(_scope = nil)
        'unsigned long long'
      end
    end

    # for float classes
    # (Float,Double,LongDouble)
    class Float
      def is_pod?
        true
      end

      def value_to_s(v, _scope = nil)
        "#{v}F"
      end
    end

    class Double
      def value_to_s(v, _scope = nil)
        v.to_s
      end
    end

    class LongDouble
      def idltype_name(_scope = nil)
        'long double'
      end

      def value_to_s(v, _scope = nil)
        "#{v}L"
      end
    end

    class Boolean
      def cxx_arg_type(_scope = nil)
        'ACE_InputCDR::to_boolean'
      end

      def cdr_to_fmt
        "ACE_InputCDR::to_boolean (#{super})"
      end

      def cdr_from_fmt
        "ACE_OutputCDR::from_boolean (#{super})"
      end

      def is_pod?
        true
      end

      def os_fmt
        'std::boolalpha << %s << std::noboolalpha'
      end
    end

    class Char
      PRINTABLES = 0x20...0x7f.freeze
      SPECIALS = ['\\', '\''].freeze
      def cxx_arg_type(_scope = nil)
        'ACE_InputCDR::to_char'
      end

      def value_to_s(v, _scope = nil)
        if PRINTABLES.include?(v)
          if SPECIALS.include?(c = v.chr)
            "'\\#{c}'"
          else
            "'#{c}'"
          end
        else
          "'#{'\x' + v.chr.unpack('H2').first}'"
        end
      end

      def cdr_to_fmt
        "ACE_InputCDR::to_char (#{super})"
      end

      def cdr_from_fmt
        "ACE_OutputCDR::from_char (#{super})"
      end

      def is_pod?
        true
      end

      def os_fmt
        return '"\'" <<  %s << "\'" '
      end
    end

    class WChar
      def cxx_arg_type(_scope = nil)
        'ACE_InputCDR::to_wchar'
      end

      def value_to_s(v, _scope = nil)
        return 'L' + v.to_i.chr.dump.gsub('"', "'") if v.is_a?(::Integer)
        return 'L' + v.to_s[0, 1].dump.gsub('"', "'") unless v.is_a?(::Array)

        'L' + case v.first
        when :char
          "'#{v.last}'"
        when :esc_ch, :esc
          "'\\#{v.last}'"
        when :oct
          "'\\0#{v.last}'"
        when :hex2, :hex4
          "'\\x#{v.last}'"
        else
          ''
        end
      end

      def cdr_to_fmt
        "ACE_InputCDR::to_wchar (#{super})"
      end

      def cdr_from_fmt
        "ACE_OutputCDR::from_wchar (#{super})"
      end

      def is_pod?
        true
      end
    end

    class Octet
      def cxx_arg_type(_scope = nil)
        'ACE_InputCDR::to_octet'
      end

      def cdr_to_fmt
        "ACE_InputCDR::to_octet (#{super})"
      end

      def cdr_from_fmt
        "ACE_OutputCDR::from_octet (#{super})"
      end

      def os_fmt
        ' std::hex << "hex:" << (int)%s << std::dec'
      end
    end

    class String
      def cxx_type(_scope = nil)
        (size.to_i.positive?) ? "TAOX11_IDL::bounded_string<#{size}>" : 'std::string'
      end

      def proxy_cxxtype(_scope = nil)
        cxx_type
      end

      def cxx_typecode
        '_tc_string'
      end

      def scoped_cxx_typecode
        "TAOX11_NAMESPACE::CORBA::#{cxx_typecode}"
      end

      def cxx_member_type_name
        (size.to_i.positive?) ? "bounded_string<#{size}>" : 'string'
      end

      def value_to_s(v, _scope = nil)
        v.dump
      end

      def is_standard_type?
        self.size.to_i.zero?
      end

      def os_fmt
        return ' "\"" <<  %s << "\"" '
      end
    end

    class WString
      def cxx_type(_scope = nil)
        (size.to_i.positive?) ? "TAOX11_IDL::bounded_wstring<#{size}>" : 'std::wstring'
      end

      def proxy_cxxtype(_scope = nil)
        cxx_type
      end

      def cxx_typecode
        '_tc_wstring'
      end

      def scoped_cxx_typecode
        "TAOX11_NAMESPACE::CORBA::#{cxx_typecode}"
      end

      def cxx_member_type_name
        (size.to_i.positive?) ? "bounded_wstring<#{size}>" : 'wstring'
      end

      def value_to_s(v, _scope = nil)
        return 'L' + v.to_s.dump unless v.is_a?(::Array)

        'L"' + v.collect do |(elt, elv)|
          case elt
          when :char
            elv
          when :esc_ch, :esc
            "\\#{elv}"
          when :oct
            "\\0#{elv}"
          when :hex2, :hex4
            "\\x#{elv}"
          else
            ''
          end
        end.join + '"'
      end

      def is_standard_type?
        self.size.to_i.zero?
      end

      def os_fmt
        return ' %s '
      end
    end

    module IdlType_Mixin
     def idltype_name(scope = nil)
       a = (scope && (scope == node)) ? node.name : ('::' + node.scoped_name)
     end

     def idltype_unescaped_name(scope = nil)
       a = (scope && (scope == node)) ? node.unescaped_name : ('::' + node.scoped_unescaped_name)
     end
   end

    module ReferenceType_Mixin
      def cxx_member_type(scope = nil, typedef = nil)
        "IDL::traits<#{(typedef || self).cxx_type(scope)}>::ref_type"
      end

      def resolved_cxx_member_type(scope = nil, _typedef = nil)
        "IDL::traits<#{resolved_cxx_type(scope)}>::ref_type"
      end

      def cxx_return_type(scope = nil)
        cxx_member_type(scope)
      end

      def cxx_byval_type(scope = nil)
        cxx_member_type(scope)
      end

      def cxx_out_type(scope = nil)
        "#{cxx_member_type(scope)}&"
      end

      def cxx_inout_type(scope = nil)
        "#{cxx_member_type(scope)}&"
      end

      def cxx_move_type(scope = nil)
        "#{cxx_member_type(scope)}&&"
      end

      def is_reference?
        true
      end
    end

    class Object
      include ReferenceType_Mixin
      def cxx_type(_scope = nil)
        'TAOX11_NAMESPACE::CORBA::Object'
      end

      def cxx_typecode
        '_tc_Object'
      end

      def scoped_cxx_typecode
        'TAOX11_NAMESPACE::CORBA::_tc_Object'
      end

      def cxx_member_type_name
        'object_reference'
      end

      def is_standard_type?
        true
      end

      def idltype_name(_scope = nil)
        'Object'
      end
    end

    class ValueBase
      include ReferenceType_Mixin
      def cxx_type(_scope = nil)
        'TAOX11_NAMESPACE::CORBA::ValueBase'
      end

      def cxx_member_type_name
        'valuetype_reference'
      end

      def cxx_typecode
        '_tc_ValueBase'
      end

      def scoped_cxx_typecode
        'TAOX11_NAMESPACE::CORBA::_tc_ValueBase'
      end

      def is_standard_type?
        true
      end

      def idltype_name(_scope = nil)
        'ValueBase'
      end
    end

    class Any
      def cxx_type(_scope = nil)
        'TAOX11_NAMESPACE::CORBA::Any'
      end

      def proxy_cxxtype(_scope = nil)
        cxx_type
      end

      def cxx_typecode
        '_tc_any'
      end

      def scoped_cxx_typecode
        'TAOX11_NAMESPACE::CORBA::_tc_any'
      end

      def is_standard_type?
        true
      end
    end

    class Native
      def cxx_type(_scope = nil)
        'void*'
      end

      def cxx_member_type(scope = nil, typedef = nil)
        typedef ? typedef.cxx_type(scope) : cxx_type
      end

      def proxy_cxxtype(_scope = nil)
        cxx_type
      end

      def is_reference?
        true
      end

      def cxx_typecode
        ''
      end

      def scoped_cxx_typecode
        ''
      end

      def is_standard_type?
        false
      end
    end

    class Valuetype
      include ReferenceType_Mixin
      include IdlType_Mixin
      def cxx_type(scope = nil)
        (scope && (scope == node || scope == node.enclosure)) ? node.cxxname : ('::' + node.scoped_cxxname)
      end

      def cxx_member_type_name
        'valuetype_reference'
      end

      def proxy_cxxtype(scope = nil)
        (scope && (scope == node || scope == node.enclosure)) ? node.proxy_cxxname : ('::' + node.scoped_proxy_cxxname)
      end
    end

    class Valuebox
      include ReferenceType_Mixin
      include IdlType_Mixin
      def cxx_type(scope = nil)
        (scope && (scope == node || scope == node.enclosure)) ? node.cxxname : ('::' + node.scoped_cxxname)
      end

      def cxx_member_type_name
        'valuetype_reference'
      end

      def proxy_cxxtype(scope = nil)
        (scope && (scope == node || scope == node.enclosure)) ? node.proxy_cxxname : ('::' + node.scoped_proxy_cxxname)
      end

      def boxed_traits_cxx_typename
        node.boxed_type.cxx_type
      end
    end

    class ScopedName
      include IdlType_Mixin
      def cxx_type(scope = nil)
        (scope && (scope == node || scope == node.enclosure)) ? node.cxxname : ('::' + node.scoped_cxxname)
      end

      def resolved_cxx_type(scope = nil)
        resolved_type.resolved_cxx_type(scope)
      end

      def proxy_cxxtype(scope = nil)
        (scope && (scope == node || scope == node.enclosure)) ? node.proxy_cxxname : ('::' + node.scoped_proxy_cxxname)
      end

      def cxx_byval_type(scope = nil)
        is_reference? ? resolved_type.cxx_member_type(scope, self) : super
      end

      def cxx_member_type(scope = nil)
        is_reference? ? resolved_type.cxx_member_type(scope, self) : super
      end

      def resolved_cxx_member_type(scope = nil)
        resolved_type.resolved_cxx_member_type(scope)
      end

      def cxx_member_type_name
        is_reference? ? resolved_type.cxx_member_type_name : super
      end

      def zero_initializer
        resolved_type.zero_initializer
      end

      def cxx_return_type(scope = nil)
        is_reference? ? resolved_type.cxx_member_type(scope, self) : super
      end

      def cxx_in_type(scope = nil)
        is_reference? ? resolved_type.cxx_member_type(scope, self) : super
      end

      def cxx_out_type(scope = nil)
        is_reference? ? "#{resolved_type.cxx_member_type(scope, self)}&" : super
      end

      def cxx_inout_type(scope = nil)
        cxx_out_type(scope)
      end

      def cxx_move_type(scope = nil)
        is_reference? ? "#{resolved_type.cxx_member_type(scope, self)}&&" : super
      end

      def is_pod?
        resolved_type.is_pod?
      end

      def is_reference?
        resolved_type.is_reference?
      end

      def value_to_s(v, scope = nil)
        exp = Expression::Value.new(self.node.idltype, v)
        exp.idltype.value_to_s(exp.value, scope)
      end

      def cxx_arg_type(_scope = nil)
        resolved_type.cxx_arg_type
      end

      def cdr_to_type(scope = nil)
        resolved_type.cdr_to_type(scope)
      end

      def cdr_from_type(scope = nil)
        resolved_type.cdr_from_type(scope)
      end

      def cdr_to_fmt
        resolved_type.cdr_to_fmt
      end

      def cdr_from_fmt
        resolved_type.cdr_from_fmt
      end
    end

    class Interface
      include ReferenceType_Mixin
      include IdlType_Mixin
      def cxx_type(scope = nil)
        (scope && (scope == node || scope == node.enclosure)) ? node.cxxname : ('::' + node.scoped_cxxname)
      end

      def cxx_member_type_name
        node.is_abstract? ? 'abstractbase_reference' : 'object_reference'
      end

      def proxy_cxxtype(scope = nil)
        (scope && (scope == node || scope == node.enclosure)) ? node.proxy_cxxname : ('::' + node.scoped_proxy_cxxname)
      end
    end

    class Struct
      include IdlType_Mixin

      def cxx_type(scope = nil)
        (scope && (scope == node || scope == node.enclosure)) ? node.cxxname : ('::' + node.scoped_cxxname)
      end

      def proxy_cxxtype(scope = nil)
        (scope && (scope == node || scope == node.enclosure)) ? node.proxy_cxxname : ('::' + node.scoped_proxy_cxxname)
      end
    end

    class Union
      include IdlType_Mixin
      def cxx_type(scope = nil)
        (scope && (scope == node || scope == node.enclosure)) ? node.cxxname : ('::' + node.scoped_cxxname)
      end

      def proxy_cxxtype(scope = nil)
        (scope && (scope == node || scope == node.enclosure)) ? node.proxy_cxxname : ('::' + node.scoped_proxy_cxxname)
      end
    end

    class Enum
      include IdlType_Mixin
      def cxx_type(scope = nil)
        (scope && (scope == node || scope == node.enclosure)) ? node.cxxname : ('::' + node.scoped_cxxname)
      end

      def proxy_cxxtype(scope = nil)
        (scope && (scope == node || scope == node.enclosure)) ? node.proxy_cxxname : ('::' + node.scoped_proxy_cxxname)
      end

      def value_to_s(v, scope = nil)
        ((scope && (scope == node.enclosure)) ? node.cxxname : ('::' + node.scoped_cxxname)) + '::' + node.enumerators[v].cxxname
      end

      def is_pod?
        true
      end
    end

    class Bitmask
      include IdlType_Mixin
      def cxx_type(scope = nil)
        (scope && (scope == node || scope == node.enclosure)) ? node.cxxname : ('::' + node.scoped_cxxname)
      end

      def proxy_cxxtype(scope = nil)
        (scope && (scope == node || scope == node.enclosure)) ? node.proxy_cxxname : ('::' + node.scoped_proxy_cxxname)
      end

      def value_to_s(v, scope = nil)
        ((scope && (scope == node.enclosure)) ? node.cxxname : ('::' + node.scoped_cxxname)) + '::' + node.bitvalues[v].cxxname
      end

      def is_pod?
        true
      end
    end

    class Bitset
      include IdlType_Mixin
      def cxx_type(scope = nil)
        (scope && (scope == node || scope == node.enclosure)) ? node.cxxname : ('::' + node.scoped_cxxname)
      end

      def proxy_cxxtype(scope = nil)
        (scope && (scope == node || scope == node.enclosure)) ? node.proxy_cxxname : ('::' + node.scoped_proxy_cxxname)
      end

      def value_to_s(v, scope = nil)
        ((scope && (scope == node.enclosure)) ? node.cxxname : ('::' + node.scoped_cxxname)) + '::' + node.bitvalues[v].cxxname
      end

      def is_pod?
        false
      end
    end

    class Fixed
      def cxx_type(_scope = nil)
        digits.nil? ? 'TAOX11_NAMESPACE::IDL::Fixed' : "TAOX11_NAMESPACE::IDL::Fixed<#{digits}, #{scale}>"
      end

      def proxy_cxxtype(_scope = nil)
        cxx_type
      end

      def value_to_s(v, _scope = nil)
        v.to_s.gsub(/d|D/, '')
      end
    end

    class Sequence
      def cxx_type(scope = nil)
        (size.to_i.positive?) ? "TAOX11_NAMESPACE::IDL::bounded_vector<#{basetype.cxx_member_type(scope)}, #{size}>" : "std::vector<#{basetype.cxx_member_type(scope)}>"
      end

      def proxy_cxxtype(scope = nil)
        (size.to_i.positive?) ? "TAOX11_NAMESPACE::IDL::bounded_vector<#{basetype.proxy_cxxtype(scope)}, #{size}>" : "std::vector<#{basetype.proxy_cxxtype(scope)}>"
      end

      def resolved_cxx_type(scope = nil)
        (size.to_i.positive?) ? "TAOX11_NAMESPACE::IDL::bounded_vector<#{basetype.resolved_cxx_member_type(scope)}, #{size}>" : "std::vector<#{basetype.resolved_cxx_member_type(scope)}>"
      end

      def base_traits_cxx_typename
        basetype.cxx_type
      end

      # any insertion/extraction operators arg typename
      def cxx_anyop_arg_typename(scope = nil)
        resolved_cxx_type(scope)
      end
    end

    class Map
      def cxx_type(scope = nil)
        (size.to_i.positive?) ? "TAOX11_NAMESPACE::IDL::bounded_map<#{keytype.cxx_member_type(scope)}, #{valuetype.cxx_member_type(scope)}, #{size}>" : "std::map<#{keytype.cxx_member_type(scope)}, #{valuetype.cxx_member_type(scope)}>"
      end

      def proxy_cxxtype(scope = nil)
        (size.to_i.positive?) ? "TAOX11_NAMESPACE::IDL::bounded_map<#{keytype.proxy_cxxtype(scope)}, #{valuetype.proxy_cxxtype(scope)}, #{size}>" : "std::map<#{keytype.proxy_cxxtype(scope)}, #{valuetype.proxy_cxxtype(scope)}>"
      end

      def resolved_cxx_type(scope = nil)
        (size.to_i.positive?) ? "TAOX11_NAMESPACE::IDL::bounded_map<#{keytype.resolved_cxx_member_type(scope)}, #{valuetype.resolved_cxx_member_type(scope)}, #{size}>" : "std::map<#{keytype.resolved_cxx_member_type(scope)}, #{valuetype.resolved_cxx_member_type(scope)}>"
      end

      def key_traits_cxx_typename
        keytype.cxx_type
      end

      def value_traits_cxx_typename
        valuetype.cxx_type
      end

      # any insertion/extraction operators arg typename
      def cxx_anyop_arg_typename(scope = nil)
        resolved_cxx_type(scope)
      end
    end

    class Array
      def cxx_type(scope = nil)
        sizes.reverse.inject(basetype.cxx_member_type(scope)) { |typestr, siz| typestr = "std::array<#{typestr}, #{siz}>" }
      end

      def proxy_cxxtype(scope = nil)
        sizes.reverse.inject(basetype.proxy_cxxtype(scope)) { |typestr, siz| typestr = "std::array<#{typestr}, #{siz}>" }
      end

      def resolved_cxx_type(scope = nil)
        sizes.reverse.inject(basetype.resolved_cxx_member_type(scope)) { |typestr, siz| typestr = "std::array<#{typestr}, #{siz}>" }
      end

      def cxx_dim
        sizes.empty? ? '[]' : sizes.collect { |s| "[#{s.to_s}]" }.join
      end

      def base_traits_cxx_typename
        basetype.cxx_type
      end

      # any insertion/extraction operators arg typename
      def cxx_anyop_arg_typename(scope = nil)
        resolved_cxx_type(scope)
      end

      def zero_initializer
        '{{}}'
      end
    end
  end # Type
end # IDL
