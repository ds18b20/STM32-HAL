/*
 *
*/
#ifndef _sensor_H
#define _sensor_H

#include "sccb.h"
#include "main.h"

#define OV7670_REG_NUM   120

#define FIFO_OE_H()    HAL_GPIO_WritePin(GPIOC, FIFO_OE_Pin, GPIO_PIN_SET)
#define FIFO_OE_L()    HAL_GPIO_WritePin(GPIOC, FIFO_OE_Pin, GPIO_PIN_RESET)

#define FIFO_RRST_H()  HAL_GPIO_WritePin(GPIOC, FIFO_RRST_Pin, GPIO_PIN_SET)
#define FIFO_RRST_L()  HAL_GPIO_WritePin(GPIOC, FIFO_RRST_Pin, GPIO_PIN_RESET)

#define FIFO_RCK_H()   HAL_GPIO_WritePin(GPIOC, FIFO_RCK_Pin, GPIO_PIN_SET)
#define FIFO_RCK_L()   HAL_GPIO_WritePin(GPIOC, FIFO_RCK_Pin, GPIO_PIN_RESET)

#define FIFO_WEN_H()   HAL_GPIO_WritePin(GPIOC, FIFO_WEN_Pin, GPIO_PIN_SET)
#define FIFO_WEN_L()   HAL_GPIO_WritePin(GPIOC, FIFO_WEN_Pin, GPIO_PIN_RESET)

/* crystal oscillator is included on CF7670-V3 board */
//void XCLK_init_ON(void);  // use STM32 MCO to supply CLOCK for ov7670 sensor

unsigned char wr_Sensor_Reg(unsigned char regID, unsigned char regDat);
unsigned char rd_Sensor_Reg(unsigned char regID, unsigned char *regDat);

unsigned char Sensor_init(unsigned char color_mode);
void Sensor_GPIO_Init(void);
void Sensor_EXTI_Config(void);  // VSYNC interruption
void Sensor_Interrupts_Config(void);

#endif /* _sensor_H */
