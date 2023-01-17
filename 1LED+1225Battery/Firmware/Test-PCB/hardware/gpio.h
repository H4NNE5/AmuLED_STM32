#include "stm32l0xx.h"

#ifndef GPIO_H_
#define GPIO_H_


class Gpio
{
  public: 

    enum Modes:uint8_t
    {
      inputFloating = 0x00,
      inputPullUp = 0x01,
      inputPullDown = 0x02,
      outputPushPull = 0x10,
      outputPushPullPullUp = 0x11,
      outputPushPullPullDown = 0x12,
      outputOpenDrain = 0x14,
      outputOpenDrainPullUp = 0x15,
      outputOpenDrainPullDown = 0x16,
      outputAlternatePushPull = 0x20,
      outputAlternatePushPullPullUp = 0x21,
      outputAlternatePushPullPullDown = 0x22,
      outputAlternateOpenDrain = 0x24,
      outputAlternateOpenDrainPullUp = 0x25,
      outputAlternateOpenDrainPullDown = 0x26,
      analog = 0x03
    };

    enum Constants:uint8_t 
    {
      maxNrOfPins = 16,

      gpioModeOut = 0x01,
      gpioModeAF = 0x02,
      gpioModeMask = 0x03,
      gpioModeShift = 4,
      gpioTypeShift = 2
    };

    enum Ports
    {
      A = RCC_IOPENR_GPIOAEN,
      B = RCC_IOPENR_GPIOBEN,
      C = RCC_IOPENR_GPIOCEN
    };

    enum AltFuncs:uint16_t
    {
      AF0 = 0x0000,
      AF1 = 0x0001, AF2, AF3, AF4, AF5, AF6, AF7};
    Gpio(){}//Default empty Constructor
    Gpio(Modes mode, Ports port, uint16_t pin); //Default Constructor for regular GPIO
    Gpio(Modes mode, AltFuncs alt_Func, Ports port, uint16_t pin); //Default Constructor for GPIO with Alternate Function

    uint8_t altFunc;
    GPIO_TypeDef * gpio;
    uint8_t mode;
    uint16_t pin;
    uint32_t port;    
    
    void initalize_Gpio();
    void set_High();
    void set_Low();
    uint8_t read_InputValue();

  private:

};


#endif //GPIO_H_