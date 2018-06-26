/*********************************************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************************************/
#ifndef  __Server188_H
#define  __Server188_H
/*********************************************************************************************************
文件包含区
*********************************************************************************************************/
#include ".\CJT-188-2004.h"
/*********************************************************************************************************
宏定义区
*********************************************************************************************************/

#define  Server188_OutPutBuff_Number    3               //输出缓冲区数量

#define  Server188_AreaCode             0xEE            //集中器 区域码

#define  Server188_Preamble             0xFEFE            //同步字
#define  Server188_StartCharacter       0x68            //帧头
#define  Server188_Pause                0x16            //帧尾

#define  ConcentratorAddr_Length        sizeof(CJT188_AddresStruct)               //集中器地址长度 
#define	 LSSB			        0x10            //冷水水表

/*********************************************************************************************************
数据类型定义
*********************************************************************************************************/

//表计地址格式
typedef   CJT188_AddresStruct  Server188_MeterAddr_Stuct;

typedef enum {
  DownIdle         = 0,    // 0:  下行通道空闲; 
  FullReading      = 1,    // 1：正在抄表;
  FullPatroling    = 2,    // 2: 正在巡检表地址;
  ReadingFinish    = 3,
}Server188_ConcenMbusState_Type;


//主站与集中器之间 命令码定义
typedef enum {
  Server188_Time_Wcmd		        = 0x01,	//1设置集中器时间														
  Server188_Time_Rcmd		        = 0x02,	//2读取集中器时间															
  Server188_ConfigData_Wcmd		= 0x03,	//3设置集中器抄表参数													
  Server188_ConfigData_Rcmd             = 0x04,	//4读取集中器抄表参数													
  Server188_Gprs_Wcmd		        = 0x07,	//5设置集中器GPRS参数													
  Server188_Gprs_Rcmd		        = 0x08,	//6读取集中器GPRS参数													
  Server188_Meter_Wcmd		        = 0x09,	//7下载表计地址															
  Server188_Meter_Rcmd		        = 0x0B,	//8读取下载成功表计地址													
  Server188_MeterErr_Rcmd		= 0x0C,	//9读取下载失败表计地址													
  Server188_Operate_Wcmd		= 0x0E,	//10下达控制命令															
  Server188_Operate_Rcmd		= 0x0F,	//11读取控制命令															
  Server188_Reading_Wcmd		= 0x14,	//12启动立即抄表															
  Server188_AllCurrentData_Rcmd	        = 0x15,	//13读取某一采集器的所有表计当前数据									
  Server188_AllFreezingData_Rcmd	= 0x16,	//14读取某一采集器的所有表计日冻结数据									
  Server188_SingCurrentData_Rcmd	= 0x17,	//15读取单表的当前数据													
  Server188_SingFreezingData_Rcmd	= 0x18,	//16读取单表的日冻结数据													
  Server188_AlterMeter_Wcmd	        = 0x1C,	//17替换表计																
  Server188_DeleteMeter_Wcmd	        = 0x1D,	//18删除表计																
  Server188_Beat_cmd		        = 0x1E,	//
  Server188_CleanMeter_cmd		= 0x20,	//
  Server188_ErrUpData                   = 0x21,
  
}Server188_CMD;
typedef enum {
  NoErr   = 0x00,
  MubusOverLoad   = 0x01,
  
}Server188_ErrType_Stuct;




typedef enum ReplyFlag
{
  OK           = 0x00,        //正常 
  TimeErr      = 0x10,        //时间不合法, 
  EepromErr    = 0x11,        //数据写入eeprom不正确 , 
  NoDats       = 0x20,        //无此包 , 
  NoMeter      = 0x21,        //未查询到表计 , 
  ParamErr     = 0x22,        //参数不合法 ,
  MbusErr      = 0x23,        //MBUS通道异常 ,
  MeterNoAck   = 0x24,        //表计无响应 ,
  NoAck        = 0x25,        //无应答 ,
  MbusBusy     = 0x26,        //下行忙 ,
  MbusTimeOut  = 0x27,        //下行操作超时 , 
  NoPackage    = 0xCA,        //无此包 
  Reading      = 0x01,        //正在抄表 , 
  Patroling    = 0x02,        //正在巡检表地址 , 
  NoDay        = 0x30,        //未找到该日期 , 
  MeterRrepeat = 0x04,        //地址重复 ,
}Server188_ReplyFlag;

#pragma pack(1)
//集中器时间  数据格式
typedef struct {
  unsigned char        Year;   	//年只取低两位 (BCD)
  unsigned char	Month;  	//月 (BCD)
  unsigned char	Day;    	//日 (BCD)
  unsigned char	Hour;		//时 (BCD)
  unsigned char        Minute;		//分 (BCD)
  unsigned char        Second;		//秒 (BCD)
}Server188_Time_Stuct;

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
}Server188_LinkDeclarator;    

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
}Server188_CommDeclarator; 

//集中器抄表参数  数据格式
typedef struct {
  union{
    unsigned short Word; 
    struct{
      unsigned char Byte_L;   //低字节
      unsigned char Byte_H;   //高字节
    }Byte;
  }ReadingCycle;	
  unsigned char	DayFrozen;	//日冻结时刻 0—23；对所有通道有效  HEX
  unsigned char	MonthFrozenH;	//月冻结时间，1-31  HEX 	
  unsigned char	MonthFrozenL;	//月冻结时刻，0-23  HEX
  union{
    unsigned char Byte; 
    struct{
      unsigned char D0 : 1;     //1为关闭抄表间隔。
      unsigned char D1 : 1;     //1为关闭日冻结，但是常规情况下不关闭日冻结。
      unsigned char D2 : 1;	//1为关闭月冻结。
      unsigned char D3 : 1;	//1为关闭整点抄表。
    }Bit;
  }FreezingSwitch;
}Server188_TimerData_Stuct;

//集中器GPRS参数  数据格式
typedef struct {
  union{
    unsigned char Byte; 
    struct{
      unsigned char D0 : 1;     //代表是否使用DNS，0，使用IP地址；1使用DNS
      unsigned char D1 : 1;     //代表是否使用APN，0，不使用APN；1使用APN 
    }Bit;
  }ConnectionMode;	        //GPRS连接方式(HEX)
  unsigned char IP[4];		//十六进制码 第一字节(HEX)
  union{
    unsigned short Word; 
    struct{
      unsigned char Byte_L;   //低字节
      unsigned char Byte_H;   //高字节
    }Byte;
  }Port;	              //低字节 十六进制 低位在前 高位在后 (HEX)       
  char APN[16];  //16字节；若不足16字节，空余部分填充‘\0’ (ASCII)
  char DNS[37];  //37字节；若不足37字节，空余部分填充 ‘\0’(ASCII)	
  unsigned char	BeatCycle; //1字节（单位：分钟）(HEX)
}Server188_Gprs_Stuct;

