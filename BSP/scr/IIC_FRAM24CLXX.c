
/*********************************************************************************
//概述：: Operation for SI446x

//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\IIC_FRAM24CLXX.h"  
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

/*********************************************************************************
私有变量定义区
*********************************************************************************/
static unsigned char   I2C_SlaverAddress = FRAM_ADDRESS;
/*********************************************************************************
测试变量定义区
*********************************************************************************/
 
/*********************************************************************************
内部数据类型定义区
*********************************************************************************/

/*********************************************************************************
内部函数定义区
*********************************************************************************/
static SystemErrName FRAM_WaitEepromStandbyState(void); //EEPROM设为待命状态
/*********************************************************************************
功能代码定义区
*********************************************************************************/
static void Delay ( unsigned long temp )
{
 while (temp -- );
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
static void I2C_Config_FRAM24CLXX(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 
  GPIO_InitTypeDef  GPIO_InitStructure; 
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB , ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType =GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 , ENABLE);
  
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);//非常重要,没有这句RESER语句会导致总线不能释放 
  
  // I2C configuration 
  I2C1->CR1 |= 0x8000;
  I2C1->CR1 &= ~0x8000;
  
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0xA0;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 100000;
  I2C_DeInit( I2C1 );
  I2C_Init(I2C1, &I2C_InitStructure);
  I2C_Cmd(I2C1, ENABLE);
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
static void I2C_Rset_FRAM24CLXX(void)
{
  unsigned long  itemp = 0x4fff;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 , DISABLE);
  while(itemp){itemp --;}
  I2C_Config_FRAM24CLXX( );
}
/*******************************************************************************
* Function Name : I2C_EE_ByteWrite
* Description    : Writes one byte to the I2C EEPROM.
* Input          : - data : 要写入的数据
*                  - WriteAddr : EEPROM中要写入数据的首地址.
* Output         : None
* Return         : None
*******************************************************************************/
SystemErrName FRAM_ByteWrite(unsigned char data, unsigned short WriteAddr)//写一个字节到EEPROM
{
 SystemErrName err; 
 unsigned long itemp = 0;
 
 err =  FRAM_WaitEepromStandbyState();//EEPROM设为待命状态
 if(NO_ERR!= err){return err;} 

 I2C_GenerateSTART(I2C1, ENABLE);//产生 I2Cx传输 START条件

 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) //检查最近一次 I2C事件是否是输入的事件
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0; 

 I2C_Send7bitAddress(I2C1, I2C_SlaverAddress, I2C_Direction_Transmitter);//向指定的从 I2C设备传送地址字,选择发送方向

 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))//检查最近一次 I2C事件是否是输入的事件
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0;       

 I2C_SendData(I2C1, WriteAddr);//通过外设 I2Cx发送地址

 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//检查最近一次 I2C事件是否是输入的事件
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0; 

 I2C_SendData(I2C1, data); //通过外设 I2Cx发送数据 /* Send the byte to be written */
   
 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//检查最近一次 I2C事件是否是输入的事件
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}//  itemp = 0; 

 I2C_GenerateSTOP(I2C1, ENABLE);//产生 I2Cx传输 STOP条件
 
 while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
 { 
  if(itemp++ > 0x4fff)
  { return Unsuccessful;}
 }
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 , DISABLE);
 
 return NO_ERR;
}
/*******************************************************************************
* Function Name : I2C_EE_PageWrite
* Description    : Writes more than one byte to the EEPROM with a single WRITE
*                  cycle. The number of byte can't exceed the EEPROM page size.
* Input          : - pBuffer : pointer to the buffer containing the data to be 
*                    written to the EEPROM.
*                  - WriteAddr : EEPROM's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the EEPROM.
* Output         : None
* Return         : None
pBuffer：指向要写入数据数组的指针
WriteAddr：24c02中要写入数据的首地址
NumByteToWrite：写入的字节数
*******************************************************************************/
SystemErrName FRAM_PageWrite(unsigned char* pBuffer, 
                             unsigned short NumByteToWrite,
                             unsigned short WriteAddr)//写少于一页的数据
{
 SystemErrName err; 
 unsigned long itemp = 0;
 
 err =  FRAM_WaitEepromStandbyState();//EEPROM设为待命状态
 if(NO_ERR!= err){return err;} 

 I2C_GenerateSTART(I2C1, ENABLE);//产生 I2Cx传输 START条件
 
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) //检查最近一次 I2C事件是否是输入的事件
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0;  
  I2C_SlaverAddress = FRAM_ADDRESS & 0xF0;
  I2C_SlaverAddress += (WriteAddr >> 7) & 0x0E;
  I2C_Send7bitAddress(I2C1, I2C_SlaverAddress, I2C_Direction_Transmitter);//向指定的从 I2C设备传送地址字,选择发送方向

  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))//检查最近一次 I2C事件是否是输入的事件
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0;

  I2C_SendData(I2C1, WriteAddr); //通过外设 I2Cx发送地址

  while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//检查最近一次 I2C事件是否是输入的事件
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0;  

  while(NumByteToWrite--) 
  {
    I2C_SendData(I2C1, *pBuffer); //通过外设 I2Cx发送数据
    pBuffer++; 
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//检查最近一次 I2C事件是否是输入的事件
   {
     if(itemp++ > 0x4ffffff)
     {return Unsuccessful;}
   }
    itemp = 0; 
  }
  I2C_GenerateSTOP(I2C1, ENABLE);//产生 I2Cx传输 STOP条件
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
 { 
  if(itemp++ > 0x4fff)
  { return Unsuccessful;}
 }
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 , DISABLE);
  return NO_ERR;
}
/*******************************************************************************
* Function Name : I2C_EE_BufferRead
* Description    : Reads a block of data from the EEPROM.
* Input          : - pBuffer : pointer to the buffer that receives the data read 
*                    from the EEPROM.
*                  - ReadAddr : EEPROM's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the EEPROM.
* Output         : None
* Return         : None
pBuffer：指向要保存读出数据的数组的指针
ReadAddr：24c02中要读出数据的首地址
NumByteToRead：读出的字节数
*******************************************************************************/
SystemErrName FRAM_PageRead(void* Buffer, 
                            unsigned short NumByteToRead,
                            unsigned short ReadAddr)//将EEPROM的数据读入缓冲器
{ 
 SystemErrName err; 
 unsigned long itemp = 0;
 unsigned char* pBuffer;
 pBuffer = Buffer;
 err =  FRAM_WaitEepromStandbyState();//EEPROM设为待命状态
 if(NO_ERR!= err)
 {return err;}

  I2C_GenerateSTART(I2C1, ENABLE);//Send START condition 

  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))//Test on EV5 and clear it 
  {if(itemp++ > 0x4fff){return Unsuccessful;}}
  itemp = 0;
  if(NumByteToRead==1)  
  {I2C_AcknowledgeConfig(I2C1, DISABLE);}//In the case of a single data transfer disable ACK before reading the data 
  
  I2C_SlaverAddress = FRAM_ADDRESS & 0xF0;
  
  I2C_SlaverAddress += (ReadAddr >> 7) & 0x0E;
  
  I2C_Send7bitAddress(I2C1, I2C_SlaverAddress, I2C_Direction_Transmitter);//Transmits the address byte to select the slave device 

  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))//Test on EV6 and clear it 
  {if(itemp++ > 0x4fff){return Unsuccessful;}}
  itemp = 0; 

  I2C_SendData(I2C1, ReadAddr & 0x00FF); // Send the EEPROM's internal address to write to 
 
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))///Test on EV8 and clear it 
  {if(itemp++ > 0x4fff){return Unsuccessful;}}
  itemp = 0;
  I2C_GenerateSTART(I2C1, ENABLE);// Send STRAT condition a second time *

  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))//Test on EV5 and clear it 
  {if(itemp++ > 0x4fff){return Unsuccessful;}}
  itemp = 0;
  
  I2C_Send7bitAddress(I2C1, I2C_SlaverAddress, I2C_Direction_Receiver);// Send EEPROM address for read 

  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))//Test on EV6 and clear it 
   {if(itemp++ > 0x4fff){return Unsuccessful;}}
  itemp = 0;
  while(NumByteToRead) 
  {
    if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) //Test on EV7 and clear it 
    {
      itemp = 0; 
      if(NumByteToRead == 2)
      {I2C_AcknowledgeConfig(I2C1, DISABLE);}//使能或者失能指定 I2C的应答功能
      if(NumByteToRead == 1)
      {I2C_GenerateSTOP(I2C1, ENABLE);}//产生 I2Cx传输 STOP条件
      *pBuffer = I2C_ReceiveData(I2C1);//返回通过 I2Cx最近接收的数据
      pBuffer++; 
      NumByteToRead--;    
   }
   if(itemp++ > 0x4ffff){return Unsuccessful;}
  }
 I2C_AcknowledgeConfig(I2C1, ENABLE);//使能或者失能指定 I2C的应答功能
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
 { 
  if(itemp++ > 0x4fff)
  { return Unsuccessful;}
 }
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 , DISABLE);
 return NO_ERR;
}
/*******************************************************************************
* Function Name  : I2C_EE_BufferWrite
* Description    : 将缓冲器的数据写入EEPROM
* Input          : - pBuffer : 指向要写入数据数组的指针 
*                  - WriteAddr : EEPROM中要写入数据的首地址.
*                  - NumByteToWrite : 写入的字节数.
* Output         : None
* Return         : None
pBuffer：
WriteAddr：
NumByteToWrite：写入的字节数
*******************************************************************************/
SystemErrName FRAM_BufferWrite(void* Buffer, 
                               short NumByteToWrite,
                               unsigned short WriteAddr)//将缓冲器的数据写入EEPROM
{
 SystemErrName err = NO_ERR ; 
 unsigned char  NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
 unsigned char* pBuffer; 
 
 Addr = WriteAddr % FRAM_PageSize;//写入地址是每页的第几位
 count = FRAM_PageSize - Addr;//在开始的一页要写入的个数
 pBuffer = Buffer;
 
 if(Addr + NumByteToWrite < FRAM_PageSize )   // 数据不跨页
  { err = FRAM_PageWrite(pBuffer, NumByteToWrite, WriteAddr);}//写少于一页的数据
 else//数据不跨页
  {
   NumByteToWrite -= count;
   NumOfPage = NumByteToWrite / FRAM_PageSize; //计算要写入的页数
   NumOfSingle = NumByteToWrite % FRAM_PageSize;//重新计算不足一页的个数
   if(count != 0)//
   { 
    err = FRAM_PageWrite(pBuffer, count, WriteAddr);//将开始的空间写满一页
    if(NO_ERR != err){return err;}
    WriteAddr += count;
    pBuffer += count;
   } 
   while(NumOfPage--)//要写入的页数
   {
    err = FRAM_PageWrite(pBuffer, FRAM_PageSize, WriteAddr);//写一页的数据
    if(NO_ERR != err){return err;}
    WriteAddr += FRAM_PageSize;
    pBuffer += FRAM_PageSize; 
   }
   if(NumOfSingle != 0)//剩余数据小于一页
   {err = FRAM_PageWrite(pBuffer, NumOfSingle, WriteAddr);  }//写少于一页的数据
  }
 return err;
}
/*******************************************************************************
* Function Name  : I2C_EE_BufferWrite
* Description    : 将缓冲器的数据写入EEPROM
* Input          : - pBuffer : 指向要写入数据数组的指针 
*                  - WriteAddr : EEPROM中要写入数据的首地址.
*                  - NumByteToWrite : 写入的字节数.
* Output         : None
* Return         : None
pBuffer：
WriteAddr：
NumByteToWrite：写入的字节数
*******************************************************************************/
SystemErrName FRAM_BufferRead(void* Buffer,
                              unsigned short NumByteToRead,
                              unsigned short ReadAddr)//将EEPROM的数据读入缓冲器
{
 SystemErrName err = NO_ERR ; 
 unsigned char  NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
 unsigned char* pBuffer; 
 
 Addr = ReadAddr % FRAM_PageSize;//写入地址是每页的第几位
 count = FRAM_PageSize - Addr;//在开始的一页要写入的个数
 pBuffer = Buffer;
 
 if(Addr + NumByteToRead < FRAM_PageSize )   // 数据不跨页
  { err = FRAM_PageRead(pBuffer, NumByteToRead,ReadAddr);}//写少于一页的数据
 else//数据不跨页
  {
   NumByteToRead -= count;
   NumOfPage = NumByteToRead / FRAM_PageSize; //计算要写入的页数
   NumOfSingle = NumByteToRead % FRAM_PageSize;//重新计算不足一页的个数
   if(count != 0)//
   { 
    err = FRAM_PageRead(pBuffer, count, ReadAddr);//将开始的空间写满一页
    if(NO_ERR != err)
    {return err;}
    ReadAddr += count;
    pBuffer += count;
   } 
   while(NumOfPage--)//要写入的页数
   {
    err = FRAM_PageRead(pBuffer, FRAM_PageSize, ReadAddr);//写一页的数据
    if(NO_ERR != err){return err;}
    ReadAddr += FRAM_PageSize;
    pBuffer += FRAM_PageSize; 
   }
   if(NumOfSingle != 0)//剩余数据小于一页
   {err = FRAM_PageRead(pBuffer, NumOfSingle, ReadAddr);  }//写少于一页的数据
  }
 return err;
}
/*******************************************************************************
* Function Name : I2C_EE_WaitEepromStandbyState
* Description    : Wait for EEPROM Standby state
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
SystemErrName FRAM_WaitEepromStandbyState(void) //EEPROM设为待命状态     
{
 unsigned long itemp = 0 ;
 unsigned long itemp1 = 0 ;
 
 //RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 , DISABLE);
 I2C_Config_FRAM24CLXX( );
 
 while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
 {if(itemp++ > 0x4fff)
 {return Unsuccessful;}}
 itemp = 0;
 do
 {
  I2C_GenerateSTART(I2C1, ENABLE);//产生 I2Cx传输 START条件
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) //检查最近一次 I2C事件是否是输入的事件
  {if(itemp++ > 0x4fff)
  {return Unsuccessful;}}
  itemp = 0;
  I2C_Send7bitAddress(I2C1, I2C_SlaverAddress, I2C_Direction_Transmitter);//向指定的从 I2C设备传送地址字 ,选择发送方向
  Delay ( 0x4ff );
  I2C_ClearFlag(I2C2, I2C_FLAG_AF);//清除 I2Cx的应答错误标志位
  I2C_ReadRegister(I2C1, I2C_Register_SR1);//读取指定的 I2C寄存器 I2C_SR1 并返回其值
  if(itemp1++ > 0x4fff)
  {
    I2C_Rset_FRAM24CLXX( );
  return Unsuccessful;
  }
 }while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));//地址发送结束
 I2C_ClearFlag(I2C1, I2C_FLAG_AF);//清除 I2Cx的应答错误标志位
 
 I2C_GenerateSTOP(I2C1, ENABLE);//产生 I2Cx传输 STOP条件
 while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
 { 
  if(itemp++ > 0x4fff)
  { return Unsuccessful;}
 }
 return NO_ERR;
}
/*********************************************************************************
函数名：		   
函数描述：	   
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
*********************************************************************************/
SystemErrName FRAM_MultipleRead(void* Buffer, 
                                unsigned short NumByteToRead,
                                unsigned short ReadAddr)
{
  SystemErrName err = NO_ERR; 
  unsigned char itemp = 0 ;
  while (itemp++ < 3)
  {
   err = FRAM_BufferRead(Buffer,  ReadAddr, NumByteToRead);
   if(NO_ERR == err)
   { break;}
  }
  return err;
}
/*********************************************************************************
函数名：		   
函数描述：	
入口参数描述：	   
                   
				  
出口参数描述：
函数返回值描述：
其他说明： 
*********************************************************************************/
SystemErrName FRAM_MultipleWrite(void* Buffer,
                                 unsigned short NumByteToWrite,
                                 unsigned short WriteAddr)
{
  SystemErrName err = NO_ERR; 
  unsigned char itemp = 0 ;
  while (itemp++ < 3)
  {
   err = FRAM_BufferWrite(Buffer, WriteAddr, NumByteToWrite);//将缓冲器的数据写入EEPROM
   if(NO_ERR == err)
   {break;}
  }
  return err;
}
/***********************************END**********************************************/


