# encoding: utf-8
# -------------------------------------------------------------------
# @file    check_ace_unused_arg.rb
# @author  Marcel Smit
#
# @brief   TAOX11 ACE_UNUSED_ARG checker
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------

module Fuzzers
  class TAOX11AceUnusedArgChecker
    include Fuzz::Fzzr
    def initialize
      @fuzz_id = :check_ace_unused_arg
      @description = 'checks against the use of the old ACE_UNUSED_ARG macros in the code'
      @errormsg = 'detected use of ACE_UNUSED_ARG. Please use X11_UNUSED_ARG instead.'
    end

    OBJECT_EXTS = ['h', 'hxx', 'hpp', 'c', 'cc', 'cxx', 'cpp', 'H', 'C', 'asm', 'rb', 'erb']

    def applies_to?(object)
      Fuzz::FileObject === object &&
        OBJECT_EXTS.include?(object.ext) &&
        !is_excluded?(object)
    end

    def run(object, apply_fix)
      object.iterate(fuzz_id) do |lnptr|
        if lnptr.text =~ /(^|\s+)(ACE_UNUSED_ARG)([\(]|\s|$)/
          lnptr.mark_error
        end
      end
    end
  end

  Fuzz.register_fzzr(TAOX11AceUnusedArgChecker.new)
end
