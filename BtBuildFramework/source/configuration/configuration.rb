require 'singleton'

class Configuration < Hash
  
  attr_reader :name
  
  def initialize(name)
    
    @directory = File.dirname(__FILE__)
    @name = name
    @file = File.join(@directory,"#{@name}.config.rb")
  
    load()
    setup()    
  end
  
  protected
  
  def load()
    raise "configuration #{@name} not found at #{@file}" unless File.exist?(@file)
    puts "load configuration #{@name} from #{@file}" if Rake.application.options.trace
    Kernel.load @file
  end
  
end









