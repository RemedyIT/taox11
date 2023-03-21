#--------------------------------------------------------------------
# @file    runlist.rb
# @author  Marijke Hengstmengel/Martin Corino
#
# @brief   Run an ACE auto_run_test regression test list.
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'brix11/command'

module BRIX11
  module TAOX11
    class RunList < Command::Base
      DESC = 'Run an auto_run_test regression test list.'.freeze

      OPTIONS = {
          list: nil,
          root: nil,
          sandbox: nil
      }

      def self.setup(optparser, options)
        options[:runlist] = OPTIONS.dup

        optparser.banner = "#{DESC}\n\n" +
            "Usage: #{options[:script_name]} run list [options] [-- test-options]\n" +
            "       (any test-options following '--' will be passed on unchecked)\n\n"

        optparser.on('-l', '--list', '=PATH',
                     'Specifies regression test list to feed auto_run_test.',
                     'Default: none') { |v|
                        options[:runlist][:list] = File.expand_path(v)
                        BRIX11.log_fatal("Cannot access list file #{v}") unless File.file?(options[:runlist][:list])
                     }
        optparser.on('-r', '--root', '=PATH',
                     'Specifies root directory for running the tests.',
                     'Default: <listfile location>') { |v|
                        options[:runlist][:root] = File.expand_path(v)
                        BRIX11.log_fatal("Cannot access root directory #{v}") unless File.directory?(options[:runlist][:root])
                     }
        optparser.on('-s', '--sandbox', '=SANDBOX',
                     'Specifies sandbox for running each separate test.',
                     'Default: none') { |v|
                        options[:runlist][:sandbox] = v
                     }
      end

      def run(argv)
        argv ||= []
        runenv = Exec.run_environment

        argv.shift unless argv.empty? || argv.first != '--'

        BRIX11.log_fatal('No test list specified.') unless options[:runlist][:list]

        test_argv = []
        test_argv << File.join(runenv['ACE_ROOT'], 'bin', 'auto_run_tests.pl')
        test_argv << '-l' << options[:runlist][:list]
        test_argv << '-r' << (options[:runlist][:root] || File.dirname(options[:runlist][:list]))
        test_argv << '-s' << options[:runlist][:sandbox] if options[:runlist][:sandbox]
        Common::Configure::Configurator.get_test_config.each { |cfg| test_argv << '-Config' << cfg }
        while !argv.empty? && argv.first != '--'
          test_argv << argv.shift
        end
        rc, exitcode = Exec.runcmd('perl', *test_argv)
        log_warning("'#{test_argv.join(' ')}' terminated with exitcode #{exitcode}") unless rc
        rc
      end

      Command.register('run:list', DESC, TAOX11::RunList)
    end # RunList
  end # Common
end # BRIX11
