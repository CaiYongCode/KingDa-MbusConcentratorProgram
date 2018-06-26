/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\CJT-188-2004.h"
#include "..\inc\MeterList.h"
#include "..\inc\Includes.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
/*********************************************************************************
公共变量定义区
*********************************************************************************/
unsigned  char CJT188_Valve_AckFlg = 0;

unsigned  char MbusMonitor_EN = 0;
/*********************************************************************************
外部变量声明区
*********************************************************************************/
/*********************************************************************************
私有变量定义区
*********************************************************************************/
static CJT188_SER_Type  SER_PV = 0;    //包序列号 

static struct CJT188_SendBuff_Struct  CJT188SendBuff[2] ;
/*********************************************************************************
测试变量定义区
*********************************************************************************/
/*********************************************************************************
外部函数定声明
*********************************************************************************/
/*********************************************************************************
内部函数定义区
*********************************************************************************/
static struct CJT188_SendBuff_Struct* get_CJT188SendBuff (CommPortNumber_Type Port);
static SystemErrName free_CJT188SendBuff (void* pbuff);
static SystemErrName CJT188_2004_AddrVerify (CJT188_AddresStruct* addr);
static CJT188_Frame_Struct* CJT188_2004_frameVerify (void *pbuff, unsigned char length, SystemErrName* err);
static SystemErrName CJT188_2004_AckVerify (CJT188_Frame_Struct *ask, CJT188_Frame_Struct* ack);
static SystemErrName CJT188_2004_Transfer (CJT188_Frame_Struct* InPutFrame,CommPortNumber_Type TargetPort);
static unsigned char  CJT188_Package_AskFrame (CJT188_Package_Struct* Package);
static unsigned char  CJT188_Package_AckFrame (CJT188_Package_Struct*  Package);
static CJT188_Frame_Struct* CJT188_2004_AckWeit (struct CJT188_SendBuff_Struct *ask, SystemErrName* err );
static CommPortNumber_Type getChannelNumberForID ( CJT188_AddresStruct* id); 
static void ReadAddress_AckFill(CJT188_Package_Struct* AckFrame, CJT188_Frame_Struct* AskFrame);
static void WriteAddress_AckFill(CJT188_Package_Struct* AckFrame,  CJT188_Frame_Struct* AskFrame);

SystemErrName Comm_send(void* Data, 
                        unsigned long DataLength, 
                        CommPortNumber_Type TargetPort,
                        SerialPort_SendEndCallBack_Type callBack, 
                        void *callBack_arg );
