
require './lib/globals.rb'
require './lib/lexer/token.rb'
require './lib/util/regexp_utils.rb'

module Cranberry
  class Lexer
    def initialize(input)
      @input = input
      @line = ""
      @prev_match = nil
      @retreats = []
      @linenum = 0
      @linecol = 0
      @regex_keyword_or_identifier = Regexp.new("^([@$]?[A-Za-z_][A-Za-z_0-9]*[\?]?)")
      @regex_operators = Regexp.new("^(#{Cranberry.escape_regexp_meta_chars_all(OPERATORS_DESCENDING_LENGTH).join('|')})")
      @regex_string1 = /^("(\\.|[^"])*")/
      @regex_string2 = /^('(\\.|[^'])*')/
    end

    def retreat
      raise "unable to retreat" unless @prev_match and @retreats.empty?
      @retreats.push(@prev_match)
    end

    def peek
      token = pop()
      retreat() if token
      return token
    end

    def pop
      return @retreats.pop unless @retreats.empty?
      @linecol += @prev_match.text.length if @prev_match
      @prev_match = nil
      while lstrip_line!().length == 0
        @line = take_next_line()
        return nil if @line == nil
      end
      if @line =~ /^(\d+\.\d+e-?\d+)/ or @line =~ /^(\d+e-\d+)/ or +@line =~ /^(\d+\.\d+)/
        return match_made($1, :floating)
      elsif @line =~ /^(\d+e\d+)/ or @line =~ /^(0x[A-Fa-f0-9]+)/ or @line =~ /^(\d+)/
        return match_made($1, :integral)
      elsif @regex_operators.match(@line)
        return match_made($1, :operator)
      elsif @regex_keyword_or_identifier.match(@line)
        match = $1
        if KEYWORDS.include?(match)
          return match_made(match, :keyword)
        else
          return match_made(match, :identifier)
        end
      elsif @regex_string1.match(@line) or @regex_string2.match(@line)
        return match_made($1, :string)
      else
        raise "Unable to parse: '#{@line}'"
      end
    end

    def match_made(match, type)
      @line = @line[match.length...@line.length]
      @prev_match = Token.new(type, match, @linenum, @linecol)
      return @prev_match
    end

    def take_next_line
      while true
        @line = @input.gets
        @linenum += 1
        @linecol  = 1
        return @line if @line == nil or @line =~ /[^\s]/
      end
    end

    def lstrip_line!()
      new_line = @line.lstrip()
      @linecol += (@line.length - new_line.length)
      @line = new_line
      return @line 
    end
  end
end

