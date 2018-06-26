/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\DriverPcb.h"
#include "..\..\User\inc\Includes.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
/*********************************************************************************
公共变量定义区
*********************************************************************************/
unsigned short  Vin = 7600;    //电源电压 MV
unsigned short  Vout  = 0;     //输出电压 MV
unsigned short  Iout  = 0;     //输出电流 MA

BIT_8W Prot_state = {0};   //
BIT_8W Prot_UP = {0};
BIT_8W Prot_DW = {0};

static unsigned short  Adcbuff[8][2];
/*********************************************************************************
外部变量声明区
*********************************************************************************/
/*********************************************************************************
私有变量定义区
*********************************************************************************/

/*********************************************************************************
测试变量定义区
*********************************************************************************/

/*********************************************************************************
内部函数定义区
*********************************************************************************/

/*********************************************************************************
功能代码定义区
*********************************************************************************/
/**********************************************************************************
  Function:       // 
  Description:    // 
  Input:          // 
                  // 
  Output:         // 
  Return:         // 
  Others:         // 
**********************************************************************************/
unsigned char get_portstate ( void )
{
 unsigned char portstate ;
 //portstate =  GPIOE->IDR & ( GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10  | GPIO_Pin_11); 
 portstate = GPIOD->IDR &  GPIO_Pin_6;                       
 portstate >>= 6;
 return ~portstate; 
}
/**********************************************************************************
  Function:       // 
  Description:    // 
  Input:          // 
                  // 
  Output:         // 
  Return:         // 
  Others:         // 
**********************************************************************************/
void Port_sacn (void)
{
  unsigned short itemp1;
  unsigned char  itemp2;
  static unsigned char count = 0;
  static unsigned char prot_buff[key_cnt];
  static unsigned char last_state= 0;

  if(count >= key_cnt)
  {count =0;}
  prot_buff[count] = get_portstate( ) ;
  count ++ ;

  itemp2 = key_cnt;
  itemp1 = 0xffff;
  while (itemp2)
  {
   itemp2--;
   itemp1 &=  prot_buff[itemp2]; 
  }
  Prot_state.Byte |= itemp1;

  itemp2 = key_cnt;
  itemp1 = 0x0000;
  while (itemp2)
  {
   itemp2--;
   itemp1 |=  prot_buff[itemp2]; 
  }

  Prot_state.Byte &= itemp1;

  Prot_UP.Byte |= (last_state ^ Prot_state.Byte) & Prot_state.Byte;
  Prot_DW.Byte |= (last_state ^ Prot_state.Byte) & (~Prot_state.Byte);  
  last_state = Prot_state.Byte;
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
unsigned char  EEROM_Read (unsigned long Address, void* buff, unsigned short length)
{
  assert_param(IS_FLASH_DATA_ADDRESS(Address)); 
  while (length-- > 0)
  {
    ((unsigned char*)buff)[length] = ((unsigned char*)Address)[length];
  }
  return 1;
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
unsigned char EEROM_Write (unsigned long Address, void* buff, unsigned short length)
{
  unsigned short itemp = 0;
  FLASH_Status   Status;
  
  DATA_EEPROM_Unlock( );
  
  while (length-- > 0)
  {
    Status = DATA_EEPROM_FastProgramByte(Address + length, ((unsigned char*)buff)[itemp + length]);
    if(FLASH_COMPLETE != Status)
    { return 0;}
  }
  DATA_EEPROM_Lock( );
  return 1;
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
unsigned short getVin ( void )
{
  unsigned char itemp;
  unsigned long Vref;
  unsigned long sum;
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef  ADC_InitStruct;
  DMA_InitTypeDef  DMA_InitStructure;
  NVIC_InitTypeDef NVIC_InitStruct;
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  
  GPIO_SetBits(GPIOC, GPIO_Pin_1); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  RCC_HSICmd(ENABLE);
  
  while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  ADC_DeInit(ADC1);
  ADC_InitStruct.ADC_ScanConvMode        = ENABLE;
  ADC_InitStruct.ADC_ContinuousConvMode  = ENABLE;  //连续模式
  ADC_InitStruct.ADC_DataAlign           = ADC_DataAlign_Right ; 
  ADC_InitStruct.ADC_ExternalTrigConv    = ADC_ExternalTrigConv_Ext_IT11;
  ADC_InitStruct.ADC_ExternalTrigConvEdge  = ADC_ExternalTrigConvEdge_None ; //选择外部触发边缘并启用常规组的触发器。
  ADC_InitStruct.ADC_NbrOfConversion     = 2;
  ADC_InitStruct.ADC_Resolution         = ADC_Resolution_12b;

  ADC_Init(ADC1, &ADC_InitStruct) ;
  
  ADC_TempSensorVrefintCmd(ENABLE);
  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_384Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_Vrefint, 2, ADC_SampleTime_384Cycles);
  
  RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1, ENABLE);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_InitStructure.DMA_BufferSize = 16;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(Adcbuff);
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  
  DMA_DeInit(DMA1_Channel1);
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  //DMA_ITConfig(DMA1_Channel1, DMA_IT_HT, DISABLE);
  DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
  DMA_Cmd(DMA1_Channel1, ENABLE);
  ADC_DMACmd(ADC1, ENABLE);
  
  NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel1_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
  
 vTaskDelay(_10ms * 3); 
 ADC_Cmd(ADC1,ENABLE);  //使能或者失能指定的 ADC
 
 while(ADC_GetFlagStatus(ADC1,ADC_FLAG_ADONS) == RESET);
 ADC_SoftwareStartConv(ADC1);
 vTaskDelay(_10ms * 2);
 
 if(GPIOC->IDR & GPIO_Pin_1)
 {
   GPIO_ResetBits(GPIOC, GPIO_Pin_1); 
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);
   return  0;
 }
 itemp = 0;
 sum = 0;
 while (itemp < 8)
 {sum += Adcbuff[itemp++][0];}
 sum >>= 3;
 Vref = VREFINT * sum;
  itemp = 0;
 sum = 0;
  while (itemp < 8)
 {sum += Adcbuff[itemp++][1];}
 sum >>= 3;
 Vref /= sum;
 
 Vref *= 197;
 Vref /= 47;  
 return  Vref;
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
SystemErrName writeToFRAM (void* Buffer, unsigned short NumByteToWrite, unsigned short WriteAddr)
{
  SystemErrName err = NO_ERR;
    
  if(pdTRUE != xSemaphoreTake(Fram_xMutex , _100ms * 5  ))
  {return RomBusy;}
  
  err = FRAM_BufferWrite(Buffer, NumByteToWrite, WriteAddr);
  
  xSemaphoreGive( Fram_xMutex);
  
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
SystemErrName readFormFRAM (void* Buffer, unsigned short NumByteToRead,unsigned short ReadAddr)
{
  SystemErrName err = NO_ERR;
  
  if(pdTRUE != xSemaphoreTake(Fram_xMutex , _100ms * 5  ))
  {return RomBusy;}
  
  err =  FRAM_BufferRead(Buffer, NumByteToRead, ReadAddr);
  
  xSemaphoreGive( Fram_xMutex);
  
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
void get_SystemTimer (TimerData_Struct* Timer)
{
   vTaskSuspendAll();  //调度锁开启
   
   Timer->Second = SystemTimer.Second;
   Timer->Year = SystemTimer.Year;
   Timer->Month = SystemTimer.Month;
   Timer->Day = SystemTimer.Day;
   Timer->WeekDay = SystemTimer.WeekDay;
   Timer->Hour = SystemTimer.Hour;
   Timer->Minute = SystemTimer.Minute;
   
   xTaskResumeAll ();  //调度锁关闭
}
/**************************************END****************************************/