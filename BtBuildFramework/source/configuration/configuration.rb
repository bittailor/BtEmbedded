require 'singleton'

class Configuration < Hash
  include Singleton
  
  def initialize()
    @directory = File.dirname(__FILE__)
    @pattern = "*.config.rb"
  end
  
  def load()
    Dir[File.join(@directory,@pattern)].each do |configuration|
      puts "load configuration #{configuration}" if Rake.application.options.trace
      Kernel.load configuration
    end
  end
  
end

Configuration.instance.load()








