

class Configuration
  
  protected
  
  def setup()
    self[:is_host] = false;
    self[:platform] = "pi";
    self[:remote_run_folder] = "/Volumes/pi/Code/Run";
    self[:hostname] =`BtPiIp`.strip()
    self[:username] = "pi"
    self[:password] = ENV["pi_pwd"]
    self[:run_folder] = "/home/pi/Code/Run";  
    self[:run_ssh] = env('pi_run_ssh', false); 

    # self[:test_filter] = "-*IntegrationTest.*";   
    # self[:test_filter] = "Quick*.*";   
      
  end
  
end
