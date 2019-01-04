/**
  ******************************************************************************
  * File Name          : led.h
  * Description        : This file contains functions for controlling LEDs 
  ******************************************************************************
  * COPYRIGHT(c) 2019 ds18b20
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __led_H
#define __led_H
#ifdef __cplusplus
 extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define MAX_TIME   (0X20000)
#define MIN_TIME   (0)
#define SPEED      (2000)  // breath speed
#define LED1_ON()  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET)
#define LED1_OFF() HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)
#define LED2_ON()  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET)
#define LED2_OFF() HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET)
#define LED3_ON()  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET)
#define LED3_OFF() HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET)
#define LED4_ON()  HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET)
#define LED4_OFF() HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET)

void Delay(__IO unsigned int nCount);
void LED1_Loop(int,int);
void LED1_Breath(void);


#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/************************ (C) COPYRIGHT ds18b20 *****END OF FILE****/
