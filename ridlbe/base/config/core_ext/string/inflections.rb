#--------------------------------------------------------------------
# @file    inflections.rb
# @author  Martin Corino
#
# @brief   ActiveSupport style inflection methods for String
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module RIDL
  module CoreExt
    module String
      ACRONYMS = %w{CORBA CCM DDS IDL}

      @@acronyms = ACRONYMS.inject({}) {|h, a| h[a.downcase] = a; h}
      @@acronyms_regex = /#{@@acronyms.values.join('|')}/

      def self.acronyms_regex(reset = false)
        @@acronyms_regex = nil if reset
        @@acronyms_regex ||= /#{@@acronyms.values.join('|')}/
      end

      def self.acronym(word)
        @@acronyms[word.downcase] = word
        acronyms_regex(true)
      end

      def self.acronyms
        @@acronyms
      end

      # Converts a string containing an IDL or programming language type expression into an identifier string.
      #
      # +identify+ strips whitespace and replaces several tokens ('::', '<>' brackets, punctuations, separators etc.)
      # by underscores ('_').
      #
      #   "MyObject::Exception".identifier        # => 'MyObject_Exception'
      #   "CORBA::object_reference < MyObject >"  # => 'CORBA_object_reference_MyObject_'
      #
      def identify
        self.dup.identify!
      end

      def identify!
        self.gsub!(/\s/, '')
        self.gsub!(/::|<|>|,|\./, '_')
        self
      end

      # Makes an underscored, lowercase form of the string.
      #
      # Changes '::' to '_' to convert namespaces.
      #
      # Examples:
      #   "InterfaceVisitor".underscore         # => "interface_visitor"
      #   "Base::InterfaceVisitor".underscore   # => "base_interface_visitor"
      #
      def underscore
        self.dup.underscore!
      end

      def underscore!
        self.gsub!(/::/, '_')
        self.gsub!(/(?:([A-Za-z\d])|^)(#{RIDL::CoreExt::String.acronyms_regex})(?=\b|[^a-z])/) { "#{::Regexp.last_match(1)}#{::Regexp.last_match(1) && '_'}#{::Regexp.last_match(2).downcase}" }
        self.gsub!(/([A-Z\d]+)([A-Z][a-z])/, '\1_\2')
        self.gsub!(/([a-z\d])([A-Z])/, '\1_\2')
        self.tr!('-', '_')
        self.downcase!
        self
      end

      # By default, +camelize+ converts strings to UpperCamelCase. If the argument
      # to +camelize+ is set to <tt>:lower</tt> then +camelize+ produces
      # lowerCamelCase.
      #
      # +camelize+ will also convert '/' to '::' which is useful for converting
      # paths to namespaces. Invalid identifier characters like (not in [a-zA-Z0-9_])
      # are replaced by '_'
      #
      # +camelize+ (like +underscore+) also takes a list of (configurable) acronyms
      # into account which include by default the terms 'CORBA', 'DDS', 'IDL', 'CCM'
      #
      #   'foo_traits'.camelize                     # => "FooTraits"
      #   'dds_data_writer'.camelize(:lower)        # => "ddsDataWriter"
      #   'test/my-foo'.camelize                    # => "Test::MyFoo"
      #
      def camelize(uppercase_first_letter = true)
        self.dup.camelize!
      end

      def camelize!(uppercase_first_letter = true)
        self.gsub!(/[^a-zA-Z0-9_]/, '_')
        if uppercase_first_letter
          self.sub!(/^[a-z\d]*/) { RIDL::CoreExt::String.acronyms[::Regexp.last_match(0)] || ::Regexp.last_match(0).capitalize }
        else
          self.sub!(/^(?:#{RIDL::CoreExt::String.acronyms_regex}(?=\b|[A-Z_])|\w)/) { ::Regexp.last_match(0).downcase }
        end
        self.gsub!(/(?:_|(\/))([a-z\d]*)/i) { "#{::Regexp.last_match(1)}#{RIDL::CoreExt::String.acronyms[::Regexp.last_match(2)] || ::Regexp.last_match(2).capitalize}" }
        self.gsub!('/', '::')
        self
      end

      # Removes the module part from the expression in the string:
      #
      #   "IDL::Base::InterfaceVisitor".demodulize  # => "InterfaceVisitor"
      #   "InterfaceVisitor".demodulize             # => "InterfaceVisitor"
      #
      def demodulize
        self.dup.demodulize!
      end

      def demodulize!
        if i = self.rindex('::')
          self.slice!(0, i+2)
        end
        self
      end
    end
  end
end

::String.__send__(:include, RIDL::CoreExt::String)
