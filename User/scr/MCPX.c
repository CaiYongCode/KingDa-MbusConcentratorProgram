/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\MCPX.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
/*********************************************************************************
公共变量定义区
*********************************************************************************/
//unsigned short              Concentrator_DebugTimer  = 0;     //调试模式倒计时器 计时器时间为零则退出调试模式
ServerState_Struct            ServerState[2];    //集中器状态
AutoCollectCycleStruct        ConcentratorUpdataTimer ={99,99};     //集中器 上传时间

/*********************************************************************************
外部变量声明区
*********************************************************************************/

extern ConcentratorParameterStruct   Concentrator;
/*********************************************************************************
私有变量定义区
*********************************************************************************/ 
//static MCPX_InputStruct  Input;
static MCPX_OutputStruct  MCPX_Output[SPCP_OutputBuff_Number];
/*********************************************************************************
测试变量定义区
*********************************************************************************/

/*********************************************************************************
内部函数定义区
*********************************************************************************/
//static SystemErrName getInputData (void);
//static void GetDataForBuff_GPRS (MCPX_InputStruct *buff);
/*********************************************************************************
功能代码定义区
*********************************************************************************/
void MCPX_DeInit(void)
{
  DeInitOutputBuff_MXPC( );
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
SystemErrName MXPC_SendSingle(CommPortNumber_Type port,
                              void * pbuff, 
                              unsigned short length,
                              SerialPort_SendEndCallBack_Type callBack,
                              void *callBack_arg  )
{
  SystemErrName err = NO_ERR;
  switch (port)
  {
  case Rs485_Port1:
  case Rs485_Port2:
  case Rs485_Port3:
  case Rs485_Port4:  
    { 
      if( port != getChannel_Rs485( ))
      {
        changeChannel_Rs485 ( port );
        err = PortBusy;
        break;  
      }
    }
  case Rs232_Port:
    {
      err =sendBuffByDMA_USART (port ,pbuff, length,callBack, callBack_arg);  
      break; 
    }
  case GPRS_CH1:
  case GPRS_CH2:
  case GPRS_CH3:
  case GPRS_CH4:  
    {
      //err = TCP_SendStart_MG2618(port-GPRS_Port,pbuff,length,callBack,callBack_arg);
      break; 
    }
  case ErthNet_Port:
    {break; }
  default :
    {return IllegalParam;}
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
void MXPC_Retrans(TimerHandle_t pxTimer )
{
  SystemErrName err = NO_ERR;
  MCPX_OutputStruct*  OutputBuff;
  
  //configASSERT( pxTimer );

  OutputBuff =   pvTimerGetTimerID( pxTimer );
 if(OutputBuff->CCR.Bit.Retrans_PV >0)
 {
   err = MXPC_SendSingle(OutputBuff->OutPort, 
                     OutputBuff->buff,  
                     OutputBuff->length,
                     OutputBuff->SendEndCallBack,
                     OutputBuff->buff); 
   
   if((GPRS_CH1 ==  OutputBuff->OutPort)
      ||(GPRS_CH2 ==  OutputBuff->OutPort)
        ||(Rs232_Port ==  OutputBuff->OutPort))
   { 
     if(NO_ERR == err)
     {OutputBuff->CCR.Bit.Retrans_PV --;}
   }
   else
   {OutputBuff->CCR.Bit.Retrans_PV --;}  
 }
 else
 {
  if(P_NULL !=  OutputBuff->RetransEndCallBack)
  {OutputBuff->RetransEndCallBack(OutputBuff->buff);}
  FreeOutputBuffForData_MXPC (OutputBuff->buff);
 }
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
SystemErrName MXPC_Send_AutoRetrans (CommPortNumber_Type  outport,   //目的端口
                                     CommPortNumber_Type  inport,
                                     void * pbuff,                   //数据缓冲区
                                     unsigned short length,          //数据长度
                                     unsigned char retrans,          //重传次数
                                     unsigned short cycle,           //重传周期数
                                     SerialPort_SendEndCallBack_Type  SendEnd_callback,     //发送完成回调
                                     MCPX_RetransEndCallBack_Type     RetransEnd_callback)  //重发完成回调
{
  SystemErrName err = NO_ERR;
  MCPX_OutputStruct*  OutputBuff;
  err = MXPC_SendSingle(outport,pbuff,length,SendEnd_callback,pbuff);
  if(IllegalParam == err){return err;}
  if(NO_ERR == err) 
  {if((0  == retrans)&&(0 == cycle )){return NO_ERR;}}
  else
  {
    if(Port_OFF == err)
    {
      if((Rs485_Port1 == outport)
         ||(Rs485_Port2 == outport)
           ||(Rs485_Port3 == outport)
             ||(Rs485_Port4 == outport))
      {changeChannel_Rs485( outport);}
    }
    if(0  == retrans)
    {
      retrans = 1;
      cycle = 50;
    }
    else
    { retrans ++;}
  }
  OutputBuff = GetIdleOutputBuff_MXPC ( );
  if(P_NULL == OutputBuff)
  { return Nothing;}
  OutputBuff->RetransTimer = xTimerCreate("Timer", /* 定时器名字 */
                                          cycle, /* 定时器周期,单位时钟节拍 */
                                          pdTRUE, /* 周期性 */
                                          OutputBuff, /* 定时器 ID */
                                          MXPC_Retrans); /* 定时器回调函数 */
  if(NULL == OutputBuff->RetransTimer)
  {
    FreeOutputBuff_MXPC(OutputBuff);
    return Nothing;
  } 
  OutputBuff->buff = pbuff;
  OutputBuff->length = length;
  OutputBuff->OutPort = outport;
  OutputBuff->SourcePort = inport;
  OutputBuff->CCR.Bit.Retrans_SV = retrans;
  OutputBuff->CCR.Bit.Retrans_PV = retrans;
  OutputBuff->SendEndCallBack  =     SendEnd_callback;
  OutputBuff->RetransEndCallBack  =  RetransEnd_callback;
  if(pdPASS !=  xTimerStart(OutputBuff->RetransTimer, 100))
  {
    FreeOutputBuff_MXPC(OutputBuff);
    return Nothing;
  }
  return NO_ERR;
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
void DeInitOutputBuff_MXPC(void)
{
  unsigned char itemp =0;    
  while ( itemp < SPCP_OutputBuff_Number)
  {
   MCPX_Output[itemp].CSR.Byte = 0; 
   MCPX_Output[itemp].CSR.Bit.idle = 1;
   itemp ++;
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
MCPX_OutputStruct* GetIdleOutputBuff_MXPC (void)
{
  unsigned char itemp =0;
  while ( itemp < SPCP_OutputBuff_Number)
  {
    if(1 == MCPX_Output[itemp].CSR.Bit.idle)
    {
     MCPX_Output[itemp].CSR.Bit.idle = 0;
     return &(MCPX_Output[itemp]);
    }
    itemp ++;
  }
  return  P_NULL;
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
MCPX_OutputStruct* GetOutputBuffForData_MXPC (void* pbuff)
{
  unsigned char itemp =0;
  while ( itemp < SPCP_OutputBuff_Number)
  {
    if(pbuff == MCPX_Output[itemp].buff)
    {return &(MCPX_Output[itemp]);}
    itemp ++;
  }
  return  P_NULL;
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
void FreeOutputBuff_MXPC(MCPX_OutputStruct* pbuff)
{
  if(P_NULL != pbuff)
  {
    if(P_NULL !=  pbuff->RetransTimer)
    //{SofTimerDel(&(pbuff->RetransTimer));}
    pbuff->CSR.Byte = 0; 
    pbuff->CSR.Bit.idle = 1;
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
void FreeOutputBuffForData_MXPC (void* pbuff)
{
  unsigned char itemp =0;
  while ( itemp < SPCP_OutputBuff_Number)
  {
    if((pbuff == MCPX_Output[itemp].buff)&&( 0 == MCPX_Output[itemp].CSR.Bit.idle)) 
    {
     if( xTimerIsTimerActive( MCPX_Output[itemp].RetransTimer ) != pdFALSE )
     {xTimerDelete(MCPX_Output[itemp].RetransTimer, 10 );}
      MCPX_Output[itemp].CSR.Byte = 0; 
      MCPX_Output[itemp].CSR.Bit.idle = 1;
    }
    itemp ++;
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
/*static void GetDataForBuff_GPRS (MCPX_InputStruct *buff)
{
 unsigned char ch;
 USART2_BuffStruct* package;
 static DataBuffSpec_Struct Data ;
 package = GetDataForBuff_USART2 ( );
 if(P_NULL == package)
 {buff->length = 0;return ;}
 Data.length = package->length;
 Data.pbuff = package->data;
 Decode_MG2618(&ch,&Data);
 if(( ch >0 )&&(ch < 5))
 {
  buff->length = Data.length; 
  buff->Port = GPRS_Port;
  buff->Port += ch; 
  while(Data.length -- )
  {buff->buff[Data.length] = Data.pbuff[Data.length];}
 }
 FreeBuff_USART2(package);  
}*/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
CommPortNumber_Type getServerPort( unsigned char Server_N0)
{ 
 if(1 ==Server_N0 ) 
 {return ServerState[0].Port;}
 else if(2 ==Server_N0 ) 
 {return ServerState[1].Port;}
 return Null_Port;
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
/*void ServerConnect_OK(unsigned char ServerNo, CommPortNumber_Type  Port)
{
  if((ServerNo == 1)||(2 == ServerNo))
  {
    ServerNo --;
    ServerState[ServerNo].SR.Bit.CN = 1;
    ServerState[ServerNo].SR.Bit.IF = 1;
    ServerState[ServerNo].SR.Bit.RC = 0;
    ServerState[ServerNo].SR.Bit.ST = 0; 
    ServerState[ServerNo].SR.Bit.GZ = 0;
    ServerState[ServerNo].Port =  Port;
    SofTimerDel(&(ServerState[ServerNo].Time));
  }
}*/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
/*static void ServerConnect(Command_MG2618 cmd, DataBuffSpec_Struct *AckData)
{
 unsigned char itemp ;
 CommPortNumber_Type Port ;
 GprsInfr_Struct  GprsInfr;
 
 if(ZIPSETUP == cmd )
 {
  if(P_NULL!= AckData)
  {
   StopListen_MG2618( );   
   if(CompString(AckData->pbuff,"CONNECTED",P_NULL)
     ||CompString(AckData->pbuff,"ESTABLISHED",P_NULL))
   {
     GprsInfr.IP[0] = MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[0];
     GprsInfr.IP[1] = MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[1];
     GprsInfr.IP[2] = MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[2];
     GprsInfr.IP[3] = MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[3];
     GprsInfr.PORT = MG2618Infr.Command.CmdPram.TcpUdp_Connect.PORT;
     GprsInfr.LOCAL_PORT = MG2618Infr.Command.CmdPram.TcpUdp_Connect.LOCAL_PORT;
     GprsConnect_OK(MG2618Infr.Command.CmdPram.TcpUdp_Connect.CH, &GprsInfr);
     itemp = 0;
     if((MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[0] == Concentrator.server1.ip[0])
        &&(MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[1] == Concentrator.server1.ip[1])
          &&(MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[2] == Concentrator.server1.ip[2])
            &&(MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[3] == Concentrator.server1.ip[3])
              &&(MG2618Infr.Command.CmdPram.TcpUdp_Connect.PORT == Concentrator.server1.port)) 
     { itemp = 1;}
     else if((MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[0] == Concentrator.server2.ip[0])
             &&(MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[1] == Concentrator.server2.ip[1])
               &&(MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[2] == Concentrator.server2.ip[2])
                 &&(MG2618Infr.Command.CmdPram.TcpUdp_Connect.IP[3] == Concentrator.server2.ip[3])
                   &&(MG2618Infr.Command.CmdPram.TcpUdp_Connect.PORT == Concentrator.server2.port)) 
     {itemp = 2;}
     Port  = GPRS_Port;
     Port += MG2618Infr.Command.CmdPram.TcpUdp_Connect.CH;
     ServerConnect_OK(itemp,Port);
   }
   else
   {freeChannel_MG2618(MG2618Infr.Command.CmdPram.TcpUdp_Connect.CH);}
  }
  else
  {freeChannel_MG2618(MG2618Infr.Command.CmdPram.TcpUdp_Connect.CH);}
 }
 StopListen_MG2618( );
}*/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
/*static void Reconnec(void *p_tmr, void *p_arg)
{
 unsigned char ServerNo;  
 CommandDescriptor_MG2618  cmdbuff; 
 
 if(P_NULL == p_arg )
 {
  SofTimerDel ((SofTimer_Struct**)&p_tmr);
  BreakServerAction(1);
  BreakServerAction(2); 
  Reboot_MG2618(P_NULL); //重启模块
  return;
 } 
 ServerNo = *(( unsigned char* )p_arg);
 if((1 == ServerNo)||(2 == ServerNo))
 {
   if(p_tmr != ServerState[ServerNo-1].Time)
   {SofTimerDel((SofTimer_Struct**)&p_tmr);}
 
   if(ServerState[ServerNo-1].ServerConnect_CNT >= 5)
   { 
    SofTimer_ChangePeriod ( p_tmr,           //操纵的定时器
                            GRPS_PauseTime,  //新的周期
                           1);             //是否复位定时器 1:重新计时  0:继续计时    
    ServerState[ServerNo-1].ServerConnect_CNT = 0;  
    ServerState[ServerNo-1].SR.Bit.GZ = 1;
   }
   else
   {
     SofTimer_ChangePeriod ( p_tmr,           //操纵的定时器
                            GRPS_ReconnectionTime,  //新的周期
                           1);             //是否复位定时器 1:重新计时  0:继续计时 
   }
   
   if((0 != ServerState[0].SR.Bit.GZ)
      &&(0 != ServerState[1].SR.Bit.GZ))
   {
     BreakServerAction(1);
     BreakServerAction(2);
     Reboot_MG2618(P_NULL); //重启模块
     return;
   }
 }
 else
 {
   SofTimerDel ((SofTimer_Struct**)&p_tmr); 
   return;
 }

 if(1 == ServerNo)   
 {
   if((0 != Concentrator.server1.ip[0])
      ||(0 != Concentrator.server1.ip[1])
        ||(0 != Concentrator.server1.ip[2])
          ||(0 != Concentrator.server1.ip[3]))
   {
     cmdbuff.Cmd = ZIPSETUP;
     cmdbuff.Format = Param;
     cmdbuff.CmdPram.TcpUdp_Connect.LOCAL_PORT = 0;
     cmdbuff.CmdPram.TcpUdp_Connect.CH = getIdleChannel_MG2618( ); 
     cmdbuff.CmdPram.TcpUdp_Connect.IP[0] = Concentrator.server1.ip[0];       //112;
     cmdbuff.CmdPram.TcpUdp_Connect.IP[1] = Concentrator.server1.ip[1];       //25;
     cmdbuff.CmdPram.TcpUdp_Connect.IP[2] = Concentrator.server1.ip[2];       //204;
     cmdbuff.CmdPram.TcpUdp_Connect.IP[3] = Concentrator.server1.ip[3];       //3;  
     cmdbuff.CmdPram.TcpUdp_Connect.PORT = Concentrator.server1.port;         //50050
   }
   else
   {
    ServerState[0].Port = Null_Port;
    ServerState[1].SR.Byte = 0;
    ServerState[0].ServerConnect_CNT = 0;
    SofTimerDel(&(ServerState[0].Time));
    SofTimerDel ((SofTimer_Struct**)&p_tmr); 
    return; 
   }
 }
 else if(2 == ServerNo)
 {
   if((0 != Concentrator.server2.ip[0])
      ||(0 != Concentrator.server2.ip[1])
        ||(0 != Concentrator.server2.ip[2])
          ||(0 != Concentrator.server2.ip[3]))
   {
     cmdbuff.Cmd = ZIPSETUP;
     cmdbuff.Format = Param;
     cmdbuff.CmdPram.TcpUdp_Connect.LOCAL_PORT = 0;
     cmdbuff.CmdPram.TcpUdp_Connect.CH = getIdleChannel_MG2618( ); 
     cmdbuff.CmdPram.TcpUdp_Connect.IP[0] = Concentrator.server2.ip[0];       //112;
     cmdbuff.CmdPram.TcpUdp_Connect.IP[1] = Concentrator.server2.ip[1];       //25;
     cmdbuff.CmdPram.TcpUdp_Connect.IP[2] = Concentrator.server2.ip[2];       //204;
     cmdbuff.CmdPram.TcpUdp_Connect.IP[3] = Concentrator.server2.ip[3];       //3;  
     cmdbuff.CmdPram.TcpUdp_Connect.PORT = Concentrator.server2.port;  //50050
   }
   else
   {
    ServerState[1].Port = Null_Port;
    ServerState[1].SR.Byte = 0;
    ServerState[1].ServerConnect_CNT = 0;
    SofTimerDel(&(ServerState[1].Time));
    SofTimerDel ((SofTimer_Struct**)&p_tmr); 
    return; 
   }
 }
 else //参数不合法
 { 
   SofTimerDel ((SofTimer_Struct**)&p_tmr); 
   return;
 }
 if(NO_ERR == SendCmd_MG2618(&cmdbuff,ServerConnect,0,6000))
 {
  if(ServerState[ServerNo-1].ServerConnect_CNT < 100)
  {ServerState[ServerNo-1].ServerConnect_CNT ++;}
  return;
 }
 else
 {freeChannel_MG2618(cmdbuff.CmdPram.TcpUdp_Connect.CH); }
 

   
} */
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
/*void ServerClose_UEV(unsigned char ServerNo)
{
  if((1 == ServerNo) || (2 == ServerNo))
  {
    ServerNo --;
    if(0 == ServerState[ServerNo].SR.Bit.RC)
    {
      BreakServerAction( ServerNo + 1 );  
      ServerState[ServerNo].Time = SofTimerCreate(GRPS_ReconnectionTime, //周期
                                                  TimerTB_1M,            //时基
                                                  SofTimer_PERIODIC,     //模式
                                                  Reconnec,      //回调
                                                 (void *)&(Integer[ServerNo + 1]));   //回调参数
      if(P_NULL != ServerState[ServerNo].Time )
      {
        ServerState[ServerNo].SR.Bit.RC = 1; 
        SofTimerStart(ServerState[ServerNo].Time); 
      }
    }
  }
}*/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
//void ReconnecServer( unsigned char ServerNo)
//{
//  BreakServerAction(ServerNo);
 // Reconnec(P_NULL, (void *)&(Integer[ServerNo]));
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
/*void BreakServerAction(unsigned char ServerNo)
{
  unsigned char itemp;
  if((1 == ServerNo)||(2 == ServerNo))
  {
    ServerNo --;
    ServerState[ServerNo].SR.Byte = 0;
    ServerState[ServerNo].ServerConnect_CNT = 0;  
    SofTimerDel(&(ServerState[ServerNo].Time)); 
    
    if((GPRS_CH1 == ServerState[ServerNo].Port)
      ||(GPRS_CH2 == ServerState[ServerNo].Port)
        ||(GPRS_CH3 == ServerState[ServerNo].Port)
          ||(GPRS_CH4 == ServerState[ServerNo].Port))
    { 
      itemp = ServerState[ServerNo].Port - GPRS_Port;
      freeChannel_MG2618(itemp);
    }
    ServerState[ServerNo].Port = Null_Port;
  }
}*/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
/*void  CommPortClose_UEV (CommPortNumber_Type  Port)
{
 if(ServerState[0].Port == Port) 
 {
  ServerClose_UEV(1);
 }
 if(ServerState[1].Port == Port) 
 {
  ServerClose_UEV(2);
 }
}*/
/******************************************END********************************************************/

