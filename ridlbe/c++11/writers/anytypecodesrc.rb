#--------------------------------------------------------------------
# @file    anytypecodesrc.rb
# @author  Marijke Hengstmengel
#
# @brief   walker class for Any and TypeCode support
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/c++11/writers/stubsource'

module IDL
  module Cxx11
    class AnyTypeCodeBaseWriter < CxxCodeWriterBase
      def initialize(output = STDOUT, opts = {})
        super
        self.template_root = File.join('cli', 'src', 'anytc')
      end
    end

    class AnyTypeCodeWriter < AnyTypeCodeBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
        @default_pre_includes = [
          'tao/CDR.h',
          'tao/Invocation_Adapter.h',
          'tao/Object_T.h'
        ]
        if params[:gen_typecodes]
          @default_pre_includes << 'tao/AnyTypeCode/TypeCode.h'
          @default_pre_includes << 'tao/AnyTypeCode/TypeCode_Constants.h'
        end
        if params[:gen_any_ops]
          @default_pre_includes << 'tao/AnyTypeCode/Any_Impl_T.h'
        end
        @default_post_includes = [
          'tao/x11/tao_corba.h',
          'tao/x11/objproxy.h',
          'tao/x11/system_exception_x11.h',
          'tao/x11/exception_macros.h',
          'tao/x11/anytypecode/typecode_impl.h',
          'tao/x11/anytypecode/typecode.h'
        ]
      end

      def pre_visit(parser)
        visit_includes(parser)

        visit_typecodes(parser)

        super
      end

      def post_visit(parser)
        visit_anyops(parser)

        super
        visitor(PostVisitor).visit
      end

      def visit_includes(parser)
        writer(AnyTypeCodeIncludeWriter,
               { default_pre_includes: @default_pre_includes,
                 default_post_includes: @default_post_includes }).visit_nodes(parser)
      end

      def visit_anyops(parser)
        writer(StubSourceAnyOpWriter).visit_nodes(parser)
      end

      def visit_typecodes(parser)
        writer(StubSourceTypecodeWriter).visit_nodes(parser)
      end
    end # AnyTypeCodeWriter

    class AnyTypeCodeIncludeWriter < AnyTypeCodeBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
        @default_pre_includes = params[:default_pre_includes]
        @default_post_includes = params[:default_post_includes]
        @includes = []
      end

      attr_reader :includes

      def post_visit(_parser)
        properties[:pre_includes] = @default_pre_includes
        properties[:post_includes] = @default_post_includes
        properties[:includes] = @includes
        visitor(PreVisitor).visit
      end

      def enter_interface(node)
        add_pre_include('tao/AnyTypeCode/Objref_TypeCode_Static.h') if params[:gen_typecodes]
        add_post_include('tao/x11/anytypecode/any_basic_impl_t.h') if params[:gen_any_ops]
        return if node.is_local? || node.is_pseudo? || node.is_abstract?

        check_idl_type(node.idltype)
      end

      def enter_valuetype(node)
        add_pre_include('tao/AnyTypeCode/Value_TypeCode_Static.h') if params[:gen_typecodes]
        add_pre_include('tao/AnyTypeCode/TypeCode_Value_Field.h') if params[:gen_typecodes]
        add_post_include('tao/x11/anytypecode/any_basic_impl_t.h') if params[:gen_any_ops]
        return if node.is_abstract? || node.is_local?

        node.state_members.each { |m| check_idl_type(m.idltype) }
      end

      def visit_valuebox(node)
        add_pre_include('tao/AnyTypeCode/Alias_TypeCode_Static.h') if params[:gen_typecodes]
        add_post_include('tao/x11/anytypecode/any_basic_impl_t.h') if params[:gen_any_ops]
        return if node.is_local?

        check_idl_type(node.boxed_type)
      end

      def enter_struct(node)
        add_pre_include('tao/AnyTypeCode/Struct_TypeCode_Static.h') if params[:gen_typecodes]
        add_pre_include('tao/AnyTypeCode/TypeCode_Struct_Field.h') if params[:gen_typecodes]
        add_post_include('tao/x11/anytypecode/any_dual_impl_t.h') if params[:gen_any_ops]
        return if node.is_local?

        # arg template included in P.h
        node.members.each { |m| check_idl_type(m.idltype) }
      end

      def enter_union(node)
        add_pre_include('tao/AnyTypeCode/Union_TypeCode_Static.h') if params[:gen_typecodes]
        add_pre_include('tao/AnyTypeCode/TypeCode_Case_T.h') if params[:gen_typecodes]
        add_post_include('tao/x11/anytypecode/any_dual_impl_t.h') if params[:gen_any_ops]
        add_post_include('tao/x11/anytypecode/typecode_case_t.h') if params[:gen_any_ops]
        return if node.is_local?

        # arg template included in P.h
        node.members.each { |m| check_idl_type(m.idltype) }
      end

      def enter_exception(node)
        add_pre_include('tao/AnyTypeCode/Struct_TypeCode_Static.h') if params[:gen_typecodes]
        add_pre_include('tao/AnyTypeCode/TypeCode_Struct_Field.h') if params[:gen_typecodes]
        add_post_include('tao/x11/anytypecode/any_dual_impl_t.h') if params[:gen_any_ops]
        # arg template included in P.h
        node.members.each { |m| check_idl_type(m.idltype) }
      end

      def visit_enum(_node)
        add_pre_include('tao/AnyTypeCode/Enum_TypeCode_Static.h') if params[:gen_typecodes]
        add_post_include('tao/x11/anytypecode/any_basic_impl_t.h') if params[:gen_any_ops]
      end

      def visit_bitmask(_node)
        add_pre_include('tao/AnyTypeCode/Enum_TypeCode_Static.h') if params[:gen_typecodes]
        add_post_include('tao/x11/anytypecode/any_basic_impl_t.h') if params[:gen_any_ops]
      end

      def visit_bitset(_node)
        add_pre_include('tao/AnyTypeCode/Enum_TypeCode_Static.h') if params[:gen_typecodes]
        add_post_include('tao/x11/anytypecode/any_basic_impl_t.h') if params[:gen_any_ops]
      end

      def visit_typedef(node)
        return if node.idltype.resolved_type.is_a?(IDL::Type::Native)

        add_pre_include('tao/AnyTypeCode/Alias_TypeCode_Static.h') if params[:gen_typecodes]
        # just an alias or a sequence, array or fixed?
        unless node.idltype.is_a?(IDL::Type::ScopedName)
          add_post_include('tao/x11/anytypecode/any_dual_impl_t.h') if params[:gen_typecodes]
          idl_type = node.idltype.resolved_type
          case idl_type
          when IDL::Type::String,
               IDL::Type::WString
            add_pre_include('tao/AnyTypeCode/String_TypeCode_Static.h') if params[:gen_typecodes]
          when IDL::Type::Sequence,
               IDL::Type::Array
            add_pre_include('tao/AnyTypeCode/Sequence_TypeCode_Static.h') if params[:gen_typecodes]
            unless node.is_local?
              check_idl_type(idl_type)
              check_idl_type(idl_type.basetype)
            end
          when IDL::Type::Map
            add_pre_include('tao/AnyTypeCode/Sequence_TypeCode_Static.h') if params[:gen_typecodes]
            unless node.is_local?
              check_idl_type(idl_type)
              check_idl_type(idl_type.basetype)
            end
          end
        end
      end

      private

      def check_idl_type(idl_type)
        idl_type = idl_type.resolved_type
        case idl_type
        when IDL::Type::Char,
             IDL::Type::Boolean,
             IDL::Type::WChar,
             IDL::Type::Octet
        when IDL::Type::LongDouble
        when IDL::Type::Integer,
             IDL::Type::Double,
             IDL::Type::Float,
             IDL::Type::Void
        when IDL::Type::Enum,
             IDL::Type::BitMask,
             IDL::Type::BitSet
        when IDL::Type::String
        when IDL::Type::WString
        when IDL::Type::Object,
             IDL::Type::Interface
        when IDL::Type::Any
        when IDL::Type::Valuebox,
             IDL::Type::Valuetype
          # arg template included in P.h
        when IDL::Type::Struct,
             IDL::Type::Union
          # arg template included in P.h
        when IDL::Type::Sequence
          # arg template included in P.h
          check_idl_type(idl_type.basetype)
        when IDL::Type::Map
          # arg template included in P.h
          check_idl_type(idl_type.basetype)
        when IDL::Type::Array
          # arg template included in P.h
          check_idl_type(idl_type.basetype)
        end
      end

      def add_include(inc_file)
        @includes << inc_file unless @includes.include?(inc_file) ||
                                     @default_pre_includes.include?(inc_file) ||
                                     @default_post_includes.include?(inc_file)
      end

      def add_pre_include(inc_file)
        @default_pre_includes << inc_file unless @includes.include?(inc_file) ||
                                     @default_pre_includes.include?(inc_file) ||
                                     @default_post_includes.include?(inc_file)
      end

      def add_post_include(inc_file)
        @default_post_includes << inc_file unless @includes.include?(inc_file) ||
                                     @default_pre_includes.include?(inc_file) ||
                                     @default_post_includes.include?(inc_file)
      end
    end
  end # Cxx11
end # IDL
