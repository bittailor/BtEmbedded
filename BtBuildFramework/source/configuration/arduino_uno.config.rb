

class Configuration
  
  protected
  
  def setup()
    store :is_host,         false
    store :platform,        "avr"
    store :ignore_unittest, true
    store :run_ssh,         false 
      
    store :mcu,   "atmega328p"
    store :f_cpu, "16000000UL"   
      
  end
  
end
