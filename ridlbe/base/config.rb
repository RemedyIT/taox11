#--------------------------------------------------------------------
# @file    config.rb
# @author  Martin Corino
#
# @brief   RIDL backend configuration
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL
  module Base
    COPYRIGHT = "Copyright (c) 2007-#{Time.now.year} Remedy IT Expertise BV, The Netherlands".freeze
    TITLE = 'RIDL backend base'.freeze

    class << self
      VERSION_REGEXP = /\#\s*define\s+TAOX11_(\w+)_VERSION\s+(\d+)/

      def determine_taox11_version
        x11_version = {
          major: 0,
          minor: 0,
          beta: 0
        }

        base = File.join(File.dirname(__FILE__), '..', '..', 'tao', 'x11', 'versionx11.h')
        File.open(base, "r") do |file|
          while (line = file.gets)
            if VERSION_REGEXP =~ line
              x11_version[::Regexp.last_match(1).downcase.to_sym] = ::Regexp.last_match(2).to_i
            end
          end
        end
        x11_version[:release] ||= x11_version[:beta]
        x11_version
      end # determine_taox11_version
      private :determine_taox11_version

      def taox11_version
        @taox11_version ||= determine_taox11_version
      end
    end

    ## Configure backend
    #
    Backend.configure('base', File.dirname(__FILE__), TITLE, COPYRIGHT, Base.taox11_version) do |becfg|

      # setup backend option handling
      #
      becfg.on_setup do |optlist, ridl_params|

        optlist.for_switch '--add-templates=PATH', type: String,
             description: [
                'Add a template library basepath to be evaluated before backend templates.'
                             ] do |swcfg|
          swcfg.on_exec do |arg, params|
            Template.add_user_templates(arg)
          end
        end
      end

      # process input / generate code
      # arguments:
      #   in parser - parser object with full AST from parsed source
      #   in options - initialized option hash
      #
      becfg.on_process_input do |parser, options|
        # do nothing
      end # becfg.on_process_input

    end # Backend.configure
  end # Base
end # IDL

# config modules
Dir.glob(File.join(File.dirname(__FILE__), 'config', '*.rb')).each do |f|
  require "ridlbe/base/config/#{File.basename(f, '.*')}"
end
