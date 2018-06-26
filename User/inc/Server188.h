/*********************************************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************************************/
#ifndef  __Server188_H
#define  __Server188_H
/*********************************************************************************************************
�ļ�������
*********************************************************************************************************/
#include ".\CJT-188-2004.h"
/*********************************************************************************************************
�궨����
*********************************************************************************************************/

#define  Server188_OutPutBuff_Number    3               //�������������

#define  Server188_AreaCode             0xEE            //������ ������

#define  Server188_Preamble             0xFEFE            //ͬ����
#define  Server188_StartCharacter       0x68            //֡ͷ
#define  Server188_Pause                0x16            //֡β

#define  ConcentratorAddr_Length        sizeof(CJT188_AddresStruct)               //��������ַ���� 
#define	 LSSB			        0x10            //��ˮˮ��

/*********************************************************************************************************
�������Ͷ���
*********************************************************************************************************/

//��Ƶ�ַ��ʽ
typedef   CJT188_AddresStruct  Server188_MeterAddr_Stuct;

typedef enum {
  DownIdle         = 0,    // 0:  ����ͨ������; 
  FullReading      = 1,    // 1�����ڳ���;
  FullPatroling    = 2,    // 2: ����Ѳ����ַ;
  ReadingFinish    = 3,
}Server188_ConcenMbusState_Type;


//��վ�뼯����֮�� �����붨��
typedef enum {
  Server188_Time_Wcmd		        = 0x01,	//1���ü�����ʱ��														
  Server188_Time_Rcmd		        = 0x02,	//2��ȡ������ʱ��															
  Server188_ConfigData_Wcmd		= 0x03,	//3���ü������������													
  Server188_ConfigData_Rcmd             = 0x04,	//4��ȡ�������������													
  Server188_Gprs_Wcmd		        = 0x07,	//5���ü�����GPRS����													
  Server188_Gprs_Rcmd		        = 0x08,	//6��ȡ������GPRS����													
  Server188_Meter_Wcmd		        = 0x09,	//7���ر�Ƶ�ַ															
  Server188_Meter_Rcmd		        = 0x0B,	//8��ȡ���سɹ���Ƶ�ַ													
  Server188_MeterErr_Rcmd		= 0x0C,	//9��ȡ����ʧ�ܱ�Ƶ�ַ													
  Server188_Operate_Wcmd		= 0x0E,	//10�´��������															
  Server188_Operate_Rcmd		= 0x0F,	//11��ȡ��������															
  Server188_Reading_Wcmd		= 0x14,	//12������������															
  Server188_AllCurrentData_Rcmd	        = 0x15,	//13��ȡĳһ�ɼ��������б�Ƶ�ǰ����									
  Server188_AllFreezingData_Rcmd	= 0x16,	//14��ȡĳһ�ɼ��������б���ն�������									
  Server188_SingCurrentData_Rcmd	= 0x17,	//15��ȡ����ĵ�ǰ����													
  Server188_SingFreezingData_Rcmd	= 0x18,	//16��ȡ������ն�������													
  Server188_AlterMeter_Wcmd	        = 0x1C,	//17�滻���																
  Server188_DeleteMeter_Wcmd	        = 0x1D,	//18ɾ�����																
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
  OK           = 0x00,        //���� 
  TimeErr      = 0x10,        //ʱ�䲻�Ϸ�, 
  EepromErr    = 0x11,        //����д��eeprom����ȷ , 
  NoDats       = 0x20,        //�޴˰� , 
  NoMeter      = 0x21,        //δ��ѯ����� , 
  ParamErr     = 0x22,        //�������Ϸ� ,
  MbusErr      = 0x23,        //MBUSͨ���쳣 ,
  MeterNoAck   = 0x24,        //�������Ӧ ,
  NoAck        = 0x25,        //��Ӧ�� ,
  MbusBusy     = 0x26,        //����æ ,
  MbusTimeOut  = 0x27,        //���в�����ʱ , 
  NoPackage    = 0xCA,        //�޴˰� 
  Reading      = 0x01,        //���ڳ��� , 
  Patroling    = 0x02,        //����Ѳ����ַ , 
  NoDay        = 0x30,        //δ�ҵ������� , 
  MeterRrepeat = 0x04,        //��ַ�ظ� ,
}Server188_ReplyFlag;

#pragma pack(1)
//������ʱ��  ���ݸ�ʽ
typedef struct {
  unsigned char        Year;   	//��ֻȡ����λ (BCD)
  unsigned char	Month;  	//�� (BCD)
  unsigned char	Day;    	//�� (BCD)
  unsigned char	Hour;		//ʱ (BCD)
  unsigned char        Minute;		//�� (BCD)
  unsigned char        Second;		//�� (BCD)
}Server188_Time_Stuct;

//��Լ���ͼ�ͨ���� ���ݸ�ʽ
typedef  union{
  unsigned char Byte; 
  struct{
    unsigned char Protocol: 4;    /* ��Լ����   0000:	����188 */
    unsigned char Channel : 3;    /*ͨ����   0000:	δָ��ͨ��
    0001:	M-BUSͨ��1
    0010:	M-BUSͨ��2
    0011:	M-BUSͨ��3
    0100:	M-BUSͨ��4
    0101:	RS485ͨ��
    0110:	����ͨ�� */
    unsigned char Poll_EN : 1;   //�Ƿ�Ѳ��   0:��Ѳ��  1:Ѳ��
  }Bit;
}Server188_LinkDeclarator;    

//ͨѶ���� ���ݸ�ʽ	
typedef union{
  unsigned char Byte; 
  struct{
    unsigned char BaudRate: 3; /*������ 000: 1200bps
    001: 2400bps
    010: 4800bps
    011: 9600bps
    100: 19200bps
    101: 38400bps  ��ʱδ��
    110: 57600bps  ��ʱδ��	
    111: 115200bps*/
    unsigned char DataNumber : 2;   //����λ  00: 6λ����λ  01: 7λ����λ 10: 8λ����λ
    unsigned char StopBits : 1;   //ֹͣλ  0 :1λֹͣλ   1 :2λֹͣλ
    unsigned char Parity : 2;   //У��λ  00:��У��     01:żУ��       10:��У��
  }Bit;
}Server188_CommDeclarator; 

