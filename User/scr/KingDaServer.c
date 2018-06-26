/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include ".\KingDaServer.h"
#include ".\KingDaV1.h"
#include ".\CRC8.h"
/*********************************************************************************
常量定义区
*********************************************************************************/

/*********************************************************************************
公共变量定义区
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/
extern ConcentratorParameterStruct   Concentrator;
extern ServerState_Struct            ServerState[2];
/*********************************************************************************
私有变量定义区
*********************************************************************************/ 
static KingDaServerSendBuff_Struct  send_buff[KingDaServerSendBuff_Number];
static CommPortNumber_Type InPort_PV;
/*********************************************************************************
测试变量定义区
*********************************************************************************/
/*********************************************************************************
内部函数定义区
*********************************************************************************/
static void DeInitBuff(void);
static SystemErrName FrameVerify (void *pbuff,unsigned char length, KingDaServer_PackageStruct** data);
static void HandCmd_func(KingDaServer_PackageStruct* pbuff);
static void ReadMeasureValue_func(KingDaServer_PackageStruct* pbuff);
static void TuneUp_RTC_func(KingDaServer_PackageStruct* pbuff);
static void GetRepeatersState_func(KingDaServer_PackageStruct* pbuff);
static void ReadALLMeasureValue_func(KingDaServer_PackageStruct* pbuff);
static void MeasureValue_UpLoad_func(KingDaServer_PackageStruct* pbuff);

void MeasureValue_ToKingDaServer(Repeaters_DescrType* repeaters, unsigned char *pbuff);

