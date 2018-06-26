/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef  __SERIAL_PORT_H
#define  __SERIAL_PORT_H 
/*********************************************************************************
文件包含区
*********************************************************************************/
#include ".\MCU_Config.h"
#include ".\PcbDefine.h"
#include "..\..\Projects\inc\SystemDefine.h"
#include "..\..\User\inc\Includes.h"
/*********************************************************************************
宏定义区
*********************************************************************************/

#define  USART_BuffNumber       32
#define  USART_BuffLength       64

#define  Mbus_Usart_NO         1

/*********************************************************************************
数据类型定义
*********************************************************************************/
typedef  struct USART_BuffStruct   UsarBuff_Struct;   

struct USART_BuffStruct{
  union {
    unsigned char Byte;
    struct {
      unsigned char idle   :1; //缓冲区空闲
      unsigned char first  :1; //多包数据的首包标志
      unsigned char flg    :1; //数据有效指示
      unsigned char org    :1;
      unsigned char end    :1;
    }Bit;
  }CCR;
  CommPortNumber_Type  port;
  UsarBuff_Struct      *Prev;
  UsarBuff_Struct      *next;
  unsigned char   Tin;
  unsigned char length;
  unsigned char data[USART_BuffLength];
};

typedef enum {
 Parity_No   =    USART_Parity_No ,  
 Parity_Even =    USART_Parity_Even ,
 Parity_Odd  =    USART_Parity_Odd ,                
}SerialPort_Parity;

typedef enum {
  StopBits_1    =      USART_StopBits_1,         
  StopBits_0_5  =      USART_StopBits_0_5,      
  StopBits_2    =      USART_StopBits_2,       
  StopBits_1_5  =      USART_StopBits_1_5,     
}SerialPort_StopBits;

typedef enum {
 Polling   = 0, 
 ByIT      = 1,
 ByDMA     = 2,
}SerialPort_ReceiveMod; 

typedef void (*SerialPort_SendEndCallBack_Type)(CommPortNumber_Type port, void *pbuff);
         
typedef struct{
  unsigned long                      BaudRate;
  SerialPort_Parity                   Parity; 
  SerialPort_StopBits                 StopBits;
  SerialPort_ReceiveMod               ReceiveMod_SV ;
  CommPortNumber_Type                 PortPV  ;
  PortStatus                          Status;
  unsigned   char*                   TX_P ;
  unsigned   short                   TX_CNT;
  UsarBuff_Struct*                    RX_P ;
  unsigned   short                   RX_CNT ;  
  SerialPort_SendEndCallBack_Type     SendEnd_CallBack ;
  void*                               SendEnd_CallBack_arg ;  
}SerialPort_Struct;  

typedef struct{
  USART_TypeDef*                       PHY;
  DMA_Channel_TypeDef*                 DMAy_ChannelRx;
  DMA_Channel_TypeDef*                 DMAy_ChannelTx;  
}SerialPort_ConfigStruct; 

typedef enum {
  IDLE = 0,
  TX  = 1,
  RX = 2
}MbusPortStatus;
  
/*********************************************************************************
外部变量声明区
*********************************************************************************/
extern MbusPortStatus   MbusStatus;
/*********************************************************************************
函数声明区
*********************************************************************************/
void DeInit_UsartBuff(void);

unsigned long GetDataFromUsart (UsarBuff_Struct* buff,
                                unsigned char *pbuff,  
                                unsigned long buffsize);

unsigned long GetDataFromPort (CommPortNumber_Type port,
                               unsigned char *pbuff,  
                               unsigned long buffsize);

unsigned long GetDataFromUsartBuff (unsigned char *pbuff,  
                                    unsigned long buffsize, 
                                    CommPortNumber_Type* source);

void FreeBuff_USART(void** buff);

void FreeBuffFormPort(CommPortNumber_Type  port);

void DeleteDataFromUsartBuff( UsarBuff_Struct* buff );

static UsarBuff_Struct* Get_UsartBuff (CommPortNumber_Type  port);

void TXE_Interrupt_USART (unsigned char Usart_NO);

void TC_Interrupt_USART (unsigned char Usart_NO);

void RXNE_Interrupt_USART (unsigned char Usart_NO);

void IDLE_Interrupt_USART (unsigned char Usart_NO);

void DMA_TE_Interrupt_USART (unsigned char Usart_NO);

void DMA_TC_Interrupt_USART (unsigned char Usart_NO);

SystemErrName Launch_USART( CommPortNumber_Type Channel, 
                           unsigned long BaudRate,
                           SerialPort_Parity  Parity, 
                           SerialPort_StopBits StopBits,
                           SerialPort_ReceiveMod   mod );

SystemErrName activateReceiveByIT_USART (unsigned char Usart_NO);

SystemErrName activateReceiveByDMA_USART (unsigned char Usart_NO);

SystemErrName sendBuffByIT_USART (CommPortNumber_Type Channel,
                                  void * buff, 
                                  unsigned short length,
                                  SerialPort_SendEndCallBack_Type callBack,
                                   void *callBack_arg );

SystemErrName sendStringByIT_USART (CommPortNumber_Type Channel,
                                    unsigned char *buff);

SystemErrName sendBuffByDMA_USART (CommPortNumber_Type Channel,
                                   void *buff, 
                                   unsigned short length,
                                   SerialPort_SendEndCallBack_Type callBack,
                                   void *callBack_arg );

SystemErrName sendStringByDMA_USART (CommPortNumber_Type Channel, unsigned char *buff);

void CloseAllMbus (void);

void OpenSingleMbus (CommPortNumber_Type Channel);

void OpenMultipleMbus (CommPortNumber_Type Channel);

FunctionalState getPowerMbus_State (CommPortNumber_Type channel);

/*SystemErrName changeChannel_Mbus( CommPortNumber_Type Channel);*/

CommPortNumber_Type  getChannel_Mbus( void );


PortStatus  getState_USART( CommPortNumber_Type port );
void setState_USART( CommPortNumber_Type port, PortStatus newState);

SystemErrName config_Mbus(CommPortNumber_Type Port, 
                           unsigned long BaudRate,
                           SerialPort_Parity  Parity, 
                           SerialPort_StopBits StopBits,
                           SerialPort_ReceiveMod ReceiveMod);

/********************************************************************************/
#endif

