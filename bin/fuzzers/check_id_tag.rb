# encoding: utf-8
# -------------------------------------------------------------------
# @file    check_id_tag.rb
# @author  Marcel Smit
#
# @brief   TAOX11 $Id$ tag checker
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------

module Fuzzers
  class TAOX11IdTagChecker
    include Fuzz::Fzzr
    def initialize
      @fuzz_id = :check_id_tag
      @description = 'checks against the use of the $Id$ tag'
      @errormsg = 'detected use of $Id:$'
    end

    def applies_to?(object)
      Fuzz::FileObject === object &&
        !is_excluded?(object)
    end

    def run(object, apply_fix)
      object.iterate(fuzz_id) do |lnptr|
        if lnptr.text =~ /^\s*([\*]|\/\/|#|^)\s*([\$]Id)/i
          lnptr.mark_error
        end
      end
    end
  end

  Fuzz.register_fzzr(TAOX11IdTagChecker.new)
end