//节点描述符格式
typedef struct {
  CJT188_MeterType              MeterType;      //节点类型ID (HEX)  
  Server188_MeterAddr_Stuct     Addr;           //节点地址
  Server188_LinkDeclarator      Link;           //节点规约类型及通道号 (HEX)
  Server188_CommDeclarator      Comm;	        //节点通讯参数
}Server188_Meter_Stuct;

//表计数据格式
typedef struct{
  Server188_Meter_Stuct Meter;             
  unsigned char Value[4];
                                // Byte1;    //当前累积流量小数点后两位(BCD)
                                // Byte2;    //当前累积流量整数部分的低两位(BCD)
                                // Byte3;    //当前累积流量整数部分的中间两位(BCD)
                                // Byte4;    //当前累积流量整数部分的高两位(BCD)
  CJT188_ST_Stuct    ST;        //表计状态
}Server188_MeterValue_Stuct;

//控制记录格式：
typedef struct{
  unsigned char               OPID;	    //操作员ID(HEX)　
  Server188_Time_Stuct	       Time;
  CJT188_MeterType             MeterType;   //被控表类型ID(HEX)
  Server188_MeterAddr_Stuct    Addr;        //表地址
  Server188_LinkDeclarator     Link;        //节点规约类型及通道号 (HEX)
  Server188_CommDeclarator     Comm;        //下行通讯参数(HEX)
  unsigned char  MeterState;               //阀门状态(HEX) 0X99：关阀；0X55：开阀
}Server188_OperateDeclarator_Stuct;

//设置集中器时间命令 主站——>集中器 命令参数定义 (控制码：0X01)
typedef struct {
  unsigned char Year;   	        //年 BCD
  unsigned char	Month;  	//月 BCD
  unsigned char	Day;    	//日 BCD
  unsigned char	Hour;		//时 BCD 
  unsigned char Minute;		//分 BCD
  unsigned char Second;		//秒 BCD
  unsigned char WeekDay;	        //周几 BCD
  unsigned char SER;	                /*序列号(HEX)*/
}Server188_TimeWcmd_askParam_Stuct; 
//设置集中器时间命令 集中器——>主站 命令参数定义 (控制码：0X01)
typedef struct{
  unsigned char ReplyFlag;     /*应答标志：(HEX)
                                  0x00 :正常
                                  0xCC : 时间不合法 */
}Server188_TimeWcmd_ackParam_Stuct;

//读取集中器时间命令 主站——>集中器 命令参数定义 (控制码：0X02)
typedef struct{
  unsigned char SER;	        /*序列号(HEX)*/
}Server188_TimeRcmd_askParam_Stuct;
//读取集中器时间命令 集中器——>主站 命令参数定义 (控制码：0X02)
typedef struct
{
  unsigned char Year;   	//年 BCD
  unsigned char Month;  	//月 BCD
  unsigned char Day;    	//日 BCD
  unsigned char Hour;		//时 BCD 
  unsigned char Minute;	//分 BCD
  unsigned char Second;	//秒 BCD
  unsigned char WeekDay;	//周几 BCD
  unsigned char ReplyFlag;     //应答标志(HEX)：0x00正常，无异常应答 
}Server188_TimeRcmd_ackParam_Stuct;

//设置集中器抄表参数   主站——>集中器  命令参数定义 (控制码：0X03)
typedef struct{
  Server188_TimerData_Stuct   data;
  unsigned char              SER;	/*序列号(HEX)*/
}Server188_ConfigDataWcmd_askParam_Stuct;
//设置集中器抄表参数   集中器——> 主站 命令参数定义 (控制码：0X03)
typedef struct{
  unsigned char  ReplyFlag;     /*应答标志：(HEX)
                                    0x00 : 正常 
                                    0xCC : 数据写入eeprom不正确*/
}Server188_ConfigDataWcmd_ackParam_Stuct;

//读取集中器抄表参数   主站——>集中器  命令参数定义 (控制码：0X04)
typedef struct{
  unsigned char         SER;	/*序列号(HEX)*/
}Server188_ConfigDataRcmd_askParam_Stuct;

//读取集中器抄表参数   集中器——> 主站  命令参数定义 (控制码：0X04) 	
typedef struct{
  Server188_TimerData_Stuct Data;
  unsigned char  ReplyFlag;   //应答标志(HEX)：0x00正常，无异常应答  
}Server188_ConfigDataRcmd_ackParam_Stuct;

//设置集中器GPRS参数   主站——>集中器  命令参数定义 (控制码：0x07) 
typedef struct{
  Server188_Gprs_Stuct   data;
  unsigned char   SER;	/*序列号(HEX)*/
}Server188_GprsWcmd_askParam_Stuct;
//设置集中器GPRS参数   集中器——> 主站 命令参数定义 (控制码：0X07)
typedef struct{
  unsigned char  ReplyFlag;   //应答标志(HEX)：0x00正常，无异常应答
}Server188_GprsWcmd_ackParam_Stuct;

//读取集中器抄表参数   主站——>集中器  命令参数定义 (控制码：0X08)
typedef struct{
  unsigned char         SER;	/*序列号(HEX)*/
}Server188_GprsRcmd_askParam_Stuct;

//读取集中器GPRS参数   集中器——>主站  命令参数定义 (控制码：0X08)
typedef struct{
  Server188_Gprs_Stuct Data;
  unsigned char ReplyFlag;   //应答标志(HEX)：0x00正常，无异常应答
}Server188_GprsRcmd_ackParam_Stuct;

//下载表计地址   主站——>集中器  命令参数定义(控制码：0X09)
typedef struct {
  unsigned char  PackageSum;      //共有分包(HEX)
  union{
    unsigned short Word;
    struct{
      unsigned char Byte_L;   //低字节
      unsigned char Byte_H;   //高字节
    }Byte;		
  }MeterSum;                     //共有表数(HEX)         
  unsigned char  PackageNumber;	 //第几包数据(HEX)
  unsigned char  MetNumber; 	 //该包表计地址个数(HEX)
  Server188_Meter_Stuct Meter[25];      //表计说明数据(HEX)
  unsigned char  SER;	     /*序列号(HEX)*/
}Server188_MeterWcmd_askParam_Stuct;

//下载表计地址   集中器——>主站 命令参数定义 (控制码：0X09)
typedef struct{
  unsigned char  ReplyFlag;   //应答标志(HEX)：0x00 下载成功，无异常
}Server188_MeterWcmd_ackParam_Stuct;

