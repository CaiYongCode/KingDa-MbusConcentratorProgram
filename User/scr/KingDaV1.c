/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\KingDaV1.h"
#include "..\inc\CRC8.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
const unsigned char  CMD_OK1[2]={'O','K'};
/*********************************************************************************
公共变量定义区
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/
extern ConcentratorParameterStruct   Concentrator;
SystemErrName OnSceneComm_send(ProtocolName name, 
                               void* data, 
                               unsigned long length, 
                               CommPortNumber_Type port,
                               SerialPort_SendEndCallBack_Type callBack,
                               void *callBack_arg );
/*********************************************************************************
私有变量定义区
*********************************************************************************/ 
PProtocolSendBuff_Struct  KingDaV1send_buff[PProtoclSendBuff_Number];
static  CommPortNumber_Type InPort_PV;

/*********************************************************************************
测试变量定义区
*********************************************************************************/

/*********************************************************************************
内部函数定义区
*********************************************************************************/
static void DeInitBuff_PProtocol(void);
static PProtocolSendBuff_Struct* GetIdleBuff_PProtocol (void);
static SystemErrName Protocol_frameVerify ( void *pbuff, 
                                           unsigned char length, 
                                           unsigned char* Preamble_OP,
                                           KindgDaSPCP_PackageDataStruct** data);
static void ReadValue_func(KindgDaSPCP_PackageDataStruct *pbuff);
static void ReadData_func (KindgDaSPCP_PackageDataStruct* pbuff );
static void ReadDDRS_func (KindgDaSPCP_PackageDataStruct *pbuff);
static void ReadDDR2_func (KindgDaSPCP_PackageDataStruct *pbuff);
static void WriteDDR2_func(KindgDaSPCP_PackageDataStruct *pbuff);
static void WriteDDR1_func(KindgDaSPCP_PackageDataStruct *pbuff);
static void Open_OV_func(KindgDaSPCP_PackageDataStruct* pbuff);         //开阀   
static void Close_OV_func(KindgDaSPCP_PackageDataStruct* pbuff);        //关阀      
static void Get_OV_func(KindgDaSPCP_PackageDataStruct* pbuff);          //读阀状态
static void Open_OV_ENF_func(KindgDaSPCP_PackageDataStruct* pbuff);     //强制开阀
static void Close_OV_ENF_func(KindgDaSPCP_PackageDataStruct* pbuff);    //强制关阀  
static void MT_OV_func(KindgDaSPCP_PackageDataStruct* pbuff);           //活动阀命令   
static void MT_OV_ALL_func(KindgDaSPCP_PackageDataStruct* pbuff);       //广播活动阀命令 
static void ReadId_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff);  
static void WriteId_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff); //
static void ReadTime_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff);    //
static void WriteTime_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff);
static void ReadCycle_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff);
static void WriteCycle_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff);
static void ReadChannel_Concentrator_func  (KindgDaSPCP_PackageDataStruct* pbuff); //读取集中器各通道起始结束中继器号
static void WriteChannel_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff);//设置集中器各通道中继器起始结束号
static void ReadIpEnd_Concentrator_func  (KindgDaSPCP_PackageDataStruct* pbuff);   //读取是否开启域名链接服务器
static void WriteIpEnd_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff);  //设置开启、关闭使用域名链接服务器  
static void ReadServerInf_Concentrator_func  (KindgDaSPCP_PackageDataStruct* pbuff);  //读取网络参数 
static void WriteServerInf_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff);  //网络参数设置
static void ReadServer_Concentrator_func  (KindgDaSPCP_PackageDataStruct* pbuff);  //读取集中器域名信息  
static void WriteServer_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff);//设置集中器域名
static void ReadID_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff); //读取中继器号
static void WriteID_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff); //设置中继器号                
static void OPen_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff);  //打开中继器命令            
static void Close_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff);  //关闭中继器：即使中继器睡眠
static void ReadChannelNodeNumber_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff);   //读取中继器N下各个通道水表数量
static void WriteChannelNodeNumber_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff);  //设置中继器各通道水表数量
static SystemErrName PProtocol_send(PProtocolSendBuff_Struct *pbuff);//, unsigned char length);
static CommPortNumber_Type getPortForAddr ( unsigned char addr);
static unsigned short  BinChangeToBCD (unsigned char bin);
static void ReadGprsRssi_func (KindgDaSPCP_PackageDataStruct* pbuff); //读取信号强度

