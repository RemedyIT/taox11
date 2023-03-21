#--------------------------------------------------------------------
# @file    servantproxy.rb
# @author  Martin Corino
#
# @brief   walker class for servant proxy header
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/c++11/writerbase'
require 'ridlbe/c++11/writers/helpers/version_helper'
require 'ridlbe/c++11/writers/helpers/include_guard_helper'

module IDL
  module Cxx11
    class ServantProxyBaseWriter < CxxCodeWriterBase
      def initialize(output = STDOUT, opts = {})
        super
        self.template_root = File.join('srv', 'prx')
      end
    end

    class ServantProxyWriter < ServantProxyBaseWriter
      helper Cxx11::VersionHelper
      helper Cxx11::IncludeGuardHelper

      attr_reader :include_guard

      def initialize(output = STDOUT, opts = {})
        super
      end

      def visit_nodes(parser)
        @include_guard = "__RIDL_#{File.basename(params[:srv_output_prx] || '').to_random_include_guard}_INCLUDED__"
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

      def visit_include(node)
        return if File.basename(node.filename) == 'orb.idl'

        at_global_scope do
          visitor(IncludeVisitor).visit(node)
        end
      end

      def enter_module(node)
        super
        println
        printiln('// generated from ServantProxyWriter#enter_module')
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
        printiln('// generated from ServantProxyWriter#enter_interface')
        printiln('namespace POA {')
        inc_nest
        visitor(InterfaceVisitor).visit_pre(node)
        inc_nest
      end

      def leave_interface(node)
        return if node.is_local? || node.is_pseudo? || node.is_abstract?

        dec_nest
        visitor(InterfaceVisitor).visit_post(node)
        dec_nest
        printiln("} // namespace POA")
        println
        super
      end
    end # ServantProxyWriter
  end # Cxx11
end # IDL
