#--------------------------------------------------------------------
# @file    stubproxy.rb
# @author  Martin Corino
#
# @brief   walker class for stub header
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/c++11/writerbase'
require 'ridlbe/c++11/writers/helpers/version_helper'
require 'ridlbe/c++11/writers/helpers/include_guard_helper'

module IDL
  module Cxx11
    class StubProxyBaseWriter < CxxCodeWriterBase
      def initialize(output = STDOUT, opts = {})
        super
        self.template_root = File.join('cli', 'prx')
      end

      attr_accessor :include_guard
    end

    class StubProxyWriter < StubProxyBaseWriter
      helper Cxx11::IncludeGuardHelper

      def initialize(output = STDOUT, opts = {})
        super
        @default_pre_includes = []
        @default_post_includes = []
      end

      def visit_nodes(parser)
        @include_guard = "__RIDL_#{File.basename(params[:output_prx] || '').to_random_include_guard}_INCLUDED__"
        super
        visitor(PostVisitor).visit
      end

      def pre_visit(parser)
        visit_includes(parser)

        super
      end

      def post_visit(parser)
        visit_obj_var_out_specializations(parser)
        visit_obj_ref_traits_specializations(parser)

        # CDR operators
        visit_cdr(parser) unless params[:no_cdr_streaming]

        visit_typecodes(parser) if params[:gen_typecodes]
        super
      end

      def enter_module(node)
        super
        println
        printiln('// generated from StubProxyWriter#enter_module')
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

        println
        printiln('// generated from StubProxyWriter#enter_interface')
        visitor(InterfaceVisitor).visit_pre(node)
        inc_nest
      end

      def leave_interface(node)
        unless node.is_local? || node.is_pseudo? || node.is_abstract?
          dec_nest
          visitor(InterfaceVisitor).visit_post(node)
        end
        super
      end

      def visit_includes(parser)
        writer(StubProxyIncludeWriter,
               { default_pre_includes: @default_pre_includes,
                 default_post_includes: @default_post_includes }) do |w|
          w.include_guard = @include_guard
          w.visit_nodes(parser)
        end
      end

      def visit_cdr(parser)
        writer(StubProxyCDRWriter).visit_nodes(parser) unless params[:no_cdr_streaming]
      end

      def visit_obj_var_out_specializations(parser)
        writer(StubProxyVarOutWriter).visit_nodes(parser)
      end

      def visit_obj_ref_traits_specializations(parser)
        writer(StubProxyObjRefTraitsWriter).visit_nodes(parser)
      end

      def visit_typecodes(parser)
        writer(StubProxyTypecodeWriter).visit_nodes(parser)
      end
    end # StubProxyWriter

    class StubProxyIncludeWriter < StubProxyBaseWriter
      helper Cxx11::VersionHelper
      helper Cxx11::IncludeGuardHelper

      def initialize(output = STDOUT, opts = {})
        super
        @default_pre_includes = params[:default_pre_includes]
        @default_post_includes = params[:default_post_includes]
        @includes = []
        @includes << 'tao/x11/anyinsert_policy_t.h' unless params[:no_cdr_streaming]
      end

      attr_reader :includes

      def post_visit(_parser)
        properties[:pre_includes] = @default_pre_includes
        properties[:post_includes] = @default_post_includes
        properties[:includes] = @includes
        visitor(PreVisitor).visit
      end

      def enter_valuetype(_node)
        # interfaces ALWAYS provide sequence cdr definitions (forward decl issue)
        add_include('tao/Valuetype/Value_VarOut_T.h')
      end

      def declare_interface(_node)
        # interfaces ALWAYS provide sequence cdr definitions (forward decl issue)
        add_include('tao/x11/sequence_cdr_t.h') unless params[:no_cdr_streaming]
        add_include('tao/x11/basic_argument_t.h')
      end

      def enter_interface(node)
        return if node.is_local? || node.is_pseudo?

        check_idl_type(node.idltype) unless node.is_abstract?
        # interfaces ALWAYS provide sequence cdr definitions (forward decl issue)
        add_include('tao/x11/sequence_cdr_t.h') unless params[:no_cdr_streaming]
      end

      def visit_operation(node)
        return if node.enclosure.is_local? || (node.enclosure.respond_to?(:is_pseudo?) && node.enclosure.is_pseudo?)

        check_idl_type(node.idltype)
        node.params.each { |parm| check_idl_type(parm.idltype) }
      end

      def visit_attribute(node)
        return if node.enclosure.is_local? || (node.enclosure.respond_to?(:is_pseudo?) && node.enclosure.is_pseudo?)

        check_idl_type(node.idltype)
      end

      def visit_valuetype(node)
        return if node.is_local?

        add_include('tao/x11/basic_argument_t.h')
        node.state_members.each { |m| check_idl_type(m.idltype) }
      end

      def visit_valuebox(node)
        return if node.is_local?

        add_include('tao/x11/basic_argument_t.h')
        check_idl_type(node.boxed_type)
      end

      def enter_struct(node)
        return if node.is_local?

        add_include('tao/x11/basic_argument_t.h')
        node.members.each { |m| check_idl_type(m.idltype) }
      end

      def enter_union(node)
        return if node.is_local?

        add_include('tao/x11/basic_argument_t.h')
        node.members.each { |m| check_idl_type(m.idltype) }
      end

      def visit_enum(_node)
        add_include('tao/x11/basic_argument_t.h')
      end

      def visit_bitmask(_node)
        add_include('tao/x11/basic_argument_t.h')
      end

      def visit_bitset(_node)
        add_include('tao/x11/basic_argument_t.h')
      end

      def visit_typedef(node)
        return if node.is_local?

        idl_type = node.idltype.resolved_type
        case idl_type
        when IDL::Type::Sequence
          add_include('tao/x11/sequence_cdr_t.h') unless params[:no_cdr_streaming]
          check_idl_type(idl_type)
        when IDL::Type::Map
          add_include('tao/x11/map_cdr_t.h') unless params[:no_cdr_streaming]
          check_idl_type(idl_type)
        when IDL::Type::Array
          add_include('tao/x11/array_cdr_t.h') unless params[:no_cdr_streaming]
          check_idl_type(idl_type)
        end
      end

      private

      def check_idl_type(idl_type)
        idl_type = idl_type.resolved_type
        case idl_type
        when IDL::Type::Enum
          add_include('tao/x11/basic_argument_t.h')
        when IDL::Type::Any
          add_include('tao/x11/anytypecode/any_arg_traits.h')
        when IDL::Type::Interface
          add_include('tao/x11/basic_argument_t.h')
        when IDL::Type::Struct,
             IDL::Type::Union
          add_include('tao/x11/basic_argument_t.h')
        when IDL::AST::Valuetype,
             IDL::AST::Valuebox
          add_include('tao/x11/basic_argument_t.h')
        when IDL::Type::Sequence
          add_include('tao/x11/basic_argument_t.h')
          check_idl_type(idl_type.basetype)
        when IDL::Type::Map
          add_include('tao/x11/basic_argument_t.h')
          check_idl_type(idl_type.keytype)
          check_idl_type(idl_type.valuetype)
        when IDL::Type::Array
          add_include('tao/x11/basic_argument_t.h')
          check_idl_type(idl_type.basetype)
        when IDL::Type::Valuetype
          add_include('tao/Valuetype/Value_VarOut_T.h')
        end
      end

      def add_include(inc_file)
        @includes << inc_file unless @includes.include?(inc_file) ||
                                     @default_pre_includes.include?(inc_file) ||
                                     @default_post_includes.include?(inc_file)
      end
    end

    class StubProxyCDRWriter < StubProxyBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(parser)
        super
        printiln('// generated from StubProxyCDRWriter#pre_visit')
      end

      def post_visit(parser)
        println
        super
      end

      def declare_interface(node)
        return if node.is_local? || node.is_pseudo? || params[:no_cdr_streaming]

        visitor(InterfaceVisitor).visit_cdr(node)
      end

      def enter_interface(node)
        return if node.is_local? || node.is_pseudo? || params[:no_cdr_streaming]

        visitor(InterfaceVisitor).visit_cdr(node)
      end

      def declare_valuetype(node)
        return if params[:no_cdr_streaming]

        visitor(ValuetypeVisitor).visit_cdr(node)
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

      def visit_typedef(node)
        return if node.is_local? || params[:no_cdr_streaming]
        # nothing to do if this is just an alias for another defined type
        return if node.idltype.is_a?(IDL::Type::ScopedName) || node.idltype.resolved_type.is_standard_type?

        idl_type = node.idltype.resolved_type
        case idl_type
        when IDL::Type::Sequence
          visitor(SequenceVisitor).visit_cdr(node)
        when IDL::Type::Map
          visitor(MapVisitor).visit_cdr(node)
        when IDL::Type::Array
          visitor(ArrayVisitor).visit_cdr(node)
        when IDL::Type::String, IDL::Type::WString
          visitor(StringVisitor).visit_cdr(node) # only bounded, unbounded is standard_type
        end
      end
    end # StubProxyCDRWriter

    class StubProxyVarOutWriter < StubProxyBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(_parser)
        printiln('// generated from StubProxyVarOutWriter#pre_visit')
      end

      def post_visit(_parser)
        printiln
      end

      def enter_module(node)
         enter_scope(node)
      end

       def leave_module(node)
         leave_scope(node)
       end

      def enter_interface(node)
         return if node.is_local? || node.is_pseudo? || node.is_abstract?

         visitor(InterfaceVisitor).visit_object_var(node)
      end

      private

      def enter_scope(node)
        printiln('// generated from StubProxyVarOutWriter#enter_scope')
        printiln('namespace ' + node.cxxname)
        printiln('{')
        inc_nest
      end

      def leave_scope(node)
        dec_nest
        printiln("} // namespace #{node.cxxname}")
      end
    end # StubProxyVarOutWriter

    class StubProxyObjRefTraitsWriter < StubProxyBaseWriter
       def initialize(output = STDOUT, opts = {})
         super
       end

       def pre_visit(_parser)
         println
         printiln('// generated from StubProxyObjRefTraitsWriter#pre_visit')
       end

       def post_visit(parser); end

       def enter_interface(node)
         return if node.is_local? || node.is_pseudo? || node.is_abstract?

         visitor(InterfaceVisitor).visit_object_ref_traits(node)
       end
    end # StubProxyObjRefTraitsWriter

    class StubProxyTypecodeWriter < StubProxyBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(parser)
        super
        println
        printiln('// generated from StubProxyTypecodeWriter#pre_visit')
        println('namespace __tao')
        println('{')
        inc_nest
      end

      def post_visit(parser)
        dec_nest
        println
        println('} // namespace __tao')
        super
      end

      private

      def enter_scope(node)
        println
        printiln('// generated from StubProxyTypecodeWriter#enter_scope')
        printiln('namespace ' + node.cxxname)
        printiln('{')
        inc_nest
      end

      def leave_scope(node)
        dec_nest
        printiln("} // namespace #{node.cxxname}")
      end

      public

      def enter_module(node)
        enter_scope(node)
      end

      def leave_module(node)
        leave_scope(node)
      end

      def declare_interface(node)
        visitor(InterfaceVisitor).visit_typecode(node)
      end

      def enter_interface(node)
        visitor(InterfaceVisitor).visit_typecode(node)
        enter_scope(node)
      end

      def leave_interface(node)
        leave_scope(node)
      end

      def declare_valuetype(node)
        visitor(ValuetypeVisitor).visit_typecode(node)
      end

      def enter_valuetype(node)
        visitor(ValuetypeVisitor).visit_typecode(node)
        enter_scope(node)
      end

      def leave_valuetype(node)
        leave_scope(node)
      end

      def visit_valuebox(node)
        visitor(ValueboxVisitor).visit_typecode(node)
      end

      def declare_struct(node)
        visitor(StructVisitor).visit_typecode(node)
      end

      def enter_struct(node)
        visitor(StructVisitor).visit_typecode(node)
        enter_scope(node)
      end

      def leave_struct(node)
        leave_scope(node)
      end

      def declare_union(node)
        visitor(UnionVisitor).visit_typecode(node)
      end

      def enter_union(node)
        visitor(UnionVisitor).visit_typecode(node)
        enter_scope(node)
      end

      def leave_union(node)
        leave_scope(node)
      end

      def enter_exception(node)
        visitor(ExceptionVisitor).visit_typecode(node)
        enter_scope(node)
      end

      def leave_exception(node)
        leave_scope(node)
      end

      def visit_enum(node)
        visitor(EnumVisitor).visit_typecode(node)
      end

      def visit_bitmask(node)
        visitor(BitmaskVisitor).visit_typecode(node)
      end

      def visit_bitset(node)
        visitor(BitsetVisitor).visit_typecode(node)
      end

      def visit_typedef(node)
        return if node.idltype.resolved_type.is_a?(IDL::Type::Native)

        visitor(TypedefVisitor).visit_typecode(node)
      end
    end # StubProxyTypecodeWriter
  end # Cxx11
end # IDL
