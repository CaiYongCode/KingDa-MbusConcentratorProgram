/*********************************************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************************************/
#ifndef  __METERLIST_H
#define  __METERLIST_H
/*********************************************************************************************************
文件包含区
*********************************************************************************************************/
#include ".\CJT-188-2004.h"
#include ".\Server188.h"
/*********************************************************************************************************
宏定义区
*********************************************************************************************************/

/*********************************************************************************************************
数据类型定义
*********************************************************************************************************/
#pragma pack(1)
//表计配置及数据  描述符
typedef struct {
  unsigned char              Flow_PV[4];              //当前累积流量
  unsigned char              Flow_Accounts[4];        //结算日累积流量
  CJT188_ST_Stuct             ST;
  Server188_Time_Stuct        Time;
  signed char                Temp;                      //温度
}Server188_MeterData_Struct;

typedef struct{
  unsigned char               OPID;	    //操作员ID(HEX)　
  Server188_Time_Stuct	       Time;
}Server188_OperateRecord_Stuct;

//表计配置及数据 存储单元格式 
typedef union{
    unsigned char Byte;
    struct{
      unsigned char Config_E :1;   //配置信息有效
      unsigned char Data_E:1;      //配置信息有效
      unsigned char Open_E :1;     //配置信息有效
      unsigned char Close_E :1;    //配置信息有效
    }Bit;
  }Server188_RecordData_CSR; //控制状态寄存器

typedef union{
    unsigned char Byte;
    struct{
      unsigned char Config_E :1;   //配置信息有效
      unsigned char PV_E:1;      //配置信息有效
      unsigned char Acc_E :1;
      unsigned char Open_E :1;     //配置信息有效
      unsigned char Close_E :1;    //配置信息有效
      unsigned char Lose :1;    //配置信息有效
    }Bit; 
  }Server188_ValueRecordData_CSR; //控制状态寄存器
  
  
typedef struct {
  Server188_RecordData_CSR              CSR;           //控制状态寄存器
  Server188_Meter_Stuct                 Config;        //表计配置信息
}Server188_MeterRecordData_Struct;

typedef struct {
  Server188_ValueRecordData_CSR         CSR;           //控制状态寄存器
  Server188_MeterData_Struct            Value;          //表计数据
  Server188_OperateRecord_Stuct         OpenRecord;    //开阀记录
  Server188_OperateRecord_Stuct         CloseRecord;   //关阀记录
}Server188_MeterValueRecordData_Struct;


typedef struct {
 Server188_MeterRecordData_Struct  data;
 unsigned short     CRC16;         //数据完整性验证
}Server188_MeterRecord_Struct;

typedef struct {
 Server188_MeterValueRecordData_Struct  data;
 unsigned short     CRC16;         //数据完整性验证
}Server188_MeterValueRecord_Struct;


//表计配置及数据 列表描述符
  
typedef struct {
  unsigned short MeterNumber[4];        //各通道表计  数量 
  unsigned short OpenNumber;            //开阀记录  数量 
  unsigned short CloseNumber;           //关阀记录  数量
}Server188_MeterListSpecData_Struct;
typedef struct {
  Server188_MeterListSpecData_Struct data;
  unsigned short CRC16;
}Server188_MeterListSpec_Struct;

#pragma pack()
/*********************************************************************************************************
外部变量声明区
*********************************************************************************************************/

/*********************************************************************************************************
函数声明区
*********************************************************************************************************/

SystemErrName Read_MeterListSpecFormRom(Server188_MeterListSpecData_Struct* List);

SystemErrName write_MeterListSpecToRom(Server188_MeterListSpecData_Struct* Spec);

SystemErrName ReadMeterRecordFormRom(Server188_MeterRecordData_Struct* Record, unsigned short No);

unsigned short ReadMeterRecordFormAddr(Server188_MeterRecordData_Struct* Record,  
                                       Server188_MeterAddr_Stuct* Addr,
                                       SystemErrName* err);

unsigned short ReadMeterRecordFormAddrAndLink(Server188_MeterRecordData_Struct* Record, 
                                              Server188_MeterAddr_Stuct* Addr,
                                              unsigned char CH,
                                              SystemErrName* err);

SystemErrName write_MeterRecordToRom(Server188_MeterRecordData_Struct* Record, unsigned short No);

SystemErrName Add_MeterRecordToRom(Server188_MeterRecordData_Struct* Record, unsigned short* StartNo);

SystemErrName delete_MeterRecordFormRom(unsigned short No);

SystemErrName ReadMeterValueFormRom(Server188_MeterValueRecordData_Struct* Value, unsigned short No);

SystemErrName write_MeterValueToRom(Server188_ValueRecordData_CSR csr, Server188_MeterData_Struct* Value, unsigned short No);

SystemErrName write_MeterOperateToRom(Server188_OperateDeclarator_Stuct* Operate,
                                      TimerData_Struct* Timer, 
                                      unsigned short No);
                                     
/********************************************************************************************************/
#endif
/******************************************END********************************************************/