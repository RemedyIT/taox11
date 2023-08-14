#--------------------------------------------------------------------
# @file    stubheader.rb
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
    class StubHeaderBaseWriter < CxxCodeWriterBase
      def initialize(output = STDOUT, opts = {})
        super
        self.template_root = File.join('cli', 'hdr')
      end

      attr_accessor :include_guard
    end

    class StubHeaderWriter < StubHeaderBaseWriter
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

        @include_guard = "__RIDL_#{File.basename(params[:output] || '').to_random_include_guard}_INCLUDED__"

        @fwd_decl_cache = {}
      end

      def pre_visit(parser)
        visit_includes(parser)

        super
      end

      def post_visit(parser)
        # traits specialization declarations
        visit_idl_traits(parser)

        # traits specialization definitions
        visit_idl_traits_def(parser)

        visit_anyops(parser) if params[:gen_any_ops]

        # generate inline methods
        visit_inlines(parser)

        if params[:gen_ostream_operators]
          visit_os(parser)
        end

        super
        visitor(PostVisitor).visit
      end

      def visit_include(node)
        return if File.basename(node.filename) == 'orb.idl'

        at_global_scope do
          visitor(IncludeVisitor).visit(node)
        end
      end

      def doc_scoped_name(node)
        visitor(Cxx11::NodeVisitorBase) { |v| v.visit(node) }.doc_scoped_name
      end

      def enter_module(node)
        super
        println
        printiln('// generated from StubHeaderWriter#enter_module')
        printiln("/// @copydoc #{self.doc_scoped_name node}")
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
        return if @fwd_decl_cache.has_key?(sn)

        @fwd_decl_cache[sn] = true
        visitor(InterfaceVisitor).visit_fwd(node)
        at_global_scope do
          visitor(InterfaceVisitor).visit_object_traits(node)
        end unless params[:no_object_traits]
      end

      def enter_interface(node)
        println
        printiln('// generated from StubHeaderWriter#enter_interface')
        sn = node.scoped_cxxname
        unless @fwd_decl_cache.has_key?(sn)
          @fwd_decl_cache[sn] = true
          visitor(InterfaceVisitor).visit_fwd(node)
          at_global_scope do
            visitor(InterfaceVisitor).visit_object_traits(node)
          end unless params[:no_object_traits]
        end
        super
        visitor(InterfaceVisitor).visit_pre(node)
        inc_nest
      end

      def leave_interface(node)
        dec_nest
        visitor(InterfaceVisitor).visit_post(node)
        super
      end

      def visit_operation(node)
        visitor(OperationVisitor).visit_operation(node)
      end

      def visit_attribute(node)
        visitor(AttributeVisitor).visit_attribute(node)
      end

      def declare_struct(node)
        sn = node.scoped_cxxname
        return if @fwd_decl_cache.has_key?(sn)

        @fwd_decl_cache[sn] = true
        visitor(StructVisitor).visit_fwd(node)
      end

      def enter_struct(node)
        super
        visitor(StructVisitor).visit_pre(node)
        inc_nest
      end

      def leave_struct(node)
        dec_nest
        visitor(StructVisitor).visit_post(node)
        super
      end

      def declare_valuetype(node)
        sn = node.scoped_cxxname
        return if @fwd_decl_cache.has_key?(sn)

        @fwd_decl_cache[sn] = true
        visitor(ValuetypeVisitor).visit_fwd(node)
        at_global_scope do
          visitor(ValuetypeVisitor).visit_traits(node)
        end
      end

      def enter_valuetype(node)
        sn = node.scoped_cxxname
        unless @fwd_decl_cache.has_key?(sn)
          @fwd_decl_cache[sn] = true
          visitor(ValuetypeVisitor).visit_fwd(node)
          at_global_scope do
            visitor(ValuetypeVisitor).visit_traits(node)
          end
        end
        # only call super after generating the forward and
        # traits stuff for super will enter valuetype scope
        # and we cannot break out of that (since C++ cannot
        # reopen classes, only namespaces)
        super
        visitor(ValuetypeVisitor).visit_pre(node)
        inc_nest
      end

      def leave_valuetype(node)
        dec_nest
        (vtv = visitor(ValuetypeVisitor)).visit_post(node)
        super
        vtv.visit_obv(node) unless node.is_abstract?
        # generate valuetype factory declaration unless it's an abstract
        # valuetype or the valuetype has no user defined initializers but does
        # have operations/attributes
        vtv.visit_init(node) unless node.is_abstract? || (node.initializers.empty? && node.has_operations_or_attributes?)
      end

      def visit_valuebox(node)
        super
        visitor(ValueboxVisitor).visit_fwd(node)
        at_global_scope do
          visitor(ValueboxVisitor).visit_traits(node)
        end
        visitor(ValueboxVisitor).visit_def(node)
        at_global_scope do
          visitor(ValueboxVisitor).visit_traits_def(node)
        end
      end

      def enter_exception(node)
        super
        visitor(ExceptionVisitor).visit_pre(node)
        inc_nest
      end

      def leave_exception(node)
        dec_nest
        visitor(ExceptionVisitor).visit_post(node)
        super
      end

      def declare_union(node)
        sn = node.scoped_cxxname
        return if @fwd_decl_cache.has_key?(sn)

        @fwd_decl_cache[sn] = true
        visitor(UnionVisitor).visit_fwd(node)
      end

      def enter_union(node)
        super
        visitor(UnionVisitor).visit_pre(node)
        inc_nest
      end

      def leave_union(node)
        dec_nest
        visitor(UnionVisitor).visit_post(node)
        super
      end

      def visit_const(node)
        println
        printiln('// generated from StubHeaderWriter#visit_const')
        printiln("/// @copydoc #{self.doc_scoped_name node}")
        case node.expression.idltype
        when Type::String, Type::WString
          if node.enclosure.is_a?(IDL::AST::Module)
            printi("const #{node.idltype.cxx_type(node.enclosure)} ")
          else
            printi("static const #{node.idltype.cxx_type(node.enclosure)} ")
          end
        else
          if node.enclosure.is_a?(IDL::AST::Module)
            case node.idltype.resolved_type
            when IDL::Type::Fixed
              printi("const #{node.idltype.cxx_type(node.enclosure)} ")
            else
              printi("constexpr #{node.idltype.cxx_type(node.enclosure)} ")
            end
          else
            case node.idltype.resolved_type
            when IDL::Type::Fixed
              printi("static const #{node.idltype.cxx_type(node.enclosure)} ")
            else
              printi("static constexpr #{node.idltype.cxx_type(node.enclosure)} ")
            end
          end
        end
        if node.enclosure.is_a?(IDL::AST::Module) || ![Type::String, Type::WString].include?(node.expression.idltype.class)
          if node.expression.is_a?(IDL::Expression::Value)
            case node.idltype.resolved_type
            when IDL::Type::Fixed
              println("#{node.cxxname};")
            else
              println("#{node.cxxname} {#{node.idltype.resolved_type.value_to_s(node.value)}};")
            end
          else
            println("#{node.cxxname} = #{expression_to_s(node.expression, node.enclosure)};")
          end
        else
          println(node.cxxname + ';')
        end
      end

      def visit_enum(node)
        visitor(EnumVisitor).visit_enum(node)
      end

      def visit_bitmask(node)
        visitor(BitmaskVisitor).visit_bitmask(node)
      end

      def visit_bitset(node)
        visitor(BitsetVisitor).visit_bitset(node)
      end

      def visit_typedef(node)
        return if node.idltype.resolved_type.is_a?(IDL::Type::Native) && params[:no_gen_native]

        visitor(TypedefVisitor).visit_typedef(node)
      end

      def visit_includes(parser)
        writer(StubHeaderIncludeWriter,
               { default_pre_includes: @default_pre_includes,
                 default_post_includes: @default_post_includes }) do |w|
          w.include_guard = @include_guard
          w.visit_nodes(parser)
        end
      end

      def visit_anyops(parser)
        writer(StubHeaderAnyOpWriter).visit_nodes(parser)
      end

      def visit_idl_traits(parser)
        writer(StubHeaderIDLTraitsWriter).visit_nodes(parser)
      end

      def visit_idl_traits_def(parser)
        writer(StubHeaderIDLTraitsDefWriter).visit_nodes(parser)
      end

      def visit_inlines(parser)
        writer(StubInlineWriter).visit_nodes(parser)
      end

      def visit_os(parser)
        writer(StubHeaderOSWriter).visit_nodes(parser)
      end
    end # StubHeaderWriter

    class StubHeaderIncludeWriter < StubHeaderBaseWriter
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
        visitor(PreVisitor).visit
      end

      def declare_interface(node)
        return if node.is_pseudo?

        add_include('tao/x11/valuetype/abstractbase_traits_t.h') if node.is_abstract?
      end

      def enter_interface(node)
        return if node.is_pseudo?

        unless node.is_abstract?
          add_include('tao/x11/object.h')
        else
          add_post_include('tao/x11/anytypecode/typecode.h') # in case not added yet
          add_post_include('tao/x11/valuetype/abstract_base.h') # after typecode include
        end
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

      def enter_exception(node)
        add_include('tao/x11/user_exception.h')
        node.members.each { |m| check_idl_type(m.idltype) }
      end

      def visit_typedef(node)
        return if node.idltype.resolved_type.is_a?(IDL::Type::Native) && params[:no_gen_native]
        return if node.idltype.is_a?(IDL::Type::ScopedName) # alias typedef

        idl_type = node.idltype.resolved_type
        case idl_type
        when IDL::Type::Fixed
          add_include('tao/x11/fixed_t.h')
        when IDL::Type::Sequence
          add_include('tao/x11/bounded_vector_t.h') if idl_type.size.to_i.positive?
          add_include('tao/x11/bounded_type_traits_t.h') if idl_type.size.to_i.positive?
          check_idl_type(idl_type.basetype)
        when IDL::Type::Map
          add_include('map')
          add_include('tao/x11/bounded_map_t.h') if idl_type.size.to_i.positive?
          add_include('tao/x11/bounded_type_traits_t.h') if idl_type.size.to_i.positive?
          check_idl_type(idl_type.keytype)
          check_idl_type(idl_type.valuetype)
        when IDL::Type::Array
          check_idl_type(idl_type.basetype)
        when IDL::Type::String, IDL::Type::WString
          add_include('tao/x11/bounded_string_t.h') if idl_type.size.to_i.positive?
          add_include('tao/x11/bounded_type_traits_t.h') if idl_type.size.to_i.positive?
        end
      end

      private

      def check_idl_type(idl_type)
        idl_type = idl_type.resolved_type
        case idl_type
        when IDL::Type::Any
          add_include('tao/x11/anytypecode/any.h') unless params[:gen_any_ops]
        when IDL::Type::Servant
          add_include('tao/x11/portable_server/servant_forward.h')
        when IDL::Type::AbstractBase
          add_post_include('tao/x11/valuetype/abstract_base.h')
        end
      end

      def add_include(inc_file)
        @includes << inc_file unless @includes.include?(inc_file)
      end

      def add_post_include(inc_file)
        @default_post_includes << inc_file unless @default_post_includes.include?(inc_file)
      end
    end

    class StubHeaderIDLTraitsWriter < StubHeaderBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(_parser)
        println
        printiln('// generated from StubHeaderIDLTraitsWriter#pre_visit')
        printiln('namespace TAOX11_NAMESPACE')
        println('{')
        inc_nest
        printiln('namespace IDL')
        printiln('{')
        inc_nest
      end

      def post_visit(_parser)
        dec_nest
        printiln('} // namespace IDL')
        dec_nest
        printiln('} // namespace TAOX11_NAMESPACE')
      end

      def declare_interface(node)
        visitor(InterfaceVisitor).visit_idl_traits(node)
      end

      def leave_interface(node)
        visitor(InterfaceVisitor).visit_idl_traits(node)
      end

      def declare_valuetype(node)
        visitor(ValuetypeVisitor).visit_idl_traits(node)
      end

      def leave_valuetype(node)
        visitor(ValuetypeVisitor).visit_idl_traits(node)
      end

      def visit_valuebox(node)
        visitor(ValueboxVisitor).visit_idl_traits(node)
      end

      def visit_enum(node)
        visitor(EnumVisitor).visit_idl_traits(node)
      end

      def visit_bitmask(node)
        visitor(BitmaskVisitor).visit_idl_traits(node)
      end

      def visit_bitset(node)
        visitor(BitsetVisitor).visit_idl_traits(node)
      end

      def declare_struct(node)
        visitor(StructVisitor).visit_idl_traits(node)
      end

      def leave_struct(node)
        visitor(StructVisitor).visit_idl_traits(node)
      end

      def leave_exception(node)
        visitor(ExceptionVisitor).visit_idl_traits(node)
      end

      def declare_union(node)
        visitor(UnionVisitor).visit_idl_traits(node)
      end

      def leave_union(node)
        visitor(UnionVisitor).visit_idl_traits(node)
      end

      def visit_typedef(node)
        return if node.idltype.resolved_type.is_a?(IDL::Type::Native) && params[:no_gen_native]
        return if node.idltype.is_a?(IDL::Type::ScopedName) # alias typedef

        idl_type = node.idltype.resolved_type
        case idl_type
        when IDL::Type::Sequence
          visitor(SequenceVisitor).visit_idl_traits(node)
        when IDL::Type::Map
          visitor(MapVisitor).visit_idl_traits(node)
        when IDL::Type::Array
          visitor(ArrayVisitor).visit_idl_traits(node)
        when IDL::Type::String, IDL::Type::WString
          visitor(StringVisitor).visit_idl_traits(node) if idl_type.size.to_i.positive? # only for bounded strings
        when IDL::Type::Fixed
          visitor(FixedVisitor).visit_idl_traits(node)
        end
      end
    end # StubHeaderIDLTraitsWriter

    class StubHeaderIDLTraitsDefWriter < StubHeaderBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(_parser)
        println
        printiln('// generated from StubHeaderIDLTraitsDefWriter#pre_visit')
        printiln('namespace TAOX11_NAMESPACE')
        println('{')
        inc_nest
        printiln('namespace IDL')
        printiln('{')
        inc_nest
      end

      def post_visit(_parser)
        dec_nest
        printiln('} // namespace IDL')
        dec_nest
        printiln('} // namespace TAOX11_NAMESPACE')
      end

      def enter_interface(node)
        visitor(InterfaceVisitor).visit_idl_traits_def(node)
      end

      def enter_valuetype(node)
        visitor(ValuetypeVisitor).visit_idl_traits_def(node)
      end

      def enter_struct(node)
        visitor(StructVisitor).visit_idl_traits_def(node)
      end

      def enter_exception(node)
        visitor(ExceptionVisitor).visit_idl_traits_def(node)
      end

      def enter_union(node)
        visitor(UnionVisitor).visit_idl_traits_def(node)
      end
    end # StubHeaderIDLTraitsDefWriter

    class StubHeaderAnyOpWriter < StubHeaderBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(parser)
        super
        println
        printiln('// generated from StubHeaderAnyOpWriter#pre_visit')
        println('namespace TAOX11_NAMESPACE')
        println('{')
        inc_nest
        println('  namespace CORBA')
        println('  {')
        inc_nest
      end

      def post_visit(parser)
        dec_nest
        println
        println('  } // namespace CORBA')
        dec_nest
        println
        println('} // namespace TAOX11_NAMESPACE')
        super
      end

      def declare_interface(node)
        visitor(InterfaceVisitor).visit_anyop(node)
      end

      def enter_interface(node)
        visitor(InterfaceVisitor).visit_anyop(node)
      end

      def declare_valuetype(node)
        visitor(ValuetypeVisitor).visit_anyop(node)
      end

      def enter_valuetype(node)
        visitor(ValuetypeVisitor).visit_anyop(node)
      end

      def visit_valuebox(node)
        visitor(ValueboxVisitor).visit_anyop(node)
      end

      def declare_struct(node)
        visitor(StructVisitor).visit_anyop(node)
      end

      def enter_struct(node)
        visitor(StructVisitor).visit_anyop(node)
      end

      def declare_union(node)
        visitor(UnionVisitor).visit_anyop(node)
      end

      def enter_union(node)
        visitor(UnionVisitor).visit_anyop(node)
      end

      def enter_exception(node)
        visitor(ExceptionVisitor).visit_anyop(node)
      end

      def visit_enum(node)
        visitor(EnumVisitor).visit_anyop(node)
      end

      def visit_typedef(node)
        # nothing to do if this is just an alias for another defined type
        return if node.idltype.is_a?(IDL::Type::ScopedName)

        _resolved_type = node.idltype.resolved_type
        return if _resolved_type.is_a?(IDL::Type::Native) ||
                  _resolved_type.is_standard_type?

        visitor(TypedefVisitor).visit_anyop(node)
      end
    end # StubHeaderAnyOpWriter

    class StubInlineWriter < CxxCodeWriterBase
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

      def enter_valuetype(node); end

      def enter_struct(node)
        visitor(StructVisitor).visit_inl(node)
      end

      def enter_union(node)
        visitor(UnionVisitor).visit_inl(node)
      end

      def enter_exception(node)
        visitor(ExceptionVisitor).visit_inl(node)
      end
    end # StubInlineWriter

    class StubHeaderOSWriter < StubHeaderBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
        @tracker = []
      end

      def enter_interface(node)
        visitor(InterfaceVisitor).visit_os(node) unless is_tracked?(node)
      end

      def enter_valuetype(node)
        visitor(ValuetypeVisitor).visit_os(node) unless is_tracked?(node)
      end

      def visit_valuebox(node)
        visitor(ValueboxVisitor).visit_os(node)
      end

      def enter_struct(node)
        visitor(StructVisitor).visit_os(node)
      end

      def enter_union(node)
        visitor(UnionVisitor).visit_os(node)
      end

      def visit_enum(node)
        visitor(EnumVisitor).visit_os(node)
      end

      def visit_typedef(node)
        return if node.idltype.resolved_type.is_a?(IDL::Type::Native)
        # nothing to do if this is just an alias for another defined type
        return if node.idltype.is_a?(IDL::Type::ScopedName)

        case node.idltype.resolved_type
        when IDL::Type::Sequence
          visitor(SequenceVisitor).visit_os(node)
        when IDL::Type::Map
          visitor(MapVisitor).visit_os(node)
        when IDL::Type::Array
          visitor(ArrayVisitor).visit_os(node)
        when IDL::Type::Fixed
          visitor(FixedVisitor).visit_os(node)
        end
      end

      private

      # by keeping track of nodes for which we generated an os op we avoid
      # generating multiple code blocks for the same type
      def is_tracked?(node)
        unless @tracker.include?(node.scoped_name)
          # not tracked yet, but add now
          @tracker << node.scoped_name
          false
        else
          true
        end
      end
    end # StubHeaderOSWriter
end # Cxx11
end # IDL
