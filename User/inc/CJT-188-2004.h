/*********************************************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************************************/
#ifndef  __CJT188_2004_H
#define  __CJT188_2004_H
/*********************************************************************************************************
文件包含区
*********************************************************************************************************/
#include "..\..\BSP\inc\bsp.h"
#include "..\..\Projects\inc\SystemDefine.h"
/*********************************************************************************************************
宏定义区
*********************************************************************************************************/
#define  CJT188_OutPutBuff_Number      4

#define  Locall_MeterType     UnknownMeterType
#define  Locall_FactoryID     ('K' *10) + 'D'

#define  Locall_Type                    0  //0 ：数据采集器   1：数据传感器

#define  PreambleNumber                 2 

#define  CJT188_Preamble                0xFE
#define  CJT188_PreambleLength          2
#define  CJT188_StartCharacter          0x68
#define  CJT188_Pause                   0x16

#define  CJT_188ValveOpenCmd            0x55      //开 阀 控 制 操作 :55H
#define  CJT_188ValveCloseCmd           0x99     //关 阀 控 制 操作 :99H
#define  CJT_188ValveMaintainCmd        0xAA     //关 阀 控 制 操作 :99H

#define CJT188_FrameParamLength         32
/*********************************************************************************************************
数据类型定义
*********************************************************************************************************/

#pragma pack(1)

typedef enum{ASK_CJT188 = 0, ACK_CJT188 = 1  }CJT188_DirType;

typedef unsigned char           CJT188_SER_Type;

typedef unsigned short          CJT188_DI_Type;

//表计类型定义
typedef enum{
  ColdWaterMeter  = 0x10,   //冷水表
  HeatWaterMeter  = 0x11,   //生活热水表
  DrinkWaterMeter = 0x12,   //直饮水表
  RecycledWaterMeter = 0x13,   //中水表
  UnknownMeterType   = 0xAA 
}CJT188_MeterType;
//命令码定义
typedef enum{
  CJT188_ReadData_CTR         =  0x01 ,       //读数据 （请求）
  CJT188_ReadVerNo_CTR        =  0x09,        //读秘钥版本号 （请求）
  CJT188_ReadAddress_CTR      =  0x03,        //读地址 （请求）
  CJT188_WriteData_CTR        =  0x04,        //写数据 （请求）
  CJT188_WriteAddress_CTR     =  0x15,        //写地址 （请求）
  CJT188_WriteOffset_CTR      =  0x16,        //写机电同步数据（请求）
  CJT188_ReadAllFlow_CTR      =  0x17,        //读取所有表计当前水量  
  CJT188_ReadSingleFlow_CTR   =  0x18,        //读取所有表计当前水量  
}CJT188_CMD;
//数据标志定义
typedef enum{
  CJT188_ReadData_DI          =  0x1F90,      //计量数据
  CJT188_ReadHistData_1_DI    =  0x20D1,      //历史计量数据1
  CJT188_ReadHistData_2_DI    =  0x21D1 ,     //历史计量数据2
  CJT188_ReadHistData_3_DI    =  0x22D1,      //历史计量数据3
  CJT188_ReadHistData_4_DI    =  0x23D1,      //历史计量数据4
  CJT188_ReadHistData_5_DI    =  0x24D1,      //历史计量数据5
  CJT188_ReadHistData_6_DI    =  0x25D1,      //历史计量数据6
  CJT188_ReadHistData_7_DI    =  0x26D1,      //历史计量数据7
  CJT188_ReadHistData_8_DI    =  0x27D1,      //历史计量数据8
  CJT188_ReadHistData_9_DI    =  0x28D1,      //历史计量数据9
  CJT188_ReadHistData_10_DI   =  0x29D1,      //历史计量数据10
  CJT188_ReadHistData_11_DI   =  0x2AD1,      //历史计量数据11
  CJT188_ReadHistData_12_DI   =  0x2BD1,      //历史计量数据12
  CJT188_ReadPriceList_DI     =  0x0281,      //读价格表
  CJT188_ReadSelDate_DI       =  0x0381,      //读结算日
  CJT188_ReadRecordDate_DI    =  0x0481,      //读抄表日
  CJT188_ReadRMoney_DI        =  0x0581,      //读购入金额
  CJT188_ReadVerNo_DI         =  0x0681,      //读秘钥版本号
  CJT188_ReadAddress_DI       =  0x0A81 ,     //读地址
  
  CJT188_WritePriceList_DI    =  0x10A0,      //写价格表
  CJT188_WriteSelDate_DI      =  0x11A0,      //写结算日期
  CJT188_WriteRecordDate_DI   =  0x12A0,      //写抄表日
  CJT188_WriteRMoney_DI       =  0x13A0,      //读购入金额
  CJT188_WriteSecretKey_DI    =  0x14A0 ,     //写秘钥版本号
  CJT188_WriteTime_DI         =  0x15A0,      //写标注时间
  CJT188_WriteValveState_DI   =  0x17A0,      //写阀门控制
  CJT188_Lock_DI              =  0x19A0 ,     //出厂启用
  CJT188_WriteAddress_DI      =  0x18A0 ,     //写地址
  CJT188_WriteOffset_DI       =  0x16A0,      //写地址
}CJT188_DI;


typedef enum{
  Wh        = 0x02,   //冷水表
  KWh       = 0x05,   //生活热水表
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
    unsigned char CommFlg : 1;   //0：通信正常  1：通信异常
    unsigned char DIR : 1;   // 0: 由主站发出的控制桢  1：由从站发出的应答桢
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
    unsigned short Valve_State :2;    //阀门状态 00：开， 01 关， 11：异常
    unsigned short Bat_Flg: 1;        //电池电压   0：正常  1：欠压
    unsigned short Lose: 1;             //保留  本次未抄到表
    unsigned short RomErr: 1;             //保留   存储器错误 
    unsigned short F3: 1;             //厂商自定义3
    unsigned short F2: 1;             //厂商自定义2
    unsigned short F1: 1;             //厂商自定义1
    unsigned short Custom :8;
  }Bit;
}CJT188_ST_Stuct;

