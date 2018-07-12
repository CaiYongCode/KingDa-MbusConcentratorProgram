/*********************************************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************************************/
#ifndef  __CJT188_2004_H
#define  __CJT188_2004_H
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include "..\..\BSP\inc\bsp.h"
#include "..\..\Projects\inc\SystemDefine.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/
#define  CJT188_OutPutBuff_Number      4

#define  Locall_MeterType     UnknownMeterType
#define  Locall_FactoryID     ('K' *10) + 'D'

#define  Locall_Type                    0  //0 �����ݲɼ���   1�����ݴ�����

#define  PreambleNumber                 2 

#define  CJT188_Preamble                0xFE
#define  CJT188_PreambleLength          2
#define  CJT188_StartCharacter          0x68
#define  CJT188_Pause                   0x16

#define  CJT_188ValveOpenCmd            0x55      //�� �� �� �� ���� :55H
#define  CJT_188ValveCloseCmd           0x99     //�� �� �� �� ���� :99H
#define  CJT_188ValveMaintainCmd        0xAA     //�� �� �� �� ���� :99H

#define CJT188_FrameParamLength         32
/*********************************************************************************************************
�������Ͷ���
*********************************************************************************************************/

#pragma pack(1)

typedef enum{ASK_CJT188 = 0, ACK_CJT188 = 1  }CJT188_DirType;

typedef unsigned char           CJT188_SER_Type;

typedef unsigned short          CJT188_DI_Type;

//������Ͷ���
typedef enum{
  ColdWaterMeter  = 0x10,   //��ˮ��
  HeatWaterMeter  = 0x11,   //������ˮ��
  DrinkWaterMeter = 0x12,   //ֱ��ˮ��
  RecycledWaterMeter = 0x13,   //��ˮ��
  UnknownMeterType   = 0xAA 
}CJT188_MeterType;
//�����붨��
typedef enum{
  CJT188_ReadData_CTR         =  0x01 ,       //������ ������
  CJT188_ReadVerNo_CTR        =  0x09,        //����Կ�汾�� ������
  CJT188_ReadAddress_CTR      =  0x03,        //����ַ ������
  CJT188_WriteData_CTR        =  0x04,        //д���� ������
  CJT188_WriteAddress_CTR     =  0x15,        //д��ַ ������
  CJT188_WriteOffset_CTR      =  0x16,        //д����ͬ�����ݣ�����
  CJT188_ReadAllFlow_CTR      =  0x17,        //��ȡ���б�Ƶ�ǰˮ��  
  CJT188_ReadSingleFlow_CTR   =  0x18,        //��ȡ���б�Ƶ�ǰˮ��  
}CJT188_CMD;
//���ݱ�־����
typedef enum{
  CJT188_ReadData_DI          =  0x1F90,      //��������
  CJT188_ReadHistData_1_DI    =  0x20D1,      //��ʷ��������1
  CJT188_ReadHistData_2_DI    =  0x21D1 ,     //��ʷ��������2
  CJT188_ReadHistData_3_DI    =  0x22D1,      //��ʷ��������3
  CJT188_ReadHistData_4_DI    =  0x23D1,      //��ʷ��������4
  CJT188_ReadHistData_5_DI    =  0x24D1,      //��ʷ��������5
  CJT188_ReadHistData_6_DI    =  0x25D1,      //��ʷ��������6
  CJT188_ReadHistData_7_DI    =  0x26D1,      //��ʷ��������7
  CJT188_ReadHistData_8_DI    =  0x27D1,      //��ʷ��������8
  CJT188_ReadHistData_9_DI    =  0x28D1,      //��ʷ��������9
  CJT188_ReadHistData_10_DI   =  0x29D1,      //��ʷ��������10
  CJT188_ReadHistData_11_DI   =  0x2AD1,      //��ʷ��������11
  CJT188_ReadHistData_12_DI   =  0x2BD1,      //��ʷ��������12
  CJT188_ReadPriceList_DI     =  0x0281,      //���۸��
  CJT188_ReadSelDate_DI       =  0x0381,      //��������
  CJT188_ReadRecordDate_DI    =  0x0481,      //��������
  CJT188_ReadRMoney_DI        =  0x0581,      //��������
  CJT188_ReadVerNo_DI         =  0x0681,      //����Կ�汾��
  CJT188_ReadAddress_DI       =  0x0A81 ,     //����ַ
  
  CJT188_WritePriceList_DI    =  0x10A0,      //д�۸��
  CJT188_WriteSelDate_DI      =  0x11A0,      //д��������
  CJT188_WriteRecordDate_DI   =  0x12A0,      //д������
  CJT188_WriteRMoney_DI       =  0x13A0,      //��������
  CJT188_WriteSecretKey_DI    =  0x14A0 ,     //д��Կ�汾��
  CJT188_WriteTime_DI         =  0x15A0,      //д��עʱ��
  CJT188_WriteValveState_DI   =  0x17A0,      //д���ſ���
  CJT188_Lock_DI              =  0x19A0 ,     //��������
  CJT188_WriteAddress_DI      =  0x18A0 ,     //д��ַ
  CJT188_WriteOffset_DI       =  0x16A0,      //д��ַ
}CJT188_DI;


