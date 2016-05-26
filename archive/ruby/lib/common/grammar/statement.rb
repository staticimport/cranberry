
module Cranberry
  class Statement
    attr_accessor :expression
    def initialize(expression)
      @expression = expression
    end

    def to_cpp
      "#{@expression.to_cpp};"
    end

    def type
      @expression.type
    end
  end
end

