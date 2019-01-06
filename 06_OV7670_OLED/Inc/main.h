/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY4_EXTI_Pin GPIO_PIN_2
#define KEY4_EXTI_GPIO_Port GPIOE
#define KEY4_EXTI_EXTI_IRQn EXTI2_IRQn
#define KEY3_Pin GPIO_PIN_3
#define KEY3_GPIO_Port GPIOE
#define KEY2_Pin GPIO_PIN_4
#define KEY2_GPIO_Port GPIOE
#define KEY1_Pin GPIO_PIN_5
#define KEY1_GPIO_Port GPIOE
#define FIFO_WEN_Pin GPIO_PIN_2
#define FIFO_WEN_GPIO_Port GPIOC
#define FIFO_RRST_Pin GPIO_PIN_3
#define FIFO_RRST_GPIO_Port GPIOC
#define FIFO_OE_Pin GPIO_PIN_4
#define FIFO_OE_GPIO_Port GPIOC
#define FIFO_RCK_Pin GPIO_PIN_5
#define FIFO_RCK_GPIO_Port GPIOC
#define BIT2_Pin GPIO_PIN_10
#define BIT2_GPIO_Port GPIOB
#define BIT3_Pin GPIO_PIN_11
#define BIT3_GPIO_Port GPIOB
#define BIT4_Pin GPIO_PIN_12
#define BIT4_GPIO_Port GPIOB
#define BIT5_Pin GPIO_PIN_13
#define BIT5_GPIO_Port GPIOB
#define BIT6_Pin GPIO_PIN_14
#define BIT6_GPIO_Port GPIOB
#define BIT7_Pin GPIO_PIN_15
#define BIT7_GPIO_Port GPIOB
#define CS0_Pin GPIO_PIN_8
#define CS0_GPIO_Port GPIOD
#define SDO_Pin GPIO_PIN_9
#define SDO_GPIO_Port GPIOD
#define SDI_Pin GPIO_PIN_10
#define SDI_GPIO_Port GPIOD
#define CLK_Pin GPIO_PIN_11
#define CLK_GPIO_Port GPIOD
#define LED1_Pin GPIO_PIN_6
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_7
#define LED2_GPIO_Port GPIOC
#define CS1_Pin GPIO_PIN_7
#define CS1_GPIO_Port GPIOD
#define SCCB_SCL_Pin GPIO_PIN_5
#define SCCB_SCL_GPIO_Port GPIOB
#define SCCB_SDA_Pin GPIO_PIN_6
#define SCCB_SDA_GPIO_Port GPIOB
#define VSYNC_EXTI_Pin GPIO_PIN_7
#define VSYNC_EXTI_GPIO_Port GPIOB
#define VSYNC_EXTI_EXTI_IRQn EXTI9_5_IRQn
#define BIT0_Pin GPIO_PIN_8
#define BIT0_GPIO_Port GPIOB
#define BIT1_Pin GPIO_PIN_9
#define BIT1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