/*********************************************************************************
功能代码定义区
*********************************************************************************/
void KingDaV1Protocol_DeInit(void)
{
  InPort_PV = Null_Port;
  DeInitBuff_PProtocol( );
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
CommPortNumber_Type getPortForAddr ( unsigned char addr)
{
  CommPortNumber_Type port;
  if((addr >= Concentrator.channel[0].min)&&(addr <= Concentrator.channel[0].max))
  {port = Mbus_Port1;}
  else if((addr >= Concentrator.channel[1].min)&&(addr <= Concentrator.channel[1].max))
  {port = Mbus_Port2;}
  else if((addr >= Concentrator.channel[2].min)&&(addr <= Concentrator.channel[2].max))
  {port = Mbus_Port3;}
  else if((addr >= Concentrator.channel[3].min)&&(addr <= Concentrator.channel[3].max))
  {port = Mbus_Port4;}
  return port;
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
SystemErrName SendCmd_KindDaSPCP(KindgDaCmd_Struct* Cmd)                        
{
  SystemErrName err;
  unsigned char i;
  unsigned crc;
  unsigned char* tc;
  unsigned char* tp;
  PProtocolSendBuff_Struct*  Buff;
  Buff = GetIdleBuff_PProtocol ( );
  if(P_NULL  == Buff)
  {return Nothing;}
  Buff->AckFunc = Cmd->AckFunc;
  Buff->SourcePort = Cmd->SourcePort;
  Buff->CCR.Bit.Preamble = Cmd->CSR.Bit.Preamble; 
  if(UP == Cmd->CSR.Bit.DIR)
  {
    Buff->buff.Prm1 = PProtocl_Preamble_Up;
    Buff->buff.Prm2 = PProtocl_Preamble_Up;
    Buff->buff.Data.TypeA.head.Sync= PProtocl_Sync_Up;
  }
  else
  {
    Buff->buff.Prm1 = PProtocl_Preamble_Down;
    Buff->buff.Prm2 = PProtocl_Preamble_Down;
    Buff->buff.Data.TypeA.head.Sync= PProtocl_Sync_Down;
  }
  Buff->buff.Data.TypeA.head.Cmd = Cmd->Cmd;
  Buff->buff.Data.TypeA.head.Length =  Cmd->length; 
  tp = (unsigned char*) &(Buff->buff.Data.TypeA.pram);
  tc = Cmd->Param;
  i =0;
  while(i < Cmd->length)
  {
    tp[i] = tc[i];
    i++;
  }
  i = sizeof (PPro_PackageHead_TypeA) + Cmd->length;
  crc = CRC8((unsigned char *) &(Buff->buff.Data.TypeA.head),i);  
  ((unsigned char *) &(Buff->buff.Data.TypeA.head))[i] = crc;
  Buff->TargetPort = getPortForAddr(Cmd->TargetPort);
  err = PProtocol_send (Buff);
  if(NO_ERR != err)
  {FreeBuff_PProtocol(Buff);}
  return err;
}
/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        // 

Others:         // 因为不支多条指令并行执行 所以只搜索一次  搜索不可重入
*********************************************************************************/
PProtocolSendBuff_Struct* getSendBuffForAckCmd (CmdCode_KindgDaSPCP cmd)
{
  unsigned char i =0;
  while (i < PProtoclSendBuff_Number)
  {
    if((0 == KingDaV1send_buff[i].CCR.Bit.idle) && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == cmd))
    {
      if(KingDaV1send_buff[i].TargetPort == InPort_PV)
      {return &KingDaV1send_buff[i];}
    }
    i++;
  }
  return P_NULL;
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
SystemErrName Transpond_KingDaSPCP (Transpond_KingDaSPCP_struct* data)
{
  const unsigned char list[] ={Open_OV,Close_OV,Get_OV,Open_OV_ENF,Close_OV_ENF,MT_OV,MT_OV_ALL};  
  unsigned char i;
  unsigned char j;
  unsigned char* tp;
  unsigned char* tc;
  PProtocolSendBuff_Struct*  buff;
  buff = GetIdleBuff_PProtocol ( );
  if(P_NULL == buff)
  {return Nothing;}
  buff->CCR.Bit.Preamble =data->CSR.Bit.Preamble;
  buff->SourcePort = data->SourcePort; 
  buff->TargetPort = data->TargetPort; 
  tc = (unsigned char*)data->pbuff;
  tp = (unsigned char*)&buff->buff.Data.TypeA.head;
  j = 0;
  while ( j < 7)
  {
    if(data->pbuff->TypeA.head.Cmd == list[j])
    {break;}
    j ++;
  }
  if(j < 7)
  {  j = 8; }
  else
  {buff->DataLength = sizeof (PPro_PackageHead_TypeA) + 1 + data->pbuff->TypeA.head.Length;}
  i = 0;
  while (i < buff->DataLength)
  {
    tp[i] = tc[i];
    i++;
  } 
  if(0 != buff->CCR.Bit.Preamble)
  {
    if(PProtocl_Sync_Down == buff->buff.Data.TypeA.head.Sync) 
    {
      buff->buff.Prm1 = PProtocl_Preamble_Down;
      buff->buff.Prm2 = PProtocl_Preamble_Down;
    }
    else
    {
      buff->buff.Prm1 = PProtocl_Preamble_Up;
      buff->buff.Prm2 = PProtocl_Preamble_Up;
    }
    buff->DataLength +=  2 ;
  }
  if(PProtocl_Sync_Down == buff->buff.Data.TypeA.head.Sync) 
  {buff->CCR.Bit.AckEN = 1;}
  
  if(0 != buff->CCR.Bit.Preamble )
    
  return  OnSceneComm_send(KingDaV1, buff, buff->DataLength, buff->TargetPort, P_NULL,P_NULL);
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
SystemErrName KingDaV1Protocol_OK_Ackfunc (KindgDaSPCP_PackageDataStruct* package)
{
  const CmdCode_KindgDaSPCP CmdList[3] = {OPen_Repeaters, Close_Repeaters, WriteChannelNodeNumber_Repeaters};
  unsigned char  i;
  unsigned char  j;
  i = 0;
  while (i < PProtoclSendBuff_Number)
  {
    j = 0;
    if((0 == KingDaV1send_buff[i].CCR.Bit.idle) && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
    {
      while (j < 3)
      {
        if(KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == CmdList[j])
        {break;}
        j ++;
      }
      if (j < 3)
      {
        if(P_NULL != KingDaV1send_buff[i].AckFunc)
        {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), package);} // 应答数据处理程序
        else
        {
          if(Null_Port!= KingDaV1send_buff[i].SourcePort)
          {
            OnSceneComm_send(Penetrate, (void*)CMD_OK1,2, KingDaV1send_buff[i].SourcePort,P_NULL,P_NULL);
          }
          FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //释放缓冲区 
          return NO_ERR;
        }
        FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //释放缓冲区 
      }
    }
    i ++;
  }
  return OperateCmd_err;
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
SystemErrName KingDaV1Protocol_input (void *pbuff, 
                                      unsigned char length,
                                      CommPortNumber_Type  SourcePort)
{
  SystemErrName err;
  unsigned  char Preamble_flg = 0; 
  KindgDaSPCP_PackageDataStruct *package;
  
  InPort_PV = SourcePort;
  if(('O' == ((unsigned char*)pbuff)[0]) && ('K' == ((unsigned char*)pbuff)[1]))
  {return KingDaV1Protocol_OK_Ackfunc (pbuff);}
  err =  Protocol_frameVerify (pbuff,length, &Preamble_flg, &package); 
  if(NO_ERR != err)
  {return err;}
  
  switch( package->TypeA.head.Cmd )
  {
  case ReadValue:
    {ReadValue_func (package); break;}
  case ReadDDRS:
    {ReadDDRS_func (package);  break;}
  case ReadDDR2:
    {ReadDDR2_func (package);  break;}
  case WriteDDR2:
    {WriteDDR2_func (package); break;}
  case WriteDDR1:
    {WriteDDR1_func (package); break;}
  case Open_OV:        //开阀
    {Open_OV_func (package); break;}   
  case Close_OV:       //关阀
    {Close_OV_func (package); break;}
  case Get_OV:         //读阀状态
    {Get_OV_func (package); break;}   
  case Open_OV_ENF:    //强制开阀
    {Open_OV_ENF_func (package); break;}
  case Close_OV_ENF:  //强制关阀
    {Close_OV_ENF_func (package); break;}
  case MT_OV:         //活动阀命令
    {MT_OV_func (package); break;}
  case MT_OV_ALL:     //广播活动阀命令 
    {MT_OV_ALL_func (package); break;}
  case ReadId_Concentrator:        //读集中器号 
    {ReadId_Concentrator_func (package); break;} 
  case WriteId_Concentrator:       //设置集中器号 
    {WriteId_Concentrator_func (package); break;} 
  case ReadTime_Concentrator:     //读取当前时间 
    {ReadTime_Concentrator_func (package); break;}   
  case WriteTime_Concentrator:     //读取当前时间 
    {WriteTime_Concentrator_func (package); break;}   
  case ReadCycle_Concentrator:     //读取自动发送时间间隔 
    {
      if((0xAB == package->TypeA.pram.ReadCycle_ask.DI1)||(0xB6 == package->TypeA.pram.ReadCycle_ask.DI2))
      {ReadCycle_Concentrator_func (package);} //读取自动发送时间间隔 
      else
      {ReadID_Repeaters_func (package);} //读取中继器号
      break;
    }  
  case WriteCycle_Concentrator:                          //设置自动发送时间间隔 
    {WriteCycle_Concentrator_func (package); break;}
  case  ReadChannel_Concentrator:                       //读取各通道起始结束中继器号
    {ReadChannel_Concentrator_func (package); break;}    
  case  WriteChannel_Concentrator:                      //设置集中器各通道中继器起始结束号
    {WriteChannel_Concentrator_func (package); break;}
  case  ReadIpEnd_Concentrator:                         //读取是否开启域名链接服务器
    {ReadIpEnd_Concentrator_func (package); break;} 
  case  WriteIpEnd_Concentrator:                        //设置开启、关闭使用域名链接服务器
    {WriteIpEnd_Concentrator_func (package); break;}   
  case  ReadServerInf_Concentrator:                     //读取网络参数
    {ReadServerInf_Concentrator_func (package); break;} 
  case  WriteServerInf_Concentrator:                     //网络参数设置
    {WriteServerInf_Concentrator_func (package); break;}  
  case  ReadServerI_Concentrator:                        //读取集中器域名信息 
  case  ReadServerII_Concentrator:                       //读取备用域名信息 
    {ReadServer_Concentrator_func (package);break;}
  case  WriteServerI_Concentrator:                       //设置集中器域名
  case  WriteServerII_Concentrator:                       //设置备用域名 
    {WriteServer_Concentrator_func (package); break;}
  case  WriteID_Repeaters:                               //设置中继器号
    {WriteID_Repeaters_func (package); break;}
  case  OPen_Repeaters:                                  //打开中继器命令
    {OPen_Repeaters_func (package); break;}
  case  Close_Repeaters:                                 //关闭中继器：即使中继器睡眠
    {Close_Repeaters_func (package); break;}
  case  ReadChannelNodeNumber_Repeaters:                 //读取中继器N下各个通道水表数量
    {
      if(0 == Preamble_flg)
      {ReadChannelNodeNumber_Repeaters_func (package);}
      else
      {ReadData_func (package);}  
      break;
    }
  case  WriteChannelNodeNumber_Repeaters:                //设置中继器各通道水表数量
    {
      if(2 == package->TypeA.head.Length)
      {ReadGprsRssi_func (package);}
      else
      {WriteChannelNodeNumber_Repeaters_func (package); } 
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
void DeInitBuff_PProtocol(void)
{
  unsigned char itemp =0;
  while ( itemp < PProtoclSendBuff_Number)
  {
    KingDaV1send_buff[itemp].CCR.Byte = 0; 
    KingDaV1send_buff[itemp].CCR.Bit.idle = 1;
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
static SystemErrName PProtocolBuffVerify (PProtocolSendBuff_Struct* pbuff) 
{
  unsigned char itemp = 0;
  while (itemp < PProtoclSendBuff_Number)
  {
    if(pbuff == &(KingDaV1send_buff[itemp]))
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
static PProtocolSendBuff_Struct* GetIdleBuff_PProtocol (void)
{
  unsigned char itemp =0;
  while ( itemp < PProtoclSendBuff_Number)
  {
    if(KingDaV1send_buff[itemp].CCR.Bit.idle)
    {
      KingDaV1send_buff[itemp].CCR.Byte = 0; 
      KingDaV1send_buff[itemp].DataLength = 0;
      KingDaV1send_buff[itemp].AckFunc = P_NULL;
      KingDaV1send_buff[itemp].SourcePort = Null_Port;
      KingDaV1send_buff[itemp].TargetPort = Null_Port;
      return &(KingDaV1send_buff[itemp]);
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
SystemErrName FreeBuff_PProtocol(PProtocolSendBuff_Struct* pbuff)
{
  if(NO_ERR == PProtocolBuffVerify (pbuff))
  {
    pbuff->CCR.Byte = 0; 
    pbuff->CCR.Bit.idle = 1;
    return NO_ERR ;
  }
  return IllegalParam ; 
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
static SystemErrName Protocol_frameVerify ( void *pbuff, 
                                           unsigned char length, 
                                           unsigned char* Preamble_OP,
                                           KindgDaSPCP_PackageDataStruct** data)
{
  const unsigned char list[] ={Open_OV,Close_OV,Get_OV,Open_OV_ENF,Close_OV_ENF,MT_OV,MT_OV_ALL}; 
  union {
    unsigned char* p;
    PPro_PackageHead_TypeA  *headp;
  }buff;
  unsigned char itemp;
  unsigned char itemp2;
  itemp = 0;
  buff.p = pbuff;
  itemp2 = 0;
  while (itemp < length)
  {
    if((PProtocl_Preamble_Down  != *(buff.p)) && (PProtocl_Preamble_Up != *(buff.p)))
    {break;} 
    buff.p ++;
    itemp2 &= 0x0F;
    itemp2 ++;
  }
  if(itemp2 >= 2)
  {*Preamble_OP = 1;}
  else
  {*Preamble_OP = 0;}
  if(length < sizeof(PPro_PackageHead_TypeA))
  {return Length_err;}
  
  if((PProtocl_Sync_Down != buff.headp->Sync)&&(PProtocl_Sync_Up != buff.headp->Sync))
  {return FrameSync_err;}
  if(length < sizeof(PPro_PackageHead_TypeA) + buff.headp->Length + 1)
  {return Length_err;}
  
  itemp = sizeof (PPro_PackageHead_TypeA);
  itemp2 = 0;
  while ( itemp2 < 7)
  {
    if(buff.headp->Cmd == list[itemp2])
    {break;}
    itemp2 ++;
  }
  if(itemp2 < 7)
  { itemp += 4;}
  else
  {itemp += buff.headp->Length;}
  pbuff = &(buff.headp->Sync); 
  if(((unsigned char *)pbuff)[itemp] != CRC8((unsigned char *) pbuff,itemp)) 
  {return Verify_err;}
  
  *data = (KindgDaSPCP_PackageDataStruct*)buff.headp;
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
static void ReadValue_func (KindgDaSPCP_PackageDataStruct *pbuff) 
{
  unsigned char i;
  Transpond_KingDaSPCP_struct  Trans;
  
  if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
  {
    i = 0;
    while (i < PProtoclSendBuff_Number)
    {
      if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
         && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == ReadValue) 
           && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
      {
        if((KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadValue_ask.addr[0] == pbuff->TypeA.pram.ReadValue_ack.addr[0]) 
           &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadValue_ask.addr[1] == pbuff->TypeA.pram.ReadValue_ack.addr[1])
             &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadValue_ask.addr[2] == pbuff->TypeA.pram.ReadValue_ack.addr[2]) )
        {break;}
      }
      i ++;
    }
    if(i < PProtoclSendBuff_Number)
    {
      if(P_NULL != KingDaV1send_buff[i].AckFunc)
      {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // 应答数据处理程序
      else
      {
        if(Null_Port!= KingDaV1send_buff[i].SourcePort)
        {
          Trans.CSR.Bit.Preamble =  1;
          Trans.pbuff = pbuff;
          Trans.SourcePort = Null_Port;
          Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
          Transpond_KingDaSPCP (&Trans); 
        }
      }
      FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //释放缓冲区 
    }  
  }
  else
  {
    Trans.CSR.Bit.Preamble =  1;
    Trans.pbuff = pbuff;
    Trans.SourcePort = InPort_PV;
    Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.ReadValue_ask.addr[0]);
    Transpond_KingDaSPCP (&Trans); 
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
static void ReadData_func (KindgDaSPCP_PackageDataStruct* pbuff )
{
  unsigned char i;
  Transpond_KingDaSPCP_struct  Trans;
  
  if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
  {
    i = 0;
    while (i < PProtoclSendBuff_Number)
    {
      if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
         && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == ReadData) 
           && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
      {
        if((KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadData_ask.addr[0] == pbuff->TypeA.pram.ReadData_ack.addr[0]) 
           &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadData_ask.addr[1] == pbuff->TypeA.pram.ReadData_ack.addr[1])
             &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadData_ask.addr[2] == pbuff->TypeA.pram.ReadData_ack.addr[2]) )
        {break;}
      }
      i ++;
    }
    if(i < PProtoclSendBuff_Number)
    {
      if(P_NULL != KingDaV1send_buff[i].AckFunc)
      {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // 应答数据处理程序
      else
      {
        if(Null_Port!= KingDaV1send_buff[i].SourcePort)
        {
          Trans.CSR.Bit.Preamble =  1;
          Trans.pbuff = pbuff;
          Trans.SourcePort = Null_Port;
          Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
          Transpond_KingDaSPCP (&Trans); 
        }
      }
      FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //释放缓冲区 
    }  
  }
  else
  {
    Trans.CSR.Bit.Preamble =  1;
    Trans.pbuff = pbuff;
    Trans.SourcePort = InPort_PV;
    Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.ReadData_ask.addr[0]);
    Transpond_KingDaSPCP (&Trans); 
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
static void ReadDDRS_func (KindgDaSPCP_PackageDataStruct* pbuff )
{
  unsigned char i;
  Transpond_KingDaSPCP_struct  Trans;
  
  if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
  {
    i = 0;
    while (i < PProtoclSendBuff_Number)
    {
      if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
         && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == ReadDDRS) 
           && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
      {
        if((KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadValue_ask.addr[0] == pbuff->TypeA.pram.ReadValue_ack.addr[0]) 
           &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadValue_ask.addr[1] == pbuff->TypeA.pram.ReadValue_ack.addr[1])
             &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadValue_ask.addr[2] == pbuff->TypeA.pram.ReadValue_ack.addr[2]) )
        {break;}
      }
      i ++;
    }
    if(i < PProtoclSendBuff_Number)
    {
      if(P_NULL != KingDaV1send_buff[i].AckFunc)
      {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // 应答数据处理程序
      else
      {
        if(Null_Port!= KingDaV1send_buff[i].SourcePort)
        {
          Trans.CSR.Bit.Preamble =  1;
          Trans.pbuff = pbuff;
          Trans.SourcePort = Null_Port;
          Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
          Transpond_KingDaSPCP (&Trans); 
        }
      }
      FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //释放缓冲区 
    }  
  }
  else
  {
    Trans.CSR.Bit.Preamble =  1;
    Trans.pbuff = pbuff;
    Trans.SourcePort = InPort_PV;
    Trans.TargetPort = get_MbusChannelNumber();
    Transpond_KingDaSPCP (&Trans); 
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
static void ReadDDR2_func (KindgDaSPCP_PackageDataStruct* pbuff)
{ 
  unsigned char i;
  Transpond_KingDaSPCP_struct  Trans;
  
  if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
  {
    i = 0;
    while (i < PProtoclSendBuff_Number)
    {
      if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
         && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == ReadDDR2) 
           && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
      {
        if((KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadDDR2_ask.DDR1[0] == pbuff->TypeA.pram.ReadDDR2_ack.DDR1[0]) 
           &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadDDR2_ask.DDR1[1] == pbuff->TypeA.pram.ReadDDR2_ack.DDR1[1])
             &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadDDR2_ask.DDR1[2] == pbuff->TypeA.pram.ReadDDR2_ack.DDR1[2])
               &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadDDR2_ask.DDR1[3] == pbuff->TypeA.pram.ReadDDR2_ack.DDR1[3]))
        {break;}
      }
      i ++;
    }
    if(i < PProtoclSendBuff_Number)
    {
      if(P_NULL != KingDaV1send_buff[i].AckFunc)
      {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // 应答数据处理程序
      else
      {
        if(Null_Port!= KingDaV1send_buff[i].SourcePort)
        {
          Trans.CSR.Bit.Preamble =  1;
          Trans.pbuff = pbuff;
          Trans.SourcePort = Null_Port;
          Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
          Transpond_KingDaSPCP (&Trans); 
        }
      }
      FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //释放缓冲区 
    }  
  }
  else
  {
    Trans.CSR.Bit.Preamble =  1;
    Trans.pbuff = pbuff;
    Trans.SourcePort = InPort_PV;
    Trans.TargetPort = get_MbusChannelNumber();
    Transpond_KingDaSPCP (&Trans); 
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
static void WriteDDR2_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
  unsigned char i;
  Transpond_KingDaSPCP_struct  Trans;
  
  if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
  {
    i = 0;
    while (i < PProtoclSendBuff_Number)
    {
      if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
         && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == WriteDDR2) 
           && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
      {
        if((KingDaV1send_buff[i].buff.Data.TypeA.pram.WriteDDR2_ask.DDR2[0] == pbuff->TypeA.pram.WriteDDR2_ack.DDR2[0]) 
           &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.WriteDDR2_ask.DDR2[1] == pbuff->TypeA.pram.WriteDDR2_ack.DDR2[1])
             &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.WriteDDR2_ask.DDR2[2] == pbuff->TypeA.pram.WriteDDR2_ack.DDR2[2]) )
        {break;}
      }
      i ++;
    }
    if(i < PProtoclSendBuff_Number)
    {
      if(P_NULL != KingDaV1send_buff[i].AckFunc)
      {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // 应答数据处理程序
      else
      {
        if(Null_Port!= KingDaV1send_buff[i].SourcePort)
        {
          Trans.CSR.Bit.Preamble =  1;
          Trans.pbuff = pbuff;
          Trans.SourcePort = Null_Port;
          Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
          Transpond_KingDaSPCP (&Trans); 
        }
      }
      FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //释放缓冲区 
    }  
  }
  else
  {
    Trans.CSR.Bit.Preamble =  1;
    Trans.pbuff = pbuff;
    Trans.SourcePort = InPort_PV;
    Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.WriteDDR2_ask.DDR2[0]);
    Transpond_KingDaSPCP (&Trans); 
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
static void WriteDDR1_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
  
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
static void Open_OV_func (KindgDaSPCP_PackageDataStruct* pbuff)        //开阀
{ 
  unsigned char i;
  Transpond_KingDaSPCP_struct  Trans;
  
  if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
  {
    i = 0;
    while (i < PProtoclSendBuff_Number)
    {
      if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
         && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == Open_OV) 
           && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
      {
        if((KingDaV1send_buff[i].buff.Data.TypeA.pram.Open_OV_ask.addr[0] == pbuff->TypeA.pram.Open_OV_ack.addr[0]) 
           &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Open_OV_ask.addr[1] == pbuff->TypeA.pram.Open_OV_ack.addr[1])
             &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Open_OV_ask.addr[2] == pbuff->TypeA.pram.Open_OV_ack.addr[2]) )
        {break;}
      }
      i ++;
    }
    if(i < PProtoclSendBuff_Number)
    {
      if(P_NULL != KingDaV1send_buff[i].AckFunc)
      {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // 应答数据处理程序
      else
      {
        if(Null_Port!= KingDaV1send_buff[i].SourcePort)
        {
          Trans.CSR.Bit.Preamble =  1;
          Trans.pbuff = pbuff;
          Trans.SourcePort = Null_Port;
          Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
          Transpond_KingDaSPCP (&Trans); 
        }
      }
      FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //释放缓冲区 
    }  
  }
  else
  {
    Trans.CSR.Bit.Preamble =  1;
    Trans.pbuff = pbuff;
    Trans.SourcePort = InPort_PV;
    Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.Open_OV_ask.addr[0]);
    Transpond_KingDaSPCP (&Trans); 
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
static void Close_OV_func (KindgDaSPCP_PackageDataStruct* pbuff)        //关阀 
{
  unsigned char i;
  Transpond_KingDaSPCP_struct  Trans;
  
  if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
  {
    i = 0;
    while (i < PProtoclSendBuff_Number)
    {
      if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
         && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == Close_OV) 
           && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
      {
        if((KingDaV1send_buff[i].buff.Data.TypeA.pram.Close_OV_ask.addr[0] == pbuff->TypeA.pram.Close_OV_ack.addr[0]) 
           &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Close_OV_ask.addr[1] == pbuff->TypeA.pram.Close_OV_ack.addr[1])
             &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Close_OV_ask.addr[2] == pbuff->TypeA.pram.Close_OV_ack.addr[2]) )
        {break;}
      }
      i ++;
    }
    if(i < PProtoclSendBuff_Number)
    {
      if(P_NULL != KingDaV1send_buff[i].AckFunc)
      {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // 应答数据处理程序
      else
      {
        if(Null_Port!= KingDaV1send_buff[i].SourcePort)
        {
          Trans.CSR.Bit.Preamble =  1;
          Trans.pbuff = pbuff;
          Trans.SourcePort = Null_Port;
          Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
          Transpond_KingDaSPCP (&Trans); 
        }
      }
      FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //释放缓冲区 
    }  
  }
  else
  {
    Trans.CSR.Bit.Preamble =  1;
    Trans.pbuff = pbuff;
    Trans.SourcePort = InPort_PV;
    Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.Close_OV_ask.addr[0]);
    Transpond_KingDaSPCP (&Trans); 
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
static void Get_OV_func (KindgDaSPCP_PackageDataStruct* pbuff)           //读阀状态
{
  unsigned char i;
  Transpond_KingDaSPCP_struct  Trans;
  
  if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
  {
    i = 0;
    while (i < PProtoclSendBuff_Number)
    {
      if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
         && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == Get_OV) 
           && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
      {
        if((KingDaV1send_buff[i].buff.Data.TypeA.pram.Get_OV_ask.addr[0] == pbuff->TypeA.pram.Get_OV_ack.addr[0]) 
           &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Get_OV_ask.addr[1] == pbuff->TypeA.pram.Get_OV_ack.addr[1])
             &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Get_OV_ask.addr[2] == pbuff->TypeA.pram.Get_OV_ack.addr[2]) )
        {break;}
      }
      i ++;
    }
    if(i < PProtoclSendBuff_Number)
    {
      if(P_NULL != KingDaV1send_buff[i].AckFunc)
      {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // 应答数据处理程序
      else
      {
        if(Null_Port!= KingDaV1send_buff[i].SourcePort)
        {
          Trans.CSR.Bit.Preamble =  1;
          Trans.pbuff = pbuff;
          Trans.SourcePort = Null_Port;
          pbuff->TypeA.head.Length = 4;
          Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
          Transpond_KingDaSPCP (&Trans); 
        }
      }
      FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //释放缓冲区 
    }  
  }
  else
  {
    
    Trans.CSR.Bit.Preamble =  1;
    
    //pbuff->head.Length = 4;
    
    Trans.pbuff = pbuff;
    Trans.SourcePort = InPort_PV;
    Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.Get_OV_ask.addr[0]);
    Transpond_KingDaSPCP (&Trans); 
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
static void Open_OV_ENF_func (KindgDaSPCP_PackageDataStruct* pbuff)     //强制开阀
{
  unsigned char i;
  Transpond_KingDaSPCP_struct  Trans;
  
  if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
  {
    i = 0;
    while (i < PProtoclSendBuff_Number)
    {
      if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
         && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == Get_OV) 
           && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
      {
        if((KingDaV1send_buff[i].buff.Data.TypeA.pram.Open_OV_ENF_ask.addr[0] == pbuff->TypeA.pram.Open_OV_ENF_ack.addr[0]) 
           &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Open_OV_ENF_ask.addr[1] == pbuff->TypeA.pram.Open_OV_ENF_ack.addr[1])
             &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Open_OV_ENF_ask.addr[2] == pbuff->TypeA.pram.Open_OV_ENF_ack.addr[2]) )
        {break;}
      }
      i ++;
    }
    if(i < PProtoclSendBuff_Number)
    {
      if(P_NULL != KingDaV1send_buff[i].AckFunc)
      {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // 应答数据处理程序
      else
      {
        if(Null_Port!= KingDaV1send_buff[i].SourcePort)
        {
          Trans.CSR.Bit.Preamble =  1;
          Trans.pbuff = pbuff;
          Trans.SourcePort = Null_Port;
          Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
          Transpond_KingDaSPCP (&Trans); 
        }
      }
      FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //释放缓冲区 
    }  
  }
  else
  {
    Trans.CSR.Bit.Preamble =  1;
    Trans.pbuff = pbuff;
    Trans.SourcePort = InPort_PV;
    Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.Open_OV_ENF_ask.addr[0]);
    Transpond_KingDaSPCP (&Trans); 
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
static void Close_OV_ENF_func (KindgDaSPCP_PackageDataStruct* pbuff)   //强制关阀 
{
  unsigned char i;
  Transpond_KingDaSPCP_struct  Trans;
  
  if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
  {
    i = 0;
    while (i < PProtoclSendBuff_Number)
    {
      if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
         && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == Close_OV_ENF) 
           && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
      {
        if((KingDaV1send_buff[i].buff.Data.TypeA.pram.Close_OV_ENF_ask.addr[0] == pbuff->TypeA.pram.Close_OV_ENF_ack.addr[0]) 
           &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Close_OV_ENF_ask.addr[1] == pbuff->TypeA.pram.Close_OV_ENF_ack.addr[1])
             &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.Close_OV_ENF_ask.addr[2] == pbuff->TypeA.pram.Close_OV_ENF_ack.addr[2]) )
        {break;}
      }
      i ++;
    }
    if(i < PProtoclSendBuff_Number)
    {
      if(P_NULL != KingDaV1send_buff[i].AckFunc)
      {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // 应答数据处理程序
      else
      {
        if(Null_Port!= KingDaV1send_buff[i].SourcePort)
        {
          Trans.CSR.Bit.Preamble =  1;
          Trans.pbuff = pbuff;
          Trans.SourcePort = Null_Port;
          Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
          Transpond_KingDaSPCP (&Trans); 
        }
      }
      FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //释放缓冲区 
    }  
  }
  else
  {
    Trans.CSR.Bit.Preamble =  1;
    Trans.pbuff = pbuff;
    Trans.SourcePort = InPort_PV;
    Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.Close_OV_ENF_ask.addr[0]);
    Transpond_KingDaSPCP (&Trans); 
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
static void MT_OV_func (KindgDaSPCP_PackageDataStruct* pbuff)             //活动阀命令 
{
  unsigned char i;
  Transpond_KingDaSPCP_struct  Trans;
  
  if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
  {
    i = 0;
    while (i < PProtoclSendBuff_Number)
    {
      if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
         && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == MT_OV) 
           && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
      {
        if((KingDaV1send_buff[i].buff.Data.TypeA.pram.MT_OV_ask.addr[0] == pbuff->TypeA.pram.MT_OV_ack.addr[0]) 
           &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.MT_OV_ask.addr[1] == pbuff->TypeA.pram.MT_OV_ack.addr[1])
             &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.MT_OV_ask.addr[2] == pbuff->TypeA.pram.MT_OV_ack.addr[2]) )
        {break;}
      }
      i ++;
    }
    if(i < PProtoclSendBuff_Number)
    {
      if(P_NULL != KingDaV1send_buff[i].AckFunc)
      {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // 应答数据处理程序
      else
      {
        if(Null_Port!= KingDaV1send_buff[i].SourcePort)
        {
          Trans.CSR.Bit.Preamble =  1;
          Trans.pbuff = pbuff;
          Trans.SourcePort = Null_Port;
          Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
          Transpond_KingDaSPCP (&Trans); 
        }
      }
      FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //释放缓冲区 
    }  
  }
  else
  {
    Trans.CSR.Bit.Preamble =  1;
    Trans.pbuff = pbuff;
    Trans.SourcePort = InPort_PV;
    Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.MT_OV_ask.addr[0]);
    Transpond_KingDaSPCP (&Trans); 
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
static void MT_OV_ALL_func (KindgDaSPCP_PackageDataStruct* pbuff)     //广播活动阀命令
{
  Transpond_KingDaSPCP_struct  Trans;
  
  if(PProtocl_Sync_Down == pbuff->TypeA.head.Sync)
  {
    Trans.CSR.Bit.Preamble =  1;
    Trans.pbuff = pbuff;
    Trans.SourcePort = InPort_PV;
    Trans.TargetPort = get_MbusChannelNumber();
    if(Null_Port == Trans.TargetPort)
    {Transpond_KingDaSPCP (&Trans);}
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
static void ReadId_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff)
{
  PProtocolSendBuff_Struct*  buff;
  
  if((0xAB != pbuff->TypeA.pram.ReadId_ask.DI1)||(0 != pbuff->TypeA.pram.ReadId_ask.DI2))
  {return;}
  buff = GetIdleBuff_PProtocol ( );
  if(P_NULL == buff){return;}
  
  buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
  buff->SourcePort = Null_Port;
  buff->TargetPort = InPort_PV;
  buff->buff.Prm1 = PProtocl_Preamble_Up;
  buff->buff.Prm2 = PProtocl_Preamble_Up;
  buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
  buff->buff.Data.TypeB.head.Cmd = ReadId_Concentrator;
  buff->buff.Data.TypeB.head.Length =  sizeof (ReadId_Concentrator_ACKstruct);
  buff->buff.Data.TypeB.pram.ReadId_ack.DI1 = 0xAB ;
  buff->buff.Data.TypeB.pram.ReadId_ack.id[0] = Concentrator.id[0] ;
  buff->buff.Data.TypeB.pram.ReadId_ack.id[1] = Concentrator.id[1];
  buff->buff.Data.TypeB.pram.ReadId_ack.id[2] = Concentrator.id[2]; 
  if(NO_ERR != PProtocol_send (buff))
  {FreeBuff_PProtocol(buff);}
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
static void WriteId_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff) //
{
  //SystemErrName err = NO_ERR; 
  PProtocolSendBuff_Struct*  buff;
  unsigned long p;
  
  if(0xAB != pbuff->TypeA.pram.WriteId_ask.DI1)
  {return;}
  
  p =(unsigned long)(((ConcentratorParameterStruct*) 0)->id);
  p += ConcentratorParameter_AddrEeprom;
  if(0 == EEPROM_I2C_MultipleWrite(pbuff->TypeA.pram.WriteId_ask.id,p, 3))
  {return ;}  
  
  Concentrator.id[0] = pbuff->TypeA.pram.WriteId_ask.id[0];
  Concentrator.id[1] = pbuff->TypeA.pram.WriteId_ask.id[1];
  Concentrator.id[2] = pbuff->TypeA.pram.WriteId_ask.id[2];
  buff = GetIdleBuff_PProtocol ( );
  if(P_NULL == buff){return;}
  buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
  buff->SourcePort = Null_Port;
  buff->TargetPort = InPort_PV;
  buff->buff.Prm1 = PProtocl_Preamble_Up;
  buff->buff.Prm2 = PProtocl_Preamble_Up;
  buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
  buff->buff.Data.TypeB.head.Cmd = WriteId_Concentrator;
  buff->buff.Data.TypeB.head.Length =  sizeof (WriteId_Concentrator_ACKstruct);
  buff->buff.Data.TypeB.pram.WriteId_ack.DI1 = 0xAB ;
  buff->buff.Data.TypeB.pram.WriteId_ack.state[0] = 'O';
  buff->buff.Data.TypeB.pram.WriteId_ack.state[1] = 'K';
  if(NO_ERR != PProtocol_send (buff))
  {FreeBuff_PProtocol(buff);}
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
static void ReadTime_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff)
{
  PProtocolSendBuff_Struct*  buff;
  TimerData_Struct timer;
  
  if((0xAB != pbuff->TypeA.pram.ReadTime_ask.DI1)||(0x00 != pbuff->TypeA.pram.ReadTime_ask.DI2))
  {return;}
  
  buff = GetIdleBuff_PProtocol ( );
  if(P_NULL == buff){return;}
  
  buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
  buff->SourcePort = Null_Port;
  buff->TargetPort = InPort_PV;
  buff->buff.Prm1 = PProtocl_Preamble_Up;
  buff->buff.Prm2 = PProtocl_Preamble_Up;
  buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
  buff->buff.Data.TypeB.head.Cmd = ReadTime_Concentrator;
  buff->buff.Data.TypeB.head.Length =  sizeof (ReadTime_Concentrator_ACKstruct);
  buff->buff.Data.TypeB.pram.ReadTime_ack.DI1 = 0xAB ;
  get_SystemTimer(&timer);
  buff->buff.Data.TypeB.pram.ReadTime_ack.year = BinChangeToBCD(timer.Year - 2000);
  buff->buff.Data.TypeB.pram.ReadTime_ack.month = BinChangeToBCD(timer.Month);
  buff->buff.Data.TypeB.pram.ReadTime_ack.day = BinChangeToBCD(timer.Day);
  buff->buff.Data.TypeB.pram.ReadTime_ack.hour = BinChangeToBCD(timer.Hour);
  buff->buff.Data.TypeB.pram.ReadTime_ack.minute = BinChangeToBCD(timer.Minute);
  buff->buff.Data.TypeB.pram.ReadTime_ack.second = BinChangeToBCD(timer.Second);
  if(NO_ERR != PProtocol_send (buff))
  {FreeBuff_PProtocol(buff);} 
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
static void WriteTime_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff)
{
  TimerData_Struct time;
  PProtocolSendBuff_Struct*  buff;
  
  if(0xAB != pbuff->TypeA.pram.WriteTime_ask.DI1)
  {return;}
  
  time.Year = (unsigned short)(pbuff->TypeA.pram.WriteTime_ask.year) + 2000;
  time.Month = pbuff->TypeA.pram.WriteTime_ask.month;
  time.Day = pbuff->TypeA.pram.WriteTime_ask.day;
  time.Hour = pbuff->TypeA.pram.WriteTime_ask.hour;
  time.Minute = pbuff->TypeA.pram.WriteTime_ask.minute;
  time.Second = pbuff->TypeA.pram.WriteTime_ask.second;
  
  buff = GetIdleBuff_PProtocol ( );
  if(P_NULL == buff){return;}
  
  buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
  
  PCF8563_Set_Time(&time);
  
  buff->SourcePort = Null_Port;
  buff->TargetPort = InPort_PV;
  buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
  buff->buff.Data.TypeB.head.Cmd = WriteTime_Concentrator;
  buff->buff.Data.TypeB.head.Length =  sizeof (WriteTime_Concentrator_ACKstruct);
  
  buff->buff.Data.TypeB.pram.WriteTime_ack.DI1 = 0xAB; 
  buff->buff.Data.TypeB.pram.WriteTime_ack.state[0] = 'O';
  buff->buff.Data.TypeB.pram.WriteTime_ack.state[1] = 'K';
  if(NO_ERR != PProtocol_send (buff))
  {FreeBuff_PProtocol(buff);} 
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
static void ReadCycle_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff)
{
  PProtocolSendBuff_Struct*  buff;
  buff = GetIdleBuff_PProtocol ( );
  if(P_NULL == buff){return;}
  buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
  buff->SourcePort = Null_Port;
  buff->TargetPort = InPort_PV;
  buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
  buff->buff.Data.TypeB.head.Cmd = ReadCycle_Concentrator;
  buff->buff.Data.TypeB.pram.ReadCycle_ack_B.DI1 = 0xAB;
  
  if(0x01 == Concentrator.CSR.Bit.Appcon_EN)
  {
    buff->buff.Data.TypeB.head.Length =  sizeof (ReadCycle_Concentrator_ACKstruct_B);
    buff->buff.Data.TypeB.pram.ReadCycle_ack_B.Day = Concentrator.cycle.day ;
    buff->buff.Data.TypeB.pram.ReadCycle_ack_B.hour = Concentrator.cycle.hour ;
    buff->buff.Data.TypeB.pram.ReadCycle_ack_B.minute = Concentrator.cycle.minute; 
  }
  else
  {
    buff->buff.Data.TypeB.head.Length =  sizeof (ReadCycle_Concentrator_ACKstruct_A);
    buff->buff.Data.TypeB.pram.ReadCycle_ack_A.hour = Concentrator.cycle.hour ;
    buff->buff.Data.TypeB.pram.ReadCycle_ack_A.minute = Concentrator.cycle.minute;  
  }
  if(NO_ERR != PProtocol_send (buff))
  {FreeBuff_PProtocol(buff);}
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
static void WriteCycle_Concentrator_func(KindgDaSPCP_PackageDataStruct* pbuff)
{
  PProtocolSendBuff_Struct*  buff;
  unsigned long p;
  AutoCollectCycleStruct  Cycle;
  
  p =(unsigned long)&(((ConcentratorParameterStruct*) 0)->cycle);
  p += ConcentratorParameter_AddrEeprom;
  if(3 == pbuff ->TypeA.head.Length) 
  {
    if(0xAB != pbuff->TypeA.pram.WriteCycle_ask_TypeA.DI1)
    {return;}
    Cycle.day = 1;
    Cycle.hour = pbuff->TypeA.pram.WriteCycle_ask_TypeA.hour;
    Cycle.minute = pbuff->TypeA.pram.WriteCycle_ask_TypeA.minute;     
    if(0 == EEPROM_I2C_MultipleWrite(&(Cycle), p,sizeof(AutoCollectCycleStruct)))
    {return ;} 
    if(0 == EEPROM_I2C_MultipleRead(&(Cycle), p,sizeof(AutoCollectCycleStruct)))
    {return ;}  
    if((0 != Cycle.day)
       ||(Cycle.hour != pbuff->TypeA.pram.WriteCycle_ask_TypeA.hour)
         ||(Cycle.minute != pbuff->TypeA.pram.WriteCycle_ask_TypeA.minute))
    {return ;}
  }
  else if(4 == pbuff ->TypeA.head.Length)
  {
    if(0xAB != pbuff->TypeA.pram.WriteCycle_ask_TypeB.DI1)
    {return;}
    Cycle.day = pbuff->TypeA.pram.WriteCycle_ask_TypeB.day;
    Cycle.hour = pbuff->TypeA.pram.WriteCycle_ask_TypeB.hour;
    Cycle.minute = pbuff->TypeA.pram.WriteCycle_ask_TypeB.minute; 
    if(0 == EEPROM_I2C_MultipleWrite(&(Cycle), p,sizeof(AutoCollectCycleStruct)))
    {return ;}
    if(0 == EEPROM_I2C_MultipleRead(&(Cycle), p, sizeof(AutoCollectCycleStruct)))
    {return ;}
    if((Cycle.day != pbuff->TypeA.pram.WriteCycle_ask_TypeB.day)
       ||(Cycle.hour != pbuff->TypeA.pram.WriteCycle_ask_TypeB.hour)
         ||(Cycle.minute != pbuff->TypeA.pram.WriteCycle_ask_TypeB.minute))
    {return ;}
  }
  else
  {return;}
  Concentrator.cycle.day = Cycle.day;
  Concentrator.cycle.hour = Cycle.hour;
  Concentrator.cycle.minute = Cycle.minute;  
  buff = GetIdleBuff_PProtocol ( );
  if(P_NULL == buff){return;}
  buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB; 
  buff->SourcePort = Null_Port;
  buff->TargetPort = InPort_PV;
  buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
  buff->buff.Data.TypeB.head.Cmd = WriteCycle_Concentrator;
  buff->buff.Data.TypeB.head.Length =  sizeof (WriteCycle_Concentrator_ACKstruct);
  buff->buff.Data.TypeB.pram.WriteCycle_ack.DI1 = 0xAB ;
  buff->buff.Data.TypeB.pram.WriteCycle_ack.state[0] = 'O';
  buff->buff.Data.TypeB.pram.WriteCycle_ack.state[1] = 'K';
  if(NO_ERR != PProtocol_send (buff))
  {FreeBuff_PProtocol(buff);}
}
/*********************************************************************************
Function:      //
Description:   //读取各通道起始结束中继器号
Input:         //
//
Output:        //
Return:        //              
Others:         // 
*********************************************************************************/
static void ReadChannel_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff) 
{
  PProtocolSendBuff_Struct*  buff;
  
  if(0xAB != pbuff->TypeA.pram.ReadChannel_ask.DI1)
  {return;}
  
  buff = GetIdleBuff_PProtocol ( );
  if(P_NULL == buff){return;}
  buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB; 
  buff->SourcePort = Null_Port;
  buff->TargetPort = InPort_PV;
  buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
  buff->buff.Data.TypeB.head.Cmd = ReadChannel_Concentrator;
  buff->buff.Data.TypeB.head.Length =  sizeof (ReadChannel_Concentrator_ACKstruct);
  buff->buff.Data.TypeB.pram.ReadChannel_ack.DI1 = 0xAB ;
  buff->buff.Data.TypeB.pram.ReadChannel_ack.channel[0][0]= Concentrator.channel[0].min;
  buff->buff.Data.TypeB.pram.ReadChannel_ack.channel[0][1]= Concentrator.channel[0].max;
  buff->buff.Data.TypeB.pram.ReadChannel_ack.channel[1][0]= Concentrator.channel[1].min;
  buff->buff.Data.TypeB.pram.ReadChannel_ack.channel[1][1]= Concentrator.channel[1].max;
  buff->buff.Data.TypeB.pram.ReadChannel_ack.channel[2][0]= Concentrator.channel[2].min;
  buff->buff.Data.TypeB.pram.ReadChannel_ack.channel[2][1]= Concentrator.channel[2].max;
  if(NO_ERR != PProtocol_send (buff))
  {FreeBuff_PProtocol(buff);}
}
/*********************************************************************************
Function:      //
Description:   //设置集中器各通道中继器起始结束号
Input:         //
//
Output:        //
Return:        //              
Others:         // 
*********************************************************************************/
static void WriteChannel_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
  PProtocolSendBuff_Struct*  buff;
  unsigned long p = 0;
  ChannelConfigStruct        channel[3];    //集中器 各个通道中继器配置 
  
  if(0xAB != pbuff->TypeA.pram.WriteChannel_ask.DI1)
  {return;}
  
  channel[0].min = pbuff->TypeA.pram.WriteChannel_ask.channel[0][0];
  channel[0].max = pbuff->TypeA.pram.WriteChannel_ask.channel[0][1];
  channel[1].min = pbuff->TypeA.pram.WriteChannel_ask.channel[1][0];
  channel[1].max = pbuff->TypeA.pram.WriteChannel_ask.channel[1][1];
  channel[2].min = pbuff->TypeA.pram.WriteChannel_ask.channel[2][0];
  channel[2].max = pbuff->TypeA.pram.WriteChannel_ask.channel[2][1];
  
  p =(unsigned long)(((ConcentratorParameterStruct*) 0)->channel);
  p += ConcentratorParameter_AddrEeprom;
  if(0 == EEPROM_I2C_MultipleWrite(channel, p, sizeof(ChannelConfigStruct)*3))
  {return;}
  Concentrator.channel[0].min = pbuff->TypeA.pram.WriteChannel_ask.channel[0][0];
  Concentrator.channel[0].max = pbuff->TypeA.pram.WriteChannel_ask.channel[0][1];
  Concentrator.channel[1].min = pbuff->TypeA.pram.WriteChannel_ask.channel[1][0];
  Concentrator.channel[1].max = pbuff->TypeA.pram.WriteChannel_ask.channel[1][1];
  Concentrator.channel[2].min = pbuff->TypeA.pram.WriteChannel_ask.channel[2][0];
  Concentrator.channel[2].max = pbuff->TypeA.pram.WriteChannel_ask.channel[2][1];
  
  buff = GetIdleBuff_PProtocol ( );
  if(P_NULL == buff){return;}
  buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;  
  buff->SourcePort = Null_Port;
  buff->TargetPort = InPort_PV;
  buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
  buff->buff.Data.TypeB.head.Cmd = WriteChannel_Concentrator;
  buff->buff.Data.TypeB.head.Length =  sizeof (WriteChannel_Concentrator_ACKstruct);
  buff->buff.Data.TypeB.pram.WriteChannel_ack.DI1 = 0xAB ;
  buff->buff.Data.TypeB.pram.WriteChannel_ack.state[0] = 'O';
  buff->buff.Data.TypeB.pram.WriteChannel_ack.state[1] = 'K';
  if(NO_ERR != PProtocol_send (buff))
  {FreeBuff_PProtocol(buff);}
}
/*********************************************************************************
Function:      //
Description:   //读取是否开启域名链接服务器
Input:         //
//
Output:        //
Return:        //              
Others:         // 
*********************************************************************************/
static void ReadIpEnd_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
  PProtocolSendBuff_Struct*  buff;
  
  if(0xAB != pbuff->TypeA.pram.ReadChannel_ask.DI1)
  {return;}
  
  buff = GetIdleBuff_PProtocol ( );
  if(P_NULL == buff){return;}
  buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;  
  buff->SourcePort = Null_Port;
  buff->TargetPort = InPort_PV;
  buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
  buff->buff.Data.TypeB.head.Cmd = ReadIpEnd_Concentrator;
  buff->buff.Data.TypeB.head.Length =  sizeof (ReadIpEnd_Concentrator_ACKstruct);
  buff->buff.Data.TypeB.pram.ReadIpEnd_ack.DI1 = 0xAB ;
  if(Concentrator.CSR.Bit.ip_flg)
  {buff->buff.Data.TypeB.pram.ReadIpEnd_ack.state = 0;}
  else
  {buff->buff.Data.TypeB.pram.ReadIpEnd_ack.state = 1;}
  if(NO_ERR != PProtocol_send (buff))
  {FreeBuff_PProtocol(buff);}
}
/*********************************************************************************
Function:      //
Description:   //设置开启、关闭使用域名链接服务器 
Input:         //
//
Output:        //
Return:        //              
Others:         // 
*********************************************************************************/
static void WriteIpEnd_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff)
{ 
  CSR_ConcentratorParameter  csr;
  PProtocolSendBuff_Struct*  buff;
  unsigned long p = 0;
  if(0xAB != pbuff->TypeA.pram.WriteChannel_ask.DI1)
  {return;}
  
  csr.Byte = Concentrator.CSR.Byte;
  if(pbuff->TypeA.pram.WriteIpEnd_ask.state)
  {csr.Bit.ip_flg = 0;}
  else
  {csr.Bit.ip_flg = 1;}
  p =(unsigned long)&(((ConcentratorParameterStruct*) 0)->CSR);
  p += ConcentratorParameter_AddrEeprom;
  if(0 == EEPROM_I2C_MultipleWrite(&csr, p, sizeof (CSR_ConcentratorParameter)))
  {return;}
  Concentrator.CSR.Byte =csr.Byte;
  
  buff = GetIdleBuff_PProtocol ( );
  if(P_NULL == buff){return;}
  buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;  
  buff->SourcePort = Null_Port;
  buff->TargetPort = InPort_PV;
  buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
  buff->buff.Data.TypeB.head.Cmd = WriteIpEnd_Concentrator;
  buff->buff.Data.TypeB.head.Length =  sizeof (WriteIpEnd_Concentrator_ACKstruct);
  buff->buff.Data.TypeB.pram.WriteChannel_ack.DI1 = 0xAB ;
  buff->buff.Data.TypeB.pram.WriteChannel_ack.state[0] = 'O';
  buff->buff.Data.TypeB.pram.WriteChannel_ack.state[1] = 'K';
  if(NO_ERR != PProtocol_send (buff))
  {FreeBuff_PProtocol(buff);}
}
/*********************************************************************************
Function:      //
Description:   //读取网络参数
Input:         //
//
Output:        //
Return:        //              
Others:         // 
*********************************************************************************/
static void ReadServerInf_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
  unsigned short itemp;
  unsigned char crc;
  PProtocolSendBuff_Struct*  buff;
  
  if(0xAB != pbuff->TypeA.pram.WriteServerInf_ask.DI1)
  {return;}
  
  buff = GetIdleBuff_PProtocol ( );
  if(P_NULL == buff){return;}
  buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;  
  buff->SourcePort = Null_Port;
  buff->TargetPort = InPort_PV;
  
  buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
  buff->buff.Data.TypeB.head.Cmd = ReadServerInf_Concentrator;
  buff->buff.Data.TypeB.head.Length =  sizeof (ReadServerInf_Concentrator_ACKstruct);
  buff->buff.Data.TypeB.pram.ReadServerInf_ack.DI1= 0xAB;
  
  buff->buff.Data.TypeB.pram.ReadServerInf_ack.IP_M[0]= Concentrator.server[0].ip[0];
  buff->buff.Data.TypeB.pram.ReadServerInf_ack.IP_M[1]= Concentrator.server[0].ip[1];
  buff->buff.Data.TypeB.pram.ReadServerInf_ack.IP_M[2]= Concentrator.server[0].ip[2];
  buff->buff.Data.TypeB.pram.ReadServerInf_ack.IP_M[3]= Concentrator.server[0].ip[3];
  
  buff->buff.Data.TypeB.pram.ReadServerInf_ack.IP_S[0]= Concentrator.server[1].ip[0];
  buff->buff.Data.TypeB.pram.ReadServerInf_ack.IP_S[1]= Concentrator.server[1].ip[1];
  buff->buff.Data.TypeB.pram.ReadServerInf_ack.IP_S[2]= Concentrator.server[1].ip[2];
  buff->buff.Data.TypeB.pram.ReadServerInf_ack.IP_S[3]= Concentrator.server[1].ip[3];
  
  buff->buff.Data.TypeB.pram.ReadServerInf_ack.PORT_M = (Concentrator.server[0].port>> 8) | (Concentrator.server[0].port << 8);
  buff->buff.Data.TypeB.pram.ReadServerInf_ack.PORT_S = (Concentrator.server[1].port>> 8) | (Concentrator.server[1].port << 8);
  
  itemp = 0;
  crc = 0;
  while(crc < 5)
  {
    itemp *= 10;
    itemp += Concentrator.er_tele[crc++];
  }
  buff->buff.Data.TypeB.pram.ReadServerInf_ack.TEL[0] = itemp >> 8;
  buff->buff.Data.TypeB.pram.ReadServerInf_ack.TEL[1] = itemp ;
  itemp = 0;
  while(crc < 9)
  {
    itemp *= 10;
    itemp += Concentrator.er_tele[crc++];
  }
  buff->buff.Data.TypeB.pram.ReadServerInf_ack.TEL[2] = itemp >> 8;
  buff->buff.Data.TypeB.pram.ReadServerInf_ack.TEL[3] = itemp ;
  itemp = 0;
  while(crc < 11)
  {
    itemp *= 10;
    itemp += Concentrator.er_tele[crc++];
  }
  buff->buff.Data.TypeB.pram.ReadServerInf_ack.TEL[4] = itemp;
  if(NO_ERR != PProtocol_send (buff))
  {FreeBuff_PProtocol(buff);}
}
/*********************************************************************************
Function:      //
Description:   // 网络参数设置
Input:         //
//
Output:        //
Return:        //              
Others:         // 
*********************************************************************************/
static void WriteServerInf_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff)
{
  unsigned short itemp ;
  unsigned long itemp2;
  
  PProtocolSendBuff_Struct*  buff;
  if(0xAB != pbuff->TypeA.pram.WriteServerInf_ask.DI1)
  {return;}
  itemp = 0;
  Concentrator.server[0].ip[0] =  pbuff->TypeA.pram.WriteServerInf_ask.IP_M[0];
  Concentrator.server[0].ip[1] =  pbuff->TypeA.pram.WriteServerInf_ask.IP_M[1];
  Concentrator.server[0].ip[2] =  pbuff->TypeA.pram.WriteServerInf_ask.IP_M[2];
  Concentrator.server[0].ip[3] =  pbuff->TypeA.pram.WriteServerInf_ask.IP_M[3];
  Concentrator.server[1].ip[0] =  pbuff->TypeA.pram.WriteServerInf_ask.IP_S[0];
  Concentrator.server[1].ip[1] =  pbuff->TypeA.pram.WriteServerInf_ask.IP_S[1];
  Concentrator.server[1].ip[2] =  pbuff->TypeA.pram.WriteServerInf_ask.IP_S[2];
  Concentrator.server[1].ip[3] =  pbuff->TypeA.pram.WriteServerInf_ask.IP_S[3];
  Concentrator.server[0].port =  (pbuff->TypeA.pram.WriteServerInf_ask.PORT_M >> 8) | (pbuff->TypeA.pram.WriteServerInf_ask.PORT_M<< 8);
  Concentrator.server[1].port =  (pbuff->TypeA.pram.WriteServerInf_ask.PORT_S >> 8) | (pbuff->TypeA.pram.WriteServerInf_ask.PORT_S<< 8);
  itemp2 = pbuff->TypeA.pram.WriteServerInf_ask.TEL[0];
  itemp2 <<= 8;
  itemp2 += pbuff->TypeA.pram.WriteServerInf_ask.TEL[1];
  itemp = 5;
  while (itemp-- > 0 )
  {
    Concentrator.er_tele[itemp] = itemp2 % 10;
    itemp2 /= 10;
  }
  itemp2 = pbuff->TypeA.pram.WriteServerInf_ask.TEL[2];
  itemp2 <<= 8;
  itemp2 += pbuff->TypeA.pram.WriteServerInf_ask.TEL[3];
  itemp = 4;
  while (itemp-- > 0 )
  {
    Concentrator.er_tele[itemp+5] = itemp2 % 10;
    itemp2 /= 10;
  }
  itemp2 = pbuff->TypeA.pram.WriteServerInf_ask.TEL[4];
  itemp = 2;
  while (itemp-- > 0 )
  {
    Concentrator.er_tele[itemp+9] = itemp2 % 10;
    itemp2 /= 10;
  }
  itemp = (unsigned long)&(((ConcentratorParameterStruct*)0)->er_tele[11]);
  itemp2 =(unsigned long)&(((ConcentratorParameterStruct*)0)->server[0]);
  itemp -= itemp2;
  itemp2 += ConcentratorParameter_AddrEeprom;
  if(0 == EEPROM_I2C_MultipleWrite(&(Concentrator.server[0]), itemp2, itemp))
  {return ;}
  buff = GetIdleBuff_PProtocol ( );
  if(P_NULL == buff){return;} 
  buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB; 
  
  buff->SourcePort = Null_Port;
  buff->TargetPort = InPort_PV;
  buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
  buff->buff.Data.TypeB.head.Cmd = WriteServerInf_Concentrator;
  buff->buff.Data.TypeB.head.Length =  sizeof (WriteServerInf_Concentrator_ACKstruct);
  buff->buff.Data.TypeB.pram.WriteChannel_ack.DI1 = 0xAB ;
  buff->buff.Data.TypeB.pram.WriteChannel_ack.state[0] = 'O';
  buff->buff.Data.TypeB.pram.WriteChannel_ack.state[1] = 'K';
  if(NO_ERR != PProtocol_send (buff))
  {FreeBuff_PProtocol(buff);} 
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
static void ReadServer_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff)  //读取集中器域名信息 
{
  unsigned char itemp;
  PProtocolSendBuff_Struct*  buff;
  
  if(0xAB != pbuff->TypeA.pram.WriteServerInf_ask.DI1)
  {return;}
  
  buff = GetIdleBuff_PProtocol ( );
  if(P_NULL == buff){return;}
  buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;  
  buff->SourcePort = Null_Port;
  buff->TargetPort = InPort_PV;
  buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
  buff->buff.Data.TypeB.head.Cmd =pbuff->TypeA.head.Cmd;
  buff->buff.Data.TypeB.pram.ReadServer_ack.DI1 = 0xAB;
  itemp =  0;
  if(ReadServerI_Concentrator == pbuff->TypeA.head.Cmd)
  {
    while(itemp < DomainNameLength)
    {
      if(0 != Concentrator.server[0].DomainName[itemp])
      {
        buff->buff.Data.TypeB.pram.ReadServer_ack.DomainName [itemp]= Concentrator.server[0].DomainName[itemp];
        itemp ++;
      }
      else
      {break;}
    }
    buff->buff.Data.TypeB.pram.ReadServer_ack.DomainName [itemp++] = (Concentrator.server[0].port>> 8);
    buff->buff.Data.TypeB.pram.ReadServer_ack.DomainName [itemp++] =  Concentrator.server[0].port; 
  }
  else
  {
    while(itemp < DomainNameLength)
    {
      if(0 != Concentrator.server[1].DomainName[itemp])
      {
        buff->buff.Data.TypeB.pram.ReadServer_ack.DomainName [itemp]= Concentrator.server[1].DomainName[itemp];
        itemp ++;
      }
      else
      {break;}
    }
    buff->buff.Data.TypeB.pram.ReadServer_ack.DomainName [itemp++] = (Concentrator.server[1].port>> 8);
    buff->buff.Data.TypeB.pram.ReadServer_ack.DomainName [itemp++] =  Concentrator.server[1].port; 
  }
  buff->buff.Data.TypeB.head.Length =  itemp + 1;
  if(NO_ERR != PProtocol_send (buff))
  {FreeBuff_PProtocol(buff);}
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
static void WriteServer_Concentrator_func (KindgDaSPCP_PackageDataStruct* pbuff)//设置集中器域名
{
  unsigned char crc;
  unsigned char itemp ;
  DataServerStruct           server;      
  
  PProtocolSendBuff_Struct*  buff;
  if(0xAB != pbuff->TypeA.pram.WriteServer_ask.DI1)
  {return;}
  itemp = pbuff->TypeA.head.Length - 3;
  crc = 0;
  while(crc < itemp)
  {
    server.DomainName[crc] = pbuff->TypeA.pram.WriteServer_ask.DomainName[crc];
    crc ++;
  }
  while (crc < DomainNameLength)
  {
    server.DomainName[crc] = 0;
    crc ++;
  }
  server.DomainName[DomainNameLength - 1] = 0;
  server.port =  pbuff->TypeA.pram.WriteServer_ask.DomainName[itemp++] ;
  server.port <<= 8;
  server.port |= pbuff->TypeA.pram.WriteServer_ask.DomainName[itemp++]; 
  
  if(WriteServerI_Concentrator == pbuff->TypeA.head.Cmd)
  {
    server.ip[0] = Concentrator.server[0].ip[0];
    server.ip[1] = Concentrator.server[0].ip[1];
    server.ip[2] = Concentrator.server[0].ip[2];
    server.ip[3] = Concentrator.server[0].ip[3];
    
    itemp =(unsigned long)&(((ConcentratorParameterStruct*)0)->server[0]);
    itemp += ConcentratorParameter_AddrEeprom;
    if(0 == EEPROM_I2C_MultipleWrite(&(server), itemp, sizeof (DataServerStruct)))
    {return ;}
    crc = 0;
    while (crc < DomainNameLength)
    {
      Concentrator.server[0].DomainName [crc] = server.DomainName[crc];
      crc ++;
    }
    Concentrator.server[0].port  = server.port;
  }
  else if(WriteServerII_Concentrator == pbuff->TypeA.head.Cmd)
  {
    server.ip[0] = Concentrator.server[1].ip[0];
    server.ip[1] = Concentrator.server[1].ip[1];
    server.ip[2] = Concentrator.server[1].ip[2];
    server.ip[3] = Concentrator.server[1].ip[3];
    
    itemp =(unsigned long)&(((ConcentratorParameterStruct*)0)->server[1]);
    itemp += ConcentratorParameter_AddrEeprom;
    if(0 == EEPROM_I2C_MultipleWrite(&(server), itemp, sizeof (DataServerStruct)))
    {return ;}
    crc = 0;
    while (crc < DomainNameLength)
    {
      Concentrator.server[1].DomainName [crc] = server.DomainName[crc];
      crc ++;
    }
    Concentrator.server[1].port  = server.port;
  }
  buff = GetIdleBuff_PProtocol ( );
  if(P_NULL == buff){return;}
  buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;  
  buff->SourcePort = Null_Port;
  buff->TargetPort = InPort_PV;
  buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
  buff->buff.Data.TypeB.head.Cmd = pbuff->TypeA.head.Cmd;
  buff->buff.Data.TypeB.head.Length =  sizeof (WriteServer_Concentrator_ACKstruct);
  buff->buff.Data.TypeB.pram.WriteServer_ack.DI1 = 0xAB ;
  buff->buff.Data.TypeB.pram.WriteServer_ack.state[0] = 'O';
  buff->buff.Data.TypeB.pram.WriteServer_ack.state[1] = 'K';
  if(NO_ERR != PProtocol_send (buff))
  {FreeBuff_PProtocol(buff);} 
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
static void ReadID_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff) //读取中继器号
{
  unsigned char i;
  Transpond_KingDaSPCP_struct  Trans;
  
  if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
  {
    i = 0;
    while (i < PProtoclSendBuff_Number)
    {
      if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
         && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == ReadID_Repeaters) 
           && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
      {break;}
      i ++;
    }
    if(i < PProtoclSendBuff_Number)
    {
      if(P_NULL != KingDaV1send_buff[i].AckFunc)
      {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // 应答数据处理程序
      else
      {
        if(Null_Port!= KingDaV1send_buff[i].SourcePort)
        {
          Trans.CSR.Bit.Preamble =  0;
          Trans.pbuff = pbuff;
          Trans.SourcePort = Null_Port;
          Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
          Transpond_KingDaSPCP (&Trans); 
        }
      }
      FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //释放缓冲区 
    }  
  }
  else
  {
    Trans.CSR.Bit.Preamble =  0;
    Trans.pbuff = pbuff;
    Trans.SourcePort = InPort_PV;
    Trans.TargetPort = get_MbusChannelNumber ( );
    if(Null_Port == Trans.TargetPort )
    {Trans.TargetPort = Mbus_Port1;}
    Transpond_KingDaSPCP (&Trans); 
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
static void WriteID_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff) //设置中继器号
{
  unsigned char i;
  Transpond_KingDaSPCP_struct  Trans;
  
  if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
  {
    i = 0;
    while (i < PProtoclSendBuff_Number)
    {
      if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
         && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == WriteID_Repeaters) 
           && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
      {break;}
      i ++;
    }
    if(i < PProtoclSendBuff_Number)
    {
      if(P_NULL != KingDaV1send_buff[i].AckFunc)
      {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // 应答数据处理程序
      else
      {
        if(Null_Port!= KingDaV1send_buff[i].SourcePort)
        {
          Trans.CSR.Bit.Preamble =  0;
          Trans.pbuff = pbuff;
          Trans.SourcePort = Null_Port;
          Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
          Transpond_KingDaSPCP (&Trans); 
        }
      }
      FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //释放缓冲区 
    }  
  }
  else
  {
    Trans.CSR.Bit.Preamble =  0;
    Trans.pbuff = pbuff;
    Trans.SourcePort = InPort_PV;
    Trans.TargetPort = get_MbusChannelNumber ( );
    if(Null_Port == Trans.TargetPort )
    {Trans.TargetPort = Mbus_Port1;}
    Transpond_KingDaSPCP (&Trans); 
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
static void OPen_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff)//打开中继器命令 
{
  unsigned char i;
  Transpond_KingDaSPCP_struct  Trans;
  
  if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
  {
    i = 0;
    while (i < PProtoclSendBuff_Number)
    {
      if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
         && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == OPen_Repeaters) 
           && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
      { 
        if((KingDaV1send_buff[i].buff.Data.TypeA.pram.OPen_Repeaters_ask.RepeatersId== pbuff->TypeA.pram.OPen_Repeaters_ack.RepeatersId)
           &&(KingDaV1send_buff[i].buff.Data.TypeA.pram.OPen_Repeaters_ask.ChannelNumber== pbuff->TypeA.pram.OPen_Repeaters_ack.ChannelNumber))
          break; 
      }
      i ++;
    }
    if(i < PProtoclSendBuff_Number)
    {
      if(P_NULL != KingDaV1send_buff[i].AckFunc)
      {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // 应答数据处理程序
      else
      {
        if(Null_Port!= KingDaV1send_buff[i].SourcePort)
        {
          Trans.CSR.Bit.Preamble =  0;
          Trans.pbuff = pbuff;
          Trans.SourcePort = Null_Port;
          Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
          Transpond_KingDaSPCP (&Trans); 
        }
      }
      FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //释放缓冲区 
    }  
  }
  else
  {
    Trans.CSR.Bit.Preamble =  0;
    Trans.pbuff = pbuff;
    Trans.SourcePort = InPort_PV;
    i = pbuff->TypeA.pram.OPen_Repeaters_ask.RepeatersId >> 4;
    i *= 10;
    i += pbuff->TypeA.pram.OPen_Repeaters_ask.RepeatersId & 0x0F; 
    Trans.TargetPort = getPortForAddr (i);
    Transpond_KingDaSPCP (&Trans); 
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
static void Close_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff) //关闭中继器：即使中继器睡眠
{
  unsigned char i;
  Transpond_KingDaSPCP_struct  Trans;
  
  if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
  {
    i = 0;
    while (i < PProtoclSendBuff_Number)
    {
      if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
         && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == Close_Repeaters) 
           && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
      {
        if(KingDaV1send_buff[i].buff.Data.TypeA.pram.Close_Repeaters_ask.RepeatersId== pbuff->TypeA.pram.Close_Repeaters_ack.RepeatersId)
        {break;}
      }
      i ++;
    }
    if(i < PProtoclSendBuff_Number)
    {
      if(P_NULL != KingDaV1send_buff[i].AckFunc)
      {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // 应答数据处理程序
      else
      {
        if(Null_Port!= KingDaV1send_buff[i].SourcePort)
        {
          Trans.CSR.Bit.Preamble =  0;
          Trans.pbuff = pbuff;
          Trans.SourcePort = Null_Port;
          Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
          Transpond_KingDaSPCP (&Trans); 
        }
      }
      FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //释放缓冲区 
    }  
  }
  else
  {
    Trans.CSR.Bit.Preamble =  0;
    Trans.pbuff = pbuff;
    Trans.SourcePort = InPort_PV;
    Trans.TargetPort = getPortForAddr (pbuff->TypeA.pram.Close_Repeaters_ask.RepeatersId);
    Transpond_KingDaSPCP (&Trans); 
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
static void ReadChannelNodeNumber_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff) //读取中继器N下各个通道水表数量
{
  unsigned char i;
  Transpond_KingDaSPCP_struct  Trans;
  
  if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
  {
    i = 0;
    while (i < PProtoclSendBuff_Number)
    {
      if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
         && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == ReadChannelNodeNumber_Repeaters) 
           && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
      {
        if(KingDaV1send_buff[i].buff.Data.TypeA.pram.ReadChannelNodeNumber_Repeaters_ask.id 
           == pbuff->TypeA.pram.ReadChannelNodeNumber_Repeaters_ack.id)
        {break;}
      }
      i ++;
    }
    if(i < PProtoclSendBuff_Number)
    {
      if(P_NULL != KingDaV1send_buff[i].AckFunc)
      {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // 应答数据处理程序
      else
      {
        if(Null_Port!= KingDaV1send_buff[i].SourcePort)
        {
          Trans.CSR.Bit.Preamble =  0;
          Trans.pbuff = pbuff;
          Trans.SourcePort = Null_Port;
          Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
          Transpond_KingDaSPCP (&Trans); 
        }
      }
      FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //释放缓冲区 
    }  
  }
  else
  {
    Trans.CSR.Bit.Preamble =  0;
    Trans.pbuff = pbuff;
    Trans.SourcePort = InPort_PV;
    i = pbuff->TypeA.pram.ReadChannelNodeNumber_Repeaters_ask.id >> 4;
    i *= 10;
    i += pbuff->TypeA.pram.ReadChannelNodeNumber_Repeaters_ask.id & 0x0F; 
    Trans.TargetPort = getPortForAddr (i );
    Transpond_KingDaSPCP (&Trans); 
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
static void ReadGprsRssi_func (KindgDaSPCP_PackageDataStruct* pbuff) //读取信号强度
{
  unsigned char itemp;
  unsigned char crc;
  PProtocolSendBuff_Struct*  buff;
  
  if(0xAB != pbuff->TypeA.pram.WriteServerInf_ask.DI1)
  {return;}
  
  buff = GetIdleBuff_PProtocol ( );
  if(P_NULL == buff){return;}
  buff->CCR.Bit.TypeA_B = PProtoco_PackageTypeB;
  buff->SourcePort = Null_Port;
  buff->TargetPort = InPort_PV;
  buff->buff.Data.TypeB.head.Sync= PProtocl_Sync_Up;
  buff->buff.Data.TypeB.head.Cmd = pbuff->TypeA.head.Cmd;
  buff->buff.Data.TypeB.head.Length =  sizeof(ReadGprsRssi_ACKstruct);
  buff->buff.Data.TypeB.pram.ReadGprsRssi_ack.DI1 = 0xAB;
  buff->buff.Data.TypeB.pram.ReadGprsRssi_ack.Rssi = gprs.Rssi;
  itemp = sizeof (PPro_PackageHead_TypeB) +  sizeof(ReadGprsRssi_ACKstruct);;
  crc = CRC8((unsigned char *) &(buff->buff.Data.TypeB.head),itemp);  
  ((unsigned char *) &(buff->buff.Data.TypeB.head))[itemp] = crc;
  if(NO_ERR != PProtocol_send (buff))
  {FreeBuff_PProtocol(buff);} 
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
static void WriteChannelNodeNumber_Repeaters_func (KindgDaSPCP_PackageDataStruct* pbuff) //设置中继器各通道水表数量
{
  unsigned char i;
  Transpond_KingDaSPCP_struct  Trans;
  
  if(PProtocl_Sync_Up == pbuff->TypeA.head.Sync)
  {
    i = 0;
    while (i < PProtoclSendBuff_Number)
    {
      if((0 == KingDaV1send_buff[i].CCR.Bit.idle)
         && (KingDaV1send_buff[i].buff.Data.TypeA.head.Cmd == WriteChannelNodeNumber_Repeaters) 
           && (KingDaV1send_buff[i].TargetPort == InPort_PV)) 
      {
        if(KingDaV1send_buff[i].buff.Data.TypeA.pram.WriteChannelNodeNumber_Repeaters_ask.id 
           == pbuff->TypeA.pram.WriteChannelNodeNumber_Repeaters_ack.id)
        {break;}
      }
      i ++;
    }
    if(i < PProtoclSendBuff_Number)
    {
      if(P_NULL != KingDaV1send_buff[i].AckFunc)
      {KingDaV1send_buff[i].AckFunc(&(KingDaV1send_buff[i]), pbuff);} // 应答数据处理程序
      else
      {
        if(Null_Port!= KingDaV1send_buff[i].SourcePort)
        {
          Trans.CSR.Bit.Preamble =  0;
          Trans.pbuff = pbuff;
          Trans.SourcePort = Null_Port;
          Trans.TargetPort = KingDaV1send_buff[i].SourcePort;
          Transpond_KingDaSPCP (&Trans); 
        }
      }
      FreeBuff_PProtocol(&KingDaV1send_buff[i]);  //释放缓冲区 
    }  
  }
  else
  {
    Trans.CSR.Bit.Preamble =  0;
    Trans.pbuff = pbuff;
    Trans.SourcePort = InPort_PV;
    
    i = pbuff->TypeA.pram.WriteChannelNodeNumber_Repeaters_ask.id >> 4;
    i *= 10;
    i += pbuff->TypeA.pram.WriteChannelNodeNumber_Repeaters_ask.id & 0x0F; 
    
    
    Trans.TargetPort = getPortForAddr (i);
    Transpond_KingDaSPCP (&Trans); 
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
static unsigned short  BinChangeToBCD (unsigned char bin)
{
  unsigned char itemp = 0;
  unsigned char data[5];
  unsigned short re = 0;
  
  while (itemp < 4)
  {
    data[itemp++] = bin % 10;
    bin /= 10;
  }
  while(itemp -- >0)
  {
    re <<= 4; 
    re |= data[itemp]; 
  }
  return re;
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
static SystemErrName PProtocol_send (PProtocolSendBuff_Struct *pbuff)
{
  unsigned short itemp;
  unsigned short length ;
  if((P_NULL == pbuff) || (Null_Port == pbuff->TargetPort))
  {return IllegalParam;}
  
  if(PProtoco_PackageTypeA == pbuff->CCR.Bit.TypeA_B)
  {
    if(0 != pbuff->CCR.Bit.Preamble)
    {
      if(PProtocl_Sync_Down == pbuff->buff.Data.TypeA.head.Sync) 
      {
        pbuff->buff.Prm1 = PProtocl_Preamble_Down;
        pbuff->buff.Prm2 = PProtocl_Preamble_Down;
      }
      else
      {
        pbuff->buff.Prm1 = PProtocl_Preamble_Up;
        pbuff->buff.Prm2 = PProtocl_Preamble_Up;
      }
      length =  2 + sizeof (PPro_PackageHead_TypeA) + 1;
    }
    else
    {
      length =  sizeof (PPro_PackageHead_TypeA) + 1;
    }
    if(PProtocl_Sync_Down == pbuff->buff.Data.TypeA.head.Sync) 
    {pbuff->CCR.Bit.AckEN =0x01; }
    length += pbuff->buff.Data.TypeA.head.Length; 
  }
  else
  {
    if(0 != pbuff->CCR.Bit.Preamble)
    {
      if(PProtocl_Sync_Down == pbuff->buff.Data.TypeB.head.Sync) 
      {
        pbuff->buff.Prm1 = PProtocl_Preamble_Down;
        pbuff->buff.Prm2 = PProtocl_Preamble_Down;
      }
      else
      {
        pbuff->buff.Prm1 = PProtocl_Preamble_Up;
        pbuff->buff.Prm2 = PProtocl_Preamble_Up;
      }
      length =  2 + sizeof (PPro_PackageHead_TypeB) ;
    }
    else
    {
      length =  sizeof (PPro_PackageHead_TypeB);
    } 
    if(PProtocl_Sync_Down == pbuff->buff.Data.TypeB.head.Sync) 
    {pbuff->CCR.Bit.AckEN =0x01; }
    length += pbuff->buff.Data.TypeB.head.Length; 
    itemp = pbuff->buff.Data.TypeB.head.Length >> 8;
    itemp |= pbuff->buff.Data.TypeB.head.Length << 8;
    pbuff->buff.Data.TypeB.head.Length = itemp; 
    itemp = CRC8((unsigned char *) &(pbuff->buff.Data),length);
    ((unsigned char *) &(pbuff->buff.Data))[length ++] = itemp; //crc;
    
  }
  pbuff->DataLength = length;
  return  OnSceneComm_send(KingDaV1,pbuff,length,pbuff->TargetPort,P_NULL,P_NULL);
}
/*********************************************************************************
Function:      //
Description:   //读取指定节点的数值
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
SystemErrName ReadNodeValue_KindDa(unsigned char* NoderAddr,
                                   unsigned char SendPort,  
                                   CommPortNumber_Type SourcePort,
                                   KindgDaSPCPCmdCode_AckFunc AckFunc)
{
  ReadValue_Node_ASKstruct data;
  KindgDaCmd_Struct cmd;
  
  data.addr[0] = NoderAddr[0];
  data.addr[1] = NoderAddr[1];
  data.addr[2] = NoderAddr[2];
  data.NC = 0;
  
  cmd.Cmd = ReadValue;               //命令码
  cmd.SourcePort = SourcePort;
  if(AutomatchPort == SendPort)
  {cmd.TargetPort = NoderAddr[0];}  //发送的预期地址
  else 
  { cmd.TargetPort = SendPort;}   //发送的预期地址       
  cmd.Param = &data;                //命令参数
  cmd.length = sizeof(ReadValue_Node_ASKstruct);   //命令参数长度
  cmd.AckFunc = AckFunc;
  cmd.CSR.Bit.DIR = DOWN;
  cmd.CSR.Bit.Preamble = 1;
  return SendCmd_KindDaSPCP(&cmd);
}
/*********************************************************************************
Function:      //
Description:   //操作指定节点阀门
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
SystemErrName  OperateValve_KindDa(CmdCode_KindgDaSPCP Operate,
                                   unsigned char* NoderAddr,
                                   unsigned char SendPort,
                                   CommPortNumber_Type SourcePort,                                   
                                   KindgDaSPCPCmdCode_AckFunc AckFunc)
{
  ReadValue_Node_ASKstruct data;
  KindgDaCmd_Struct cmd;
  
  if((Open_OV == Operate) ||(Open_OV_ENF == Operate))
  {Operate = Open_OV_ENF;}
  else if((Close_OV == Operate) ||(Close_OV_ENF == Operate))
  {Operate = Close_OV_ENF;}
  else
  {return CmdValid;}
  
  data.addr[0] = NoderAddr[0];
  data.addr[1] = NoderAddr[1];
  data.addr[2] = NoderAddr[2];
  data.NC = 0;
  
  cmd.Cmd = Operate;               //命令码
  cmd.SourcePort = SourcePort;
  if(AutomatchPort == SendPort)
  {cmd.TargetPort = NoderAddr[0];}  //发送的预期地址
  else 
  { cmd.TargetPort = SendPort;}   //发送的预期地址   
  cmd.Param = &data;                //命令参数
  cmd.length = sizeof(ReadValue_Node_ASKstruct);   //命令参数长度
  cmd.AckFunc = AckFunc;
  cmd.CSR.Bit.DIR = DOWN;
  cmd.CSR.Bit.Preamble = 1;
  return SendCmd_KindDaSPCP(&cmd);
}
/*********************************************************************************
Function:      //
Description:   //打开指定中继器下指定通道
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
SystemErrName OpenNETChannel_KindDa ( unsigned char RepeatersId, 
                                     unsigned char ChannelNumber,
                                     CommPortNumber_Type SourcePort,
                                     KindgDaSPCPCmdCode_AckFunc AckFunc)
{
  unsigned char itemp;
  OPen_Repeaters_ASKstruct data;
  KindgDaCmd_Struct cmd;
  
  data.RepeatersId   = RepeatersId % 10; 
  itemp = RepeatersId /10;
  itemp %= 10;
  itemp <<= 4;
  data.RepeatersId  |= itemp;
  data.ChannelNumber = ChannelNumber;
  cmd.SourcePort = SourcePort;
  cmd.Cmd = OPen_Repeaters;      //命令码
  cmd.TargetPort = RepeatersId;           //发送的预期地址        
  cmd.Param = &data;                //命令参数
  cmd.length = sizeof(OPen_Repeaters_ASKstruct);   //命令参数长度
  cmd.AckFunc = AckFunc;
  cmd.CSR.Bit.DIR = DOWN;
  return SendCmd_KindDaSPCP(&cmd);
}
/*********************************************************************************
Function:      //
Description:   //关闭指定中继器下指定通道
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
SystemErrName CloseNETChannel_KindDa ( unsigned char RepeatersId, 
                                      unsigned char ChannelNumber,
                                      CommPortNumber_Type SourcePort,
                                      KindgDaSPCPCmdCode_AckFunc AckFunc)
{
  unsigned char itemp;
  Close_Repeaters_ASKstruct data;
  KindgDaCmd_Struct cmd;
  
  data.RepeatersId   = RepeatersId % 10; 
  itemp = RepeatersId /10;
  itemp %= 10;
  itemp <<= 4;
  data.RepeatersId |= itemp;
  data.Zero = 0;
  cmd.SourcePort = SourcePort;
  cmd.Cmd = Close_Repeaters;      //命令码
  cmd.TargetPort = RepeatersId;   //发送的预期地址        
  cmd.Param = &data;             //命令参数
  cmd.length = sizeof(Close_Repeaters_ASKstruct);   //命令参数长度
  cmd.AckFunc = AckFunc;
  cmd.CSR.Bit.DIR = DOWN;
  return SendCmd_KindDaSPCP(&cmd);
}
/*********************************************************************************
Function:      //
Description:   //关闭指定中继器下指定通道
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
SystemErrName CloseRepeatersId_KindDa ( unsigned char RepeatersId, 
                                       CommPortNumber_Type SourcePort,
                                       KindgDaSPCPCmdCode_AckFunc AckFunc)
{
  unsigned char itemp;
  Close_Repeaters_ASKstruct data;
  KindgDaCmd_Struct cmd;
  
  data.RepeatersId   = RepeatersId % 10; 
  itemp = RepeatersId /10;
  itemp %= 10;
  itemp <<= 4;
  data.RepeatersId  |= itemp;
  data.Zero = 0;
  cmd.SourcePort = SourcePort;
  cmd.Cmd = Close_Repeaters;      //命令码
  cmd.TargetPort = RepeatersId;   //发送的预期地址        
  cmd.Param = &data;             //命令参数
  cmd.length = sizeof(Close_Repeaters_ASKstruct);   //命令参数长度
  cmd.AckFunc = AckFunc;
  cmd.CSR.Bit.DIR = DOWN;
  return SendCmd_KindDaSPCP(&cmd);
}
/*********************************************************************************
Function:      //
Description:   //读取制定中继器各通道节点数量
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
SystemErrName ReadRepeatersNodeNumber_KindDa ( unsigned char RepeatersId,
                                              CommPortNumber_Type SourcePort,
                                              KindgDaSPCPCmdCode_AckFunc AckFunc)
{
  unsigned char itemp;
  ReadChannelNodeNumber_Repeaters_ASKstruct data;
  KindgDaCmd_Struct cmd;
  
  data.id   = RepeatersId % 10; 
  itemp = RepeatersId /10;
  itemp %= 10;
  itemp <<= 4;
  data.id  |= itemp;
  data.NC  = 0; 
  cmd.SourcePort = SourcePort;
  cmd.Cmd = ReadChannelNodeNumber_Repeaters;      //命令码
  cmd.TargetPort = RepeatersId;                   //发送的预期地址        
  cmd.Param = &data;                              //命令参数
  cmd.length = sizeof(ReadChannelNodeNumber_Repeaters_ASKstruct);   //命令参数长度
  cmd.AckFunc = AckFunc;
  cmd.CSR.Bit.DIR = DOWN;
  cmd.CSR.Bit.Preamble = 0;
  return SendCmd_KindDaSPCP(&cmd); 
}
/******************************************END********************************************************/