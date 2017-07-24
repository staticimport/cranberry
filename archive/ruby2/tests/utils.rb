
require 'test/unit'

module TestUtils
  def assert_token(token, text, type, linenum, linecol)
    assert_equal(type, token.type)
    assert_equal(text, token.text)
    assert_equal(linenum, token.linenum)
    assert_equal(linecol, token.linecol)
  end
end
