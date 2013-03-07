

class Configuration
  
  protected
  
  def setup()
    self[:is_host] = false;
    self[:platform] = "pi";
    self[:remote_run_folder] = "/Volumes/pi/Code/Run";
    self[:hostname] = ENV["pi_host"]
    self[:username] = "pi"
    self[:password] = ENV["pi_pwd"]
    self[:run_folder] = "/home/pi/Code/Run";  
      
  end
  
end