/*********************************************************************************
功能代码定义区
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void CJT188_2004_init (void)
{
  CJT188SendBuff[0].mutex = xSemaphoreCreateMutex();       //互斥信号量 EEPROM
  CJT188SendBuff[1].mutex = xSemaphoreCreateMutex();       //互斥信号量 EEPROM
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
SystemErrName CJT188_2004_InPut (void *pbuff, unsigned length,CommPortNumber_Type SourcePort)
{
  SystemErrName err = NO_ERR;
  unsigned short itemp;
  CommPortNumber_Type  TargetPort;
  CJT188_Frame_Struct* InPutFrame;
  struct CJT188_SendBuff_Struct*  SendBuff;
  
  InPutFrame =  CJT188_2004_frameVerify((unsigned char *)pbuff, length, &err);
  if(NO_ERR != err)
  {return err;}
  
  if(0 != InPutFrame->Head.CTR.Bit.DIR)
  {return NO_ERR;}
  
  if(CJT188_ReadAddress_CTR == InPutFrame->Head.CTR.Bit.Infr)
  {
    err =  CJT188_2004_AddrVerify (&(InPutFrame->Head.Addr));
    if(NO_ERR !=  err)
    {return err;}
    SendBuff = get_CJT188SendBuff (Null_Port);
    if(P_NULL == SendBuff)
    {return   PortBusy;}
    ReadAddress_AckFill(&SendBuff->Package, InPutFrame);
    itemp = CJT188_Package_AckFrame(&SendBuff->Package);
    Comm_send(&SendBuff->Package, itemp, SourcePort, P_NULL, P_NULL);
  }
  else if(CJT188_WriteAddress_CTR == InPutFrame->Head.CTR.Bit.Infr)
  {
    err =  CJT188_2004_AddrVerify (&(InPutFrame->Head.Addr));
    if(NO_ERR !=  err)
    {return err;}
    SendBuff = get_CJT188SendBuff (Null_Port);
    if(P_NULL == SendBuff)
    {return   PortBusy;}
    WriteAddress_AckFill( &SendBuff->Package, InPutFrame);
    itemp = CJT188_Package_AckFrame(&SendBuff->Package);
    Comm_send(&SendBuff->Package, itemp, SourcePort, P_NULL, P_NULL);
  }
  else
  {
    //向下转发
    TargetPort = getChannelNumberForID ( &(InPutFrame->Head.Addr));
    SendBuff = get_CJT188SendBuff (TargetPort);
    if(P_NULL == SendBuff)
    {return   PortBusy;}
    
    if((Mbus_Port1 ==TargetPort )
       ||(Mbus_Port2 ==TargetPort)
         ||(Mbus_Port3 ==TargetPort )
           ||(Mbus_Port4 ==TargetPort))
    {
      if(pdTRUE == xSemaphoreTake( Mbus_xMutex, _100ms * 3 ))
      {
        SendBuff->SourcePort = SourcePort;
        SendBuff->TargetPort = TargetPort;
        itemp = 0;
        while(itemp < CJT188_PreambleLength)
        {SendBuff->Package.Preamble[itemp++] = CJT188_Preamble;}
        itemp = InPutFrame->Head.Length + sizeof (CJT188_FrameHeadStruct) + sizeof (CJT188_FrameStern_Struct); 
        CopyBuff (&(SendBuff->Package.Frame), InPutFrame, itemp);
        SendBuff->PackageLength = itemp + CJT188_PreambleLength;
        config_Mbus(TargetPort, 2400, Parity_Even, StopBits_1,ByDMA);
        InPutFrame = CJT188_2004_AckWeit (SendBuff, &err);
        if(NO_ERR == err)
        {
          if((Mbus_Port1 !=SendBuff->SourcePort )
             &&(Mbus_Port2 !=SendBuff->SourcePort)
               &&(Mbus_Port3 !=SendBuff->SourcePort )
                 &&(Mbus_Port4 !=SendBuff->SourcePort))
          { 
            xSemaphoreGive(Mbus_xMutex);
            if(Null_Port != SendBuff->SourcePort)
            {CJT188_2004_Transfer (InPutFrame,SendBuff->SourcePort);} 
          }
          else
          {
            CJT188_2004_Transfer (InPutFrame,SendBuff->SourcePort);
            xSemaphoreGive(Mbus_xMutex);
          }
        }
        else
        {xSemaphoreGive(Mbus_xMutex);}
      }
      else
      {err = PortBusy;}
    }
  }
  free_CJT188SendBuff (SendBuff);
  return err;  
}
/*********************************************************************************
 Function:      //
 Description:   // 抄读水表下发命令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName CJT188_ReadDataFormNode (CJT188_DI DI, 
                                       CJT188_MeterDeclarator *Meter, 
                                       CommPortNumber_Type Port, 
                                       CJT188_Frame_Struct*  ack)
{
  SystemErrName err;
  struct CJT188_SendBuff_Struct*  SendBuff;
  CJT188_Frame_Struct*  ackframe;

  SendBuff = get_CJT188SendBuff (Port);
  if(P_NULL == SendBuff)
  {return PortBusy;}

  SendBuff->SourcePort = Null_Port;
  SendBuff->TargetPort = Port;
  SendBuff->Package.Frame.Head.CTR.Byte =  0;
  SendBuff->Package.Frame.Head.CTR.Bit.Infr = CJT188_ReadData_CTR;  //读数据 
  SendBuff->Package.Frame.Head.Type = Meter->Type;
  CopyBuff (&SendBuff->Package.Frame.Head.Addr, &(Meter->Addr), sizeof (CJT188_AddresStruct));
  SendBuff->Package.Frame.Head.Length = sizeof(CJT188_DataHeadStruct);
  SendBuff->Package.Frame.Data.OK.DataHead.DI =  DI;   //
  SendBuff->PackageLength = CJT188_Package_AskFrame (&SendBuff->Package);
  
  ackframe = CJT188_2004_AckWeit (SendBuff, &err);
  
  if(NO_ERR == err )
  {CopyBuff (ack, ackframe, ackframe->Head.Length + sizeof(CJT188_FrameHeadStruct) + sizeof (CJT188_FrameStern_Struct));}
  
/*****************************************
  为了兼容不同厂商的ST状态做出的处理
*****************************************/
  if(ack->Data.OK.Data.MeterData.ST.Word == 0x0000)//对于不带阀门部分厂商ST状态转为我们的格式
  {
     ack->Data.OK.Data.MeterData.ST.Bit.Custom = 2;//强制转为我们适应的ST状态
  }
  else if(ack->Data.OK.Data.MeterData.ST.Bit.Custom == 0xff)//针对带阀门的其他厂商的表 ST2为0XFF的处理
  {
    ack->Data.OK.Data.MeterData.ST.Bit.Custom = 0;
    if(ack->Data.OK.Data.MeterData.ST.Bit.Valve_State == 0)//阀门开状态处理
      ack->Data.OK.Data.MeterData.ST.Bit.Custom |= 0x02;
    else if(ack->Data.OK.Data.MeterData.ST.Bit.Valve_State == 2)//阀门异常处理
    {
      ack->Data.OK.Data.MeterData.ST.Bit.Custom |= 0x17;  //阀门丢失并报故障
    }
  }
