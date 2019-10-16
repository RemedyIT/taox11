# encoding: utf-8
# -------------------------------------------------------------------
# @file    check_noending_newline.rb
# @author  Johnny Willemsen
#
# @brief   Checks whether a file ends with a newline
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------

module Fuzzers
  class NoEndingNewlineChecker
    include Fuzz::Fzzr
    def initialize
      @fuzz_id = :check_noending_newline
      @description = 'checks whether a file ends with a newline'
      @errormsg = 'file does not end with a newline'
    end

    OBJECT_EXTS = ['h', 'hxx', 'hpp', 'c', 'cc', 'cxx', 'cpp', 'H', 'C', 'idl']

    def applies_to?(object)
      Fuzz::FileObject === object &&
        OBJECT_EXTS.include?(object.ext) &&
        !is_excluded?(object)
    end

    def run(object, apply_fix)
      object.iterate(fuzz_id) do |lnptr|
        if (object.lines.count == lnptr.line_nr)
          if lnptr.text !~ /\n/
            if apply_fix
              lnptr.text << "\n"
            else
              lnptr.mark_error
            end
          end
        end
      end
    end
  end

  Fuzz.register_fzzr(NoEndingNewlineChecker.new)
end
