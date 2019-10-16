# encoding: utf-8
# -------------------------------------------------------------------
# @file    check_printf_keyword.rb
# @author  Johnny Willemsen
#
# @brief   TAOX11 printf checker
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------

module Fuzzers
  class TAOX11PrintfChecker
    include Fuzz::Fzzr
    def initialize
      @fuzz_id = :check_printf_keyword
      @description = 'checks against the use of the printf keyword in test code'
      @errormsg = 'detected use of printf'
    end

    OBJECT_EXTS = ['h', 'hxx', 'hpp', 'c', 'cc', 'cxx', 'cpp', 'H', 'C']

    def applies_to?(object)
      Fuzz::FileObject === object &&
        OBJECT_EXTS.include?(object.ext) &&
        !is_excluded?(object)
    end

    def run(object, apply_fix)
      object.iterate(fuzz_id) do |lnptr|
        if lnptr.text =~ /(^|\s+|::)(printf)(\s+|$)/
          lnptr.mark_error
        end
      end
    end
  end

  Fuzz.register_fzzr(TAOX11PrintfChecker.new)
end
