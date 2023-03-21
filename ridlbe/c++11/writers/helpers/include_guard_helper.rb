#--------------------------------------------------------------------
# @file    include_guard_helper.rb
# @author  Martin Corino
#
# @brief   Helper module for TAOX11 version check generation
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL
  module Cxx11
    module IncludeGuardHelper
      def include_guard_macro
        writer.include_guard
      end

      def include_guard_start
        %Q{#ifndef #{include_guard_macro}\n#define #{include_guard_macro}}
      end

      def include_guard_end
        %Q{#endif /* #{include_guard_macro} */}
      end
    end # IncludeGuardHelper
  end # Cxx11
end # IDL
