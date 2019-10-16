# encoding: utf-8
# -------------------------------------------------------------------
# @file    check_cout_cerr.rb
# @author  Marcel Smit
#
# @brief   TAOX11 cout/cerr in core checker
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------

module Fuzzers
  class TAOX11CoutCerrChecker
    include Fuzz::Fzzr
    def initialize
      @fuzz_id = :check_cout_cerr
      @description = 'checks against the use of cout and cerr in the CORE code (to be expanded to all code)'
      @errormsg = 'detected use of cout and/or cerr'
    end

    OBJECT_EXTS = ['h', 'hxx', 'hpp', 'c', 'cc', 'cxx', 'cpp', 'H', 'C']

    def applies_to?(object)
      Fuzz::FileObject === object &&
        OBJECT_EXTS.include?(object.ext) &&
        !is_excluded?(object)
    end

    def run(object, apply_fix)
      object.iterate(fuzz_id) do |lnptr|
        if lnptr.text =~ /(^|\s+)(cout|cerr|std::cout|std::cerr|std::wcout|std::wcerr|wcout|wcerr)(\s+|$)/
          lnptr.mark_error
        end
      end
    end
  end

  Fuzz.register_fzzr(TAOX11CoutCerrChecker.new)
end
