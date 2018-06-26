/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef __GPRS_SIM800C_H__
#define __GPRS_SIM800C_H__
/*******************************************************************************
文件包含区
*******************************************************************************/
#include ".\Includes.h"
/*******************************************************************************
宏定义区
*******************************************************************************/
#define  GPRS_COMM               USART3_Port
#define  GPRS_GprsBuffLength     4096
#define  GprsRecveBuffLength     GPRS_GprsBuffLength
#define  GprsSendBuffLength      GPRS_GprsBuffLength

#define  weitGPRS_COMM( )       weitPortIdle (GPRS_COMM, 200);
/*******************************************************************************
数据类型定义
*******************************************************************************/
typedef enum{
  RecErr  = 0x00,
  RecOK   = 0x01,
  ExeOK   = 0x02,
  ExeErr  = 0x03,
}Cmd_ACK_type;

typedef struct{
  enum{
    CONNECT_FAIL = 0x00,
    CONNECT_OK   = 0x01,
    ALREADY_CONNECT = 0x02,
    CME_ERROR = 0x03,
  }state;
  unsigned short ERROR_CODE;  
}CIPSTART_ACK_type;

typedef struct
{
  union{
    unsigned char mode  :2;  //联结模式  00:UDP  01 : TCP CLient  10:TCP Server
    unsigned char state :1;  //联结标志 1:联结上 0:联结断开
  }CSR; 
 unsigned long ip;
 unsigned short prot;
}GPRS_IP_struct;

typedef struct{
   union{
    unsigned char Byte;
    struct{
    unsigned char power :1;   //电源标志 
    unsigned char ready :1;  //启动配置完成
   }Bit;
  }CSR; 
 unsigned long ip;
 unsigned short prot; 
 unsigned char  Rssi;
 unsigned char  Ber;
 unsigned long  localip;
 GPRS_IP_struct  cip[6]; //
 char   ID[16];
}GPRS_MODE_struct;

typedef struct{
unsigned char   ch;
unsigned short  length;
void*            pbuff; 
}GPRS_IP_Package_struct;
/*******************************************************************************
外部变量声明区
*******************************************************************************/
extern GPRS_MODE_struct gprs;
/*******************************************************************************
函数声明区
*******************************************************************************/
// 检查SIM卡状态 
Cmd_ACK_type  _CPIN_func(void);
 
// 检查网络信号强度
Cmd_ACK_type  _CSQ_func(void);

//检查网络注册状态
Cmd_ACK_type  _CREG_func(unsigned char *mode, unsigned char* stat);

//设置多链路模式 
Cmd_ACK_type  _CIPMUX_func(BaseType_t MUX);

// 设置透传模式
Cmd_ACK_type  _CIPMODE_func(BaseType_t mode);

// 检查GPRS附着状态 
Cmd_ACK_type  _CGATT_func(unsigned char* state);

// 设置APN
Cmd_ACK_type  _CSTT_func( char *apn);

//建立无线链路（GPRS或者CSD）
Cmd_ACK_type  _CIICR_func(void);

// 获取本地 IP 地址
Cmd_ACK_type  _CIFSR_func(unsigned long* IP);

// 建立TCP链接
Cmd_ACK_type _CIPSTART_func(unsigned char ch, unsigned long ip, unsigned short port, CIPSTART_ACK_type* ack);

//发送数据到远端服务器 
Cmd_ACK_type  _CIPSEND_func(unsigned char ch, unsigned char* pbuff, unsigned short length);
// 关闭远端连接 
Cmd_ACK_type  _CIPCLOSE_func(unsigned char ch);

//同步通信串口比特率
Cmd_ACK_type  _SyncRate_func(void);


void GprsTask_func ( void* Param );

SystemErrName GprsIP_SendFunc(unsigned char CH, void* Data,unsigned short DataLength);
unsigned char connectServer (unsigned char ServerNO);
void closeServer (unsigned char ServerNO);
/******************************************************************************/
#endif
/****************************** END *******************************************/