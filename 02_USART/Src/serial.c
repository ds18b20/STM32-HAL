/* Ref:
 * https://simonmartin.ch/resources/stm32/dl/STM32%20Tutorial%2003%20-%20UART%20Communication%20using%20HAL%20(and%20FreeRTOS).pdf
 * */
#include "usart.h"
#include "serial.h"
#include <string.h>

extern UART_HandleTypeDef huart1;


/* print a string */
void Debug_Print(UART_HandleTypeDef *huart, char _out[])
{
  uint32_t time_out = 1000 * strlen(_out) * (8 + 1) / huart1.Init.BaudRate;  // round to N Ms
  HAL_UART_Transmit(huart, (uint8_t *) _out, strlen(_out), time_out + 1);
}

/* print a string followed by a 0x0A 0x0D */
void Debug_Print_ln(UART_HandleTypeDef *huart, char _out[])
{
  Debug_Print(huart, _out);
  Debug_Print(huart, "\r\n");
}

/* send strings Demo */
void Uart_Send_Demo_Strings()
{
  Debug_Print_ln(&huart1, "Hello world!");
  Debug_Print_ln(&huart1, "This is a UART test.");
}

/* send SCII Art Demo */
void Uart_Send_Demo_ASCII_Art()
{
  Debug_Print_ln(&huart1, "                     _|_");
  Debug_Print_ln(&huart1, "                 --o-(_)-o--");
  Debug_Print(&huart1, "\r\n");
  Debug_Print_ln(&huart1, "       --|--");
  Debug_Print_ln(&huart1, "*--o--o-(_)-o--o--*");
}

/* receive strings Demo */
void Uart_Receive_Demo_Strings()
{
  Debug_Print_ln(&huart1, "Type something:");
  char in[8];
  HAL_UART_Receive(&huart1, (uint8_t *)in, 8, 1000);  // time_out is set to 1000 Ms
  Debug_Print(&huart1, "\n");
  HAL_UART_Transmit(&huart1, (uint8_t *)in, 8, 1);
  Debug_Print(&huart1, "\n");
}
