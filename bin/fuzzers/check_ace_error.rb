# encoding: utf-8
# -------------------------------------------------------------------
# @file    check_ace_error.rb
# @author  Martin Corino
#
# @brief   TAOX11 ACE_ERROR checker
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------

module Fuzzers
  class TAOX11AceErrorChecker
    include Fuzz::Fzzr
    def initialize
      @fuzz_id = :check_ace_error
      @description = 'checks against the use of the old ACE logging macros in test code'
      @errormsg = 'detected use of ACE_ERROR, ACE_DEBUG, ACE_ERROR_RETURN, TAOLIB_DEBUG, and/or TAOLIB_ERROR'
    end

    OBJECT_EXTS = ['h', 'hxx', 'hpp', 'c', 'cc', 'cxx', 'cpp', 'H', 'C', 'asm']

    def applies_to?(object)
      Fuzz::FileObject === object &&
        OBJECT_EXTS.include?(object.ext) &&
        !is_excluded?(object)
    end

    def run(object, apply_fix)
      object.iterate(fuzz_id) do |lnptr|
        if lnptr.text =~ /(^|\s+)(ACE_ERROR|ACE_ERROR_RETURN|ACE_DEBUG|TAOLIB_DEBUG|TAOLIB_ERROR)([\(]|\s|$)/
          lnptr.mark_error
        end
      end
    end
  end

  Fuzz.register_fzzr(TAOX11AceErrorChecker.new)
end
