
require './lib/lexer/lexer.rb'
require './lib/lexer/token.rb'

module Cranberry
  class Parser
    def initialize
      @functions = []
    end
    def parse(lexer)
      token = lexer.peek()
      if token.text == 'fcn'
        @functions.push(Function.parse(lexer))
      end
    end
  end
end
