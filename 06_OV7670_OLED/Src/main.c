/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sensor.h"
#include "oled.h"
#include "delay.h"
#include <string.h>

#include "color_bar_test.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define DEBUG
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//extern unsigned int Vsync;
unsigned int Vsync=0;
unsigned int a, b, c_data;
char str[]="0x0000";
int i;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void OV7670_SendOneFrame(void)
{
  for(a=0; a<240; a++)  // 240 rows
  {
    for(b=0; b<320; b++)  // 320 columns
    {
      FIFO_RCK_L();
      asm("NOP");asm("NOP");
      c_data=GPIOB->IDR&0xff00;  // read high-8 bits of 1 byte
      FIFO_RCK_H();
      asm("NOP");asm("NOP");
      FIFO_RCK_L();
      asm("NOP");asm("NOP");
      c_data|=(GPIOB->IDR>>8)&0x00ff;  // read low-8 bits of 1 byte
      FIFO_RCK_H();
      // LCD_WriteRAM(c_data);  // write RGB565 data to TFT GRAM
      i = sprintf(str, "%x", c_data);
      HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen(str), 10);
      HAL_UART_Transmit(&huart1, (uint8_t *)",", strlen(","), 10);
    }
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
  }
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  HAL_UART_Transmit(&huart1, (uint8_t *)"Init...", strlen("Init..."), 10);

  FIFO_OE_L();
  FIFO_WEN_H();

  while(1!=Sensor_init()){}  // Init CMOS Sensor
  HAL_UART_Transmit(&huart1, (uint8_t *)"Init OK", strlen("Init OK"), 10);
  Vsync=0;
  ////////////////////////////////////////
  FIFO_RRST_L();
  FIFO_RCK_L();
  FIFO_RCK_H();
  FIFO_RCK_L();
  FIFO_RRST_H();
  FIFO_RCK_H();
  Delay(50);
  ////////////////////////////////////////
//  DispStr_GB2312(20,0,"¹þ¹þ");
  OLED_DispStr(0,0, (uint8_t*)"Size 8X6 Test", FONT_8X6);
  OLED_DispStr(0,6, (uint8_t*)"Size 16X8 Test", FONT_16X8);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      if(!HAL_GPIO_ReadPin(GPIOE, KEY1_Pin))  // 0
      {
        HAL_Delay(10);
        if(HAL_GPIO_ReadPin(GPIOE, KEY1_Pin))  // 1
        {
  	#ifdef DEBUG
  	  OV7670_SendOneFrame_ColorBar();
  	  HAL_UART_Transmit(&huart1, (uint8_t *)"\r", strlen("\r"), 10);
  	#else
  	  if(Vsync==2)
  	  {
  	    FIFO_RRST_L();
  	    FIFO_RCK_L();
  	    FIFO_RCK_H();
  	    FIFO_RCK_L();
  	    FIFO_RRST_H();
  	    FIFO_RCK_H();

  	    OV7670_SendOneFrame();
  	    HAL_UART_Transmit(&huart1, (uint8_t *)"\r", strlen("\r"), 10);
  	    Vsync=0;
            }
  	#endif
        }
      }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /*VSYNC EXTI line: 7*/
  if(__HAL_GPIO_EXTI_GET_FLAG(VSYNC_EXTI_Pin))
  {
    if(Vsync==0)
    {
      Vsync=1;
      FIFO_WEN_H();
    }
    else if(Vsync==1)
    {
      FIFO_WEN_L();
      Vsync=2;
      HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    }

  }
  /*KEY1 EXTI line: 5*/
  if(__HAL_GPIO_EXTI_GET_FLAG(KEY4_EXTI_Pin))
  {
    HAL_Delay(10);
    if(HAL_GPIO_ReadPin(KEY4_EXTI_GPIO_Port, KEY4_EXTI_Pin) == 0)  // if KEY1 is still PRESSED DOWN
    {
      HAL_UART_Transmit(&huart1, (uint8_t *)"KEY4\t", strlen("KEY4\t"), 10);
      /* ·­×ªLED0 */
      HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    }
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
