#--------------------------------------------------------------------
# @file    servantsource.rb
# @author  Martin Corino
#
# @brief   walker class for servant source
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/c++11/writerbase'

module IDL
  module Cxx11
    class ServantSourceBaseWriter < CxxCodeWriterBase
      def initialize(output = STDOUT, opts = {})
        super
        self.template_root = File.join('srv', 'src')
      end
    end

    class ServantSourceWriter < ServantSourceBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
        @default_pre_includes = [
          'tao/PortableServer/Servant_Base.h',
          'tao/PortableServer/Upcall_Wrapper.h',
          'tao/TAO_Server_Request.h',
          'tao/ORB_Core.h',
          'tao/Stub.h',
          'tao/Object_T.h',
          'tao/operation_details.h',
          'tao/PortableInterceptor.h',
          'tao/x11/portable_server/portableserver_proxies.h'
        ].compact
        @default_post_includes = [
          'tao/x11/portable_server/servant_proxy.h',
          'tao/x11/objproxy.h',
          'tao/x11/system_exception_x11.h',
          'tao/x11/exception_macros.h',
          'tao/x11/portable_server/operation_table_std_map.h',
          'tao/x11/portable_server/stub_sarg_traits.h',
          'tao/x11/portable_server/upcall_command.h',
          'tao/x11/stub_arg_traits.h'
        ]
        @default_post_includes << 'tao/x11/anytypecode/typecode_impl.h' if (params[:gen_typecodes] || params[:gen_any_ops]) && !params[:gen_anytypecode_source]
      end

      def pre_visit(parser)
        visit_includes(parser)

        super

        visit_sarg_traits(parser)
      end

      def post_visit(parser)
        super

        visitor(PostVisitor).visit
      end

      def enter_module(node)
        super
        println
        printiln('// generated from ServantSourceWriter#enter_module')
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
        return if node.is_local? || node.is_pseudo? || node.is_abstract?

        super
        println
        printiln('// generated from ServantSourceWriter#enter_interface')
        printiln('namespace POA')
        printiln('{')
        inc_nest
        inc_nest
      end

      def leave_interface(node)
        return if node.is_local? || node.is_pseudo? || node.is_abstract?

        dec_nest
        visitor(InterfaceVisitor).visit_post(node)
        # visit all operations (incl. inherited) here directly
        node.operations(true).each do |_op|
          visitor(OperationVisitor) { |v| v.interface(node); v.visit_operation(_op) }
        end
        # visit all attributes (incl. inherited) here directly
        node.attributes(true).each do |_att|
          visitor(AttributeVisitor) { |v| v.interface(node); v.visit_attribute(_att) }
        end
        println
        visitor(InterfaceVisitor).visit_skel(node)
        dec_nest
        printiln('} // namespace POA')
        println
        super
      end

      def enter_valuetype(node)
        super
        return if node.is_local? || !node.supports_concrete_interface?

        visitor(ValuetypeVisitor).visit_pre(node)
      end

      def visit_includes(parser)
        writer(ServantSourceIncludeWriter,
               { default_pre_includes: @default_pre_includes,
                 default_post_includes: @default_post_includes }).visit_nodes(parser)
      end

      def visit_sarg_traits(parser)
        writer(ServantSourceSArgTraitsWriter).visit_nodes(parser)
      end
    end # ServantSourceWriter

    class ServantSourceIncludeWriter < ServantSourceBaseWriter
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
        return if node.is_local? || node.is_pseudo? || node.is_abstract?

        check_idl_type(node.idltype)
        # interfaces ALWAYS provide sequence cdr definitions (forward decl issue)
        add_include('tao/x11/sequence_cdr_t.h') unless params[:no_cdr_streaming]
        # required by default for interface because of standard Object methods like 'is_a'
        add_include('tao/x11/portable_server/basic_sarguments.h')
      end

      def visit_operation(node)
        return if node.enclosure.is_local? || node.enclosure.is_pseudo? || node.enclosure.is_abstract?

        check_idl_type(node.idltype)
        node.params.each { |parm| check_idl_type(parm.idltype) }
      end

      def visit_attribute(node)
        return if node.enclosure.is_local? || node.enclosure.is_pseudo? || node.enclosure.is_abstract?

        check_idl_type(node.idltype)
      end

      def visit_valuetype(node)
        return if node.is_abstract? || node.is_local?

        node.state_members.each { |m| check_idl_type(m.idltype) }
      end

      def visit_valuebox(node)
        return if node.is_local?

        check_idl_type(node.boxed_type)
      end

      def enter_struct(node)
        return if node.is_local?

        node.members.each { |m| check_idl_type(m.idltype) }
      end

      def enter_union(node)
        return if node.is_local?

        node.members.each { |m| check_idl_type(m.idltype) }
      end

      def visit_typedef(node)
        return if node.is_local?

        idl_type = node.idltype.resolved_type
        case idl_type
        when IDL::Type::Sequence,
             IDL::Type::Map,
             IDL::Type::Array
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
          add_include('tao/x11/portable_server/special_basic_sarguments.h')
          add_include('tao/x11/special_basic_arguments.h')
        when IDL::Type::Integer,
             IDL::Type::Double,
             IDL::Type::LongDouble,
             IDL::Type::Float,
          add_include('tao/x11/portable_server/basic_sarguments.h')
          add_include('tao/x11/basic_arguments.h')
        when IDL::Type::Enum,
             IDL::Type::BitMask,
             IDL::Type::BitSet
          add_include('tao/x11/portable_server/basic_sargument_t.h')
          add_include('tao/x11/basic_argument_t.h')
        when IDL::Type::String,
             IDL::Type::WString
          add_include('tao/x11/portable_server/basic_sarguments.h')
          add_include('tao/x11/basic_arguments.h')
        when IDL::Type::Any
          add_include('tao/x11/anytypecode/any_sarg_traits.h')
        when IDL::Type::Interface,
             IDL::Type::Component
          add_include('tao/x11/portable_server/basic_sargument_t.h')
        when IDL::Type::Valuebox,
             IDL::Type::Valuetype
          add_include('tao/x11/portable_server/basic_sargument_t.h')
          add_include('tao/x11/basic_argument_t.h')
        when IDL::Type::Struct,
             IDL::Type::Union
          add_include('tao/x11/portable_server/basic_sargument_t.h')
          add_include('tao/x11/basic_argument_t.h')
        when IDL::Type::Sequence
          add_include('tao/x11/portable_server/basic_sargument_t.h')
          add_include('tao/x11/basic_argument_t.h')
          add_include('tao/x11/sequence_cdr_t.h') unless params[:no_cdr_streaming]
          check_idl_type(idl_type.basetype)
        when IDL::Type::Map
          add_include('tao/x11/portable_server/basic_sargument_t.h')
          add_include('tao/x11/basic_argument_t.h')
          add_include('tao/x11/map_cdr_t.h') unless params[:no_cdr_streaming]
          check_idl_type(idl_type.keytype)
          check_idl_type(idl_type.valuetype)
        when IDL::Type::Array
          add_include('tao/x11/portable_server/basic_sargument_t.h')
          add_include('tao/x11/basic_argument_t.h')
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
    end # ServantSourceIncludeWriter

    class ServantSourceSArgTraitsWriter < ServantSourceBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
        @node_tracker = []
      end

      def pre_visit(parser)
        super
        println
        printiln('// generated from ServantSourceSArgTraitsWriter#pre_visit')
        printiln('namespace TAOX11_NAMESPACE::PS')
        printiln('{')
      end

      def post_visit(parser)
        println
        println('} // namespace TAOX11_NAMESPACE:PS')
        super
      end

      def visit_operation(node)
        return if node.enclosure.is_local? || node.enclosure.is_pseudo? || node.enclosure.is_abstract?

        check_idl_type(node.idltype)
        node.params.each { |parm| check_idl_type(parm.idltype) }
      end

      def visit_attribute(node)
        return if node.enclosure.is_local? || node.enclosure.is_pseudo? || node.enclosure.is_abstract?

        check_idl_type(node.idltype)
      end

      private

      # by keeping track of nodes for which we generated an SArg trait we avoid
      # generating multiple code blocks for the same type
      def is_tracked?(node)
        unless @node_tracker.include?(node)
          # not tracked yet, but add now
          @node_tracker << node
          false
        else
          true
        end
      end

      def check_idl_type(idl_type)
        res_idl_type = idl_type.resolved_type
        case res_idl_type
        when IDL::Type::Interface
          visitor(InterfaceVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        when IDL::Type::Valuebox
          visitor(ValueboxVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        when IDL::Type::Valuetype
          visitor(ValuetypeVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        when IDL::Type::Struct
          visitor(StructVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        when IDL::Type::Enum
          visitor(EnumVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        when IDL::Type::BitSet
          visitor(BitsetVisitor).visit_sarg_traits(res_idl_type.node)# unless is_tracked?(res_idl_type.node)
        when IDL::Type::BitMask
          visitor(BitmaskVisitor).visit_sarg_traits(res_idl_type.node)# unless is_tracked?(res_idl_type.node)
        when IDL::Type::Union
          visitor(UnionVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        when IDL::Type::Sequence
          # find the base typedef for this sequence
          return unless idl_type.is_a?(IDL::Type::ScopedName) # can't handle anonymous sequence types

          # find base typedef for sequence
          res_idl_type = idl_type
          while res_idl_type.node.idltype.is_a?(IDL::Type::ScopedName)
            res_idl_type = res_idl_type.node.idltype
          end
          visitor(SequenceVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        when IDL::Type::Map
          # find the base typedef for this map
          return unless idl_type.is_a?(IDL::Type::ScopedName) # can't handle anonymous map types

          # find base typedef for map
          res_idl_type = idl_type
          while res_idl_type.node.idltype.is_a?(IDL::Type::ScopedName)
            res_idl_type = res_idl_type.node.idltype
          end
          visitor(MapVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        when IDL::Type::Array
          # find the base typedef for this array
          return unless idl_type.is_a?(IDL::Type::ScopedName) # can't handle anonymous array types

          # find base typedef for array
          res_idl_type = idl_type
          while res_idl_type.node.idltype.is_a?(IDL::Type::ScopedName)
            res_idl_type = res_idl_type.node.idltype
          end
          # recheck if already done
          visitor(ArrayVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        when IDL::Type::String, IDL::Type::WString
          return if idl_type.resolved_type.is_standard_type? # handle only bounded strings (unbounded is standard_tpe)
          # find the base typedef for this string
          return unless idl_type.is_a?(IDL::Type::ScopedName) # can't handle anonymous sequence types

          # find base typedef for string
          res_idl_type = idl_type
          while res_idl_type.node.idltype.is_a?(IDL::Type::ScopedName)
            res_idl_type = res_idl_type.node.idltype
          end
          visitor(StringVisitor).visit_sarg_traits(res_idl_type.node) unless is_tracked?(res_idl_type.node)
        end
      end
    end # ServantSourceSArgTraitsWriter
  end # Cxx11
end # IDL
