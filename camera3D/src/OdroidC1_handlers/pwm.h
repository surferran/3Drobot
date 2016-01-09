/* 
	based on the link : http://odroid.com/dokuwiki/doku.php?id=en:c1_hardware_pwm
	last edited on : 17/12/15
 general explanation :
 using PWM PINS:  PWM0: 33, PWM1: 19 (using TWO pwm setup, You have: enable0, freq0, duty0 AND enable1, freq1, duty1
 - set_duty_cycle() : Duty cycle is in 0..1023 range (10bit resolution)
 - enable()			: Enable/Disable that PWM
 - set_freq()		: setting PWM Frequency in Hertz! (Maximum Frequency is 1Mhz)
*/
#ifdef COMPILING_ON_ROBOT
#ifndef pwm_H
#define pwm_H

#include <memory>

class PWM
{
public:
	virtual ~PWM() {};
	virtual void enable(int id, bool state) = 0;
	virtual void set_freq(int id, int freq) = 0;
	virtual void set_duty_cycle(int id, int value) = 0;
  
  typedef std::shared_ptr<PWM> pwm_ptr;
  static pwm_ptr create();
};

#endif // pwm_H

#else

#ifndef pwm_H
#define pwm_H

#include <memory>

class PWM
{
public:
	virtual		 ~PWM() {};
	virtual void enable(int id, bool state) {};
	virtual void set_freq(int id, int freq) {};
	virtual void set_duty_cycle(int id, int value) {};

	typedef std::shared_ptr<PWM> pwm_ptr;
	static pwm_ptr create();
};

#endif // pwm_H

#endif // COMPILING_ON_ROBOT