typedef enum{
  Wh        = 0x02,   //��ˮ��
  KWh       = 0x05,   //������ˮ��
  MWh       = 0x08,
  MWh_100   = 0x0a,
  J         = 0x01,
  KJ        = 0x0B,
  MJ        = 0x0E, 
  GJ        = 0x11,
  GJ_100    = 0x13,
  W         = 0x14,
  KW        = 0x17,
  MW        = 0x1A,
  L         = 0x29,
  m3        = 0x2C,
  L_h       = 0x32,
  m3_h      = 0x35,
}CJT188_UnitType;

typedef struct{
  unsigned char   MeterID[5];
  unsigned short  FactoryID;
}CJT188_AddresStruct;

typedef union{
  unsigned char Byte;
  struct{
    unsigned char Infr : 6;
    unsigned char CommFlg : 1;   //0��ͨ������  1��ͨ���쳣
    unsigned char DIR : 1;   // 0: ����վ�����Ŀ�����  1���ɴ�վ������Ӧ����
  }Bit;
}CJT188_CTRStruct;

typedef struct {
  unsigned char      StartCharacter;
  CJT188_MeterType    Type;
  CJT188_AddresStruct Addr;
  CJT188_CTRStruct    CTR;
  unsigned char      Length;
}CJT188_FrameHeadStruct;

typedef struct{
  CJT188_DI_Type   DI;
  CJT188_SER_Type  SER;
}CJT188_DataHeadStruct;

typedef  union {
  unsigned short Word;
  struct{
    unsigned char State1;
    unsigned char State2; 
  }Byte;
  struct{
    unsigned short Valve_State :2;    //����״̬ 00������ 01 �أ� 11���쳣
    unsigned short Bat_Flg: 1;        //��ص�ѹ   0������  1��Ƿѹ
    unsigned short Lose: 1;             //����  ����δ������
    unsigned short RomErr: 1;             //����   �洢������ 
    unsigned short F3: 1;             //�����Զ���3
    unsigned short F2: 1;             //�����Զ���2
    unsigned short F1: 1;             //�����Զ���1
    unsigned short Custom :8;
  }Bit;
}CJT188_ST_Stuct;

//ʵʱʱ�Ӹ�ʽ
typedef struct{
  unsigned short  Year;
  unsigned char   Mon;
  unsigned char   Day;
  unsigned char   Hour;
  unsigned char   Min;
  unsigned char   Sec;
}CJT188_Time_Struct;