//实时时钟格式
typedef struct{
  unsigned short  Year;
  unsigned char   Mon;
  unsigned char   Day;
  unsigned char   Hour;
  unsigned char   Min;
  unsigned char   Sec;
}CJT188_Time_Struct;

//读数据 格式
typedef struct{
  unsigned char      Flow_PV[4];              //当前累积流量
  CJT188_UnitType     Flow_PV_Unit;            //当前累积流量单位
  unsigned char      Flow_Accounts[4];        //结算日累积流量
  CJT188_UnitType     Flow_Accounts_Unit;      //结算日累积流量单位
  CJT188_Time_Struct  Time;                    //实时时间
  CJT188_ST_Stuct     ST;
}CJT188_MeterData_Struct;  

//正常应帧格式
typedef struct{
  CJT188_DataHeadStruct    DataHead;
  union{
   CJT188_MeterData_Struct MeterData; 
   CJT188_ST_Stuct         ST;
  }Data;  
}CJT188_OkACK_Struct;

//错误应答帧格式
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

//帧尾格式
typedef struct {
  unsigned char CS;     //校验码,从帧头开始到帧校验标志之前所有字节的累加和低8位	
  unsigned char Pause;  //结束符0x16H
}CJT188_FrameStern_Struct;

//
typedef enum{
  CJT188_ValveON      = 0x55,   //开阀控制操作:
  CJT188_ValveOFF     = 0x99,   //关阀控制操作:
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
  CommPortNumber_Type     TargetPort;     //转发的目标端口
  unsigned char          PackageLength;
  CJT188_Package_Struct   Package;
  //unsigned char          Preamble[CJT188_PreambleLength];
  //CJT188_Frame_Struct     Package; 
};

#pragma pack()
//表计描述符 
typedef struct{
 CJT188_AddresStruct  Addr; 
 CJT188_MeterType     Type;
}CJT188_MeterDeclarator;

//CJT188命令描述符
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
//规约类型及通道号 数据格式
typedef  union{
  unsigned char Byte; 
  struct{
    unsigned char Protocol: 4;    /* 规约类型   0000:	阀控188 */
    unsigned char Channel : 3;    /*通道号   0000:	未指定通道
    0001:	M-BUS通道1
    0010:	M-BUS通道2
    0011:	M-BUS通道3
    0100:	M-BUS通道4
    0101:	RS485通道
    0110:	无线通道 */
    unsigned char Poll_EN : 1;   //是否巡检   0:不巡检  1:巡检
  }Bit;
}CJT188_LinkDeclarator;    
//通讯参数 数据格式	
typedef union{
  unsigned char Byte; 
  struct{
    unsigned char BaudRate: 3; /*波特率 000: 1200bps
    001: 2400bps
    010: 4800bps
    011: 9600bps
    100: 19200bps
    101: 38400bps  暂时未用
    110: 57600bps  暂时未用	
    111: 115200bps*/
    unsigned char DataNumber : 2;   //数据位  00: 6位数据位  01: 7位数据位 10: 8位数据位
    unsigned char StopBits : 1;   //停止位  0 :1位停止位   1 :2位停止位
    unsigned char Parity : 2;   //校验位  00:无校验     01:偶校验       10:奇校验
  }Bit;
}CJT188_CommDeclarator; 
typedef struct {
  CJT188_MeterType        MeterType;      //节点类型ID (HEX)  
  CJT188_AddresStruct     Addr;           //节点地址
  CJT188_LinkDeclarator   Link;           //节点规约类型及通道号 (HEX)
  CJT188_CommDeclarator   Comm;	        //节点通讯参数
  unsigned char         Flow_PV[4];              //当前累积流量
  CJT188_ST_Stuct         ST;
}MeterData_Struct;
//多表当前水量报文结构体
typedef struct{
  unsigned char        Preamble[2];              //前导符
  unsigned char        Initiator;              //起始符
  unsigned char        ConcentratorID[5];      //集中器地址
  unsigned char        FactoryID[2];            //出厂编号
  unsigned char        FunctionCode;           //功能码
  unsigned char        DataLength[2];            //数据长度
  unsigned char        ReplyFlag;              //响应标志
  unsigned char        PackageSum;             //包总数
  unsigned char        PackageNum;              //包序号
  unsigned char        MeterNum;               //表数
  MeterData_Struct       MeterData[17];  //表信息
  unsigned char        CS;                     //校验和
  unsigned char        Terminator;             //结束符
}MultiMeterFlowPVMessageStruct;
//单表当前水量报文结构体
typedef struct{
  unsigned char        Preamble[2];              //前导符
  unsigned char        Initiator;              //起始符
  unsigned char        ConcentratorID[5];      //集中器地址
  unsigned char        FactoryID[2];            //出厂编号
  unsigned char        FunctionCode;           //功能码
  unsigned char        DataLength[2];            //数据长度
  unsigned char        ReplyFlag;              //响应标志
  MeterData_Struct       MeterData;  //表信息
  unsigned char        CS;                     //校验和
  unsigned char        Terminator;             //结束符
}SingleMeterFlowPVMessageStruct;
/*********************************************************************************************************
外部变量声明区
*********************************************************************************************************/
extern  unsigned  char MbusMonitor_EN ;
/*********************************************************************************************************
函数声明区
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