//�������������  ���ݸ�ʽ
typedef struct {
  union{
    unsigned short Word; 
    struct{
      unsigned char Byte_L;   //���ֽ�
      unsigned char Byte_H;   //���ֽ�
    }Byte;
  }ReadingCycle;	
  unsigned char	DayFrozen;	//�ն���ʱ�� 0��23��������ͨ����Ч  HEX
  unsigned char	MonthFrozenH;	//�¶���ʱ�䣬1-31  HEX 	
  unsigned char	MonthFrozenL;	//�¶���ʱ�̣�0-23  HEX
  union{
    unsigned char Byte; 
    struct{
      unsigned char D0 : 1;     //1Ϊ�رճ�������
      unsigned char D1 : 1;     //1Ϊ�ر��ն��ᣬ���ǳ�������²��ر��ն��ᡣ
      unsigned char D2 : 1;	//1Ϊ�ر��¶��ᡣ
      unsigned char D3 : 1;	//1Ϊ�ر����㳭��
    }Bit;
  }FreezingSwitch;
}Server188_TimerData_Stuct;

//������GPRS����  ���ݸ�ʽ
typedef struct {
  union{
    unsigned char Byte; 
    struct{
      unsigned char D0 : 1;     //�����Ƿ�ʹ��DNS��0��ʹ��IP��ַ��1ʹ��DNS
      unsigned char D1 : 1;     //�����Ƿ�ʹ��APN��0����ʹ��APN��1ʹ��APN 
    }Bit;
  }ConnectionMode;	        //GPRS���ӷ�ʽ(HEX)
  unsigned char IP[4];		//ʮ�������� ��һ�ֽ�(HEX)
  union{
    unsigned short Word; 
    struct{
      unsigned char Byte_L;   //���ֽ�
      unsigned char Byte_H;   //���ֽ�
    }Byte;
  }Port;	              //���ֽ� ʮ������ ��λ��ǰ ��λ�ں� (HEX)       
  char APN[16];  //16�ֽڣ�������16�ֽڣ����ಿ����䡮\0�� (ASCII)
  char DNS[37];  //37�ֽڣ�������37�ֽڣ����ಿ����� ��\0��(ASCII)	
  unsigned char	BeatCycle; //1�ֽڣ���λ�����ӣ�(HEX)
}Server188_Gprs_Stuct;

//�ڵ���������ʽ
typedef struct {
  CJT188_MeterType              MeterType;      //�ڵ�����ID (HEX)  
  Server188_MeterAddr_Stuct     Addr;           //�ڵ��ַ
  Server188_LinkDeclarator      Link;           //�ڵ��Լ���ͼ�ͨ���� (HEX)
  Server188_CommDeclarator      Comm;	        //�ڵ�ͨѶ����
}Server188_Meter_Stuct;

//������ݸ�ʽ
typedef struct{
  Server188_Meter_Stuct Meter;             
  unsigned char Value[4];
                                // Byte1;    //��ǰ�ۻ�����С�������λ(BCD)
                                // Byte2;    //��ǰ�ۻ������������ֵĵ���λ(BCD)
                                // Byte3;    //��ǰ�ۻ������������ֵ��м���λ(BCD)
                                // Byte4;    //��ǰ�ۻ������������ֵĸ���λ(BCD)
  CJT188_ST_Stuct    ST;        //���״̬
}Server188_MeterValue_Stuct;

//���Ƽ�¼��ʽ��
typedef struct{
  unsigned char               OPID;	    //����ԱID(HEX)��
  Server188_Time_Stuct	       Time;
  CJT188_MeterType             MeterType;   //���ر�����ID(HEX)
  Server188_MeterAddr_Stuct    Addr;        //���ַ
  Server188_LinkDeclarator     Link;        //�ڵ��Լ���ͼ�ͨ���� (HEX)
  Server188_CommDeclarator     Comm;        //����ͨѶ����(HEX)
  unsigned char  MeterState;               //����״̬(HEX) 0X99���ط���0X55������
}Server188_OperateDeclarator_Stuct;

//���ü�����ʱ������ ��վ����>������ ����������� (�����룺0X01)
typedef struct {
  unsigned char Year;   	        //�� BCD
  unsigned char	Month;  	//�� BCD
  unsigned char	Day;    	//�� BCD
  unsigned char	Hour;		//ʱ BCD 
  unsigned char Minute;		//�� BCD
  unsigned char Second;		//�� BCD
  unsigned char WeekDay;	        //�ܼ� BCD
  unsigned char SER;	                /*���к�(HEX)*/
}Server188_TimeWcmd_askParam_Stuct; 
//���ü�����ʱ������ ����������>��վ ����������� (�����룺0X01)
typedef struct{
  unsigned char ReplyFlag;     /*Ӧ���־��(HEX)
                                  0x00 :����
                                  0xCC : ʱ�䲻�Ϸ� */
}Server188_TimeWcmd_ackParam_Stuct;

//��ȡ������ʱ������ ��վ����>������ ����������� (�����룺0X02)
typedef struct{
  unsigned char SER;	        /*���к�(HEX)*/
}Server188_TimeRcmd_askParam_Stuct;
//��ȡ������ʱ������ ����������>��վ ����������� (�����룺0X02)
typedef struct
{
  unsigned char Year;   	//�� BCD
  unsigned char Month;  	//�� BCD
  unsigned char Day;    	//�� BCD
  unsigned char Hour;		//ʱ BCD 
  unsigned char Minute;	//�� BCD
  unsigned char Second;	//�� BCD
  unsigned char WeekDay;	//�ܼ� BCD
  unsigned char ReplyFlag;     //Ӧ���־(HEX)��0x00���������쳣Ӧ�� 
}Server188_TimeRcmd_ackParam_Stuct;

//���ü������������   ��վ����>������  ����������� (�����룺0X03)
typedef struct{
  Server188_TimerData_Stuct   data;
  unsigned char              SER;	/*���к�(HEX)*/
}Server188_ConfigDataWcmd_askParam_Stuct;
//���ü������������   ����������> ��վ ����������� (�����룺0X03)
typedef struct{
  unsigned char  ReplyFlag;     /*Ӧ���־��(HEX)
                                    0x00 : ���� 
                                    0xCC : ����д��eeprom����ȷ*/
}Server188_ConfigDataWcmd_ackParam_Stuct;

//��ȡ�������������   ��վ����>������  ����������� (�����룺0X04)
typedef struct{
  unsigned char         SER;	/*���к�(HEX)*/
}Server188_ConfigDataRcmd_askParam_Stuct;

//��ȡ�������������   ����������> ��վ  ����������� (�����룺0X04) 	
typedef struct{
  Server188_TimerData_Stuct Data;
  unsigned char  ReplyFlag;   //Ӧ���־(HEX)��0x00���������쳣Ӧ��  
}Server188_ConfigDataRcmd_ackParam_Stuct;

