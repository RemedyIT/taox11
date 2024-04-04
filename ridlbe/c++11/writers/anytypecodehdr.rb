#--------------------------------------------------------------------
# @file    anytypecodehdr.rb
# @author  Johnny Willemsen
#
# @brief   walker class for Any and TypeCode support
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/c++11/writers/stubheader'
require 'ridlbe/c++11/writers/helpers/version_helper'
require 'ridlbe/c++11/writers/helpers/include_guard_helper'

module IDL
  module Cxx11
    class AnyTypeCodeHeaderBaseWriter < CxxCodeWriterBase
      def initialize(output = STDOUT, opts = {})
        super
        self.template_root = File.join('cli', 'hdr')
      end

      attr_accessor :include_guard
    end

    class AnyTypeCodeHeaderWriter < AnyTypeCodeHeaderBaseWriter
      helper Cxx11::IncludeGuardHelper

      def initialize(output = STDOUT, opts = {})
        super
        @default_pre_includes = [
          'tao/CDR.h',
          'tao/Invocation_Adapter.h',
          'tao/Object_T.h'
        ]
        if params[:gen_typecodes]
          @default_pre_includes << 'tao/AnyTypeCode/TypeCode.h'
          @default_pre_includes << 'tao/AnyTypeCode/TypeCode_Constants.h'
        end
        if params[:gen_any_ops]
          @default_pre_includes << 'tao/AnyTypeCode/Any_Impl_T.h'
        end
        @default_post_includes = [
          'tao/x11/tao_corba.h',
          'tao/x11/objproxy.h',
          'tao/x11/system_exception_x11.h',
          'tao/x11/exception_macros.h',
          'tao/x11/anytypecode/typecode_impl.h',
          'tao/x11/anytypecode/typecode.h'
        ]

        @include_guard = "__RIDL_#{File.basename(params[:output_anytypecode_header] || '').to_random_include_guard}_INCLUDED__"
      end

      def pre_visit(parser)
        visit_includes(parser)

        visit_typecodes(parser)

        super
      end

      def post_visit(parser)
        visit_anyops(parser)

        super
        visitor(PostVisitor).visit
      end

      def visit_includes(parser)
        writer(AnyTypeCodeHeaderIncludeWriter,
               { default_pre_includes: @default_pre_includes,
                 default_post_includes: @default_post_includes }) do |w|
          w.include_guard = @include_guard
          w.visit_nodes(parser)
        end
      end

      def visit_anyops(parser)
        writer(StubHeaderAnyOpWriter).visit_nodes(parser)
      end

      def visit_typecodes(parser)
        writer(StubHeaderTypecodeWriter).visit_nodes(parser)
      end
    end # AnyTypeCodeHeaderWriter

    class AnyTypeCodeHeaderIncludeWriter < AnyTypeCodeHeaderBaseWriter
      helper Cxx11::VersionHelper
      helper Cxx11::IncludeGuardHelper

      def initialize(output = STDOUT, opts = {})
        super
        @includes = []
        @default_pre_includes = params[:default_pre_includes]
        @default_post_includes = params[:default_post_includes]
        @default_post_includes << 'tao/x11/anytypecode/any.h'

        if params[:gen_typecodes]
          if params[:gen_anytypecode_source]
            @default_post_includes << 'tao/x11/anytypecode/typecode_ref.h'
          else
            @default_post_includes << 'tao/x11/anytypecode/typecode.h'
          end
        end
        @default_post_includes << 'tao/x11/anytypecode/typecode_constants.h' if params[:gen_typecodes]
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
      end

      def enter_interface(node)
      end

      def visit_exception(_node)
      end

      def visit_operation(node)
      end

      def visit_attribute(node)
      end

      def declare_valuetype(_node)
      end

      def enter_valuetype(node)
      end

      def visit_valuebox(node)
      end

      def enter_struct(node)
      end

      def enter_union(node)
      end

      def enter_exception(node)
      end

      def visit_typedef(node)
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
        when IDL::Type::WString,
             IDL::Type::WChar
          add_post_include('tao/x11/wstringwchar_ostream.h') if params[:gen_ostream_operators]
        end
      end

      def add_include(inc_file)
        @includes << inc_file unless @includes.include?(inc_file)
      end

      def add_post_include(inc_file)
        @default_post_includes << inc_file unless @default_post_includes.include?(inc_file)
      end
    end
  end # Cxx11
end # IDL
