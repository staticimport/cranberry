
require './lib/lexer/token.rb'

module Cranberry
  module ParseUtils
    def self.pop_identifier(lexer, context)
      token = lexer.pop()
      raise "expected identifier but found #{token} in #{context}" unless token.type == :identifier
      return token
    end
    def self.pop_specific(lexer, text, context)
      token = lexer.pop()
      raise "expected '#{text}' but found #{token} in #{context}" unless token.text == text
      return token
    end
    def self.pop_type(lexer, context)
      token = lexer.pop()
      next_token = lexer.peek()
      if next_token and next_token.text == '{'
        open_brace_count = 1
        token.append(lexer.pop())
        while open_brace_count > 0
          next_token = lexer.pop()
          token.append(next_token)
          if next_token.text == '{'
            open_brace_count += 1
          elsif next_token.text == '}'
            open_brace_count -= 1
          end
        end
      end
      return token
    end
  end

end
