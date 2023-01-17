#include "stm32l0xx.h"

#ifndef PWM_H_
#define PWM_H_

class Pwm
{
  public:

    enum Modes : uint8_t {Mode1 = 0b110, Mode2 = 0b111};
    enum Directions : bool {Up = 0, Down = 1};
    enum Channels : uint8_t {CH1 = 1, CH2, CH3, CH4};
    enum Alignments : uint8_t {Edge = 0x0, Center1 = 0x1, Center2, Center3};


    
    void get_info();
    //void set_dutyCycle(uint8_t dutyCycle);
    void set_PWM(uint16_t compareValue);
    void set_PWM_period(uint16_t period);
    bool initialize_Timers();

    Pwm(){}; //Default Constructor
    Pwm(TIM_TypeDef *timerReg, Channels channel, Modes mode, Directions direction, Alignments alignment, uint16_t period);
    
  private:

    TIM_TypeDef * timerReg;
        
    uint16_t period;
    uint8_t mode;
    uint8_t dutyCycle;
    uint16_t count;
    uint8_t direction;
    uint8_t alignment;
    uint8_t channel;

};

#endif //PWM_H_