//���ü�����GPRS����   ��վ����>������  ����������� (�����룺0x07) 
typedef struct{
  Server188_Gprs_Stuct   data;
  unsigned char   SER;	/*���к�(HEX)*/
}Server188_GprsWcmd_askParam_Stuct;
//���ü�����GPRS����   ����������> ��վ ����������� (�����룺0X07)
typedef struct{
  unsigned char  ReplyFlag;   //Ӧ���־(HEX)��0x00���������쳣Ӧ��
}Server188_GprsWcmd_ackParam_Stuct;

//��ȡ�������������   ��վ����>������  ����������� (�����룺0X08)
typedef struct{
  unsigned char         SER;	/*���к�(HEX)*/
}Server188_GprsRcmd_askParam_Stuct;

//��ȡ������GPRS����   ����������>��վ  ����������� (�����룺0X08)
typedef struct{
  Server188_Gprs_Stuct Data;
  unsigned char ReplyFlag;   //Ӧ���־(HEX)��0x00���������쳣Ӧ��
}Server188_GprsRcmd_ackParam_Stuct;

//���ر�Ƶ�ַ   ��վ����>������  �����������(�����룺0X09)
typedef struct {
  unsigned char  PackageSum;      //���зְ�(HEX)
  union{
    unsigned short Word;
    struct{
      unsigned char Byte_L;   //���ֽ�
      unsigned char Byte_H;   //���ֽ�
    }Byte;		
  }MeterSum;                     //���б���(HEX)         
  unsigned char  PackageNumber;	 //�ڼ�������(HEX)
  unsigned char  MetNumber; 	 //�ð���Ƶ�ַ����(HEX)
  Server188_Meter_Stuct Meter[25];      //���˵������(HEX)
  unsigned char  SER;	     /*���к�(HEX)*/
}Server188_MeterWcmd_askParam_Stuct;

//���ر�Ƶ�ַ   ����������>��վ ����������� (�����룺0X09)
typedef struct{
  unsigned char  ReplyFlag;   //Ӧ���־(HEX)��0x00 ���سɹ������쳣
}Server188_MeterWcmd_ackParam_Stuct;

//��ȡ���سɹ���Ƶ�ַ  ��վ����>������  �����������(�����룺0x0B)
typedef struct {
  //unsigned char  Channel;   /*ͨ����:(HEX)0x01��0x06									
  //00000001:	M-BUSͨ��1							
  //00000010:	M-BUSͨ��2							
  //00000011:	M-BUSͨ��3
  //00000100:	M-BUSͨ��4
  //00000101:	RS485ͨ��
  //00000110:	����ͨ��*/
  unsigned char  PackageNumber;   //�ڼ�������(HEX)
  unsigned char  SER;	     /*���к�(HEX)*/
}Server188_MeterRcmd_askParam_Stuct;

//��ȡ���سɹ���Ƶ�ַ  ����������>��վ  �����������(�����룺0x0B)
typedef struct {
  unsigned char  PackageSum;	 //���зְ�(HEX)
  union{
    unsigned short Word;
    struct{
      unsigned char Byte_L;      //���ֽ�
      unsigned char Byte_H;      //���ֽ�
    }Byte;
  }MeterSum;                     //���б���(HEX)       
  unsigned char  PackageNumber;	 //�ڼ�������(HEX)	
  unsigned char  MetNumber; 	 //�ð���Ƹ���
}Server188_MeterRcmd_ackParam_Spec_Stuct;

typedef struct {
  Server188_MeterRcmd_ackParam_Spec_Stuct   Spec;
  Server188_Meter_Stuct  Meter[25];      //���˵������  
}Server188_MeterRcmd_ackParam_Stuct;

//��ȡ����ʧ�ܱ�Ƶ�ַ  ��վ����>������  �����������(�����룺0x0C)
typedef struct {
  unsigned char  PackageNumber;	//�ڼ�������(HEX)
  unsigned char  SER;	        /*���к�(HEX)*/
}Server188_MeterErrRcmd_askParam_Stuct;

//��ȡ���سɹ���Ƶ�ַ  ����������>��վ  �����������(�����룺0x0C)
typedef struct {
  unsigned char  PackageSum;    //���зְ�(HEX)
  union{
    unsigned short Word;
    struct{
      unsigned char Byte_L;     //���ֽ�
      unsigned char Byte_H;     //���ֽ�
    }Byte;		
  }MeterSum;                            //  ���б���(HEX)
  unsigned char  PackageNumber;	//�ڼ�������(HEX)	
  unsigned char  MetNumber; 	        //�ð���Ƶ�ַ����(HEX)
}Server188_MeterErrRcmd_ackParam_Spec_Stuct;

typedef struct {
  Server188_MeterErrRcmd_ackParam_Spec_Stuct  Spec;
  Server188_Meter_Stuct Meter[25];     //���˵������(HEX)  
}Server188_MeterErrRcmd_ackParam_Stuct;

//�´�������� ��վ����>������  �����������(�����룺0X0E)  
typedef struct {
  Server188_OperateDeclarator_Stuct  Operate;
  unsigned char           SER;	    /*���к�(HEX)*/
}Server188_OperateWcmd_askParam_Stuct;

//�´��������  ����������>��վ  �����������(�����룺0X0E)
typedef struct {
  CJT188_MeterType  MeterType;	  	//�������ID   0x16   HEX	    
  Server188_MeterAddr_Stuct  Addr;
  unsigned char  MetState;  /*����״̬(HEX)  0x00���ط��ɹ�
  0x01���ط�����λ
  0x02���ط�ʧ��
  0x03�������ɹ�
  0x04����������λ
  0x05������ʧ��*/
  unsigned char ReplyFlag;  /*Ӧ���־ (HEX) 0x00: �ɹ���
  0x01: δ��ѯ����ƣ�
  0x02: �������Ϸ���
  0x05: MBUSͨ���쳣��
  0x06: �������Ӧ��
  0x07: ��Ӧ��
  0xCB: ����æ��
  0xCE: ���в�����ʱ*/
}Server188_OperateWcmd_ackParam_Stuct;

//ȡ���Ƽ�¼   ��վ����>������  �����������(�����룺0X0F	)  
typedef struct {
  unsigned char  PackageNumber;	//�ڼ�������  HEX
  unsigned char  SER;	        /*���к�(HEX)*/
}Server188_OperateRcmd_askParam_Stuct;

