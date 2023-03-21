#--------------------------------------------------------------------
# @file    genidl.rb
# @author  Marijke Hengstmengel/Martin Corino
#
# @brief   Generate IDL definitions for CORBA project
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'brix11/command'

module BRIX11
  module TAOX11
    class GenerateIDL < Command::Base
      DESC = 'Generate IDL definitions for CORBA project.'.freeze

      OPTIONS = {
        name: 'test',
        modules: [],
        interface: nil,
        shutdown: true
      }

      def self.setup(optparser, options)
        options[:genidl] = OPTIONS.dup
        optparser.banner = "#{DESC}\n\n" +
                           "Usage: #{options[:script_name]} gen[erate] idl [options] [NAME]\n\n" +
                           "       NAME := name to use for IDL file; default 'test'\n\n"

        optparser.on('-i', '--interface', '=NAME',
                     'Defines name to use for interface.',
                     'Use scoped name (i.e. <name>::[<name>::]::<name>) to generate enclosing module(s).',
                     'Default: derived from IDL file basename with first character uppercased') { |v|
                       options[:genidl][:modules] = v.split('::')
                       options[:genidl][:interface] = options[:genidl][:modules].pop
                     }
        optparser.on('--without-shutdown',
                     'Do NOT generate a shutdown method.',
                     'Default: generate shutdown.') { options[:genidl][:shutdown] = false }
      end

      def run(argv)
        argv ||= []
        unless argv.empty? || Command.is_command_arg?(argv.first, options) || argv.first.start_with?('-')
          options[:genidl][:name] = argv.shift
        end
        unless options[:genidl][:interface]
          options[:genidl][:interface] = options[:genidl][:name].sub(/\A(.)/) { $1.upcase }
        end
        # run file creation task for IDL file
        unless File.exist?("#{options[:genidl][:name]}.idl") && !options[:force]
          w = IDLWriter.new(GenFile.new("#{options[:genidl][:name]}.idl"), options[:genidl])
          w.visit_corba_idl
        end
        true # make sure to return true if we get to here to allow chaining
      end

      class IDLWriter < Generate::Writer
        def initialize(output, options)
          @options = options
          super(output)
        end

        helper_method :project_name, :module_names,
                      :interface_name, :has_shutdown?

        def project_name
          @options[:name]
        end

        def module_names
          @options[:modules]
        end

        def interface_name
          @options[:interface]
        end

        def has_shutdown?
          @options[:shutdown]
        end
      end

      Command.register('generate:idl', DESC, TAOX11::GenerateIDL)
    end # GenerateIDL
  end # Common
end # BRIX11
