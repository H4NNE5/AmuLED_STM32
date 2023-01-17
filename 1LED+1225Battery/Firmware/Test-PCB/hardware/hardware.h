#include <debugio.h>
#include <stdint.h>

#include "stm32l0xx.h"
#include "stm32l011xx.h"
#include "core_cm0plus.h"

#include "gpio.h"
#include "pwm.h"
#include "semaphore.h"



#ifndef HARDWARE_H_
#define HARDWARE_H_

#define SUCCESS 0x01
#define FAIL 0x00

constexpr uint32_t HSI_CLOCK = 16000000;

/* Time-out values */
static const uint32_t HSI_TIMEOUT_VALUE = 100;  /* 100 ms */
static const uint32_t PLL_TIMEOUT_VALUE = 100;  /* 100 ms */
static const uint32_t CLOCKSWITCH_TIMEOUT_VALUE = 500; /* 500 ms    */

//extern __IO uint32_t tick; // __IO "volatile" expression from core_cm0plus.h

class ClockSettings
{ 

  public:

    ClockSettings(){}; //Default Constructor
  
       

    uint32_t sysCoreClock;
    uint32_t ahbClock;
    uint32_t sysClock;
    uint32_t highClock;
    uint32_t apb1Clock;
    uint32_t apb2Clock;

    bool hsiDiv;
    uint32_t ahbPrescaler;
    uint32_t apb1Prescaler;
    uint32_t apb2Prescaler;

    enum class AhbClockPrescaler
    : uint32_t
    {
      div1 = uint32_t(0x00 << 4),
      div2 = uint32_t(0x08 << 4),
      div4 = uint32_t(0x09 << 4),
      div8 = uint32_t(0x0A << 4),
      div16 = uint32_t(0x0B << 4),
      div64 = uint32_t(0x0C << 4),
      div128 = uint32_t(0x0D << 4),
      div256 = uint32_t(0x0E << 4),
      div512 = uint32_t(0x0F << 4),
    };

    enum class ApbClockPrescaler
    : uint32_t
    {
      div1 = uint32_t(0x03 << 8),
      div2 = uint32_t(0x04 << 8),
      div4 = uint32_t(0x05 << 8),
      div8 = uint32_t(0x06 << 8),
      div16 = uint32_t(0x07 << 8),
    };

     private:
};





class Hardware 
{
  public:
    


    Hardware(){};
    Hardware(int value)
    {
      this->testvalue = value;
    }; //Default Constructor

    Pwm * pwmsLeds[3]; //Array on heap
    Gpio * gpiosLeds[3]; //Array on heap

    Gpio * pinPushbuttonBig;

    Semaphore * semaArray[3];
    Semaphore * semaDebounce;
    Semaphore * semaDelay;
    Semaphore * semaExtendedPress;
    //SemaphoreNode * semaphores;

    int init(void);
    void get_clocks();
    uint32_t testvalue;
    void update_Semaphores();
    bool get_PushButton();   

  private:   
  
    ClockSettings * clocks; 

    int init_clocks();
    int init_PWMs();
    int init_gpios();
    int init_buttons();
    int init_power();
    
};

#endif //HARDWARE_H_