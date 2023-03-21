#--------------------------------------------------------------------
# @file    impl_source.rb
# @author  Martin Corino
#
# @brief   writer class for implementation source file
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/c++11/writerbase'

module IDL
  module Cxx11

    class ImplSourceBaseWriter < CxxCodeWriterBase
      def initialize(output = STDOUT, opts = {})
        super
        self.template_root = File.join('impl', 'src')
      end

      def generate_servant_implementation?
        params[:gen_impl_servant]
      end
    end

    class ImplSourceWriter < ImplSourceBaseWriter
      def initialize(output = STDOUT, opts = {})
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
        printiln('// generated from ImplSourceWriter#enter_module')
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
        if node.is_local? || node.is_pseudo?
        else
          if generate_servant_implementation?
            println
            printiln('// generated from ImplSourceWriter#enter_interface')
            visitor(InterfaceVisitor).visit_pre(node)
            inc_nest
          end
        end
      end

      def leave_interface(node)
        return if node.is_abstract?

        if node.is_local? || node.is_pseudo?
        else
          if generate_servant_implementation?
            dec_nest
            # visit all operations (incl. inherited) here directly
            node.operations(true).each do |_op|
              visitor(OperationVisitor) { |v| v.interface(node); v.visit_operation(_op) }
            end
            # visit all attributes (incl. inherited) here directly
            node.attributes(true).each do |_att|
              visitor(AttributeVisitor) { |v| v.interface(node); v.visit_attribute(_att) }
            end
            visitor(InterfaceVisitor).visit_post(node)
            println
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

    end # ImplSourceWriter

  end # Cxx11
end # IDL