//������ ��ʽ
typedef struct{
  unsigned char      Flow_PV[4];              //��ǰ�ۻ�����
  CJT188_UnitType     Flow_PV_Unit;            //��ǰ�ۻ�������λ
  unsigned char      Flow_Accounts[4];        //�������ۻ�����
  CJT188_UnitType     Flow_Accounts_Unit;      //�������ۻ�������λ
  CJT188_Time_Struct  Time;                    //ʵʱʱ��
  CJT188_ST_Stuct     ST;
}CJT188_MeterData_Struct;  

//����Ӧ֡��ʽ
typedef struct{
  CJT188_DataHeadStruct    DataHead;
  union{
   CJT188_MeterData_Struct MeterData; 
   CJT188_ST_Stuct         ST;
  }Data;  
}CJT188_OkACK_Struct;

//����Ӧ��֡��ʽ
typedef struct{
  CJT188_SER_Type         SER;
  CJT188_ST_Stuct         ST;
}CJT8188_ErrACK_Struct;

typedef struct{
  //CJT188_SER_Type         SER;
  unsigned char          ValveCmd;
}CJT8188_ValveAsk_Struct;
  
typedef struct{
CJT188_DataHeadStruct  DataHead;
CJT188_AddresStruct    Addres;
}CJT188_Address_OkAckStruct; 

//֡β��ʽ
typedef struct {
  unsigned char CS;     //У����,��֡ͷ��ʼ��֡У���־֮ǰ�����ֽڵ��ۼӺ͵�8λ	
  unsigned char Pause;  //������0x16H
}CJT188_FrameStern_Struct;

//
typedef enum{
  CJT188_ValveON      = 0x55,   //�������Ʋ���:
  CJT188_ValveOFF     = 0x99,   //�ط����Ʋ���:
}CJT188_ValveState_Type;


typedef struct{ 
  CJT188_FrameHeadStruct  Head;
  union{
    CJT188_OkACK_Struct    OK;
    CJT8188_ErrACK_Struct  ERR;
    unsigned char buff[CJT188_FrameParamLength];
  }Data;
}CJT188_Frame_Struct;
    
typedef struct {
  unsigned char          Preamble[CJT188_PreambleLength];
  CJT188_Frame_Struct     Frame; 
}CJT188_Package_Struct;

struct  CJT188_SendBuff_Struct{
  //unsigned char          flg;
  SemaphoreHandle_t       mutex; 
  CommPortNumber_Type     SourcePort;
  CommPortNumber_Type     TargetPort;     //ת����Ŀ��˿�
  unsigned char          PackageLength;
  CJT188_Package_Struct   Package;
  //unsigned char          Preamble[CJT188_PreambleLength];
  //CJT188_Frame_Struct     Package; 
};

#pragma pack()
//��������� 
typedef struct{
 CJT188_AddresStruct  Addr; 
 CJT188_MeterType     Type;
}CJT188_MeterDeclarator;

//CJT188����������
typedef struct{
 CommPortNumber_Type SourcePort; 
 unsigned char       Type;
 CJT188_AddresStruct Addr;
 CJT188_CTRStruct    CTR;
 CJT188_SER_Type     SER;
 CJT188_DI_Type      DataDI;
 unsigned char  ParamLength;
 unsigned char  Param[32];
}CJT188_CmdDeclarator;

