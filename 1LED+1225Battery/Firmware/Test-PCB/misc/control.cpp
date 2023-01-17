#include "control.h"
#include "Pushbutton.h"
#include "led.h"
#include "hardware.h"



  // Instanziation of PushButton object
  //PushButton PB1(/*Hardware & hw*/);
 
  Control::Control(/*Hardware & hw,*/ PushButton &PB1, LED &LED1) :/* hw(hw),*/ PB1(PB1), LED1(LED1) //Default Constructor
  { 
    this->lightModeIndex = lightModes::fadeColors;
    this->specialModeIndex = 1;    
  }
  
  /******************************************************************************/
  /*     Send C to sleep                                                       */
  /******************************************************************************/
  void Control::goSleep()
  {
    //PWR->CR |= 
               //PWR_CSR_EWUP1 // Enable WakeUp pin (Only necessary if C is in Standby mode)
               //;
    __WFE();
    __WFE();
  }

/******************************************************************************/
/*     Deal with button presses                                               */
/******************************************************************************/
void Control::handlePBpress()
{ 
  PressStates push = PressStates::noPress;
  push = PB1.debouncePB();
  if( PB1.flag_pressEvent == true ){

  switch( push )
  { //first debounce the PushButton
    
      /***** short press handling *****/
      case PressStates::shortPress:
        //PB1.pressState = PressStates::noPress;

        if(PB1.flag_extendedPress == true){
          if(this->specialModeIndex == SPECIALMODECOUNTMAX){
            this->specialModeIndex = 1; 
          }
          else
            this->specialModeIndex += 1;

          LED1.led_event(10);
  //          LED1.led_event(4);
  //          LED1.led_event(8);
        }
 
        if(this->lightModeIndex < 4){
          this->lightModeIndex += 1;
          LED1.flag_modeChanged = true;
        }
        else /*if (this->lightModeIndex >= 4)*/{ //switch lightModeIndex and colorIndex separately, if highest colorIndex is reached, next mode is entered with colorIndex gets resetted
          if(LED1.colorIndex == colorNames::magenta){
            this->lightModeIndex += 1;
            LED1.colorIndex = colorNames::red;
            LED1.flag_modeChanged = true; //
          }
          else
            LED1.colorIndex = static_cast<colorNames>( static_cast<uint8_t>(LED1.colorIndex) + 1 ); //casting and recasting for enum class compatibility

          //LED1.flag_modeChanged = true; //
        }
        if(this->lightModeIndex >  MODECOUNTMAX) //Cycle through modes
          this->lightModeIndex = 1;
     
      break; //short Press handling

      /***** long press handling *****/
      case PressStates::longPress:
          //PB1.pressState = PressStates::noPress;
          if(LED1.brightnessReduction == 8)
            LED1.brightnessReduction = 1;
          else  
            LED1.brightnessReduction *= 2;
   
        break; //long press handling

        case PressStates::pressHold:
          this->lightModeIndex = 1;
          //this->goSleep();    //go to sleep IF NO SHORT PRESS DETECTED AFTER A LONG PRESS in the required time frame    
          break;

        default:
          break;
    } //END switch case
  }
}

  /******************************************************************************/
  /*     Cycle different light modes                                            */
  /******************************************************************************/
  void Control::handleLightModes()
  {
    if( (LED1.flag_cycleComplete == false) && (LED1.pwmIndex <= 100)) 
      LED1.pwmIndex += 1;

    switch (/*static_cast<lightModes>*/(this->lightModeIndex))
        { 
          /* FADE COLORS into each other rainbowlike */

          case lightModes::fadeColors:

            LED1.checkModeandCycleRainbow(20000);
            if(LED1.flag_cycleComplete){
//              LED1.actualColor = LED_COLORS[static_cast<uint8_t>(colorNames::orange)];
//              LED1.colorIndexOld = colorNames::orange;
//              LED1.colorIndex = colorNames::dark;
              LED1.flag_cycleComplete = false;
            }
            LED1.fade_color();                        
            break;

          /* FLASH COLORS rainbow pattern */
          case lightModes::flashColorsFull:

            LED1.checkModeandCycleRainbow(30000);
            LED1.flash_color();                
            break;

          /* FLASH SINGLE COLORS */
          case lightModes::flashColorsSingle:
     
//            if( LED1.flag_modeChanged == true ){
//              LED1.flag_modeChanged = false;
//              LED1.colorIndexOld = LED1.colorIndex;
//              LED1.reset_LED();
//              hw.timerLP1forLED->set_period(30000);
//            } 
            LED1.checkModeandCycleSingleColor(30000);
            LED1.flash_color();
            break;

          /* PULSE COLORS rainbow pattern */
          case lightModes::pulseColorsFull:

            LED1.checkModeandCycleRainbow(30000);
            LED1.pulse_color();
            break;
                          
          /* PULSE SINGLE COLORS */
          case lightModes::pulseColorsSingle:

            if( LED1.flag_modeChanged == 1 ){
             // LED1.flag_modeChanged = false;
              LED1.reset_LED();
              //hw.timerLP1forLED->set_period(40000);
            } 
            LED1.pulse_color();
            break;

          default:
            break;
        }//switch

        
        LED1.send_color_to_PWM(); // Output color information to LED-PWM
  
  }

  void Control::handleSysModes()
  {
    
//    switch(this->specialModeIndex){
//      case 1:
//      case 2:
//      default: break;
//    }
    
  }
  
