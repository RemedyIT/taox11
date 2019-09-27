#--------------------------------------------------------------------
# @file    core.rb
# @author  Martin Corino
#
# @brief   Extensions for core Ruby classes
#
# @copyright Copyright (c) Remedy IT Expertise BV
# Chamber of commerce Rotterdam nr.276339, The Netherlands
#--------------------------------------------------------------------
Dir["#{File.dirname(__FILE__)}/core_ext/*.rb"].sort.each do |path|
  require "ridlbe/c++11/config/core_ext/#{File.basename(path, '.*')}"
end
