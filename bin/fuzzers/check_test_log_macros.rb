# encoding: utf-8
# -------------------------------------------------------------------
# @file    check_test_log_macros.rb
# @author  Marcel Smit
#
# @brief   The tests should use the log macro and not the taox11_xxx
#          streaming operators
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------

module Fuzzers
  class TAOX11TestLogMacroChecker
    include Fuzz::Fzzr
    def initialize
      @fuzz_id = :check_test_log_macros
      @description = 'checks against the use of ((ci|t)ao|dance)x11_xxx logging operators'
      @errormsg = 'detected use of a ((ci|t)ao|dance)x11_xxx logging operator'
    end

    def applies_to?(object)
      !is_excluded?(object)
    end

    def run(object, apply_fix)
      object.iterate(fuzz_id) do |lnptr|
        if lnptr.text =~ /(^|\s+)((ci|t)ao|dance)x11_(trace|debug|info|warning|error|critical|panic)(\s+|\.+)/
          lnptr.mark_error
        end
      end
    end
  end

  Fuzz.register_fzzr(TAOX11TestLogMacroChecker.new)
end
