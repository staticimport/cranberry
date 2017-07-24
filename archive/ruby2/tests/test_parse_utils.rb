
require './lib/lexer/lexer.rb'
require './lib/parser/parse_utils.rb'
require './tests/utils.rb'
require 'test/unit'

class TestLexer < Test::Unit::TestCase
  include Cranberry, TestUtils
  def test_pop_identifier
    lexer = Lexer.new(StringIO.new("foo int 3 bar"))
    assert_token(ParseUtils.pop_identifier(lexer, ""), 'foo', :identifier, 1, 1)
    assert_raise do ParseUtils.pop_identifier(lexer, "") end # int
    assert_raise do ParseUtils.pop_identifier(lexer, "") end # 3
    assert_token(ParseUtils.pop_identifier(lexer, ""), 'bar', :identifier, 1, 11)
  end
  def test_pop_specific
    lexer = Lexer.new(StringIO.new("foo int 3 8.76"))
    assert_token(ParseUtils.pop_specific(lexer, 'foo', ""), 'foo', :identifier, 1, 1)
    assert_raise do ParseUtils.pop_specific(lexer, 'Int', "") end # int
    assert_raise do ParseUtils.pop_specific(lexer, '34', "") end # 3
    assert_token(ParseUtils.pop_specific(lexer, '8.76', ""), '8.76', :floating, 1, 11)
  end
  def test_pop_type
    lexer = Lexer.new(StringIO.new("int FooBar arr{what} Map{Map{k1,  v1 }, what, ever}"))
    assert_token(ParseUtils.pop_type(lexer, ""), "int", :keyword, 1, 1)
    assert_token(ParseUtils.pop_type(lexer, ""), "FooBar", :identifier, 1, 5)
    assert_token(ParseUtils.pop_type(lexer, ""), "arr{what}", :complex, 1, 12)
    assert_token(ParseUtils.pop_type(lexer, ""), "Map{Map{k1,v1},what,ever}", :complex, 1, 22)
  end
end
