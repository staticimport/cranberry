
require 'lib/common/defs.rb'
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
        elsif @line =~ OPERATORS_REGEXP
          return match_made($1, :operator)
        elsif @line =~ /(@{0,2}[A-Za-z_][A-Za-z_0-9]*\??)/
          match = $1
          puts "Match: #{match}"
          if KEYWORDS.include?(match)
            return match_made(match, :keyword)
          else
            return match_made(match, :name)
          end
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

