
require './lib/lexer/lexer.rb'
require './lib/lexer/token.rb'

module Cranberry
  class Argument
    attr_reader :name, :type
    def initialize(name, type)
      @name = name
      @type = type
    end
  end
  end
  class Function
    def initialize(name, args, return_type, statement_block)
      @name = name
      @args = args
      @return_type = return_type
      @statement_block = statement_block
    end
    def self.parse(lexer)
      ParserUtils.pop_specific(lexer, 'fcn', 'function declaration')
      name = ParserUtils.pop_identifier(lexer, 'function name')
      ParserUtils.pop_specific(lexer, '(', 'function arg list')
      args = []
      until lexer.peek().text() == ')'
        arg_name = ParserUtils.pop_identifier(lexer, "argument name")
        ParserUtils.pop_specific(lexer, ':', 'function arg')
        arg_type = ParserUtils.pop_type(lexer)
        args.append(Argument.new(arg_name, arg_type))
      end
      ParserUtils.pop_specific(lexer, ':', 'function return type')
      return_type = ParserUtils.pop_type(lexer)
      statement_block = StatementBlock.parse(lexer)
      return Function.new(name, args, return_type, statement_block)
    end
  end
end
