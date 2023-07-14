#--------------------------------------------------------------------
# @file    expression.rb
# @author  Martin Corino
#
# @brief   C++ extensions to ExpressionConverter mixin
#
# @copyright Copyright (c) Remedy IT Expertise BV
#--------------------------------------------------------------------

module IDL
  module Base
    # reopen and extend module
    module ExpressionConverter
      def expression_to_s(exp, scope = nil)
        case exp
        when Expression::Value
          value_to_s(exp)
        when Expression::Operation
          operation_to_s(exp)
        when Expression::ScopedName
          (scope && (scope == exp.node.enclosure)) ? exp.node.cxxname : '::' + exp.node.scoped_cxxname
        when Expression::Enumerator
          ((scope && (scope == exp.node.enclosure)) ? exp.node.enum.cxxname : '::' + exp.node.enum.scoped_cxxname) + '::' + exp.node.cxxname
        else
          raise "unknown expression type: #{exp.class.name}"
        end
      end

      def operation_to_s(exp, scope = nil)
        s = nil
        op = exp.operands
        case exp
        when Expression::Operation::UnaryPlus
          s = expression_to_s(op[0], scope)
        when Expression::Operation::UnaryMinus
          s = '-' + expression_to_s(op[0], scope)
        when Expression::Operation::UnaryNot
          s = '~' + expression_to_s(op[0], scope)
        when Expression::Operation::Xor
          s = expression_to_s(op[0], scope) + ' ^ ' + expression_to_s(op[1], scope)
        when Expression::Operation::Or
          s = expression_to_s(op[0], scope) + ' | ' + expression_to_s(op[1], scope)
        when Expression::Operation::And
          s = expression_to_s(op[0], scope) + ' & ' + expression_to_s(op[1], scope)
        when Expression::Operation::LShift
          s = expression_to_s(op[0], scope) + ' << ' + expression_to_s(op[1], scope)
        when Expression::Operation::RShift
          s = expression_to_s(op[0], scope) + ' >> ' + expression_to_s(op[1], scope)
        when Expression::Operation::Add
          s = expression_to_s(op[0], scope) + ' + ' + expression_to_s(op[1], scope)
        when Expression::Operation::Minus
          s = expression_to_s(op[0], scope) + ' - ' + expression_to_s(op[1], scope)
        when Expression::Operation::Mult
          s = expression_to_s(op[0], scope) + ' * ' + expression_to_s(op[1], scope)
        when Expression::Operation::Div
          s = expression_to_s(op[0], scope) + ' / ' + expression_to_s(op[1], scope)
        when Expression::Operation::Mod
          s = expression_to_s(op[0], scope) + ' % ' + expression_to_s(op[1], scope)
        else
          raise "unknown operation: #{exp.type.name}"
        end
        '(' + s + ')'
      end
    end
  end
end
