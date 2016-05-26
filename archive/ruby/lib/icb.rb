
require 'lib/lexer/lexer.rb'

module Cranberry
  module ICB
    QUIT = ":q"
    def ICB.run(params)
      while true
        print "icb> "
        line = gets.chomp
        if line == QUIT
          break
        else
          puts "You entered: #{line}"
          lexer = Lexer.new(OneLiner.new(line))
          while (token = lexer.next)
            puts token
          end
        end
      end
    end   
  end

  class OneLiner
    def initialize(line)
      @line = line
    end
    
    def gets
      old_line = @line
      @line = nil
      old_line
    end
  end
end

