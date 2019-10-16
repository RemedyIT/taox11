# encoding: utf-8
# -------------------------------------------------------------------
# @file    check_brief.rb
# @author  Johnny Willemsen
#
# @brief   TAOX11 brief doxygen checker
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------

module Fuzzers
  class TAOX11BriefChecker
    include Fuzz::Fzzr
    def initialize
      @fuzz_id = :check_brief
      @description = 'checks against the use of the @brief with no description'
      @errormsg = 'detected @brief with no description'
    end

    def applies_to?(object)
      Fuzz::FileObject === object &&
        !is_excluded?(object)
    end

    def run(object, apply_fix)
      object.iterate(fuzz_id) do |lnptr|
        if lnptr.text =~ /@brief$/i
          lnptr.mark_error
        end
      end
    end
  end

  Fuzz.register_fzzr(TAOX11BriefChecker.new)
end
