#--------------------------------------------------------------------
# @file    servantheader.rb
# @author  Martin Corino
#
# @brief   walker class for servant header
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/c++11/writerbase'
require 'ridlbe/c++11/writers/helpers/version_helper'
require 'ridlbe/c++11/writers/helpers/include_guard_helper'

module IDL
  module Cxx11
    class ServantHeaderBaseWriter < CxxCodeWriterBase
      def initialize(output = STDOUT, opts = {})
        super
        self.template_root = File.join('srv', 'hdr')
      end

      attr_accessor :include_guard
    end

    class ServantHeaderWriter < ServantHeaderBaseWriter
      helper Cxx11::VersionHelper
      helper Cxx11::IncludeGuardHelper

      def initialize(output = STDOUT, opts = {})
        super
        @include_guard = "__RIDL_#{File.basename(params[:srv_output_hdr] || '').to_random_include_guard}_INCLUDED__"
      end

      def pre_visit(parser)
        visit_includes(parser)

        super
      end

      def post_visit(parser)
        if params[:gen_tie]
          visit_servant_tie(parser)
        end

        # Servant traits specializations
        visit_servant_traits_specializations(parser)

        super
        visitor(PostVisitor).visit
      end

      def visit_include(node)
        return if File.basename(node.filename) == 'orb.idl'

        at_global_scope do
          visitor(IncludeVisitor).visit(node)
        end
      end

      def enter_module(node)
        super
        println
        printiln('// generated from ServantHeaderWriter#enter_module')
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
        return if node.is_local? || node.is_abstract?

        super
        visitor(InterfaceVisitor).visit_pre(node)
        inc_nest  # POA
        inc_nest  # servant skeleton class
      end

      def leave_interface(node)
        return if node.is_local? || node.is_abstract?

        dec_nest
        dec_nest
        visitor(InterfaceVisitor).visit_post(node)
        super
      end

      def enter_valuetype(node)
        super
        return if node.is_local? || !node.supports_concrete_interface?

        visitor(ValuetypeVisitor).visit_pre(node)
      end

      def visit_operation(node)
        return if node.enclosure.is_local? || node.enclosure.is_abstract?

        visitor(OperationVisitor).visit_operation(node)
      end

      def visit_attribute(node)
        return if node.enclosure.is_local? || node.enclosure.is_abstract?

        visitor(AttributeVisitor).visit_attribute(node)
      end

      def visit_servant_traits_specializations(parser)
        writer(ServantHeaderSrvTraitsWriter).visit_nodes(parser)
      end

      def visit_servant_tie(parser)
        writer(ServantHeaderTieWriter).visit_nodes(parser)
      end

      def visit_includes(parser)
        writer(ServantHeaderIncludeWriter) do |w|
          w.include_guard = @include_guard
          w.visit_nodes(parser)
        end
      end
    end

    class ServantHeaderIncludeWriter < ServantHeaderBaseWriter
      helper Cxx11::VersionHelper
      helper Cxx11::IncludeGuardHelper

      def initialize(output = STDOUT, opts = {})
        super
        @includes = []
        @includes << 'tao/x11/portable_server/servantbase.h' unless params[:no_servant_code]
      end

      attr_reader :includes

      def post_visit(_parser)
        properties[:includes] = @includes

        visitor(PreVisitor).visit
      end
    end # ServantHeaderIncludeWriter

    class ServantHeaderSrvTraitsWriter < ServantHeaderBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
      end

      def pre_visit(_parser)
        println
        printiln('// generated from ServantHeaderSrvTraitsWriter#pre_visit')
        printiln('namespace TAOX11_NAMESPACE::CORBA {')
        inc_nest
      end

      def post_visit(_parser)
        dec_nest
        printiln('} // namespace TAOX11_NAMESPACE::CORBA')
      end

      def enter_interface(node)
        super
        return if node.is_local? || node.is_abstract?

        visitor(InterfaceVisitor).visit_servant_traits(node)
      end

      def enter_valuetype(node)
        super
        return if node.is_local? || !node.supports_concrete_interface?

        visitor(ValuetypeVisitor).visit_servant_traits(node)
      end
    end

    class ServantHeaderTieWriter < ServantHeaderBaseWriter
      def initialize(output = STDOUT, opts = {})
        super
        self.template_root = File.join('srv', 'hdr', 'tie')
      end

      def enter_module(node)
        super
        println
        printiln('// generated from ServantHeaderTieWriter#pre_visit')
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
        return if node.is_local? || node.is_abstract?

        visitor(InterfaceVisitor).visit_pre(node)
        inc_nest  # POA
        inc_nest  # servant tie template
      end

      def leave_interface(node)
        return if node.is_local? || node.is_abstract?

        dec_nest
        dec_nest
        visitor(InterfaceVisitor).visit_post(node)
        super
      end

      def visit_operation(node)
        return if node.enclosure.is_local? || node.enclosure.is_abstract?

        visitor(OperationVisitor).visit_operation(node)
      end

      def visit_attribute(node)
        return if node.enclosure.is_local? || node.enclosure.is_abstract?

        visitor(AttributeVisitor).visit_attribute(node)
      end
    end
  end # Cxx11
end # IDL
