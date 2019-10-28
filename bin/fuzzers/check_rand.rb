# encoding: utf-8
# -------------------------------------------------------------------
# @file   check_rand.rb
# @author Marcel Smit
#
# @brief  Check for the use of rand and rand_r in the tests.
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------
module Fuzzers
  class TAOX11RandChecker
    include Fuzz::Fzzr
    def initialize
      @fuzz_id = :check_rand
      @description = 'checks against the use of rand and rand_r in the tests.'
      @errormsg = 'detected use of \'rand\' or \'rand_r\''
    end

    OBJECT_EXTS = ['h', 'hxx', 'hpp', 'c', 'cc', 'cxx', 'cpp', 'H', 'C', 'inl']

    def applies_to?(object)
      Fuzz::FileObject === object &&
        OBJECT_EXTS.include?(object.ext) &&
        !is_excluded?(object)
    end

    def run(object, apply_fix)
      object.iterate(fuzz_id) do |lnptr|
        if lnptr.text =~ /(^|\s+|::)(rand|rand_r)(\s+|$)/
          lnptr.mark_error
        end
      end
    end
  end

  Fuzz.register_fzzr(TAOX11RandChecker.new)
end
