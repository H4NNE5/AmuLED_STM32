#include "led.h"
#include "misc.h"

#include "hardware.h"


//#include <hardware/hardware.h>
//#include "hardware/led.h"
#include <stdlib.h>
////#include <stdint.h>

__IO static uint8_t countingIndex = 0;



  LED::LED(Hardware &hw):hw(hw) //LED constructor with initial settings
  { 
    this->colorIndex = colorNames::red;
    //this->reset_LED();
    brightnessReduction = 8;
    
    //this->colorIndex = colorNames::red; //set initial color
  }

  /******************************************************************************/
  /*                          Reset LED-state                                   */
  /******************************************************************************/
  void LED::reset_LED()
  {
    this->flag_cycleComplete = true;
    this->flag_halfCycleComplete = false;
    this->flag_modeChanged = false;
    this->colorIndex = colorNames::red;
    this->pwmIndex = 0;
    this->fadingDirection = 1;
//    this->actualColor = LED_COLORS[static_cast<uint8_t>(this->colorIndex)]; //reassign indexed color to RGB-PWM values
    this->actualColor = LED_COLORS[static_cast<uint8_t>(colorNames::red)]; //reassign indexed color to RGB-PWM values
  }


  /******************************************************************************/
  /*                          Change LED Brightness                             */
  /******************************************************************************/
  RGB_UINT16 LED::changeBrightness(RGB_UINT16 rgbc)
  {
    rgbc.red /= this->brightnessReduction;
    rgbc.green /= this->brightnessReduction;
    rgbc.blue /= this->brightnessReduction;

    return rgbc;
  }

  /******************************************************************************/
  /*                          Check for change in mode and cycle                */
  /******************************************************************************/
  void LED::checkModeandCycleRainbow(uint16_t period)
  {
    if( this->flag_modeChanged == true ){                        
      this->flag_modeChanged = false;
      this->actualColor = LED_COLORS[static_cast<uint8_t>(this->colorIndex)];
      hw.pwmsLeds[0]->set_PWM_period(period);
      //hw.timerLP1forLED->set_period(period);
      this->pwmIndex = 0;
    }

    if(this->flag_cycleComplete){
      this->pwmIndex = 0;
      this->flag_cycleComplete = false;
      this->colorIndexOld = this->colorIndex;
      this->actualColor = LED_COLORS[/*static_cast<uint8_t>*/(this->colorIndexOld)]; //assign indexed color to RGB-PWM values

      if(this->colorIndex == colorNames::magenta) //cycle to red if color is magenta
        this->colorIndex = colorNames::red;
      else
        this->colorIndex = static_cast<colorNames>(/*static_cast<uint8_t>*/(this->colorIndex) + 1 );
      
    }
  }

  /******************************************************************************/
  /*                          Check for change in mode and cycle                */
  /******************************************************************************/
  void LED::checkModeandCycleSingleColor(uint16_t period)
  {
    if( this->flag_modeChanged == true ){
      this->flag_modeChanged = false;
      hw.pwmsLeds[0]->set_PWM_period(period);
      //hw.timerLP1forLED->set_period(period);
      this->reset_LED();      
    }

    if(this->flag_cycleComplete){
      this->flag_cycleComplete = false;
      this->actualColor = LED_COLORS[static_cast<uint8_t>(this->colorIndex)]; //reassign indexed color to RGB-PWM values
    }
  }

  /******************************************************************************/
  /*                          Correct Color Values for equal Perception         */
  /******************************************************************************/
  RGB_UINT16 LED::correctColorPerception(RGB_UINT16 rgbc)
  { 
    rgbc.red    = static_cast<uint16_t>(rgbc.red / colorPerceptionCorrection.red);
    rgbc.green  = static_cast<uint16_t>(rgbc.green / colorPerceptionCorrection.green);
    rgbc.blue   = static_cast<uint16_t>(rgbc.blue / colorPerceptionCorrection.blue);
    

    return rgbc;
  }

  /******************************************************************************/
  /*                          BlinkEvent for Feedback                           */
  /******************************************************************************/
  void LED::led_event(uint8_t color)
  { 
    //this->brightnessReduction = 3;
    this->actualColor = LED_COLORS[color];
    this->send_color_to_PWM();
    while( !hw.semaDelay->get_And_Reset_If_Larger(10) ){}    

    this->actualColor = LED_COLORS[0];
    this->send_color_to_PWM();
    while( !hw.semaDelay->get_And_Reset_If_Larger(10) ){}

  }

  /******************************************************************************/
  /*                          fade color                                       */
  /******************************************************************************/
  void LED::fade_color()
  { 
    /*if( (this->actualColor != LED_COLORS[static_cast<uint8_t>(this->colorIndex)]){
      for( countingIndex = 0; countingIndex++; countingIndex < NUMOFLEDPWMS ){
         this->actualColor[countingIndex] = this->calc_new_value(LED_COLORS[static_cast<uint8_t>(this->colorIndexOld)].red, LED_COLORS[static_cast<uint8_t>(this->colorIndex)].red);
         }
         // Need to change from struct to 2dimensional array LED_COLORS[index][RED]
    }*/
      
    
    
    if( (this->actualColor.red != LED_COLORS[static_cast<uint8_t>(this->colorIndex)].red)||
        (this->actualColor.green != LED_COLORS[static_cast<uint8_t>(this->colorIndex)].green) ||
        (this->actualColor.blue != LED_COLORS[static_cast<uint8_t>(this->colorIndex)].blue) ){

           this->actualColor.red = this->calc_new_value(LED_COLORS[static_cast<uint8_t>(this->colorIndexOld)].red, LED_COLORS[static_cast<uint8_t>(this->colorIndex)].red);
           this->actualColor.green = this->calc_new_value(LED_COLORS[static_cast<uint8_t>(this->colorIndexOld)].green, LED_COLORS[static_cast<uint8_t>(this->colorIndex)].green);
           this->actualColor.blue = this->calc_new_value(LED_COLORS[static_cast<uint8_t>(this->colorIndexOld)].blue, LED_COLORS[static_cast<uint8_t>(this->colorIndex)].blue);
    }
    else{
            this->flag_cycleComplete = true;
            //this->colorIndexOld = this->colorIndexNew; // current LED_COLORS index becomes "old" color index
            this->pwmIndex = 0; // Reset pwmIndex for all 3 colors
    }

  }//fade_color()

  /******************************************************************************/
  /*                          flash color                                       */
  /******************************************************************************/
  void LED::flash_color()
  {
    if( (this->actualColor.red != LED_COLORS[static_cast<uint8_t>(colorNames::dark)].red)||
        (this->actualColor.green != LED_COLORS[static_cast<uint8_t>(colorNames::dark)].green) ||
        (this->actualColor.blue != LED_COLORS[static_cast<uint8_t>(colorNames::dark)].blue) ){		
      //LED_EVENT(6);
          this->actualColor.red = this->calc_new_value(LED_COLORS[static_cast<uint8_t>(this->colorIndex)].red, LED_COLORS[static_cast<uint8_t>(colorNames::dark)].red);
          this->actualColor.green = this->calc_new_value(LED_COLORS[static_cast<uint8_t>(this->colorIndex)].green, LED_COLORS[static_cast<uint8_t>(colorNames::dark)].green);
          this->actualColor.blue = this->calc_new_value(LED_COLORS[static_cast<uint8_t>(this->colorIndex)].blue, LED_COLORS[static_cast<uint8_t>(colorNames::dark)].blue);
    }		
    else{
        this->flag_cycleComplete = true;
        this->pwmIndex = 0; // Reset pwmIndex
        //hw.mcu.delay(300);
    }
  }//flash_color()

  /******************************************************************************/
  /*                          pulse color                                       */
  /******************************************************************************/
  void LED::pulse_color()
  {
    if( this->flag_cycleComplete == 1 ){  //initially when color index has changed
      this->flag_cycleComplete = 0;
    }

//    if( (this->actualColor.red != LED_COLORS[static_cast<uint8_t>(this->colorIndex)].red)||
//        (this->actualColor.green != LED_COLORS[static_cast<uint8_t>(this->colorIndex)].green) ||
//        (this->actualColor.blue != LED_COLORS[static_cast<uint8_t>(this->colorIndex)].blue) ){		
      //LED_EVENT(6);
        if(this->flag_halfCycleComplete == false){
          this->actualColor.red = this->calc_new_value(LED_COLORS[static_cast<uint8_t>(colorNames::dark)].red, LED_COLORS[static_cast<uint8_t>(this->colorIndex)].red);
          this->actualColor.green = this->calc_new_value(LED_COLORS[static_cast<uint8_t>(colorNames::dark)].green, LED_COLORS[static_cast<uint8_t>(this->colorIndex)].green);
          this->actualColor.blue = this->calc_new_value(LED_COLORS[static_cast<uint8_t>(colorNames::dark)].blue, LED_COLORS[static_cast<uint8_t>(this->colorIndex)].blue);
        }
        else{
          this->actualColor.red = this->calc_new_value(LED_COLORS[static_cast<uint8_t>(this->colorIndex)].red, LED_COLORS[static_cast<uint8_t>(colorNames::dark)].red);
          this->actualColor.green = this->calc_new_value(LED_COLORS[static_cast<uint8_t>(this->colorIndex)].green, LED_COLORS[static_cast<uint8_t>(colorNames::dark)].green);
          this->actualColor.blue = this->calc_new_value(LED_COLORS[static_cast<uint8_t>(this->colorIndex)].blue, LED_COLORS[static_cast<uint8_t>(colorNames::dark)].blue);
        }
    //}	
    if( this->pwmIndex == COUNT_INDEX_MAX ){
      if( this->flag_halfCycleComplete == false ){
              this->fadingDirection = -1;
              this->flag_halfCycleComplete = true;              
      }
      else{
              this->flag_cycleComplete = true;
              this->flag_halfCycleComplete = false;
              //LED->flag_isActive = 0;
      }
      this->pwmIndex = 0;
    }
    

  }
  /******************************************************************************/
  /*                          calc new value                                    */
  /******************************************************************************/
  uint16_t LED::calc_new_value(uint16_t pwmValueOld, uint16_t pwmValueNew)
  {    
    static uint8_t divider = 0, offsetIndex = 0;
    static uint16_t result = 0, offsetPWM = 0;
    static uint32_t pwmBaseValue = 0;
    static int8_t direction = 0;	
    static int32_t pwmDiff = 0;

    pwmDiff = pwmValueNew - pwmValueOld;

    if( pwmDiff == 0 ) return pwmValueOld;

    if( abs(pwmDiff) == PWM_MAX )
            divider = CORRECTION_FACTOR;
    else
            divider = 2*CORRECTION_FACTOR;

    if( pwmDiff < 0 ){
            if( pwmDiff == -pwmValueNew - 1 ){ //Check if diff = -PWM_MAX/2
                    offsetPWM = pwmValueNew;
                    pwmBaseValue = pwmValueNew + 1;
            }
            else {
                    offsetPWM = 0;			
                    pwmBaseValue = pwmValueOld;
            }
            direction = -1;
            offsetIndex = COUNT_INDEX_MAX;
    }
    else if( pwmDiff > 0){
            if( pwmDiff == pwmValueOld + 1 ){//Check if diff = +PWM_MAX/2
                    offsetPWM = pwmValueOld;
                    pwmBaseValue = pwmValueOld + 1;
            }
            else {
                    offsetPWM = 0;			
                    pwmBaseValue = pwmValueNew;
            }
            direction = 1;
            offsetIndex = 0;		
    }
//    cntx_debug_printf("\n\n pwmValue: %i", pwmValue);
//    cntx_debug_printf("\n pwmDiff: %i", pwmDiff);

    // OLD FORMULA! result = offsetPWM + pow(offsetIndex + direction * pwmIndex, EXPONENT) * quotient/*/pow(COUNT_INDEX_MAX, EXPONENT)*/ * (pwmBaseValue) + 0.5; // Calculate PWM value with upper formula
    /***********************************************************************************************/
    /*                                                                           pwmBaseValue      */
    /* offsetPWM + (offsetIndex + fadingDirection * pwmIndex)^EXPONENT * ------------------------- */
    /*                                                                    COUNT_INDEX_MAX^EXPONENT */
    /***********************************************************************************************/

    result = offsetPWM + (pwmBaseValue * (offsetIndex + direction * this->pwmIndex)/COUNT_INDEX_MAX - this->pwmIndex*(COUNT_INDEX_MAX-this->pwmIndex)/divider); 
    /* formula from Mikrocontroller.net forum */
    /* Old formula left commented for comparison. Same principle but faster operation */
    /* Basic mapping of pwmIndex to range of 0 to 65535 PWM values but with correction formula to achieve exponential looking curve */
    
//    cntx_debug_printf("\n result: %i", result);
    return result;
  }

  /******************************************************************************/
  /*                          Output Color Values to LED/PWM                    */
  /******************************************************************************/
  void LED::send_color_to_PWM()
  { 
      //if( hw.semaDebounce->get_And_Reset_If_Larger(1000) )
      //printf("\n 1second");

    static RGB_UINT16 colorTemp;

    colorTemp = this->actualColor;

    colorTemp = this->correctColorPerception(colorTemp);

    colorTemp = this->changeBrightness(colorTemp);//this->brightnessReduction);

    hw.pwmsLeds[RED]->set_PWM(colorTemp.red);
    hw.pwmsLeds[GREEN]->set_PWM(colorTemp.green);
    hw.pwmsLeds[BLUE]->set_PWM(colorTemp.blue);     
 
  }//send2LED()


