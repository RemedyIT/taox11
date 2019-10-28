#--------------------------------------------------------------------
# @file    helpers.rb
# @author  Martin Corino
#
# @brief   Support for Rails like helper methods/modules.
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
require 'ridlbe/base/config/concern'

module IDL
  module Base
    module Helpers
      extend Base::Concern

      included do
        class_attribute :_helpers
        self._helpers = Module.new

        class_attribute :_helper_methods
        self._helper_methods = Array.new
      end

      module ClassMethods
        # When a class is inherited, wrap its helper module in a new module.
        # This ensures that the parent class's module can be changed
        # independently of the child class's.
        def inherited(klass)
          helpers = _helpers
          klass._helpers = Module.new { include helpers }
          super
        end

        # Declare a writer method as a template helper. For example, the following
        # makes the +current_idl_file+ writer method available to the templates:
        #   class Writer < IDL::Base::Writer
        #     helper_method :current_idl_file
        #
        #     def current_idl_file
        #       @params[:idlfile]
        #     end
        #   end
        #
        # In a template:
        #  // Generated from <%= current_idl_file %>
        #
        # ==== Parameters
        # * <tt>method[, method]</tt> - A name or names of a method on the writer
        #   to be made available in the template.
        def helper_method(*meths)
          meths.flatten!
          self._helper_methods += meths

          meths.each do |meth|
            _helpers.class_eval <<-ruby_eval, __FILE__, __LINE__ + 1
              def #{meth}(*args, &blk)
                writer.send(%(#{meth}), *args, &blk)
              end
            ruby_eval
          end
        end

        # The +helper+ class method can take a series of helper module names, a block, or both.
        #
        # ==== Parameters
        # * <tt>*args</tt> - Module
        # * <tt>block</tt> - A block defining helper methods
        #
        # ==== Examples
        # When the argument is a module it will be included directly in the template class.
        #   helper FooHelper # => includes FooHelper
        #
        # Additionally, the +helper+ class method can receive and evaluate a block, making the methods defined available
        # to the template.
        #
        #   # One line
        #   helper { def hello() "Hello, world!" end }
        #
        #   # Multi-line
        #   helper do
        #     def foo(bar)
        #       "#{bar} is the very best"
        #     end
        #   end
        #
        # Finally, the above styles can be mixed together, and the +helper+ method can be invoked with a combination of
        # +modules+ and a block.
        #
        #   helper(BlindHelper) { def mice() 'mice' end }
        #
        def helper(*args, &block)
          modules_for_helpers(args).each do |mod|
            add_template_helper(mod)
          end

          _helpers.module_eval(&block) if block_given?
        end

        # Clears up all existing helpers in this class, only keeping the helper
        # with the same name as this class.
        def clear_helpers
          inherited_helper_methods = _helper_methods
          self._helpers = Module.new
          self._helper_methods = Array.new

          inherited_helper_methods.each { |meth| helper_method meth }
        end

        # Returns a list of modules, normalized from the acceptable kinds of
        # helpers.
        #
        # ==== Parameters
        # * <tt>args</tt> - An array of helpers
        #
        # ==== Returns
        # * <tt>Array</tt> - A normalized list of modules for the list of
        #   helpers provided.
        def modules_for_helpers(args)
          args.flatten.map! do |arg|
            case arg
              when Module
                arg
              else
                raise ArgumentError, "helper must be a Module"
            end
          end
        end

        private
        # Makes all the (instance) methods in the helper module available to templates
        # rendered through this writer.
        #
        # ==== Parameters
        # * <tt>module</tt> - The module to include into the current helper module
        #   for the class
        def add_template_helper(mod)
          _helpers.module_eval { include mod } unless _helpers.included_modules.include?(mod)
        end
      end
    end # Helpers
  end # Base
end # IDL