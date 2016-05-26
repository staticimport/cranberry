
module Cranberry
  class Token
    attr_reader :type, :text
    def initialize(type, text)
      @type = type
      @text = text
    end

    def to_s
      "Token(type=#{type}, text=#{text})"
    end
  end
end

