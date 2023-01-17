#include <stdint.h>
#include "Pushbutton.h"
#include "hardware.h"


PushButton::PushButton(Hardware & hw) : debounceCounter(0),
                                        //pressState(pressType::noPress),
                                        debounceStateMachine(DebounceStates::normal),
                                        hw(hw)
{
};  
    //cntx_debug_printf("\n SemaphoreVal: %i ",semaDebounce->get());

    void PushButton::testFunction(void)
    {
          if( hw.semaDebounce->get_And_Reset_If_Larger(500) )
      printf("\n 1second");
     
    }

/* Debounce Version 1 */
//void PushButton::debouncePB(){
PressStates PushButton::debouncePB(void)
//void PushButton::debouncePB(void)
{  
  static int debounceAverage = 0;
  static int debounceCycles = 1;

  if(hw.semaDebounce->get_And_Reset_If_Larger(10) ){ //repeated after 100ms

    switch (this->debounceStateMachine)
    {
      case DebounceStates::normal :
        if( hw.get_PushButton() == PushButtonStates::pressed ){ 
          this->debounceStateMachine = DebounceStates::quick;
          this->flag_pressEvent = true;
        }
//         cntx_debug_printf("\nDeboState: %u", this->pressStateMachine);
        this->pressState = PressStates::noPress;
        break;

      case DebounceStates::quick :
        this->debounceCounter++;
        //if( !(this->debounceCounter % 10) )
        //  printf("\nDeb Count: %i", this->debounceCounter);
        if( hw.get_PushButton() == PushButtonStates::released ){


          if( this->debounceCounter > 10 ){
            debounceAverage = (debounceAverage + this->debounceCounter) / 2;
            debounceCycles++; 
            this->pressState = PressStates::shortPress;
            this->debounceStateMachine = DebounceStates::goToNormal;
            hw.semaExtendedPress->get_And_Reset(); // Reset Semaphore after each short press            
          }
        }
        //if(this->debounceCounter > 500)
        //  printf("\nHere");          

        else if( this->debounceCounter > 50 ){
            this->debounceStateMachine = DebounceStates::extended;            
            this->pressState = PressStates::longPress;
           // this->flag_extendedPress = true; // After a long press special menu is entered for a delayed period            
        }
        else
          this->pressState = PressStates::noPress;
        break;

      case DebounceStates::extended :
        if (hw.get_PushButton() == PushButtonStates::released){
          this->debounceStateMachine = DebounceStates::goToNormal;
          
          
          //this->flag_extendedPress = false;
          //this->pressState = PressStates::longPress;
        }
        else
          this->pressState = PressStates::pressHold;
        break;
    
      case DebounceStates::goToNormal :
        this->debounceStateMachine = DebounceStates::normal;
        this->debounceCounter = 0;
        this->flag_pressEvent = false;
        this->pressState = PressStates::noPress;
        break;

      default:
        this->pressState = PressStates::noPress;
        break;
    }/* switch pressStateMachine */

  
  
  /**** Strange Behavior with the following if-clause ****/
  //uint8_t var = 0;
  //bool state;
  //state = (bool)(hw.semaExtendedPress->get_And_Reset_If_Larger(10) && (this->flag_extendedPress == true));
  //if( state ){
  //  printf("state: %u \n",state);
  //}
  /*if( hw.semaExtendedPress->get_And_Reset_If_Larger(50) && this->flag_extendedPress == true ){*/ //After time is gone without any press, system goes back to normal
  //if( this->flag_extendedPress == true )
  //  printf("semaExtended: %u \n", hw.semaExtendedPress->value);
  if( hw.semaExtendedPress->get_And_Reset_If_Larger(30) ){
    //if( this->flag_extendedPress == true ){
    if( this->pressState == PressStates::pressHold ){
      this->flag_extendedPress = false;
      //this->debounceStateMachine = DebounceStates::normal; //without, the state machine would proceed with a regular long press
      //this->pressState = PressStates::noPress;
      this->pressState = PressStates::pressHold;
    }
  }
    //if( 1 ){

    //  printf("state: %u \n",state);
    //}
  return this->pressState;
  }
  return PressStates::noPress;
}/* debouncePB */


//        if((hw.getPushButton() == ONE_LED_PCB::pushButtonState::pressed) && (PushButton::debounceCounter > 8)){
//          return longORshortPress::longPress;          
//        }
//        else {






