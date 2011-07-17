
require 'lib/lexer/token.rb'

module Cranberry
  class Lexer
    def initialize(input)
      @input = input
      @line = ""
    end

    def next
      if @line.length != 0
        if @line =~ /^(\d+\.\d+)/
          return match_made($1, :floating)
        elsif @line =~ /^(\d+)/
          return match_made($1, :integral)
        elsif @line =~ /^([\+\-\*\/])/
          return match_made($1, :operator)
        else
          raise "Unable to parse: #{@line}"
        end
      else
        @line = nextLine
        unless @line == nil
          @line =~ /(\s*)/
          return match_made($1, :indent)
        else
          return nil
        end
      end
    end

    def match_made(match, type)
      @line = @line[match.length...@line.length].strip
      Token.new(type, match)
    end

    def nextLine
      while true
        @line = @input.gets
        return @line if @line == nil or @line =~ /[^\s]/
      end
    end
  end
end