//ȡ���Ƽ�¼   ����������>��վ  �����������(�����룺0X0F)
typedef struct {
  unsigned short  PackageSum;           //����������(HEX)
  unsigned short  PackageNumber;        //�ڼ�������(HEX)
  unsigned short  ConNumber;            //���Ƽ�¼��N (HEX)
}Server188_OperateRcmd_ackParam_Spec_Stuct;
typedef struct {

  Server188_OperateRcmd_ackParam_Spec_Stuct  Spec;
  Server188_OperateDeclarator_Stuct  Record[15];  //���Ƽ�¼����;
  unsigned char  ReplyFlag;	       /*Ӧ���־(HEX)��0x00:����
                                         0xCA:�޴˰�*/
}Server188_OperateRcmd_ackParam_Stuct;

//������������  ��վ����>������  �����������(�����룺0X14)  
typedef struct {
  unsigned char SER;	   /*���к�(HEX)*/  	
}Server188_ReadingWcmd_askParam_Stuct;

//������������  ����������>��վ  �����������(�����룺0X14)  
typedef struct {
  unsigned char ReplyFlag; /*Ӧ���־ 0x00:������������
                              0x01:����æ�Ѿ��ڳ�����,�����ٴ�����
                              0x02:��������Ѳ����ַ,������������*/ 	
}Server188_ReadingWcmd_ackParam_Stuct;

//��ȡ���б�Ƶ�ǰ����  ��վ����>������  �����������(�����룺0X15)
typedef struct {
  unsigned char PackageNumber;	//�ڼ�������(HEX)
  unsigned char SER;	   /*���к�(HEX)*/  
}Server188_AllCurrentDataRcmd_askParam_Stuct; 

//��ȡ���б�Ƶ�ǰ����  ����������>��վ  �����������(�����룺0X15) 
typedef struct {
  unsigned char FileSum;    //�ļ�����(HEX)	
  unsigned char Filecount;  //�ļ����(HEX)
  unsigned char RecordNum;  //��¼�������������� (HEX)
  Server188_Time_Stuct Time;       //ʱ�� 
}Server188_AllCurrentDataRcmd_ackParam_Spec_Stuct;

typedef struct {
  Server188_AllCurrentDataRcmd_ackParam_Spec_Stuct Spec;
  Server188_MeterValue_Stuct Value[17];
}Server188_AllCurrentDataRcmd_ackParam_Stuct;

//��ȡȫ������ն�������   ��վ����>������  �����������(�����룺0X16)
typedef struct {
  unsigned char PackageNumber;	//�ڼ�������(HEX)
  unsigned char Year;	        //�� ���ֽ�(BCD)
  unsigned char Month;          //��(BCD)
  unsigned char Day;		//��(BCD)
  unsigned char SER;	        /*���к�(HEX)*/  
}Server188_AllFreezingDataRcmd_askParam_Stuct;  

//��ȡȫ������ն�������   ����������>��վ  �����������(�����룺0X16) 
typedef struct {
  unsigned char FileSum;       //�ļ�����(HEX)	
  unsigned char Filecount;     //�ļ����(HEX)
  unsigned char RecordNum;     //��¼�������������� (HEX)
  Server188_Time_Stuct Time;          //ʱ�� 
}Server188_AllFreezingDataRcmd_ackParam_Spec_Stuct;

typedef struct {
  Server188_AllFreezingDataRcmd_ackParam_Spec_Stuct   Spec;
  union{
    Server188_MeterValue_Stuct  CJT188[17];
    unsigned char buff[sizeof(Server188_MeterValue_Stuct )*17];
  }Value; 
}Server188_AllFreezingDataRcmd_ackParam_Stuct;

//������ĵ�ǰ����   ��վ����>������  �����������(�����룺0X17)
typedef struct {
  Server188_Meter_Stuct  Meter; //�������
  unsigned char   SER;	 /*���к�(HEX)*/  
}Server188_SingCurrentDataRcmd_askParam_Stuct;  

//������ĵ�ǰ����   ����������>��վ  �����������(�����룺0X17)  
typedef struct {
  //Server188_MeterValue_Stuct Data;
  Server188_Meter_Stuct Meter;             
  unsigned char Value[4];
                                // Byte1;    //��ǰ�ۻ�����С�������λ(BCD)
                                // Byte2;    //��ǰ�ۻ������������ֵĵ���λ(BCD)
                                // Byte3;    //��ǰ�ۻ������������ֵ��м���λ(BCD)
                                // Byte4;    //��ǰ�ۻ������������ֵĸ���λ(BCD)
  CJT188_ST_Stuct    ST;        //���״̬
  unsigned char ReplyFlag; /*Ӧ���־(HEX)��0x00:������0xCF:δ�ҵ��� */
}Server188_SingCurrentDataRcmd_ackParam_Stuct;

//��ȡ������ն�������   ��վ����>������  �����������(�����룺0X18)
typedef struct {
  unsigned char	  Year;	          //�� ���ֽ�(BCD)
  unsigned char	  Month;          //��(BCD)
  unsigned char	  Day;	          //��(BCD)
  Server188_Meter_Stuct   Meter;          //�������
  unsigned char   SER;	                  /*���к�(HEX)*/  
}Server188_SingFreezingDataRcmd_askParam_Stuct;

//��ȡ������ն�������   ����������>��վ  �����������(�����룺0X18)  
typedef struct {
  Server188_Time_Stuct          Time;            //ʱ��
  Server188_Meter_Stuct         Meter;           //�ڵ�������           
  unsigned char                Value[4];       // Byte1;    //��ǰ�ۻ�����С�������λ(BCD)
                                                // Byte2;    //��ǰ�ۻ������������ֵĵ���λ(BCD)
                                                // Byte3;    //��ǰ�ۻ������������ֵ��м���λ(BCD)
                                                // Byte4;    //��ǰ�ۻ������������ֵĸ���λ(BCD)
  CJT188_ST_Stuct               ST;             //״̬
  unsigned char ReplyFlag;                    /*Ӧ���־(HEX)��0x00:����;
                                                                0xCD:δ�ҵ������� 
                                                                0xCF:δ�ҵ��� */
}Server188_SingFreezingDataRcmd_ackParam_Stuct;

//�滻���   ��վ����>������  �����������(�����룺0X1C)
typedef struct {
  Server188_Meter_Stuct  OldMeter; 	//�������
  Server188_Meter_Stuct  NewMeter; 	//�������
  unsigned char         SER;	        /*���к�(HEX)*/  
}Server188_AlterMeterWcmd_askParam_Stuct;  
//�滻���   ����������>��վ  �����������(�����룺0X1C) 
typedef struct {
  unsigned char ReplyFlag; /*Ӧ���־(HEX): 0x00: �滻�ɹ�
                              0x01: δ��ѯ�����
                              0x02: �������Ϸ�
                              0x04: ��ַ�ظ�
                              0x05: MBUSͨ���쳣
                              0x06: �������Ӧ
                              0xCB: ����æ
                              0xCE: ���в�����ʱ
                              0xEF: �洢��д�����  
                                */
  //unsigned char   SER;	    /*���к�(HEX)*/ 
}Server188_AlterMeterWcmd_ackParam_Stuct;   

