/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef __INCLUDES_H__
#define __INCLUDES_H__
/*******************************************************************************
文件包含区
*******************************************************************************/

/*********************************************************
********************* 标准库 *****************************
**********************************************************/
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
//#include <syslib.h>

/*********************************************************
********************* 其它库 *****************************
**********************************************************/

/********************************************************* 
*********************** OS *******************************
*********************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "timers.h"
#include "semphr.h"
/********************************************************* 
********************** SoftLiby **************************
*********************************************************/
#include "..\..\LIB\Common\inc\SofTimer.h"

/********************************************************* 
*********************** BSP ******************************
*********************************************************/
#include "..\..\BSP\inc\bsp.h"

/********************************************************* 
********************* UserCode ***************************
*********************************************************/
#include ".\SystemInit.h"
#include ".\CRC8.h"
//#include ".\KingDaV1.h"
//#include ".\KingDaServer.h"
#include ".\Manual.h"
//#include ".\MCPX.h"
#include ".\GPRS.h"
#include ".\OnSceneComm.h"
/*******************************************************************************
宏定义区
*******************************************************************************/

#define   ConcentratorParameter_AddrEeprom   ConCofigDataAtEEROM
#define  AutomatchPort              0

#define   _10ms    (1) 
#define   _100ms   ((_10ms) * 10)
#define   _1s      ((_10ms) * 100)
#define   _10s     ((_10ms) * 1000)
#define   _1Minute ((_1s) * 60)


#define  EEROM_Address                  0x08080000

#define  VersionInfoAtEEROM             EEROM_Address 

#define  ConCofigDataAtEEROM            VersionInfoAtEEROM + 32

#define  Server188_ConfigData_AddrEeprom    VersionInfoAtEEROM + 128


#define  MbusChannelNumber              4       //通道数量
#define  SingleChannel_MeterNumber      100     //单一通道 可容纳的表计总数 
#define  TotalNumber_MeterConfig        (MbusChannelNumber * SingleChannel_MeterNumber)   //可容纳的表计总数 

#define  MeterListDecAtFRAM             16
#define  MeterListAtFRAM                32                                     //表计配置信息在 存储器中的首地址
#define  MeterListEndAtFRAM            (MeterListAtFRAM +((TotalNumber_MeterConfig) * sizeof (Server188_MeterRecord_Struct)))//表计配置信息在 存储器中的结束地址


#define  MeterValueAtFRAM              (MeterListEndAtFRAM + sizeof (Server188_MeterRecord_Struct))//表计数据信息在 存储器中的首地址

/*******************************************************************************
数据类型定义
*******************************************************************************/

/*******************************************************************************
外部变量声明区
*******************************************************************************/

/*******************************************************************************
函数声明区
*******************************************************************************/


/******************************************************************************/
#endif
/****************************** END *******************************************/
