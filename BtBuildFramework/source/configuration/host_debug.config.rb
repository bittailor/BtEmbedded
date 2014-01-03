

class Configuration
  
  protected
  
  def setup()
    self[:is_host] = true;
    self[:platform] = "osx";
    self[:test_filter] = "-*IntegrationTest.*";
  end
  
end
