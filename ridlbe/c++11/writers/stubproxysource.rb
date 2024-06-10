#--------------------------------------------------------------------
# @file    stubproxysource.rb
# @author  Johnny Willemsen
#
# @brief   walker class for stub proxy source
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/c++11/writerbase'

module IDL
  module Cxx11
    class IncludeStubProxyDefineVisitor < VisitorBase
      def visit
        visit_template(:include_stub_proxy_define)
      end
    end

    class StubProxySourceBaseWriter < CxxCodeWriterBase
      def initialize(output = STDOUT, opts = {})
        super
        self.template_root = File.join('cli', 'src')
      end
    end

    class StubProxySourceWriter < StubProxySourceBaseWriter
      def initialize(output = STDOUT, opts = {})
        super

        @default_pre_includes = []
        @default_post_includes = [
          'tao/x11/corba.h',
          'tao/x11/anytypecode/typecode_impl.h',
          'tao/x11/anytypecode/typecode.h'
        ]
        unless params[:no_cdr_streaming]
          @default_pre_includes << 'tao/CDR.h'
          @default_post_includes << 'tao/x11/cdr_long_double.h'
        end
        if params[:gen_typecodes]
          @default_pre_includes << 'tao/AnyTypeCode/TypeCode.h'
          @default_pre_includes << 'tao/AnyTypeCode/TypeCode_Constants.h'
        end
      end

      # Object traits are only required for interfaces and valuetypes
      @object_traits_specializations = false
      # Proxy implementation is only required for interfaces
      @proxy_impl = false

      def enter_interface(node)
        super

        @object_traits_specializations = true

        return if node.is_local? || node.is_pseudo? || node.is_abstract?

        @proxy_impl = true
      end

      def enter_valuetype(node)
        @object_traits_specializations = true
        super
      end

      def pre_visit(parser)
        visit_includes(parser)

        visit_tao_typecodes(parser) if params[:gen_typecodes]

        super
      end

      def post_visit(parser)
        # stub proxy implementations
        if @proxy_impl
          visit_proxy_implementation(parser)
        end

        if @object_traits_specializations
          # Object ref traits specializations
          visit_proxy_object_ref_traits_specializations(parser) unless params[:no_stub_proxy_hdr]
        end

        # CDR operators
        visit_cdr(parser) unless params[:no_cdr_streaming]

        super
        visitor(PostVisitor).visit
      end

      def visit_cdr(parser)
        writer(StubProxySourceCDRWriter).visit_nodes(parser)
      end

      def visit_includes(parser)
        writer(StubProxySourceIncludeWriter,
               { default_pre_includes: @default_pre_includes,
                 default_post_includes: @default_post_includes }).visit_nodes(parser)
      end

      def visit_proxy_object_ref_traits_specializations(parser)
        writer(StubSourceProxyObjRefTraitsWriter).visit_nodes(parser)
      end

      def visit_proxy_implementation(parser)
        writer(StubProxySourceProxyImplWriter).visit_nodes(parser)
      end

      def visit_tao_typecodes(parser)
        writer(StubSourceTaoTypecodeWriter).visit_nodes(parser)
      end
    end # StubProxySourceWriter

    class StubProxySourceCDRWriter < StubProxySourceBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
        # CDR operators are generated outside the scopes of the IDL defined types
        # in a common naming scope so do not track scopes such that all typenames
        # will always be generated fully scoped.
        self.disable_scope_tracking = true
      end

      def pre_visit(parser)
        super
        printiln('// generated from StubProxySourceCDRWriter#pre_visit')
        println('TAO_BEGIN_VERSIONED_NAMESPACE_DECL')
      end

      def post_visit(parser)
        println
        println('TAO_END_VERSIONED_NAMESPACE_DECL')
        super
      end

      def enter_interface(node)
        return if node.is_local? || node.is_pseudo? || params[:no_cdr_streaming]

        visitor(InterfaceVisitor).visit_cdr(node)
      end

      def enter_valuetype(node)
        return if node.is_local? || params[:no_cdr_streaming]

        visitor(ValuetypeVisitor).visit_cdr(node)
      end

      def visit_valuebox(node)
        return if node.is_local? || params[:no_cdr_streaming]

        visitor(ValueboxVisitor).visit_cdr(node)
      end

      def enter_struct(node)
        return if node.is_local? || params[:no_cdr_streaming]

        visitor(StructVisitor).visit_cdr(node)
      end

      def enter_union(node)
        return if node.is_local? || params[:no_cdr_streaming]

        visitor(UnionVisitor).visit_cdr(node)
      end

      def enter_exception(node)
        return if params[:no_cdr_streaming]

        visitor(ExceptionVisitor).visit_cdr(node)
      end

      def visit_enum(node)
        return if params[:no_cdr_streaming]

        visitor(EnumVisitor).visit_cdr(node)
      end

      def visit_bitmask(node)
        return if params[:no_cdr_streaming]

        visitor(BitmaskVisitor).visit_cdr(node)
      end

      def visit_bitset(node)
        return if params[:no_cdr_streaming]

        visitor(BitsetVisitor).visit_cdr(node)
      end
    end # StubProxySourceCDRWriter

    class StubProxySourceProxyImplWriter < StubProxySourceBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def enter_module(node)
        super
        println
        printiln('// generated from StubProxySourceProxyImplWriter')
        printiln('namespace ' + node.cxxname)
        printiln('{')
        inc_nest
      end

      def leave_module(node)
        dec_nest
        printiln("} // namespace #{node.cxxname}")
        println
        super
      end

      def enter_interface(node)
        super
        return if node.is_local? || node.is_pseudo? || node.is_abstract?

        visitor(InterfaceVisitor).visit_proxy(node)
        println
      end
    end

    class StubSourceProxyObjRefTraitsWriter < StubProxySourceBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(_parser)
        println
        printiln('// generated from StubSourceProxyObjRefTraitsWriter#pre_visit')
      end

      def post_visit(parser); end

      def enter_interface(node)
        return if node.is_local? || node.is_pseudo? || node.is_abstract?

        visitor(InterfaceVisitor).visit_object_ref_traits(node)
      end
    end

    class StubProxySourceIncludeWriter < StubProxySourceBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
        @default_pre_includes = params[:default_pre_includes]
        @default_post_includes = params[:default_post_includes]
        @includes = []
      end

      def generate_typecodes?
        params[:gen_typecodes] && params[:gen_stub_proxy_source]
      end

      def generate_anyops?
        params[:gen_any_ops] && !params[:gen_anytypecode_source]
      end

      attr_reader :includes

      def post_visit(_parser)
        properties[:pre_includes] = @default_pre_includes
        properties[:post_includes] = @default_post_includes
        properties[:includes] = @includes
        visitor(HeaderVisitor).visit
        visitor(IncludeStubProxyDefineVisitor).visit
        visitor(PreVisitor).visit
      end

      def enter_interface(node)
        add_pre_include('tao/Object_T.h') unless node.is_local?
        add_pre_include('tao/Invocation_Adapter.h') unless node.is_local?
        if generate_typecodes?
          add_pre_include('tao/AnyTypeCode/Objref_TypeCode_Static.h')
        end
        add_post_include('tao/x11/anytypecode/any_basic_impl_t.h') if generate_anyops?
        add_post_include('tao/x11/exception_macros.h') unless node.is_local?
        return if node.is_local? || node.is_pseudo? || node.is_abstract?

        add_post_include('tao/x11/objproxy.h')
        check_idl_type(node.idltype)
      end

      def visit_operation(node)
        return if node.enclosure.is_local? || (node.enclosure.respond_to?(:is_pseudo?) && node.enclosure.is_pseudo?) || (node.enclosure.respond_to?(:is_abstract?) && node.enclosure.is_abstract?)

        check_idl_type(node.idltype)
        node.params.each { |parm| check_idl_type(parm.idltype) }
        unless node.raises.empty?
          add_include('tao/x11/user_exception_proxy_in_t.h')
          add_include('tao/x11/user_exception_proxy_out.h')
          add_include('tao/Exception_Data.h')
        end
      end

      def visit_attribute(node)
        return if node.enclosure.is_local? || (node.enclosure.respond_to?(:is_pseudo?) && node.enclosure.is_pseudo?) || (node.enclosure.respond_to?(:is_abstract?) && node.enclosure.is_abstract?)

        check_idl_type(node.idltype)
        add_include('tao/x11/basic_arguments.h') unless node.readonly ## for void return of setter
        unless node.get_raises.empty? && node.set_raises.empty?
          add_include('tao/x11/user_exception_proxy_in_t.h')
          add_include('tao/x11/user_exception_proxy_out.h')
          add_include('tao/Exception_Data.h')
        end
      end

      def enter_valuetype(node)
        if generate_typecodes?
          add_pre_include('tao/AnyTypeCode/Value_TypeCode_Static.h')
          add_pre_include('tao/AnyTypeCode/TypeCode_Value_Field.h')
          add_pre_include('tao/AnyTypeCode/Recursive_Type_TypeCode.h') if node.is_recursive?
        end
        add_post_include('tao/x11/anytypecode/any_basic_impl_t.h') if generate_anyops?
        add_post_include('tao/x11/anytypecode/typecode.h') # in case not added yet
        add_post_include('tao/x11/valuetype/valuetype_proxies.h') # after typecode includes
        return if node.is_abstract? || node.is_local?

        node.state_members.each { |m| check_idl_type(m.idltype) }
      end

      def visit_valuebox(node)
        add_pre_include('tao/AnyTypeCode/Alias_TypeCode_Static.h') if generate_typecodes?
        add_post_include('tao/x11/anytypecode/any_basic_impl_t.h') if generate_anyops?
        add_post_include('tao/x11/anytypecode/typecode.h') # in case not added yet
        add_post_include('tao/x11/valuetype/valuetype_proxies.h') # after typecode includes
        return if node.is_local?

        check_idl_type(node.boxed_type)
      end

      def enter_struct(node)
        if generate_typecodes?
          add_pre_include('tao/AnyTypeCode/Struct_TypeCode_Static.h')
          add_pre_include('tao/AnyTypeCode/TypeCode_Struct_Field.h')
          add_pre_include('tao/AnyTypeCode/Recursive_Type_TypeCode.h') if node.is_recursive?
        end
        add_post_include('tao/x11/anytypecode/any_dual_impl_t.h') if generate_anyops?
        return if node.is_local?

        # arg template included in P.h
        node.members.each { |m| check_idl_type(m.idltype) }
      end

      def enter_union(node)
        if generate_typecodes?
          add_pre_include('tao/AnyTypeCode/Union_TypeCode_Static.h')
          add_post_include('tao/x11/anytypecode/typecode_case_t.h')
          add_pre_include('tao/AnyTypeCode/Recursive_Type_TypeCode.h') if node.is_recursive?
        end
        add_post_include('tao/x11/anytypecode/any_dual_impl_t.h') if generate_anyops?
        return if node.is_local?

        # arg template included in P.h
        node.members.each { |m| check_idl_type(m.idltype) }
      end

      def enter_exception(node)
        if generate_typecodes?
          add_pre_include('tao/AnyTypeCode/Struct_TypeCode_Static.h')
          add_pre_include('tao/AnyTypeCode/TypeCode_Struct_Field.h')
        end
        add_post_include('tao/x11/anytypecode/any_dual_impl_t.h') if generate_anyops?
        # arg template included in P.h
        node.members.each { |m| check_idl_type(m.idltype) }
      end

      def visit_enum(_node)
        if generate_typecodes?
          add_pre_include('tao/AnyTypeCode/Enum_TypeCode_Static.h')
        end
        add_post_include('tao/x11/anytypecode/any_basic_impl_t.h') if generate_anyops?
      end

      def visit_bitmask(_node)
        if generate_typecodes?
          add_pre_include('tao/AnyTypeCode/Enum_TypeCode_Static.h')
        end
        add_post_include('tao/x11/anytypecode/any_basic_impl_t.h') if generate_anyops?
      end

      def visit_bitset(_node)
        if generate_typecodes?
          add_pre_include('tao/AnyTypeCode/Enum_TypeCode_Static.h')
        end
        add_post_include('tao/x11/anytypecode/any_basic_impl_t.h') if generate_anyops?
      end

      def visit_typedef(node)
        return if node.idltype.resolved_type.is_a?(IDL::Type::Native)

        add_pre_include('tao/AnyTypeCode/Alias_TypeCode_Static.h') if generate_typecodes?
        # just an alias or a sequence, array or fixed?
        unless node.idltype.is_a?(IDL::Type::ScopedName)
          add_post_include('tao/x11/anytypecode/any_dual_impl_t.h') if generate_anyops?
          if generate_typecodes?
            idl_type = node.idltype.resolved_type
            case idl_type
            when IDL::Type::String,
                 IDL::Type::WString
              add_pre_include('tao/AnyTypeCode/String_TypeCode_Static.h')
            when IDL::Type::Sequence,
                 IDL::Type::Array
              add_pre_include('tao/AnyTypeCode/Sequence_TypeCode_Static.h')
              unless node.is_local?
                check_idl_type(idl_type)
                check_idl_type(idl_type.basetype)
              end
            when IDL::Type::Map
              add_pre_include('tao/AnyTypeCode/Sequence_TypeCode_Static.h')
              unless node.is_local?
                check_idl_type(idl_type)
                check_idl_type(idl_type.basetype)
              end
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
          add_include('tao/x11/special_basic_arguments.h') unless params[:no_cdr_streaming]
        when IDL::Type::LongDouble
          add_include('tao/x11/basic_arguments.h') unless params[:no_cdr_streaming]
        when IDL::Type::Integer,
             IDL::Type::Double,
             IDL::Type::Float,
             IDL::Type::Void
          add_include('tao/x11/basic_arguments.h') unless params[:no_cdr_streaming]
        when IDL::Type::String,
             IDL::Type::WString
          add_include('tao/x11/basic_arguments.h') unless params[:no_cdr_streaming]
        when IDL::Type::Object,
             IDL::Type::Interface,
             IDL::Type::Component
          add_include('tao/x11/stub_arg_traits.h') unless params[:no_cdr_streaming]
        when IDL::Type::Sequence
          # arg template included in P.h
          check_idl_type(idl_type.basetype)
        when IDL::Type::Map
          # arg template included in P.h
          check_idl_type(idl_type.keytype)
          check_idl_type(idl_type.valuetype)
        when IDL::Type::Array
          # arg template included in P.h
          check_idl_type(idl_type.basetype)
        when IDL::Type::Servant
          add_include('tao/x11/portable_server/servantbase.h')
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

  end
end
