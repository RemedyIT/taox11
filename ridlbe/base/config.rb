#--------------------------------------------------------------------
# @file    config.rb
# @author  Martin Corino
#
# @brief   RIDL backend configuration
#
# @copyright Copyright (c) Remedy IT Expertise BV
# Chamber of commerce Rotterdam nr.276339, The Netherlands
#--------------------------------------------------------------------

module IDL

  module Base
    COPYRIGHT = "Copyright (c) 2007-#{Time.now.year} Remedy IT Expertise BV, The Netherlands".freeze
    TITLE = 'RIDL backend base'.freeze
    VERSION = {
        :major => 1,
        :minor => 0,
        :release => 0
    }

    ## Configure backend
    #
    Backend.configure('base', File.dirname(__FILE__), TITLE, COPYRIGHT, VERSION) do |becfg|

      # setup backend option handling
      #
      becfg.on_setup do |optlist, ridl_params|

        optlist.for_switch '--add-templates=PATH', :type => String,
             :description => [
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
