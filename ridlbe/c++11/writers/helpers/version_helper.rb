#--------------------------------------------------------------------
# @file    version_helper.rb
# @author  Martin Corino
#
# @brief   Helper module for TAOX11 version check generation
#
# @copyright Copyright (c) Remedy IT Expertise BV
# Chamber of commerce Rotterdam nr.276339, The Netherlands
#--------------------------------------------------------------------

module IDL

  module Cxx11

    module VersionHelper

      def taox11_version_header
        "tao/x11/versionx11.h"
      end

      def taox11_version
        writer.params[:taox11_version]
      end

      def taox11_title
        IDL::Cxx11::TITLE
      end

      def taox11_version_check
        "#include /**/ \"#{taox11_version_header}\"\n\n"+
            "#if TAOX11_MAJOR_VERSION != #{taox11_version[:major]} || "+
            "TAOX11_MINOR_VERSION != #{taox11_version[:minor]} || "+
            "TAOX11_MICRO_VERSION != #{taox11_version[:micro]}\n"+
            "#error This file was generated with another #{taox11_title} version "+
            "(#{taox11_version[:major]}.#{taox11_version[:minor]}.#{taox11_version[:micro]}). "+
            "Please re-generate.\n#endif"
      end
    end

  end # Cxx11

end # IDL
