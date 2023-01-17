#ifndef LED_H_
#define LED_H_

#include <stdint.h>
#include "misc.h"
//#include "hardware/config.h"


  //constexpr uint16_t PWM_MAX = 65535; 
  constexpr uint16_t PWM_HALF = PWM_MAX/2;
  constexpr uint16_t PWM_ZERO = 0; //actually 5 is the PWM value, where the LED turns off 

  //constexpr uint16_t PWM_MAX_16 = 65535;

  //constexpr uint8_t PWM_DuCy_FULL = 100;
  //constexpr uint8_t PWM_DuCy_HALF = 50;

  constexpr uint8_t COUNT_INDEX_MAX = 100;
  constexpr uint8_t CORRECTION_FACTOR = 45;

  
  struct RGB_UINT16 {uint16_t red; uint16_t green; uint16_t blue;}; //Declaration of Color struct with 16bit integer
  struct RGB_FLOAT {float red; float green; float blue;};  //Declaration of Color struct with float
//  struct RGB_COLOR_DuCy {uint8_t green; uint8_t red; uint8_t blue;}; //Declaration of Color struct for DutyCycle of color
  
  constexpr RGB_UINT16 LED_COLORS[14]= //colors in RGB order
  { 
    {PWM_ZERO          , PWM_ZERO         , PWM_ZERO     },  // black/dark
    {PWM_MAX          , PWM_ZERO   , PWM_ZERO     },  // red
    {PWM_MAX   , PWM_HALF   , PWM_ZERO     },  // orange
    {PWM_MAX    , PWM_MAX   , PWM_ZERO     },  // yellow
    {PWM_HALF    , PWM_MAX  , PWM_ZERO     },  // green-yellow
    {PWM_ZERO    , PWM_MAX         , PWM_ZERO     },  // green
    {PWM_ZERO    , PWM_MAX         , PWM_HALF},// mint-green
    {PWM_ZERO    , PWM_MAX         , PWM_MAX}, // turquois
    {PWM_ZERO   , PWM_HALF         , PWM_MAX}, // light blue
    {PWM_ZERO          , PWM_ZERO         , PWM_MAX}, // blue
    {PWM_HALF          , PWM_ZERO  , PWM_MAX}, // violet
    {PWM_MAX          , PWM_ZERO   , PWM_MAX}, // pink
    {PWM_MAX          , PWM_ZERO   , PWM_HALF},// magenta
    {PWM_MAX    , PWM_MAX   , PWM_MAX}  // white
  };

  constexpr RGB_FLOAT colorPerceptionCorrection = {3.0, 2.0, 1.0};  



  enum colorNames : uint8_t{
    dark=0, red=1, orange, yellow, greenYellow, green, mintGreen, turquois, lightBlue, blue, violet, pink, magenta, white=13
  };

  class Hardware; //Forward Declaration

  class LED
  {
    public:
      colorNames colorIndexOld;      
      colorNames colorIndex;

      uint8_t fadingDirection;
      uint8_t pwmIndex;

      bool flag_cycleComplete;
      bool flag_halfCycleComplete;
      bool flag_isActive;
      bool flag_modeChanged;

      RGB_UINT16 actualColor;

      uint8_t brightnessReduction;      

//      RGB_UINT16 LED_COLORS[14]; // WARUM gibt das Probleme??? UWE fraagen!

//      static constexpr RGB_UINT16 LED_COLORS[14]= //colors in RGB order
//      { 
//        {PWM_ZERO          , PWM_ZERO         , PWM_ZERO     },  // black/dark
//        {PWM_MAX          , PWM_ZERO   , PWM_ZERO     },  // red
//        {PWM_MAX   , PWM_HALF   , PWM_ZERO     },  // orange
//        {PWM_MAX    , PWM_MAX   , PWM_ZERO     },  // yellow
//        {PWM_HALF    , PWM_MAX  , PWM_ZERO     },  // green-yellow
//        {PWM_ZERO    , PWM_MAX         , PWM_ZERO     },  // green
//        {PWM_ZERO    , PWM_MAX         , PWM_HALF},// mint-green
//        {PWM_ZERO    , PWM_MAX         , PWM_MAX}, // turquois
//        {PWM_ZERO   , PWM_HALF         , PWM_MAX}, // light blue
//        {PWM_ZERO          , PWM_ZERO         , PWM_MAX}, // blue
//        {PWM_HALF          , PWM_ZERO  , PWM_MAX}, // violet
//        {PWM_MAX          , PWM_ZERO   , PWM_MAX}, // pink
//        {PWM_MAX          , PWM_ZERO   , PWM_HALF},// magenta
//        {PWM_MAX    , PWM_MAX   , PWM_MAX}  // white
//      };

      //void adjust_brightness();
      
      void checkModeandCycleSingleColor(uint16_t period);
      void checkModeandCycleRainbow (uint16_t period);
      void reset_LED();
      void flash_color();
      void fade_color();
      void pulse_color();
      void led_event(uint8_t color);
      void send_color_to_PWM();      

      LED(Hardware &hw); // Default Constructor

      private:
        Hardware &hw;
        
        uint16_t calc_new_value(uint16_t pwmValueOld, uint16_t pwmValueNew);
        RGB_UINT16 changeBrightness(RGB_UINT16 rgbc);
        RGB_UINT16 correctColorPerception(RGB_UINT16 rgbc);
  };

#endif /* LED_H_ */