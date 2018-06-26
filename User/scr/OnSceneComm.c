/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\OnSceneComm.h"
#include ".\Server188.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
/*********************************************************************************
公共变量定义区
*********************************************************************************/
ServerBuff_struct     ServerRecBuff[2][ServerReceiveNO];  
ServerState_Struct    ServerState[2];
unsigned long       SilenceTimer[2] = {0,0};
extern unsigned char ReportAllCurrentDataFinish;
/*********************************************************************************
外部变量声明区
*********************************************************************************/
//e//xtern ServerState_Struct    ServerState[2];
//extern ConcentratorParameterStruct   Concentrator;
/*********************************************************************************
私有变量定义区
*********************************************************************************/
/*********************************************************************************
测试变量定义区
*********************************************************************************/
  
/*********************************************************************************
内部函数定义区
*********************************************************************************/
static void ServerInit (unsigned char ServerNO);
/*********************************************************************************
功能代码定义区
*********************************************************************************/
void get_ErrInformation (Server188_Err_Param_Stuct*  err);
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        // 
Others:         // 
*********************************************************************************/
SystemErrName Comm_send(void* Data, 
                        unsigned long DataLength, 
                        CommPortNumber_Type TargetPort,
                        SerialPort_SendEndCallBack_Type callBack, 
                        void *callBack_arg )
{
  SystemErrName err = NO_ERR; 
  
  switch(TargetPort)
  {
  case Mbus_Port1:
  case Mbus_Port2:
  case Mbus_Port3:
  case Mbus_Port4:
    {
      sendBuffByDMA_USART (TargetPort,
                           Data, 
                           DataLength, 
                           callBack, 
                           callBack_arg);
      if( 0 != MbusMonitor_EN )
      {TargetPort = Rs232_Port;}
      else{ break;}
    }
  case Rs232_Port:
    {
      weitPortIdle (TargetPort, 100);      
      sendBuffByDMA_USART (TargetPort,
                           Data, 
                           DataLength, 
                           callBack, 
                           callBack_arg);
      break;
    }  
  case GPRS_CH1:
  case GPRS_CH2:
  case GPRS_CH3:
  case GPRS_CH4:
  case GPRS_CH5:  
    {
      err = GprsIP_SendFunc(TargetPort - GPRS_CH1, Data, DataLength);
      break;
    }   
  default:
    { 
      err = CmdValid;
      break;
    }
  }
  return err;
}                            
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        // 
Others:         // 
*********************************************************************************/
static SystemErrName  HandServer(unsigned char ServerNO,  unsigned char MaxNO)
{
  SystemErrName   err;
  unsigned char  itemp = 0;
  unsigned char  DataLength;
  ServerBuff_struct*  ServerData;
  Server188_Frame_Struct*  SendBuff;

  do{
    SendBuff = CreateHandshakeFrame(&DataLength);
    if(P_NULL == SendBuff)
    {
      if(itemp++ > 180)
      {return Nothing;}
      else
      {vTaskDelay(_1s);}
    }
    else
    { break;}  
  }while (1) ;
 
  while (MaxNO > 0)
  {
    err = Comm_send( SendBuff, DataLength, ServerState[ServerNO].Port,P_NULL, P_NULL);
    ServerState[ServerNO].SR.Bit.ACK = 0;
    if(NO_ERR == err)
    {
      MaxNO --;  
      itemp = 0;
      while(itemp ++ < 30)
      {
        if(pdPASS == xQueueReceive(ServerQueue[ServerNO],&ServerData, _1s * 1))
        {
          if(NO_ERR == Server188Hand_AckVerify(ServerData->data, ServerData->DataLength))
          {
            ServerState[ServerNO].SR.Bit.ACK = 1;
            SilenceTimer[ServerNO] = 0;
          }
          ServerData->CSR.Bit.idle = 1;
        }
        if(0 != ServerState[ServerNO].SR.Bit.ACK )
        {break;}
      }
    }
    else if(PortBusy == err)
    {vTaskDelay(_1s * 3);}
    else 
    {break;}
    
    if(0 != ServerState[ServerNO].SR.Bit.ACK )
    {break;}
  }
  
  freeServer188SendBuff(SendBuff);
  
  if(0 != ServerState[ServerNO].SR.Bit.ACK )
  {return NO_ERR;}
  
  return Unsuccessful;
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
static void  Reconnection (unsigned char ServerNO)
{
  if(0 == gprs.CSR.Bit.ready)
  {vTaskDelay(_1s * 3);}
  else
  {
    if(0 == connectServer (ServerNO ))
    { 
      if(++ServerState[ServerNO].Connect_CNT >3)
      {ServerState[ServerNO].SR.Bit.GZ = 1;}
      else
      {ServerState[ServerNO].SR.Bit.GZ = 0;}
      vTaskDelay(_1Minute * 1);
    }
    else if (NO_ERR != HandServer(ServerNO,  3))//握手服务器 最多 3 次 握手不上则断开服务器
    {
     closeServer (ServerNO);
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
void Server_Task (void* Param )
{
  unsigned char ServerNO;
  ServerBuff_struct*  ServerData;
  Server188_Err_Param_Stuct  Err; 
  
  if('1' == *(unsigned char*)Param)
  {ServerNO = 0;}
  else if('2' == *(unsigned char*)Param)
  {ServerNO = 1;}
  else
  {vTaskDelete(NULL);}
  
  ServerInit (ServerNO);
  while(1)
  { 
    
    if(0 == ServerState[ServerNO].SR.Bit.CN)
    {
      SilenceTimer[ServerNO] = 0;
      Reconnection (ServerNO);   
    }
    else
    {
      //如果抄表完成，且不在上报期间，则引导集中器上报所有表计当前水量
      if( ReadingMeterState == ReadingFinish) 
      {
        ReadingMeterState = DownIdle;
        
        if(ReportAllCurrentDataFinish == 1)
        {
          GuideReportAllCurrentData();
        }
      }
      
      if(pdPASS == xQueueReceive(ServerQueue[ServerNO], &ServerData, _1s *2))
      {
        Server188_InPut (ServerData->data, ServerData->DataLength, ServerState[ServerNO].Port);
        ServerData->CSR.Bit.idle = 1;
        SilenceTimer[ServerNO] = 0;
      }
      else
      { 
        get_ErrInformation(&Err);
        if(NoErr != Err.ErrType)
        {Server188_SendErr (ServerState[ServerNO].Port, &Err);}
        else if(SilenceTimer[ServerNO] >= _188_ConcenConfigData.gprs.BeatCycle * 60 * 100)
        {
          SilenceTimer[ServerNO] = 0;
          if(NO_ERR != HandServer(ServerNO,  3))//握手服务器 最多3次 握手不上则断开服务器
          {closeServer (ServerNO);}
        }
      }
    }
    ServerTask_MonitorTimer[ServerNO] = 0;   //服务器 1 任务监视看门狗定时器 复位
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
static void ServerInit (unsigned char ServerNO)
{
  unsigned short itemp;
  unsigned char *p;
  itemp = 0;
  while (itemp < ServerReceiveNO)
  {
    ServerRecBuff[ServerNO][itemp].CSR.Bit.idle = 1;
    itemp ++;
  }
  
  itemp = 0;
  p = (unsigned char *)&ServerState[ServerNO];
  while(itemp < sizeof(ServerState_Struct))
  {p[itemp++] = 0x00;}
  if(0== ServerNO)
  {
    if((0 != _188_ConcenConfigData.gprs.IP[0]) 
       &&(0 != _188_ConcenConfigData.gprs.IP[1]) 
         &&(0 != _188_ConcenConfigData.gprs.IP[2])
           &&(0 != _188_ConcenConfigData.gprs.IP[3]))
    {
      if(0 != _188_ConcenConfigData.gprs.Port.Word)
      {ServerState[ServerNO].SR.Bit.EN = 1; }
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
unsigned char WriteDtatToServerRecBuff(unsigned char ServerNO,  char* data, unsigned short length)
{
  unsigned short itemp =0;
  ServerBuff_struct* buff = P_NULL;
  while (itemp < ServerReceiveNO)
  {
   if(ServerRecBuff[ServerNO][itemp].CSR.Bit.idle == 1)
   {
     ServerRecBuff[ServerNO][itemp].CSR.Bit.idle = 0;
     buff = &(ServerRecBuff[ServerNO][itemp]);
     break;
   }
   itemp ++;
  }
  if(P_NULL == buff)
  {return 0;}  
  if(length <= ServerReceiveBuffLength)
  {
    itemp = 0;
    while(itemp < length)
    {
      buff->data[itemp] = data[itemp];
      itemp ++;
    }
   buff->DataLength = length;

   if(pdPASS != xQueueSend(ServerQueue[ServerNO],&buff,0))
   {buff->CSR.Bit.idle = 1;}
   return 1; 
  }
  return 0;
}
/**************************************END****************************************/