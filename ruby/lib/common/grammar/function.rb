
module Cranberry
  class Function
    attr_accessor :name, :args, :block
    def initialize(name, args)
      @name = name
      @args = args
      @block = StatementBlock.new
    end

    def to_cpp
      "#{type} #{name}(" +
        "#{args.map { |a| "#{a.type} #{a.name}" }.join(',')})\n" +
        @block.to_cpp  
    end

    def type
      @block.type
    end
  end
end

