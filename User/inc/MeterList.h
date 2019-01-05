/*********************************************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************************************/
#ifndef  __METERLIST_H
#define  __METERLIST_H
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include ".\CJT-188-2004.h"
#include ".\Server188.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/

/*********************************************************************************************************
�������Ͷ���
*********************************************************************************************************/
#pragma pack(1)
//������ü�����  ������
typedef struct {
  unsigned char              Flow_PV[4];              //��ǰ�ۻ�����
  unsigned char              Flow_Accounts[4];        //�������ۻ�����
  CJT188_ST_Stuct             ST;
  Server188_Time_Stuct        Time;
  signed char                Temp;                      //�¶�
}Server188_MeterData_Struct;

typedef struct{
  unsigned char               OPID;	    //����ԱID(HEX)��
  Server188_Time_Stuct	       Time;
}Server188_OperateRecord_Stuct;

//������ü����� �洢��Ԫ��ʽ 
typedef union{
    unsigned char Byte;
    struct{
      unsigned char Config_E :1;   //������Ϣ��Ч
      unsigned char Data_E:1;      //������Ϣ��Ч
      unsigned char Open_E :1;     //������Ϣ��Ч
      unsigned char Close_E :1;    //������Ϣ��Ч
    }Bit;
  }Server188_RecordData_CSR; //����״̬�Ĵ���

typedef union{
    unsigned char Byte;
    struct{
      unsigned char Config_E :1;   //������Ϣ��Ч
      unsigned char PV_E:1;      //������Ϣ��Ч
      unsigned char Acc_E :1;
      unsigned char Open_E :1;     //������Ϣ��Ч
      unsigned char Close_E :1;    //������Ϣ��Ч
      unsigned char Lose :1;    //������Ϣ��Ч
    }Bit; 
  }Server188_ValueRecordData_CSR; //����״̬�Ĵ���
  
  
typedef struct {
  Server188_RecordData_CSR              CSR;           //����״̬�Ĵ���
  Server188_Meter_Stuct                 Config;        //���������Ϣ
}Server188_MeterRecordData_Struct;

typedef struct {
  Server188_ValueRecordData_CSR         CSR;           //����״̬�Ĵ���
  Server188_MeterData_Struct            Value;          //�������
  Server188_OperateRecord_Stuct         OpenRecord;    //������¼
  Server188_OperateRecord_Stuct         CloseRecord;   //�ط���¼
}Server188_MeterValueRecordData_Struct;


typedef struct {
 Server188_MeterRecordData_Struct  data;
 unsigned short     CRC16;         //������������֤
}Server188_MeterRecord_Struct;

typedef struct {
 Server188_MeterValueRecordData_Struct  data;
 unsigned short     CRC16;         //������������֤
}Server188_MeterValueRecord_Struct;


//������ü����� �б�������
  
typedef struct {
  unsigned short MeterNumber[4];        //��ͨ�����  ���� 
  unsigned short OpenNumber;            //������¼  ���� 
  unsigned short CloseNumber;           //�ط���¼  ����
}Server188_MeterListSpecData_Struct;
typedef struct {
  Server188_MeterListSpecData_Struct data;
  unsigned short CRC16;
}Server188_MeterListSpec_Struct;

#pragma pack()
/*********************************************************************************************************
�ⲿ����������
*********************************************************************************************************/

/*********************************************************************************************************
����������
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