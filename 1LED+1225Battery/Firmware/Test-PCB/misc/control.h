#ifndef CONTROL_H_
#define CONTROL_H_

#include <stdint.h>
//#include "Pushbutton.h"


  class Hardware; // Forward Declaration
  class PushButton; // Forward Declaration
  class LED;        // Forward Declaration


  enum lightModes : uint8_t{
    fadeColors = 1,
    flashColorsFull,
    pulseColorsFull,
    flashColorsSingle,    
    pulseColorsSingle
  };


  constexpr uint8_t MODECOUNTMAX = 5;
  constexpr uint8_t SPECIALMODECOUNTMAX = 1;

  class Control
  { 
    public:
      
      Control(/*Hardware & hw,*/PushButton &PB1, LED &LED1);   //Default Constructor 
        
      uint16_t newTimerPeriod;      
      
      void handleLightModes();
      void handlePBpress();
      void handleSysModes();
    
    private:
      
      uint8_t lightModeIndex; //Can become static?
      uint8_t specialModeIndex; //Can become static?
      
      //Hardware & hw;
      LED &LED1;
      PushButton &PB1;

      void goSleep();
    
  };

#endif /* CONTROL_H_ */