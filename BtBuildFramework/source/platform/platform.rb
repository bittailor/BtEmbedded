require 'singleton'

class Platform < Hash
  
  attr_reader :directory, :name
  
  def initialize(configuration)
    @configuration = configuration
    @directory = File.dirname(__FILE__)
    @name = @configuration[:platform]
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










