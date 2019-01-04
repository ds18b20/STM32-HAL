/**
  ******************************************************************************
  * File Name          : led.h
  * Description        : This file contains functions for controlling LEDs 
  ******************************************************************************
  * COPYRIGHT(c) 2019 ds18b20
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "led.h"

/* time */
int time = 0;

/* simple delay function */
void Delay(__IO unsigned int nCount)
{
  for(; nCount != 0; nCount--);
} 
/* LED ON/OFF one time */
void LED1_Loop(int ON_Time,int OFF_Time)
{
  LED1_ON();
  Delay(ON_Time);
  LED1_OFF();
  Delay(OFF_Time);
}
/*LED breath one loop*/
void LED1_Breath(void)
{
  /*fade*/
  time = MAX_TIME;
  while(1)
  {
    time -= SPEED;
    LED1_Loop(time,MAX_TIME-time);
    if(time < SPEED)
      break;
  }
  /*grow*/
  time = MAX_TIME;
  while(1)
  {
    time -= SPEED;
    LED1_Loop(MAX_TIME-time,time);
    if(time < SPEED)
      break;
  }
}

/************************ (C) COPYRIGHT ds18b20 *****END OF FILE****/
