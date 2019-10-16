# encoding: utf-8
# -------------------------------------------------------------------
# @file    check_catch_ex_as_const.rb
# @author  Marcel Smit
#
# @brief   Checks whether an exception is caught as const
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------

module Fuzzers
  class ExAsConstChecker
    include Fuzz::Fzzr
    def initialize
      @fuzz_id = :check_catch_ex_as_const
      @description = 'checks whether an exception is caught as const'
      @errormsg = 'exceptions should be caught as const'
    end

    OBJECT_EXTS = ['h', 'hxx', 'hpp', 'c', 'cc', 'cxx', 'cpp', 'H', 'C']

    def applies_to?(object)
      Fuzz::FileObject === object &&
        OBJECT_EXTS.include?(object.ext) &&
        !is_excluded?(object)
    end

    def run(object, apply_fix)
      object.iterate(fuzz_id) do |lnptr|
        if lnptr.text =~ /^\s*(catch\(|catch \()/
          unless lnptr.text =~ /([...])/
            if not lnptr.text =~ /const/
              lnptr.mark_error
            end
          end
        end
      end
    end
  end

  Fuzz.register_fzzr(ExAsConstChecker.new)
end
