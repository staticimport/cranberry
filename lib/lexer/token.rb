
module Cranberry
  class Token
    attr_reader :type, :text, :linenum, :linecol
    def initialize(type, text, linenum, linecol)
      @type = type
      @text = text
      @linenum = linenum
      @linecol = linecol
    end
    def append(t)
      @type = :complex
      @text += t.text
    end
    def to_s
      "Token(type='#{@type}' text='#{@text}' loc=#{@linenum}:#{@linecol})"
    end
  end
end

