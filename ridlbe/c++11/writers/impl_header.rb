#--------------------------------------------------------------------
# @file    impl_header.rb
# @author  Martin Corino
#
# @brief   writer class for implementation header file
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/c++11/writerbase'
require 'ridlbe/c++11/writers/helpers/include_guard_helper'

module IDL
  module Cxx11

    class ImplHeaderBaseWriter < CxxCodeWriterBase
      def initialize(output = STDOUT, opts = {})
        super
        self.template_root = File.join('impl', 'hdr')
      end
      def generate_servant_implementation?
        params[:gen_impl_servant]
      end
    end

    class ImplHeaderWriter < ImplHeaderBaseWriter

      helper Cxx11::IncludeGuardHelper

      attr_reader :include_guard

      def initialize(output = STDOUT, opts = {})
        super
      end

      def visit_nodes(parser)
        @include_guard = "__RIDL_#{File.basename(params[:impl_output] || '').to_include_guard}_INCLUDED__"
        super
      end

      def pre_visit(parser)
        visitor(PreVisitor).visit
        super
      end

      def post_visit(parser)
        super
        visitor(PostVisitor).visit
      end

      def enter_module(node)
        super
        println
        printiln('// generated from ImplHeaderWriter#enter_module')
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
        return if node.is_abstract?

        super
        if node.is_local?
        else
          if generate_servant_implementation?
            visitor(InterfaceVisitor).visit_pre(node)
            inc_nest # servant implementation class
          end
        end
      end
      def leave_interface(node)
        return if node.is_abstract?

        if node.is_local?
        else
          if generate_servant_implementation?
            dec_nest
            visitor(InterfaceVisitor).visit_post(node)
          end
        end
        super
      end

      # NOT SUPPORTED
#      def enter_valuetype(node)
#        super
#        return if node.is_local? || !node.supports_concrete_interface?
#        visitor(ValuetypeVisitor).visit_pre(node)
#      end

      def visit_operation(node)
        return if node.enclosure.is_local?

        if node.enclosure.is_local?
        else
          if generate_servant_implementation?
            visitor(OperationVisitor).visit_operation(node)
          end
        end
      end

      def visit_attribute(node)
        return if node.enclosure.is_local?

        if node.enclosure.is_local?
        else
          if generate_servant_implementation?
            visitor(AttributeVisitor).visit_attribute(node)
          end
        end
      end
    end

  end # Cxx11
end # IDL