/*********************************************************************************
功能代码定义区
*********************************************************************************/
void KingDaServer_DeInit(void)
{
  InPort_PV = Null_Port;
  DeInitBuff( );
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
SystemErrName SendCmd_KingDaServer(KingDaServer_Struct* Cmd)                         
{
  unsigned char i;
  unsigned char* tc;
  unsigned char* tp;
  KingDaServerSendBuff_Struct*  Buff;
  Buff = Get_KingDaServerBuff ( );
  if(P_NULL  == Buff)
  {return Nothing;}
  Buff->AckFunc = Cmd->AckFunc;
  if(UP == Cmd->CSR.Bit.DIR)
  {Buff->buff.head.Sync= KingDaServerSync_ToServer;}
  else
  {Buff->buff.head.Sync= KingDaServerSync_ToConcentrator;}
  Buff->buff.head.Cmd = Cmd->Cmd;
  Buff->buff.head.Length_L =  Cmd->length;
  Buff->buff.head.Length_H =  Cmd->length  >> 8; 
  tp = (unsigned char*) &(Buff->buff.pram);
  tc = Cmd->Param;
  i =0;
  while(i < Cmd->length)
  {
    tp[i] = tc[i];
    i++;
  }
  KingDaServer_BuffSend (Buff);
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
CommPortNumber_Type getServerPort(unsigned char ServerNO)
{
  if((0 != ServerNO)&&(1 != ServerNO))
  {return Null_Port;}
  return ServerState[ServerNO].Port;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        // 1：长度错误
                   2: 帧头同步错误
                   3：同步字错误
                   4：CRC 校验错误 
                   5: 不识别命令
Others:         // 
*********************************************************************************/
SystemErrName KingDaServer_input (void *pbuff, 
                                  unsigned char length, 
                                  CommPortNumber_Type port)
{
 SystemErrName err;
 KingDaServer_PackageStruct *package;
 InPort_PV  = port;
 err =  FrameVerify (pbuff, length, &package); 
 
 if(NO_ERR != err)
 {return err;}
 
 if(Rs232_Port == port)
 {
   Concentrator.CSR.Bit.Debug  = 1;
   //Concentrator_DebugTimer = 300;
 }
 switch( package->head.Cmd )
 {
  case HandCmd_Server:
   {
     HandCmd_func (package); 
     break;
   }
  case ReadMeasureValue:
  case ReadMeasureValue_OFF:  
   {
     ReadMeasureValue_func (package);
     break;
   }
  case TuneUp_RTC:          
    {
      TuneUp_RTC_func (package);
      break;
    }
  case GetRepeatersState:  
    {
      GetRepeatersState_func (package);
      break;
    }
  case ReadALLMeasureValue:
    {
      ReadALLMeasureValue_func (package);
      break;
    }
  case MeasureValue_UpLoad:   
    {
      MeasureValue_UpLoad_func(package);
      break;
    } 
  default:
   {return OperateCmd_err;break;}
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
static void DeInitBuff(void)
{
  unsigned char itemp =0;
  while ( itemp < KingDaServerSendBuff_Number)
  {
   send_buff[itemp].CCR.Byte = 0; 
   send_buff[itemp].CCR.Bit.idle = 1;
   itemp ++;
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
static SystemErrName BuffVerify (KingDaServerSendBuff_Struct* pbuff) 
{
 unsigned char itemp = 0;
 while (itemp < KingDaServerSendBuff_Number)
 {
   if(pbuff == &(send_buff[itemp]))
   {
     return NO_ERR;
   }
   itemp ++;
 }
 return IllegalParam;
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
KingDaServerSendBuff_Struct* Get_KingDaServerBuff (void)
{
  unsigned char itemp =0;
  while ( itemp < KingDaServerSendBuff_Number)
  {
    if(send_buff[itemp].CCR.Bit.idle)
    {
     send_buff[itemp].CCR.Byte = 0; 
     send_buff[itemp].RetransNumber  = RetransNumber_ToServer_Default;
     send_buff[itemp].AckFunc = P_NULL;
     send_buff[itemp].TargetPort = Null_Port;
     //send_buff[itemp].ServerON = 0;
     return &(send_buff[itemp]);
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
void Free_KingDaServerBuff(KingDaServerSendBuff_Struct* pbuff)
{
  if(NO_ERR == BuffVerify (pbuff))
  {
    //FreeOutputBuffForData_MXPC (&(pbuff->buff));
    pbuff->CCR.Byte = 0; 
    pbuff->CCR.Bit.idle = 1;
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        // 1：长度错误
                   2: 帧头同步错误
                   3：同步字错误
                   4：CRC 校验错误 
                   5: 不识别命令
Others:         // 
*********************************************************************************/
static SystemErrName FrameVerify (void *pbuff,unsigned char length, KingDaServer_PackageStruct** data)
{
 union {
   unsigned short* p;
   KindgDaServer_PackageHead  *headp;
 }buff;
 //unsigned char itemp;
 unsigned short Length;
   
 //itemp = 0;
 buff.p = pbuff;
 while (length > 0)
 {
   if((KingDaServerSync_ToServer  == *(buff.p))
      ||(KingDaServerSync_ToConcentrator == *(buff.p)))
   {break;}
  buff.p ++;
  length--;
 }
 if(length < sizeof(KindgDaServer_PackageHead))
 {return Length_err;}
  Length = buff.headp->Length_H;
  Length  <<= 8;
  Length += buff.headp->Length_L;
  Length += sizeof(KindgDaServer_PackageHead);
 if(length < Length + 1)
 {return Length_err;}

 if(((unsigned char*)buff.p)[Length] != CRC8((unsigned char*)buff.p,Length)) 
 {return Verify_err;}
 
 *data = (KingDaServer_PackageStruct*)buff.headp;
 return NO_ERR;
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
static void HandCmd_func(KingDaServer_PackageStruct* pbuff)
{
 unsigned char i;
 
 if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
 {
  i = 0;
  while (i < KingDaServerSendBuff_Number)
  {
   if((0 == send_buff[i].CCR.Bit.idle)
      && (send_buff[i].buff.head.Cmd == HandCmd_Server) 
      && (send_buff[i].TargetPort == InPort_PV)) 
   {
     if((pbuff->pram.HandsData_Receive.OK.id[0] == Concentrator.id[0]) 
        &&(pbuff->pram.HandsData_Receive.OK.id[1] == Concentrator.id[1])
        &&(pbuff->pram.HandsData_Receive.OK.id[2] == Concentrator.id[2]))
     {
       if(('O' == pbuff->pram.HandsData_Receive.OK.DI[0])
          &&('K' == pbuff->pram.HandsData_Receive.OK.DI[1]))
       {
         break;
       }  
     }
   }
   i ++;
  }
  if(i < KingDaServerSendBuff_Number)
  {
   if(P_NULL != send_buff[i].AckFunc)
   {send_buff[i].AckFunc(&(send_buff[i].buff), pbuff);} // 应答数据处理程序
   //else
   {Free_KingDaServerBuff(&send_buff[i]);}  //释放缓冲区 
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
Others:         // 
*********************************************************************************/
void ReadMeasureValue_AckFunc(PProtocolSendBuff_Struct* SendBuff, void* AckData)
{
 SystemErrName err = NO_ERR; 
 KingDaServerSendBuff_Struct*  buff;  
 KindgDaSPCP_PackageDataStruct* ACK;   //水表数据  应答数据 
 
 if(P_NULL == AckData ){return ;}
 ACK = AckData;
 
 buff =  Get_KingDaServerBuff ( ); 
 if(P_NULL == buff){return ;}
 buff->buff.head.Sync = KingDaServerSync_ToServer;
 buff->buff.head.Cmd = ReadMeasureValue;
 buff->buff.head.Length_L = sizeof(ReadMeasureValue_ToKingDaServer_Type);
 buff->buff.head.Length_H = sizeof(ReadMeasureValue_ToKingDaServer_Type)>> 8;
 buff->buff.pram.ReadMeasureValue_Send.Concentrator_ID[0] = Concentrator.id[0];
 buff->buff.pram.ReadMeasureValue_Send.Concentrator_ID[1] = Concentrator.id[1];
 buff->buff.pram.ReadMeasureValue_Send.Concentrator_ID[2] = Concentrator.id[2];
 buff->buff.pram.ReadMeasureValue_Send.Node_ID[0] = ACK->TypeA.pram.ReadValue_ack.addr[0];
 buff->buff.pram.ReadMeasureValue_Send.Node_ID[1] = ACK->TypeA.pram.ReadValue_ack.addr[1];
 buff->buff.pram.ReadMeasureValue_Send.Node_ID[2] = ACK->TypeA.pram.ReadValue_ack.addr[2];

 buff->buff.pram.ReadMeasureValue_Send.Value =  ACK->TypeA.pram.ReadValue_ack.value;
 buff->TargetPort = SendBuff->SourcePort;
 err = KingDaServer_BuffSend(buff);
 if(NO_ERR != err)
 {Free_KingDaServerBuff(buff);}
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
void ReadMeasureValue_OFF_AckFunc(PProtocolSendBuff_Struct* SendBuff, void* AckData)
{
 if(P_NULL == SendBuff ){return ;}
 changeChannel_Mbus( Null_Port);
 ReadMeasureValue_AckFunc( SendBuff, AckData);
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
static void ReadMeasureValue_func(KingDaServer_PackageStruct* pbuff)
{
 if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
 {
  if((pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
       &&(pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[1] == Concentrator.id[1])
       &&(pbuff->pram.ReadMeasureValue_Receive.Concentrator_ID[2] == Concentrator.id[2]))
  {
   if(ReadMeasureValue == pbuff->head.Cmd) 
   {
    ReadNodeValue_KindDa( pbuff->pram.ReadMeasureValue_Receive.Node_ID,
                          AutomatchPort, 
                          InPort_PV,
                          ReadMeasureValue_AckFunc);
   }
   else if(ReadMeasureValue_OFF == pbuff->head.Cmd) 
   {
    if(pbuff->head.Length_L >= sizeof(APPCON_ReadMeasureValue_ToKingDaConcentrator_Type)) 
    {
//      APPCON_CMD_SendFunc ( InPort_PV,
//                           pbuff->pram.ReadMeasureValue_Receive_APPCON.Node_ID,
//                           P_NULL,
//                           ReadNodeValue_APPCON_AckFunc); 
    }
    else
    {
     ReadNodeValue_KindDa( pbuff->pram.ReadMeasureValue_Receive.Node_ID,
                          AutomatchPort, 
                         InPort_PV,
                         ReadMeasureValue_OFF_AckFunc); 
      
    }
   }
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
Others:         // 
*********************************************************************************/
static void TuneUp_RTC_func(KingDaServer_PackageStruct* pbuff)
{
 SystemErrName err = NO_ERR;  
 TimerData_Struct time;
 KingDaServerSendBuff_Struct*  buff; 
 
 if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
 {
  if((pbuff->pram.TuneUpRTC_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
       &&(pbuff->pram.TuneUpRTC_Receive.Concentrator_ID[1] == Concentrator.id[1])
       &&(pbuff->pram.TuneUpRTC_Receive.Concentrator_ID[2] == Concentrator.id[2]))
  {
   time.Year   =  pbuff->pram.TuneUpRTC_Receive.Year;
   time.Year  +=  2000;
   time.Month  =  pbuff->pram.TuneUpRTC_Receive.Month;
   time.Day    =  pbuff->pram.TuneUpRTC_Receive.Day;
   time.Hour   =  pbuff->pram.TuneUpRTC_Receive.Hour;
   time.Minute =  pbuff->pram.TuneUpRTC_Receive.Minute;
   time.Second =  pbuff->pram.TuneUpRTC_Receive.Second;
 
   buff =  Get_KingDaServerBuff ( ); 
   if(P_NULL == buff){return;}
   PCF8563_Set_Time(&time);
   buff->buff.head.Sync = KingDaServerSync_ToServer;
   buff->buff.head.Cmd = TuneUp_RTC;
   buff->buff.head.Length_L = sizeof(TuneUpRTC_ToKingDaServer);
   buff->buff.head.Length_H = sizeof(TuneUpRTC_ToKingDaServer)>> 8;
   buff->buff.pram.TuneUpRTC_Send.Concentrator_ID[0] = Concentrator.id[0];
   buff->buff.pram.TuneUpRTC_Send.Concentrator_ID[1] = Concentrator.id[1];
   buff->buff.pram.TuneUpRTC_Send.Concentrator_ID[2] = Concentrator.id[2];
   buff->buff.pram.TuneUpRTC_Send.DIO[0] = 'O';
   buff->buff.pram.TuneUpRTC_Send.DIO[0] = 'K'; 

   buff->TargetPort = InPort_PV;
   err = KingDaServer_BuffSend(buff);
   if(NO_ERR != err)
   {Free_KingDaServerBuff(buff);}
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
Others:         // 
*********************************************************************************/
void GetRepeatersState_AckFunc(PProtocolSendBuff_Struct* SendBuff, void* AckData)
{
 SystemErrName err = NO_ERR; 
 KingDaServerSendBuff_Struct*  buff;  

 if(P_NULL == SendBuff ){return ;}
 buff =  Get_KingDaServerBuff ( ); 
 if(P_NULL == buff){return ;}
 buff->buff.head.Sync = KingDaServerSync_ToServer;
 buff->buff.head.Cmd = GetRepeatersState;
 buff->buff.head.Length_L = sizeof(GetRepeatersState_ToKingDaServer);
 buff->buff.head.Length_H = sizeof(GetRepeatersState_ToKingDaServer)>> 8;
 buff->buff.pram.GetRepeatersState_Send.Concentrator_ID[0] = Concentrator.id[0];
 buff->buff.pram.GetRepeatersState_Send.Concentrator_ID[1] = Concentrator.id[1];
 buff->buff.pram.GetRepeatersState_Send.Concentrator_ID[2] = Concentrator.id[2];
 if(P_NULL == AckData )
 { buff->buff.pram.GetRepeatersState_Send.State = 0;}
 else
 {buff->buff.pram.GetRepeatersState_Send.State = 1;}
 buff->TargetPort = SendBuff->SourcePort;
 err = KingDaServer_BuffSend(buff);
 if(NO_ERR != err)
 {Free_KingDaServerBuff(buff);}
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
static void GetRepeatersState_func(KingDaServer_PackageStruct* pbuff)
{
 if(KingDaServerSync_ToConcentrator == pbuff->head.Sync)
 {
  if((pbuff->pram.GetRepeatersState_Receive.Concentrator_ID[0] == Concentrator.id[0]) 
       &&(pbuff->pram.GetRepeatersState_Receive.Concentrator_ID[1] == Concentrator.id[1])
       &&(pbuff->pram.GetRepeatersState_Receive.Concentrator_ID[2] == Concentrator.id[2]))
  {
   ReadRepeatersNodeNumber_KindDa ( pbuff->pram.GetRepeatersState_Receive.Repeaters_ID,
                                    InPort_PV,
                                    GetRepeatersState_AckFunc);
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
Others:         // 
*********************************************************************************/
static void MeasureValue_UpLoad_func(KingDaServer_PackageStruct* pbuff)
{
 unsigned char length; 
 if(KingDaServerSync_ToConcentrator != pbuff->head.Sync)
 {return;}
 if((pbuff->pram.HandsData_Receive.OK.id[0] == Concentrator.id[0])
     &&(pbuff->pram.HandsData_Receive.OK.id[1] == Concentrator.id[1])
       &&(pbuff->pram.HandsData_Receive.OK.id[2] == Concentrator.id[2]))
 {
   length = 0;
   while (length < KingDaServerSendBuff_Number)
   {
    if((0 == send_buff[length].CCR.Bit.idle)
        &&(send_buff[length].buff.head.Cmd == pbuff->head.Cmd)
          &&(send_buff[length].TargetPort == InPort_PV)) 
    { 
     if(P_NULL != send_buff[length].AckFunc) 
     {send_buff[length].AckFunc(&send_buff[length].buff, pbuff);}
     Free_KingDaServerBuff(&(send_buff[length]));
     break;
    }
    length ++;
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
Others:         // 
*********************************************************************************/
static void ReadALLMeasureValue_func(KingDaServer_PackageStruct* pbuff)
{
 unsigned char length;  
 if(KingDaServerSync_ToConcentrator != pbuff->head.Sync)
 {return;}
 if((pbuff->pram.ReadALLMeasureValue_Receive.OK.Concentrator_ID[0] == Concentrator.id[0])
     &&(pbuff->pram.ReadALLMeasureValue_Receive.OK.Concentrator_ID[1] == Concentrator.id[1])
       &&(pbuff->pram.ReadALLMeasureValue_Receive.OK.Concentrator_ID[2] == Concentrator.id[2]))
 {
  if(0x03 == pbuff->head.Length_L )
  {
   //if(ServerState[0].Port == InPort_PV)
   //{SendMeasureValue_ToKingDaserver[0]_ACK(P_NULL, P_NULL); }
   //else if(ServerState[1].Port == InPort_PV)
   //{SendMeasureValue_ToKingDaserver[1]_ACK(P_NULL, P_NULL); }
   //else
   {return;}
  }
  else 
  {
   length = 0;
   while (length < KingDaServerSendBuff_Number)
   {
    if((0 == send_buff[length].CCR.Bit.idle)
       &&(send_buff[length].buff.head.Cmd == pbuff->head.Cmd)
       &&(send_buff[length].TargetPort == InPort_PV)) 
    { 
     if(P_NULL != send_buff[length].AckFunc) 
     {send_buff[length].AckFunc(&send_buff[length].buff, pbuff);}
     Free_KingDaServerBuff(&(send_buff[length]));
     break;
    }
    length ++;
   }  
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
Others:         // 
*********************************************************************************/
//static void SendEnd_CallBack(CommPortNumber_Type port, void *pbuff)
//{
 // unsigned long itemp;
 // KingDaServerSendBuff_Struct* p =  (KingDaServerSendBuff_Struct*)0;
 // itemp = (unsigned long)pbuff;
  //itemp  -= (unsigned long)(&(p->buff));
  //Free_KingDaServerBuff((KingDaServerSendBuff_Struct*) itemp);
//}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        // 
Others:         // 
*********************************************************************************/
//static void TimerOut_CallBack (void *pbuff)
//{
 // unsigned long itemp;
 // KingDaServerSendBuff_Struct* buff =  (KingDaServerSendBuff_Struct*)0;
 // itemp = (unsigned long)pbuff;
 // itemp  -= (unsigned long)(&(buff->buff));
 // buff = (KingDaServerSendBuff_Struct*) itemp;
 // if(P_NULL != buff->AckFunc)
 // {buff->AckFunc(&(buff->buff),P_NULL);}
  //Free_KingDaServerBuff((KingDaServerSendBuff_Struct*) itemp);
//}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        // 
Others:         // 
*********************************************************************************/
SystemErrName KingDaServer_BuffSend (KingDaServerSendBuff_Struct *pbuff)
{
 unsigned short length  ;
 unsigned char itemp = 0;
 if((P_NULL == pbuff) || (Null_Port == pbuff->TargetPort))
 {return IllegalParam;}

 length = pbuff->buff.head.Length_H;
 length <<= 8;
 length += pbuff->buff.head.Length_L; 
 length += sizeof (KindgDaServer_PackageHead);
 itemp = CRC8((unsigned char *)&(pbuff->buff),length);
 ((unsigned char *)&(pbuff->buff))[length++] = itemp;
  //if(0 != SendData_PHY (pbuff->TargetPort, (unsigned char *)&(pbuff->buff), length))
  //{return NO_ERR; }
  //else
  //{return Nothing;}
  
  //return OnSceneComm_send(KingDaServer, pbuff, length, pbuff->TargetPort, P_NULL, P_NULL );
 return NO_ERR;
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
SystemErrName SendHandCmdToKingDaServer(CommPortNumber_Type Port, 
                                        KingDaServer_CmdAckFunc AckFunc, 
                                        unsigned char Rssi)
{
 SystemErrName err = NO_ERR; 
 KingDaServerSendBuff_Struct*  buff; 
 if((GPRS_CH1 != Port)
    &&(GPRS_CH2 != Port)
      &&(GPRS_CH3 != Port)
        &&(GPRS_CH4 != Port))
 {return IllegalParam;}
 buff =  Get_KingDaServerBuff ( ); 
 if(P_NULL == buff)
 {return Nothing;}
 buff->CCR.Bit.AckEn = 1;
 buff->AckFunc =  AckFunc;
 buff->buff.head.Sync = KingDaServerSync_ToServer;
 buff->buff.head.Cmd = HandCmd_Server;
 buff->buff.head.Length_L = sizeof(HandsData_ToKingDaServer);
 buff->buff.head.Length_H = sizeof(HandsData_ToKingDaServer)>> 8;
 buff->buff.pram.HandsData_Send.id[0] = Concentrator.id[0];
 buff->buff.pram.HandsData_Send.id[1] = Concentrator.id[1];
 buff->buff.pram.HandsData_Send.id[2] = Concentrator.id[2];
 buff->buff.pram.HandsData_Send.DIO[0] = 'H';
 buff->buff.pram.HandsData_Send.DIO[1] = 'A';
 buff->buff.pram.HandsData_Send.DIO[2] = 'N';
 buff->buff.pram.HandsData_Send.DIO[3] = 'D';
 buff->buff.pram.HandsData_Send.DIO[4] = 'S';
 buff->buff.pram.HandsData_Send.CSQ =  Rssi;
 buff->TargetPort = Port;
 err = KingDaServer_BuffSend(buff);
 if(NO_ERR != err)
 {Free_KingDaServerBuff(buff);}
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
SystemErrName Send_ALLMeasureValue_ToKingDaServer(unsigned char Repeaters_ID,
                                                  KingDaServer_CmdCode Code)
{
 SystemErrName err = NO_ERR; 
 unsigned short length;
 unsigned short itemp ;
 KingDaServerSendBuff_Struct*  buff; 
 
 buff =  Get_KingDaServerBuff ( ); 
 if(P_NULL == buff){return Nothing;}

 buff->buff.head.Sync = KingDaServerSync_ToServer;
 buff->buff.head.Cmd = Code;
   
 buff->buff.pram.ReadALLMeasureValue_Send.Concentrator_ID[0] = Concentrator.id[0];
 buff->buff.pram.ReadALLMeasureValue_Send.Concentrator_ID[1] = Concentrator.id[1];
 buff->buff.pram.ReadALLMeasureValue_Send.Concentrator_ID[2] = Concentrator.id[2];
   
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[0] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[1] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[2] = 0;
   
 buff->buff.pram.ReadALLMeasureValue_Send.Repeaters_ID = Repeaters_ID;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersVoltage = 12;
 buff->buff.pram.ReadALLMeasureValue_Send.DIO[0] = 0xAA;
 buff->buff.pram.ReadALLMeasureValue_Send.DIO[1] = 0xAA;
   
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[0] = 1;
 length = buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[0];
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[1] = 2;
 length += buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[1];
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[2] = 3;
 length += buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[2];
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[3] = 4;
 length += buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[3];

 itemp = 0;
 while(itemp < length)
 {
  buff->buff.pram.ReadALLMeasureValue_Send.Data[itemp] = itemp;
  itemp++;
 }
 length = itemp <<1; 
 length += 14;
 buff->buff.head.Length_L = length;
 buff->buff.head.Length_H = length >> 8;

 buff->TargetPort = InPort_PV;
 err = KingDaServer_BuffSend(buff);
 if(NO_ERR != err)
 {Free_KingDaServerBuff(buff);} 
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
SystemErrName UpdatMeasureValue_ToKingDaServer(unsigned char Repeaters_NO, 
                                               CommPortNumber_Type Port,
                                               KingDaServer_CmdAckFunc AckFunc)
{
 SystemErrName err = NO_ERR; 
 unsigned short length;
 KingDaServerSendBuff_Struct*  buff; 
 Repeaters_DescrType repeaters;
 
 buff =  Get_KingDaServerBuff ( ); 
 if(P_NULL == buff){return Nothing;}
 
 buff->CCR.Bit.AckEn = 1;
 buff->AckFunc =  AckFunc;
 
 buff->buff.head.Sync = KingDaServerSync_ToServer;
 buff->buff.head.Cmd = MeasureValue_UpLoad;
   
 buff->buff.pram.ReadALLMeasureValue_Send.Concentrator_ID[0] = Concentrator.id[0];
 buff->buff.pram.ReadALLMeasureValue_Send.Concentrator_ID[1] = Concentrator.id[1];
 buff->buff.pram.ReadALLMeasureValue_Send.Concentrator_ID[2] = Concentrator.id[2];
   
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[0] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[1] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[2] = 0;
   
 buff->buff.pram.ReadALLMeasureValue_Send.Repeaters_ID = Repeaters_NO;
 
 repeaters.ID =  Repeaters_NO;
 MeasureValue_ToKingDaServer(&repeaters, buff->buff.pram.ReadALLMeasureValue_Send.Data);
   
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersVoltage = repeaters.Voltage;
 buff->buff.pram.ReadALLMeasureValue_Send.DIO[0] = 0xAA;
 buff->buff.pram.ReadALLMeasureValue_Send.DIO[1] = 0xAA;
   
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[0] = repeaters.CH_NO[0];
 length = buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[0];
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[1] = repeaters.CH_NO[1];
 length += buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[1];
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[2] = repeaters.CH_NO[2];
 length += buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[2];
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[3] = repeaters.CH_NO[3];
 length += buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[3];

 length <<= 1; 
 length += 14;
 buff->buff.head.Length_L = length;
 buff->buff.head.Length_H = length >> 8;

 buff->TargetPort = Port;
 err = KingDaServer_BuffSend(buff);
 if(NO_ERR != err)
 {Free_KingDaServerBuff(buff);} 
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
SystemErrName UpdatAppconMeasureValue_ToKingDaServer(unsigned char ServerNo,
                                                     KingDaServer_CmdAckFunc AckFunc)
{
 SystemErrName err = NO_ERR; 
 unsigned char Number;
 unsigned short length;
 KingDaServerSendBuff_Struct*  buff;
 TimerData_Struct timer;
 
 buff =  Get_KingDaServerBuff ( ); 
 if(P_NULL == buff){return Nothing;}

 Number = 50;
 //if(1 == ServerNo)
 //{AppconMeasureValue_ToKingDaserver[0](&Number, (unsigned char*)&(buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Datas));}
 //else if(2 == ServerNo)
 //{AppconMeasureValue_ToKingDaserver[1](&Number, (unsigned char*)&(buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Datas));}
 //else
 //{
   //Free_KingDaServerBuff(buff);
   //return NoData;
 //}
 if(0 == Number)
 {
   Free_KingDaServerBuff(buff);
   return NoData;
 }
 
 buff->CCR.Bit.AckEn = 1;
 buff->AckFunc =  AckFunc;
 
 buff->buff.head.Sync = KingDaServerSync_ToServer;
 buff->buff.head.Cmd = MeasureValue_UpLoad;
 get_SystemTimer (&timer);
 if(timer.Year < 2010)
 {timer.Year = 2010;}
  if((timer.Month < 1)||(timer.Month > 12))
 {timer.Month = 1;}
  if((timer.Day < 1)||(timer.Day > 31))
 {timer.Day = 1;}
  if(timer.Hour > 23)
 {timer.Hour = 0;}
  if(timer.Minute > 59)
 {timer.Minute = 0;}
   if(timer.Second > 59)
 {timer.Second = 0;}
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Year = timer.Year- 2000;
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Month = timer.Month;
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Day = timer.Day;
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Hour = timer.Hour;
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Minute = timer.Minute;
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Time.Second = timer.Second;
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Concentrator_ID[0] = Concentrator.id[0];
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Concentrator_ID[1] = Concentrator.id[1];
 buff->buff.pram.NodeDatas_APPCON_ToKingDaServer.Concentrator_ID[2] = Concentrator.id[2];
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[0] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[1] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[2] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.Repeaters_ID = 0;
 length = Number;
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[3] = length %10;
 length /= 10;
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[2] = length %10;
 length /= 10;
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[1] = length %10;
 length /= 10;
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[0] = length %10;
 
 length = Number * sizeof (NodeDatas_APPCON_Struct);
 length += 17;
 buff->buff.head.Length_L = length;
 buff->buff.head.Length_H = length >> 8;
 //buff->TargetPort = getServerPort(ServerNo); 
 err = KingDaServer_BuffSend(buff);
 if(NO_ERR != err)
 {Free_KingDaServerBuff(buff);} 
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
SystemErrName ReadALLMeasureValue_ToKingDaServer_ACK(unsigned char Repeaters_NO, 
                                                     CommPortNumber_Type Port,
                                                     KingDaServer_CmdAckFunc AckFunc)
{
 SystemErrName err = NO_ERR; 
 unsigned short length;
 KingDaServerSendBuff_Struct*  buff; 
 Repeaters_DescrType repeaters;
 
 buff =  Get_KingDaServerBuff ( ); 
 if(P_NULL == buff){return Nothing;}
 
 buff->CCR.Bit.AckEn = 1;
 buff->AckFunc =  AckFunc;
 
 buff->buff.head.Sync = KingDaServerSync_ToServer;
 buff->buff.head.Cmd = ReadALLMeasureValue;
   
 buff->buff.pram.ReadALLMeasureValue_Send.Concentrator_ID[0] = Concentrator.id[0];
 buff->buff.pram.ReadALLMeasureValue_Send.Concentrator_ID[1] = Concentrator.id[1];
 buff->buff.pram.ReadALLMeasureValue_Send.Concentrator_ID[2] = Concentrator.id[2];
   
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[0] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[1] = 0;
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersNet_ID[2] = 0;
   
 buff->buff.pram.ReadALLMeasureValue_Send.Repeaters_ID = Repeaters_NO;
 
 repeaters.ID =  Repeaters_NO;
 MeasureValue_ToKingDaServer(&repeaters, buff->buff.pram.ReadALLMeasureValue_Send.Data);
   
 buff->buff.pram.ReadALLMeasureValue_Send.RepeatersVoltage = repeaters.Voltage;
 buff->buff.pram.ReadALLMeasureValue_Send.DIO[0] = 0xAA;
 buff->buff.pram.ReadALLMeasureValue_Send.DIO[1] = 0xAA;
   
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[0] = repeaters.CH_NO[0];
 length = buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[0];
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[1] = repeaters.CH_NO[1];
 length += buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[1];
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[2] = repeaters.CH_NO[2];
 length += buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[2];
 buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[3] = repeaters.CH_NO[3];
 length += buff->buff.pram.ReadALLMeasureValue_Send.ChannelNode_ID[3];

 length <<= 1; 
 length += 14;
 buff->buff.head.Length_L = length;
 buff->buff.head.Length_H = length >> 8;

 buff->TargetPort = Port;
 err = KingDaServer_BuffSend(buff);
 if(NO_ERR != err)
 {Free_KingDaServerBuff(buff);} 
 return err;
}     
/******************************************END********************************************************/

