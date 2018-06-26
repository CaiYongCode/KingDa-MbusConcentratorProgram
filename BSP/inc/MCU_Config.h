/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef  MCU_CONFIG_H
#define  MCU_CONFIG_H 
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "stm32l1xx_conf.h"
/*********************************************************************************
宏定义区
*********************************************************************************/

#define  _4V2_EN_ENpin           GPIOC->IDR  & GPIO_Pin_2
#define  set_4V2_ENpin           GPIOC->BSRRL = GPIO_Pin_2
#define  clr_4V2_ENpin           GPIOC->BRR  = GPIO_Pin_2

#define  HV_EN_ENpin             (GPIOC->IDR  & GPIO_Pin_3)
#define  set_HV_ENpin            GPIOC->BSRRL = GPIO_Pin_3
#define  clr_HV_ENpin            GPIOC->BRR  = GPIO_Pin_3

#define  GPRS_RESTpin            GPIOC->IDR  & GPIO_Pin_6
#define  set_GPRS_RESTpin        GPIOC->BSRRL = GPIO_Pin_6
#define  clr_GPRS_RESTpin        GPIOC->BRR  = GPIO_Pin_6

#define  GPRS_ONOFFpin           GPIOC->IDR  & GPIO_Pin_7
#define  set_GPRS_ONOFFpin       GPIOC->BSRRL = GPIO_Pin_7
#define  clr_GPRS_ONOFFpin       GPIOC->BRR  = GPIO_Pin_7

#define  Mbus_RXDpin             GPIOA->IDR  & GPIO_Pin_1
#define  set_Mbus_RXDpin         GPIOA->BSRRL = GPIO_Pin_1
#define  clr_Mbus_RXDpin         GPIOA->BRR  = GPIO_Pin_1

#define  Mbus_OverLoadpin        GPIOA->IDR  & GPIO_Pin_7


#define  OUT_CH1_ENpin           (GPIOB->IDR  & GPIO_Pin_1)
#define  set_Mbus_CH1_ENpin      GPIOB->BSRRL = GPIO_Pin_1
#define  clr_Mbus_CH1_ENpin      GPIOB->BRR  = GPIO_Pin_1

#define  OUT_CH2_ENpin           (GPIOB->IDR  & GPIO_Pin_0)
#define  set_Mbus_CH2_ENpin      GPIOB->BSRRL = GPIO_Pin_0
#define  clr_Mbus_CH2_ENpin      GPIOB->BRR  = GPIO_Pin_0

#define  OUT_CH3_ENpin           (GPIOC->IDR  & GPIO_Pin_5)
#define  set_Mbus_CH3_ENpin      GPIOC->BSRRL = GPIO_Pin_5
#define  clr_Mbus_CH3_ENpin      GPIOC->BRR  = GPIO_Pin_5

#define  OUT_CH4_ENpin           (GPIOC->IDR  & GPIO_Pin_4)
#define  set_Mbus_CH4_ENpin      GPIOC->BSRRL = GPIO_Pin_4
#define  clr_Mbus_CH4_ENpin      GPIOC->BRR  = GPIO_Pin_4

#define  LED1pin                 GPIOC->IDR  & GPIO_Pin_8
#define  set_LED1pin             GPIOC->BSRRL = GPIO_Pin_8
#define  clr_LED1pin             GPIOC->BRR  = GPIO_Pin_8

#define  LED2pin                 GPIOC->IDR  & GPIO_Pin_9
#define  set_LED2pin             GPIOC->BSRRL = GPIO_Pin_9
#define  clr_LED2pin             GPIOC->BRR  = GPIO_Pin_9

#define  User_Kpin               GPIOA->IDR  & GPIO_Pin_0


/*********************************************************************************
数据类型定义
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
函数声明区
*********************************************************************************/
void RCC_Configuration (void);
void GPIO_Configuration(void);
void I2C1_Rset(void);
void I2C2_Configuration(void);
void I2C2_Rset(void);
void SPI1_Configuration(void);
void MCU_init ( void );
void ADC1_TCIE_IRQ (void);
void I2C1_Configuration(void);
/********************************************************************************/
#endif













