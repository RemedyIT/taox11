#--------------------------------------------------------------------
# @file   writerbase.rb
# @author Martin Corino
#
# @brief  Base class for IDL typecode and client stubs
#
# @copyright Copyright (c) Remedy IT Expertise BV
# Chamber of commerce Rotterdam nr.276339, The Netherlands
#--------------------------------------------------------------------

module IDL
  module Cxx11

    # Output writer base class for sourcecode.
    #
    class CxxCodeWriterBase < Base::Writer

      include IDL::Base::CodeWriterMethods

      def initialize(output = STDOUT, opts = {})
        super
        @root_namespace = nil
        @disable_scope_tracking = false
        @only_record_module_scopes = false
      end

      protected

      # overrides providing language mapping specifics

      def write_open_scope(scope)
        printiln("namespace #{scope.cxxname}")
        printiln("{")
        inc_nest
      end

      def write_close_scope(scope)
        dec_nest
        printiln("} // namespace #{scope.cxxname}")
      end

    end # CxxCodeWriterBase

  end
end
