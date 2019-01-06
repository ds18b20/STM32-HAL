#include "oled.h"
#include "oledfont.h"  	 

void OLED_DispChinese(uint8_t x,uint8_t y,const uint8_t code[])
{
  uint8_t n;
  OLED_WR_Byte (0xb0+y,OLED_CMD);	//设置页地址（0~7）

  OLED_WR_Byte (((x&0xf0)>>4)|0x10,OLED_CMD);   //设置显示位置—列低地址
  OLED_WR_Byte (((x&0x0f)|0x01),OLED_CMD);   //设置显示位置—列高地址

  for(n=0;n<16;n++)
  OLED_WR_Byte(code[n],OLED_DATA);

  y++;
  OLED_WR_Byte (0xb0+y,OLED_CMD);    //设置页地址（0~7）
  OLED_WR_Byte (((x&0xf0)>>4)|0x10,OLED_CMD);   //设置显示位置—列低地址
  OLED_WR_Byte (((x&0x0f)|0x01),OLED_CMD);   //设置显示位置—列高地址
  for(n=0;n<16;n++)
  OLED_WR_Byte(code[n+16],OLED_DATA);
}

void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{
  uint8_t i;
  if(cmd)
    OLED_DC_Set();
  else
    OLED_DC_Clr();

  OLED_CS_Clr();

  for(i=0;i<8;i++)
  {
    OLED_SCLK_Clr();
    if(dat&0x80)
      OLED_SDIN_Set();
    else
      OLED_SDIN_Clr();

    OLED_SCLK_Set();
    dat<<=1;
  }
  OLED_CS_Set();
  OLED_DC_Set();
}

//开启OLED显示    
void OLED_Display_On(void)
{
  OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
  OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
  OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
  OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
  OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
  OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{
  uint8_t i,n;
  for(i=0;i<8;i++)
  {
    OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
    OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
    OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址
    for(n=0;n<132;n++)
      OLED_WR_Byte(0,OLED_DATA);
  }
}

void OLED_SetPos(uint8_t x, uint8_t y) //设置起始点坐标
{ 
  OLED_WR_Byte(0xb0+y,OLED_CMD);
  OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
  OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD);
}

//显示字符串
//x,y:	起点坐标
//*s:	字符串起始地址
//TextSize:字体
void OLED_DispStr(uint8_t x, uint8_t y, uint8_t s[], uint8_t TextSize)
{
  uint8_t c = 0,i = 0,j = 0;
  switch(TextSize)
  {
    case FONT_8X6:
    {
      while(s[j] != '\0')
      {
	c = s[j] - 32;
	if(x > 126)
	{
	  x = 0;
	  y++;
	}
	OLED_SetPos(x,y);
	for(i=0;i<6;i++)
	  OLED_WR_Byte(F6x8[c][i],OLED_DATA);
	x += 6;
	j++;
      }
    }break;

    case FONT_16X8:
    {
      while(s[j] != '\0')
      {
	c = s[j] - 32;
	if(x > 120)
	{
	  x = 0;
	  y++;
	}
	OLED_SetPos(x,y);
	for(i=0;i<8;i++)
	  OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
	OLED_SetPos(x,y+1);
	for(i=0;i<8;i++)
	  OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
	x += 8;
	j++;
      }
    }break;
  }
}

