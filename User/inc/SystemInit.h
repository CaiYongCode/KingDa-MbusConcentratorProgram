/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef __SYSTEMINIT_H__
#define __SYSTEMINIT_H__
/*******************************************************************************
文件包含区
*******************************************************************************/
#include ".\Includes.h"
/*******************************************************************************
宏定义区
*******************************************************************************/

/*******************************************************************************
数据类型定义
*******************************************************************************/
/*******************************************************************************
外部变量声明区
*******************************************************************************/
extern TaskHandle_t    ManualTask;
extern TaskHandle_t    ServerTask; 
extern TaskHandle_t    SacnTask; 

extern QueueHandle_t   ManualQueue;
extern QueueHandle_t   GprsQueue ;
//extern QueueHandle_t   MbusSendQueue;
extern QueueHandle_t   MbusReceiveQueue;
extern QueueHandle_t   ServerQueue[];

extern SemaphoreHandle_t  Gprs_xMutex;
extern SemaphoreHandle_t  Fram_xMutex ;       //互斥信号量 铁电
extern SemaphoreHandle_t  Mbus_xMutex ;       //互斥信号量 铁电 
//extern SemaphoreHandle_t  Eeprom_xMutex ;     //互斥信号量 EEPROM

extern unsigned short   ServerTask_MonitorTimer[2]; //服务器  任务监视看门狗定时器
extern unsigned short   Server2Task_MonitorTimer;   //服务器 2 任务监视看门狗定时器
extern unsigned short   GprsTask_MonitorTimer;      //Gprs通信 任务监视看门狗定时器
extern unsigned short   ManualTask_MonitorTimer;    //调试手柄 任务监视看门狗定时器
/*******************************************************************************
函数声明区
*******************************************************************************/
int Systerm_INIT(void);
void Launch_Systerm ( void*  Param);

void weitPortIdle (CommPortNumber_Type port, unsigned short  _T10ms); //等待发送完毕 

/******************************************************************************/
#endif
/****************************** END *******************************************/
