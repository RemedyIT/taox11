#--------------------------------------------------------------------
# @file    amistubproxy.rb
# @author  Marijke Hengstmengel
#
# @brief   walker class for stub header
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/c++11/writerbase'
require 'ridlbe/c++11/writers/amistubsource'
require 'ridlbe/c++11/writers/helpers/version_helper'
require 'ridlbe/c++11/writers/helpers/include_guard_helper'

module IDL
  module Cxx11
    class AmiStubProxyBaseWriter < AmiBaseWriter # CxxCodeWriterBase
      def initialize(output = STDOUT, opts = {})
        super
         self.template_root = File.join('cli', 'prx', 'ami')
      end

      attr_accessor :include_guard
    end

    class AmiStubProxyWriter < AmiStubProxyBaseWriter
      helper Cxx11::IncludeGuardHelper

      def initialize(output = STDOUT, opts = {})
        super
        @default_pre_includes = []
        @default_post_includes = []
      end

      def visit_nodes(parser)
        @include_guard = "__RIDL_AMI_#{File.basename(params[:output_prx] || '').to_random_include_guard}_INCLUDED__"
        super
        visitor(PostVisitor).visit
      end

      def pre_visit(parser)
        visit_includes(parser)
        super
      end

      def post_visit(parser)
        # traits specialization declarations
        visit_obj_var_out_specializations(parser)
        visit_obj_ref_traits_specializations(parser)

        # CDR operators
        visit_cdr(parser)

        visit_typecodes(parser) if params[:gen_typecodes]

        # skeleton functionality  ReplyHandler
        visit_servant_proxy(parser)

        super
      end

      def enter_module(node)
        super
        println
        printiln('// generated from AmiStubProxyWriter#enter_module')
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
        return unless needs_ami_generation?(node)

        println
        printiln('// generated from AmiStubProxyWriter#enter_interface')
        ami_handler_interface_with_ami_inheritance.visit_pre(node)
        inc_nest
      end

      def leave_interface(node)
        if needs_ami_generation?(node)
          dec_nest
          ami_handler_interface.visit_post(node)
        end
        super
      end

      def visit_operation(node)
        return if !node.enclosure.is_a?(IDL::AST::Interface) ||
        !needs_ami_generation?(node.enclosure)

        visitor(OperationVisitor).visit_operation(node)
      end

       def visit_attribute(node)
         return if !node.enclosure.is_a?(IDL::AST::Interface) ||
         !needs_ami_generation?(node.enclosure)

         visitor(AttributeVisitor).visit_attribute(node)
       end

      def visit_includes(parser)
        writer(AmiStubProxyIncludeWriter,
               { default_pre_includes: @default_pre_includes,
                 default_post_includes: @default_post_includes }) do |w|
          w.include_guard = @include_guard
          w.visit_nodes(parser)
        end
      end

      def visit_cdr(parser)
        writer(AmiStubProxyCDRWriter).visit_nodes(parser) unless params[:no_cdr_streaming]
      end

      def visit_typecodes(parser)
        writer(AmiStubProxyTypecodeWriter).visit_nodes(parser)
      end

      def visit_obj_var_out_specializations(parser)
        writer(AmiStubProxyVarOutWriter).visit_nodes(parser)
      end

      def visit_obj_ref_traits_specializations(parser)
         writer(AmiStubProxyObjRefTraitsWriter).visit_nodes(parser)
      end

      def visit_servant_proxy(parser)
        writer(AmiStubProxySrvWriter).visit_nodes(parser)
      end
    end # AmiStubProxyWriter

    class AmiStubProxyIncludeWriter < AmiStubProxyBaseWriter
      helper Cxx11::VersionHelper
      helper Cxx11::IncludeGuardHelper

      def initialize(output = STDOUT, opts = {})
        super
        @default_pre_includes = params[:default_pre_includes]
        @default_post_includes = params[:default_post_includes]
        @includes = []
        @includes << 'tao/x11/anyinsert_policy_t.h'
      end

      attr_reader :includes

      def post_visit(_parser)
        properties[:pre_includes] = @default_pre_includes
        properties[:post_includes] = @default_post_includes
        properties[:includes] = @includes
        visitor(PreVisitor).visit
      end

      def enter_valuetype(node)
        # interfaces ALWAYS provide sequence cdr definitions (forward decl issue)
        return if node.is_local?

        add_include('tao/Valuetype/Value_VarOut_T.h')
      end

      def declare_interface(node)
        return unless needs_ami_generation?(node)

        # interfaces ALWAYS provide sequence cdr definitions (forward decl issue)
        add_include('tao/x11/sequence_cdr_t.h') unless params[:no_cdr_streaming]
        add_include('tao/x11/basic_argument_t.h')
      end

      def enter_interface(node)
        return unless needs_ami_generation?(node)

        check_idl_type(node.idltype) unless node.is_abstract?
        # interfaces ALWAYS provide sequence cdr definitions (forward decl issue)
        add_include('tao/x11/sequence_cdr_t.h') unless params[:no_cdr_streaming]
      end

      def visit_operation(node)
        return unless needs_ami_generation?(node.enclosure)

        check_idl_type(node.idltype)
        node.params.each { |parm| check_idl_type(parm.idltype) }
      end

      def visit_attribute(node)
        return unless needs_ami_generation?(node.enclosure)

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
        when IDL::Type::Sequence,
             IDL::Type::Array,
             IDL::Type::Map
          check_idl_type(idl_type)
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
        when IDL::Type::Integer,
             IDL::Type::Double,
             IDL::Type::Float,
             IDL::Type::Void
        when IDL::Type::Enum,
             IDL::Type::BitMask
             IDL::Type::BitSet
          add_include('tao/x11/basic_argument_t.h')
        when IDL::Type::String
        when IDL::Type::WString
        when IDL::Type::Interface
          add_include('tao/x11/basic_argument_t.h')
        when IDL::Type::Object
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
          check_idl_type(idl_type.basetype)
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

    class AmiStubProxyCDRWriter < AmiStubProxyBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(parser)
        super
        printiln('// generated from AmiStubProxyCDRWriter#pre_visit')
      end

      def post_visit(parser)
        println
        super
      end

      def declare_interface(node)
        return unless needs_ami_generation?(node)

        ami_handler_interface.visit_cdr(node)
      end

      def enter_interface(node)
        return unless needs_ami_generation?(node)

        ami_handler_interface.visit_cdr(node)
      end
    end # AmiStubProxyCDRWriter

    class AmiStubProxyVarOutWriter < AmiStubProxyBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(_parser)
        printiln('// generated from AmiStubProxyVarOutWriter#pre_visit')
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
        return unless needs_ami_generation?(node)

        ami_handler_interface.visit_object_var(node)
      end

      private

      def enter_scope(node)
        printiln('// generated from AmiStubProxyVarOutWriter#enter_scope')
        printiln('namespace ' + node.cxxname)
        printiln('{')
        inc_nest
      end

      def leave_scope(node)
        dec_nest
        printiln("} // namespace #{node.cxxname}")
      end
    end # AmiStubProxyVarOutWriter

    class AmiStubProxyObjRefTraitsWriter < AmiStubProxyBaseWriter
       def initialize(output = STDOUT, opts = {})
         super
       end

       def pre_visit(_parser)
         println
         printiln('// generated from AmiStubProxyObjRefTraitsWriter#pre_visit')
       end

       def post_visit(parser); end

       def enter_interface(node)
         return unless needs_ami_generation?(node)

         ami_handler_interface.visit_object_ref_traits(node)
       end
    end # AmiStubProxyObjRefTraitsWriter

    class AmiStubProxyTypecodeWriter < AmiStubProxyBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(parser)
        super
        println
        printiln('// generated from AmiStubProxyTypecodeWriter#pre_visit')
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
        printiln('// generated from AmiStubProxyTypecodeWriter#enter_scope')
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
        if needs_ami_generation?(node)
          ami_handler_interface.visit_typecode(node)
        end
      end

      def enter_interface(node)
        if needs_ami_generation?(node)
          ami_handler_interface.visit_typecode(node)
          enter_scope(node)
        end
      end

      def leave_interface(node)
        if needs_ami_generation?(node)
          leave_scope(node)
        end
      end
    end # AmiStubProxyTypecodeWriter

    class AmiStubProxySrvBaseWriter < AmiBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
        self.template_root = File.join('srv', 'prx', 'ami')
      end
    end

    class AmiStubProxySrvWriter < AmiStubProxySrvBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(parser); end

      def post_visit(parser); end

      def enter_module(node)
        super
        println
        printiln('// generated from AmiStubProxySrvWriter#enter_module')
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
        return unless needs_ami_generation?(node)

        super
        println
        printiln('// generated from AmiStubProxySrvWriter#enter_interface')
        printiln('namespace POA {')
        inc_nest
        ami_handler_interface.visit_pre(node)
        inc_nest
      end

      def leave_interface(node)
        return if !needs_ami_generation?(node) || node.is_pseudo?

        dec_nest
        ami_handler_interface.visit_post(node)
        dec_nest
        printiln('} // namespace POA')
        println
        super
      end
     end # AmiStubProxySrvWriter
  end # Cxx11
end # IDL
