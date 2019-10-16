# encoding: utf-8
# -------------------------------------------------------------------
# @file    check_ace_os_sleep.rb
# @author  Martin Corino
#
# @brief   TAOX11 ACE_ERROR checker
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------

module Fuzzers
  class TAOX11AceSleepChecker
    include Fuzz::Fzzr
    def initialize
      @fuzz_id = :check_ace_os_sleep
      @description = 'checks against the use of the old ACE sleep method in code'
      @errormsg = 'detected use of ACE_OS::sleep. Please use std::this_thread::sleep_for instead.'
    end

    OBJECT_EXTS = ['h', 'hxx', 'hpp', 'c', 'cc', 'cxx', 'cpp', 'H', 'C', 'asm']

    def applies_to?(object)
      Fuzz::FileObject === object &&
        OBJECT_EXTS.include?(object.ext) &&
        !is_excluded?(object)
    end

    def run(object, apply_fix)
      object.iterate(fuzz_id) do |lnptr|
        if lnptr.text =~ /(^|\s+)(ACE_OS::sleep)([\(]|\s|$)/
          lnptr.mark_error
        end
      end
    end
  end

  Fuzz.register_fzzr(TAOX11AceSleepChecker.new)
end
