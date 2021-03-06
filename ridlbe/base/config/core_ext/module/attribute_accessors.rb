#--------------------------------------------------------------------
# @file    attribute_accessors.rb
# @author  Martin Corino
#
# @brief   ActiveSupport style utility methods
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

require 'ridlbe/base/config/core_ext/array/extract_options'

class Module
  def mattr_reader(*syms)
    options = syms.extract_options!
    syms.each do |sym|
      class_eval(<<-EOS, __FILE__, __LINE__ + 1)
        @@#{sym} = nil unless defined? @@#{sym}

        def self.#{sym}
          @@#{sym}
        end
      EOS

      unless options[:instance_reader] == false || options[:instance_accessor] == false
        class_eval(<<-EOS, __FILE__, __LINE__ + 1)
          def #{sym}
            @@#{sym}
          end
        EOS
      end
    end
  end

  def mattr_writer(*syms)
    options = syms.extract_options!
    syms.each do |sym|
      class_eval(<<-EOS, __FILE__, __LINE__ + 1)
        def self.#{sym}=(obj)
          @@#{sym} = obj
        end
      EOS

      unless options[:instance_writer] == false || options[:instance_accessor] == false
        class_eval(<<-EOS, __FILE__, __LINE__ + 1)
          def #{sym}=(obj)
            @@#{sym} = obj
          end
        EOS
      end
    end
  end

  # Extends the module object with module and instance accessors for class attributes,
  # just like the native attr* accessors for instance attributes.
  #
  #  module AppConfiguration
  #    mattr_accessor :google_api_key
  #    self.google_api_key = "123456789"
  #
  #    mattr_accessor :paypal_url
  #    self.paypal_url = "www.sandbox.paypal.com"
  #  end
  #
  #  AppConfiguration.google_api_key = "overriding the api key!"
  #
  # To opt out of the instance writer method, pass :instance_writer => false.
  # To opt out of the instance reader method, pass :instance_reader => false.
  # To opt out of both instance methods, pass :instance_accessor => false.
  def mattr_accessor(*syms)
    mattr_reader(*syms)
    mattr_writer(*syms)
  end
end
