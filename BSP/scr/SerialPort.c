/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\SerialPort.h"
#include "..\inc\DriverPcb.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
const SerialPort_ConfigStruct  USART_Config[3]= {
  {USART1, DMA1_Channel5, DMA1_Channel4}, 
  {USART2, DMA1_Channel6, DMA1_Channel7},
  {USART3, DMA1_Channel3, DMA1_Channel2}
};
/*********************************************************************************
公共变量定义区
*********************************************************************************/
MbusPortStatus   MbusStatus = IDLE;
unsigned short   Mbus_IdleTime  = 0x00;
unsigned  short OverLoad_timer[4] = {0,0,0,0};
Bit_8_struct OverLoadFlg[4] = {0,0,0,0};
/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
私有变量定义区
*********************************************************************************/
static SerialPort_Struct   USART_Des[3];
static UsarBuff_Struct    USART_buff[USART_BuffNumber];
/*********************************************************************************
测试变量定义区
*********************************************************************************/
/*********************************************************************************
内部函数定义区
*********************************************************************************/

/*********************************************************************************
功能代码定义区
*********************************************************************************/
void GprsDisplay_func(void* buff );
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
void DeInit_UsartBuff(void)
{
  unsigned char itemp =0;
  while ( itemp < USART_BuffNumber)
  {
    USART_buff[itemp].CCR.Byte = 0; 
    USART_buff[itemp].CCR.Bit.idle = 1;
    USART_buff[itemp].port = Null_Port;
    itemp ++;
  }
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
unsigned long GetDataFromUsart (UsarBuff_Struct* buff,
                                unsigned char *pbuff,  
                                unsigned long buffsize)
{
  unsigned short itemp =0;
  unsigned long  number = 0;
  number = 0;
  do{
    if(P_NULL == buff->Prev)
    {break;}
    else
    {buff = buff->Prev;}
  }while (1);
  
  // copy 数据
  do{
    itemp = 0; 
    while (itemp < buff->length)
    {
      if(buffsize-- > 0)
      {pbuff[number++] = buff->data[itemp++];}
    }
    buff->CCR.Byte = 0;
    buff->CCR.Bit.idle = 1;
    if(P_NULL != buff->next)
    {buff = buff->next;}
    else
    {break;}
  }while (1);
  if(buffsize > 0)
  {pbuff[number] = '\0';}
  else
  {pbuff[--number] = '\0';}
  GprsDisplay_func(pbuff);
  return number;
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
unsigned long GetDataFromPort (CommPortNumber_Type port,
                               unsigned char *pbuff,  
                               unsigned long buffsize)
{
  unsigned short itemp =0;
  unsigned long  number = 0;
  UsarBuff_Struct* buff;
  //查找首反冲区；
  while ( itemp < USART_BuffNumber)
  {
    if((USART_buff[itemp].port == port)
       &&(1 == USART_buff[itemp].CCR.Bit.flg)
         &&(1 == USART_buff[itemp].CCR.Bit.end))
    {
      buff = &(USART_buff[itemp]);
      do{
        if(P_NULL == buff->Prev)
        {break;}
        else
        {buff = buff->Prev;}
      }while (1);
      
      // copy 数据
      do{
        itemp = 0; 
        while (itemp < buff->length)
        {
          if(buffsize-- > 0)
          {pbuff[number++] = buff->data[itemp++];}
        }
        buff->CCR.Byte = 0;
        buff->CCR.Bit.idle = 1;
        if(P_NULL != buff->next)
        {buff = buff->next;}
        else
        {break;}
      }while (1);
      return number; 
    }
    itemp ++;
  }
  return number; 
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
unsigned long GetDataFromUsartBuff (unsigned char *pbuff,  
                                    unsigned long buffsize, 
                                    CommPortNumber_Type* source)
{
  unsigned short itemp =0;
  unsigned long number = 0;
  UsarBuff_Struct* buff;  
  
  //查找首反冲区；
  while ( itemp < USART_BuffNumber)
  {
    if((1 == USART_buff[itemp].CCR.Bit.flg)
       &&(1 == USART_buff[itemp].CCR.Bit.end))
    {
      buff = &(USART_buff[itemp]);
      do{
        if(P_NULL == buff->Prev)
        {break;}
        else
        {buff = buff->Prev;}
      }while (1);
      
      // copy 数据
      do{
        itemp = 0; 
        while (itemp < buff->length)
        {
          if(buffsize-- > 0)
          {pbuff[number++] = buff->data[itemp++];}
        }
        buff->CCR.Byte = 0;
        buff->CCR.Bit.idle = 1;
        if(P_NULL != buff->next)
        {buff = buff->next;}
        else
        {break;}
      }while (1);
      *source = buff->port;
      return number; 
    }
    itemp ++;
  }
  *source = Null_Port;
  return number; 
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
void FreeBuff_USART(void** buff)
{
  unsigned short itemp = 0;
  while (itemp < USART_BuffNumber)
  {
    if(*buff == &(USART_buff[itemp]))
    { break ;}
    else if(*buff == &(USART_buff[itemp].data))
    {break ;}
    itemp ++;
  }
  if(itemp < USART_BuffNumber)
  {
    ((UsarBuff_Struct*)(*buff))->CCR.Byte = 0; 
    ((UsarBuff_Struct*)(*buff))->CCR.Bit.idle = 1;
    ((UsarBuff_Struct*)(*buff))->port = Null_Port;
    *buff = P_NULL;
  }
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
void FreeBuffFormPort(CommPortNumber_Type  port)
{
  unsigned short itemp = 0;
  while (itemp < USART_BuffNumber)
  {
    if(USART_buff[itemp].port == port)
    { 
      USART_buff[itemp].port = Null_Port;
      USART_buff[itemp].CCR.Byte = 0;
      USART_buff[itemp].CCR.Bit.idle = 1; 
    }
    itemp ++;
  }
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
void DeleteDataFromUsartBuff( UsarBuff_Struct* buff )
{
  unsigned short itemp = 0;
  UsarBuff_Struct* pbuff; 
  
  while (itemp < USART_BuffNumber)
  {
    if(buff == &(USART_buff[itemp]))
    { break ;}
    itemp ++;
  }
  if( USART_BuffNumber <= itemp)
  {return;}
  
  
  
  buff->CCR.Byte = 0x00;
  buff->length = 0x00;
  buff->CCR.Bit.idle = 0x01;
  
  pbuff = buff;
  while(P_NULL != pbuff->next)
  {
    pbuff = pbuff->next;
    pbuff->CCR.Byte = 0x00;
    pbuff->length = 0x00;
    pbuff->CCR.Bit.idle = 0x01;
  }
  
  pbuff = buff;
  while(P_NULL != pbuff->Prev)
  {
    pbuff = pbuff->Prev;
    pbuff->CCR.Byte = 0x00;
    pbuff->length = 0x00;
    pbuff->CCR.Bit.idle = 0x01;
  }
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
static UsarBuff_Struct* Get_UsartBuff (CommPortNumber_Type  port)
{
  unsigned char itemp =0;
  while ( itemp < USART_BuffNumber)
  {
    if(1 == USART_buff[itemp].CCR.Bit.idle)
    {
      USART_buff[itemp].CCR.Bit.idle = 0;
      USART_buff[itemp].port = port;
      USART_buff[itemp].next = P_NULL;
      USART_buff[itemp].Prev = P_NULL;
      return &(USART_buff[itemp]);
    }
    itemp ++;
  }
  return  P_NULL;
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
void TXE_Interrupt_USART (unsigned char Usart_NO)
{
  if(USART_Des[Usart_NO].TX_CNT > 1)
  {
    USART_Config[Usart_NO].PHY->DR = *USART_Des[Usart_NO].TX_P ++;
    USART_Des[Usart_NO].TX_CNT --;
  } 
  else if(1 ==USART_Des[Usart_NO].TX_CNT)
  {
    USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_TXE, DISABLE); 
    USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_TC, ENABLE);
    USART_Config[Usart_NO].PHY->DR = *USART_Des[Usart_NO].TX_P ++; 
    USART_Des[Usart_NO].TX_CNT = 0;
  }
  else
  {
    USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_TXE, DISABLE); 
    USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_TC, DISABLE);
    USART_Des[Usart_NO].Status = Idle;
  }
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
void TC_Interrupt_USART (unsigned char Usart_NO)
{
  USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_TC, DISABLE);
  if(Usart_NO == Mbus_Usart_NO)
  {MbusStatus = IDLE;}
  USART_Des[Usart_NO].Status = Idle;
  if(P_NULL != USART_Des[Usart_NO].SendEnd_CallBack)
  {
    USART_Des[Usart_NO].SendEnd_CallBack(USART_Des[Usart_NO].PortPV ,USART_Des[Usart_NO].SendEnd_CallBack_arg);
    USART_Des[Usart_NO].SendEnd_CallBack = P_NULL;
  }
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
void RXNE_Interrupt_USART (unsigned char Usart_NO)
{
  if(P_NULL != USART_Des[Usart_NO].RX_P)
  {
    if(USART_Des[Usart_NO].RX_CNT >= USART_BuffLength )
    {
      USART_Des[Usart_NO].RX_P->CCR.Bit.org = 1;
      USART_Des[Usart_NO].RX_P->CCR.Bit.flg =1;
      USART_Des[Usart_NO].RX_P->length =USART_BuffLength;
      USART_Des[Usart_NO].RX_P->next = Get_UsartBuff (USART_Des[Usart_NO].PortPV);
      USART_Des[Usart_NO].RX_P->next->Prev = USART_Des[Usart_NO].RX_P;
      USART_Des[Usart_NO].RX_P = USART_Des[Usart_NO].RX_P->next;
      USART_Des[Usart_NO].RX_CNT = 0;
      USART_Des[Usart_NO].RX_P->data[USART_Des[Usart_NO].RX_CNT ++] = USART_Config[Usart_NO].PHY->DR;
    }
    else
    {USART_Des[Usart_NO].RX_P->data[USART_Des[Usart_NO].RX_CNT ++] = USART_Config[Usart_NO].PHY->DR;}   
  }
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
void IDLE_Interrupt_USART (unsigned char Usart_NO)
{ 
  DMA_InitTypeDef         DMA_InitStructure;
  BaseType_t xHigherPriorityTaskWoken = pdTRUE;
  
  DMA_InitStructure.DMA_PeripheralBaseAddr = USART_Config[Usart_NO].PHY->DR;   //清除空闲中断 
  
  if(P_NULL != USART_Des[Usart_NO].RX_P)
  {
    if(ByIT == USART_Des[Usart_NO].ReceiveMod_SV)
    {USART_Des[Usart_NO].RX_P->length = USART_Des[Usart_NO].RX_CNT;}
    else if(ByDMA == USART_Des[Usart_NO].ReceiveMod_SV)
    {USART_Des[Usart_NO].RX_P->length = USART_BuffLength - DMA_GetCurrDataCounter(USART_Config[Usart_NO].DMAy_ChannelRx);}
    USART_Des[Usart_NO].RX_P->CCR.Bit.flg = 1;
    USART_Des[Usart_NO].RX_P->CCR.Bit.end = 1;
    
    if(0 == Usart_NO) //
    {
      if(NULL != ManualQueue)
      {
        if(pdFALSE == xQueueIsQueueFullFromISR( ManualQueue))
        {xQueueSendFromISR (ManualQueue,(void *)&(USART_Des[Usart_NO].RX_P),&xHigherPriorityTaskWoken);}
        else
        {DeleteDataFromUsartBuff(USART_Des[Usart_NO].RX_P);}
      }
      else
      {DeleteDataFromUsartBuff(USART_Des[Usart_NO].RX_P);}
      
    }else if(1 == Usart_NO)
    {
      if(NULL != MbusReceiveQueue)
      {
        if(pdFALSE == xQueueIsQueueFullFromISR( MbusReceiveQueue))
        {xQueueSendFromISR (MbusReceiveQueue,(void *)&(USART_Des[Usart_NO].RX_P),&xHigherPriorityTaskWoken);}
        else
        {DeleteDataFromUsartBuff(USART_Des[Usart_NO].RX_P);} 
      }
      else
      {DeleteDataFromUsartBuff(USART_Des[Usart_NO].RX_P);}
    }else if(2 == Usart_NO)
    {
      if(NULL != GprsQueue)
      {
        if(pdFALSE == xQueueIsQueueFullFromISR(GprsQueue))
        {xQueueSendFromISR (GprsQueue,(void *)&(USART_Des[Usart_NO].RX_P),&xHigherPriorityTaskWoken);}
        else
        {DeleteDataFromUsartBuff(USART_Des[Usart_NO].RX_P);} 
      }
      else
      {DeleteDataFromUsartBuff(USART_Des[Usart_NO].RX_P);}
    }
  }
  USART_Des[Usart_NO].RX_P = Get_UsartBuff(USART_Des[Usart_NO].PortPV);
  USART_Des[Usart_NO].RX_CNT = 0;
  if(P_NULL != USART_Des[Usart_NO].RX_P)
  {
    USART_Des[Usart_NO].RX_P->CCR.Bit.first = 1;
    if(ByDMA == USART_Des[Usart_NO].ReceiveMod_SV)
    {
      RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1, ENABLE);
      DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART_Config[Usart_NO].PHY->DR));
      DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
      DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
      DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
      DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
      DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
      DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
      DMA_InitStructure.DMA_BufferSize = (uint16_t)USART_BuffLength;
      DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(USART_Des[Usart_NO].RX_P->data);
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
      DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
      DMA_DeInit(USART_Config[Usart_NO].DMAy_ChannelRx);
      DMA_Init(USART_Config[Usart_NO].DMAy_ChannelRx, &DMA_InitStructure);
      DMA_ITConfig(USART_Config[Usart_NO].DMAy_ChannelRx, DMA_IT_HT, DISABLE);
      DMA_ITConfig(USART_Config[Usart_NO].DMAy_ChannelRx, DMA_IT_TC| DMA_IT_TE,ENABLE);
      /* Enable the USART Rx DMA request */
      USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_RXNE, DISABLE); 
      USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_IDLE, ENABLE);
      USART_DMACmd(USART_Config[Usart_NO].PHY, USART_DMAReq_Rx, ENABLE);
      /* Enable the DMA channel */
      DMA_Cmd(USART_Config[Usart_NO].DMAy_ChannelRx, ENABLE);
    }
    if(ByIT == USART_Des[Usart_NO].ReceiveMod_SV)
    {
      USART_DMACmd(USART_Config[Usart_NO].PHY, USART_DMAReq_Rx, DISABLE);
      USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_IDLE, ENABLE);
      USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_RXNE, ENABLE);
    }
  }
  else
  {
    USART_DMACmd(USART_Config[Usart_NO].PHY,   USART_DMAReq_Rx, DISABLE);
    USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_RXNE, DISABLE);
    USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_IDLE, ENABLE);
  }
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
void DMA_TE_Interrupt_USART (unsigned char Usart_NO)
{
  DMA_InitTypeDef         DMA_InitStructure; 
  if(ByDMA == USART_Des[Usart_NO].ReceiveMod_SV) 
  {
    DeleteDataFromUsartBuff(USART_Des[Usart_NO].RX_P);
    USART_Des[Usart_NO].RX_P = Get_UsartBuff (USART_Des[Usart_NO].PortPV); 
    if(P_NULL != USART_Des[Usart_NO].RX_P)
    {
      RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1, ENABLE);
      DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART_Config[Usart_NO].PHY->DR));
      DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
      DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
      DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
      DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
      DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
      DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
      DMA_InitStructure.DMA_BufferSize = (uint16_t)USART_BuffLength;
      DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(USART_Des[Usart_NO].RX_P->data);
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
      DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
      DMA_DeInit(USART_Config[Usart_NO].DMAy_ChannelRx);
      DMA_Init(USART_Config[Usart_NO].DMAy_ChannelRx, &DMA_InitStructure);
      DMA_ITConfig(USART_Config[Usart_NO].DMAy_ChannelRx, DMA_IT_HT, DISABLE);
      DMA_ITConfig(USART_Config[Usart_NO].DMAy_ChannelRx, DMA_IT_TC| DMA_IT_TE,ENABLE);
      /* Enable the USART Rx DMA request */
      
      USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_RXNE, DISABLE); 
      USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_IDLE, ENABLE);
      USART_DMACmd(USART_Config[Usart_NO].PHY, USART_DMAReq_Rx, ENABLE);
      /* Enable the DMA channel */
      DMA_Cmd(USART_Config[Usart_NO].DMAy_ChannelRx, ENABLE);
    }
    else
    {USART_DMACmd(USART_Config[Usart_NO].PHY, USART_DMAReq_Rx, DISABLE);}
    
  }
  else
  { 
    USART_DMACmd(USART_Config[Usart_NO].PHY, USART_DMAReq_Rx, DISABLE);
    USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_RXNE, DISABLE); 
    USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_IDLE, ENABLE);
  }
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
void DMA_TC_Interrupt_USART (unsigned char Usart_NO)
{
  DMA_InitTypeDef         DMA_InitStructure; 
  if(ByDMA == USART_Des[Usart_NO].ReceiveMod_SV) 
  {
    if(P_NULL == USART_Des[Usart_NO].RX_P)
    {USART_Des[Usart_NO].RX_P = Get_UsartBuff (USART_Des[Usart_NO].PortPV);}
    else
    {
      USART_Des[Usart_NO].RX_P->CCR.Bit.org = 1;
      USART_Des[Usart_NO].RX_P->CCR.Bit.flg =1;
      USART_Des[Usart_NO].RX_P->length = USART_BuffLength - DMA_GetCurrDataCounter(USART_Config[Usart_NO].DMAy_ChannelRx);
      USART_Des[Usart_NO].RX_P->next = Get_UsartBuff (USART_Des[Usart_NO].PortPV);
      USART_Des[Usart_NO].RX_P->next->Prev = USART_Des[Usart_NO].RX_P;
      USART_Des[Usart_NO].RX_P = USART_Des[Usart_NO].RX_P->next;
      USART_Des[Usart_NO].RX_CNT = 0;
    }
    if(P_NULL != USART_Des[Usart_NO].RX_P)
    {
      RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1, ENABLE);
      DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART_Config[Usart_NO].PHY->DR));
      DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
      DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
      DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
      DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
      DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
      DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
      DMA_InitStructure.DMA_BufferSize = (uint16_t)USART_BuffLength;
      DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(USART_Des[Usart_NO].RX_P->data);
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
      DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
      DMA_DeInit(USART_Config[Usart_NO].DMAy_ChannelRx);
      DMA_Init(USART_Config[Usart_NO].DMAy_ChannelRx, &DMA_InitStructure);
      DMA_ITConfig(USART_Config[Usart_NO].DMAy_ChannelRx, DMA_IT_HT, DISABLE);
      DMA_ITConfig(USART_Config[Usart_NO].DMAy_ChannelRx, DMA_IT_TC| DMA_IT_TE,ENABLE);
      /* Enable the USART Rx DMA request */
      USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_RXNE, DISABLE); 
      USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_IDLE, ENABLE);
      USART_DMACmd(USART_Config[Usart_NO].PHY, USART_DMAReq_Rx, ENABLE);
      /* Enable the DMA channel */
      DMA_Cmd(USART_Config[Usart_NO].DMAy_ChannelRx, ENABLE);
    }
    else
    {USART_DMACmd(USART_Config[Usart_NO].PHY, USART_DMAReq_Rx, DISABLE);}
    
  }
  else
  { 
    USART_DMACmd(USART_Config[Usart_NO].PHY, USART_DMAReq_Rx, DISABLE);
    USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_RXNE, DISABLE); 
    USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_IDLE, ENABLE);
  }
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
SystemErrName Launch_USART( CommPortNumber_Type Channel, 
                           unsigned long BaudRate,
                           SerialPort_Parity  Parity, 
                           SerialPort_StopBits StopBits,
                           SerialPort_ReceiveMod   mod )
{
  unsigned char Usart_NO = 0;
  SystemErrName err = NO_ERR; 
  GPIO_InitTypeDef        GPIO_InitStructure;
  NVIC_InitTypeDef        NVIC_InitStructure;
  USART_InitTypeDef       USART_InitStruct;
  EXTI_InitTypeDef        EXTI_InitStruct;
  
  switch (Channel)
  {
  case Rs232_Port:
    {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE);
      Usart_NO = 0; 
      DMA1->IFCR |= 0x0000000f << 12;
      DMA1->IFCR |= 0x0000000f << 16;
      NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
      NVIC_Init(&NVIC_InitStructure); 
      NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
      NVIC_Init(&NVIC_InitStructure); 
      
      NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
      
      NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);

      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
      NVIC_InitStructure.NVIC_IRQChannel =  USART1_IRQn;
      
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
      GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
      GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
      GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
      GPIO_InitStructure.GPIO_PuPd  =  GPIO_PuPd_NOPULL;
      GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;	     
      GPIO_Init(GPIOB, &GPIO_InitStructure);
      break;
    }
  case Mbus_Port1:
  case Mbus_Port2:
  case Mbus_Port3:
  case Mbus_Port4:
    {
      FreeBuffFormPort(Mbus_Port1);
      FreeBuffFormPort(Mbus_Port2);
      FreeBuffFormPort(Mbus_Port3);
      FreeBuffFormPort(Mbus_Port4);
      Usart_NO = 1;
      RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);
      DMA1->IFCR |= 0x0000000f << 20;
      DMA1->IFCR |= 0x0000000f << 24;
      NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
      NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
      
      NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn ;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure); 
      NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn ;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure); 
      
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
      NVIC_InitStructure.NVIC_IRQChannel =  USART2_IRQn;
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE); 
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
      
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;	     
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;   // OverLoad 
      GPIO_Init(GPIOA, &GPIO_InitStructure); 
      
      GPIOA->BSRRL = GPIO_Pin_1;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;    
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;    //USART2_RX
      GPIO_Init(GPIOA, &GPIO_InitStructure); 
      
      Mbus_CH1_OFF( );
      Mbus_CH2_OFF( );
      Mbus_CH3_OFF( );
      Mbus_CH4_OFF( ); 
      
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;  //CH2_EN	CH1_EN     
      GPIO_Init(GPIOB, &GPIO_InitStructure);      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;   //CH4_EN  CH3_EN
      GPIO_Init(GPIOC, &GPIO_InitStructure);      
      OpenSingleMbus (Channel);
      MbusStatus = IDLE;
      
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;    //RX-P  RX-N
      GPIO_Init(GPIOA, &GPIO_InitStructure);  
      SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource5 | EXTI_PinSource6);
      EXTI_InitStruct.EXTI_Line = EXTI_Line6 | EXTI_Line5;
      EXTI_InitStruct.EXTI_LineCmd = ENABLE;
      EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
      EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
      EXTI_Init (&EXTI_InitStruct);
      NVIC_InitStructure.NVIC_IRQChannel =  EXTI9_5_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
      
      NVIC_InitStructure.NVIC_IRQChannel =  USART2_IRQn;
      break;
    }   
  case USART3_Port:
    {
      FreeBuffFormPort(USART3_Port);
      Usart_NO = 2;
      DMA1->IFCR |= 0x0000000f << 4;
      DMA1->IFCR |= 0x0000000f << 8;
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, DISABLE);
      NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn ;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
      NVIC_Init(&NVIC_InitStructure); 
      NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn ;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
      NVIC_Init(&NVIC_InitStructure); 
      
      NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn ;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure); 
      NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn ;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure); 

      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
      NVIC_InitStructure.NVIC_IRQChannel =  USART3_IRQn;      
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
      GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
      GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
      GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd  =  GPIO_PuPd_NOPULL;
      GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;	     
      GPIO_Init(GPIOC, &GPIO_InitStructure);
      break;
    } 
  default :
    {return IllegalParam;}
  }
  
  USART_Des[Usart_NO].BaudRate = BaudRate;
  USART_Des[Usart_NO].Parity = Parity;
  USART_Des[Usart_NO].StopBits = StopBits;
  USART_Des[Usart_NO].PortPV = Channel;
  USART_Des[Usart_NO].Status = Idle; 
  USART_Des[Usart_NO].TX_P  = P_NULL;
  USART_Des[Usart_NO].TX_CNT = 0;
  USART_Des[Usart_NO].RX_P = P_NULL;
  USART_Des[Usart_NO].RX_CNT = 0 ;  
  USART_Des[Usart_NO].SendEnd_CallBack = P_NULL;
  USART_Des[Usart_NO].SendEnd_CallBack_arg = P_NULL;  
  
  USART_DeInit(USART_Config[Usart_NO].PHY);
  USART_InitStruct.USART_BaudRate = BaudRate;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode = USART_Mode_Rx |  USART_Mode_Tx;
  USART_InitStruct.USART_Parity = Parity; 
  USART_InitStruct.USART_StopBits= StopBits;
  if((Parity_Even == Parity)||(Parity_Odd == Parity))
  {USART_InitStruct.USART_WordLength = USART_WordLength_9b;}
  else
  {USART_InitStruct.USART_WordLength = USART_WordLength_8b;}
  USART_Init(USART_Config[Usart_NO].PHY, &USART_InitStruct);
  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  USART_Des[Usart_NO].ReceiveMod_SV = Polling;
  if(ByIT ==  mod)
  {activateReceiveByIT_USART(Usart_NO);}
  else if(ByDMA ==  mod)
  {activateReceiveByDMA_USART(Usart_NO); }
  
  USART_Cmd(USART_Config[Usart_NO].PHY, ENABLE);
  return err; 
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
SystemErrName activateReceiveByIT_USART (unsigned char Usart_NO)
{
  if(ByIT == USART_Des[Usart_NO].ReceiveMod_SV)
  {
    return NO_ERR;
  }
  
  if(P_NULL != USART_Des[Usart_NO].RX_P)
  {
    if(ByDMA == USART_Des[Usart_NO].ReceiveMod_SV)
    {
      USART_Des[Usart_NO].RX_P->length = USART_BuffLength - DMA_GetCurrDataCounter(USART_Config[Usart_NO].DMAy_ChannelRx);
      if(0 != USART_Des[Usart_NO].RX_P->length)
      {
        USART_Des[Usart_NO].RX_P->CCR.Bit.flg = 1;
        USART_Des[Usart_NO].RX_P->CCR.Bit.end = 1;
      }
    }
  }
  
  USART_Des[Usart_NO].RX_P = Get_UsartBuff (USART_Des[Usart_NO].PortPV);
  USART_Des[Usart_NO].RX_CNT =0;
  if(P_NULL == USART_Des[Usart_NO].RX_P)
  {return Nothing;}
  USART_Des[Usart_NO].RX_P->CCR.Bit.first = 1;
  USART_DMACmd(USART_Config[Usart_NO].PHY, USART_DMAReq_Rx, DISABLE);
  USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_IDLE, ENABLE);
  USART_Des[Usart_NO].ReceiveMod_SV = ByIT;
  return NO_ERR;
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
SystemErrName activateReceiveByDMA_USART (unsigned char Usart_NO)
{ 
  DMA_InitTypeDef         DMA_InitStructure;
  
  if(ByDMA == USART_Des[Usart_NO].ReceiveMod_SV)
  {return NO_ERR;}
  
  if(P_NULL != USART_Des[Usart_NO].RX_P)
  {
    if((ByIT == USART_Des[Usart_NO].ReceiveMod_SV)
       &&(0 != USART_Des[Usart_NO].RX_CNT ))
    {
      USART_Des[Usart_NO].RX_P->length = USART_Des[Usart_NO].RX_CNT;
      USART_Des[Usart_NO].RX_P->CCR.Bit.flg = 1;
      USART_Des[Usart_NO].RX_P->CCR.Bit.end = 1;
    }
  }
  
  USART_Des[Usart_NO].RX_P = Get_UsartBuff (USART_Des[Usart_NO].PortPV);
  USART_Des[Usart_NO].RX_CNT = 0;
  if(P_NULL == USART_Des[Usart_NO].RX_P)
  {return Nothing;}
  USART_Des[Usart_NO].RX_P->CCR.Bit.first = 1;
  
  RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1, ENABLE);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART_Config[Usart_NO].PHY->DR));
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_InitStructure.DMA_BufferSize = (uint16_t)USART_BuffLength;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(USART_Des[Usart_NO].RX_P->data);
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_DeInit(USART_Config[Usart_NO].DMAy_ChannelRx);
  DMA_Init(USART_Config[Usart_NO].DMAy_ChannelRx, &DMA_InitStructure);
  DMA_ITConfig (USART_Config[Usart_NO].DMAy_ChannelRx, DMA_IT_HT, DISABLE);
  DMA_ITConfig (USART_Config[Usart_NO].DMAy_ChannelRx, DMA_IT_TC| DMA_IT_TE, ENABLE);
  /* Enable the USART Rx DMA request */
  USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_RXNE, DISABLE); 
  USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_IDLE, ENABLE);
  USART_DMACmd(USART_Config[Usart_NO].PHY, USART_DMAReq_Rx, ENABLE);
  /* Enable the DMA channel */
  DMA_Cmd(USART_Config[Usart_NO].DMAy_ChannelRx, ENABLE);
  USART_Des[Usart_NO].ReceiveMod_SV = ByDMA;
  return NO_ERR;
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
static SystemErrName UsartstartSend (CommPortNumber_Type Channel,unsigned char* Usart_NO)
{
  SystemErrName err;
  switch (Channel)
  {
  case Rs232_Port:
    {
      *Usart_NO = 0;
      if(Busy == USART_Des[0].Status)
      {return PortBusy;}
      USART_Des[0].Status = Busy;
      err = NO_ERR;
      break;
    }
  case Mbus_Port1:
  case Mbus_Port2:
  case Mbus_Port3:
  case Mbus_Port4:
    {
      *Usart_NO = 1;
      if(Busy == USART_Des[1].Status)
      {return PortBusy;}
      MbusStatus = TX; 
      USART_Des[1].Status = Busy;
      Mbus_IdleTime = 0;
      err = NO_ERR;
      break;
    }   
  case USART3_Port:
    {
      *Usart_NO = 2;
      if(Busy == USART_Des[2].Status)
      {return PortBusy;}
      USART_Des[2].Status = Busy;
      err = NO_ERR;
      break;
    } 
  default :
    {
      *Usart_NO = 255;
      err = IllegalParam;
      break;
    }
  }
  
  return err;
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
SystemErrName sendBuffByIT_USART (CommPortNumber_Type Channel,
                                  void * buff, 
                                  unsigned short length,
                                  SerialPort_SendEndCallBack_Type callBack,
                                  void *callBack_arg )
{
  unsigned char Usart_NO = 0;
  SystemErrName  err = NO_ERR;
  
  if(length > 0)
  {
    err = UsartstartSend (Channel, &Usart_NO);
    if(NO_ERR != err)
    {return err; }
    USART_Des[Usart_NO].SendEnd_CallBack = callBack;
    USART_Des[Usart_NO].SendEnd_CallBack_arg = callBack_arg;
    USART_Des[Usart_NO].TX_P = (unsigned char *)buff;
    USART_Des[Usart_NO].TX_CNT = length - 1;
    USART_Config[Usart_NO].PHY->DR = *USART_Des[Usart_NO].TX_P ++;
    USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_TXE, ENABLE);  
  }
  else
  {err = IllegalParam;}
  return err; 
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
SystemErrName sendStringByIT_USART (CommPortNumber_Type Channel,
                                    unsigned char *buff)
{
  unsigned short itmep = 0;
  while('\0'!= buff[itmep])
  {
    itmep ++ ;
    if(itmep >256 )
    {return IllegalParam;}
  }
  return sendBuffByIT_USART (Channel,buff, itmep,P_NULL,P_NULL);
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
SystemErrName sendBuffByDMA_USART (CommPortNumber_Type Channel,
                                   void *buff, 
                                   unsigned short length,
                                   SerialPort_SendEndCallBack_Type callBack,
                                   void *callBack_arg )
{
  unsigned char Usart_NO = 0;
  SystemErrName  err = NO_ERR;
  DMA_InitTypeDef         DMA_InitStructure;
  if(length > 0)
  {
    err = UsartstartSend (Channel, &Usart_NO);
    if(NO_ERR != err)
    {return err; }
    
    USART_Des[Usart_NO].SendEnd_CallBack = callBack;
    USART_Des[Usart_NO].SendEnd_CallBack_arg = callBack_arg;
    USART_Des[Usart_NO].TX_P = (unsigned char *)buff;
    USART_Des[Usart_NO].TX_CNT = length ;
    
    RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1, ENABLE);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART_Config[Usart_NO].PHY->DR));
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_BufferSize = (uint16_t)length;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)buff;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_DeInit(USART_Config[Usart_NO].DMAy_ChannelTx);
    DMA_Init(USART_Config[Usart_NO].DMAy_ChannelTx, &DMA_InitStructure);
    /* Enable the USART DMA requests */
    USART_DMACmd(USART_Config[Usart_NO].PHY, USART_DMAReq_Tx, ENABLE);
    /* Clear the TC bit in the SR register by writing 0 to it */
    USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_TXE, DISABLE); 
    USART_ITConfig(USART_Config[Usart_NO].PHY, USART_IT_TC, DISABLE);
    /* Enable the DMA USART Tx Channel */
    DMA_ITConfig (USART_Config[Usart_NO].DMAy_ChannelTx, DMA_IT_HT, DISABLE);
    DMA_ITConfig (USART_Config[Usart_NO].DMAy_ChannelTx, DMA_IT_TC| DMA_IT_TE, ENABLE);
    DMA_Cmd(USART_Config[Usart_NO].DMAy_ChannelTx, ENABLE);
  }
  else
  {err = IllegalParam;}
  /* Enable the DMA USART Rx channel */
  return err;
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
SystemErrName sendStringByDMA_USART (CommPortNumber_Type Channel, unsigned char *buff)
{
  unsigned short itmep = 0;
  while('\0'!= buff[itmep])
  {
    itmep ++ ;
    if(itmep >2048 )
    {return IllegalParam;}
  }
  return sendBuffByDMA_USART (Channel,buff, itmep, P_NULL, P_NULL);
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
void CloseAllMbus (void)
{
  GPIO_InitTypeDef        GPIO_InitStructure;
  NVIC_InitTypeDef        NVIC_InitStructure;
  EXTI_InitTypeDef        EXTI_InitStruct; 
  
  HVPower_OFF( );
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);;
  Mbus_CH1_OFF( );
  Mbus_CH2_OFF( );
  Mbus_CH3_OFF( );
  Mbus_CH4_OFF( );
 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;    //RX-P  RX-N
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource5 | EXTI_PinSource6);
  EXTI_InitStruct.EXTI_Line = EXTI_Line6 | EXTI_Line5;
  EXTI_InitStruct.EXTI_LineCmd = DISABLE;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_Init (&EXTI_InitStruct);
  
  EXTI_ClearFlag(EXTI_Line5);
  EXTI_ClearFlag(EXTI_Line6);
  NVIC_InitStructure.NVIC_IRQChannel =  EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);
  FreeBuffFormPort(Mbus_Port1);
  FreeBuffFormPort(Mbus_Port2);
  FreeBuffFormPort(Mbus_Port3);
  FreeBuffFormPort(Mbus_Port4);
  USART_Des[1].PortPV = Null_Port;
  USART_Des[1].Status = OFF;
  USART_Des[1].BaudRate = 0;
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
void OpenSingleMbus (CommPortNumber_Type Channel)
{
  if(0 == HVPower_state)
  {
    HVPower_ON( );
    vTaskDelay(_100ms * 5);
  }
  switch (Channel)
  {
  case Mbus_Port1 : 
    {
      Mbus_CH2_OFF( );
      Mbus_CH3_OFF( );
      Mbus_CH4_OFF( ); 
      Mbus_CH1_ON( );
      break;
    }
  case Mbus_Port2 : 
    {
      Mbus_CH1_OFF( );
      Mbus_CH3_OFF( );
      Mbus_CH4_OFF( );
      Mbus_CH2_ON( ); 
      break;
    }
  case Mbus_Port3 : 
    {
      Mbus_CH1_OFF( );
      Mbus_CH2_OFF( );
      Mbus_CH4_OFF( );
      Mbus_CH3_ON( );
      break;
    }
  case Mbus_Port4 : 
    {
      Mbus_CH1_OFF( );
      Mbus_CH2_OFF( );
      Mbus_CH3_OFF( );
      Mbus_CH4_ON( );
      break;
    } 
  case Null_Port :  
    {
      CloseAllMbus ( );
      break;
    }
  default :
    {return ;}
  }
  return ;
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
void OpenMultipleMbus (CommPortNumber_Type Channel)
{
  unsigned char itemp = 0 ;
  if(0 == HVPower_state)
  {
    HVPower_ON( );
    vTaskDelay(_100ms * 5);
  }
  
  if(Channel & Mbus_Port1)
  { 
    Mbus_CH1_ON( );
    itemp = 1;
  }
  if(Channel & Mbus_Port2)
  {
    Mbus_CH2_ON( );
    itemp = 1;
  }
  if(Channel & Mbus_Port3)
  {
    Mbus_CH3_ON( );
    itemp = 1;
  }
  if(Channel & Mbus_Port4)
  {
    Mbus_CH3_ON( );
    itemp = 1;
  }
  if(0 == itemp)
  {
    CloseAllMbus();
  }
  return;
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
FunctionalState getPowerMbus_State (CommPortNumber_Type channel)
{
  FunctionalState state;
  
  if (0 == HVPower_state)
  {return DISABLE;}

  switch( channel)
  {
  case Mbus_Port1 : 
    {
      if(Mbus_CH1_State)  
      {state = DISABLE ;}
      else
      {state = ENABLE ;}
      break;
    }
  case Mbus_Port2:
    {
      if(Mbus_CH2_State) 
      {state = DISABLE;}
      else
      {state = ENABLE;}
      break;
    }
  case Mbus_Port3: 
    {
      if(Mbus_CH3_State)   
      {state = DISABLE;}
      else
      {state = ENABLE;}
      break;
    }
  case Mbus_Port4 :    
    {
      if(Mbus_CH4_State)   
      {state = DISABLE;}
      else
      {state = ENABLE;}
      break;
    }
  case Mbus_ALL :    
    {
      if((Mbus_CH1_State)
         &&(Mbus_CH2_State)
           &&(Mbus_CH3_State)
             &&(Mbus_CH4_State))   
      {state = DISABLE;}
      else
      {state = ENABLE;}
      break;
    }
  default:
    {
      state = DISABLE;
      break;
    }
  }
  return state;
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
/*SystemErrName changeChannel_Mbus(CommPortNumber_Type Channel)
{
  SystemErrName err = NO_ERR;
  if(Null_Port == USART_Des[1].PortPV)
  {
    err = Launch_USART(Channel, 2400, Parity_Even, StopBits_1, ByDMA);
  }
  else if( USART_Des[1].PortPV != Channel)
  {
    OpenSingleMbus (Channel);
    USART_Des[1].PortPV = Channel;
  } 
  return err;
}*/
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
CommPortNumber_Type  getChannel_Mbus( void )
{
  return USART_Des[1].PortPV;
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
PortStatus  getState_USART( CommPortNumber_Type port )
{
  PortStatus   Status = Undf;
  switch (port)
  {
    
  case Rs232_Port :
    {
      Status = USART_Des[0].Status;
      break;
    }
  case Mbus_Port1 :
  case Mbus_Port2 :
  case Mbus_Port3 :
  case Mbus_Port4 :
  case Mbus_ALL :
    
    {
      Status = USART_Des[1].Status;
      break;
    }
  case USART3_Port :
    {
      Status = USART_Des[2].Status;
      break;
    }
  }
  return Status;
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
void setState_USART( CommPortNumber_Type port, PortStatus newState)
{
  switch (port)
  {
    
  case Rs232_Port :
    {
      USART_Des[0].Status = newState;
      break;
    }
  case Mbus_Port1 :
  case Mbus_Port2 :
  case Mbus_Port3 :
  case Mbus_Port4 :
  case Mbus_ALL :
    
    {
      USART_Des[1].Status = newState;
      break;
    }
  case USART3_Port :
    {
      USART_Des[2].Status = newState;
      break;
    }
  }
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
SystemErrName config_Mbus(CommPortNumber_Type Port, 
                           unsigned long BaudRate,
                           SerialPort_Parity  Parity, 
                           SerialPort_StopBits StopBits,
                           SerialPort_ReceiveMod ReceiveMod)
{
  SystemErrName err = NO_ERR;
  
  switch (Port)
  {
  case Mbus_Port1 : 
    {
      if(0 != OverLoad_timer[0])
      {return OverLoad;}
      break;
    }
  case Mbus_Port2 : 
    {
      if(0 != OverLoad_timer[1])
      {return OverLoad;}
      break;
    }
  case Mbus_Port3 : 
    {
      if(0 != OverLoad_timer[2])
      {return OverLoad;}
      break;
    }
  case Mbus_Port4 : 
    {
      if(0 != OverLoad_timer[3])
      {return OverLoad;}
      break;
    } 
  default :
    {return IllegalParam;}
  }
  
  if((BaudRate != USART_Des[1].BaudRate)
     ||(Parity != USART_Des[1].Parity)
       ||(StopBits != USART_Des[1].StopBits)
         ||(ReceiveMod != USART_Des[1].ReceiveMod_SV)
           ||(Null_Port == USART_Des[1].PortPV))
  {
    err = Launch_USART(Port, BaudRate, Parity, StopBits, ReceiveMod);   
    if(NO_ERR == err)
    {vTaskDelay(_100ms * 5);}
  }
  else if (USART_Des[1].PortPV != Port)
  {
    OpenSingleMbus (Port);
    USART_Des[1].PortPV = Port;
    vTaskDelay(_100ms * 5);
  }
    
  return err; 
}
/******************************************END********************************************************/