//读取下载成功表计地址  主站——>集中器  命令参数定义(控制码：0x0B)
typedef struct {
  //unsigned char  Channel;   /*通道号:(HEX)0x01—0x06									
  //00000001:	M-BUS通道1							
  //00000010:	M-BUS通道2							
  //00000011:	M-BUS通道3
  //00000100:	M-BUS通道4
  //00000101:	RS485通道
  //00000110:	无线通道*/
  unsigned char  PackageNumber;   //第几包数据(HEX)
  unsigned char  SER;	     /*序列号(HEX)*/
}Server188_MeterRcmd_askParam_Stuct;

//读取下载成功表计地址  集中器——>主站  命令参数定义(控制码：0x0B)
typedef struct {
  unsigned char  PackageSum;	 //共有分包(HEX)
  union{
    unsigned short Word;
    struct{
      unsigned char Byte_L;      //低字节
      unsigned char Byte_H;      //高字节
    }Byte;
  }MeterSum;                     //共有表数(HEX)       
  unsigned char  PackageNumber;	 //第几包数据(HEX)	
  unsigned char  MetNumber; 	 //该包表计个数
}Server188_MeterRcmd_ackParam_Spec_Stuct;

typedef struct {
  Server188_MeterRcmd_ackParam_Spec_Stuct   Spec;
  Server188_Meter_Stuct  Meter[25];      //表计说明数据  
}Server188_MeterRcmd_ackParam_Stuct;

//读取下载失败表计地址  主站——>集中器  命令参数定义(控制码：0x0C)
typedef struct {
  unsigned char  PackageNumber;	//第几包数据(HEX)
  unsigned char  SER;	        /*序列号(HEX)*/
}Server188_MeterErrRcmd_askParam_Stuct;

//读取下载成功表计地址  集中器——>主站  命令参数定义(控制码：0x0C)
typedef struct {
  unsigned char  PackageSum;    //共有分包(HEX)
  union{
    unsigned short Word;
    struct{
      unsigned char Byte_L;     //低字节
      unsigned char Byte_H;     //高字节
    }Byte;		
  }MeterSum;                            //  共有表数(HEX)
  unsigned char  PackageNumber;	//第几包数据(HEX)	
  unsigned char  MetNumber; 	        //该包表计地址个数(HEX)
}Server188_MeterErrRcmd_ackParam_Spec_Stuct;

typedef struct {
  Server188_MeterErrRcmd_ackParam_Spec_Stuct  Spec;
  Server188_Meter_Stuct Meter[25];     //表计说明数据(HEX)  
}Server188_MeterErrRcmd_ackParam_Stuct;

//下达控制命令 主站——>集中器  命令参数定义(控制码：0X0E)  
typedef struct {
  Server188_OperateDeclarator_Stuct  Operate;
  unsigned char           SER;	    /*序列号(HEX)*/
}Server188_OperateWcmd_askParam_Stuct;

//下达控制命令  集中器——>主站  命令参数定义(控制码：0X0E)
typedef struct {
  CJT188_MeterType  MeterType;	  	//表计类型ID   0x16   HEX	    
  Server188_MeterAddr_Stuct  Addr;
  unsigned char  MetState;  /*阀门状态(HEX)  0x00：关阀成功
  0x01：关阀不到位
  0x02：关阀失败
  0x03：开阀成功
  0x04：开阀不到位
  0x05：开阀失败*/
  unsigned char ReplyFlag;  /*应答标志 (HEX) 0x00: 成功，
  0x01: 未查询到表计；
  0x02: 参数不合法；
  0x05: MBUS通道异常；
  0x06: 表计无响应；
  0x07: 无应答
  0xCB: 下行忙；
  0xCE: 下行操作超时*/
}Server188_OperateWcmd_ackParam_Stuct;

//取控制记录   主站——>集中器  命令参数定义(控制码：0X0F	)  
typedef struct {
  unsigned char  PackageNumber;	//第几包数据  HEX
  unsigned char  SER;	        /*序列号(HEX)*/
}Server188_OperateRcmd_askParam_Stuct;

//取控制记录   集中器——>主站  命令参数定义(控制码：0X0F)
typedef struct {
  unsigned short  PackageSum;           //共几包数据(HEX)
  unsigned short  PackageNumber;        //第几包数据(HEX)
  unsigned short  ConNumber;            //控制记录数N (HEX)
}Server188_OperateRcmd_ackParam_Spec_Stuct;
typedef struct {

  Server188_OperateRcmd_ackParam_Spec_Stuct  Spec;
  Server188_OperateDeclarator_Stuct  Record[15];  //控制记录数据;
  unsigned char  ReplyFlag;	       /*应答标志(HEX)：0x00:正常
                                         0xCA:无此包*/
}Server188_OperateRcmd_ackParam_Stuct;

//启动立即抄表  主站——>集中器  命令参数定义(控制码：0X14)  
typedef struct {
  unsigned char SER;	   /*序列号(HEX)*/  	
}Server188_ReadingWcmd_askParam_Stuct;

//启动立即抄表  集中器——>主站  命令参数定义(控制码：0X14)  
typedef struct {
  unsigned char ReplyFlag; /*应答标志 0x00:正常启动抄表
                              0x01:下行忙已经在抄表了,不需再次启动
                              0x02:下行正在巡检表地址,不能启动抄表*/ 	
}Server188_ReadingWcmd_ackParam_Stuct;

//读取所有表计当前数据  主站——>集中器  命令参数定义(控制码：0X15)
typedef struct {
  unsigned char PackageNumber;	//第几包数据(HEX)
  unsigned char SER;	   /*序列号(HEX)*/  
}Server188_AllCurrentDataRcmd_askParam_Stuct; 

//读取所有表计当前数据  集中器——>主站  命令参数定义(控制码：0X15) 
typedef struct {
  unsigned char FileSum;    //文件总数(HEX)	
  unsigned char Filecount;  //文件序号(HEX)
  unsigned char RecordNum;  //纪录数；包含表数量 (HEX)
  Server188_Time_Stuct Time;       //时间 
}Server188_AllCurrentDataRcmd_ackParam_Spec_Stuct;

typedef struct {
  Server188_AllCurrentDataRcmd_ackParam_Spec_Stuct Spec;
  Server188_MeterValue_Stuct Value[17];
}Server188_AllCurrentDataRcmd_ackParam_Stuct;

