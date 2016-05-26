
module Cranberry
  class StatementBlock
    attr_accessor :statements
    def initialize
      @statements = []
    end

    def to_cpp
      "{\n#{@statements.map{|s| s.to_cpp}.join('\n')}\n}\n" 
    end

    def type
      if @statements.empty?
        'void'
      else
        @statements.last.type
      end
    end
  end
end

