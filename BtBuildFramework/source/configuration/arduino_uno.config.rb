

class Configuration
  
  protected
  
  def setup()
    self[:is_host] = false;
    self[:platform] = "avr";
    self[:ignore_unittest] = true;
    self[:run_ssh] = false;  
      
  end
  
end