//读取全部表计日冻结数据   主站——>集中器  命令参数定义(控制码：0X16)
typedef struct {
  unsigned char PackageNumber;	//第几包数据(HEX)
  unsigned char Year;	        //年 低字节(BCD)
  unsigned char Month;          //月(BCD)
  unsigned char Day;		//日(BCD)
  unsigned char SER;	        /*序列号(HEX)*/  
}Server188_AllFreezingDataRcmd_askParam_Stuct;  

//读取全部表计日冻结数据   集中器——>主站  命令参数定义(控制码：0X16) 
typedef struct {
  unsigned char FileSum;       //文件总数(HEX)	
  unsigned char Filecount;     //文件序号(HEX)
  unsigned char RecordNum;     //纪录数；包含表数量 (HEX)
  Server188_Time_Stuct Time;          //时间 
}Server188_AllFreezingDataRcmd_ackParam_Spec_Stuct;

typedef struct {
  Server188_AllFreezingDataRcmd_ackParam_Spec_Stuct   Spec;
  union{
    Server188_MeterValue_Stuct  CJT188[17];
    unsigned char buff[sizeof(Server188_MeterValue_Stuct )*17];
  }Value; 
}Server188_AllFreezingDataRcmd_ackParam_Stuct;

//读单表的当前数据   主站——>集中器  命令参数定义(控制码：0X17)
typedef struct {
  Server188_Meter_Stuct  Meter; //表计描述
  unsigned char   SER;	 /*序列号(HEX)*/  
}Server188_SingCurrentDataRcmd_askParam_Stuct;  

//读单表的当前数据   集中器——>主站  命令参数定义(控制码：0X17)  
typedef struct {
  //Server188_MeterValue_Stuct Data;
  Server188_Meter_Stuct Meter;             
  unsigned char Value[4];
                                // Byte1;    //当前累积流量小数点后两位(BCD)
                                // Byte2;    //当前累积流量整数部分的低两位(BCD)
                                // Byte3;    //当前累积流量整数部分的中间两位(BCD)
                                // Byte4;    //当前累积流量整数部分的高两位(BCD)
  CJT188_ST_Stuct    ST;        //表计状态
  unsigned char ReplyFlag; /*应答标志(HEX)：0x00:正常，0xCF:未找到表 */
}Server188_SingCurrentDataRcmd_ackParam_Stuct;

//读取单表的日冻结数据   主站——>集中器  命令参数定义(控制码：0X18)
typedef struct {
  unsigned char	  Year;	          //年 低字节(BCD)
  unsigned char	  Month;          //月(BCD)
  unsigned char	  Day;	          //日(BCD)
  Server188_Meter_Stuct   Meter;          //表计描述
  unsigned char   SER;	                  /*序列号(HEX)*/  
}Server188_SingFreezingDataRcmd_askParam_Stuct;

//读取单表的日冻结数据   集中器——>主站  命令参数定义(控制码：0X18)  
typedef struct {
  Server188_Time_Stuct          Time;            //时间
  Server188_Meter_Stuct         Meter;           //节点描述符           
  unsigned char                Value[4];       // Byte1;    //当前累积流量小数点后两位(BCD)
                                                // Byte2;    //当前累积流量整数部分的低两位(BCD)
                                                // Byte3;    //当前累积流量整数部分的中间两位(BCD)
                                                // Byte4;    //当前累积流量整数部分的高两位(BCD)
  CJT188_ST_Stuct               ST;             //状态
  unsigned char ReplyFlag;                    /*应答标志(HEX)：0x00:正常;
                                                                0xCD:未找到该日期 
                                                                0xCF:未找到表 */
}Server188_SingFreezingDataRcmd_ackParam_Stuct;

//替换表计   主站——>集中器  命令参数定义(控制码：0X1C)
typedef struct {
  Server188_Meter_Stuct  OldMeter; 	//表计描述
  Server188_Meter_Stuct  NewMeter; 	//表计描述
  unsigned char         SER;	        /*序列号(HEX)*/  
}Server188_AlterMeterWcmd_askParam_Stuct;  
//替换表计   集中器——>主站  命令参数定义(控制码：0X1C) 
typedef struct {
  unsigned char ReplyFlag; /*应答标志(HEX): 0x00: 替换成功
                              0x01: 未查询到表计
                              0x02: 参数不合法
                              0x04: 地址重复
                              0x05: MBUS通道异常
                              0x06: 表计无响应
                              0xCB: 下行忙
                              0xCE: 下行操作超时
                              0xEF: 存储器写入错误  
                                */
  //unsigned char   SER;	    /*序列号(HEX)*/ 
}Server188_AlterMeterWcmd_ackParam_Stuct;   

//删除表计   主站——>集中器  命令参数定义(控制码：0X1c)
typedef struct {
  unsigned char         TotalNumber;    //删除记录条数(HEX)  
  Server188_Meter_Stuct  Meter[25]; 	  //表计描述
  unsigned char         SER;	          /*序列号(HEX)*/ 
}Server188_DeleteMeterWcmd_askParam_Stuct; 

typedef struct {
  Server188_MeterAddr_Stuct     Addr;
  unsigned char Flg;
}Server188_DeleteResult_Stuct;

//删除表计   集中器——>主站  命令参数定义(控制码：0X1c) 
typedef struct {
  unsigned char TotalNumber;      //删除记录条数(HEX)
  Server188_DeleteResult_Stuct Result[25]; // 第N条 00删除成功，01删除失败

  //unsigned char ReplyFlag;  //应答标志 0x00:删除成功
                              //0x01:下行忙已经在抄表了,不需再次启动
                              //0x02:下行正在巡检表地址,不能启动抄表	
}Server188_DeleteMeterWcmd_ackParam_Stuct; 


  
//清除表计   主站——>集中器  命令参数定义(控制码：0X20)
typedef struct {
  unsigned char         SER;	          /*序列号(HEX)*/ 
}Server188_CleanMeterWcmd_askParam_Stuct;   
  
//清除表计   集中器——>主站  命令参数定义(控制码：0X20) 
typedef struct {
  unsigned char ReplyFlag;  //应答标志 0x00:删除成功
                              //0x01:下行忙已经在抄表了,不需再次启动
                              //0x02:下行正在巡检表地址,不能启动抄表	
}Server188_CleanMeterWcmd_ackParam_Stuct; 



  
 typedef  union{
  unsigned long Word; 
  struct{

    unsigned long Mbus1 : 1;    
    unsigned long Mbus2 : 1;
    unsigned long Mbus3 : 1;
    unsigned long Mbus4 : 1;
    unsigned long MbusPower : 1;  
    unsigned long Mode  : 2;  //工作模式
  }Bit;
}Server188_ConcentratorState_Stuct; 

