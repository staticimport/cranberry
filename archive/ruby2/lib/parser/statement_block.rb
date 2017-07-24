
require './lib/lexer/lexer.rb'
require './lib/lexer/token.rb'

module Cranberry
  class StatementBlock
    def initialize(statements)
      @statements = statements
    end
    def self.parse(lexer)
      statements = []
      while lexer.peek().text != 'end'
        # TODO statements.append(Statement.parse(lexer))
      end
      return StatementBlock.new(statements)
    end
  end
end
