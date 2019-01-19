
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __serial_H
#define __serial_H

void Debug_Print(UART_HandleTypeDef *huart, char _out[]);
void Debug_Print_ln(UART_HandleTypeDef *huart, char _out[]);

void Uart_Send_Demo_Strings();
void Uart_Receive_Demo_Strings();
void Uart_Send_Demo_ASCII_Art();

#endif /*__serial_H */
