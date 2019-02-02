/*
 * Initialize sensor by SCCB interface
 * reference: guanfu_wang
 * */
#include "usart.h"
#include "delay.h"
#include "serial.h"
#include "sensor.h"
#include "sensor_config.h"
#include <string.h>

extern const char OV7670_reg[OV7670_REG_NUM][2];
//extern const char ov7670_init_reg_tbl_YUV[OV7670_REG_NUM][2];
/*
 *Function: supply CLOCK for sensor if no crystal oscillator installed
 *
 * */
/*
void XCLK_init_ON(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  RCC_MCOConfig(RCC_MCO_HSE  );  // HSI
}
*/

/*Function: write OV7670 register through SCCB interface
 * if success: return 1
 * if error: return 0
 * */
unsigned char wr_Sensor_Reg(unsigned char regID, unsigned char regDat)
{
  startSCCB();  // send SCCB start sequence, ready to write command
  if(0==SCCBwriteByte(0x42))  // write ov7670 device ID(last bit controls W=0/R=1)
  {
    stopSCCB();  // send SCCB stop sequence
    return(0);  // if error: return 0
  }
  Delay_Us(20);
  if(0==SCCBwriteByte(regID))  // write register ID
  {
    stopSCCB();  // send SCCB stop sequence
    return(0);  // if error: return 0
  }
  Delay_Us(20);
  if(0==SCCBwriteByte(regDat))  // write data into register
  {
    stopSCCB();  // send SCCB stop sequence
    return(0);  // if error: return 0
  }
  stopSCCB();  // send SCCB stop sequence

  return(1);  // if success: return 1
}

/*Function: read OV7670 register through SCCB interface
 * if success: return 1
 * if error: return 0
 * */
unsigned char rd_Sensor_Reg(unsigned char regID, unsigned char *regDat)
{
  /* send register ID */
  startSCCB();  // send SCCB start sequence, ready to write command
  if(0==SCCBwriteByte(0x42))  // write ov7670 device ID(last bit controls W=0/R=1)
  {
    stopSCCB();  // send SCCB stop sequence
    HAL_UART_Transmit(&huart1, (uint8_t *)"yyy", strlen("yyy"), 10);
    return(0);  // if error: return 0
  }
  Delay_Us(20);
  if(0==SCCBwriteByte(regID))  // write ov7670 register ID
  {
    stopSCCB();  // send SCCB stop sequence
    HAL_UART_Transmit(&huart1, (uint8_t *)"zzz", strlen("zzz"), 10);
    return(0);  // if error: return 0
  }
  /* a stop is needed(differ from IIC) */
  stopSCCB();  // send SCCB stop sequence

  Delay_Us(20);

  /* read value from register */
  startSCCB();
  if(0==SCCBwriteByte(0x43))  // write ov7670 device ID(last bit controls W=0/R=1)
  {
    stopSCCB();  // send SCCB stop sequence
    HAL_UART_Transmit(&huart1, (uint8_t *)"qqq", strlen("qqq"), 10);
    return(0);  // if error: return 0
  }
  Delay_Us(20);
  *regDat=SCCBreadByte();  // return read value
  noAck();  // send SCCB NACK sequence
  stopSCCB();  // send SCCB stop sequence
  return(1);  // if success: return 1
}

/* Function: Sensor_init
 * if success: return 1
 * if error: return 0
 * */