//ɾ�����   ��վ����>������  �����������(�����룺0X1c)
typedef struct {
  unsigned char         TotalNumber;    //ɾ����¼����(HEX)  
  Server188_Meter_Stuct  Meter[25]; 	  //�������
  unsigned char         SER;	          /*���к�(HEX)*/ 
}Server188_DeleteMeterWcmd_askParam_Stuct; 

typedef struct {
  Server188_MeterAddr_Stuct     Addr;
  unsigned char Flg;
}Server188_DeleteResult_Stuct;

//ɾ�����   ����������>��վ  �����������(�����룺0X1c) 
typedef struct {
  unsigned char TotalNumber;      //ɾ����¼����(HEX)
  Server188_DeleteResult_Stuct Result[25]; // ��N�� 00ɾ���ɹ���01ɾ��ʧ��

  //unsigned char ReplyFlag;  //Ӧ���־ 0x00:ɾ���ɹ�
                              //0x01:����æ�Ѿ��ڳ�����,�����ٴ�����
                              //0x02:��������Ѳ����ַ,������������	
}Server188_DeleteMeterWcmd_ackParam_Stuct; 


  
//������   ��վ����>������  �����������(�����룺0X20)
typedef struct {
  unsigned char         SER;	          /*���к�(HEX)*/ 
}Server188_CleanMeterWcmd_askParam_Stuct;   
  
