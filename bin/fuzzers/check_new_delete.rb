# encoding: utf-8
# -------------------------------------------------------------------
# @file    check_new_delete.rb
# @author  Martin Corino
#
# @brief   TAOX11 new delete checker
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------

module Fuzzers
  class TAOX11NewDeleteChecker
    include Fuzz::Fzzr
    def initialize
      @fuzz_id = :check_new_delete
      @description = 'checks against the use of new and/or delete in test code'
      @errormsg = 'detected use of new and/or delete'
    end

    OBJECT_EXTS = ['h', 'hxx', 'hpp', 'c', 'cc', 'cxx', 'cpp', 'H', 'C']

    def applies_to?(object)
      Fuzz::FileObject === object &&
        OBJECT_EXTS.include?(object.ext) &&
        !is_excluded?(object)
    end

    def run(object, apply_fix)
      object.iterate(fuzz_id) do |lnptr|
        if lnptr.text =~ /(^|\s+|\()(new|delete)(\s+|$)/
          lnptr.mark_error
        end
      end
    end
  end

  Fuzz.register_fzzr(TAOX11NewDeleteChecker.new)
end
