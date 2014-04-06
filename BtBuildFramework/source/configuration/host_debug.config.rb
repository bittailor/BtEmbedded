

class Configuration
  
  protected
  
  def setup()
       
    if is_osx?
      self[:platform] = "osx";
    elsif is_linux?
      self[:platform] = "linux";
    else
      throw "unsupported host"
    end   
      
    self[:is_host] = true;
    self[:test_filter] = "-*IntegrationTest.*";
      
  end
  
  
  def is_osx?
    RUBY_PLATFORM.downcase.include?("darwin")
  end
   
  def is_linux?
    RUBY_PLATFORM.downcase.include?("linux")
  end
  
end
