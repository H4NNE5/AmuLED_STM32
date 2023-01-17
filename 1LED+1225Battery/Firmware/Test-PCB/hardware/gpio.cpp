#include "gpio.h"
#include "misc.h"
#include "hardware.h"


Gpio::Gpio(Modes mode, Ports port, uint16_t pin):mode(mode), port(port), pin(pin)//Default Constructor for regular GPIO
{
  //this->altFunc = NULL;
  this->initalize_Gpio();
}

Gpio::Gpio(Modes mode, AltFuncs altFunc, Ports port, uint16_t pin):mode(mode), altFunc(altFunc), port(port), pin(pin)
{
  this->initalize_Gpio();
}//Default Constructor for GPIO with Alternate Function



void Gpio::initalize_Gpio()
{   
  if( !(RCC->IOPENR & this->port) ) //Enable clock for this port if it's not already enabled; maybe static_cast<uint32_t>() needed?
    RCC->IOPENR |= this->port;
  /* determine the right register for the given port */
  switch (this->port)
  {
    case Gpio::Ports::A:
      this->gpio = GPIOA;
      break;
    case Gpio::Ports::B:
      this->gpio = GPIOB;
      break;
    case Gpio::Ports::C:
      this->gpio = GPIOC;
      break;
    default:
      this->gpio = NULL;
      break;
   }

  //this->gpio->MODER &= ~(GPIO_MODER_MODE0 << 2* this->pin); //Set mode bits to zero
  //this->gpio->MODER |= this->mode << 2* this->pin; //set mode bits shifted by pin number
  
  if( altFunc != NULL){ //check if Alternate Function is set
    if( this->pin < 8)
      this->gpio->AFR[0] |= this->altFunc << (this->pin * 4); //Set bits in AFR LOW
    if( this->pin > 7)
      this->gpio->AFR[1] |= this->altFunc << ((this->pin - 8) * 4); //Set bits in AFR HIGH; pin numbers have to be decremented by 8
  }

    uint8_t stmMode = (this->mode >> Constants::gpioModeShift) & Constants::gpioModeMask;
    uint8_t stmType = (this->mode >> Constants::gpioTypeShift) & Constants::gpioModeMask; 
    uint8_t stmPull = this->mode & Constants::gpioModeMask;
    uint8_t pinOffset = this->pin * 2; // offset in some registers for the pin
    
    /* if pin is to be configured in output mode, speed+output_type have to be set */
      if ((stmMode == Constants::gpioModeOut) || (stmMode == Constants::gpioModeAF))
      {
        /* set the speed */
        //this->gpio->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEED0 << pinOffset);
        //this->gpio->OSPEEDR |= (this->speed << pinOffset);

        /* set the output type */
        this->gpio->OTYPER &= ~(GPIO_OTYPER_OT_0 << this->pin);
        this->gpio->OTYPER |= (stmType << this->pin);
      }

      /* set the mode */
      this->gpio->MODER &= ~(GPIO_MODER_MODE0 << pinOffset);
      this->gpio->MODER |= (stmMode << pinOffset);

      /* set Pull-up/Pull-Down */
      this->gpio->PUPDR &= ~(GPIO_PUPDR_PUPD0 << pinOffset);

      this->gpio->PUPDR |= (stmPull << pinOffset);
  
} //initalize_Gpio

void Gpio::set_High()
{
  this->gpio->BSRR |= 1 << this->pin; //Set bits are in [15:0] of BSRR register 
} //set_High

void Gpio::set_Low()
{
  this->gpio->BSRR |= 1 << this->pin << 16; //Reset bits are in [31:16] of BSRR register
} //set_High

uint8_t Gpio::read_InputValue()
{
  if( this->gpio->IDR & (1 << this->pin) )
    return 1;
  else
    return 0;
  //return (this->gpio->IDR & (1 << this->pin));
}