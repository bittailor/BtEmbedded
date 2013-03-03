require 'singleton'

class Platform < Hash
  include Singleton
  
  attr_reader :directory
  
  def initialize()
    @directory = File.dirname(__FILE__)
    @pattern = "*.platform.rb"
  end
  
  def load()
    Dir[File.join(@directory,@pattern)].each do |platform|
      puts "load platform #{platform}" if Rake.application.options.trace
      Kernel.load platform
    end
  end
   
end

Platform.instance.load()