typedef  union{
  unsigned long Word; 
  struct{
    unsigned long Mbus1 : 1;    
    unsigned long Mbus2 : 1;
    unsigned long Mbus3 : 1;
    unsigned long Mbus4 : 1;    
  }Bit;
}Server188_ConcentratorErrFlg_Stuct; 

typedef struct {
  unsigned char         gprsRssi;
  unsigned char         Bat;      //电源电压       
  Server188_Time_Stuct   time;
  unsigned long long    Flow[2];
  union{
      unsigned char Byte;
      struct{
        unsigned char  CN     :1;    //服务器联接标志    
        unsigned char  ACK    :1;    //服务器 应答
        unsigned char  GZ     :1;    //重联超过规次数
        unsigned char  EN     :1;    //服务器启用
      }Bit; 
    } ServerState[2];
  Server188_ConcentratorState_Stuct   ST;
  Server188_ConcentratorErrFlg_Stuct  ERR;  
}Server188_HeartbeatPacket_Param_Stuct;

typedef struct {
  unsigned char         ErrType;
  unsigned short        ErrParam;
}Server188_Err_Param_Stuct;





typedef unsigned short   Server188_PreambleType;
//帧帧头格式
typedef struct {	
  unsigned char StartCharacter;  //帧头值为0x68H
  unsigned char LAI;             //区域识别码	1Byte	用于识别数据传输的区域
  unsigned char Concentrator[ConcentratorAddr_Length]; //集中器地址 5字节BCD码
  unsigned char Code;            //功能码
  union{
    unsigned short Word;
    struct{
      unsigned char Byte_L;
      unsigned char Byte_H;
    }Byte;
  }DataLength;                   //数据段长度
}Server188_FrameHead_Struct;

/*
typedef  union{ 
  void* Param;
  //设置集中器时间命令 主站——>集中器 命令参数定义 (控制码：0X01)
  Server188_TimeWcmd_askParam_Stuct*              TimeWcmd_ask; 
  //设置集中器时间命令 集中器——>主站 命令参数定义 (控制码：0X01)
  Server188_TimeWcmd_ackParam_Stuct*              TimeWcmd_ack;
  //读取集中器时间命令 主站——>集中器 命令参数定义 (控制码：0X02)
  Server188_TimeRcmd_askParam_Stuct*              TimeRcmd_ask;
  //读取集中器时间命令 集中器——>主站 命令参数定义 (控制码：0X02)
  Server188_TimeRcmd_ackParam_Stuct*              TimeRcmd_ack;
  //设置集中器抄表参数   主站——>集中器  命令参数定义 (控制码：0X03)
  Server188_ConfigDataWcmd_askParam_Stuct*        ConfigDataWcmd_ask;
  //设置集中器抄表参数   集中器——> 主站 命令参数定义 (控制码：0X03)
  Server188_ConfigDataWcmd_ackParam_Stuct*        ConfigDataWcmd_ack;
  //读取集中器抄表参数   主站——>集中器  命令参数定义 (控制码：0X04)
  Server188_ConfigDataRcmd_askParam_Stuct*        ConfigDataRcmd_ask; 	
  //读取集中器抄表参数   集中器——> 主站  命令参数定义 (控制码：0X04) 	
  Server188_ConfigDataRcmd_ackParam_Stuct*        ConfigDataRcmd_ack;
  //设置集中器GPRS参数   主站——>集中器  命令参数定义 (控制码：0x07) 
  Server188_GprsWcmd_askParam_Stuct*              GprsWcmd_ask;
  //设置集中器GPRS参数   集中器——> 主站 命令参数定义 (控制码：0X07)
  Server188_GprsWcmd_ackParam_Stuct*              GprsWcmd_ack;
  //读取集中器抄表参数   主站——>集中器  命令参数定义 (控制码：0X08)
  Server188_GprsRcmd_askParam_Stuct*              GprsRcmd_ask;
  //读取集中器GPRS参数   集中器——>主站  命令参数定义 (控制码：0X08)
  Server188_GprsRcmd_ackParam_Stuct*              GprsRcmd_ack;
  //下载表计地址   主站——>集中器  命令参数定义(控制码：0X09)
  Server188_MeterWcmd_askParam_Stuct*             MeterWcmd_ask;
  //下载表计地址   集中器——>主站 命令参数定义 (控制码：0X09)
  Server188_MeterWcmd_ackParam_Stuct*             MeterWcmd_ack;
  //读取下载成功表计地址  主站——>集中器  命令参数定义(控制码：0x0B)
  Server188_MeterRcmd_askParam_Stuct*             MeterRcmd_ask;
  //读取下载成功表计地址  集中器——>主站  命令参数定义(控制码：0x0B)
  Server188_MeterRcmd_ackParam_Stuct*             MeterRcmd_ack;
  //读取下载失败表计地址  主站——>集中器  命令参数定义(控制码：0x0C)
  Server188_MeterErrRcmd_askParam_Stuct*          MeterErrRcmd_ask;
  //读取下载成功表计地址  集中器——>主站  命令参数定义(控制码：0x0C)
  Server188_MeterErrRcmd_ackParam_Stuct*          MeterErrRcmd_ack;
  //下达控制命令 主站——>集中器  命令参数定义(控制码：0X0E)  
  Server188_OperateWcmd_askParam_Stuct*          OperateWcmd_ask;
  //下达控制命令  集中器——>主站  命令参数定义(控制码：0X0E)
  Server188_OperateWcmd_ackParam_Stuct*          OperateWcmd_ack;
  //取控制记录   主站——>集中器  命令参数定义(控制码：0X0F	)  
  Server188_OperateRcmd_askParam_Stuct*          OperateRcmd_ask;
  //取控制记录   集中器——>主站  命令参数定义(控制码：0X0F)
  Server188_OperateRcmd_ackParam_Stuct*          OperateRcmd_ack;
  //启动立即抄表  主站——>集中器  命令参数定义(控制码：0X14)  
  Server188_ReadingWcmd_askParam_Stuct*          ReadingWcmd_ask;
  //启动立即抄表  集中器——>主站  命令参数定义(控制码：0X14)  
  Server188_ReadingWcmd_ackParam_Stuct*          ReadingWcmd_ack;
  //读取所有表计当前数据  主站——>集中器  命令参数定义(控制码：0X15)
  Server188_AllCurrentDataRcmd_askParam_Stuct*   AllCurrentDataRcmd_ask; 
  //读取所有表计当前数据  集中器——>主站  命令参数定义(控制码：0X15)  
  Server188_AllCurrentDataRcmd_ackParam_Stuct*   AllCurrentDataRcmd_ack;
  //读取全部表计日冻结数据   主站——>集中器  命令参数定义(控制码：0X16)
  Server188_AllFreezingDataRcmd_askParam_Stuct*   AllFreezingDataRcmd_ask;  
  //读取全部表计日冻结数据   集中器——>主站  命令参数定义(控制码：0X16)  
  Server188_AllFreezingDataRcmd_ackParam_Stuct*     AllFreezingDataRcmd_ack;
  //读单表的当前数据   主站——>集中器  命令参数定义(控制码：0X17)
  Server188_SingCurrentDataRcmd_askParam_Stuct*         SingCurrentDataRcmd_ask;  
  //读单表的当前数据   集中器——>主站  命令参数定义(控制码：0X17)  
  Server188_SingCurrentDataRcmd_ackParam_Stuct*  SingCurrentDataRcmd_ack;
  //读取单表的日冻结数据   主站——>集中器  命令参数定义(控制码：0X18)
  Server188_SingFreezingDataRcmd_askParam_Stuct*       SingFreezingDataRcmd_ask;
  //读取单表的日冻结数据   集中器——>主站  命令参数定义(控制码：0X18)  
  Server188_SingFreezingDataRcmd_ackParam_Stuct*       SingFreezingDataRcmd_ack;
  //替换表计   主站——>集中器  命令参数定义(控制码：0X1C)
  Server188_AlterMeterWcmd_askParam_Stuct*    AlterMeterWcmd_ask; 
  //替换表计   集中器——>主站  命令参数定义(控制码：0X1C) 
  Server188_AlterMeterWcmd_ackParam_Stuct*    AlterMeterWcmd_ack;   
  //删除表计   主站——>集中器  命令参数定义(控制码：0X1c)
  Server188_DeleteMeterWcmd_askParam_Stuct*   DeleteMeterWcmd_ask;
  //删除表计   集中器——>主站  命令参数定义(控制码：0X1c) 
  Server188_DeleteMeterWcmd_ackParam_Stuct*   DeleteMeterWcmd_ack;
  //清除表计   主站——>集中器  命令参数定义(控制码：0X20)
  Server188_CleanMeterWcmd_askParam_Stuct     CleanMeterWcmd_ask;   
    //清除表计   集中器——>主站  命令参数定义(控制码：0X20) 
  Server188_CleanMeterWcmd_ackParam_Stuct     CleanMeterWcmd_ack; 
  unsigned char                       Data[256]; 
}Server188_FrameParam1;*/


