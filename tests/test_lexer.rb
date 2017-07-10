
require './lib/lexer/lexer.rb'
require 'test/unit'

class TestLexer < Test::Unit::TestCase
  include Cranberry

  # TESTS
  def test_numbers
    File.open("tests/resources/test_lexer_numbers.txt") do |file|
      lexer = Lexer.new(file)
      expect_next(lexer, '1', :integral, 1, 1)
      expect_next(lexer, '89', :integral, 2, 2)
      expect_next(lexer, '1.982', :floating, 3, 3)
      expect_next(lexer, '0.1275828', :floating, 4, 2)
      expect_next(lexer, '1e9', :integral, 5, 1)
      expect_next(lexer, '94e-7', :floating, 6, 3)
      expect_next(lexer, '0xDEADBEEF', :integral, 7, 2)
      expect_next(lexer, '0x9bac04', :integral, 8, 1)
      expect_next(lexer, '1.2e4', :floating, 9, 3)
      assert(nil == lexer.next())
    end
  end

  # HELPERS
  def expect_next(lexer, text, type, linenum, linecol)
    token = lexer.next()
    assert(token.type == type)
    assert(token.text == text)
    assert($CURRENT_LINENUM == linenum)
    assert($CURRENT_LINECOL == linecol)
  end
end

