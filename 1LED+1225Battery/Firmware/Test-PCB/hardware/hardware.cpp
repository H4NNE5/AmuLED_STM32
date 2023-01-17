#include "hardware.h"
#include "misc.h"
#include "Pushbutton.h"


/* Delay value : short one is used for the error coding, long one (~1s) in case 
   of no error or between two bursts */
#define SHORT_DELAY 200
#define LONG_DELAY 1000


//extern uint32_t SystemCoreClock;
//Gpio ledPins[3];// = {pinLedRed, pinLedGreen, pinLedBlue};


  
int Hardware::init()
{ 
  this->init_power();
  this->init_clocks();
  this->init_gpios();
  this->init_PWMs();

  this->semaArray[0] = new Semaphore(2); //set the period rather than frequency like in cntxlib; 2x0,5ms = 1ms
  this->semaDebounce = this->semaArray[0];

  this->semaArray[1] = new Semaphore(50); //25ms base
  this->semaDelay = this->semaArray[1];

  this->semaArray[2] = new Semaphore(200); //100ms base
  this->semaExtendedPress = this->semaArray[2];
  
  //SemaphoreNode * semaphoreHead = NULL;
  //SemaphoreNode * semaphores = NULL;  

  //this->semaDebounce = semaphores->add_Semaphore(1000);
  //semaphoreHead = semaphores;

  //this->semaDelay = semaphores->add_Semaphore(100);
  //this->semaExtendedPress = semaphores->add_Semaphore(10);

  //printf("\n semaphorepointer %p", semaphores);

  return SUCCESS;
}

  
int Hardware::init_power()
{
  RCC->APB1ENR |= (RCC_APB1ENR_PWREN); //Enable Clock for APB1
  //PWR->CR |= 0b10 << PWR_CR_VOS_Pos; //Voltage Scaling Range 2; shift "10" to bit 11 of register
  //todo: Enable Gpio, DMA, ADC

  return SUCCESS;
}


int Hardware::init_gpios()
{
  /* Gpio instantiation for Boost PWM */
  //Gpio pinNwmNFet (Gpio::Modes::outputPushPull, Gpio::AltFuncs::AF0, Gpio::Ports::A, 10);  
  //Gpio pinPwmNFet (Gpio::Modes::outputPushPull, Gpio::AltFuncs::AF5, Gpio::Ports::A, 9);

  this->pinPushbuttonBig = new Gpio(Gpio::Modes::inputPullUp, Gpio::Ports::A, 7);
  //this->pinPushButtonSmall = new Gpio(Gpio::Modes::inputPullUp, Gpio::Ports::A, 8); 

  /* Gpio instantiation for LED PWM */
  this->gpiosLeds[RED] = new Gpio(Gpio::Modes::outputAlternatePushPull, Gpio::AltFuncs::AF2, Gpio::Ports::A, 1); //RED
  this->gpiosLeds[GREEN] = new Gpio(Gpio::Modes::outputAlternatePushPull, Gpio::AltFuncs::AF5, Gpio::Ports::A, 5); //GREEN
  this->gpiosLeds[BLUE] = new Gpio(Gpio::Modes::outputAlternatePushPull, Gpio::AltFuncs::AF2, Gpio::Ports::A, 2); //BLUE


  
  //ledPins[0] = pinLedRed;
  
  //this->gpiosLeds[0]->set_High();  
  //pinLedGreen.set_High();
  //pinLedBlue.set_High();

  //pinLedRed.set_Low();
  //pinLedGreen.set_Low();
  //pinLedBlue.set_Low();

  return SUCCESS;
}


int Hardware::init_PWMs()
{
  /* PWM at A1 for Red LED */
  this->pwmsLeds[RED] = new Pwm(TIM2, Pwm::Channels::CH2, Pwm::Modes::Mode1, Pwm::Directions::Up, Pwm::Alignments::Edge, 30000);

  /* PWM at A5 for Green LED */
  this->pwmsLeds[GREEN] = new Pwm(TIM2, Pwm::Channels::CH1, Pwm::Modes::Mode1, Pwm::Directions::Up, Pwm::Alignments::Edge, 30000);

  /* PWM at A2 for Blue LED */
  this->pwmsLeds[BLUE] = new Pwm(TIM2, Pwm::Channels::CH3, Pwm::Modes::Mode1, Pwm::Directions::Up, Pwm::Alignments::Edge, 30000);
//  pwmsLEDs[0] = new Pwm();
  
  //PWM PWM_CatR();
  //PWM PWM_CatG();
  //PWM PWM_CatB();

  
  
  return SUCCESS;
}

int Hardware::init_clocks()
{  
  this->clocks->hsiDiv = true;
  this->clocks->ahbPrescaler = (uint32_t)ClockSettings::AhbClockPrescaler::div2;
  this->clocks->apb1Prescaler = (uint32_t)ClockSettings::ApbClockPrescaler::div4;
  this->clocks->apb2Prescaler = 1;

  //this->clocks->sysClock = (uint32_t) (HSI_CLOCK >> 2* (uint8_t)this->clocks->hsiDiv); //shift right 2 times if hsiDiv is set -->division by 4
  //this->clocks->highClock = this->clocks->sysClock / this->clocks->ahbPrescaler;
  //this->clocks.apb1Clock = this->clocks->highClock / this->clocks->apb1Prescaler;
  //this->clocks.apb2Clock = this->clocks->highClock / this->clocks.apb2Prescaler;

  /*uint32_t tick;
  uint32_t tickstart;
  tickstart = tick;*/
  RCC->CR |= RCC_CR_HSION | ((uint8_t)this->clocks->hsiDiv << RCC_CR_HSIDIVEN_Pos); //enable HSI and divide by 4 --> 4MHz
  while( (RCC->CR & (RCC_CR_HSIRDY | RCC_CR_HSIDIVF)) != (RCC_CR_HSIRDY | RCC_CR_HSIDIVF)){ //wait for HSI is ready
  //  if( (tick - tickstart) > HSI_TIMEOUT_VALUE )
  //    return FAIL;
  } 

  /*tickstart = tick;*/
  RCC->CFGR |=  RCC_CFGR_SW_HSI | 
                RCC_CFGR_HPRE_3 |
                (RCC_CFGR_PPRE1 & this->clocks->apb1Prescaler); //set HSI as sysclock source in sysclock MUX and AHB prescaler to 2 and APB1 prescaler to
                //as APB1 Prescaler is not 1 in this case, the according Timer clock for TIM2 is 2x the APB1 clock, in this case 1MHz
  while( (RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_HSI ){ //wait for HSI is selected
    //if( (tick - tickstart) > HSI_TIMEOUT_VALUE )
    //  return FAIL;
  }
  return SUCCESS;
} //init_clocks

void Hardware::get_clocks()
{
  printf("\nClocks:\nSysClock: %d\nHighClock: %d\nAPB1: %d\n", this->clocks->sysClock, this->clocks->highClock, this->clocks->apb1Clock); 
}

/******************************************************************************/
/*     Read back status of button                                             */
/******************************************************************************/
  bool Hardware::get_PushButton()
  {
    if(this->pinPushbuttonBig->read_InputValue() == 0)
      return PushButtonStates::pressed;
    else
      return PushButtonStates::released;
  }


