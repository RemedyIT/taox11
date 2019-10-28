# encoding: utf-8
# -------------------------------------------------------------------
# @file    check_executablebit.rb
# @author  Johnny Willemsen
#
# @brief   executable bit checker
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------

module Fuzzers
  class ExecutablebitChecker
    include Fuzz::Fzzr
    def initialize
      @fuzz_id = :check_executablebit
      @description = 'checks for executable bit set'
    end

    EXE_OBJECT_EXTS = ['py', 'pl', 'sh', 'bat']

    def applies_to?(object)
      Fuzz::FileObject === object &&
        !is_excluded?(object)
    end

    def run(object, apply_fix)
      if EXE_OBJECT_EXTS.include?(object.ext)
        if !File::executable?(object.path) || !File::executable_real?(object.path)
          Fuzz.log_error(%Q{#{object.path} - lacks executable bit})
          if apply_fix
            FileUtils.chmod "+x", object.path
          end
          return false
        else
          return true
        end
      end
      if File::executable?(object.path) || File::executable_real?(object.path)
        Fuzz.log_error(%Q{#{object.path} - has executable bit set})
        if apply_fix
          FileUtils.chmod "-x", object.path
        end
        return false
      end
      return true
    end

  end

  Fuzz.register_fzzr(ExecutablebitChecker.new)
end
