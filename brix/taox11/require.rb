#--------------------------------------------------------------------
# @file    require.rb
# @author  Martin Corino
#
# @brief   BRIX11 TAOX11 brix collection loader
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module BRIX11
  module TAOX11
    ROOT = File.dirname(__FILE__)
    TITLE = 'TAOX11'.freeze
    DESC = 'BRIX11 TAOX11 brix collection'.freeze
    COPYRIGHT = "Copyright (c) 2014-#{Time.now.year} Remedy IT Expertise BV, The Netherlands".freeze
    VERSION = { major: 2, minor: 5, release: 0 }

    Collection.configure(:taox11, ROOT, TITLE, DESC, COPYRIGHT, VERSION) do |cfg|
      cfg.on_setup do |optparser, options|
        # define common environment for spawning BRIX11 subprocesses

        # make sure to copy the contents of the global search path variables irrespective of '-E' switch
        # unless the (user) config already defined a value
        Exec.update_run_environment('PATH', ENV['PATH']) unless Exec.has_run_environment?('PATH')
        unless Exec.mswin?
          Exec.update_run_environment('LD_LIBRARY_PATH', ENV['LD_LIBRARY_PATH']) unless Exec.has_run_environment?('LD_LIBRARY_PATH')
        end

        # base
        base_root = Exec.get_run_environment('X11_BASE_ROOT')

        # standard
        taox11_root = Exec.get_run_environment('TAOX11_ROOT')
        taox11_root ||= Exec.update_run_environment('TAOX11_ROOT', File.expand_path(File.join(ROOT, '..', '..')))
        ace_root = Exec.get_run_environment('ACE_ROOT')
        unless ace_root
          # in case of alternative dir layout (for Github CI builds)
          ace_root = Exec.update_run_environment('ACE_ROOT', File.join(taox11_root, 'ACE', 'ACE')) if File.directory?(File.join(taox11_root, 'ACE', 'ACE'))
          # regular layout
          ace_root ||= Exec.update_run_environment('ACE_ROOT', File.join(base_root, 'ACE', 'ACE'))
        end
        Exec.update_run_environment('MPC_ROOT', File.expand_path(File.join(ace_root, '..', 'MPC'))) unless Exec.has_run_environment?('MPC_ROOT')
        Exec.update_run_environment('MPC_BASE', File.join(taox11_root, 'bin', 'MPC')) unless Exec.has_run_environment?('MPC_BASE')
        Exec.update_run_environment('TAO_ROOT', File.expand_path(File.join(ace_root, '..', 'TAO'))) unless Exec.has_run_environment?('TAO_ROOT')

        # should we prepare for crossbuilds?
        if BRIX11.options.config.crossbuild
          x11_host_root = Exec.get_run_environment('X11_HOST_ROOT', true)
          BRIX11.log_fatal('Crossbuild configuration REQUIRES a X11_HOST_ROOT variable defined!') unless x11_host_root
          Exec.update_run_environment('X11_HOST_ROOT', x11_host_root)
          # define HOST_ROOT for possible crossbuild tao_idl invocations (requires reguler layout)
          Exec.update_run_environment('HOST_ROOT', File.join(x11_host_root, 'ACE', 'ACE')) unless Exec.has_run_environment?('HOST_ROOT')
          # and update library search paths for host (tool) libs
          if Exec.mswin?
            Exec.update_run_environment('PATH', [File.join(x11_host_root, 'lib'), File.join(Exec.get_run_environment('HOST_ROOT'), 'lib')], :prepend)
          else
            Exec.update_run_environment('LD_LIBRARY_PATH', [File.join(x11_host_root, 'lib'), File.join(Exec.get_run_environment('HOST_ROOT'), 'lib')], :prepend)
          end
        else # or regular build environment
          # and update library search paths for (tool) libs
          if Exec.mswin?
            Exec.update_run_environment('PATH', [File.join(base_root, 'lib'), File.join(ace_root, 'lib')], :prepend)
          else
            Exec.update_run_environment('LD_LIBRARY_PATH', [File.join(base_root, 'lib'), File.join(ace_root, 'lib')], :prepend)
          end
        end

        # ridl
        Exec.update_run_environment('RIDL_BE_SELECT', 'c++11') unless Exec.has_run_environment?('RIDL_BE_SELECT')
        ridl_be_path = Exec.get_run_environment('RIDL_BE_PATH')
        # if not set yet update
        ridl_be_path ||= Exec.update_run_environment(
                              'RIDL_BE_PATH',
                              taox11_root,
                              :append)
        ridl_be_path.split(/:|;/).each { |p| $: << p unless $:.include?(p) }
        # update executable search path for MPC scripts (mwc.pl/mpc.pl)
        Exec.update_run_environment('PATH', File.join(taox11_root, 'bin'), :prepend)
        Project.mpc_path = File.join(taox11_root, 'bin')
        # update executable search path for ridlc script
        Exec.update_run_environment('PATH', File.join(base_root, 'bin'), :prepend)

        # load collection
        Dir.glob(File.join(ROOT, 'lib', '*.rb')).each { |p| require "brix/taox11/lib/#{File.basename(p)}" }
        Dir.glob(File.join(ROOT, 'cmds', '*.rb')).each { |p| require "brix/taox11/cmds/#{File.basename(p)}" }

        # update documentation config
        Common::GenerateDocumentation::OPTIONS[:docsources]['taox11'] = ['${TAOX11_ROOT}/docs']
        Common::GenerateDocumentation::OPTIONS[:adoc_attribs]['taox11_src_root'] = Exec.get_run_environment('TAOX11_ROOT')

        Common::GenerateDocumentation::OPTIONS[:doxygen_config] = '${TAOX11_ROOT}/etc/taox11.doxygen'
      end
    end
  end
end
