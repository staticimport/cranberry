
require './lib/globals.rb'
require 'test/unit'

class TestGlobals < Test::Unit::TestCase
  include Cranberry
  def test_keywords_ascending
    prev = ''
    KEYWORDS_ASCENDING_LENGTH.each do |kw|
      assert(KEYWORDS.include? kw)
      assert(kw.length >= prev.length)
      prev = kw
    end
  end
  def test_keywords_descending
    prev = '................'
    KEYWORDS_DESCENDING_LENGTH.each do |kw|
      assert(KEYWORDS.include? kw)
      assert(kw.length <= prev.length)
      prev = kw
    end
  end
  def test_operators_ascending
    prev = ''
    assert(OPERATORS_ASCENDING_LENGTH.length == OPERATORS.length)
    OPERATORS_ASCENDING_LENGTH.each do |o|
      assert(OPERATORS.include? o)
      assert(o.length >= prev.length)
      prev = o
    end
  end
  def test_operators_descending
    prev = '................'
    assert(OPERATORS_DESCENDING_LENGTH.length == OPERATORS.length)
    OPERATORS_DESCENDING_LENGTH.each do |o|
      assert(OPERATORS.include? o)
      assert(o.length <= prev.length)
      prev = o 
    end
  end
  def test_keywords_regexp
    KEYWORDS.each do |kw|
      assert(kw =~ KEYWORDS_REGEXP)
      assert(nil == (kw =~ OPERATORS_REGEXP))
    end
  end
  def test_operators_regexp
    OPERATORS.each do |o|
      assert(o =~ OPERATORS_REGEXP)
      assert(nil == (o =~ KEYWORDS_REGEXP))
    end
  end
end

