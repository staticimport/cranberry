#require "./lib/NAME.rb"
require "test/unit"

class TestFoo < Test::Unit::TestCase

  def test_sample
    assert_equal(4, 2+2)
  end

end
