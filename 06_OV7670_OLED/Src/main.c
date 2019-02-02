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
#include "serial.h"
#include <string.h>
#include <assert.h>

#include "color_bar_test.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define DEBUG
#define COLOR_MODE  1
#define MONO_MODE   0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
unsigned int Vsync=0;
unsigned int a, b, c_data;
char str[]="0x0000";
char str_gray[]="0x00";
int i;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void OV7670_SendOneFrame(unsigned int output_h, unsigned int output_w)
{
  for(a=0; a<output_h; a++)            // rows
  {
    for(b=0; b<output_w; b++)          // columns
    {
      FIFO_RCK_L();
      asm("NOP");asm("NOP");
      c_data=GPIOB->IDR&0xff00;        // read high-8 bits of 1 byte
      FIFO_RCK_H();
      asm("NOP");asm("NOP");
      FIFO_RCK_L();
      asm("NOP");asm("NOP");
      c_data|=(GPIOB->IDR>>8)&0x00ff;  // read low-8 bits of 1 byte
      FIFO_RCK_H();
      // LCD_WriteRAM(c_data);         // write RGB565 data to TFT GRAM
      i = sprintf(str, "%x", c_data);
      Debug_Print_String(&huart1, str);
      Debug_Print_String(&huart1, ",");
    }
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
  }
}

void OV7670_SendOneFrame_Zip(unsigned int output_h, unsigned int output_w)
{
  unsigned int row, col, i;
  uint16_t tmp;
  uint16_t sum28[28][3];
  uint16_t r_data = 0;
  uint16_t g_data = 0;
  uint16_t b_data = 0;
  assert(0==output_h%8);
  assert(0==output_w%8);
  for(row=0; row<output_h; row++)      // rows
  {
	for(col=0; col<output_w; col++)    // columns
    {
      FIFO_RCK_L();
      asm("NOP");asm("NOP");
      c_data=GPIOB->IDR&0xff00;        // read high-8 bits of 1 byte
      FIFO_RCK_H();
      asm("NOP");asm("NOP");
      FIFO_RCK_L();
      asm("NOP");asm("NOP");
      c_data|=(GPIOB->IDR>>8)&0x00ff;  // read low-8 bits of 1 byte
      r_data = c_data>>11;
      g_data = (c_data>>5)&0x3F;
      b_data = c_data&0x1F;
      FIFO_RCK_H();
      // LCD_WriteRAM(c_data);         // write RGB565 data to TFT GRAM

      sum28[col/8][0] += r_data;
      sum28[col/8][1] += g_data;
      sum28[col/8][2] += b_data;
    }

    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);

    if(0==(row+1)%8)
    {
		for(i=0;i<28;i++)
		{
			tmp = ((sum28[i][0]>>6)<<11)|((sum28[i][1]>>6)<<5)|(sum28[i][2]>>6);
			sprintf(str, "%x", tmp);
//			sprintf(str, "%x", i);
			Debug_Print_String(&huart1, str);
			Debug_Print_String(&huart1, ",");
		}

		for(i=0;i<28;i++)
		{
			sum28[i][0] = 0;
			sum28[i][1] = 0;
			sum28[i][2] = 0;
		}
    }
  }
}

void OV7670_SendOneFrame_Gray(void)   // need to check!
{
  for(a=0; a<240; a++)  // 240 rows
  {
    for(b=0; b<320; b++)  // 320 columns
    {
      FIFO_RCK_L();
      asm("NOP");asm("NOP");
//      c_data=(GPIOB->IDR>>8)&0xff;  // read high-4 bits of 2 bytes GPIO bits
      c_data=(GPIOB->IDR>>12)&0x0f;   // read high-4 bits of 2 bytes GPIO bits
      FIFO_RCK_H();
      asm("NOP");asm("NOP");
      // LCD_WriteRAM(c_data);        // write RGB565 data to TFT GRAM
      i = sprintf(str_gray, "%x", c_data);
      Debug_Print_String(&huart1, str_gray);
      Debug_Print_String(&huart1, ",");
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
  unsigned int mode = COLOR_MODE;
  unsigned int output_w = 224;
  unsigned int output_h = 224;
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
  OLED_Clear();

  Debug_Print_String_ln(&huart1, "Init ov7670...");

  FIFO_OE_L();
  FIFO_WEN_H();

  if(0==Sensor_init(mode))  // Init CMOS Sensor
  {
	Debug_Print_String_ln(&huart1, "Init out");
	return 0;
  }
  if(0==OV7670_config_window(10, 184, output_h, output_w))
  {
    Debug_Print_String_ln(&huart1, "conf out");
    return 0;
  }
//  ov7670_set_hw(0,240,0,240);
  Debug_Print_String_ln(&huart1, "Init ov7670 success!");
  Vsync = 0;
  ////////////////////////////////////////
  FIFO_RRST_L();
  FIFO_RCK_L();
  FIFO_RCK_H();
  FIFO_RCK_L();
  FIFO_RRST_H();
  FIFO_RCK_H();
  Delay_Us(50);
  ////////////////////////////////////////
//  DispStr_GB2312(20,0,"ÄãºÃ");
//  OLED_DispStr(0,0, (uint8_t*)"Size 8X6 Test", FONT_8X6);
  OLED_DispStr(0,0, (uint8_t *)"Sample Display", FONT_16X8);
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
	  Debug_Print_String(&huart1, "\r");
      #else
	  if(Vsync==2)
	  {
	    FIFO_RRST_L();
	    FIFO_RCK_L();
	    FIFO_RCK_H();
	    FIFO_RCK_L();
	    FIFO_RRST_H();
	    FIFO_RCK_H();
	    if(mode)
	      {
//	    	OV7670_SendOneFrame(output_h, output_w);
	    	OV7670_SendOneFrame_Zip(output_h, output_w);
	      }
	    else
	      {OV7670_SendOneFrame_Gray();}
	    Debug_Print_String(&huart1, "\r");
	    Vsync = 0;
	  }
	  #endif
      }
    }
  }
  return 1;
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
