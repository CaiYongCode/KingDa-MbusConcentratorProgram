/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
#ifndef __IIC_FRAM24CLXX_H__
#define __IIC_FRAM24CLXX_H__


/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\..\Projects\inc\SystemDefine.h"
/*********************************************************************************
�궨����
*********************************************************************************/
#define FRAM_I2C_Speed         10000
#define FRAM_ADDRESS           0xA0
#define FRAM_PageSize          256

/*********************************************************************************
�������Ͷ���
*********************************************************************************/

/*********************************************************************************
�ⲿ����������
*********************************************************************************/

/*********************************************************************************
����������
*********************************************************************************/
SystemErrName FRAM_BufferRead(void* Buffer,
                              unsigned short NumByteToRead,
                              unsigned short ReadAddr);//��EEPROM�����ݶ��뻺����

SystemErrName FRAM_PageWrite(unsigned char* pBuffer, 
                             unsigned short NumByteToWrite,
                             unsigned short WriteAddr);//д����һҳ������

SystemErrName FRAM_ByteWrite(unsigned char data, 
                             unsigned short WriteAddr);//дһ���ֽڵ�EEPROM

SystemErrName FRAM_BufferWrite(void* Buffer,
                               short NumByteToWrite,
                               unsigned short WriteAddr);//��������������д��EEPROM

SystemErrName FRAM_MultipleRead(void* Buffer,
                                unsigned short NumByteToRead,
                                unsigned short ReadAddr);

SystemErrName FRAM_MultipleWrite(void* Buffer, 
                                 unsigned short NumByteToWrite,
                                 unsigned short WriteAddr);
/********************************************************************************/
#endif