unsigned char Sensor_init(unsigned char color_mode)
{
  unsigned char temp;
  unsigned int i=0;
//  XCLK_init_ON();  // switch on MCO to supply clock for CMOS sensor if there is no external crystal oscillator
//  uchar ovidmsb=0,
//  ovidlsb=0;
//  Sensor_GPIO_Init();  // configured in file: gpio.c
//  SCCB_GPIO_Config();  // configured in file: gpio.c

  temp=0x80;
  if(0==wr_Sensor_Reg(0x12, temp)) //Reset SCCB
  {
	  Debug_Print_ln(&huart1, "wr_Sensor_Reg error");
	  return 0 ;  // return 0 if error
  }
  Delay_Us(10);
  if(0==rd_Sensor_Reg(0x0b, &temp))  // read ID
  {
	  Debug_Print_ln(&huart1, "read temp error");
	  return 0 ;  // return 0 if error
  }

  if(temp==0x73)//OV7670
  {
    if(color_mode)
    {
      for(i=0; i<OV7670_REG_NUM; i++)
      {
		if(0==wr_Sensor_Reg(OV7670_reg[i][0], OV7670_reg[i][1]))
		{
			Debug_Print_ln(&huart1, "write color params error");
			return 0;  // return 0 if error
		}
      }
    }
    else
    {
      for(i=0;i<sizeof(ov7670_init_reg_tbl_YUV)/sizeof(ov7670_init_reg_tbl_YUV[0])/2;i++)
      {
		wr_Sensor_Reg(ov7670_init_reg_tbl_YUV[i][0], ov7670_init_reg_tbl_YUV[i][1]);
		HAL_Delay(2);
      }
    }
//  Sensor_EXTI_Config();  // configured in file: gpio.c
//  Sensor_Interrupts_Config();  // configured in file: gpio.c
  }
  return 0x01;  // return 1 = OK
}

unsigned char OV7670_config_window(uint16_t start_h, uint16_t start_w, uint16_t height, uint16_t width)
{
	uint16_t endx=(start_w+width*2)%784;
	uint16_t endy=(start_h+height*2);
	uint8_t x_reg, y_reg;
	uint8_t state=0;
	uint8_t temp=0;

	state += rd_Sensor_Reg(0x32, &x_reg);
	x_reg &= 0xC0;
	state += rd_Sensor_Reg(0x03, &y_reg);
	y_reg &= 0xF0;
	// set HREF
	temp = x_reg|((endx&0x7)<<3)|(start_w&0x7);
	state += wr_Sensor_Reg(0x32, temp);
	temp = (start_w&0x7F8)>>3;
	state += wr_Sensor_Reg(0x17, temp);
	temp = (endx&0x7F8)>>3;
	state += wr_Sensor_Reg(0x18, temp);
	// set VREF
	temp = y_reg|((endy&0x3)<<2)|(start_h&0x3);
	state += wr_Sensor_Reg(0x03, temp);
	temp = (start_h&0x3FC)>>2;
	state += wr_Sensor_Reg(0x19, temp);
	temp = (endy&0x3FC)>>2;
	state += wr_Sensor_Reg(0x1A, temp);

	return state;
}

/*
 * Store a set of start/stop values into the camera.
 * https://chromium.googlesource.com/chromiumos/third_party/kernel-next/+/0.12.433.B/drivers/media/video/ov7670.c
 */
int ov7670_set_hw(int hstart, int hstop, int vstart, int vstop)
{
	int ret;
	unsigned char v;
/*
 * Horizontal: 11 bits, top 8 live in hstart and hstop.  Bottom 3 of
 * hstart are in href[2:0], bottom 3 of hstop in href[5:3].  There is
 * a mystery "edge offset" value in the top two bits of href.
 */
	ret =  wr_Sensor_Reg(REG_HSTART, (hstart >> 3) & 0xff);
	ret += wr_Sensor_Reg(REG_HSTOP, (hstop >> 3) & 0xff);
	ret += rd_Sensor_Reg(REG_HREF, &v);
	v = (v & 0xc0) | ((hstop & 0x7) << 3) | (hstart & 0x7);
	HAL_Delay(10);
	ret += wr_Sensor_Reg(REG_HREF, v);
/*
 * Vertical: similar arrangement, but only 10 bits.
 */
	ret += wr_Sensor_Reg(REG_VSTART, (vstart >> 2) & 0xff);
	ret += wr_Sensor_Reg(REG_VSTOP, (vstop >> 2) & 0xff);
	ret += rd_Sensor_Reg(REG_VREF, &v);
	v = (v & 0xf0) | ((vstop & 0x3) << 2) | (vstart & 0x3);
	HAL_Delay(10);
	ret += wr_Sensor_Reg(REG_VREF, v);
	return ret;
}
