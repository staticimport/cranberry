
require './lib/lexer/lexer.rb'
require './tests/utils.rb'
require 'test/unit'

class TestLexer < Test::Unit::TestCase
  include Cranberry, TestUtils

  # TESTS
  def test_numbers
    File.open("tests/resources/test_lexer_numbers.txt") do |file|
      lexer = Lexer.new(file)
      assert_token(lexer.pop(), '1', :integral, 1, 1)
      assert_token(lexer.pop(), '89', :integral, 2, 2)
      assert_token(lexer.pop(), '1.982', :floating, 3, 3)
      assert_token(lexer.pop(), '0.1275828', :floating, 4, 2)
      assert_token(lexer.pop(), '1e9', :integral, 5, 1)
      assert_token(lexer.pop(), '94e-7', :floating, 6, 3)
      assert_token(lexer.pop(), '0xDEADBEEF', :integral, 7, 2)
      assert_token(lexer.pop(), '0x9bac04', :integral, 8, 1)
      assert_token(lexer.pop(), '1.2e4', :floating, 9, 3)
      assert_equal(nil, lexer.pop())
    end
  end

  def test_operators
    lexer = Lexer.new(StringIO.new(OPERATORS.join(' ')))
    next_col = 1
    (0...OPERATORS.length).each do |idx|
      op = OPERATORS[idx]
      assert_token(lexer.pop(), op, :operator, 1, next_col)
      next_col += op.length + 1 # space
    end
    assert_equal(nil, lexer.pop())
  end

  def test_keywords
    lexer = Lexer.new(StringIO.new(KEYWORDS.join("\t")))
    next_col = 1
    (0...KEYWORDS.length).each do |idx|
      keyword = KEYWORDS[idx]
      assert_token(lexer.pop(), keyword, :keyword, 1, next_col)
      next_col += keyword.length + 1 # tab
    end
    assert_equal(nil, lexer.pop())
  end

  def test_identifiers
    File.open("tests/resources/test_lexer_identifiers.txt") do |file|
      lexer = Lexer.new(file)
      assert_token(lexer.pop(), 'foo', :identifier, 1, 1)
      assert_token(lexer.pop(), 'Bar', :identifier, 2, 4)
      assert_token(lexer.pop(), 'bLaH', :identifier, 3, 2)
      assert_token(lexer.pop(), 'HERE', :identifier, 4, 1)
      assert_token(lexer.pop(), '@this_one', :identifier, 6, 2)
      assert_token(lexer.pop(), '$sup?', :identifier, 6, 12)
      assert_equal(nil, lexer.pop())
    end
  end

  def test_strings
    File.open("tests/resources/test_lexer_strings.txt") do |file|
      lexer = Lexer.new(file)
      assert_token(lexer.pop(), '""', :string, 1, 1)
      assert_token(lexer.pop(), "''", :string, 2, 3)
      assert_token(lexer.pop(), '"hi there"', :string, 3, 2)
      assert_token(lexer.pop(), "'what\"s\" Up?'", :string, 4, 1)
      assert_token(lexer.pop(), '"how \'about\' now?"', :string, 4, 15)
      assert_token(lexer.pop(), "'(\\'\"'", :string, 5, 2)
      assert_token(lexer.pop(), '"#\\"$"', :string, 6, 1)
    end
  end

  def test_retreat
    lexer = Lexer.new(StringIO.new('42'))
    assert_token(lexer.pop(), '42', :integral, 1, 1)
    lexer.retreat()
    assert_token(lexer.pop(), '42', :integral, 1, 1)
    assert_equal(nil, lexer.pop())
  end

  def test_peek
    lexer = Lexer.new(StringIO.new('42'))
    assert_equal('42', lexer.peek().text)
    assert_equal('42', lexer.peek().text)
    assert_equal('42', lexer.peek().text)
    assert_token(lexer.pop(), '42', :integral, 1, 1)
    assert_equal(nil, lexer.peek())
  end
end

