#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "misc.h"

void add_Semaphore();

class Semaphore
{
  public:
    
    uint16_t value;
    uint16_t countValue;
    uint16_t frequency;
    
    Semaphore(){};
    Semaphore(uint16_t frequency);

    uint16_t get(void);
    uint16_t get_And_Reset(void);
    uint16_t get_And_Reset_If_Larger(uint16_t comp);
    void update(void);
};


class SemaphoreNode //class for linked list of semaphores
{
  public:

    SemaphoreNode * next;
    Semaphore newSema;
    
    SemaphoreNode();

    Semaphore * add_Semaphore(uint16_t frequency);
    void show_Semaphores();
};


#endif /* SEMAPHORE_H_ */