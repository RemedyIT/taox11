#--------------------------------------------------------------------
# @file    concern.rb
# @author  Martin Corino
#
# @brief   Support for properly handling module dependencies
#          like ActiveSupport::Concern
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL
  module Base
  # A typical module looks like this:
  #
  #   module M
  #     def self.included(base)
  #       base.extend ClassMethods
  #       base.class_eval do
  #         scope :disabled, where(:disabled => true)
  #       end
  #     end
  #
  #     module ClassMethods
  #       ...
  #     end
  #   end
  #
  # By using <tt>Base::Concern</tt> the above module could instead be written as:
  #
  #   require 'ridlbe/base/config/concern'
  #
  #   module M
  #     extend IDL::Base::Concern
  #
  #     included do
  #       scope :disabled, where(:disabled => true)
  #     end
  #
  #     module ClassMethods
  #       ...
  #     end
  #   end
  #
  # Moreover, it gracefully handles module dependencies. Given a +Foo+ module and a +Bar+
  # module which depends on the former, we would typically write the following:
  #
  #   module Foo
  #     def self.included(base)
  #       base.class_eval do
  #         def self.method_injected_by_foo
  #           ...
  #         end
  #       end
  #     end
  #   end
  #
  #   module Bar
  #     def self.included(base)
  #       base.method_injected_by_foo
  #     end
  #   end
  #
  #   class Host
  #     include Foo # We need to include this dependency for Bar
  #     include Bar # Bar is the module that Host really needs
  #   end
  #
  # But why should +Host+ care about +Bar+'s dependencies, namely +Foo+? We could try to hide
  # these from +Host+ directly including +Foo+ in +Bar+:
  #
  #   module Bar
  #     include Foo
  #     def self.included(base)
  #       base.method_injected_by_foo
  #     end
  #   end
  #
  #   class Host
  #     include Bar
  #   end
  #
  # Unfortunately this won't work, since when +Foo+ is included, its <tt>base</tt> is the +Bar+ module,
  # not the +Host+ class. With <tt>ActiveSupport::Concern</tt>, module dependencies are properly resolved:
  #
  #   require 'ridlbe/base/config/concern'
  #
  #   module Foo
  #     extend IDL::Base::Concern
  #     included do
  #       class_eval do
  #         def self.method_injected_by_foo
  #           ...
  #         end
  #       end
  #     end
  #   end
  #
  #   module Bar
  #     extend IDL::Base::Concern
  #     include Foo
  #
  #     included do
  #       self.method_injected_by_foo
  #     end
  #   end
  #
  #   class Host
  #     include Bar # works, Bar takes care now of its dependencies
  #   end
  #
    module Concern
      def self.extended(base)
        base.instance_variable_set("@_dependencies", [])
      end

      def append_features(base)
        if base.instance_variable_defined?("@_dependencies")
          base.instance_variable_get("@_dependencies") << self
          return false
        else
          return false if base < self
          @_dependencies.each { |dep| base.send(:include, dep) }
          super
          base.extend const_get("ClassMethods") if const_defined?("ClassMethods")
          base.class_eval(&@_included_block) if instance_variable_defined?("@_included_block")
        end
      end

      def included(base = nil, &block)
        if base.nil?
          @_included_block = block
        else
          super
        end
      end
    end

  end # Base

end # IDL
