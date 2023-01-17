#include "pwm.h"
//#include "hardware.h"
#include "misc.h"


Pwm::Pwm(TIM_TypeDef *timerReg, Channels channel, Modes mode, Directions direction, Alignments alignment, uint16_t period)
        :timerReg(timerReg), channel(channel), mode(mode), direction(direction), alignment(alignment), period(period)
{
  this->initialize_Timers();
  
}

bool Pwm::initialize_Timers()
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //Enable Timer

  this->timerReg->CR1 |= TIM_CR1_CEN | (this->alignment << TIM_CR1_CMS_Pos); //Enable Counter and set alignment mode bits

  this->timerReg->CCER |= TIM_CCER_CC1E |TIM_CCER_CC2E | TIM_CCER_CC3E;

  //this->timerReg->PSC = 0x00FF;

  this->timerReg->ARR = this->period;

  /* Set PWM Mode */
  uint8_t ccmrxShift = 8 * ((this->channel - 1) % 2);
  if( this->channel <= 2) //Set PWM Modes
    this->timerReg->CCMR1 |= this->mode << TIM_CCMR1_OC1M_Pos << ccmrxShift; //shift left for odd channels and shift left *8 for even ones
  else if( this->channel > 2 )
    this->timerReg->CCMR2 |= this->mode << TIM_CCMR2_OC3M_Pos << ccmrxShift;

  /* Set Compare Polarity */
  this->timerReg->CCER |= TIM_CCER_CC1P << 4*(this->channel-1);

  return SUCCESS;
}

void Pwm::set_PWM_period(uint16_t value)
{
  this->timerReg->ARR = value;
}

//void Pwm::set_dutyCycle(uint8_t dC)
void Pwm::set_PWM(uint16_t compareValue)
{
  //this->dutyCycle = dC;
  this->dutyCycle = compareValue/PWM_MAX;

  static uint32_t pulse = 0; //must be 32 bit because gets bigger than 16 bit
  //pulse = (this->dutyCycle * (this->timerReg->ARR + 1)) / 100; //real percent

  pulse = (compareValue * (this->timerReg->ARR + 1))/PWM_MAX;

  switch (this->channel)
  {
    case 1:
      this->timerReg->CCR1 = pulse;
      break;
    case 2:
      this->timerReg->CCR2 = pulse;
      break;
    case 3:
      this->timerReg->CCR3 = pulse;
      break;
    case 4:
      this->timerReg->CCR4 = pulse;
      break;
  }
}

void Pwm::get_info()
{
   static uint16_t value = 0;
    switch (this->channel)
  {
    case 1:
      value = this->timerReg->CCR1;
      break;
    case 2:
      value = this->timerReg->CCR2;
      break;
    case 3:
      value = this->timerReg->CCR3;
      break;
    case 4:
      value = this->timerReg->CCR4;
      break;
  }
  printf("TimerRegStuff: \nARR: %u\nCCR: %u\nDuty Cycle: %u\n", this->timerReg->ARR, value, this->dutyCycle);


 }
