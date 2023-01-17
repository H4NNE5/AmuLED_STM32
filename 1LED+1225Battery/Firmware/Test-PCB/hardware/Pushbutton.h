/*
 * debounce.h
 *
 * Created: 23.04.2016 20:38:16
 *  Author: Hannes
 */ 

#ifndef PUSHBUTTON_H_
#define PUSHBUTTON_H_

#include <stdint.h>
#include "hardware.h"

  class Hardware; //Forward Declaration

  enum PressStates
  : uint8_t
  {
    noPress = 0,
    shortPress,
    longPress,
    pressHold
  };

  enum DebounceStates
  : uint8_t
  {
    normal=0,
    quick,
    extended,
    goToNormal
  };

  enum PushButtonStates
  : bool
  { 
    released = false,
    pressed = true
  };


  class PushButton
  {
    public:

      PushButton(Hardware & hw);
      
      bool flag_extendedPress;
      bool flag_pressEvent;
      PressStates pressState;
      DebounceStates debounceStateMachine;
      PushButtonStates pushButtonState;
      //PressType pt;

      PressStates debouncePB(void);
      //void debouncePB(void);
      void testFunction(void);

    private:
      
      uint16_t debounceCounter;
      Hardware &hw;
  }; 


#endif /* PUSHBUTTON */
