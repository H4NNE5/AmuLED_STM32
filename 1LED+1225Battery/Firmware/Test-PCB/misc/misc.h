#ifndef MISC_H_
#define MISC_H_

//#include <__cross_studio_io.h>
#include <stdlib.h>
#include <stdint.h>
#include <cstddef>
#include <stdio.h>

#define NUMOFLEDPWMS 3

constexpr uint16_t PWM_MAX = 65535; 

  enum{
      RED = 0, 
      GREEN,
      BLUE
    };



//#define True 1
//#define False 0

#endif //MISC_H_