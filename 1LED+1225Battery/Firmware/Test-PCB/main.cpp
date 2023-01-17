/* Includes ------------------------------------------------------------------*/
#include "misc.h"
#include "hardware.h"
#include "control.h"
#include "LED.h"
#include "Pushbutton.h"
//#include "stm32l0xx.h"
//#include "stm32l011xx.h"

 //static uint32_t tick = 2000; // __IO "volatile" expression from core_cm0plus.h
 __IO bool flagMillisecs = false;
 __IO bool flagPwmIndex = false;
 __IO bool flagSemaphore = false;
 static constexpr uint16_t DELAYMS = 50;
 static constexpr uint16_t RELOADPWMINDEX = 20;

void delay_ms(uint16_t msValue);
Hardware hw;
LED led1(hw);
PushButton puBo1Big(hw);
Control ctrl(puBo1Big, led1);



/* MAIN ------------------------------------------------------------------*/
int main(void)
{
  #ifndef __Vendor_SysTickConfig
  #error __Vendor_SysTickConfig not defined
  #endif

  hw.init();
  //if( !hw.init() )
  // printf("Hardware Init failed!");

 // printf("Start...\n");
  SysTick_Config(1000); // 2MHz : 1000 = 2000Hz which is 500µs
  //printf("Clock: %d\n", SystemCoreClock);
  
  
  while(1){
   // hw.pwmsLeds[0]->set_dutyCycle(i);
   //// hw.pwmsLeds[0]->get_info();

   // if (flagMillisecs == true){
   //   i += 1;
   //   flagMillisecs = false;
   // }
   // i = (i >= 100) ? 0 : i;
  //printf("PushButton: %i\n",hw.pinPushbuttonBig->read_InputValue());

    /* Cyclic Update Lighting */
    ctrl.handlePBpress();
    if(flagPwmIndex){
      flagPwmIndex = false; // Reset flag after interrupt occured

      ctrl.handleLightModes();
      ctrl.handleSysModes();
    }

    
    //if( flagSemaphore ){
    //  for (uint8_t index = 0; index <3; index++){
    //    hw.semaArray[index]->update();
    //  }
    //  flagSemaphore = false;
    //}

    //printf("Debouncestate: %i\n",puBo1Big.debouncePB());

    //uint8_t push;
    //push = puBo1Big.debouncePB();
    //if( push == PressStates::shortPress )
    //  printf("short push\n");
    //else if( push == PressStates::longPress )
    //  printf("long push\n");
     
     //puBo1Big.testFunction();

    //if( hw.semaDebounce->get_And_Reset_If_Larger(1000) )
    //  printf("\n 1second");

  }


  hw.get_clocks();

//  debug_exit(0);
} //main end

//void delay_ms( uint16_t msValue)
//{ 
//  uint16_t ms;
//  tick = ms; 
//  while(tick){}
//}

extern "C" void SysTick_Handler(void)
{ 
  for (uint8_t index = 0; index <3; index++){
    hw.semaArray[index]->update();
  }

  flagSemaphore = true;
  static uint16_t delayMillisecs = DELAYMS;
  if( --delayMillisecs == 0){
    flagMillisecs = true;
    delayMillisecs = DELAYMS;
  }

  static uint16_t delayPwmIndex = RELOADPWMINDEX;
  if( --delayPwmIndex == 0){
    flagPwmIndex = true;
    delayPwmIndex = RELOADPWMINDEX;
  }

  //if( tick == 0){
  //  tick = 2000;
  //  printf("\n 1second");
  //}

  //--tick;
}

