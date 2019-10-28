#--------------------------------------------------------------------
# @file    core.rb
# @author  Martin Corino
#
# @brief   Extensions for core Ruby classes
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------
Dir["#{File.dirname(__FILE__)}/core_ext/*.rb"].sort.each do |path|
  require "ridlbe/base/config/core_ext/#{File.basename(path, '.*')}"
end