//帧尾格式
typedef struct {
  unsigned char CS;     //校验码,从帧头开始到帧校验标志之前所有字节的累加和低8位	
  unsigned char Pause;  //结束符0x16H
}Server188_FrameStern_Struct;


//正常帧格式
typedef struct{
  Server188_FrameHead_Struct  Head;
  //Server188_FrameParam1  Param;
  union{ 
    //设置集中器时间命令 主站——>集中器 命令参数定义 (控制码：0X01)
    Server188_TimeWcmd_askParam_Stuct              TimeWcmd_ask; 
    //设置集中器时间命令 集中器——>主站 命令参数定义 (控制码：0X01)
    Server188_TimeWcmd_ackParam_Stuct              TimeWcmd_ack;
    //读取集中器时间命令 主站——>集中器 命令参数定义 (控制码：0X02)
    Server188_TimeRcmd_askParam_Stuct              TimeRcmd_ask;
    //读取集中器时间命令 集中器——>主站 命令参数定义 (控制码：0X02)
    Server188_TimeRcmd_ackParam_Stuct              TimeRcmd_ack;
    //设置集中器抄表参数   主站——>集中器  命令参数定义 (控制码：0X03)
    Server188_ConfigDataWcmd_askParam_Stuct        ConfigDataWcmd_ask;
    //设置集中器抄表参数   集中器——> 主站 命令参数定义 (控制码：0X03)
    Server188_ConfigDataWcmd_ackParam_Stuct        ConfigDataWcmd_ack;
    //读取集中器抄表参数   主站——>集中器  命令参数定义 (控制码：0X04)
    Server188_ConfigDataRcmd_askParam_Stuct        ConfigDataRcmd_ask; 	
    //读取集中器抄表参数   集中器——> 主站  命令参数定义 (控制码：0X04) 	
    Server188_ConfigDataRcmd_ackParam_Stuct        ConfigDataRcmd_ack;
    //设置集中器GPRS参数   主站——>集中器  命令参数定义 (控制码：0x07) 
    Server188_GprsWcmd_askParam_Stuct              GprsWcmd_ask;
    //设置集中器GPRS参数   集中器——> 主站 命令参数定义 (控制码：0X07)
    Server188_GprsWcmd_ackParam_Stuct              GprsWcmd_ack;
    //读取集中器抄表参数   主站——>集中器  命令参数定义 (控制码：0X08)
    Server188_GprsRcmd_askParam_Stuct              GprsRcmd_ask;
    //读取集中器GPRS参数   集中器——>主站  命令参数定义 (控制码：0X08)
    Server188_GprsRcmd_ackParam_Stuct              GprsRcmd_ack;
    //下载表计地址   主站——>集中器  命令参数定义(控制码：0X09)
    Server188_MeterWcmd_askParam_Stuct             MeterWcmd_ask;
    //下载表计地址   集中器——>主站 命令参数定义 (控制码：0X09)
    Server188_MeterWcmd_ackParam_Stuct             MeterWcmd_ack;
    //读取下载成功表计地址  主站——>集中器  命令参数定义(控制码：0x0B)
    Server188_MeterRcmd_askParam_Stuct             MeterRcmd_ask;
    //读取下载成功表计地址  集中器——>主站  命令参数定义(控制码：0x0B)
    Server188_MeterRcmd_ackParam_Stuct             MeterRcmd_ack;
    //读取下载失败表计地址  主站——>集中器  命令参数定义(控制码：0x0C)
    Server188_MeterErrRcmd_askParam_Stuct          MeterErrRcmd_ask;
    //读取下载成功表计地址  集中器——>主站  命令参数定义(控制码：0x0C)
    Server188_MeterErrRcmd_ackParam_Stuct          MeterErrRcmd_ack;
    //下达控制命令 主站——>集中器  命令参数定义(控制码：0X0E)  
    Server188_OperateWcmd_askParam_Stuct          OperateWcmd_ask;
    //下达控制命令  集中器——>主站  命令参数定义(控制码：0X0E)
    Server188_OperateWcmd_ackParam_Stuct          OperateWcmd_ack;
    //取控制记录   主站——>集中器  命令参数定义(控制码：0X0F	)  
    Server188_OperateRcmd_askParam_Stuct          OperateRcmd_ask;
    //取控制记录   集中器——>主站  命令参数定义(控制码：0X0F)
    Server188_OperateRcmd_ackParam_Stuct          OperateRcmd_ack;
    //启动立即抄表  主站——>集中器  命令参数定义(控制码：0X14)  
    Server188_ReadingWcmd_askParam_Stuct          ReadingWcmd_ask;
    //启动立即抄表  集中器——>主站  命令参数定义(控制码：0X14)  
    Server188_ReadingWcmd_ackParam_Stuct          ReadingWcmd_ack;
    //读取所有表计当前数据  主站——>集中器  命令参数定义(控制码：0X15)
    Server188_AllCurrentDataRcmd_askParam_Stuct   AllCurrentDataRcmd_ask; 
    //读取所有表计当前数据  集中器——>主站  命令参数定义(控制码：0X15)  
    Server188_AllCurrentDataRcmd_ackParam_Stuct   AllCurrentDataRcmd_ack;
    //读取全部表计日冻结数据   主站——>集中器  命令参数定义(控制码：0X16)
    Server188_AllFreezingDataRcmd_askParam_Stuct   AllFreezingDataRcmd_ask;  
    //读取全部表计日冻结数据   集中器——>主站  命令参数定义(控制码：0X16)  
    Server188_AllFreezingDataRcmd_ackParam_Stuct     AllFreezingDataRcmd_ack;
    //读单表的当前数据   主站——>集中器  命令参数定义(控制码：0X17)
    Server188_SingCurrentDataRcmd_askParam_Stuct         SingCurrentDataRcmd_ask;  
    //读单表的当前数据   集中器——>主站  命令参数定义(控制码：0X17)  
    Server188_SingCurrentDataRcmd_ackParam_Stuct  SingCurrentDataRcmd_CJT188_ack;
    //读取单表的日冻结数据   主站——>集中器  命令参数定义(控制码：0X18)
    Server188_SingFreezingDataRcmd_askParam_Stuct       SingFreezingDataRcmd_ask;
    //读取单表的日冻结数据   集中器——>主站  命令参数定义(控制码：0X18)  
    Server188_SingFreezingDataRcmd_ackParam_Stuct       SingFreezingDataRcmd_ack;
    //替换表计   主站——>集中器  命令参数定义(控制码：0X1C)
    Server188_AlterMeterWcmd_askParam_Stuct    AlterMeterWcmd_ask; 
    //替换表计   集中器——>主站  命令参数定义(控制码：0X1C) 
    Server188_AlterMeterWcmd_ackParam_Stuct    AlterMeterWcmd_ack;   
    //删除表计   主站——>集中器  命令参数定义(控制码：0X1c)
    Server188_DeleteMeterWcmd_askParam_Stuct   DeleteMeterWcmd_ask;
    //删除表计   集中器——>主站  命令参数定义(控制码：0X1c) 
    Server188_DeleteMeterWcmd_ackParam_Stuct   DeleteMeterWcmd_ack;  
    //清除表计   主站——>集中器  命令参数定义(控制码：0X20)
    Server188_CleanMeterWcmd_askParam_Stuct     CleanMeterWcmd_ask;   
    //清除表计   集中器——>主站  命令参数定义(控制码：0X20) 
    Server188_CleanMeterWcmd_ackParam_Stuct     CleanMeterWcmd_ack; 
    
    Server188_HeartbeatPacket_Param_Stuct      HeartBeat;
    Server188_Err_Param_Stuct                  Err_Param;
    unsigned char                       Data[256]; 
  }Param;
}Server188_FrameDatas_Struct;

