/*********************************************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************************************/
#ifndef  __ONSCENECOMM_H
#define  __ONSCENECOMM_H
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include ".\Includes.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/

#define     ServerReceiveNO            2
#define     ServerReceiveBuffLength    1024
/*********************************************************************************************************
�������Ͷ���
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
}OnSceneComm_PackageStruct;       //�ر��м�������  Ӧ������ 

typedef struct{
  union{
    unsigned char Byte;
    struct{
      unsigned char idle   :1; //����������
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
        unsigned char  CN     :1;    //���������ӱ�־   
        //unsigned char  IF     :1;    //�������Ϸ������ױ�־  
        //unsigned char  RC     :1;    //���������������� ��־  
        //unsigned char  ST     :1;    //������ ���־  
        unsigned char  ACK    :1;    //������ Ӧ��
        unsigned char  GZ     :1;    //�������������
        unsigned char  EN     :1;    //����������
      }Bit; 
    }SR;
    CommPortNumber_Type    Port;       //�������˿�
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
�ⲿ����������
*********************************************************************************************************/
extern ServerState_Struct    ServerState[2];
extern unsigned  short      OverLoad_timer[4];
extern Bit_8_struct       OverLoadFlg[4];
extern unsigned long       SilenceTimer[2];
/*********************************************************************************************************
����������
*********************************************************************************************************/
//void MbusCOMM_Task (void* Param );
   
void Server_Task (void* Param );

unsigned char WriteDtatToServerRecBuff(unsigned char ServerNO, char* data, unsigned short length);


/********************************************************************************************************/
#endif

/******************************************END********************************************************/