/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\inc\SystemInit.h"
#include "..\inc\CJT-188-2004.h"
#include "..\inc\Server188.h"
/*********************************************************************************
����������
*********************************************************************************/
const unsigned char  S1 = 1;
const unsigned char  S2 = 2;
/*********************************************************************************
��������������
*********************************************************************************/
//ConcentratorParameterStruct   Concentrator;

unsigned char  TaskLose = 0; 

TaskHandle_t    ManualTask = NULL;      //����
TaskHandle_t    ServerTask = NULL;      //����
TaskHandle_t    SacnTask = NULL;        //���� 
TaskHandle_t    MbusTask = NULL;        //����
TaskHandle_t    GprsTask = NULL;        //����
TaskHandle_t    Server1Task = NULL;     //���� 
TaskHandle_t    Server2Task = NULL;     //����

QueueHandle_t   ManualQueue = NULL;             //��Ϣ����
QueueHandle_t   GprsQueue = NULL;               //��Ϣ����
QueueHandle_t   GprsIPSendQueue = NULL;         //��Ϣ����
//QueueHandle_t   MbusSendQueue = NULL;           //��Ϣ���� 
QueueHandle_t   MbusReceiveQueue = NULL;        //��Ϣ����
QueueHandle_t   ServerQueue[] = {NULL,NULL};    //��Ϣ����

SemaphoreHandle_t  Gprs_xMutex = NULL;       //�����ź���     
SemaphoreHandle_t  Fram_xMutex = NULL;       //�����ź��� ����
SemaphoreHandle_t  Mbus_xMutex = NULL;      

unsigned short   ServerTask_MonitorTimer[2] = {0,0};    //������  ������ӿ��Ź���ʱ��
unsigned short   GprsTask_MonitorTimer = 0;              //Gprsͨ�� ������ӿ��Ź���ʱ��
unsigned short   ManualTask_MonitorTimer = 0;            //�����ֱ� ������ӿ��Ź���ʱ��

/*********************************************************************************
�ⲿ����������
*********************************************************************************/

/*********************************************************************************
˽�б���������
*********************************************************************************/
static unsigned long FullReadingTime = 0;
/*********************************************************************************
���Ա���������
*********************************************************************************/
static TaskHandle_t Led1Task = NULL;
//static TaskHandle_t Led2Task = NULL;   

//static  unsigned short  vin;
//static  unsigned short  wfrmbuff[512];
/*********************************************************************************
�ڲ�����������
*********************************************************************************/
static void Led1_Task ( void* Param );
//static void Led2_Task ( void* Param );

