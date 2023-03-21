#--------------------------------------------------------------------
# @file    runtest.rb
# @author  Marijke Hengstmengel/Martin Corino
#
# @brief   Run (regression) test script.
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'brix11/command'

module BRIX11
  module TAOX11
    class RunTest < Command::Base
      DESC = 'Run test runner script for project.'.freeze

      OPTIONS = {
        name: 'run_test',
        runopts: {},
        tool: nil
      }

      TOOLS = {
        '.pl' => 'perl',
        '.sh' => %w(bash -c),
        '.rb' => 'ruby',
        '.py' => 'python'
      }

      def self.setup(optparser, options)
        options[:runtest] = OPTIONS.dup

        optparser.banner = "#{DESC}\n\n" +
                           "Usage: #{options[:script_name]} run test|script [options] [TEST [test-options]]|[-- test-options]\n\n" +
                           "       TEST := Path to project folder or test script (extension guessed if not supplied).\n" +
                           "               Default script name = 'run_test.pl'\n" +
                           "       (any test-options will be passed unchecked to the script)\n\n"

        optparser.on('--program', '=TOOL',
                     'Use TOOL to execute test script.',
                     "Default: guessed from extension (supported: #{TOOLS.keys.join(' ')}).") { |v| options[:runtest][:tool] = v }
      end

      private

      def find_tool(cmd)
        TOOLS.fetch(File.extname(cmd)) { |_| log_error("Do not know how to run #{cmd}"); nil }
      end

      public

      def run(argv)
        argv ||= []
        unless argv.empty? || Command.is_command_arg?(argv.first, options) || argv.first.start_with?('-')
          options[:runtest][:test] = argv.shift
        else
          argv.shift unless argv.empty? || argv.first != '--'
        end
        options[:runtest][:test] ||= 'run_test.pl'
        if File.directory?(options[:runtest][:test])
          options[:runtest][:runopts][:chdir] = options[:runtest][:test]
          options[:runtest][:test] = 'run_test.pl'
        elsif File.file?(options[:runtest][:test])
          unless File.basename(options[:runtest][:test]) == options[:runtest][:test]
            options[:runtest][:runopts][:chdir] = File.dirname(options[:runtest][:test])
            options[:runtest][:test] = File.basename(options[:runtest][:test])
          end
        elsif File.directory?(File.dirname(options[:runtest][:test]))
          options[:runtest][:runopts][:chdir] = File.dirname(options[:runtest][:test])
          options[:runtest][:test] = File.basename(options[:runtest][:test])
          # attempt to guess extension
          if _ext = TOOLS.keys.find { |ext| File.file?(File.join(options[:runtest][:runopts][:chdir], options[:runtest][:test] + ext)) }
            options[:runtest][:test] += _ext
          end
        end
        _rundir = options[:runtest][:runopts][:chdir] || '.'
        unless File.file?(File.join(_rundir, options[:runtest][:test]))
          log_error("Cannot find test #{File.join(_rundir, options[:runtest][:test])}")
          return false
        end
        test_argv = [options[:runtest][:test]]
        while !argv.empty? && argv.first != '--'
          test_argv << argv.shift
        end
        test_argv << options[:runtest][:runopts]
        tool = options[:runtest][:tool] || find_tool(options[:runtest][:test])
        rc = false
        if tool
          rc, exitcode = Exec.runcmd(tool, *test_argv)
          log_warning("Test '#{test_argv.join(' ')}' terminated with exitcode #{exitcode}") unless rc
        end
        rc
      end

      Command.register('run:test|script', DESC, TAOX11::RunTest)
    end
  end
end
