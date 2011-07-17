
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
        end
      end
    end   
  end
end