/*********************************************************************************
���ܴ��붨����
*********************************************************************************/
void ManualTask_func ( void* Param );
/*********************************************************************************
 Function:      //
 Description:   // 
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
int Systerm_INIT(void)
{
  Launch_USART(Rs232_Port, 9600, Parity_Even, StopBits_2, ByDMA);
  //Launch_USART(GPRS_COMM,38400, Parity_No, StopBits_2, ByDMA);  //�򿪿��ƶ˿�;
  

    
  CJT188_2004_init ( );
  Server188_SPCP_init( );
  
  ServerState[0].Port =  Null_Port;
  ServerState[0].Connect_CNT = 0;
  ServerState[0].SR.Byte = 0;
  
  ServerState[1].Port =  Null_Port;
  ServerState[1].Connect_CNT = 0;
  ServerState[1].SR.Byte = 0;
  
  return 0;
}
/*********************************************************************************
 Function:      //
 Description:   //ϵͳ��������
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/  
void Launch_Systerm ( void* Param )
{
   
  //SystemErrName err;
  //unsigned short itemp;
  //CommPortNumber_Type Port;
  //CJT188_Frame_Struct  ack;  
  //CJT188_MeterDeclarator  Meter;
  //CJT8188_ValveAsk_Struct  ValveAsk; 
 
  Systerm_INIT(  ); 
     
  ManualQueue = xQueueCreate(32, sizeof(void *));
  GprsQueue = xQueueCreate(1, sizeof(void *));
  MbusReceiveQueue = xQueueCreate(2, sizeof(void *));;
  //MbusSendQueue = xQueueCreate(16, sizeof(OnSceneComm_PackageStruct)); 
  ServerQueue[0] = xQueueCreate(4, sizeof(void *));
  ServerQueue[1] = xQueueCreate(4, sizeof(void *));
  
  Gprs_xMutex = xSemaphoreCreateMutex();
  Fram_xMutex = xSemaphoreCreateMutex();       //�����ź��� ���� 
  Mbus_xMutex = xSemaphoreCreateMutex();       //�����ź��� ���� 
  
  
  xTaskCreate(Led1_Task,         /* ������ */
              "vTaskUserIF",     /* ������ */
              512,               /* stack ��С����λ word */
              NULL,              /* ������� */
             1,                 /* �������ȼ�*/
             &Led1Task);        /* ������ */

  xTaskCreate(ManualTask_func,         /* ������ */
             "ManualTask",     /* ������ */
             512,               /* stack ��С����λ word */
              NULL,              /* ������� */
              4,                 /* �������ȼ�*/
              &ManualTask);        /* ������ */
  
  xTaskCreate(GprsTask_func,         /* ������ */
              "GprsTask",     /* ������ */
              512,               /* stack ��С����λ word */
              NULL,              /* ������� */
              2,                 /* �������ȼ�*/
              &GprsTask);        /* ������ */
  
  xTaskCreate(Server_Task,         /* ������ */
              "Server_1",     /* ������ */
              1024,               /* stack ��С����λ word */
              "1",              /* ������� */
              3,                 /* �������ȼ�*/
              &Server1Task);        /* ������ */ 
  
  //xTaskCreate(Server_Task,         /* ������ */
  //          "Server_2",     /* ������ */
  //          512,               /* stack ��С����λ word */
  //         "2",              /* ������� */
  //          3,                 /* �������ȼ�*/
  //         &Server2Task);        /* ������ */
  

  
 
  
  while(1)
  {
    vTaskDelay(_1s);
    
    if(ServerTask_MonitorTimer[0] > 600)
    { TaskLose = 1;}   //������ 1  ����ֹͣ��ת
    else
    {ServerTask_MonitorTimer[0] ++; }
    
    if(ServerTask_MonitorTimer[1] > 600)
    {}   //������ 2  ����ֹͣ��ת
    else
    {ServerTask_MonitorTimer[1] ++; }
    
    if(GprsTask_MonitorTimer > 600)
    {TaskLose = 1; }   //Gprsͨ�� ����ֹͣ��ת
    else
    {GprsTask_MonitorTimer ++; }
    
    if(ManualTask_MonitorTimer > 20)
    {TaskLose = 1;}   //�����ֱ� ����ֹͣ��ת
    else
    {ManualTask_MonitorTimer ++; }
    
     if(0 == TaskLose )
     {IWDG_ReloadCounter( );} //��λ���Ź�}
    
     PCF8563_Read_time(&SystemTimer);
    
    if((0 != ServerState[0].SR.Bit.ACK)||(0 != ServerState[1].SR.Bit.ACK))
    {GLED_ON();}
    else
    {GLED_OFF();}
    
    if(OverLoad_timer[0] > 0)
    {OverLoad_timer[0]--;}
    
    if(OverLoad_timer[1] > 0)
    {OverLoad_timer[1]--;}
    
    if(OverLoad_timer[2] > 0)
    {OverLoad_timer[2]--;}
    
    if(OverLoad_timer[3] > 0)
    {OverLoad_timer[3]--;}
    
    
    
    if(FullReadingTime >= _188_ConcenConfigData.timer.ReadingCycle.Word * 60 * 100)
    {
      if(ReadingMeterState != FullReading)
      {
        ReadingMeterState = FullReading;
        FullReadingTime = 0;
      }
      
    }
    
     //vin = getVin ( );
   // FRAM_BufferRead(frmbuff,64*2,176); 
    //itemp = 0;
   /// while (itemp < 64)
    //{
   //   wfrmbuff[itemp] = frmbuff[itemp] + 1;
   //   itemp++;
   // }
  // FRAM_BufferWrite(wfrmbuff,64*2,176);
    //if(0 != gprs.cip[0].CSR.state)
    //{_CIPSEND_func(0, "abcdefghijklmnopq", 10);}
    /* if(pdPASS == xQueueReceive(GprsQueue, &InputBuff, 0))
    { 
    GprsData_length = GetDataFromUsart (InputBuff,GprsRecveBuff, GPRS_GprsBuffLength);
    GprsRecveBuff[GprsData_length] = '\0';
    Display_func(GprsRecveBuff);
  }*/ 
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/ 
static void Led1_Task ( void* Param )
{
   while(1)
   {
    RLED_OFF ( ); 
    vTaskDelay(30);
    RLED_ON ( ); 
    vTaskDelay(30);
    ReadTask( );
  //  PatrolTask( );
   }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/ 
//static void Led2_Task ( void* Param )
//{
//  while(1)
 //  {
  //  GLED_ON ( ); 
  //  vTaskDelay(10);
  //  GLED_OFF( ); 
  //  vTaskDelay(10);
  // }
//}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/ 
void vApplicationTickHook( void )
{
  static unsigned  short OverLoad_CN  = 0;
  
  FullReadingTime  ++;
  SilenceTimer[0]++;
  SilenceTimer[1]++;  
  if(0 != HVPower_state )
  {
    if(0  == (Mbus_OverLoadpin))
    {
      OverLoad_CN ++;
    }
    else
    {OverLoad_CN = 0;}
  }
  else{OverLoad_CN = 0;}
 
  if(OverLoad_CN > 6)
  {
    HVPower_OFF( );
    if(0 == Mbus_CH1_State )
    {
      OverLoad_timer[0] = 100;
      OverLoadFlg[0].Byte = 0xFF;
    }
    if(0 == Mbus_CH2_State )
    {
      OverLoad_timer[1] = 100;
      OverLoadFlg[1].Byte = 0xFF;
    }
    if(0 == Mbus_CH3_State )
    {
      OverLoad_timer[2] = 100;
      OverLoadFlg[2].Byte = 0xFF;
    }
    if(0 == Mbus_CH4_State )
    {
      OverLoad_timer[3] = 100;
      OverLoadFlg[3].Byte = 0xFF;
    }
    CloseAllMbus ( );
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/ 
void get_ErrInformation (Server188_Err_Param_Stuct*  err)
{
  err->ErrType = NoErr;
  if((0 != OverLoadFlg[0].Bit.n0)
     ||(0 != OverLoadFlg[1].Bit.n0)
       ||(0 != OverLoadFlg[2].Bit.n0)
         ||(0 != OverLoadFlg[3].Bit.n0))
  {
    err->ErrType = MubusOverLoad;
    if(0 != OverLoadFlg[0].Bit.n0)
    {
      err->ErrParam= 1;
      OverLoadFlg[0].Bit.n0 = 0;
    }
    else if(0 != OverLoadFlg[1].Bit.n0)
    {
      err->ErrParam= 2;
      OverLoadFlg[1].Bit.n0 = 0;
    }
    else if(0 != OverLoadFlg[2].Bit.n0)
    {
      err->ErrParam= 3;
      OverLoadFlg[2].Bit.n0 = 0;
    }
    else
    {
      err->ErrParam= 4;
      OverLoadFlg[3].Bit.n0 = 0;
    }
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/ 
/*
BaseType_t getRs485(TaskHandle_t  Task)
{
  BaseType_t itemp = pdFALSE;
  
  if(Task == ManualTask)
  {
    if(Rs485ToManual_ReceiveQueue != Rs485_ReceiveQueue)
    {
      if( pdTRUE == xSemaphoreTake (Rs485_SendMutex, 80)) 
      {
        Rs485_ReceiveQueue =  Rs485ToManual_ReceiveQueue;
        itemp = pdTRUE;
      }
    }
    else
    {itemp = pdTRUE;}
  }else if(ServerTask == Task)
  {
    if(Rs485ToServer_ReceiveQueue != Rs485_ReceiveQueue)
    {
      if(pdTRUE == xSemaphoreTake (Rs485_SendMutex, 80)) 
      {
        Rs485_ReceiveQueue =  Rs485ToServer_ReceiveQueue;
        itemp = pdTRUE;
      }
    }
    else
    {itemp = pdTRUE;}
  }else if(SacnTask == Task)
  {
    if(Rs485ToSacn_ReceiveQueue != Rs485_ReceiveQueue)
    {
      if(pdTRUE == xSemaphoreTake(Rs485_SendMutex, 80)) 
      {
        Rs485_ReceiveQueue =  Rs485ToSacn_ReceiveQueue;
        itemp = pdTRUE;
      }
    }
    else
    {itemp = pdTRUE;}
  }
  else
  {
    itemp = pdFALSE;
    Rs485_ReceiveQueue =  NULL;
  }
  return itemp;
}
*/
/*********************************************************************************
Function:      //
Description:   // �ȴ�������� 
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
void weitPortIdle (CommPortNumber_Type port, unsigned short  _T10ms)
{
  while(Idle != getState_USART(port))
  {
    vTaskDelay( _100ms );
    if(_T10ms-- > 0)
    {
      setState_USART(port, Idle);
      break;
    }
  }
}
/**************************************END****************************************/



