#--------------------------------------------------------------------
# @file    genruntest.rb
# @author  Marijke Hengstmengel/Martin Corino
#
# @brief   Generate test runner script for CORBA project
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'brix11/command'

module BRIX11
  module TAOX11

    class GenerateRunTest < Command::Base

      DESC = 'Generate test runner script for CORBA project.'.freeze

      OPTIONS = {
          name: 'run_test',
          client: 'client',
          server: 'server',
      }

      def self.setup(optparser, options)
        options[:genruntest] = OPTIONS.dup

        optparser.banner = "#{DESC}\n\n"+
                           "Usage: #{options[:script_name]} gen[erate] runner|script [options] [NAME]\n\n"+
                           "       NAME := name to use for script (no extension); default 'run_test'\n\n"

        optparser.on('-S', '--server', '=NAME',
                     'Defines name to use for server executable (no extension).',
                     'Default: server') { |v| options[:genruntest][:server] = v }
        optparser.on('-C', '--client', '=NAME',
                     'Defines name to use for client executable (no extension).',
                     'Default: client') { |v| options[:genruntest][:client] = v }
      end

      def run(argv)
        argv ||= []
        unless argv.empty? || Command.is_command_arg?(argv.first, options) || argv.first.start_with?('-')
          options[:genruntest][:name] = argv.shift
        end
        # run file creation task for test runner
        unless File.exist?("#{options[:genruntest][:name]}.pl") && !options[:force]
          w = RunTestWriter.new(GenFile.new("#{options[:genruntest][:name]}.pl"), options[:genruntest])
          w.visit_corba_run_test
        end
        true # make sure to return true if we get to here to allow chaining
      end

      class RunTestWriter < Generate::Writer
        def initialize(output, options)
          @options = options
          super(output)
        end

        helper_method :test_name, :server_name, :client_name

        def test_name
          @options[:name]
        end
        def server_name
          @options[:server]
        end
        def client_name
          @options[:client]
        end
      end

      Command.register('generate:runner|script', DESC, TAOX11::GenerateRunTest)
    end # GenerateTest

  end # Common
end # BRIX11