//初始化SSD1306					    
void OLED_Init(void)
{
/* defined in gpio.c*/
//  GPIO_InitTypeDef  GPIO_InitStructure;
//  GPIO_SetBits(GPIOB,GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_14|GPIO_Pin_15);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_14|GPIO_Pin_15;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		   //字库数据输出  FL_SDO
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//  Delay_ms(100);
  HAL_Delay(100);

  OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
  OLED_WR_Byte(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
  OLED_WR_Byte(80,OLED_CMD);   //[3:0],分频因子;[7:4],震荡频率
  OLED_WR_Byte(0xA8,OLED_CMD); //设置驱动路数
  OLED_WR_Byte(0X3F,OLED_CMD); //默认0X3F(1/64)
  OLED_WR_Byte(0xD3,OLED_CMD); //设置显示偏移
  OLED_WR_Byte(0X00,OLED_CMD); //默认为0

  OLED_WR_Byte(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.

  OLED_WR_Byte(0x8D,OLED_CMD); //电荷泵设置
  OLED_WR_Byte(0x14,OLED_CMD); //bit2，开启/关闭
  OLED_WR_Byte(0x20,OLED_CMD); //设置内存地址模式
  OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
  OLED_WR_Byte(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;

  OLED_WR_Byte(0xC8,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
  //OLED_WR_Byte(0xC0,OLED_CMD); //垂直反显


  OLED_WR_Byte(0xDA,OLED_CMD); //设置COM硬件引脚配置
  OLED_WR_Byte(0x12,OLED_CMD); //[5:4]配置

  OLED_WR_Byte(0x81,OLED_CMD); //对比度设置
  OLED_WR_Byte(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
  OLED_WR_Byte(0xD9,OLED_CMD); //设置预充电周期
  OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
  OLED_WR_Byte(0xDB,OLED_CMD); //设置VCOMH 电压倍率
  OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

  OLED_WR_Byte(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
  OLED_WR_Byte(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示
  OLED_WR_Byte(0xAF,OLED_CMD); //开启显示

  OLED_Clear();
}  

/****送指令到字库IC***/
void GB_WriteCmd( uint8_t cmd )
{
  uint8_t i;
  for(i=0;i<8;i++ )
  {
    if(cmd&0x80)
    {
      OLED_SDIN_Set();
      asm("NOP");
    }
    else
    {
      OLED_SDIN_Clr();
      asm("NOP");
    }
    cmd = cmd<<1;
    OLED_SCLK_Clr();
    asm("NOP");
    OLED_SCLK_Set();
    asm("NOP");
  }
}

/****从字库IC中取汉字或字符数据（1个字节）***/
static uint8_t GB_GetData(void)
{
  uint8_t i;
  uint8_t ret_data=0;
  OLED_SCLK_Set();
  asm("NOP");

  for(i=0;i<8;i++)
  {
    OLED_SCLK_Clr();
    asm("NOP");
    ret_data=ret_data<<1;

    if( GB_SDO )
    {
      ret_data=ret_data+1;
      asm("NOP");
    }

    OLED_SCLK_Set();
    asm("NOP");
  }
  return(ret_data);
}

/*从相关地址（addrHigh：地址高字节,addrMid：地址中字节,addrLow：地址低字节）中连续读出DataLen个字节的数据到 pBuff的地址*/
/*连续读取*/
void GB_GetBytes(uint8_t addrHigh,uint8_t addrMid,uint8_t addrLow,uint8_t *pBuff,uint8_t DataLen )
{
  uint8_t i;
  GB_CS_Clr();
  OLED_SCLK_Clr();
  GB_WriteCmd(0x03);
  GB_WriteCmd(addrHigh);
  GB_WriteCmd(addrMid);
  GB_WriteCmd(addrLow);
  for(i=0;i<DataLen;i++ )
    *(pBuff+i) =GB_GetData();
  GB_CS_Set();
}

uint32_t  fontaddr=0;
void DispStr_GB2312(uint8_t x,uint8_t y,uint8_t *text)
{
  uint8_t i= 0;
  uint8_t addrHigh,addrMid,addrLow ;
  uint8_t fontbuf[32];

  while((text[i]>0x00))
  {
    if(((text[i]>=0xb0) &&(text[i]<=0xf7))&&(text[i+1]>=0xa1))
    {
      /*国标简体（GB2312）汉字在字库IC中的地址由以下公式来计算：*/
      /*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
      /*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
      fontaddr = (text[i]- 0xb0)*94;
      fontaddr += (text[i+1]-0xa1)+846;
      fontaddr = (uint32_t)(fontaddr*32);

      addrHigh = (fontaddr&0xff0000)>>16;  /*地址的高8位,共24位*/
      addrMid = (fontaddr&0xff00)>>8;      /*地址的中8位,共24位*/
      addrLow = fontaddr&0xff;	     /*地址的低8位,共24位*/

      GB_GetBytes(addrHigh,addrMid,addrLow,fontbuf,32 );/*取32个字节的数据，存到"fontbuf[32]"*/
      //display_graphic_16x16(y,x,fontbuf);/*显示汉字到LCD上，y为页地址，x为列地址，fontbuf[]为数据*/
      OLED_DispChinese(x,y,fontbuf);
      i+=2;
      x+=16;
    }
    else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))
    {
      /*国标简体（GB2312）15x16点的字符在字库IC中的地址由以下公式来计算：*/
      /*Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0*/
      /*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
      fontaddr = (text[i]- 0xa1)*94;
      fontaddr += (text[i+1]-0xa1);
      fontaddr = (uint32_t)(fontaddr*32);

      addrHigh = (fontaddr&0xff0000)>>16;  /*地址的高8位,共24位*/
      addrMid = (fontaddr&0xff00)>>8;      /*地址的中8位,共24位*/
      addrLow = fontaddr&0xff;	     /*地址的低8位,共24位*/
      GB_GetBytes(addrHigh,addrMid,addrLow,fontbuf,32 );/*取32个字节的数据，存到"fontbuf[32]"*/
      OLED_DispChinese(x,y,fontbuf);

      i+=2;
      x+=16;
    }
    else if((text[i]>=0x20) &&(text[i]<=0x7e))
    {
      unsigned char fontbuf[16];
      fontaddr = (text[i]- 0x20);
      fontaddr = (unsigned long)(fontaddr*16);
      fontaddr = (unsigned long)(fontaddr+0x3cf80);
      addrHigh = (fontaddr&0xff0000)>>16;
      addrMid = (fontaddr&0xff00)>>8;
      addrLow = fontaddr&0xff;

      GB_GetBytes(addrHigh,addrMid,addrLow,fontbuf,16 );/*取16个字节的数据，存到"fontbuf[32]"*/
      OLED_DispStr(x,y,fontbuf,FONT_16X8);

      i+=1;
      x+=8;
    }
    else
    i++;
  }
}
