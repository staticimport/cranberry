
require './lib/globals.rb'
require './lib/lexer/token.rb'

module Cranberry
  class Lexer
    def initialize(input)
      @input = input
      @line = ""
      @prev_match_len = 0
      $CURRENT_LINENUM = 0
      $CURRENT_LINECOL = 0
    end

    def next
      $CURRENT_LINECOL += @prev_match_len
      @prev_match_len = 0
      while lstrip_line!().length == 0
        @line = take_next_line()
        return nil if @line == nil
      end
      if @line =~ /^(\d+\.\d+e-?\d+)/ or @line =~ /^(\d+e-\d+)/ or +@line =~ /^(\d+\.\d+)/
        return match_made($1, :floating)
      elsif @line =~ /^(\d+e\d+)/ or @line =~ /^(0x[A-Fa-f0-9]+)/ or @line =~ /^(\d+)/
        return match_made($1, :integral)
      elsif @line =~ OPERATORS_REGEXP
        return match_made($1, :operator)
      elsif @line =~ /^(@{0,2}[A-Za-z_][A-Za-z_0-9]*\??)/
        match = $1
        if KEYWORDS.include?(match)
          return match_made(match, :keyword)
        else
          return match_made(match, :identifier)
        end
      elsif @line =~ /^("[^"]*")/ # TODO: much too simple
        return match_made($1, :string)
      else
        raise "Unable to parse: '#{@line}'"
      end
    end

    def match_made(match, type)
      @line = @line[match.length...@line.length].strip
      @prev_match_len = match.length
      Token.new(type, match)
    end

    def take_next_line
      while true
        @line = @input.gets
        $CURRENT_LINE     = @line
        $CURRENT_LINENUM += 1
        $CURRENT_LINECOL  = 1
        return @line if @line == nil or @line =~ /[^\s]/
      end
    end

    def lstrip_line!()
      new_line = @line.lstrip()
      $CURRENT_LINECOL += (@line.length - new_line.length)
      @line = new_line
      return @line 
    end
  end
end

