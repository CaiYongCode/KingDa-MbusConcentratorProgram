
/*********************************************************************************
//������: Operation for SI446x

//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\inc\IIC_FRAM24CLXX.h"  
#include "stm32l1xx_conf.h"
/*********************************************************************************
����������
*********************************************************************************/
/*********************************************************************************
��������������
*********************************************************************************/
 
/*********************************************************************************
�ⲿ����������
*********************************************************************************/

/*********************************************************************************
˽�б���������
*********************************************************************************/
static unsigned char   I2C_SlaverAddress = FRAM_ADDRESS;
/*********************************************************************************
���Ա���������
*********************************************************************************/
 
/*********************************************************************************
�ڲ��������Ͷ�����
*********************************************************************************/

/*********************************************************************************
�ڲ�����������
*********************************************************************************/
static SystemErrName FRAM_WaitEepromStandbyState(void); //EEPROM��Ϊ����״̬
/*********************************************************************************
���ܴ��붨����
*********************************************************************************/
static void Delay ( unsigned long temp )
{
 while (temp -- );
}
/**********************************************
��������		  
����������		   
�����б�		   
�������б�
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 	SPI/I2S2_RX  
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
  
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);//�ǳ���Ҫ,û�����RESER���ᵼ�����߲����ͷ� 
  
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
��������		  
����������		   
�����б�		   
�������б�
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 	SPI/I2S2_RX  
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
* Input          : - data : Ҫд�������
*                  - WriteAddr : EEPROM��Ҫд�����ݵ��׵�ַ.
* Output         : None
* Return         : None
*******************************************************************************/
SystemErrName FRAM_ByteWrite(unsigned char data, unsigned short WriteAddr)//дһ���ֽڵ�EEPROM
{
 SystemErrName err; 
 unsigned long itemp = 0;
 
 err =  FRAM_WaitEepromStandbyState();//EEPROM��Ϊ����״̬
 if(NO_ERR!= err){return err;} 

 I2C_GenerateSTART(I2C1, ENABLE);//���� I2Cx���� START����

 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) //������һ�� I2C�¼��Ƿ���������¼�
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0; 

 I2C_Send7bitAddress(I2C1, I2C_SlaverAddress, I2C_Direction_Transmitter);//��ָ���Ĵ� I2C�豸���͵�ַ��,ѡ���ͷ���

 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))//������һ�� I2C�¼��Ƿ���������¼�
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0;       

 I2C_SendData(I2C1, WriteAddr);//ͨ������ I2Cx���͵�ַ

 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//������һ�� I2C�¼��Ƿ���������¼�
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0; 

 I2C_SendData(I2C1, data); //ͨ������ I2Cx�������� /* Send the byte to be written */
   
 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//������һ�� I2C�¼��Ƿ���������¼�
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}//  itemp = 0; 

 I2C_GenerateSTOP(I2C1, ENABLE);//���� I2Cx���� STOP����
 
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
pBuffer��ָ��Ҫд�����������ָ��
WriteAddr��24c02��Ҫд�����ݵ��׵�ַ
NumByteToWrite��д����ֽ���
*******************************************************************************/
SystemErrName FRAM_PageWrite(unsigned char* pBuffer, 
                             unsigned short NumByteToWrite,
                             unsigned short WriteAddr)//д����һҳ������
{
 SystemErrName err; 
 unsigned long itemp = 0;
 
 err =  FRAM_WaitEepromStandbyState();//EEPROM��Ϊ����״̬
 if(NO_ERR!= err){return err;} 

 I2C_GenerateSTART(I2C1, ENABLE);//���� I2Cx���� START����
 
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) //������һ�� I2C�¼��Ƿ���������¼�
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0;  
  I2C_SlaverAddress = FRAM_ADDRESS & 0xF0;
  I2C_SlaverAddress += (WriteAddr >> 7) & 0x0E;
  I2C_Send7bitAddress(I2C1, I2C_SlaverAddress, I2C_Direction_Transmitter);//��ָ���Ĵ� I2C�豸���͵�ַ��,ѡ���ͷ���

  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))//������һ�� I2C�¼��Ƿ���������¼�
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0;

  I2C_SendData(I2C1, WriteAddr); //ͨ������ I2Cx���͵�ַ

  while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//������һ�� I2C�¼��Ƿ���������¼�
 {if(itemp++ > 0x4ffff){return Unsuccessful;}}
  itemp = 0;  

  while(NumByteToWrite--) 
  {
    I2C_SendData(I2C1, *pBuffer); //ͨ������ I2Cx��������
    pBuffer++; 
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//������һ�� I2C�¼��Ƿ���������¼�
   {
     if(itemp++ > 0x4ffffff)
     {return Unsuccessful;}
   }
    itemp = 0; 
  }
  I2C_GenerateSTOP(I2C1, ENABLE);//���� I2Cx���� STOP����
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
pBuffer��ָ��Ҫ����������ݵ������ָ��
ReadAddr��24c02��Ҫ�������ݵ��׵�ַ
NumByteToRead���������ֽ���
*******************************************************************************/
SystemErrName FRAM_PageRead(void* Buffer, 
                            unsigned short NumByteToRead,
                            unsigned short ReadAddr)//��EEPROM�����ݶ��뻺����
{ 
 SystemErrName err; 
 unsigned long itemp = 0;
 unsigned char* pBuffer;
 pBuffer = Buffer;
 err =  FRAM_WaitEepromStandbyState();//EEPROM��Ϊ����״̬
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
      {I2C_AcknowledgeConfig(I2C1, DISABLE);}//ʹ�ܻ���ʧ��ָ�� I2C��Ӧ����
      if(NumByteToRead == 1)
      {I2C_GenerateSTOP(I2C1, ENABLE);}//���� I2Cx���� STOP����
      *pBuffer = I2C_ReceiveData(I2C1);//����ͨ�� I2Cx������յ�����
      pBuffer++; 
      NumByteToRead--;    
   }
   if(itemp++ > 0x4ffff){return Unsuccessful;}
  }
 I2C_AcknowledgeConfig(I2C1, ENABLE);//ʹ�ܻ���ʧ��ָ�� I2C��Ӧ����
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
* Description    : ��������������д��EEPROM
* Input          : - pBuffer : ָ��Ҫд�����������ָ�� 
*                  - WriteAddr : EEPROM��Ҫд�����ݵ��׵�ַ.
*                  - NumByteToWrite : д����ֽ���.
* Output         : None
* Return         : None
pBuffer��
WriteAddr��
NumByteToWrite��д����ֽ���
*******************************************************************************/
SystemErrName FRAM_BufferWrite(void* Buffer, 
                               short NumByteToWrite,
                               unsigned short WriteAddr)//��������������д��EEPROM
{
 SystemErrName err = NO_ERR ; 
 unsigned char  NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
 unsigned char* pBuffer; 
 
 Addr = WriteAddr % FRAM_PageSize;//д���ַ��ÿҳ�ĵڼ�λ
 count = FRAM_PageSize - Addr;//�ڿ�ʼ��һҳҪд��ĸ���
 pBuffer = Buffer;
 
 if(Addr + NumByteToWrite < FRAM_PageSize )   // ���ݲ���ҳ
  { err = FRAM_PageWrite(pBuffer, NumByteToWrite, WriteAddr);}//д����һҳ������
 else//���ݲ���ҳ
  {
   NumByteToWrite -= count;
   NumOfPage = NumByteToWrite / FRAM_PageSize; //����Ҫд���ҳ��
   NumOfSingle = NumByteToWrite % FRAM_PageSize;//���¼��㲻��һҳ�ĸ���
   if(count != 0)//
   { 
    err = FRAM_PageWrite(pBuffer, count, WriteAddr);//����ʼ�Ŀռ�д��һҳ
    if(NO_ERR != err){return err;}
    WriteAddr += count;
    pBuffer += count;
   } 
   while(NumOfPage--)//Ҫд���ҳ��
   {
    err = FRAM_PageWrite(pBuffer, FRAM_PageSize, WriteAddr);//дһҳ������
    if(NO_ERR != err){return err;}
    WriteAddr += FRAM_PageSize;
    pBuffer += FRAM_PageSize; 
   }
   if(NumOfSingle != 0)//ʣ������С��һҳ
   {err = FRAM_PageWrite(pBuffer, NumOfSingle, WriteAddr);  }//д����һҳ������
  }
 return err;
}
/*******************************************************************************
* Function Name  : I2C_EE_BufferWrite
* Description    : ��������������д��EEPROM
* Input          : - pBuffer : ָ��Ҫд�����������ָ�� 
*                  - WriteAddr : EEPROM��Ҫд�����ݵ��׵�ַ.
*                  - NumByteToWrite : д����ֽ���.
* Output         : None
* Return         : None
pBuffer��
WriteAddr��
NumByteToWrite��д����ֽ���
*******************************************************************************/
SystemErrName FRAM_BufferRead(void* Buffer,
                              unsigned short NumByteToRead,
                              unsigned short ReadAddr)//��EEPROM�����ݶ��뻺����
{
 SystemErrName err = NO_ERR ; 
 unsigned char  NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
 unsigned char* pBuffer; 
 
 Addr = ReadAddr % FRAM_PageSize;//д���ַ��ÿҳ�ĵڼ�λ
 count = FRAM_PageSize - Addr;//�ڿ�ʼ��һҳҪд��ĸ���
 pBuffer = Buffer;
 
 if(Addr + NumByteToRead < FRAM_PageSize )   // ���ݲ���ҳ
  { err = FRAM_PageRead(pBuffer, NumByteToRead,ReadAddr);}//д����һҳ������
 else//���ݲ���ҳ
  {
   NumByteToRead -= count;
   NumOfPage = NumByteToRead / FRAM_PageSize; //����Ҫд���ҳ��
   NumOfSingle = NumByteToRead % FRAM_PageSize;//���¼��㲻��һҳ�ĸ���
   if(count != 0)//
   { 
    err = FRAM_PageRead(pBuffer, count, ReadAddr);//����ʼ�Ŀռ�д��һҳ
    if(NO_ERR != err)
    {return err;}
    ReadAddr += count;
    pBuffer += count;
   } 
   while(NumOfPage--)//Ҫд���ҳ��
   {
    err = FRAM_PageRead(pBuffer, FRAM_PageSize, ReadAddr);//дһҳ������
    if(NO_ERR != err){return err;}
    ReadAddr += FRAM_PageSize;
    pBuffer += FRAM_PageSize; 
   }
   if(NumOfSingle != 0)//ʣ������С��һҳ
   {err = FRAM_PageRead(pBuffer, NumOfSingle, ReadAddr);  }//д����һҳ������
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
SystemErrName FRAM_WaitEepromStandbyState(void) //EEPROM��Ϊ����״̬     
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
  I2C_GenerateSTART(I2C1, ENABLE);//���� I2Cx���� START����
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) //������һ�� I2C�¼��Ƿ���������¼�
  {if(itemp++ > 0x4fff)
  {return Unsuccessful;}}
  itemp = 0;
  I2C_Send7bitAddress(I2C1, I2C_SlaverAddress, I2C_Direction_Transmitter);//��ָ���Ĵ� I2C�豸���͵�ַ�� ,ѡ���ͷ���
  Delay ( 0x4ff );
  I2C_ClearFlag(I2C2, I2C_FLAG_AF);//��� I2Cx��Ӧ������־λ
  I2C_ReadRegister(I2C1, I2C_Register_SR1);//��ȡָ���� I2C�Ĵ��� I2C_SR1 ��������ֵ
  if(itemp1++ > 0x4fff)
  {
    I2C_Rset_FRAM24CLXX( );
  return Unsuccessful;
  }
 }while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));//��ַ���ͽ���
 I2C_ClearFlag(I2C1, I2C_FLAG_AF);//��� I2Cx��Ӧ������־λ
 
 I2C_GenerateSTOP(I2C1, ENABLE);//���� I2Cx���� STOP����
 while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
 { 
  if(itemp++ > 0x4fff)
  { return Unsuccessful;}
 }
 return NO_ERR;
}
/*********************************************************************************
��������		   
����������	   
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
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
��������		   
����������	
��ڲ���������	   
                   
				  
���ڲ���������
��������ֵ������
����˵���� 
*********************************************************************************/
SystemErrName FRAM_MultipleWrite(void* Buffer,
                                 unsigned short NumByteToWrite,
                                 unsigned short WriteAddr)
{
  SystemErrName err = NO_ERR; 
  unsigned char itemp = 0 ;
  while (itemp++ < 3)
  {
   err = FRAM_BufferWrite(Buffer, WriteAddr, NumByteToWrite);//��������������д��EEPROM
   if(NO_ERR == err)
   {break;}
  }
  return err;
}
/***********************************END**********************************************/


