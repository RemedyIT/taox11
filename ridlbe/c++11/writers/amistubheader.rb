#--------------------------------------------------------------------
# @file    amistubheader.rb
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
    class AmiStubHeaderBaseWriter < AmiBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
        self.template_root = File.join('cli', 'hdr', 'ami')
      end

      attr_accessor :include_guard
    end

    class AmiStubHeaderWriter < AmiStubHeaderBaseWriter
      helper Cxx11::IncludeGuardHelper

      def initialize(output = STDOUT, opts = {})
        super
        @default_pre_includes = [
          'tao/x11/stddef.h',
          'tao/x11/basic_traits.h',
          'tao/x11/corba.h',
          'tao/x11/system_exception.h'
        ]
        @default_pre_includes << 'tao/x11/orb.h' unless params[:no_orb_include]
        @default_post_includes = []
        @default_post_includes << 'tao/x11/anytypecode/any.h' if params[:gen_any_ops]
        if params[:gen_typecodes]
          if params[:gen_anytypecode_source]
            @default_post_includes << 'tao/x11/anytypecode/typecode_ref.h'
          else
            @default_post_includes << 'tao/x11/anytypecode/typecode.h'
          end
        end
        @default_post_includes << 'tao/x11/anytypecode/typecode_constants.h' if params[:gen_typecodes]
        @default_post_includes << 'tao/x11/corba_ostream.h'
        @default_post_includes << 'tao/x11/messaging/messaging.h'
        @default_post_includes << 'tao/x11/amic_traits_t.h'
        @default_post_includes << 'tao/x11/portable_server/servantbase.h'
        # include stub header
        @default_post_includes << "#{File.basename(params[:output])}"

        @include_guard = "__RIDL_AMI_#{File.basename(params[:output] || '').to_random_include_guard}_INCLUDED__"
        @fwd_decl_cache = {}
      end

      def pre_visit(parser)
        visit_includes(parser)

        super
      end

      def post_visit(parser)
        # traits specialization declarations
        visit_traits_specializations(parser)

        visit_anyops(parser) if params[:gen_any_ops]

        # generate inline methods
        visit_inlines(parser)

        # sendc functionality
        visit_amic_header(parser)

        # amic traits specializations
        visit_amic_traits(parser)

        # skeleton functionality  ReplyHandler
        visit_servant_header(parser)

        # ostream operators for replyhandler and AmiC classes
        if params[:gen_ostream_operators]
          visit_os(parser)
        end

        super
        visitor(PostVisitor) do |v|
          v.class_eval do
            ###
            # Overload standard #client_proxy for this visitor instance
            def client_proxy
              File.basename(params[:idlfile], params[:idlext]) + params[:ami_pfx] + params[:stub_pfx] + params[:proxy_pfx] + '.h'
            end
          end
          v.visit
        end
      end

      def visit_include(node)
        return if File.basename(node.filename) == 'orb.idl'
        return unless ((@params[:ami] && node.has_ami_interfaces_included?) ||
                    (@params[:ami_bc] && node.has_interfaces_ami_candidates))

        at_global_scope do
          visitor(IncludeVisitor).visit(node)
        end
      end

      def enter_module(node)
        super
        println
        printiln('// generated from AmiStubHeaderWriter#enter_module')
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

      def declare_interface(node)
        sn = node.scoped_cxxname
        return if @fwd_decl_cache.has_key?(sn) || !needs_ami_generation?(node)

        @fwd_decl_cache[sn] = true
        # use ami_interface for ReplyHandler
        _ami_intf = ami_handler_interface
        _ami_intf.visit_fwd(node)
        at_global_scope do
          _ami_intf.visit_object_traits(node)
        end unless params[:no_object_traits]
      end

      def enter_interface(node)
        return unless needs_ami_generation?(node)

        println
        printiln('// generated from AmiStubHeaderWriter#enter_interface')
        sn = node.scoped_cxxname
        # use ami_interface for ReplyHandler
        _ami_intf = ami_handler_interface_with_ami_inheritance
        unless @fwd_decl_cache.has_key?(sn)
           @fwd_decl_cache[sn] = true
          _ami_intf.visit_fwd(node)
          at_global_scope do
            _ami_intf.visit_object_traits(node)
          end unless params[:no_object_traits]
        end

        super
        _ami_intf.visit_pre(node)
        inc_nest
      end

      def leave_interface(node)
        return unless needs_ami_generation?(node)

        dec_nest
        ami_handler_interface.visit_post(node)
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
        writer(AmiStubHeaderIncludeWriter,
               { default_pre_includes: @default_pre_includes,
                 default_post_includes: @default_post_includes }) do |w|
          w.include_guard = @include_guard
          w.visit_nodes(parser)
        end
      end

      def visit_anyops(parser)
        writer(AmiStubHeaderAnyOpWriter).visit_nodes(parser)
      end

      def visit_traits_specializations(parser)
        writer(AmiStubHeaderTraitsWriter).visit_nodes(parser)
      end

      def visit_amic_traits(parser)
        writer(AmiStubHeaderAmiCTraitsWriter).visit_nodes(parser)
      end

      def visit_inlines(parser)
        writer(AmiStubInlineWriter).visit_nodes(parser)
      end

      def visit_os(parser)
        writer(AmiStubHeaderOSWriter).visit_nodes(parser)
      end

      def visit_amic_header(parser)
        writer(AmiStubHeaderAmiCWriter).visit_nodes(parser)
      end

      def visit_servant_header(parser)
         writer(AmiStubHeaderSrvWriter).visit_nodes(parser)
      end
    end # AmiStubHeaderWriter

    class AmiStubHeaderIncludeWriter < AmiStubHeaderBaseWriter
      helper Cxx11::VersionHelper
      helper Cxx11::IncludeGuardHelper

      def initialize(output = STDOUT, opts = {})
        super
        @includes = []
        @default_pre_includes = params[:default_pre_includes]
        @default_post_includes = params[:default_post_includes]
      end

      attr_reader :includes

      def post_visit(_parser)
        properties[:includes] = @includes
        properties[:pre_includes] = @default_pre_includes
        properties[:post_includes] = @default_post_includes
        visitor(PreVisitor) do |v|
          ###
          # Overloads for this visitor only.
          v.class_eval do
            def stub_export_include?
              !params[:amic_export_include].nil? || super
            end

            def stub_export_include
              params[:amic_export_include] || super
            end
          end
          v.visit
        end
      end

      def declare_interface(node)
        return if node.is_pseudo?

        add_include('tao/x11/valuetype/abstractbase_traits_t.h') if node.is_abstract?
      end

      def enter_interface(node)
        return unless needs_ami_generation?(node)

          add_include('tao/x11/object.h')
      end

      def visit_operation(node)
        check_idl_type(node.idltype)
        node.params.each { |parm| check_idl_type(parm.idltype) }
      end

      def visit_attribute(node)
        check_idl_type(node.idltype)
      end

      def declare_valuetype(_node)
        add_include('tao/x11/valuetype/valuetype_traits_t.h')
      end

      def enter_valuetype(node)
        add_post_include('tao/x11/anytypecode/typecode.h') # in case not added yet
        add_post_include('tao/x11/valuetype/value_base.h') # after typecode include
        add_post_include('tao/x11/valuetype/value_factory_base.h') unless node.is_abstract? # after typecode include
        node.state_members.each { |m| check_idl_type(m.idltype) }
      end

      def visit_valuebox(node)
        add_post_include('tao/x11/anytypecode/typecode.h') # in case not added yet
        add_post_include('tao/x11/valuetype/value_box_t.h')
        check_idl_type(node.boxed_type)
      end

      def enter_struct(node)
        node.members.each { |m| check_idl_type(m.idltype) }
      end

      def enter_union(node)
        node.members.each { |m| check_idl_type(m.idltype) }
      end

      def enter_exception(node); end

      def visit_typedef(node)
        return if node.idltype.resolved_type.is_a?(IDL::Type::Native) && params[:no_gen_native]
        return if node.idltype.is_a?(IDL::Type::ScopedName) # alias typedef

        idl_type = node.idltype.resolved_type
        case idl_type
        when IDL::Type::Sequence,
             IDL::Type::Array,
             IDL::Type::Map
          check_idl_type(idl_type.basetype)
        when IDL::Type::String,
             IDL::Type::WString
        end
      end

      private

      def check_idl_type(idl_type)
        idl_type = idl_type.resolved_type
        case idl_type
        when IDL::Type::Any
          add_include('tao/x11/anytypecode/any.h') unless params[:gen_any_ops]
        end
      end

      def add_include(inc_file)
        @includes << inc_file unless @includes.include?(inc_file)
      end

      def add_post_include(inc_file)
        @default_post_includes << inc_file unless @default_post_includes.include?(inc_file)
      end
    end

    class AmiStubHeaderTraitsWriter < AmiStubHeaderBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(_parser)
        println
        printiln('// generated from AmiStubHeaderTraitsWriter#pre_visit')
        printiln('namespace TAOX11_NAMESPACE::IDL')
        printiln('{')
        inc_nest
      end

      def post_visit(_parser)
        dec_nest
        printiln('} // namespace TAOX11_NAMESPACE::IDL')
      end

      def enter_interface(node)
        return unless needs_ami_generation?(node)

        # use ami_interface for ReplyHandler
        _ami_intf = ami_handler_interface
        _ami_intf.visit_idl_traits(node)
        _ami_intf.visit_idl_traits_def(node)
      end
    end # AmiStubHeaderTraitsWriter

    class AmiStubHeaderAnyOpWriter < AmiStubHeaderBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(parser)
        super
        println
        printiln('// generated from AmiStubHeaderAnyOpWriter#pre_visit')
        println('namespace TAOX11_NAMESPACE::CORBA')
        println('{')
        inc_nest
      end

      def post_visit(parser)
        dec_nest
        println
        println('  } // namespace TAOX11_NAMESPACE::CORBA')
        super
      end

      def declare_interface(node)
        if needs_ami_generation?(node)
          ami_handler_interface.visit_anyop(node)
        end
      end

      def enter_interface(node)
        if needs_ami_generation?(node)
        ami_handler_interface.visit_anyop(node)
        end
      end
    end # AmiStubHeaderAnyOpWriter

    class AmiStubInlineWriter < CxxCodeWriterBase
      def initialize(output = STDOUT, opts = {})
        super
        # reroute to cli/inl folder for inline generation
        self.template_root = File.join('cli', 'inl')
      end

      def pre_visit(parser)
        super
      end

      def post_visit(parser)
        super
      end

      def enter_interface(node); end

      def leave_interface(node); end
    end # AmiStubInlineWriter

    class AmiStubHeaderOSWriter < AmiStubHeaderBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def enter_interface(node)
        if needs_ami_generation?(node)
          ami_handler_interface.visit_os(node)
          ami_interface.visit_amic_os(node)
        end
      end
    end # AmiStubHeaderOSWriter

    class AmiStubHeaderAmiCWriter < AmiStubHeaderBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
          @fwd_decl_cache = {}
      end

      def post_visit(parser); end

      def enter_module(node)
        super
        println
        printiln('// generated from AmiStubHeaderAmiCWriter#enter_module')
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

      def declare_interface(node)
        sn = node.scoped_cxxname
        return if @fwd_decl_cache.has_key?(sn) || !needs_ami_generation?(node)

        @fwd_decl_cache[sn] = true
        ami_interface.visit_amic_fwd(node)
        at_global_scope do
          ami_interface.visit_amic_object_traits(node)
        end unless params[:no_object_traits]
      end

      def enter_interface(node)
        return unless needs_ami_generation?(node)

        println
        printiln('// generated from AmiStubHeaderAmiCWriter#enter_interface')
        sn = node.scoped_cxxname
        unless @fwd_decl_cache.has_key?(sn)
          @fwd_decl_cache[sn] = true
          ami_interface.visit_amic_fwd(node)
          at_global_scope do
            ami_interface.visit_amic_object_traits(node)
          end unless params[:no_object_traits]
        end
        super
        ami_interface.visit_amic_pre(node)
        inc_nest
      end

      def leave_interface(node)
        return unless needs_ami_generation?(node)

        dec_nest
        ami_interface.visit_amic_post(node)
        super
      end

      def visit_operation(node)
        return if !node.enclosure.is_a?(IDL::AST::Interface) ||
                  !needs_ami_generation?(node.enclosure)

        ami_operation.visit_amic(node)
      end

      def visit_attribute(node)
        return if !node.enclosure.is_a?(IDL::AST::Interface) ||
                  !needs_ami_generation?(node.enclosure)

        ami_attribute.visit_amic(node)
      end
    end # AmiStubHeaderAmiCWriter

    class AmiStubHeaderAmiCTraitsWriter < AmiStubHeaderBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(_parser)
        println
        printiln('// generated from AmiStubHeaderAmicTraitsWriter#pre_visit')
        printiln('namespace TAOX11_NAMESPACE::IDL')
        printiln('{')
        inc_nest
      end

      def post_visit(_parser)
        dec_nest
        printiln('} // namespace TAOX11_NAMESPACE::IDL')
      end

      def enter_interface(node)
        return unless needs_ami_generation?(node)

        ami_interface.visit_amic_traits(node)
      end
    end # AmiStubHeaderTraitsWriter

    class AmiStubHeaderBaseSrvWriter < AmiBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
        self.template_root = File.join('srv', 'hdr', 'ami')
      end
    end

    class AmiStubHeaderSrvWriter < AmiStubHeaderBaseSrvWriter
      def initialize(output = STDOUT, opts = {})
         super
      end

      def post_visit(parser)
        # Servant traits specializations
        visit_servant_traits_specializations(parser)
        super
      end

      def enter_module(node)
        super
        println
        printiln('// generated from AmiStubHeaderSrvWriter#enter_module')
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
        ami_handler_interface_with_ami_inheritance.visit_pre(node)
        inc_nest  # POA
        inc_nest  # servant skeleton class
      end

      def leave_interface(node)
        return unless needs_ami_generation?(node)

        dec_nest
        dec_nest
        ami_handler_interface.visit_post(node)
        super
      end

      def visit_operation(node)
        return unless needs_ami_generation?(node.enclosure)

        visitor(OperationVisitor).visit_operation(node)
      end

      def visit_attribute(node)
        return unless needs_ami_generation?(node.enclosure)

        visitor(AttributeVisitor).visit_attribute(node)
      end

      def visit_servant_traits_specializations(parser)
        writer(AmiStubHeaderSrvTraitsWriter).visit_nodes(parser)
      end
    end

    class AmiStubHeaderSrvTraitsWriter < AmiStubHeaderBaseSrvWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(_parser)
        println
        printiln('// generated from AmiStubHeaderSrvTraitsWriter#pre_visit')
        printiln('namespace TAOX11_NAMESPACE::CORBA {')
        inc_nest
      end

      def post_visit(_parser)
        dec_nest
        printiln('} // namespace TAOX11_NAMESPACE::CORBA')
      end

      def enter_interface(node)
        super
        return unless needs_ami_generation?(node)

        ami_handler_interface.visit_servant_traits(node)
      end
    end
end # Cxx11
end # IDL
