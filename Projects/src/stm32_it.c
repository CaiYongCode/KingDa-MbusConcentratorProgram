/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\..\BSP\inc\bsp.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
/*********************************************************************************
公共变量定义区
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/
extern unsigned char _1mS_cnt;
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
/*********************************************************************************
 Function:      //
 Description:   // SysTick Handler
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
/*void SysTick_Handler (void)
{
 _1mS_cnt ++;
} */     
/*********************************************************************************
 Function:      //
 Description:   //  Window Watchdog
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void WWDG_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //  PVD through EXTI Line detect
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void PVD_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //  Tamper
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void TAMPER_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //   RTC
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void RTC_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //  Flash
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void FLASH_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //  RCC
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void RCC_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //  EXTI Line 0
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void EXTI0_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   // EXTI Line 1
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void EXTI1_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   // EXTI Line 2
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void EXTI2_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //  EXTI Line 3
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void EXTI3_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //  EXTI Line 4
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void EXTI4_IRQHandler (void)
{
  //unsigned long itemp;
 //itemp = EXTI->PR;
 //if(itemp & (1ul << 4))
 //{
 // EXTI->PR |= 1ul << 4;
 // F8563_Interrupt( );  // 
 //} 
}
/*********************************************************************************
 Function:      //
 Description:   //  DMA1 Channel 1  ADC1
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void DMA1_Channel1_IRQHandler (void)
{
  unsigned long itemp; 
  itemp = DMA1->ISR;
  DMA1->IFCR |= 0x0000000f;
 if(itemp & 0x00000001 << 1)
 {
  // ADC1_TCIE_IRQ ( );
   GPIO_ResetBits(GPIOC, GPIO_Pin_1); 
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);
 }
}
/*********************************************************************************
 Function:      //
 Description:   //  USART3_TX
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void DMA1_Channel2_IRQHandler (void)
{
 unsigned long itemp;
 itemp = DMA1->ISR;
 DMA1->IFCR |= 0x0000000f << 4;
 if(itemp & 0x0000000f << 4) 
 {
   if((DMA1_Channel2->CCR & DMA_IT_TE)||(DMA1_Channel2->CCR & DMA_IT_TC))
   {
     USART3->CR3 &= ~(USART_DMAReq_Tx);
     USART_ITConfig(USART3, USART_IT_TC, ENABLE);
   }
 } 
}
/*********************************************************************************
 Function:      //
 Description:   //  USART3_RX
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void DMA1_Channel3_IRQHandler (void)
{
 unsigned long itemp;
 itemp = DMA1->ISR;
 DMA1->IFCR |= 0x0000000f << 8;
 if(itemp & 0x0000000f << 8) 
 {
   if( DMA1_IT_TC3 & itemp)
   {DMA_TC_Interrupt_USART (2);}
   if( DMA1_IT_TE3 & itemp)
   {DMA_TE_Interrupt_USART (2);}
 }
}
/*********************************************************************************
 Function:      //
 Description:   // USART1_TX
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void DMA1_Channel4_IRQHandler (void)
{
 unsigned long itemp;
 itemp = DMA1->ISR;
 DMA1->IFCR |= 0x0000000f << 12;
 itemp >>= 12;
 if(itemp & 0x0000000A ) 
 {
   if((DMA1_Channel4->CCR & DMA_IT_TE)||(DMA1_Channel4->CCR & DMA_IT_TC))
   {
     USART1->CR3 &= ~(USART_DMAReq_Tx);
     USART_ITConfig(USART1, USART_IT_TC, ENABLE);  
   }
 } 
}
/*********************************************************************************
 Function:      //
 Description:   //  USART1_RX
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void DMA1_Channel5_IRQHandler (void)
{
 unsigned long itemp;
 itemp = DMA1->ISR;
 DMA1->IFCR |= 0x0000000f << 16;
 if(itemp & 0x0000000f << 16) 
 {
   if( DMA1_IT_TC5 & itemp)
   {DMA_TC_Interrupt_USART (0);}
   if( DMA1_IT_TE5 & itemp)
   {DMA_TE_Interrupt_USART (0);}
 }
}
/*********************************************************************************
 Function:      //
 Description:   //USART2_RX
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void DMA1_Channel6_IRQHandler (void)
{
 unsigned long itemp;
 itemp = DMA1->ISR;
 DMA1->IFCR |= 0x0000000f << 20;
 if(itemp & 0x0000000f << 20) 
 {
   if( DMA1_IT_TC6 & itemp)
   {DMA_TC_Interrupt_USART (1);}
   if( DMA1_IT_TE6 & itemp)
   {DMA_TE_Interrupt_USART (1);}
 } 
}
/*********************************************************************************
 Function:      //
 Description:   //  USART2_TX
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void DMA1_Channel7_IRQHandler (void)
{
 unsigned long itemp;
 itemp = DMA1->ISR;
 DMA1->IFCR |= 0x0000000f << 24;
 if(itemp & 0x0000000f << 24) 
 {
   if((DMA1_Channel7->CCR & DMA_IT_TE)||(DMA1_Channel7->CCR & DMA_IT_TC))
   {
     //USART2->CR3 &= ~(USART_DMAReq_Rx);
     USART_ClearITPendingBit(USART2, USART_IT_TC);
     USART_ITConfig(USART2, USART_IT_TC, ENABLE);
   }
 }
}
/*********************************************************************************
 Function:      //
 Description:   //   ADC1 & ADC2
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void ADC1_2_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   // USB High Priority or CAN1 TX
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void USB_HP_CAN1_TX_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //  USB Low  Priority or CAN1 RX0
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void USB_LP_CAN1_RX0_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //   CAN1 RX1
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void CAN1_RX1_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //  CAN1 SCE
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void CAN1_SCE_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //  EXTI Line 9..5
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void EXTI9_5_IRQHandler (void)
{
 if(SET == EXTI_GetFlagStatus(EXTI_Line5))
 {
   EXTI_ClearFlag(EXTI_Line5);
   //set_Mbus_RXDpin;
   if(TX != MbusStatus )
   {clr_Mbus_RXDpin;}
   else
   {set_Mbus_RXDpin;}
 }
  
 if(SET == EXTI_GetFlagStatus(EXTI_Line6))
 {
   EXTI_ClearFlag(EXTI_Line6);
   set_Mbus_RXDpin;
 }
}
/*********************************************************************************
 Function:      //
 Description:   //  TIM1 Break
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void TIM1_BRK_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //   TIM1 Update
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void TIM1_UP_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //   TIM1 Trigger and Commutation
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void TIM1_TRG_COM_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   // TIM1 Capture Compare
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void TIM1_CC_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //  TIM2
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void TIM2_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //  TIM3
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void TIM3_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //   TIM4
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void TIM4_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   // I2C1 Event
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void I2C1_EV_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //  I2C1 Error
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void I2C1_ER_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   // I2C2 Event
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void I2C2_EV_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //  I2C2 Error
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void I2C2_ER_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //  SPI1
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void SPI1_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //   SPI2
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void SPI2_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //  USART1
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void USART1_IRQHandler (void)
{
 unsigned long  i;
 i=USART1->SR;
 USART1->SR = 0;
 if((0!=(i&USART_FLAG_TXE)) && (USART1->CR1&0x0080))//
   { TXE_Interrupt_USART ( 0 );} 
 if((0!=(i&USART_FLAG_TC)) && (USART1->CR1&0x0040))
   { TC_Interrupt_USART ( 0 );}
 if((0!=(i&USART_FLAG_IDLE)) && (USART1->CR1&0x0010))
   { IDLE_Interrupt_USART ( 0 );} 
 if(0!=(i&USART_FLAG_RXNE) && (USART1->CR1&0x0020))//
   {RXNE_Interrupt_USART ( 0 );} 
}
/*********************************************************************************
 Function:      //
 Description:   //  USART2
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void USART2_IRQHandler (void)
{ 
 unsigned long i;
 i=USART2->SR;
 USART2->SR = 0;
 if((0!=(i&USART_FLAG_TXE)) && (USART2->CR1&0x0080))
   { TXE_Interrupt_USART ( 1 );} 
 if((0!=(i&USART_FLAG_TC)) && (USART2->CR1&0x0040))
   { TC_Interrupt_USART ( 1 );}
 if((0!=(i&USART_FLAG_IDLE)) && (USART2->CR1&0x0010))
   { IDLE_Interrupt_USART ( 1 );} 
 if(0!=(i&USART_FLAG_RXNE) && (USART2->CR1&0x0020))
   {RXNE_Interrupt_USART ( 1 );} 
}
/*********************************************************************************
 Function:      //
 Description:   //  USART3
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void USART3_IRQHandler (void)
{
 unsigned long i;
 i=USART3->SR;
 USART3->SR = 0;
 if((0!=(i&USART_FLAG_TXE))&&(USART3->CR1&0x0080))
 { TXE_Interrupt_USART ( 2 );} 
 if((0!=(i&USART_FLAG_TC))&&(USART3->CR1&0x0040))
 { TC_Interrupt_USART ( 2 );}
 if(0!=(i&USART_FLAG_IDLE) && (USART3->CR1&0x0010))
 { IDLE_Interrupt_USART ( 2 );} 
 if(0!=(i&USART_FLAG_RXNE) && (USART3->CR1&0x0020))//
 {RXNE_Interrupt_USART ( 2 );} 
}
/*********************************************************************************
 Function:      //
 Description:   // EXTI Line 15..10
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void EXTI15_10_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   // RTC Alarm through EXTI Line
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void RTCAlarm_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   // USB Wakeup from suspend
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void USBWakeUp_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   // TIM8 Break
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void TIM8_BRK_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   // TIM8 Update
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void TIM8_UP_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //  TIM8 Trigger and Commutation
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void TIM8_TRG_COM_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   // TIM8 Capture Compare
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void TIM8_CC_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //  ADC3
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void ADC3_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   // FSMC
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void FSMC_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   // SDIO
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void SDIO_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   // TIM5
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void TIM5_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //SPI3
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void SPI3_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //UART4
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void UART4_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //UART5
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void UART5_IRQHandler  (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //TIM6
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void TIM6_IRQHandler  (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //TIM7
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void TIM7_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   //DMA2 Channel1
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void DMA2_Channel1_IRQHandler (void)
{
  
}
/*********************************************************************************
 Function:      //
 Description:   // DMA2 Channel3
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void DMA2_Channel2_IRQHandler (void)
{
  
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
void DMA2_Channel3_IRQHandler (void) 
{
  
}
/*********************************************************************************
 Function:      //
 Description:   // DMA2 Channel4 & Channel5
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/          
void DMA2_Channel4_5_IRQHandler (void)
{
  
}