# encoding: utf-8
# -------------------------------------------------------------------
# @file    check_exit_keyword.rb
# @author  Marcel Smit
#
# @brief   TAOX11 exit checker
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------

module Fuzzers
  class TAOX11ExitChecker
    include Fuzz::Fzzr
    def initialize
      @fuzz_id = :check_exit_keyword
      @description = 'checks against the use of the exit keyword in test code'
      @errormsg = 'detected use of exit'
    end

    OBJECT_EXTS = ['h', 'hxx', 'hpp', 'c', 'cc', 'cxx', 'cpp', 'H', 'C']

    def applies_to?(object)
      Fuzz::FileObject === object &&
      OBJECT_EXTS.include?(object.ext) &&
        !is_excluded?(object)
    end

    def run(object, apply_fix)
      object.iterate(fuzz_id) do |lnptr|
        if lnptr.text =~ /(^|\s+)(exit)(\s+|$)/
          lnptr.mark_error
        end
      end
    end
  end

  Fuzz.register_fzzr(TAOX11ExitChecker.new)
end
