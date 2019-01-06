#include "main.h"
#include "delay.h"

/* simple delay */
void Delay(unsigned long i)
{
  while(i--)
  {
    asm("NOP");
  }
}

uint32_t Get_Us(void)
{
  uint32_t usTicks = HAL_RCC_GetSysClockFreq() / 1000000;
  register uint32_t ms, cycle_cnt;
  do{
    ms = HAL_GetTick();
    cycle_cnt = SysTick->VAL;
  }while (ms != HAL_GetTick());
  return (ms * 1000) + (usTicks * 1000 - cycle_cnt) / usTicks;
}

void Delay_Us(uint16_t micros)
{
  uint32_t start = Get_Us();
  while(Get_Us()-start < (uint32_t) micros)
  {
    asm("NOP");
  }
}

