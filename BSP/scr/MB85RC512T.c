
/*********************************************************************************
//������: Operation for SI446x

//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\inc\MB85RC512T.h"  
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
SystemErrName  FRAM_I2C_Rset(void)
{
  unsigned char cnt = 0;
  unsigned long itemp;
  
  while(cnt ++ < 9)
  {
    I2C_GenerateSTART(I2C1, ENABLE);//���� I2Cx���� START����
    itemp = 0;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) //������һ�� I2C�¼��Ƿ���������¼�
    {if(itemp++ > 0x4ffff){return Unsuccessful;}}
    
  }
  return NO_ERR;
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
  SystemErrName  err =  NO_ERR;
  unsigned long itemp = 0 ;
  unsigned long itemp1 = 0 ;
  
  I2C_Config_FRAM24CLXX( );
  
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
  {if(itemp++ > 0x4fff)
  {return Unsuccessful;}}
  
  err = FRAM_I2C_Rset( );
  if(NO_ERR != err)
  {return err;}
  
  itemp = 0;
  do
  {
    I2C_GenerateSTOP(I2C1, ENABLE);//���� I2Cx���� STOP����
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
    { 
      if(itemp++ > 0x4fff)
      { return Unsuccessful;}
    }
    I2C_GenerateSTART(I2C1, ENABLE);//���� I2Cx���� START����
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) //������һ�� I2C�¼��Ƿ���������¼�
    {
      if(itemp++ > 0x4fff)
      {return Unsuccessful;}
    }
    itemp = 0;
    I2C_Send7bitAddress(I2C1, I2C_SlaverAddress, I2C_Direction_Transmitter);//��ָ���Ĵ� I2C�豸���͵�ַ�� ,ѡ���ͷ���
    Delay ( 0x4ff );
    I2C_ClearFlag(I2C2, I2C_FLAG_AF);//��� I2Cx��Ӧ������־λ
    //I2C_ReadRegister(I2C1, I2C_Register_SR1);//��ȡָ���� I2C�Ĵ��� I2C_SR1 ��������ֵ
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

 I2C_SendData(I2C1, WriteAddr>>8);//ͨ������ I2Cx���͵�ַ

 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//������һ�� I2C�¼��Ƿ���������¼�
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
WriteAddr����Ҫд�����ݵ��׵�ַ
NumByteToWrite��д����ֽ���
*******************************************************************************/
SystemErrName FRAM_PageWrite(unsigned char* pBuffer, 
                             unsigned short NumByteToWrite,
                             unsigned short WriteAddr)//д����һҳ������
{
  SystemErrName err; 
  unsigned long itemp;
  
  err =  FRAM_WaitEepromStandbyState();//EEPROM��Ϊ����״̬
  if(NO_ERR!= err){return err;} 
  
  I2C_GenerateSTART(I2C1, ENABLE);//���� I2Cx���� START����
  itemp = 0;  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) //������һ�� I2C�¼��Ƿ���������¼�
  {
    if(itemp++ > 0x4ffff)
    {return Unsuccessful;}
  }
  
  I2C_Send7bitAddress(I2C1, I2C_SlaverAddress, I2C_Direction_Transmitter);//��ָ���Ĵ� I2C�豸���͵�ַ��,ѡ���ͷ���
  itemp = 0;  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))//������һ�� I2C�¼��Ƿ���������¼�
  {
    if(itemp++ > 0x4ffff)
    {return Unsuccessful;}
  }
  
  I2C_SendData(I2C1, WriteAddr >> 8); //ͨ������ I2Cx���͵�ַ
  itemp = 0;  
  while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//������һ�� I2C�¼��Ƿ���������¼�
  {
    if(itemp++ > 0x4ffff)
    {return Unsuccessful;}
  }
  
  I2C_SendData(I2C1, WriteAddr); //ͨ������ I2Cx���͵�ַ
  itemp = 0;  
  while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))//������һ�� I2C�¼��Ƿ���������¼�
  {
    if(itemp++ > 0x4ffff)
    {return Unsuccessful;}
  }
  
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
  itemp = 0; 
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
SystemErrName FRAM_PageRead(unsigned char* Buffer, 
                            unsigned short NumByteToRead,
                            unsigned short ReadAddr)//��EEPROM�����ݶ��뻺����
{ 
  SystemErrName err; 
  unsigned long itemp ;
  err =  FRAM_WaitEepromStandbyState();//EEPROM��Ϊ����״̬
  if(NO_ERR!= err)
  {return err;}
  
  I2C_GenerateSTART(I2C1, ENABLE);//Send START condition 
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))//Test on EV5 and clear it 
  {if(itemp++ > 0x4fff)
  {return Unsuccessful;}}
  
  if(NumByteToRead==1)  
  {I2C_AcknowledgeConfig(I2C1, DISABLE);}//In the case of a single data transfer disable ACK before reading the data 
  
  I2C_Send7bitAddress(I2C1, I2C_SlaverAddress, I2C_Direction_Transmitter);//Transmits the address byte to select the slave device 
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))//Test on EV6 and clear it 
  {
    if(itemp++ > 0x4fff)
    {return Unsuccessful;}
  }
  
  I2C_SendData(I2C1, ReadAddr >> 8); // Send the EEPROM's internal address to write to 
  itemp = 0; 
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))///Test on EV8 and clear it 
  {
    if(itemp++ > 0x4fff)
    {return Unsuccessful;}
  }
  
  I2C_SendData(I2C1, ReadAddr & 0x00FF); // Send the EEPROM's internal address to write to
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))///Test on EV8 and clear it 
  {
    if(itemp++ > 0x4fff)
    {return Unsuccessful;}
  }
  
  I2C_GenerateSTART(I2C1, ENABLE);// Send STRAT condition a second time *
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))//Test on EV5 and clear it 
  {
    if(itemp++ > 0x4fff)
    {return Unsuccessful;}
  }
  
  I2C_Send7bitAddress(I2C1, I2C_SlaverAddress, I2C_Direction_Receiver);// Send EEPROM address for read 
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))//Test on EV6 and clear it 
  {
    if(itemp++ > 0x4fff)
    {return Unsuccessful;}
  }
  
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
      *Buffer = I2C_ReceiveData(I2C1);//����ͨ�� I2Cx������յ�����
      Buffer++; 
      NumByteToRead--;    
    }
    if(itemp++ > 0x4ffff)
    {return Unsuccessful;}
  }
  I2C_AcknowledgeConfig(I2C1, ENABLE);//ʹ�ܻ���ʧ��ָ�� I2C��Ӧ����
  
  itemp = 0;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
  { 
    if(itemp++ > 0x4fff)
    { return Unsuccessful;}
  }
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 , DISABLE);
  return NO_ERR;
}
/***********************************END**********************************************/


