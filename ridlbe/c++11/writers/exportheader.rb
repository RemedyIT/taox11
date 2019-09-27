#--------------------------------------------------------------------
# @file    exportheader.rb
# @author  Martin Corino
#
# @brief   walker class for export header generation
#
# @copyright Copyright (c) Remedy IT Expertise BV
# Chamber of commerce Rotterdam nr.276339, The Netherlands
#--------------------------------------------------------------------
require 'ridlbe/c++11/writerbase'
require 'ridlbe/c++11/writers/helpers/version_helper'
require 'ridlbe/c++11/writers/helpers/include_guard_helper'

module IDL
  module Cxx11

    class ExportHeaderWriter < Base::Writer

      helper Cxx11::IncludeGuardHelper
      helper Cxx11::VersionHelper
      helper_method :macro_base, :file_name

      attr_reader :include_guard

      def initialize(macro, filename, output = STDOUT, opts = {})
        super(output, opts)
        self.template_root = File.join('exp', 'hdr')
        @export_macro = macro
        @file_name = filename
        @include_guard = "__RIDL_#{filename.to_random_include_guard}_EXPORT_INCLUDED__"
      end

      def run(parser=nil)
        visitor(ExportVisitor).visit
      end

      private

      def macro_base
        @export_macro.sub(/_Export$/, '')
      end

      def file_name
        @file_name.sub(/\./, '_').upcase
      end
    end

  end # Cxx11
end # IDL