//  constexpr RGB_UINT16 LED_COLORS[14]= //colors in GRB order
//  { 
//    {PWM_ZERO          , PWM_ZERO         , PWM_ZERO     },  // black/dark
//    {PWM_ZERO          , PWM_MAX   , PWM_ZERO     },  // red
//    {PWM_HALF   , PWM_MAX   , PWM_ZERO     },  // orange
//    {PWM_MAX    , PWM_MAX   , PWM_ZERO     },  // yellow
//    {PWM_MAX    , PWM_HALF  , PWM_ZERO     },  // green-yellow
//    {PWM_MAX    , PWM_ZERO         , PWM_ZERO     },  // green
//    {PWM_MAX    , PWM_ZERO         , PWM_HALF},// mint-green
//    {PWM_MAX    , PWM_ZERO         , PWM_MAX}, // turquois
//    {PWM_HALF   , PWM_ZERO         , PWM_MAX}, // light blue
//    {PWM_ZERO          , PWM_ZERO         , PWM_MAX}, // blue
//    {PWM_ZERO          , PWM_HALF  , PWM_MAX}, // violet
//    {PWM_ZERO          , PWM_MAX   , PWM_MAX}, // pink
//    {PWM_ZERO          , PWM_MAX   , PWM_HALF},// magenta
//    {PWM_MAX    , PWM_MAX   , PWM_MAX}  // white
//  };


