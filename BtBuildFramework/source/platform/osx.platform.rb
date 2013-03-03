require 'builder'



Platform.instance["OSX"] = 
{
  :builder => Builder.new(File.expand_path(File.join(Platform.instance.directory,"osx"))) 
    
} 