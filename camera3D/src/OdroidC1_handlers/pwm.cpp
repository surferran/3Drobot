
#ifdef COMPILING_ON_ROBOT

#include "pwm.h"
#include <iostream>
#include <fstream>
#include "xstring.h"
#include <cstdlib>

class FSPWM : public PWM
{
  static void write_file(const xstring& name, int value)
  {
    static const xstring dir="/sys/devices/platform/pwm-ctrl/";
    std::ofstream f(dir+name);
    f << value << std::endl;
  }
public:
  FSPWM()
  {
    system("modprobe pwm-meson npwm=2");
    system("modprobe pwm-ctrl");
  }
  
  virtual void enable(int id, bool state) override
  {
    write_file("enable"+xstring(id),state?1:0);
  }
  
  virtual void set_freq(int id, int freq) override
  {
    write_file("freq"+xstring(id),freq);
  }
  
  virtual void set_duty_cycle(int id, int value) override
  {
    write_file("duty"+xstring(id),value);
  }
  
};

PWM::pwm_ptr PWM::create()
{
  return PWM::pwm_ptr(new FSPWM);
}

#endif // COMPILING_ON_ROBOT