//    constexpr RGB_UINT16 LED_COLORS[14]= //inverted colors in RGB order
//    { //Instant Initialization of new struct array for default colors
//      {PWM_MAX  , PWM_MAX , PWM_MAX},  // 0 black/dark 0
//      {PWM_MAX  , 0       , PWM_MAX},  // 1 red 1
//      {PWM_HALF , 0       , PWM_MAX},  // 2 orange 2
//      {0        , 0       , PWM_MAX},  // 3 yellow 3
//      {0        , PWM_HALF, PWM_MAX},  // 4 green-yellow 4
//      {0        , PWM_MAX , PWM_MAX},  // 5 green 5
//      {0        , PWM_MAX , PWM_HALF}, // 6 mint-green 6
//      {0        , PWM_MAX , 0       }, // 7 turquois 7
//      {PWM_HALF , PWM_MAX , 0       }, // 8 light-blue 8
//      {PWM_MAX  , PWM_MAX , 0       }, // 9 blue 9
//      {PWM_MAX  , PWM_HALF, 0       }, // 10 violet 10
//      {PWM_MAX  , 0       , 0       }, // 11 pink 11
//      {PWM_MAX  , 0       , PWM_HALF}, // 12 magenta 12
//      {0        , 0       , 0       }  // 13 white 13
//    };
