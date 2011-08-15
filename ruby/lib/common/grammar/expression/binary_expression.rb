
require 'lib/common/grammar/expression.rb'

module Cranberry
  class BinaryExpression < Expression
    attr_accessor :left, :right, :operator
    def initialize(type, left, right, operator)
      super(type)
      @left = left
      @right = right
      @operator = operator
    end

    def to_cpp
      "#{@left.to_cpp} #{@operator} #{@right.to_cpp}"
    end
  end

  class AddExpression < BinaryExpression
    def initialize(left, right)
      super(:add_expression, left, right '+')
    end
  end
  
  class SubtractExpression < BinaryExpression
    def initialize(left, right)
      super(:subtract_expression, left, right '-')
    end
  end

  class MultiplyExpression < BinaryExpression
    def initialize(left, right)
      super(:multiply_expression, left, right '*')
    end
  end

  class DivideExpression < BinaryExpression
    def initialize(left, right)
      super(:divide_expression, left, right, '/')
    end
  end

  class AndExpression < BinaryExpression
    def initialize(left, right)
      super(:and_expression, left, right, 'and')
    end
  end

  class OrExpression < BinaryExpression
    def initialize(left, right)
      super(:or_expression, left, right, 'or')
    end
  end

  class XorExpression < BinaryExpression
    def initialize(left, right)
      super(:xor_expression, left, right, 'xor')
    end
  end
end