//������   ����������>��վ  �����������(�����룺0X20) 
typedef struct {
  unsigned char ReplyFlag;  //Ӧ���־ 0x00:ɾ���ɹ�
                              //0x01:����æ�Ѿ��ڳ�����,�����ٴ�����
                              //0x02:��������Ѳ����ַ,������������	
}Server188_CleanMeterWcmd_ackParam_Stuct; 



  
 typedef  union{
  unsigned long Word; 
  struct{

    unsigned long Mbus1 : 1;    
    unsigned long Mbus2 : 1;
    unsigned long Mbus3 : 1;
    unsigned long Mbus4 : 1;
    unsigned long MbusPower : 1;  
    unsigned long Mode  : 2;  //����ģʽ
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
  unsigned char         Bat;      //��Դ��ѹ       
  Server188_Time_Stuct   time;
  unsigned long long    Flow[2];
  union{
      unsigned char Byte;
      struct{
        unsigned char  CN     :1;    //���������ӱ�־    
        unsigned char  ACK    :1;    //������ Ӧ��
        unsigned char  GZ     :1;    //�������������
        unsigned char  EN     :1;    //����������
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
//֡֡ͷ��ʽ
typedef struct {	
  unsigned char StartCharacter;  //֡ͷֵΪ0x68H
  unsigned char LAI;             //����ʶ����	1Byte	����ʶ�����ݴ��������
  unsigned char Concentrator[ConcentratorAddr_Length]; //��������ַ 5�ֽ�BCD��
  unsigned char Code;            //������
  union{
    unsigned short Word;
    struct{
      unsigned char Byte_L;
      unsigned char Byte_H;
    }Byte;
  }DataLength;                   //���ݶγ���
}Server188_FrameHead_Struct;

/*
typedef  union{ 
  void* Param;
  //���ü�����ʱ������ ��վ����>������ ����������� (�����룺0X01)
  Server188_TimeWcmd_askParam_Stuct*              TimeWcmd_ask; 
  //���ü�����ʱ������ ����������>��վ ����������� (�����룺0X01)
  Server188_TimeWcmd_ackParam_Stuct*              TimeWcmd_ack;
  //��ȡ������ʱ������ ��վ����>������ ����������� (�����룺0X02)
  Server188_TimeRcmd_askParam_Stuct*              TimeRcmd_ask;
  //��ȡ������ʱ������ ����������>��վ ����������� (�����룺0X02)
  Server188_TimeRcmd_ackParam_Stuct*              TimeRcmd_ack;
  //���ü������������   ��վ����>������  ����������� (�����룺0X03)
  Server188_ConfigDataWcmd_askParam_Stuct*        ConfigDataWcmd_ask;
  //���ü������������   ����������> ��վ ����������� (�����룺0X03)
  Server188_ConfigDataWcmd_ackParam_Stuct*        ConfigDataWcmd_ack;
  //��ȡ�������������   ��վ����>������  ����������� (�����룺0X04)
  Server188_ConfigDataRcmd_askParam_Stuct*        ConfigDataRcmd_ask; 	
  //��ȡ�������������   ����������> ��վ  ����������� (�����룺0X04) 	
  Server188_ConfigDataRcmd_ackParam_Stuct*        ConfigDataRcmd_ack;
  //���ü�����GPRS����   ��վ����>������  ����������� (�����룺0x07) 
  Server188_GprsWcmd_askParam_Stuct*              GprsWcmd_ask;
  //���ü�����GPRS����   ����������> ��վ ����������� (�����룺0X07)
  Server188_GprsWcmd_ackParam_Stuct*              GprsWcmd_ack;
  //��ȡ�������������   ��վ����>������  ����������� (�����룺0X08)
  Server188_GprsRcmd_askParam_Stuct*              GprsRcmd_ask;
  //��ȡ������GPRS����   ����������>��վ  ����������� (�����룺0X08)
  Server188_GprsRcmd_ackParam_Stuct*              GprsRcmd_ack;
  //���ر�Ƶ�ַ   ��վ����>������  �����������(�����룺0X09)
  Server188_MeterWcmd_askParam_Stuct*             MeterWcmd_ask;
  //���ر�Ƶ�ַ   ����������>��վ ����������� (�����룺0X09)
  Server188_MeterWcmd_ackParam_Stuct*             MeterWcmd_ack;
  //��ȡ���سɹ���Ƶ�ַ  ��վ����>������  �����������(�����룺0x0B)
  Server188_MeterRcmd_askParam_Stuct*             MeterRcmd_ask;
  //��ȡ���سɹ���Ƶ�ַ  ����������>��վ  �����������(�����룺0x0B)
  Server188_MeterRcmd_ackParam_Stuct*             MeterRcmd_ack;
  //��ȡ����ʧ�ܱ�Ƶ�ַ  ��վ����>������  �����������(�����룺0x0C)
  Server188_MeterErrRcmd_askParam_Stuct*          MeterErrRcmd_ask;
  //��ȡ���سɹ���Ƶ�ַ  ����������>��վ  �����������(�����룺0x0C)
  Server188_MeterErrRcmd_ackParam_Stuct*          MeterErrRcmd_ack;
  //�´�������� ��վ����>������  �����������(�����룺0X0E)  
  Server188_OperateWcmd_askParam_Stuct*          OperateWcmd_ask;
  //�´��������  ����������>��վ  �����������(�����룺0X0E)
  Server188_OperateWcmd_ackParam_Stuct*          OperateWcmd_ack;
  //ȡ���Ƽ�¼   ��վ����>������  �����������(�����룺0X0F	)  
  Server188_OperateRcmd_askParam_Stuct*          OperateRcmd_ask;
  //ȡ���Ƽ�¼   ����������>��վ  �����������(�����룺0X0F)
  Server188_OperateRcmd_ackParam_Stuct*          OperateRcmd_ack;
  //������������  ��վ����>������  �����������(�����룺0X14)  
  Server188_ReadingWcmd_askParam_Stuct*          ReadingWcmd_ask;
  //������������  ����������>��վ  �����������(�����룺0X14)  
  Server188_ReadingWcmd_ackParam_Stuct*          ReadingWcmd_ack;
  //��ȡ���б�Ƶ�ǰ����  ��վ����>������  �����������(�����룺0X15)
  Server188_AllCurrentDataRcmd_askParam_Stuct*   AllCurrentDataRcmd_ask; 
  //��ȡ���б�Ƶ�ǰ����  ����������>��վ  �����������(�����룺0X15)  
  Server188_AllCurrentDataRcmd_ackParam_Stuct*   AllCurrentDataRcmd_ack;
  //��ȡȫ������ն�������   ��վ����>������  �����������(�����룺0X16)
  Server188_AllFreezingDataRcmd_askParam_Stuct*   AllFreezingDataRcmd_ask;  
  //��ȡȫ������ն�������   ����������>��վ  �����������(�����룺0X16)  
  Server188_AllFreezingDataRcmd_ackParam_Stuct*     AllFreezingDataRcmd_ack;
  //������ĵ�ǰ����   ��վ����>������  �����������(�����룺0X17)
  Server188_SingCurrentDataRcmd_askParam_Stuct*         SingCurrentDataRcmd_ask;  
  //������ĵ�ǰ����   ����������>��վ  �����������(�����룺0X17)  
  Server188_SingCurrentDataRcmd_ackParam_Stuct*  SingCurrentDataRcmd_ack;
  //��ȡ������ն�������   ��վ����>������  �����������(�����룺0X18)
  Server188_SingFreezingDataRcmd_askParam_Stuct*       SingFreezingDataRcmd_ask;
  //��ȡ������ն�������   ����������>��վ  �����������(�����룺0X18)  
  Server188_SingFreezingDataRcmd_ackParam_Stuct*       SingFreezingDataRcmd_ack;
  //�滻���   ��վ����>������  �����������(�����룺0X1C)
  Server188_AlterMeterWcmd_askParam_Stuct*    AlterMeterWcmd_ask; 
  //�滻���   ����������>��վ  �����������(�����룺0X1C) 
  Server188_AlterMeterWcmd_ackParam_Stuct*    AlterMeterWcmd_ack;   
  //ɾ�����   ��վ����>������  �����������(�����룺0X1c)
  Server188_DeleteMeterWcmd_askParam_Stuct*   DeleteMeterWcmd_ask;
  //ɾ�����   ����������>��վ  �����������(�����룺0X1c) 
  Server188_DeleteMeterWcmd_ackParam_Stuct*   DeleteMeterWcmd_ack;
  //������   ��վ����>������  �����������(�����룺0X20)
  Server188_CleanMeterWcmd_askParam_Stuct     CleanMeterWcmd_ask;   
    //������   ����������>��վ  �����������(�����룺0X20) 
  Server188_CleanMeterWcmd_ackParam_Stuct     CleanMeterWcmd_ack; 
  unsigned char                       Data[256]; 
}Server188_FrameParam1;*/


//֡β��ʽ
typedef struct {
  unsigned char CS;     //У����,��֡ͷ��ʼ��֡У���־֮ǰ�����ֽڵ��ۼӺ͵�8λ	
  unsigned char Pause;  //������0x16H
}Server188_FrameStern_Struct;


//����֡��ʽ
typedef struct{
  Server188_FrameHead_Struct  Head;
  //Server188_FrameParam1  Param;
  union{ 
    //���ü�����ʱ������ ��վ����>������ ����������� (�����룺0X01)
    Server188_TimeWcmd_askParam_Stuct              TimeWcmd_ask; 
    //���ü�����ʱ������ ����������>��վ ����������� (�����룺0X01)
    Server188_TimeWcmd_ackParam_Stuct              TimeWcmd_ack;
    //��ȡ������ʱ������ ��վ����>������ ����������� (�����룺0X02)
    Server188_TimeRcmd_askParam_Stuct              TimeRcmd_ask;
    //��ȡ������ʱ������ ����������>��վ ����������� (�����룺0X02)
    Server188_TimeRcmd_ackParam_Stuct              TimeRcmd_ack;
    //���ü������������   ��վ����>������  ����������� (�����룺0X03)
    Server188_ConfigDataWcmd_askParam_Stuct        ConfigDataWcmd_ask;
    //���ü������������   ����������> ��վ ����������� (�����룺0X03)
    Server188_ConfigDataWcmd_ackParam_Stuct        ConfigDataWcmd_ack;
    //��ȡ�������������   ��վ����>������  ����������� (�����룺0X04)
    Server188_ConfigDataRcmd_askParam_Stuct        ConfigDataRcmd_ask; 	
    //��ȡ�������������   ����������> ��վ  ����������� (�����룺0X04) 	
    Server188_ConfigDataRcmd_ackParam_Stuct        ConfigDataRcmd_ack;
    //���ü�����GPRS����   ��վ����>������  ����������� (�����룺0x07) 
    Server188_GprsWcmd_askParam_Stuct              GprsWcmd_ask;
    //���ü�����GPRS����   ����������> ��վ ����������� (�����룺0X07)
    Server188_GprsWcmd_ackParam_Stuct              GprsWcmd_ack;
    //��ȡ�������������   ��վ����>������  ����������� (�����룺0X08)
    Server188_GprsRcmd_askParam_Stuct              GprsRcmd_ask;
    //��ȡ������GPRS����   ����������>��վ  ����������� (�����룺0X08)
    Server188_GprsRcmd_ackParam_Stuct              GprsRcmd_ack;
    //���ر�Ƶ�ַ   ��վ����>������  �����������(�����룺0X09)
    Server188_MeterWcmd_askParam_Stuct             MeterWcmd_ask;
    //���ر�Ƶ�ַ   ����������>��վ ����������� (�����룺0X09)
    Server188_MeterWcmd_ackParam_Stuct             MeterWcmd_ack;
    //��ȡ���سɹ���Ƶ�ַ  ��վ����>������  �����������(�����룺0x0B)
    Server188_MeterRcmd_askParam_Stuct             MeterRcmd_ask;
    //��ȡ���سɹ���Ƶ�ַ  ����������>��վ  �����������(�����룺0x0B)
    Server188_MeterRcmd_ackParam_Stuct             MeterRcmd_ack;
    //��ȡ����ʧ�ܱ�Ƶ�ַ  ��վ����>������  �����������(�����룺0x0C)
    Server188_MeterErrRcmd_askParam_Stuct          MeterErrRcmd_ask;
    //��ȡ���سɹ���Ƶ�ַ  ����������>��վ  �����������(�����룺0x0C)
    Server188_MeterErrRcmd_ackParam_Stuct          MeterErrRcmd_ack;
    //�´�������� ��վ����>������  �����������(�����룺0X0E)  
    Server188_OperateWcmd_askParam_Stuct          OperateWcmd_ask;
    //�´��������  ����������>��վ  �����������(�����룺0X0E)
    Server188_OperateWcmd_ackParam_Stuct          OperateWcmd_ack;
    //ȡ���Ƽ�¼   ��վ����>������  �����������(�����룺0X0F	)  
    Server188_OperateRcmd_askParam_Stuct          OperateRcmd_ask;
    //ȡ���Ƽ�¼   ����������>��վ  �����������(�����룺0X0F)
    Server188_OperateRcmd_ackParam_Stuct          OperateRcmd_ack;
    //������������  ��վ����>������  �����������(�����룺0X14)  
    Server188_ReadingWcmd_askParam_Stuct          ReadingWcmd_ask;
    //������������  ����������>��վ  �����������(�����룺0X14)  
    Server188_ReadingWcmd_ackParam_Stuct          ReadingWcmd_ack;
    //��ȡ���б�Ƶ�ǰ����  ��վ����>������  �����������(�����룺0X15)
    Server188_AllCurrentDataRcmd_askParam_Stuct   AllCurrentDataRcmd_ask; 
    //��ȡ���б�Ƶ�ǰ����  ����������>��վ  �����������(�����룺0X15)  
    Server188_AllCurrentDataRcmd_ackParam_Stuct   AllCurrentDataRcmd_ack;
    //��ȡȫ������ն�������   ��վ����>������  �����������(�����룺0X16)
    Server188_AllFreezingDataRcmd_askParam_Stuct   AllFreezingDataRcmd_ask;  
    //��ȡȫ������ն�������   ����������>��վ  �����������(�����룺0X16)  
    Server188_AllFreezingDataRcmd_ackParam_Stuct     AllFreezingDataRcmd_ack;
    //������ĵ�ǰ����   ��վ����>������  �����������(�����룺0X17)
    Server188_SingCurrentDataRcmd_askParam_Stuct         SingCurrentDataRcmd_ask;  
    //������ĵ�ǰ����   ����������>��վ  �����������(�����룺0X17)  
    Server188_SingCurrentDataRcmd_ackParam_Stuct  SingCurrentDataRcmd_CJT188_ack;
    //��ȡ������ն�������   ��վ����>������  �����������(�����룺0X18)
    Server188_SingFreezingDataRcmd_askParam_Stuct       SingFreezingDataRcmd_ask;
    //��ȡ������ն�������   ����������>��վ  �����������(�����룺0X18)  
    Server188_SingFreezingDataRcmd_ackParam_Stuct       SingFreezingDataRcmd_ack;
    //�滻���   ��վ����>������  �����������(�����룺0X1C)
    Server188_AlterMeterWcmd_askParam_Stuct    AlterMeterWcmd_ask; 
    //�滻���   ����������>��վ  �����������(�����룺0X1C) 
    Server188_AlterMeterWcmd_ackParam_Stuct    AlterMeterWcmd_ack;   
    //ɾ�����   ��վ����>������  �����������(�����룺0X1c)
    Server188_DeleteMeterWcmd_askParam_Stuct   DeleteMeterWcmd_ask;
    //ɾ�����   ����������>��վ  �����������(�����룺0X1c) 
    Server188_DeleteMeterWcmd_ackParam_Stuct   DeleteMeterWcmd_ack;  
    //������   ��վ����>������  �����������(�����룺0X20)
    Server188_CleanMeterWcmd_askParam_Stuct     CleanMeterWcmd_ask;   
    //������   ����������>��վ  �����������(�����룺0X20) 
    Server188_CleanMeterWcmd_ackParam_Stuct     CleanMeterWcmd_ack; 
    
    Server188_HeartbeatPacket_Param_Stuct      HeartBeat;
    Server188_Err_Param_Stuct                  Err_Param;
    unsigned char                       Data[256]; 
  }Param;
}Server188_FrameDatas_Struct;

//����֡��ʽ
typedef struct{
  Server188_PreambleType          Preamble; //ǰ���ֽ�  ����2���ֽ�FEH��������У�飩��
  Server188_FrameDatas_Struct     Datas;
}Server188_Frame_Struct;

#pragma pack()

typedef struct {
  union{
    unsigned char Byte;
    struct{
      unsigned char idle :1; //����������
    }Bit;
  }CSR; //����״̬�Ĵ���
  CommPortNumber_Type      SourcePort;
  Server188_Frame_Struct   Frame; 
}Server188_OutPutBuff_Struct;

//����������  ���ݸ�ʽ
typedef struct {
  unsigned char AreaCode;   //������
  char ID[ConcentratorAddr_Length]; //��������ַ 5�ֽ�BCD��
  Server188_TimerData_Stuct  timer;
  Server188_Gprs_Stuct       gprs; 
}Server188_ConfigData_Stuct;

typedef  union{ 
  void* Param;
  //���ü�����ʱ������ ��վ����>������ ����������� (�����룺0X01)
  Server188_TimeWcmd_askParam_Stuct*              TimeWcmd_ask; 
  //���ü�����ʱ������ ����������>��վ ����������� (�����룺0X01)
  Server188_TimeWcmd_ackParam_Stuct*              TimeWcmd_ack;
  //��ȡ������ʱ������ ��վ����>������ ����������� (�����룺0X02)
  Server188_TimeRcmd_askParam_Stuct*              TimeRcmd_ask;
  //��ȡ������ʱ������ ����������>��վ ����������� (�����룺0X02)
  Server188_TimeRcmd_ackParam_Stuct*              TimeRcmd_ack;
  //���ü������������   ��վ����>������  ����������� (�����룺0X03)
  Server188_ConfigDataWcmd_askParam_Stuct*        ConfigDataWcmd_ask;
  //���ü������������   ����������> ��վ ����������� (�����룺0X03)
  Server188_ConfigDataWcmd_ackParam_Stuct*        ConfigDataWcmd_ack;
  //��ȡ�������������   ��վ����>������  ����������� (�����룺0X04)
  Server188_ConfigDataRcmd_askParam_Stuct*        ConfigDataRcmd_ask; 	
  //��ȡ�������������   ����������> ��վ  ����������� (�����룺0X04) 	
  Server188_ConfigDataRcmd_ackParam_Stuct*        ConfigDataRcmd_ack;
  //���ü�����GPRS����   ��վ����>������  ����������� (�����룺0x07) 
  Server188_GprsWcmd_askParam_Stuct*              GprsWcmd_ask;
  //���ü�����GPRS����   ����������> ��վ ����������� (�����룺0X07)
  Server188_GprsWcmd_ackParam_Stuct*              GprsWcmd_ack;
  //��ȡ�������������   ��վ����>������  ����������� (�����룺0X08)
  Server188_GprsRcmd_askParam_Stuct*              GprsRcmd_ask;
  //��ȡ������GPRS����   ����������>��վ  ����������� (�����룺0X08)
  Server188_GprsRcmd_ackParam_Stuct*              GprsRcmd_ack;
  //���ر�Ƶ�ַ   ��վ����>������  �����������(�����룺0X09)
  Server188_MeterWcmd_askParam_Stuct*             MeterWcmd_ask;
  //���ر�Ƶ�ַ   ����������>��վ ����������� (�����룺0X09)
  Server188_MeterWcmd_ackParam_Stuct*             MeterWcmd_ack;
  //��ȡ���سɹ���Ƶ�ַ  ��վ����>������  �����������(�����룺0x0B)
  Server188_MeterRcmd_askParam_Stuct*             MeterRcmd_ask;
  //��ȡ���سɹ���Ƶ�ַ  ����������>��վ  �����������(�����룺0x0B)
  Server188_MeterRcmd_ackParam_Stuct*             MeterRcmd_ack;
  //��ȡ����ʧ�ܱ�Ƶ�ַ  ��վ����>������  �����������(�����룺0x0C)
  Server188_MeterErrRcmd_askParam_Stuct*          MeterErrRcmd_ask;
  //��ȡ���سɹ���Ƶ�ַ  ����������>��վ  �����������(�����룺0x0C)
  Server188_MeterErrRcmd_ackParam_Stuct*          MeterErrRcmd_ack;
  //�´�������� ��վ����>������  �����������(�����룺0X0E)  
  Server188_OperateWcmd_askParam_Stuct*          OperateWcmd_ask;
  //�´��������  ����������>��վ  �����������(�����룺0X0E)
  Server188_OperateWcmd_ackParam_Stuct*          OperateWcmd_ack;
  //ȡ���Ƽ�¼   ��վ����>������  �����������(�����룺0X0F	)  
  Server188_OperateRcmd_askParam_Stuct*          OperateRcmd_ask;
  //ȡ���Ƽ�¼   ����������>��վ  �����������(�����룺0X0F)
  Server188_OperateRcmd_ackParam_Stuct*          OperateRcmd_ack;
  //������������  ��վ����>������  �����������(�����룺0X14)  
  Server188_ReadingWcmd_askParam_Stuct*          ReadingWcmd_ask;
  //������������  ����������>��վ  �����������(�����룺0X14)  
  Server188_ReadingWcmd_ackParam_Stuct*          ReadingWcmd_ack;
  //��ȡ���б�Ƶ�ǰ����  ��վ����>������  �����������(�����룺0X15)
  Server188_AllCurrentDataRcmd_askParam_Stuct*   AllCurrentDataRcmd_ask; 
  //��ȡ���б�Ƶ�ǰ����  ����������>��վ  �����������(�����룺0X15)  
  Server188_AllCurrentDataRcmd_ackParam_Stuct*   AllCurrentDataRcmd_ack;
  //��ȡȫ������ն�������   ��վ����>������  �����������(�����룺0X16)
  Server188_AllFreezingDataRcmd_askParam_Stuct*   AllFreezingDataRcmd_ask;  
  //��ȡȫ������ն�������   ����������>��վ  �����������(�����룺0X16)  
  Server188_AllFreezingDataRcmd_ackParam_Stuct*     AllFreezingDataRcmd_ack;
  //������ĵ�ǰ����   ��վ����>������  �����������(�����룺0X17)
  Server188_SingCurrentDataRcmd_askParam_Stuct*         SingCurrentDataRcmd_ask;  
  //������ĵ�ǰ����   ����������>��վ  �����������(�����룺0X17)  
  Server188_SingCurrentDataRcmd_ackParam_Stuct*  SingCurrentDataRcmd_ack;
  //��ȡ������ն�������   ��վ����>������  �����������(�����룺0X18)
  Server188_SingFreezingDataRcmd_askParam_Stuct*       SingFreezingDataRcmd_ask;
  //��ȡ������ն�������   ����������>��վ  �����������(�����룺0X18)  
  Server188_SingFreezingDataRcmd_ackParam_Stuct*       SingFreezingDataRcmd_ack;
  //�滻���   ��վ����>������  �����������(�����룺0X1C)
  Server188_AlterMeterWcmd_askParam_Stuct*    AlterMeterWcmd_ask; 
  //�滻���   ����������>��վ  �����������(�����룺0X1C) 
  Server188_AlterMeterWcmd_ackParam_Stuct*    AlterMeterWcmd_ack;   
  //ɾ�����   ��վ����>������  �����������(�����룺0X1c)
  Server188_DeleteMeterWcmd_askParam_Stuct*   DeleteMeterWcmd_ask;
  //ɾ�����   ����������>��վ  �����������(�����룺0X1c) 
  Server188_DeleteMeterWcmd_ackParam_Stuct*   DeleteMeterWcmd_ack;
  //������   ��վ����>������  �����������(�����룺0X20)
  Server188_CleanMeterWcmd_askParam_Stuct*     CleanMeterWcmd_ask;   
    //������   ����������>��վ  �����������(�����룺0X20) 
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
�ⲿ����������
*********************************************************************************************************/
extern Server188_ConfigData_Stuct      _188_ConcenConfigData;
extern Server188_ConcenMbusState_Type  ReadingMeterState;  
/*********************************************************************************************************
����������
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