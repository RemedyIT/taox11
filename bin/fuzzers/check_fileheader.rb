# encoding: utf-8
# -------------------------------------------------------------------
# @file    check_fileheader.rb
# @author  Marcel Smit
#
# @brief   TAOX11 file header checker
#
# @copyright Copyright (c) Remedy IT Expertise BV
# -------------------------------------------------------------------

module Fuzzers
  class TAOX11FileHeaderChecker
    include Fuzz::Fzzr
    def initialize
      @fuzz_id = :check_fileheader
      @description = 'checks whether a file contains a correct file header'
      @def_errormsg='incorrect file header detected:'
      @errormsg = ''
    end

    def applies_to?(object)
      Fuzz::FileObject === object &&
        !is_excluded?(object)
    end

    def run(object, apply_fix)
      @errormsg = ''

      file_tag_found = false
      author_tag_found = false
      copyright_tag_found = false

      object.iterate(fuzz_id) do |lnptr|
        if lnptr.text.match('@file')
          file_tag_found = true
          ln = lnptr.text.slice(lnptr.text.rindex(' ')+1, object.name.length)
          if ln != object.name
            msg = ' - Incorrect file name used'
            @errormsg.empty? ? @errormsg = @def_errormsg+msg : @errormsg += msg
            lnptr.mark_error
          end
        elsif lnptr.text.match('@author')
          author_tag_found = true
          if lnptr.text =~ /@author$/i
            msg = ' - detected @author without a name.'
            @errormsg.empty? ? @errormsg = @def_errormsg+msg : @errormsg += msg
            lnptr.mark_error
          end
        elsif lnptr.text.match('@brief')
          if lnptr.text =~ /@brief$/i
            msg = ' - detected @brief without a description.'
            @errormsg.empty? ? @errormsg = @def_errormsg+msg : @errormsg += msg
            lnptr.mark_error
          end
        elsif lnptr.text.match('@copyright')
          copyright_tag_found = true
          if lnptr.text =~ /@copyright$/i
            msg = ' - detected @copyright without a description.'
            @errormsg.empty? ? @errormsg = @def_errormsg+msg : @errormsg += msg
            lnptr.mark_error
          end
        end

        if file_tag_found && author_tag_found && copyright_tag_found
          # Everything ok.
          lnptr.to_eof
        elsif lnptr.line_nr >= 30 || lnptr.line_nr == object.lines.count
          msg = ' - No or incorrect file header detected before line 30'
          if !file_tag_found
            msg += ' - No @file tag detected'
          end
          if !author_tag_found
            msg += ' - No @author tag detected'
          end
          if !copyright_tag_found
            msg += ' - No @copyright tag detected'
          end
          if !msg.empty?
            @errormsg.empty? ? @errormsg = @def_errormsg+msg : @errormsg += msg
            lnptr.mark_error 30
          end
          lnptr.to_eof
        end
      end

    end #def run
  end

  Fuzz.register_fzzr(TAOX11FileHeaderChecker.new)
end
