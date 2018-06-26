/*********************************************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************************************/
#ifndef  __ONSCENECOMM_H
#define  __ONSCENECOMM_H
/*********************************************************************************************************
文件包含区
*********************************************************************************************************/
#include ".\Includes.h"
/*********************************************************************************************************
宏定义区
*********************************************************************************************************/

#define     ServerReceiveNO            2
#define     ServerReceiveBuffLength    1024
/*********************************************************************************************************
数据类型定义
*********************************************************************************************************/
typedef enum{
 Penetrate    = 0x00,
 KingDaV1     = 0x01,
 KingDaServer = 0x02,
 CJT_188_2004 = 0x03  
}ProtocolName;

typedef struct{
  ProtocolName  name;   
  union{
    void *pbuff;
    struct KindgDaSPCPSendBuff*      KingDa;
    struct CJT188_SendBuff_Struct*    CJT188;
  }data;
}OnSceneComm_PackageStruct;       //关闭中继器命令  应答数据 

typedef struct{
  union{
    unsigned char Byte;
    struct{
      unsigned char idle   :1; //缓冲区空闲
    }Bit;
  }CSR;
  unsigned short DataLength;
  unsigned char data[ServerReceiveBuffLength]; 
}ServerBuff_struct;


typedef struct
  {
    union{
      unsigned char Byte;
      struct{
        unsigned char  CN     :1;    //服务器联接标志   
        //unsigned char  IF     :1;    //次联接上服务器首标志  
        //unsigned char  RC     :1;    //服务器重联工作中 标志  
        //unsigned char  ST     :1;    //服务器 活动标志  
        unsigned char  ACK    :1;    //服务器 应答
        unsigned char  GZ     :1;    //重联超过规次数
        unsigned char  EN     :1;    //服务器启用
      }Bit; 
    }SR;
    CommPortNumber_Type    Port;       //服务器端口
    unsigned char         Connect_CNT ;
  }ServerState_Struct;


typedef union{
  unsigned char Byte;
  struct{
    unsigned char  n0     :1;    //
    unsigned char  n1     :1;    //
    unsigned char  n2     :1;    //
    unsigned char  n3     :1;    //
    unsigned char  n4     :1;    //
    unsigned char  n5     :1;    //
    unsigned char  n6     :1;    //
    unsigned char  n7     :1;    //
  }Bit; 
}Bit_8_struct;
/*********************************************************************************************************
外部变量声明区
*********************************************************************************************************/
extern ServerState_Struct    ServerState[2];
extern unsigned  short      OverLoad_timer[4];
extern Bit_8_struct       OverLoadFlg[4];
extern unsigned long       SilenceTimer[2];
/*********************************************************************************************************
函数声明区
*********************************************************************************************************/
//void MbusCOMM_Task (void* Param );
   
void Server_Task (void* Param );

unsigned char WriteDtatToServerRecBuff(unsigned char ServerNO, char* data, unsigned short length);


/********************************************************************************************************/
#endif

/******************************************END********************************************************/