#--------------------------------------------------------------------
# @file    require.rb
# @author  Martin Corino
#
# @brief   IDL language mapping loader
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

# Try to load simplecov
begin
  require 'simplecov'
  SimpleCov.start
rescue LoadError
end

# base modules
require 'ridlbe/c++11/config'
require 'ridlbe/c++11/visitorbase'

# visitors
Dir.glob(File.join(File.dirname(__FILE__), 'visitors', '*.rb')).each do |f|
  require "ridlbe/c++11/visitors/#{File.basename(f, '.*')}"
end

# writers
Dir.glob(File.join(File.dirname(__FILE__), 'writers', '*.rb')).each do |f|
  require "ridlbe/c++11/writers/#{File.basename(f, '.*')}"
end

