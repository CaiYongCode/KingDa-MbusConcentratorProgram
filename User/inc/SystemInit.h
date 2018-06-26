/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
#ifndef __SYSTEMINIT_H__
#define __SYSTEMINIT_H__
/*******************************************************************************
�ļ�������
*******************************************************************************/
#include ".\Includes.h"
/*******************************************************************************
�궨����
*******************************************************************************/

/*******************************************************************************
�������Ͷ���
*******************************************************************************/
/*******************************************************************************
�ⲿ����������
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
extern SemaphoreHandle_t  Fram_xMutex ;       //�����ź��� ����
extern SemaphoreHandle_t  Mbus_xMutex ;       //�����ź��� ���� 
//extern SemaphoreHandle_t  Eeprom_xMutex ;     //�����ź��� EEPROM

extern unsigned short   ServerTask_MonitorTimer[2]; //������  ������ӿ��Ź���ʱ��
extern unsigned short   Server2Task_MonitorTimer;   //������ 2 ������ӿ��Ź���ʱ��
extern unsigned short   GprsTask_MonitorTimer;      //Gprsͨ�� ������ӿ��Ź���ʱ��
extern unsigned short   ManualTask_MonitorTimer;    //�����ֱ� ������ӿ��Ź���ʱ��
/*******************************************************************************
����������
*******************************************************************************/
int Systerm_INIT(void);
void Launch_Systerm ( void*  Param);

void weitPortIdle (CommPortNumber_Type port, unsigned short  _T10ms); //�ȴ�������� 

/******************************************************************************/
#endif
/****************************** END *******************************************/