/***************************************/
  
  free_CJT188SendBuff (SendBuff);
  
  return err;
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
SystemErrName CJT188_WriteDataToNode(CJT188_DI DI, 
                                     CJT188_MeterDeclarator* Meter, 
                                     CommPortNumber_Type Port, 
                                     void* Data, 
                                     unsigned char DataLength,
                                     CJT188_Frame_Struct*  ack)
{
  SystemErrName err;
  struct CJT188_SendBuff_Struct*  SendBuff;
  CJT188_Frame_Struct*  ackframe;
  
  if(DataLength > sizeof(CJT188_FrameParamLength))
  {return IllegalParam;}
  
  SendBuff = get_CJT188SendBuff (Port);
  if(P_NULL == SendBuff)
  {return PortBusy;}
  
  SendBuff->SourcePort = Null_Port;
  SendBuff->TargetPort = Port;
  SendBuff->Package.Frame.Head.CTR.Byte =  0;
  SendBuff->Package.Frame.Head.CTR.Bit.Infr = CJT188_WriteData_CTR;  //写数据 
  SendBuff->Package.Frame.Head.Type = Meter->Type;
  CopyBuff (&SendBuff->Package.Frame.Head.Addr, &(Meter->Addr), sizeof (CJT188_AddresStruct));
  SendBuff->Package.Frame.Head.Length = sizeof(CJT188_DataHeadStruct) + DataLength;
  SendBuff->Package.Frame.Data.OK.DataHead.DI =  DI;   //
  CopyBuff (&SendBuff->Package.Frame.Data.OK.Data, Data, DataLength);
  SendBuff->PackageLength = CJT188_Package_AskFrame (&SendBuff->Package);
  
  ackframe = CJT188_2004_AckWeit (SendBuff, &err);
  
  if(NO_ERR == err )
  {CopyBuff (ack, ackframe, ackframe->Head.Length + sizeof(CJT188_FrameHeadStruct) + sizeof (CJT188_FrameStern_Struct));}
  
  free_CJT188SendBuff (SendBuff);
  
  return err;
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
static void ReadAddress_AckFill(CJT188_Package_Struct*  AckFrame,CJT188_Frame_Struct* AskFrame)
{
  
 AckFrame->Frame.Head.CTR.Bit.Infr = CJT188_ReadAddress_CTR;
 AckFrame->Frame.Head.CTR.Bit.DIR = 0x01;
 AckFrame->Frame.Head.CTR.Bit.CommFlg = 0x00;
 AckFrame->Frame.Head.Length = 3;
 
 AckFrame->Frame.Data.OK.DataHead.DI = CJT188_ReadAddress_DI;
 AckFrame->Frame.Data.OK.DataHead.SER = AskFrame->Data.OK.DataHead.SER; 
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
static void WriteAddress_AckFill(CJT188_Package_Struct*  AckFrame,CJT188_Frame_Struct* AskFrame)
{
  unsigned long  RommAdddr;
  unsigned char addr[sizeof(CJT188_AddresStruct)];
  RommAdddr = sizeof(CJT188_AddresStruct);
  while (RommAdddr-- > 0)
  {addr[RommAdddr] = ((unsigned char *)&AskFrame->Data.OK.Data)[sizeof(CJT188_AddresStruct)- 1- RommAdddr];}
  vTaskSuspendAll();  //调度锁开启
  
  RommAdddr =  (unsigned long)(&(((Server188_ConfigData_Stuct*)0)->ID));
  RommAdddr += Server188_ConfigData_AddrEeprom;                              
  EEROM_Write(RommAdddr,addr,sizeof(CJT188_AddresStruct));                              
  //校验存储
  EEROM_Read (RommAdddr, &(_188_ConcenConfigData.ID), sizeof(CJT188_AddresStruct));
  xTaskResumeAll ();  //调度锁关闭
  
  AckFrame->Frame.Head.CTR.Bit.Infr = CJT188_WriteAddress_CTR;
  AckFrame->Frame.Head.CTR.Bit.DIR = 0x01;
  AckFrame->Frame.Head.CTR.Bit.CommFlg = 0x00;
  AckFrame->Frame.Head.Length = 3;
  AckFrame->Frame.Data.OK.DataHead.DI = CJT188_WriteAddress_DI;
  AckFrame->Frame.Data.OK.DataHead.SER = AskFrame->Data.OK.DataHead.SER; 
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
static struct CJT188_SendBuff_Struct* get_CJT188SendBuff (CommPortNumber_Type Port)
{
  switch ( Port )
  {
  case Null_Port :
    {
      if(pdTRUE == xSemaphoreTake(CJT188SendBuff[1].mutex , _100ms * 10))
      {return &CJT188SendBuff[1];}
      break;
    }
  case Mbus_Port1 :
  case Mbus_Port2 :
  case Mbus_Port3 :
  case Mbus_Port4 :  
    {
      if(pdTRUE == xSemaphoreTake(CJT188SendBuff[0].mutex , _100ms * 10))
      {return &CJT188SendBuff[0];}
      break;
    }
    
  default:
  {break;}    
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
 Others:        //
*********************************************************************************/
static SystemErrName free_CJT188SendBuff (void* pbuff)
{
 SystemErrName err = NO_ERR;
 if((&CJT188SendBuff[0] == pbuff)||(&(CJT188SendBuff[0].Package) == &(((struct CJT188_SendBuff_Struct*)pbuff)->Package)))
 {xSemaphoreGive(CJT188SendBuff[0].mutex);}
 else if((&CJT188SendBuff[1] == pbuff)||(&(CJT188SendBuff[1].Package) == &(((struct CJT188_SendBuff_Struct*)pbuff)->Package)))
 {xSemaphoreGive(CJT188SendBuff[1].mutex);}
 else
 {err = Unsuccessful;}
  return err;
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
static CommPortNumber_Type getChannelNumberForID ( CJT188_AddresStruct* id) 
{
  CommPortNumber_Type  Port = Mbus_Port1;
  
  return Port;
}
/*********************************************************************************
 Function:      //
 Description:   // 水表读取返回
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static CJT188_Frame_Struct* CJT188_2004_frameVerify (void *pbuff, unsigned char length, SystemErrName* err)
{
  unsigned char itemp ;
  unsigned char itemp2 ;
  unsigned char itemp3 ;
  unsigned short itemp4 = 0 ;
  
  CJT188_Frame_Struct *frame;
  do{
    //搜寻 起始字符
    while(length >= sizeof (CJT188_StartCharacter))
    {
      if(CJT188_StartCharacter ==((unsigned char *)pbuff)[itemp4])   //搜寻
      {
        frame =(CJT188_Frame_Struct *)(&((unsigned char *)pbuff)[itemp4 ++]);
        break; 
      }
      itemp4 ++;
      length --;
    }
    if(length < sizeof (CJT188_StartCharacter))
    {
      *err = FrameSync_err;
      return frame; 
    }
    //长度校验
    itemp = frame->Head.Length;
    itemp += sizeof(CJT188_FrameHeadStruct) + 2;
    if(length < itemp)
    {
      *err = Length_err;
      return frame;
    }
    //校验 和   
    itemp = 0;
    itemp2 = frame->Head.Length + sizeof(CJT188_FrameHeadStruct);
    itemp3 = 0;
    while(itemp < itemp2)
    {
      itemp3 +=  ((unsigned char *)frame)[itemp];
      itemp ++;
    }
    if(itemp3 == ((unsigned char *)frame)[itemp])
    {    
      //校验 操作码 
      if((CJT188_ReadData_CTR == frame->Head.CTR.Bit.Infr)
         ||(CJT188_ReadVerNo_CTR == frame->Head.CTR.Bit.Infr)
           ||(CJT188_ReadAddress_CTR == frame->Head.CTR.Bit.Infr)
             ||(CJT188_WriteData_CTR == frame->Head.CTR.Bit.Infr)
               ||(CJT188_WriteAddress_CTR == frame->Head.CTR.Bit.Infr))
      {
        *err = NO_ERR;
        return frame;
      }
      else
      {
        *err = CmdValid;
        return frame;
      }
    }
  }while(itemp4 < length);
  
  *err = DataValid;
  return frame;
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
static SystemErrName CJT188_2004_AddrVerify (CJT188_AddresStruct* addr)
{
  
  if((0xAAAA != addr->FactoryID) 
     || (0xAA != addr->MeterID[0])
       || (0xAA != addr->MeterID[1])
         || (0xAA != addr->MeterID[2])
           || (0xAA != addr->MeterID[3])
             ||(0xAA != addr->MeterID[4]))
  {
    if((_188_ConcenConfigData.ID[0] != (addr->FactoryID >>8)) 
       || (_188_ConcenConfigData.ID[1] != (addr->FactoryID & 0x00FF))
         || (_188_ConcenConfigData.ID[2] != addr->MeterID[4])
           || (_188_ConcenConfigData.ID[3] != addr->MeterID[3])
             || (_188_ConcenConfigData.ID[4] != addr->MeterID[2])
               ||(_188_ConcenConfigData.ID[5] != addr->MeterID[1])
                  ||(_188_ConcenConfigData.ID[6] != addr->MeterID[0]))
    {
      //地址不匹配
      return DataValid;
    }
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
static CJT188_Frame_Struct* CJT188_2004_AckWeit (struct CJT188_SendBuff_Struct *ask, SystemErrName* err )
{
  static unsigned char buff[128];
  unsigned char SendCnt;
  unsigned char queryCnt;
  unsigned short buffLength;
  UsarBuff_Struct* ReceiveData;
  CJT188_Frame_Struct* AckFrame = P_NULL;
  
  SendCnt = 0;
  do{
    //发送数据
    Comm_send(ask->Package.Preamble, ask->PackageLength, ask->TargetPort, P_NULL, P_NULL);
    
    queryCnt = 0;
    do{
      if(pdPASS == xQueueReceive(MbusReceiveQueue, &ReceiveData, _100ms * 1))
      {
        buffLength = GetDataFromUsart (ReceiveData, buff, 64);
        if( 0 != MbusMonitor_EN )
        {Comm_send(buff, buffLength, Rs232_Port, P_NULL, P_NULL);}
        AckFrame =  CJT188_2004_frameVerify(buff, buffLength, err);
       if(NO_ERR == *err)
       {
         *err = CJT188_2004_AckVerify (&ask->Package.Frame,AckFrame);
         if(NO_ERR == *err)
         { break;}
       }
      }
    }while(queryCnt++ < 8);
    if(queryCnt < 8)
    {break;}
  }while(SendCnt++ < 15);
  
  if(SendCnt >= 15)
  {*err = CommAck_Err;}
  else
  {*err = NO_ERR;}
  
  return AckFrame;
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
static SystemErrName CJT188_2004_AckVerify (CJT188_Frame_Struct *ask, CJT188_Frame_Struct* ack)
{
  if(ack->Head.CTR.Bit.Infr != ask->Head.CTR.Bit.Infr)
  {
    //命令码错误
     return DataValid;
  }
  if(UnknownMeterType != ask->Head.Type)
  {
    if(ack->Head.Type != ask->Head.Type)
    {
      //表计类型错误
      return DataValid;
    } 
  }
  
  if((0xAAAA != ask->Head.Addr.FactoryID) 
     || (0xAA != ask->Head.Addr.MeterID[0])
       || (0xAA != ask->Head.Addr.MeterID[1])
         || (0xAA != ask->Head.Addr.MeterID[2])
           || (0xAA != ask->Head.Addr.MeterID[3])
             ||(0xAA != ask->Head.Addr.MeterID[4]))
  {
    if((ack->Head.Addr.FactoryID != ask->Head.Addr.FactoryID) 
       || (ack->Head.Addr.MeterID[0] != ask->Head.Addr.MeterID[0])
         || (ack->Head.Addr.MeterID[1] != ask->Head.Addr.MeterID[1])
           || (ack->Head.Addr.MeterID[2] != ask->Head.Addr.MeterID[2])
             || (ack->Head.Addr.MeterID[3] != ask->Head.Addr.MeterID[3])
               ||(ack->Head.Addr.MeterID[4] != ask->Head.Addr.MeterID[4]))
    {
      //地址不匹配
      return DataValid;
    }
  }
  if(ack->Data.OK.DataHead.SER != ask->Data.OK.DataHead.SER)
  {return DataValid;}
  if( 0 != ack->Head.CTR.Bit.CommFlg)
  {
    if(ack->Data.ERR.SER != ask->Data.ERR.SER)
    {
      //序列号不匹配
      return DataValid; 
    }
    else
    {
      //1：通信异常
      return CommErr;
    } 
  }
  else
  {
    //0：通信正常
    if(ack->Data.OK.DataHead.SER != ask->Data.OK.DataHead.SER)
    {
      //序列号不匹配
      return DataValid;
    }
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
static SystemErrName CJT188_2004_Transfer (CJT188_Frame_Struct* InPutFrame,
                                           CommPortNumber_Type TargetPort)
{
  static CJT188_Package_Struct  sendbuff ;
  unsigned short DataLength;
  
  if(Null_Port != TargetPort)
  {
    DataLength = 0;
    while(DataLength < CJT188_PreambleLength)
    {sendbuff.Preamble[DataLength++] = CJT188_Preamble;}
    DataLength = InPutFrame->Head.Length + sizeof (CJT188_FrameHeadStruct) + sizeof (CJT188_FrameStern_Struct);
    CopyBuff (&(sendbuff.Frame), InPutFrame, DataLength);
    DataLength += CJT188_PreambleLength;
    
    //发送数据
    return Comm_send(sendbuff.Preamble, DataLength, TargetPort,P_NULL, P_NULL);
    
  }
  return IllegalParam; 
}
/*********************************************************************************
 Function:      //
 Description:   //封包 协议数据包
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static unsigned char CJT188_Package_AskFrame (CJT188_Package_Struct* Package)
{
  unsigned char cs;
  unsigned char itemp,  itemp2;
  CJT188_FrameStern_Struct  *Stern;
  
  itemp  = 0;
  while(itemp < CJT188_PreambleLength)
  {Package->Preamble[itemp++] = CJT188_Preamble;}
  
  Package->Frame.Head.StartCharacter = CJT188_StartCharacter;
  
  Package->Frame.Data.OK.DataHead.SER = ++SER_PV;
  
  itemp = 0;
  itemp2 = Package->Frame.Head.Length + sizeof(CJT188_FrameHeadStruct);
  cs = 0;
  while(itemp < itemp2)
  {
    cs +=  ((unsigned char *)&Package->Frame)[itemp++];
  }
  Stern = (CJT188_FrameStern_Struct*)&(Package->Frame.Data.buff[Package->Frame.Head.Length]);
  Stern->CS = cs;
  Stern->Pause = CJT188_Pause; 
 itemp = sizeof (CJT188_FrameStern_Struct) + CJT188_PreambleLength ;
 itemp += itemp2 ;
 return itemp;
}
/*********************************************************************************
 Function:      //
 Description:   //封包 协议数据包
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static unsigned char  CJT188_Package_AckFrame (CJT188_Package_Struct*  Package) 
{
 unsigned char cs;
  unsigned char itemp,  itemp2;
  CJT188_FrameStern_Struct  *Stern;
  
  itemp  = 0;
  while(itemp < CJT188_PreambleLength)
  {Package->Preamble[itemp++] = CJT188_Preamble;}
  
  Package->Frame.Head.StartCharacter = CJT188_StartCharacter;
  Package->Frame.Head.Type = Locall_MeterType;
  
  Package->Frame.Head.Addr.FactoryID = _188_ConcenConfigData.ID[0];
  Package->Frame.Head.Addr.FactoryID <<= 8;
  Package->Frame.Head.Addr.FactoryID += _188_ConcenConfigData.ID[1];
  Package->Frame.Head.Addr.MeterID[4] = _188_ConcenConfigData.ID[2];
  Package->Frame.Head.Addr.MeterID[3] = _188_ConcenConfigData.ID[3];
  Package->Frame.Head.Addr.MeterID[2] = _188_ConcenConfigData.ID[4];
  Package->Frame.Head.Addr.MeterID[1] = _188_ConcenConfigData.ID[5];
  Package->Frame.Head.Addr.MeterID[0] = _188_ConcenConfigData.ID[6];
  
  //memcpy(&(Package->Frame.Head.Addr),_188_ConcenConfigData.ID, sizeof(CJT188_AddresStruct));
  
  itemp = 0;
  itemp2 = Package->Frame.Head.Length + sizeof(CJT188_FrameHeadStruct);
  cs = 0;
  while(itemp < itemp2)
  {
    cs +=  ((unsigned char *)&Package->Frame)[itemp++];
  }
  Stern = (CJT188_FrameStern_Struct*)&(Package->Frame.Data.buff[Package->Frame.Head.Length]);
  Stern->CS = cs;
  Stern->Pause = CJT188_Pause; 
 itemp = sizeof (CJT188_FrameStern_Struct) + sizeof(CJT188_FrameHeadStruct) + CJT188_PreambleLength ;
 itemp += Package->Frame.Head.Length ;
 return itemp;
}
//******************************************END********************************************************/

