#--------------------------------------------------------------------
# @file    generate.rb
# @author  Martin Corino
#
# @brief   BRIX11 Output generator classes
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'erb'
require 'delegate'
require 'brix/taox11/lib/eridl'
require 'brix/taox11/lib/templates'

module BRIX11
  GenFile = IDL::GenFile

  # reopen Command module
  module Command
    # override run method
    def self.run(cmd, argv)
      rc = false
      BRIX11::GenFile.transaction do
        rc = cmd.run(argv)
      end
      rc
    end
  end

  module Generate
    class Visitor < IDL::Base::Visitor
      def initialize(output, properties, writer)
        super
      end

      # override base method
      # NOTE this override does not resolve template extensions like the RIDL base backend supports
      # NOTE this override uses a BRIX template manager
      def resolve_template(sym)
        tpl = self.class.template_map[sym.to_sym]
        unless tpl
          tpl = sym if BRIX11::Template.exists?(File.join(params[:template_root], sym.to_s))
        end
        tpl
      end

      # override base method
      # NOTE this override uses a BRIX template manager
      def visit_template(template, extra_props = {})
        yield(self, template) if block_given?
        if BRIX11::Template.exists?(File.join(params[:template_root], template.to_s)) || required_template?(template)
          visit_resolved_template(template, extra_props)
        end
      end

      # override base method; ignore bases argument (3rd) since this is not used with BRIX
      # NOTE this override uses a BRIX template manager
      def visit_resolved_template(template, extra_props = {}, _ = nil)
        _file = File.join(params[:template_root], template.to_s)
        IDL::Base::Visitor.context(self, properties, extra_props).parse(@output, BRIX11::Template.new(_file), extra_props)
        @output.erbout.flush
      end
    end

    class Writer < IDL::Base::Writer
      def initialize(output = STDOUT, opts = {})
        super
        self.template_root = ''
      end

      private

      def method_missing(method, *args, &block)
        # only match regular visit_ methods (no ?,!,= decorations)
        if Visitor::VISIT_METHOD_RE =~ method
          # create a base visitor and trigger visit on this
          visitor(Visitor).send(method, *args, &block)
        else
          super
        end
      end
    end
  end
end