///////////////////////////////////////////////////////////////////////////////////////////
//��Լ���ͼ�ͨ���� ���ݸ�ʽ
typedef  union{
  unsigned char Byte; 
  struct{
    unsigned char Protocol: 4;    /* ��Լ����   0000:	����188 */
    unsigned char Channel : 3;    /*ͨ����   0000:	δָ��ͨ��
    0001:	M-BUSͨ��1
    0010:	M-BUSͨ��2
    0011:	M-BUSͨ��3
    0100:	M-BUSͨ��4
    0101:	RS485ͨ��
    0110:	����ͨ�� */
    unsigned char Poll_EN : 1;   //�Ƿ�Ѳ��   0:��Ѳ��  1:Ѳ��
  }Bit;
}CJT188_LinkDeclarator;    
//ͨѶ���� ���ݸ�ʽ	
typedef union{
  unsigned char Byte; 
  struct{
    unsigned char BaudRate: 3; /*������ 000: 1200bps
    001: 2400bps
    010: 4800bps
    011: 9600bps
    100: 19200bps
    101: 38400bps  ��ʱδ��
    110: 57600bps  ��ʱδ��	
    111: 115200bps*/
    unsigned char DataNumber : 2;   //����λ  00: 6λ����λ  01: 7λ����λ 10: 8λ����λ
    unsigned char StopBits : 1;   //ֹͣλ  0 :1λֹͣλ   1 :2λֹͣλ
    unsigned char Parity : 2;   //У��λ  00:��У��     01:żУ��       10:��У��
  }Bit;
}CJT188_CommDeclarator; 
typedef struct {
  CJT188_MeterType        MeterType;      //�ڵ�����ID (HEX)  
  CJT188_AddresStruct     Addr;           //�ڵ��ַ
  CJT188_LinkDeclarator   Link;           //�ڵ��Լ���ͼ�ͨ���� (HEX)
  CJT188_CommDeclarator   Comm;	        //�ڵ�ͨѶ����
  unsigned char         Flow_PV[4];              //��ǰ�ۻ�����
  CJT188_ST_Stuct         ST;
}MeterData_Struct;
//���ǰˮ�����Ľṹ��
typedef struct{
  unsigned char        Preamble[2];              //ǰ����
  unsigned char        Initiator;              //��ʼ��
  unsigned char        ConcentratorID[5];      //��������ַ
  unsigned char        FactoryID[2];            //�������
  unsigned char        FunctionCode;           //������
  unsigned char        DataLength[2];            //���ݳ���
  unsigned char        ReplyFlag;              //��Ӧ��־
  unsigned char        PackageSum;             //������
  unsigned char        PackageNum;              //�����
  unsigned char        MeterNum;               //����
  MeterData_Struct       MeterData[17];  //����Ϣ
  unsigned char        CS;                     //У���
  unsigned char        Terminator;             //������
}MultiMeterFlowPVMessageStruct;
//����ǰˮ�����Ľṹ��
typedef struct{
  unsigned char        Preamble[2];              //ǰ����
  unsigned char        Initiator;              //��ʼ��
  unsigned char        ConcentratorID[5];      //��������ַ
  unsigned char        FactoryID[2];            //�������
  unsigned char        FunctionCode;           //������
  unsigned char        DataLength[2];            //���ݳ���
  unsigned char        ReplyFlag;              //��Ӧ��־
  MeterData_Struct       MeterData;  //����Ϣ
  unsigned char        CS;                     //У���
  unsigned char        Terminator;             //������
}SingleMeterFlowPVMessageStruct;
/*********************************************************************************************************
�ⲿ����������
*********************************************************************************************************/
extern  unsigned  char MbusMonitor_EN ;
/*********************************************************************************************************
����������
*********************************************************************************************************/
void CJT188_2004_init (void);

SystemErrName CJT188_2004_InPut (void *pbuff, unsigned length, CommPortNumber_Type port);

SystemErrName CJT188_ReadDataFormNode (CJT188_DI DI, 
                                       CJT188_MeterDeclarator *Meter, 
                                       CommPortNumber_Type Port, 
                                       CJT188_Frame_Struct*  ack);

SystemErrName CJT188_WriteDataToNode(CJT188_DI DI, 
                                              CJT188_MeterDeclarator* Meter, 
                                              CommPortNumber_Type Port, 
                                              void* Data, 
                                              unsigned char DataLength,
                                              CJT188_Frame_Struct*  ack);
void CJT188_ReadAllFlow_Func(unsigned char packegeNum,CommPortNumber_Type SourcePort);
void CJT188_ReadSingleFlow_Func(unsigned char*meterID,CommPortNumber_Type SourcePort);
/********************************************************************************************************/
#endif

/******************************************END********************************************************/