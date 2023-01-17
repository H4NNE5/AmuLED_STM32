#include "semaphore.h"

Semaphore::Semaphore(uint16_t frequency)
{
  this->value = 0;
  this->countValue = 0;
  this->frequency = frequency;
}

uint16_t Semaphore::get(void)
{
  return this->value;
}

uint16_t Semaphore::get_And_Reset(void)
{
  if( this->value ){
    uint16_t retVal = this->value;
    this->value = 0;
    return retVal;
  }
  else
    return 0;
}

uint16_t Semaphore::get_And_Reset_If_Larger(uint16_t comp)
{
  if( this->value > comp ){
    return this->get_And_Reset();
  }
  else
    return 0;
}

void Semaphore::update(void)
{
  //uint32_t countValue = 0;
  
  if( this->countValue > this->frequency-1 ){
    this->value++;
    this->countValue = 0;
  }

  this->countValue++;
}


/**
 * Semaphore List
 */

SemaphoreNode::SemaphoreNode()
{
  this->newSema = NULL;
  this->next = NULL;
}

Semaphore* SemaphoreNode::add_Semaphore(uint16_t frequency)
{
  SemaphoreNode * myList = this;
  printf("\nmyList %p", myList);
  printf("\nmyList->newSema %p", myList->newSema);
  printf("\nmyList->next %p", myList->next);

  //if( (bool)myList->next)
  //  printf("list is existing");
  while(myList->next != NULL){
    myList = myList->next;
  }
  //do{   
  //    if (myList->next)      {
        //myList = myList->next;
  //    }
  //}while(myList->next);

    SemaphoreNode * newElement = new SemaphoreNode();

    newElement->newSema.frequency = frequency;
    myList->next = NULL;
    if( myList == NULL){
      myList = newElement;
    }
  return &myList->newSema;
}

void SemaphoreNode::show_Semaphores()
{

}

