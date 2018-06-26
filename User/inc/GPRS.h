/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
#ifndef __GPRS_SIM800C_H__
#define __GPRS_SIM800C_H__
/*******************************************************************************
�ļ�������
*******************************************************************************/
#include ".\Includes.h"
/*******************************************************************************
�궨����
*******************************************************************************/
#define  GPRS_COMM               USART3_Port
#define  GPRS_GprsBuffLength     4096
#define  GprsRecveBuffLength     GPRS_GprsBuffLength
#define  GprsSendBuffLength      GPRS_GprsBuffLength

#define  weitGPRS_COMM( )       weitPortIdle (GPRS_COMM, 200);
/*******************************************************************************
�������Ͷ���
*******************************************************************************/
typedef enum{
  RecErr  = 0x00,
  RecOK   = 0x01,
  ExeOK   = 0x02,
  ExeErr  = 0x03,
}Cmd_ACK_type;

typedef struct{
  enum{
    CONNECT_FAIL = 0x00,
    CONNECT_OK   = 0x01,
    ALREADY_CONNECT = 0x02,
    CME_ERROR = 0x03,
  }state;
  unsigned short ERROR_CODE;  
}CIPSTART_ACK_type;

typedef struct
{
  union{
    unsigned char mode  :2;  //����ģʽ  00:UDP  01 : TCP CLient  10:TCP Server
    unsigned char state :1;  //�����־ 1:������ 0:����Ͽ�
  }CSR; 
 unsigned long ip;
 unsigned short prot;
}GPRS_IP_struct;

typedef struct{
   union{
    unsigned char Byte;
    struct{
    unsigned char power :1;   //��Դ��־ 
    unsigned char ready :1;  //�����������
   }Bit;
  }CSR; 
 unsigned long ip;
 unsigned short prot; 
 unsigned char  Rssi;
 unsigned char  Ber;
 unsigned long  localip;
 GPRS_IP_struct  cip[6]; //
 char   ID[16];
}GPRS_MODE_struct;

typedef struct{
unsigned char   ch;
unsigned short  length;
void*            pbuff; 
}GPRS_IP_Package_struct;
/*******************************************************************************
�ⲿ����������
*******************************************************************************/
extern GPRS_MODE_struct gprs;
/*******************************************************************************
����������
*******************************************************************************/
// ���SIM��״̬ 
Cmd_ACK_type  _CPIN_func(void);
 
// ��������ź�ǿ��
Cmd_ACK_type  _CSQ_func(void);

//�������ע��״̬
Cmd_ACK_type  _CREG_func(unsigned char *mode, unsigned char* stat);

//���ö���·ģʽ 
Cmd_ACK_type  _CIPMUX_func(BaseType_t MUX);

// ����͸��ģʽ
Cmd_ACK_type  _CIPMODE_func(BaseType_t mode);

// ���GPRS����״̬ 
Cmd_ACK_type  _CGATT_func(unsigned char* state);

// ����APN
Cmd_ACK_type  _CSTT_func( char *apn);

//����������·��GPRS����CSD��
Cmd_ACK_type  _CIICR_func(void);

// ��ȡ���� IP ��ַ
Cmd_ACK_type  _CIFSR_func(unsigned long* IP);

// ����TCP����
Cmd_ACK_type _CIPSTART_func(unsigned char ch, unsigned long ip, unsigned short port, CIPSTART_ACK_type* ack);

//�������ݵ�Զ�˷����� 
Cmd_ACK_type  _CIPSEND_func(unsigned char ch, unsigned char* pbuff, unsigned short length);
// �ر�Զ������ 
Cmd_ACK_type  _CIPCLOSE_func(unsigned char ch);

//ͬ��ͨ�Ŵ��ڱ�����
Cmd_ACK_type  _SyncRate_func(void);


void GprsTask_func ( void* Param );

SystemErrName GprsIP_SendFunc(unsigned char CH, void* Data,unsigned short DataLength);
unsigned char connectServer (unsigned char ServerNO);
void closeServer (unsigned char ServerNO);
/******************************************************************************/
#endif
/****************************** END *******************************************/