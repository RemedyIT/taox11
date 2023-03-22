#--------------------------------------------------------------------
# @file    prepend_and_append.rb
# @author  Martin Corino
#
# @brief   ActiveSupport style utility methods
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

class Array
  # The human way of thinking about adding stuff to the end of a list is with append
  alias_method :append,  :<<

  # The human way of thinking about adding stuff to the beginning of a list is with prepend
  alias_method :prepend, :unshift
end