//正常帧格式
typedef struct{
  Server188_PreambleType          Preamble; //前导字节  至少2个字节FEH（不计入校验）。
  Server188_FrameDatas_Struct     Datas;
}Server188_Frame_Struct;

#pragma pack()

typedef struct {
  union{
    unsigned char Byte;
    struct{
      unsigned char idle :1; //缓冲区空闲
    }Bit;
  }CSR; //控制状态寄存器
  CommPortNumber_Type      SourcePort;
  Server188_Frame_Struct   Frame; 
}Server188_OutPutBuff_Struct;

//集中器参数  数据格式
typedef struct {
  unsigned char AreaCode;   //区域码
  char ID[ConcentratorAddr_Length]; //集中器地址 5字节BCD码
  Server188_TimerData_Stuct  timer;
  Server188_Gprs_Stuct       gprs; 
}Server188_ConfigData_Stuct;

typedef  union{ 
  void* Param;
  //设置集中器时间命令 主站——>集中器 命令参数定义 (控制码：0X01)
  Server188_TimeWcmd_askParam_Stuct*              TimeWcmd_ask; 
  //设置集中器时间命令 集中器——>主站 命令参数定义 (控制码：0X01)
  Server188_TimeWcmd_ackParam_Stuct*              TimeWcmd_ack;
  //读取集中器时间命令 主站——>集中器 命令参数定义 (控制码：0X02)
  Server188_TimeRcmd_askParam_Stuct*              TimeRcmd_ask;
  //读取集中器时间命令 集中器——>主站 命令参数定义 (控制码：0X02)
  Server188_TimeRcmd_ackParam_Stuct*              TimeRcmd_ack;
  //设置集中器抄表参数   主站——>集中器  命令参数定义 (控制码：0X03)
  Server188_ConfigDataWcmd_askParam_Stuct*        ConfigDataWcmd_ask;
  //设置集中器抄表参数   集中器——> 主站 命令参数定义 (控制码：0X03)
  Server188_ConfigDataWcmd_ackParam_Stuct*        ConfigDataWcmd_ack;
  //读取集中器抄表参数   主站——>集中器  命令参数定义 (控制码：0X04)
  Server188_ConfigDataRcmd_askParam_Stuct*        ConfigDataRcmd_ask; 	
  //读取集中器抄表参数   集中器——> 主站  命令参数定义 (控制码：0X04) 	
  Server188_ConfigDataRcmd_ackParam_Stuct*        ConfigDataRcmd_ack;
  //设置集中器GPRS参数   主站——>集中器  命令参数定义 (控制码：0x07) 
  Server188_GprsWcmd_askParam_Stuct*              GprsWcmd_ask;
  //设置集中器GPRS参数   集中器——> 主站 命令参数定义 (控制码：0X07)
  Server188_GprsWcmd_ackParam_Stuct*              GprsWcmd_ack;
  //读取集中器抄表参数   主站——>集中器  命令参数定义 (控制码：0X08)
  Server188_GprsRcmd_askParam_Stuct*              GprsRcmd_ask;
  //读取集中器GPRS参数   集中器——>主站  命令参数定义 (控制码：0X08)
  Server188_GprsRcmd_ackParam_Stuct*              GprsRcmd_ack;
  //下载表计地址   主站——>集中器  命令参数定义(控制码：0X09)
  Server188_MeterWcmd_askParam_Stuct*             MeterWcmd_ask;
  //下载表计地址   集中器——>主站 命令参数定义 (控制码：0X09)
  Server188_MeterWcmd_ackParam_Stuct*             MeterWcmd_ack;
  //读取下载成功表计地址  主站——>集中器  命令参数定义(控制码：0x0B)
  Server188_MeterRcmd_askParam_Stuct*             MeterRcmd_ask;
  //读取下载成功表计地址  集中器——>主站  命令参数定义(控制码：0x0B)
  Server188_MeterRcmd_ackParam_Stuct*             MeterRcmd_ack;
  //读取下载失败表计地址  主站——>集中器  命令参数定义(控制码：0x0C)
  Server188_MeterErrRcmd_askParam_Stuct*          MeterErrRcmd_ask;
  //读取下载成功表计地址  集中器——>主站  命令参数定义(控制码：0x0C)
  Server188_MeterErrRcmd_ackParam_Stuct*          MeterErrRcmd_ack;
  //下达控制命令 主站——>集中器  命令参数定义(控制码：0X0E)  
  Server188_OperateWcmd_askParam_Stuct*          OperateWcmd_ask;
  //下达控制命令  集中器——>主站  命令参数定义(控制码：0X0E)
  Server188_OperateWcmd_ackParam_Stuct*          OperateWcmd_ack;
  //取控制记录   主站——>集中器  命令参数定义(控制码：0X0F	)  
  Server188_OperateRcmd_askParam_Stuct*          OperateRcmd_ask;
  //取控制记录   集中器——>主站  命令参数定义(控制码：0X0F)
  Server188_OperateRcmd_ackParam_Stuct*          OperateRcmd_ack;
  //启动立即抄表  主站——>集中器  命令参数定义(控制码：0X14)  
  Server188_ReadingWcmd_askParam_Stuct*          ReadingWcmd_ask;
  //启动立即抄表  集中器——>主站  命令参数定义(控制码：0X14)  
  Server188_ReadingWcmd_ackParam_Stuct*          ReadingWcmd_ack;
  //读取所有表计当前数据  主站——>集中器  命令参数定义(控制码：0X15)
  Server188_AllCurrentDataRcmd_askParam_Stuct*   AllCurrentDataRcmd_ask; 
  //读取所有表计当前数据  集中器——>主站  命令参数定义(控制码：0X15)  
  Server188_AllCurrentDataRcmd_ackParam_Stuct*   AllCurrentDataRcmd_ack;
  //读取全部表计日冻结数据   主站——>集中器  命令参数定义(控制码：0X16)
  Server188_AllFreezingDataRcmd_askParam_Stuct*   AllFreezingDataRcmd_ask;  
  //读取全部表计日冻结数据   集中器——>主站  命令参数定义(控制码：0X16)  
  Server188_AllFreezingDataRcmd_ackParam_Stuct*     AllFreezingDataRcmd_ack;
  //读单表的当前数据   主站——>集中器  命令参数定义(控制码：0X17)
  Server188_SingCurrentDataRcmd_askParam_Stuct*         SingCurrentDataRcmd_ask;  
  //读单表的当前数据   集中器——>主站  命令参数定义(控制码：0X17)  
  Server188_SingCurrentDataRcmd_ackParam_Stuct*  SingCurrentDataRcmd_ack;
  //读取单表的日冻结数据   主站——>集中器  命令参数定义(控制码：0X18)
  Server188_SingFreezingDataRcmd_askParam_Stuct*       SingFreezingDataRcmd_ask;
  //读取单表的日冻结数据   集中器——>主站  命令参数定义(控制码：0X18)  
  Server188_SingFreezingDataRcmd_ackParam_Stuct*       SingFreezingDataRcmd_ack;
  //替换表计   主站——>集中器  命令参数定义(控制码：0X1C)
  Server188_AlterMeterWcmd_askParam_Stuct*    AlterMeterWcmd_ask; 
  //替换表计   集中器——>主站  命令参数定义(控制码：0X1C) 
  Server188_AlterMeterWcmd_ackParam_Stuct*    AlterMeterWcmd_ack;   
  //删除表计   主站——>集中器  命令参数定义(控制码：0X1c)
  Server188_DeleteMeterWcmd_askParam_Stuct*   DeleteMeterWcmd_ask;
  //删除表计   集中器——>主站  命令参数定义(控制码：0X1c) 
  Server188_DeleteMeterWcmd_ackParam_Stuct*   DeleteMeterWcmd_ack;
  //清除表计   主站——>集中器  命令参数定义(控制码：0X20)
  Server188_CleanMeterWcmd_askParam_Stuct*     CleanMeterWcmd_ask;   
    //清除表计   集中器——>主站  命令参数定义(控制码：0X20) 
  Server188_CleanMeterWcmd_ackParam_Stuct*     CleanMeterWcmd_ack; 
}Server188_FrameParam;

