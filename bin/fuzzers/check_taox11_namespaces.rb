# encoding: utf-8
# -------------------------------------------------------------------
# @file    check_taox11_namespaces.rb
# @author  Martin Corino
#
# @brief   TAOX11 namespace checker
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------

module Fuzzers
  class TAOX11NamespaceChecker
    include Fuzz::Fzzr
    def initialize
      @fuzz_id = :check_taox11_namespaces
      @description = 'checks against the use of TAOX11_(NAMESPACE|CORBA|PORTABLE_SERVER|IDL) macros in user/test code'
      @errormsg = 'detected use of a TAOX11_(NAMESPACE|CORBA|PORTABLE_SERVER|IDL) macro'
    end

    OBJECT_EXTS = ['h', 'hxx', 'hpp', 'c', 'cc', 'cxx', 'cpp', 'H', 'C']

    def applies_to?(object)
      Fuzz::FileObject === object &&
        OBJECT_EXTS.include?(object.ext) &&
        !is_excluded?(object)
    end

    def run(object, apply_fix)
      object.iterate(fuzz_id) do |lnptr|
        if lnptr.text =~ /(TAOX11_NAMESPACE|TAOX11_CORBA|TAOX11_PORTABLE_SERVER|TAOX11_IDL)::/ ||
           lnptr.text =~ /namespace\s+(TAOX11_NAMESPACE|TAOX11_CORBA|TAOX11_PORTABLE_SERVER|TAOX11_IDL)/
          lnptr.mark_error
        end
      end
    end
  end

  Fuzz.register_fzzr(TAOX11NamespaceChecker.new)
end
