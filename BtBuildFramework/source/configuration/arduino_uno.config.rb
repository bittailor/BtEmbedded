

class Configuration
  
  protected
  
  def setup()
    self[:is_host] = false;
    self[:platform] = "avr";
    self[:run_ssh] = false;  
      
  end
  
end
