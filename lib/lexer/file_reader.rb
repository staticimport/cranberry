
module Cranberry
  class FileReader
    def initialize(file)
      @file = file
      @line = ""
      @index = 0
    end
    def peek
      while @index == @line.length
        @index = 0
        @line = @file.gets
        if @line == nil
          return nil
        end
      end
      @line[@index..@index]
    end
    def step
      @index += 1
    end
  end
end

