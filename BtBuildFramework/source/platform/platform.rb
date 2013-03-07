require 'singleton'

class Platform < Hash
  
  attr_reader :directory
  
  def initialize(name)
    @directory = File.dirname(__FILE__)
    @name = name
    @file = File.join(@directory,"#{@name}.platform.rb")
    
    load()
    setup()
    
  end
  
  protected
  
  def load()
    raise "platform #{@name} not found at #{@file}" unless File.exist?(@file)
    puts "load platform #{@name} from #{@file}" if Rake.application.options.trace
    Kernel.load @file
  end
   
end