typedef struct {
  CommPortNumber_Type   port;
  Server188_FrameParam  pdata;
}Server188_InputData_Stuct;
typedef struct {
  unsigned short  length;
  Server188_FrameParam  pdata;
}Server188_OututData_Stuct;

/*********************************************************************************************************
外部变量声明区
*********************************************************************************************************/
extern Server188_ConfigData_Stuct      _188_ConcenConfigData;
extern Server188_ConcenMbusState_Type  ReadingMeterState;  
/*********************************************************************************************************
函数声明区
*********************************************************************************************************/
void Server188_SPCP_init(void);

Server188_OutPutBuff_Struct* getServer188SendBuff(CommPortNumber_Type port);

void freeServer188SendBuff(void* frame );

SystemErrName Server188_frameVerify (Server188_FrameDatas_Struct *frame, unsigned short length);

Server188_Frame_Struct* CreateHandshakeFrame(unsigned char* Length); 

SystemErrName Server188Hand_AckVerify(unsigned char* AckData, unsigned short DataLength);

SystemErrName Server188_InPut (void *pbuff, unsigned short length,CommPortNumber_Type port);

SystemErrName Server188_Send(CommPortNumber_Type Port, Server188_Frame_Struct* frame);

SystemErrName Server188_SendErr (CommPortNumber_Type port, Server188_Err_Param_Stuct* Err );

void  ReadTask( void );
void  PatrolTask( void );

void GuideReportAllCurrentData(void);
/********************************************************************************************************/
#endif
/******************************************END********************************************************/