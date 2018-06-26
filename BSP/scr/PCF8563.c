/*********************************************************************************
//概述：: Operation for SI446x

//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "PCF8563.h"  
#include "bsp.h"
#include "stm32l1xx_conf.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
/*********************************************************************************
公共变量定义区
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/
TimerData_Struct  SystemTimer;


/*********************************************************************************
私有变量定义区
**********************************************************************************/ 
/*********************************************************************************
测试变量定义区
*********************************************************************************/
//unsigned  short  ioiiiii =0;
/*********************************************************************************
内部数据类型定义区
*********************************************************************************/
/*********************************************************************************
内部函数定义区
*********************************************************************************/

/*********************************************************************************
功能代码定义区
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
static void delay_us(unsigned int us)
{while (us--);}
/**********************************************
函数名：		  
函数描述：		   
调用列表：		   
被调用列表：
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 	SPI/I2S2_RX  
***********************************************/
static void I2C_Config_PCF8563(void)
{

  GPIO_InitTypeDef  gpio_config;
  I2C_InitTypeDef  i2c_config;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB , ENABLE);
  
  gpio_config.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  gpio_config.GPIO_Mode = GPIO_Mode_AF;
  gpio_config.GPIO_Speed = GPIO_Speed_10MHz;
  gpio_config.GPIO_OType = GPIO_OType_OD;
  gpio_config.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB,&gpio_config);
  
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_I2C2);  //这两个的配置必须有，没有则无法输出
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_I2C2);
  
  i2c_config.I2C_ClockSpeed = 100000;  
  i2c_config.I2C_Mode = I2C_Mode_I2C;
  i2c_config.I2C_DutyCycle = I2C_DutyCycle_2;
  i2c_config.I2C_OwnAddress1 = 0x10;      //此处地址可以任意设置，当其作为从机的时候，主机对其的寻找地                                                                                                                                 址为此地址和0xa0相同
  i2c_config.I2C_Ack = I2C_Ack_Enable;
  i2c_config.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  
  I2C_Init(I2C2,&i2c_config);
  I2C_Cmd(I2C2,ENABLE);
   

}
/**********************************************
函数名：		  
函数描述：		   
调用列表：		   
被调用列表：
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 	SPI/I2S2_RX  
***********************************************/
static void I2C_Rset_PCF8563(void)
{
  unsigned long  itemp = 0x4fff;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2 , DISABLE);
  while(itemp){itemp --;}
  I2C_Config_PCF8563( );
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
unsigned char I2C_8563_ByteWrite(unsigned char WriteAddr,unsigned char pBuffer)
{
  unsigned long itemp = 0;
  /* Send STRAT condition */
  I2C_GenerateSTART(I2C2, ENABLE);
  /* Test on EV5 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
  {
   itemp ++;
   if(itemp > 0x4ff)		//超时
   {return 0;}
  }  
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C2, PCF8563ADDR_WRITE, I2C_Direction_Transmitter);
  /* Test on EV6 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
   itemp ++;
   if(itemp > 0x4ff)		//超时
   {return 0;}
  }     
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C2, WriteAddr);
  /* Test on EV8 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    itemp ++;
    if(itemp > 0x4ff)		//超时
    {return 0;}
  }  
  /* Send the byte to be written */
  I2C_SendData(I2C2, pBuffer); 
  /* Test on EV8 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    itemp ++;
    if(itemp > 0x4ff)		//超时
    {return 0;}
  }  
  /* Send STOP condition */
  I2C_GenerateSTOP(I2C2, ENABLE);
  return 1;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
unsigned char I2C_8563_byteRead(unsigned char ReadAddr)
{ 
  
  unsigned long itemp = 0; 
  unsigned char readbyte = 0;
  /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
  {
    itemp ++;
    if(itemp > 0x4ff)
    {I2C_Rset_PCF8563( ); 
    break;
    }
  }		//超时
  I2C_GenerateSTART(I2C2, ENABLE);
  itemp = 0;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
  {
    itemp ++;if(itemp > 0x4ff)
    {
      I2C_Rset_PCF8563( ); 
      break;
    }
  } 
  I2C_Send7bitAddress(I2C2, PCF8563ADDR_WRITE, I2C_Direction_Transmitter);
  itemp = 0;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    itemp ++;if(itemp > 0x4ff)
    {
      I2C_Rset_PCF8563( );  
      break;
    } 
  }
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(I2C2, ENABLE);
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C2, ReadAddr);  
  /* Test on EV8 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    itemp ++;if(itemp > 0x4ff)
    {
      I2C_Rset_PCF8563( );
      break;
    }
  }		
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2C2, ENABLE);
  /* Test on EV5 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
  {itemp ++;
  if(itemp > 0x4ff)
  {
    I2C_Rset_PCF8563( ); 
    break;
  }
  }
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(I2C2, PCF8563ADDR_READ, I2C_Direction_Receiver);
  /* Test on EV6 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {
    itemp ++;
    if(itemp > 0x4ff)
    {
      I2C_Rset_PCF8563( );
      break;
    }
  }
  /* Test on EV7 and clear it */
  itemp = 0;		
  /* Disable Acknowledgement */
  I2C_AcknowledgeConfig(I2C2, DISABLE);	
  /* Send STOP Condition */
  I2C_GenerateSTOP(I2C2, ENABLE);			
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))
  {
    itemp ++;if(itemp > 0x4ff)
    {
      I2C_Rset_PCF8563( ); 
      break;
    }
  }
  readbyte = I2C_ReceiveData(I2C2);
  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C2, ENABLE);
  return readbyte;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void PCF8563_Set_Time(TimerData_Struct* timer)
{
 unsigned char itemp1;
 unsigned char itemp2;
 if(P_NULL == timer){return;}
 if(timer->Year > 1999)
 {
  if(timer->Year < 2099){itemp1 = timer->Year - 2000;}
  else{return;}
 }
 else if(timer->Year > 1900){itemp1 = timer->Year - 1900;}
 else{return;}
  
 if((timer->Month < 1)||(timer->Month > 12)){return;}
 if((timer->Day < 1)||( timer->Day > 31 )){return;}
 if(timer->Hour > 23){return;}
 if(timer->Minute > 59){return;} 
 if(timer->Second > 59){return;}  
 
 I2C_Config_PCF8563( );

 itemp2 = itemp1 / 10;
 itemp2 <<=4 ;
 itemp2 |=itemp1 % 10;
 I2C_8563_ByteWrite(YEAR, itemp2);

 itemp1 = timer->Month;
 itemp2 = itemp1 / 10;
 itemp2 <<=4 ;
 itemp2 |=itemp1 % 10;
 if(timer->Year < 2000){itemp2|= 0x80;}
 I2C_8563_ByteWrite(MONTH, itemp2); 
 
 itemp1 = timer->Day;
 itemp2 = itemp1 / 10;
 itemp2 <<=4 ;
 itemp2 |=itemp1 % 10; 
 I2C_8563_ByteWrite(DAY, itemp2);//
 
 I2C_8563_ByteWrite(WEEK, timer->WeekDay);//

 itemp1 = timer->Hour;
 itemp2 = itemp1 / 10;
 itemp2 <<=4 ;
 itemp2 |=itemp1 % 10; 
 I2C_8563_ByteWrite(HOUR, itemp2);//

 itemp1 = timer->Minute;
 itemp2 = itemp1 / 10;
 itemp2 <<=4 ;
 itemp2 |=itemp1 % 10; 
 I2C_8563_ByteWrite(MINUTE, itemp2);//

 itemp1 = timer->Second;
 itemp2 = itemp1 / 10;
 itemp2 <<=4 ;
 itemp2 |=itemp1 % 10;  
 I2C_8563_ByteWrite(SECOND, itemp2);//
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2 , DISABLE);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void PCF8563_Read_time(TimerData_Struct* timer)
{
  unsigned char itemp1;
  unsigned char itemp2;
  unsigned char flg;
  
  I2C_Config_PCF8563( );
   delay_us(20);
  itemp1 = I2C_8563_byteRead(MONTH);//
  flg = itemp1 & 0x80;
  itemp1 &= 0x1F; 
  itemp2 = itemp1 & 0x0F;
  itemp1 >>= 4;
  itemp2 += itemp1 * 10;
  timer->Month = itemp2 ;
  
  delay_us(100);
  itemp1 =  I2C_8563_byteRead(YEAR);  ////
  itemp2 = itemp1 & 0x0F;
  itemp1 >>= 4;
  itemp2 += itemp1 * 10;
  if(flg){timer->Year = itemp2 + 1900;}
  else{timer->Year = itemp2 + 2000;}
  
  delay_us(20);
  itemp1 = (I2C_8563_byteRead(DAY) & 0x3F);//
  itemp2 = itemp1 & 0x0F;
  itemp1 >>= 4;
  itemp2 += itemp1 * 10;
  timer->Day = itemp2 ;
  
  delay_us(20);
  timer->WeekDay = (I2C_8563_byteRead(WEEK) & 0x03);//
  delay_us(20);
  itemp1 = (I2C_8563_byteRead(HOUR) & 0x3F);//
  itemp2 = itemp1 & 0x0F;
  itemp1 >>= 4;
  itemp2 += itemp1 * 10;
  timer->Hour = itemp2 ;
  
  delay_us(20);
  itemp1 = (I2C_8563_byteRead(MINUTE)& 0x7F);//
  itemp2 = itemp1 & 0x0F;
  itemp1 >>= 4;
  itemp2 += itemp1 * 10;
  timer->Minute = itemp2 ;
  
  delay_us(20);
  itemp1 = (I2C_8563_byteRead(SECOND)& 0x7F);//
  itemp2 = itemp1 & 0x0F;
  itemp1 >>= 4;
  itemp2 += itemp1 * 10;
  timer->Second = itemp2 ;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2 , DISABLE);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void F8563_woring_mode(unsigned char mod)
{
 mod &= WARING_DEFINE_TIME; 
 mod |= 0x80;   //脉冲中断
 I2C_8563_ByteWrite(CONTROL2,mod);  //
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void F8563_worning_init(unsigned char hour,unsigned char minute)  ///   
{
 I2C_8563_ByteWrite(WAR_MINUTE,minute);
 I2C_8563_ByteWrite(WAR_HOUR,hour);      
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void F8563_back_init(unsigned char time, unsigned char TD)  ///  
{
  TD &= 0x03;
  TD |= 0x80; //倒计时有效
  I2C_8563_ByteWrite(DTIME_CONTROL,TD);   //0x80  频率设置 1/60Hz  单位为分钟
  I2C_8563_ByteWrite(TIME_CONT,time);   		// 	
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void F8563_Interrupt(void)  // 
{
  unsigned char flg;
  flg = I2C_8563_byteRead( CONTROL2 );	//
  I2C_8563_ByteWrite(CONTROL2,flg & 0x03); //清除中断标志
  if(flg & 0x04)  //定时中断
  {PCF8563_Read_time(&SystemTimer);}
  if(flg & 0x08)  //报警中断
  { flg = 0;}
}

/**************************************END****************************************/