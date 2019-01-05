/*
*/
#include "sccb.h"
#include "main.h"
#include "delay.h"
/*
#include "stm32f10x_lib.h"
*/

/*
* Initialize SCCB GPIO
*/
void SCCB_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level: GPIOB_Pin[5] = SCCB_SCL_Pin : High
   *                                 GPIOB_Pin[6] = SCCB_SDA_Pin : High
   * */
  HAL_GPIO_WritePin(GPIOB, SCCB_SCL_Pin|SCCB_SDA_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins: GPIOB_Pin[5] = SCCB_SCL_Pin
   *                     GPIOB_Pin[6] = SCCB_SDA_Pin
   * */
  GPIO_InitStruct.Pin = SCCB_SCL_Pin|SCCB_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/** Configure SCCB_SDA_Pin to IMPUT mode
*/
void SCCB_SID_GPIO_INPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
//  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins: GPIOB_Pin[6] = SCCB_SDA_Pin
   * */
  GPIO_InitStruct.Pin = SCCB_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;  // 上拉 若无外部上拉电阻 必须内部有上拉
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/** Configure SCCB_SDA_Pin to OUTPUT mode
*/
void SCCB_SID_GPIO_OUTPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
//  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins: GPIOB_Pin[6] = SCCB_SDA_Pin
   * */
  GPIO_InitStruct.Pin = SCCB_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/*
-----------------------------------------------
功能: start命令,SCCB的起始信号
参数: 无
返回值: 无
-----------------------------------------------
*/
void startSCCB(void)
{
    SCCB_SID_H();     //数据线高电平
    Delay(50);

    SCCB_SIC_H();	   //在时钟线高的时候数据线由高至低
    Delay(50);

    SCCB_SID_L();
    Delay(50);

    SCCB_SIC_L();	 //数据线恢复低电平，单操作函数必要
    Delay(50);
}
/*
-----------------------------------------------
   功能: stop命令,SCCB的停止信号
   参数: 无
 返回值: 无
-----------------------------------------------
*/
void stopSCCB(void)
{
    SCCB_SID_L();
    Delay(50);

    SCCB_SIC_H();
    Delay(50);


    SCCB_SID_H();
    Delay(50);
}

/*
-----------------------------------------------
   功能: noAck,用于连续读取中的最后一个结束周期
   参数: 无
 返回值: 无
-----------------------------------------------
*/
void noAck(void)
{
	SCCB_SID_H();
	Delay(50);

	SCCB_SIC_H();
	Delay(50);

	SCCB_SIC_L();
	Delay(50);

	SCCB_SID_L();
	Delay(50);
}

/*
-----------------------------------------------
   功能: 写入一个字节的数据到SCCB
   参数: 写入数据
 返回值: 发送成功返回1，发送失败返回0
-----------------------------------------------
*/
unsigned char SCCBwriteByte(unsigned char m_data)
{
	unsigned char j,tem;

	for(j=0; j<8; j++) //循环8次发送数据
	{
		if((m_data<<j)&0x80)
		{
			SCCB_SID_H();
		}
		else
		{
			SCCB_SID_L();
		}
		Delay(50);
		SCCB_SIC_H();
		Delay(50);
		SCCB_SIC_L();
		Delay(2);
	}
	//Delay(50);
	SCCB_SID_IN;/*设置SDA为输入*/
	Delay(10);
	SCCB_SIC_H();
	Delay(80);
	if(SCCB_SID_STATE)
		{tem=0;}   //SDA=1发送失败，返回0}
	else
		{tem=1;}   //SDA=0发送成功，返回1
	SCCB_SIC_L();
	Delay(50);
    SCCB_SID_OUT;/*设置SDA为输出*/

	return (tem);
}

/*
-----------------------------------------------
   功能: 一个字节数据读取并且返回
   参数: 无
 返回值: 读取到的数据
-----------------------------------------------
*/
unsigned char SCCBreadByte(void)
{
	unsigned char read,j;
	read=0x00;

	SCCB_SID_IN;/*设置SDA为输入*/
	Delay(50);
	for(j=8; j>0; j--) //循环8次接收数据
	{
		//Delay(100);
		SCCB_SIC_H();
		Delay(50);
		read=read<<1;
		if(SCCB_SID_STATE)
		{
			read=read+1;
		}
		SCCB_SIC_L();
		Delay(50);
	}
        SCCB_SID_OUT;/*设置SDA为输出*/
	return(read);
}
