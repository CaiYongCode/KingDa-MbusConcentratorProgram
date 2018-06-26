/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef __IIC_FRAM24CLXX_H__
#define __IIC_FRAM24CLXX_H__


/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\..\Projects\inc\SystemDefine.h"
/*********************************************************************************
宏定义区
*********************************************************************************/
#define FRAM_I2C_Speed         10000
#define FRAM_ADDRESS           0xA0
#define FRAM_PageSize          256

/*********************************************************************************
数据类型定义
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
函数声明区
*********************************************************************************/
SystemErrName FRAM_BufferRead(void* Buffer,
                              unsigned short NumByteToRead,
                              unsigned short ReadAddr);//将EEPROM的数据读入缓冲器

SystemErrName FRAM_PageWrite(unsigned char* pBuffer, 
                             unsigned short NumByteToWrite,
                             unsigned short WriteAddr);//写少于一页的数据

SystemErrName FRAM_ByteWrite(unsigned char data, 
                             unsigned short WriteAddr);//写一个字节到EEPROM

SystemErrName FRAM_BufferWrite(void* Buffer,
                               short NumByteToWrite,
                               unsigned short WriteAddr);//将缓冲器的数据写入EEPROM

SystemErrName FRAM_MultipleRead(void* Buffer,
                                unsigned short NumByteToRead,
                                unsigned short ReadAddr);

SystemErrName FRAM_MultipleWrite(void* Buffer, 
                                 unsigned short NumByteToWrite,
                                 unsigned short WriteAddr);
/********************************************************************************/
#endif