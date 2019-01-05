/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\Server188.h"
#include "..\inc\Includes.h"
#include "..\inc\MeterList.h"
/*********************************************************************************
常量定义区
*********************************************************************************/

/*********************************************************************************
公共变量定义区
*********************************************************************************/
Server188_ConfigData_Stuct     _188_ConcenConfigData;
Server188_ConcenMbusState_Type  ReadingMeterState = DownIdle;  
unsigned char ReportAllCurrentDataFinish = 1;
/*********************************************************************************
外部变量声明区
*********************************************************************************/
/*********************************************************************************
私有变量定义区
*********************************************************************************/ 
static Server188_OutPutBuff_Struct  Server188_OutPutBuff[Server188_OutPutBuff_Number];
  
static unsigned short   OperateRcmd_0N [6];
/*********************************************************************************
测试变量定义区
*********************************************************************************/

/*********************************************************************************
内部函数定义区
*********************************************************************************/
//static SystemErrName Server188_frameVerify (Server188_FrameDatas_Struct *frame, unsigned char length);
//设置集中器时间
static SystemErrName TimeWcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData);
//读取集中器时间
static SystemErrName TimeRcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData);
//设置集中器抄表参数
static SystemErrName ConfigDataWcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData);
//读取集中器抄表参数
static SystemErrName ConfigDataRcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData);	
//设置集中器GPRS参数
static SystemErrName GprsWcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData);	
//读取集中器GPRS参数
static SystemErrName GprsRcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData);	
//下载表计地址	
static SystemErrName MeterWcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData);		
//读取下载成功表计地址
static SystemErrName MeterRcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData);		
//读取下载失败表计地址
static SystemErrName MeterErrRcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData);	
//下达控制命令
static SystemErrName OperateWcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData);	
//读取控制命令
static SystemErrName OperateRcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData);		
//启动立即抄表
static SystemErrName ReadingWcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData);	
//读取某一采集器的所有表计当前数据
static SystemErrName AllCurrentDataRcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData);	
//读取某一采集器的所有表计日冻结数据	
static SystemErrName AllFreezingDataRcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData);	
//读取单表的当前数据
static SystemErrName SingCurrentDataRcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData);	
//读取单表的日冻结数据
static SystemErrName SingFreezingDataRcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData);	
//替换表计
static SystemErrName AlterMeterWcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData);
//删除表计
static SystemErrName DeleteMeterWcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData);

static SystemErrName CleanMeterWcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData);


static SystemErrName ReadingSteup_Func( void );
static SystemErrName config_Mbus_188(CommPortNumber_Type Port, Server188_CommDeclarator Comm);
/*********************************************************************************
功能代码定义区
*********************************************************************************/

SystemErrName Comm_send(void* Data, 
                        unsigned long DataLength, 
                        CommPortNumber_Type TargetPort,
                        SerialPort_SendEndCallBack_Type callBack, 
                        void *callBack_arg );
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void Server188_SPCP_init(void)
{
 unsigned char itemp = 0;
 while (itemp < Server188_OutPutBuff_Number)
 {
   Server188_OutPutBuff[itemp].CSR.Bit.idle = 1;
   itemp++;
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
static void Frame_Pack(Server188_Frame_Struct* frame)
{
   
 unsigned char  sum;
 unsigned short itemp;
 unsigned long Length;  
 unsigned char *pin; 
           
  frame->Preamble = Server188_Preamble;   //填充  同步字
  frame->Datas.Head.StartCharacter = Server188_StartCharacter;  //填充  帧头
  frame->Datas.Head.LAI = Server188_AreaCode;       //填充  集中器 区域码
  
  //memcpy(frame->Datas.Head.Concentrator,&(_188_ConcenConfigData.ID), sizeof(CJT188_AddresStruct));
  itemp = 0;
  do{
    frame->Datas.Head.Concentrator[itemp]  = _188_ConcenConfigData.ID[itemp];
  }while (++itemp < ConcentratorAddr_Length);
 //计算校验和
 itemp = 0;
 sum = 0;
 Length = frame->Datas.Head.DataLength.Byte.Byte_H;
 Length <<= 8;
 Length += frame->Datas.Head.DataLength.Byte.Byte_L;
 Length += sizeof(Server188_FrameHead_Struct);
 pin = &frame->Datas.Head.StartCharacter; 
 while(itemp++ < Length)
 {sum += *pin++;}
 //填充帧尾
 ((Server188_FrameStern_Struct*)pin)->CS = sum;
 ((Server188_FrameStern_Struct*)pin)->Pause = Server188_Pause;
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
Server188_OutPutBuff_Struct* getServer188SendBuff(CommPortNumber_Type port)
{
 unsigned char itemp = 0;
 while (itemp < Server188_OutPutBuff_Number)
 {
   if(1 == Server188_OutPutBuff[itemp].CSR.Bit.idle)
   {
     Server188_OutPutBuff[itemp].CSR.Bit.idle = 0;
     //Frame_init(&(Server188_OutPutBuff[itemp].Frame));
     Server188_OutPutBuff[itemp].SourcePort = port;
     return (&(Server188_OutPutBuff[itemp]));
   }
   itemp ++;
 }
 return (P_NULL);
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
void freeServer188SendBuff(void* frame )
{
 unsigned char itemp = 0;
 while (itemp < Server188_OutPutBuff_Number)
 {
   if(frame == &(Server188_OutPutBuff[itemp])
      ||frame == &(Server188_OutPutBuff[itemp].Frame))
   {
     Server188_OutPutBuff[itemp].CSR.Bit.idle = 1;
     break;
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
Others:         // 
*********************************************************************************/
Server188_Frame_Struct* CreateHandshakeFrame(unsigned char* Length) 
{
  unsigned char itemp; 
  unsigned char* data;
  Server188_OutPutBuff_Struct*  SendBuff;
  *Length = 0;
  SendBuff = getServer188SendBuff(Null_Port);
  if(P_NULL == SendBuff)
  {return P_NULL;}
  
  //memcpy(&(SendBuff->Frame),"KINGDA", sizeof(Server188_TimerData_Stuct));
  //strcpy((char *)&(SendBuff->Frame), "KINGDA");
  //strncat((char *)&(SendBuff->Frame),(_188_ConcenConfigData.ID),ConcentratorAddr_Length);
  //strncat((char *)&(SendBuff->Frame),(_188_ConcenConfigData.ID),ConcentratorAddr_Length);
  if(pdTRUE == xSemaphoreTake( Gprs_xMutex, _1s * 1 ))
  {
    _CSQ_func( );
    xSemaphoreGive( Gprs_xMutex);
  }
  
  data = (unsigned char*) &SendBuff->Frame;
  data[(*Length) ++]= 'K';
  data[(*Length) ++]= 'I';
  data[(*Length) ++]= 'N';
  data[(*Length) ++]= 'G';
  data[(*Length) ++]= 'D';
  data[(*Length) ++]= 'A';
  itemp = 0;
  while (itemp <  ConcentratorAddr_Length)
  {data[(*Length) ++] = _188_ConcenConfigData.ID[itemp++];}

  data[(*Length) ++] = gprs.Rssi;
  data[(*Length) ++ ] = 118;
  
  return &(SendBuff->Frame);
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
SystemErrName Server188Hand_AckVerify(unsigned char* AckData, unsigned short DataLength)
{
  const unsigned char AckMark[]= "kingda";
  unsigned short itemp = 0;
  unsigned char  itemp2 = 0;
  unsigned char  itemp3 = 0;
  
  while (itemp-- < DataLength)
  { 
    if('k' == *AckData)
    {
      itemp2 = 0;
      while ('\0' != AckMark[itemp2])
      {
        if(AckMark[itemp2] != AckData[itemp2])
        {break;}
        else
        {itemp2 ++;}
      }
      if('\0' == AckMark[itemp2])
      { 
        itemp3 = 0;
        while (itemp3  < ConcentratorAddr_Length)
        {
          if(AckData[itemp2] != _188_ConcenConfigData.ID[itemp3++])
          {break;}
          itemp2++;
        }
        if(ConcentratorAddr_Length == itemp3)
        {
          if( 'o'==(AckData[itemp2++]))
          {
            if( 'k'==(AckData[itemp2++]))
            {return NO_ERR;}
          }
        }
      } 
    }
    AckData ++;
  }
  return DataValid; 
}
/*********************************************************************************
 Function:      //
 Description:   // 协议入口 
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName Server188_InPut (void *pbuff, unsigned short length,CommPortNumber_Type port)
{
  SystemErrName err = NO_ERR;
  unsigned short itemp;
  Server188_FrameDatas_Struct *inpackage;
  Server188_OutPutBuff_Struct* ack;
  Server188_InputData_Stuct    InputData;
  Server188_OututData_Stuct    OutputData;
  
  itemp = 0;
  while(length >= sizeof (Server188_FrameHead_Struct))
  {
    if(Server188_StartCharacter ==((unsigned char *)pbuff)[itemp])
    {
      inpackage =(Server188_FrameDatas_Struct *)(&((unsigned char *)pbuff)[itemp]);
      err = Server188_frameVerify(inpackage, length);
      if(NO_ERR == err) 
      {break;}
    }
    itemp ++;
    length --;
  }
  if(length <= sizeof (Server188_FrameHead_Struct))
  {return FrameInvalid;}
  
  ack =  getServer188SendBuff(port);
  if(P_NULL == ack)
  {return Nothing;}
  
  InputData.pdata.Param = &(inpackage->Param);
  InputData.port = port;
  OutputData.pdata.Param = &(ack->Frame.Datas.Param);
  OutputData.length = 0;
  switch (inpackage->Head.Code)
  {
  case Server188_Time_Wcmd:                                     //设置集中器时间
    {
      if(sizeof (Server188_TimeWcmd_askParam_Stuct) == inpackage->Head.DataLength.Word)
      {err = TimeWcmd_Func(&InputData, &OutputData);}
      else{err = Length_err;}
      break;
    }
  case Server188_Time_Rcmd:                                     //读取集中器时间
    {
      if(sizeof (Server188_TimeRcmd_askParam_Stuct) == inpackage->Head.DataLength.Word)
      { err = TimeRcmd_Func(&InputData, &OutputData);}
      else{err = Length_err;}
      break;
    }
  case Server188_ConfigData_Wcmd:                               //设置集中器抄表参数
    {
      if(sizeof (Server188_ConfigDataWcmd_askParam_Stuct) == inpackage->Head.DataLength.Word)
      {err = ConfigDataWcmd_Func(&InputData, &OutputData);}
      else{ err = Length_err;}
      break;
    }
  case Server188_ConfigData_Rcmd:                               //读取集中器抄表参数
    {
      if(sizeof (Server188_TimeRcmd_askParam_Stuct) == inpackage->Head.DataLength.Word)
      {err = ConfigDataRcmd_Func(&InputData, &OutputData); }
      else{err = Length_err;}
      break;
    } 
  case Server188_Gprs_Wcmd:                                     //设置集中器GPRS参数
    {
      if(sizeof (Server188_GprsWcmd_askParam_Stuct) == inpackage->Head.DataLength.Word)
      { err = GprsWcmd_Func(&InputData, &OutputData); }
      else{err = Length_err;}
      break;
    }     
  case Server188_Gprs_Rcmd:                                     //读取集中器GPRS参数
    {
      if(sizeof (Server188_TimeRcmd_askParam_Stuct) == inpackage->Head.DataLength.Word)
      {err = GprsRcmd_Func(&InputData, &OutputData);}
      else{err = Length_err;} 
      break;
    } 
  case Server188_Meter_Wcmd:                                    //下载表计地址
    {
      err = MeterWcmd_Func(&InputData, &OutputData);
      break;
    }
  case Server188_Meter_Rcmd:                                    //读取下载成功表计地址
    {
      err = MeterRcmd_Func(&InputData, &OutputData);
      break;
    }    
  case Server188_MeterErr_Rcmd:                                 //读取下载失败表计地址
    {
      err = MeterErrRcmd_Func(&InputData, &OutputData);
      break;
    }  	
  case Server188_Operate_Wcmd:                                  //下达控制命令
    {
      err = OperateWcmd_Func(&InputData, &OutputData);
      break;
    } 
  case Server188_Operate_Rcmd:                                  //读取控制命令
    {
      err = OperateRcmd_Func(&InputData, &OutputData);
      break;
    } 
  case Server188_Reading_Wcmd:                                  //启动立即抄表
    {
      err = ReadingWcmd_Func(&InputData, &OutputData);
      break;
    } 
  case Server188_AllCurrentData_Rcmd:                           //读取某一采集器的所有表计当前数据
  case Server188_AllCurrentDatas_Rcmd:                           //读取某一采集器的所有表计当前数据（包含温度）
    {
      err = AllCurrentDataRcmd_Func(&InputData, &OutputData);
      inpackage->Head.Code = Server188_AllCurrentDatas_Rcmd;      //将功能码强制转换成读取所有表计当前数据（包含温度）
      break;
    }     
  case Server188_AllFreezingData_Rcmd:                          //读取某一采集器的所有表计日冻结数据
    { 
      err = AllFreezingDataRcmd_Func(&InputData, &OutputData);
      break;
    } 
  case Server188_SingCurrentData_Rcmd:                          //读取单表的当前数据
    { 
      err = SingCurrentDataRcmd_Func(&InputData, &OutputData);
      break;
    }     
  case Server188_SingFreezingData_Rcmd:                         //读取单表的日冻结数据
    {
      err = SingFreezingDataRcmd_Func(&InputData, &OutputData);
      break;
    }  
  case Server188_AlterMeter_Wcmd:                               //替换表计
    {
      err = AlterMeterWcmd_Func(&InputData, &OutputData);
      break;
    }
  case Server188_DeleteMeter_Wcmd:                              //删除表计
    {
      err = DeleteMeterWcmd_Func(&InputData, &OutputData);
      break;
    }   
  case Server188_CleanMeter_cmd:                                //清空表计
    {
      if(sizeof (Server188_CleanMeterWcmd_askParam_Stuct) == inpackage->Head.DataLength.Word)
      { err = CleanMeterWcmd_Func(&InputData, &OutputData);}
      else{err = Length_err;}
      break;
    }    
  default:
    {err =  OperateCmd_err;}
  }
  if(NO_ERR != err) 
  {
    freeServer188SendBuff(ack);
    return err;
  }
  itemp = inpackage->Head.DataLength.Byte.Byte_H;
  itemp <<= 8;
  itemp += inpackage->Head.DataLength.Byte.Byte_L;
  //填充 功能码 
  ack->Frame.Datas.Head.Code = inpackage->Head.Code;
  //填充 序列号 SER
  ack->Frame.Datas.Param.Data[OutputData.length++] = inpackage->Param.Data[itemp - 1]; 
  //填充 数据域长度 
  ack->Frame.Datas.Head.DataLength.Byte.Byte_H = OutputData.length >>8;
  ack->Frame.Datas.Head.DataLength.Byte.Byte_L = OutputData.length;
  
  err = Server188_Send(port, &(ack->Frame));
  freeServer188SendBuff(ack);
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
SystemErrName Server188_frameVerify (Server188_FrameDatas_Struct *frame, unsigned short length)
{
 unsigned short itemp ;
 unsigned short  itemp2 ;
 unsigned char  sum;
 
 if(Server188_AreaCode != frame->Head.LAI)
 {return FrameInvalid;}
 
 itemp = frame->Head.DataLength.Byte.Byte_H;
 itemp <<= 8;
 itemp += frame->Head.DataLength.Byte.Byte_L;
 itemp += sizeof(Server188_FrameHead_Struct);
 itemp += sizeof(Server188_FrameStern_Struct);    //校验和 + 结束符
 if(length < itemp)
 {return Length_err;}
 
 itemp2 = 0;
 while(itemp2 < ConcentratorAddr_Length)
 {
  if(frame->Head.Concentrator[itemp2] != _188_ConcenConfigData.ID[itemp2])
  {return AddrValid;}
  itemp2 ++;
 }
 
 itemp2 = 0;
 sum = 0; 
 itemp -= sizeof(Server188_FrameStern_Struct); 
 while(itemp2 < itemp)
 {
  sum += ((unsigned char *)frame)[itemp2];
  itemp2 ++;
 }
 if(sum != ((unsigned char *)frame)[itemp])
 {return Verify_err;}
 return NO_ERR; 
}
/*********************************************************************************
 Function:      //
 Description:   //设置集中器时间
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName TimeWcmd_Func(Server188_InputData_Stuct* Indata, 
                                   Server188_OututData_Stuct* OutputData)
{
  SystemErrName err = NO_ERR;
  unsigned char itemp;
  TimerData_Struct timer;
  
  if( 0 != BCDToInt8 (&(Indata->pdata.TimeWcmd_ask->Year),2,&itemp))
  {
    timer.Year = 2000 + itemp;
    if(timer.Year < 2017)
    {err = Warning;}
  }else
  {err = Warning;}
  
  if( 0 != BCDToInt8 (&(Indata->pdata.TimeWcmd_ask->Month),2,&itemp))
  {
    timer.Month = itemp;
    if(timer.Month > 12)
    {err = Warning;}
  }else
  {err = Warning;}
  
  if( 0 != BCDToInt8 (&(Indata->pdata.TimeWcmd_ask->Day),2,&itemp))
  {
    timer.Day = itemp;
    if(timer.Day > getDaysInMonth (timer.Year,timer.Month))
    {err = Warning;}
  }else
  {err = Warning;}
  
  if( 0 != BCDToInt8 (&(Indata->pdata.TimeWcmd_ask->Hour),2,&itemp))
  {
    timer.Hour = itemp;
    if(timer.Hour > 23)
    {err = Warning;}
  }else
  {err = Warning;}
  
  if( 0 != BCDToInt8 (&(Indata->pdata.TimeWcmd_ask->Minute),2,&itemp))
  {
    timer.Minute = itemp;
    if(timer.Minute > 59)
    {err = Warning;}
  }else
  {err = Warning;} 
  
  
  if( 0 != BCDToInt8 (&(Indata->pdata.TimeWcmd_ask->Second),2,&itemp))
  {
    timer.Second = itemp;
    if(timer.Second > 59)
    {err = Warning;}
  }else
  {err = Warning;} 
  
  
  if( 0 != BCDToInt8 (&(Indata->pdata.TimeWcmd_ask->WeekDay),2,&itemp))
  {
    timer.WeekDay = itemp;
    if(timer.WeekDay > 7)
    {err = Warning;}
  }else
  {err = Warning;} 
  
  if(NO_ERR == err)
  {
    vTaskSuspendAll();  //调度锁开启
    PCF8563_Set_Time(&timer);
    xTaskResumeAll ();  //调度锁关闭
    OutputData->pdata.TimeWcmd_ack->ReplyFlag = 0x00;
  }
  else
  {OutputData->pdata.TimeWcmd_ack->ReplyFlag = 0xCC;}
  
  OutputData->length = sizeof(Server188_TimeWcmd_ackParam_Stuct);
  return NO_ERR;
}
/*********************************************************************************
 Function:      //读取集中器时间
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName TimeRcmd_Func(Server188_InputData_Stuct* Indata, 
                                   Server188_OututData_Stuct* OutputData)
{
  TimerData_Struct timer;

  get_SystemTimer(&timer);
  if(timer.Year > 2000)
  {timer.Year -= 2000;}
  else
  {timer.Year = 0;}
  Int8ToBCD (timer.Year, &(OutputData->pdata.TimeRcmd_ack->Year), 2);
  Int8ToBCD (timer.Month, &(OutputData->pdata.TimeRcmd_ack->Month), 2);
  Int8ToBCD (timer.Day, &OutputData->pdata.TimeRcmd_ack->Day, 2);
  Int8ToBCD (timer.Hour, &OutputData->pdata.TimeRcmd_ack->Hour, 2);
  Int8ToBCD (timer.Minute, &OutputData->pdata.TimeRcmd_ack->Minute, 2);
  Int8ToBCD (timer.Second, &OutputData->pdata.TimeRcmd_ack->Second, 2);
  Int8ToBCD (timer.WeekDay, &OutputData->pdata.TimeRcmd_ack->WeekDay, 2);

  OutputData->pdata.TimeRcmd_ack->ReplyFlag = 0x00;
  
  OutputData->length = sizeof(Server188_TimeRcmd_ackParam_Stuct);
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //设置集中器抄表参数
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName ConfigDataWcmd_Func(Server188_InputData_Stuct* Indata, 
                                         Server188_OututData_Stuct* OutputData)
{
  unsigned long RommAdddr;
  
  vTaskSuspendAll();  //调度锁开启
  
  memcpy(&(_188_ConcenConfigData.timer),&(Indata->pdata.ConfigDataWcmd_ask->data), sizeof(Server188_TimerData_Stuct));
  //保存抄表数据
  RommAdddr =  (unsigned long)(&(((Server188_ConfigData_Stuct*)0)->timer));
  RommAdddr += Server188_ConfigData_AddrEeprom;                              
  EEROM_Write(RommAdddr,&(_188_ConcenConfigData.timer),sizeof(Server188_TimerData_Stuct));                              
  //校验存储
  EEROM_Read (RommAdddr, &(_188_ConcenConfigData.timer), sizeof(Server188_TimerData_Stuct));
  
  if(0 == memcmp((const void *)&(_188_ConcenConfigData.timer),(const void *)&(Indata->pdata.ConfigDataWcmd_ask->data),sizeof(Server188_TimerData_Stuct)))
  { OutputData->pdata.ConfigDataWcmd_ack->ReplyFlag = 0x00; }  //正常 
  else
  { OutputData->pdata.ConfigDataWcmd_ack->ReplyFlag = 0xCC; }  //数据写入eeprom不正确
  
  xTaskResumeAll ();  //调度锁关闭
  
  OutputData->length = sizeof(Server188_ConfigDataWcmd_ackParam_Stuct);
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //读取集中器抄表参数
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName ConfigDataRcmd_Func(Server188_InputData_Stuct* Indata, 
                                   Server188_OututData_Stuct* OutputData)	
{
  memcpy(&(OutputData->pdata.ConfigDataRcmd_ack->Data), &(_188_ConcenConfigData.timer),sizeof(Server188_TimerData_Stuct));
  OutputData->pdata.ConfigDataRcmd_ack->ReplyFlag = 0;
  
  OutputData->length = sizeof(Server188_ConfigDataRcmd_ackParam_Stuct);
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //设置集中器GPRS参数
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName GprsWcmd_Func(Server188_InputData_Stuct* Indata, 
                                   Server188_OututData_Stuct* OutputData)
{
  unsigned long  RommAdddr;

  vTaskSuspendAll();  //调度锁开启
  
  memcpy(&(_188_ConcenConfigData.gprs),&(Indata->pdata.GprsWcmd_ask->data), sizeof(Server188_Gprs_Stuct));
  //保存抄表数据
  RommAdddr =  (unsigned long)(&(((Server188_ConfigData_Stuct*)0)->gprs));
  RommAdddr += Server188_ConfigData_AddrEeprom;                              
  EEROM_Write(RommAdddr, &(_188_ConcenConfigData.gprs), sizeof(Server188_Gprs_Stuct));                              
  //校验存储
  EEROM_Read (RommAdddr, &(_188_ConcenConfigData.gprs), sizeof(Server188_Gprs_Stuct));
  
  if(0 == memcmp((const void *)&(_188_ConcenConfigData.gprs),(const void *)&(Indata->pdata.GprsWcmd_ask->data),sizeof(Server188_Gprs_Stuct)))
  {OutputData->pdata.GprsWcmd_ack->ReplyFlag = 0x00;} //正常 
  else
  {OutputData->pdata.GprsWcmd_ack->ReplyFlag = 0xCC;}//数据写入eeprom不正确
  
  xTaskResumeAll ();  //调度锁关闭

  OutputData->length = sizeof(Server188_GprsWcmd_ackParam_Stuct);
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //读取集中器GPRS参数
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName GprsRcmd_Func(Server188_InputData_Stuct* Indata, 
                                   Server188_OututData_Stuct* OutputData)
{
 memcpy(&(OutputData->pdata.GprsRcmd_ack->Data), &(_188_ConcenConfigData.gprs),sizeof(Server188_Gprs_Stuct));
 OutputData->pdata.GprsRcmd_ack->ReplyFlag = 0;
   
 OutputData->length = sizeof(Server188_GprsRcmd_ackParam_Stuct);
 return NO_ERR; 
}
/*********************************************************************************
 Function:      //
 Description:   //下载表计地址	
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName MeterWcmd_Func(Server188_InputData_Stuct* Indata, 
                                    Server188_OututData_Stuct* OutputData)		
{
  SystemErrName  err = NO_ERR; 
  unsigned char Channel = 0x00;
  unsigned char MetNumber = 0;
  unsigned short RomAddr;
  Server188_MeterRecordData_Struct  Record;
  
  if((Indata->pdata.MeterWcmd_ask->MetNumber <= 25 )
     &&(Indata->pdata.MeterWcmd_ask->MetNumber > 0))
  {
    while(MetNumber < Indata->pdata.MeterWcmd_ask->MetNumber)
    {
      if((0x01 ==Indata->pdata.MeterWcmd_ask->Meter[MetNumber].Link.Bit.Channel)
         ||(0x02 ==Indata->pdata.MeterWcmd_ask->Meter[MetNumber].Link.Bit.Channel)
           ||(0x03 ==Indata->pdata.MeterWcmd_ask->Meter[MetNumber].Link.Bit.Channel)
             ||(0x04 ==Indata->pdata.MeterWcmd_ask->Meter[MetNumber].Link.Bit.Channel))
      {
        if(Channel != Indata->pdata.MeterWcmd_ask->Meter[MetNumber].Link.Bit.Channel)
        {
          RomAddr = 0;
          Channel = Indata->pdata.MeterWcmd_ask->Meter[MetNumber].Link.Bit.Channel;
        }
        else
        {RomAddr ++;}
        memset(&Record, 0, sizeof(Server188_MeterRecord_Struct));
        memcpy(&(Record.Config),&(Indata->pdata.MeterWcmd_ask->Meter[MetNumber]), sizeof(Server188_Meter_Stuct));
        err = Add_MeterRecordToRom(&Record, &RomAddr);
        if(NO_ERR != err)
        {break;}
      }
      else
      {break;}
      MetNumber ++;
    }
    if(NO_ERR== err)
    {OutputData->pdata.MeterWcmd_ack->ReplyFlag =  0x00;}
    else if(MeterRepeat== err)
    {OutputData->pdata.MeterWcmd_ack->ReplyFlag =  0x04;}  //地址重复 = 0x04,
    else
    {OutputData->pdata.MeterWcmd_ack->ReplyFlag =  0x11;}   // 数据写入eeprom不正确 = 
  }
  else
  {OutputData->pdata.MeterWcmd_ack->ReplyFlag =  0x22;}  //参数不合法 = 0x22,

  OutputData->length = sizeof(Server188_MeterWcmd_ackParam_Stuct);
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //读取下载成功表计地址
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName MeterRcmd_Func(Server188_InputData_Stuct* Indata, 
                                    Server188_OututData_Stuct* OutputData)	
{
  static unsigned short   MeterRcmd_ON [5] = {0,0,0,0,0};
  SystemErrName  err = NO_ERR;  
  unsigned short itemp;
  unsigned char *ReplyFlag;
  unsigned short* ReadAddr;
  Server188_MeterListSpecData_Struct   List;
  Server188_MeterRecordData_Struct     Record; 
  
  if(NO_ERR != Read_MeterListSpecFormRom(&List))
  {
    OutputData->pdata.MeterRcmd_ack->Spec.PackageSum = Indata->pdata.MeterRcmd_ask->PackageNumber;
    OutputData->pdata.MeterRcmd_ack->Spec.MeterSum.Word = 0;
    OutputData->pdata.MeterRcmd_ack->Spec.PackageNumber = Indata->pdata.MeterRcmd_ask->PackageNumber; 
    OutputData->pdata.MeterRcmd_ack->Spec.MetNumber = 0;
    ReplyFlag = (unsigned char*)(OutputData->pdata.MeterRcmd_ack->Meter);
    *ReplyFlag =  0x20;   //   无此包  
  }
  else
  {
    //共有分包(HEX)
    itemp  = List.MeterNumber[0];
    itemp += List.MeterNumber[1];
    itemp += List.MeterNumber[2];
    itemp += List.MeterNumber[3];
    OutputData->pdata.MeterRcmd_ack->Spec.PackageSum = itemp /25;
    if(0 != (itemp % 25))
    {OutputData->pdata.MeterRcmd_ack->Spec.PackageSum ++;}
    //共有表数(HEX) 
    OutputData->pdata.MeterRcmd_ack->Spec.MeterSum.Word = 0;
    OutputData->pdata.MeterRcmd_ack->Spec.MeterSum.Byte.Byte_H = itemp >> 8; 
    OutputData->pdata.MeterRcmd_ack->Spec.MeterSum.Byte.Byte_L = itemp & 0x00FF;  
    
    if(Indata->pdata.MeterRcmd_ask->PackageNumber <= OutputData->pdata.MeterRcmd_ack->Spec.PackageSum)
    {
      switch (Indata->port)
      {
      case GPRS_CH1:
        {ReadAddr = &MeterRcmd_ON[0];break;}
      case GPRS_CH2:
        {ReadAddr = &MeterRcmd_ON[1];break;}
      case GPRS_CH3:
        {ReadAddr = &MeterRcmd_ON[2];break;}
      case GPRS_CH4:
        {ReadAddr = &MeterRcmd_ON[3];break;}
      case Rs232_Port:
        {ReadAddr = &MeterRcmd_ON[4];break;}
      default :{return CmdValid;}
      }
      
      //第几包数据(HEX)
      OutputData->pdata.MeterRcmd_ack->Spec.PackageNumber = Indata->pdata.MeterRcmd_ask->PackageNumber; 
      if(1 == Indata->pdata.MeterRcmd_ask->PackageNumber)
      {*ReadAddr = 0;}
      //该包表计地址个数
      OutputData->pdata.MeterRcmd_ack->Spec.MetNumber = 0;
      
      while(OutputData->pdata.MeterRcmd_ack->Spec.MetNumber < 25)
      {
        err = ReadMeterRecordFormRom(&Record, (*ReadAddr) ++);
        if(NO_ERR == err)
        {
          memcpy(&(OutputData->pdata.MeterRcmd_ack->Meter[OutputData->pdata.MeterRcmd_ack->Spec.MetNumber]),&(Record.Config), sizeof(Server188_Meter_Stuct));
          OutputData->pdata.MeterRcmd_ack->Spec.MetNumber ++; 
        }
        if(*ReadAddr >= (MbusChannelNumber * SingleChannel_MeterNumber))   //通道结束
        {
          OutputData->pdata.MeterRcmd_ack->Spec.PackageSum = OutputData->pdata.MeterRcmd_ack->Spec.PackageNumber;
          break;
        }
      }
      ReplyFlag = (unsigned char*)&(OutputData->pdata.MeterRcmd_ack->Meter[OutputData->pdata.MeterRcmd_ack->Spec.MetNumber]);
      *ReplyFlag =  0;   //应答标志(HEX):0x00:正常；
    }
    else
    {
      OutputData->pdata.MeterRcmd_ack->Spec.PackageNumber = Indata->pdata.MeterRcmd_ask->PackageNumber; 
      OutputData->pdata.MeterRcmd_ack->Spec.MetNumber = 0;
      ReplyFlag = (unsigned char*)(OutputData->pdata.MeterRcmd_ack->Meter);
      *ReplyFlag =  0x20;   //   无此包 
    }
  }
  
  OutputData->length = (sizeof(Server188_Meter_Stuct) * OutputData->pdata.MeterRcmd_ack->Spec.MetNumber) 
    + sizeof(Server188_MeterRcmd_ackParam_Spec_Stuct) + 1;
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //读取下载失败表计地址
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName MeterErrRcmd_Func(Server188_InputData_Stuct* Indata, 
                                   Server188_OututData_Stuct* OutputData)
{
  unsigned char *ReplyFlag;
  OutputData->pdata.MeterErrRcmd_ack->Spec.PackageSum = 0;
  OutputData->pdata.MeterErrRcmd_ack->Spec.MeterSum.Word = 0;
  OutputData->pdata.MeterErrRcmd_ack->Spec.PackageNumber = Indata->pdata.MeterErrRcmd_ask->PackageNumber;
  OutputData->pdata.MeterErrRcmd_ack->Spec.MetNumber = 0;
  ReplyFlag = (unsigned char*)OutputData->pdata.MeterErrRcmd_ack->Meter;
  *ReplyFlag = 0xCB; //:无此包;
  
    
  OutputData->length = sizeof(Server188_MeterErrRcmd_ackParam_Spec_Stuct) + 1;
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //下达控制命令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName OperateWcmd_Func(Server188_InputData_Stuct* Indata, 
                                      Server188_OututData_Stuct* OutputData)	
{
  SystemErrName err = NO_ERR;
  CommPortNumber_Type  TargetPort;
  unsigned short No;
  TimerData_Struct Timer;
  CJT188_MeterDeclarator  Meter;
  CJT8188_ValveAsk_Struct  ValveAsk; 
  CJT188_Frame_Struct  ack;
  Server188_MeterRecordData_Struct  Record;
  
  OutputData->pdata.OperateWcmd_ack->MeterType = Indata->pdata.OperateWcmd_ask->Operate.MeterType;
  memcpy(&(OutputData->pdata.OperateWcmd_ack->Addr),&(Indata->pdata.OperateWcmd_ask->Operate.Addr), sizeof(CJT188_AddresStruct));
  OutputData->pdata.OperateWcmd_ack->ReplyFlag = OK;
  
  switch (Indata->pdata.OperateWcmd_ask->Operate.Link.Bit.Channel)
  {
  case 1:  //M-BUS通道1
    {TargetPort = Mbus_Port1; break;}
  case 2:  //M-BUS通道2
    {TargetPort = Mbus_Port2; break;}
  case 3:  //M-BUS通道3
    {TargetPort = Mbus_Port3; break;}
  case 4:  //M-BUS通道4
    {TargetPort = Mbus_Port4; break;}
  default:
    {
      OutputData->pdata.OperateWcmd_ack->ReplyFlag = ParamErr;   //参数不合法；
    }
  }
  
  if((0x55 != Indata->pdata.OperateWcmd_ask->Operate.MeterState)
     &&(0x99 != Indata->pdata.OperateWcmd_ask->Operate.MeterState))  //阀门状态(HEX) 0X99：关阀；0X55：开阀
  {
    OutputData->pdata.OperateWcmd_ack->ReplyFlag = ParamErr;   //参数不合法；
  }
  
  if(OK == OutputData->pdata.OperateWcmd_ack->ReplyFlag)
  {
    No = ReadMeterRecordFormAddrAndLink(&Record, 
                                        &(Indata->pdata.OperateWcmd_ask->Operate.Addr),
                                        Indata->pdata.OperateWcmd_ask->Operate.Link.Bit.Channel,
                                        &err);  //读取表计记录
  }
  if(NO_ERR == err)
  {
    if((Indata->pdata.OperateWcmd_ask->Operate.MeterType != Record.Config.MeterType)
       ||(Indata->pdata.OperateWcmd_ask->Operate.Link.Byte != Record.Config.Link.Byte)
         ||(Indata->pdata.OperateWcmd_ask->Operate.Comm.Byte != Record.Config.Comm.Byte)  )
    {OutputData->pdata.OperateWcmd_ack->ReplyFlag = NoMeter; }  //未查询到表计
  }
  else
  {OutputData->pdata.OperateWcmd_ack->ReplyFlag = NoMeter;}   //未查询到表计
  
  if(OK == OutputData->pdata.OperateWcmd_ack->ReplyFlag)
  { 
    get_SystemTimer (&Timer);
    err =  write_MeterOperateToRom(&(Indata->pdata.OperateWcmd_ask->Operate),&Timer, No);//记录操作
    if(pdTRUE == xSemaphoreTake( Mbus_xMutex, _1s * 6 ))
    {
      config_Mbus_188(TargetPort, Record.Config.Comm);
      if(0x00 == Indata->pdata.OperateWcmd_ask->Operate.Link.Bit.Protocol) // /* 规约类型   0000:	阀控188 */
      {
        memcpy(&(Meter.Addr),&(Indata->pdata.OperateWcmd_ask->Operate.Addr), sizeof(CJT188_AddresStruct));
        Meter.Type = Indata->pdata.OperateWcmd_ask->Operate.MeterType; 
        if(0X55 == Indata->pdata.OperateWcmd_ask->Operate.MeterState)  //阀门状态(HEX) 0X99：关阀；0X55：开阀
        {ValveAsk.ValveCmd = CJT_188ValveOpenCmd;}
        else
        {ValveAsk.ValveCmd = CJT_188ValveCloseCmd;}
        //向表计发送操作指令
        err = CJT188_WriteDataToNode(CJT188_WriteValveState_DI, 
                                     &Meter, 
                                     TargetPort, 
                                     &ValveAsk, 
                                     sizeof (CJT8188_ValveAsk_Struct), 
                                     &ack);
        
        vTaskDelay(_1s * 10);
        //读取阀状态
        err = CJT188_ReadDataFormNode (CJT188_ReadData_DI, &Meter, TargetPort, &ack);
        //未查询到表计
        if(NO_ERR == err)
        {
          OutputData->pdata.OperateWcmd_ack->ReplyFlag = OK;
          switch ( ack.Data.OK.Data.ST.Bit.Valve_State )  ////阀门状态 00：开， 01 关， 11：异常
          {
          case 0x00:
            {
              OutputData->pdata.OperateWcmd_ack->MetState = 0x03; //开阀成功;
              break ;
            } 
          case 01:
            { 
              OutputData->pdata.OperateWcmd_ack->MetState = 0x00; //关阀成功;
              break ;
            }
          case 11:
            {
              if(CJT_188ValveOpenCmd == ValveAsk.ValveCmd)
              {OutputData->pdata.OperateWcmd_ack->MetState = 0x05;} //开阀失败}
              else if (CJT_188ValveCloseCmd == ValveAsk.ValveCmd)
              {OutputData->pdata.OperateWcmd_ack->MetState = 0x02;} //关阀失败;}
              break ;
            } 
          }
        }
        else
        { OutputData->pdata.OperateWcmd_ack->ReplyFlag = MeterNoAck; } //表计无响应 
      }
      else
      {OutputData->pdata.OperateWcmd_ack->ReplyFlag = ParamErr; }  //参数不合法
      CloseAllMbus ( );
      xSemaphoreGive(Mbus_xMutex);
    }
    else
    {OutputData->pdata.OperateWcmd_ack->ReplyFlag  = MbusBusy;}  //下行忙  
  }
  OutputData->length = sizeof(Server188_OperateWcmd_ackParam_Stuct);
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //读取控制命令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName OperateRcmd_Func(Server188_InputData_Stuct* Indata, 
                                      Server188_OututData_Stuct* OutputData)	
{
  SystemErrName err = NO_ERR;
  unsigned char *ReplyFlag;
  unsigned short* ReadAddr;
  Server188_MeterListSpecData_Struct  List;
  Server188_MeterRecordData_Struct Record; 
  
  if(NO_ERR != Read_MeterListSpecFormRom(&List))
  {
    OutputData->pdata.OperateRcmd_ack->Spec.PackageSum = Indata->pdata.OperateRcmd_ask->PackageNumber;
    OutputData->pdata.OperateRcmd_ack->Spec.PackageNumber = Indata->pdata.OperateRcmd_ask->PackageNumber; 
    OutputData->pdata.OperateRcmd_ack->Spec.ConNumber = 0;
    ReplyFlag = (unsigned char*)OutputData->pdata.OperateRcmd_ack->Record;
    *ReplyFlag =  0xCB;   //0xCB:无此包
    OutputData->length = sizeof(Server188_OperateRcmd_ackParam_Spec_Stuct)+ 1;
    return NO_ERR;
  }
  //共有分包(HEX)
  switch (Indata->port)
  {
  case GPRS_CH1:
    {ReadAddr = &OperateRcmd_0N[0];break;}
  case GPRS_CH2:
    {ReadAddr = &OperateRcmd_0N[1];break;}
  case GPRS_CH3:
    {ReadAddr = &OperateRcmd_0N[2];break;}
  case GPRS_CH4:
    {ReadAddr = &OperateRcmd_0N[3];break;}
  case Rs232_Port:
    {ReadAddr = &OperateRcmd_0N[4];break;}
  default :{return CmdValid;}
  }
  
  OutputData->pdata.OperateRcmd_ack->Spec.PackageSum = (List.CloseNumber  + List.OpenNumber ) /15;
  if(0 != ((List.CloseNumber  + List.OpenNumber ) % 15))
  {OutputData->pdata.OperateRcmd_ack->Spec.PackageSum ++;}
  
  OutputData->pdata.OperateRcmd_ack->Spec.PackageNumber = Indata->pdata.OperateRcmd_ask->PackageNumber;
  if(1 == OutputData->pdata.OperateRcmd_ack->Spec.PackageNumber);
  {* ReadAddr = 0;}
  OutputData->pdata.OperateRcmd_ack->Spec.ConNumber = 0;
  
  while (OutputData->pdata.OperateRcmd_ack->Spec.ConNumber < 14)
  {
    err = ReadMeterRecordFormRom(&Record, (*ReadAddr)++);
    if(NO_ERR == err)
    {
      if(0 != Record.CSR.Bit.Open_E)
      {
        OutputData->pdata.OperateRcmd_ack->Record[OutputData->pdata.OperateRcmd_ack->Spec.ConNumber].Comm.Byte = Record.Config.Comm.Byte;
        OutputData->pdata.OperateRcmd_ack->Record[OutputData->pdata.OperateRcmd_ack->Spec.ConNumber].Link.Byte = Record.Config.Link.Byte;
        OutputData->pdata.OperateRcmd_ack->Record[OutputData->pdata.OperateRcmd_ack->Spec.ConNumber].MeterType = Record.Config.MeterType;
        OutputData->pdata.OperateRcmd_ack->Record[OutputData->pdata.OperateRcmd_ack->Spec.ConNumber].MeterState = 0X55;  //阀门状态(HEX) 0X99：关阀；0X55：开阀
        //OutputData->pdata.OperateRcmd_ack->Record[OutputData->pdata.OperateRcmd_ack->Spec.ConNumber].OPID = Record.OpenRecord.OPID;
        memcpy(&(OutputData->pdata.OperateRcmd_ack->Record[OutputData->pdata.OperateRcmd_ack->Spec.ConNumber].Addr),
               &( Record.Config.Addr), 
               sizeof(Server188_MeterAddr_Stuct));
        //memcpy(&(OutputData->pdata.OperateRcmd_ack->Record[OutputData->pdata.OperateRcmd_ack->Spec.ConNumber].Time),
             //  &( Record.OpenRecord.Time), 
             //  sizeof(Server188_Time_Stuct));             
        OutputData->pdata.OperateRcmd_ack->Spec.ConNumber ++;
        
      }
      if(0 != Record.CSR.Bit.Close_E)
      {
        OutputData->pdata.OperateRcmd_ack->Record[OutputData->pdata.OperateRcmd_ack->Spec.ConNumber].Comm.Byte = Record.Config.Comm.Byte;
        OutputData->pdata.OperateRcmd_ack->Record[OutputData->pdata.OperateRcmd_ack->Spec.ConNumber].Link.Byte = Record.Config.Link.Byte;
        OutputData->pdata.OperateRcmd_ack->Record[OutputData->pdata.OperateRcmd_ack->Spec.ConNumber].MeterType = Record.Config.MeterType;
        OutputData->pdata.OperateRcmd_ack->Record[OutputData->pdata.OperateRcmd_ack->Spec.ConNumber].MeterState = 0X99;  //阀门状态(HEX) 0X99：关阀；0X55：开阀
       // OutputData->pdata.OperateRcmd_ack->Record[OutputData->pdata.OperateRcmd_ack->Spec.ConNumber].OPID = Record.CloseRecord.OPID;
       // memcpy(&(OutputData->pdata.OperateRcmd_ack->Record[OutputData->pdata.OperateRcmd_ack->Spec.ConNumber].Addr),
       //        &( Record.Config.Addr), 
       //        sizeof(Server188_MeterAddr_Stuct));
       // memcpy(&(OutputData->pdata.OperateRcmd_ack->Record[OutputData->pdata.OperateRcmd_ack->Spec.ConNumber].Time),
       //        &( Record.CloseRecord.Time), 
        //       sizeof(Server188_Time_Stuct));             
        OutputData->pdata.OperateRcmd_ack->Spec.ConNumber ++;
      }
    }
    if(*ReadAddr >= TotalNumber_MeterConfig)   //通道结束
    {
      OutputData->pdata.MeterRcmd_ack->Spec.PackageSum = OutputData->pdata.MeterRcmd_ack->Spec.PackageNumber;
      break;
    }
  }
  ReplyFlag = (unsigned char*)&(OutputData->pdata.OperateRcmd_ack->Record[OutputData->pdata.OperateRcmd_ack->Spec.ConNumber]);
  *ReplyFlag =  0x00;   //0xCB:无此包
  OutputData->length = sizeof(Server188_OperateDeclarator_Stuct)* OutputData->pdata.OperateRcmd_ack->Spec.ConNumber;
  OutputData->length += sizeof(Server188_OperateRcmd_ackParam_Spec_Stuct)+ 1;
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //启动立即抄表
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName ReadingWcmd_Func(Server188_InputData_Stuct* Indata, 
                                      Server188_OututData_Stuct* OutputData)	
{
  if(FullReading == ReadingMeterState)     // 正在抄表; 
  {OutputData->pdata.ReadingWcmd_ack ->ReplyFlag =1; }  // 下行忙已经在抄表了,不需再次启动
  //else if(MbusPatroling == ReadingMeterState)     // 正在巡检表地址;
  //{OutputData->pdata.ReadingWcmd_ack ->ReplyFlag = 2; } // 下行正在巡检表地址,不能启动抄表
  else
  {
    ReadingSteup_Func( );
    OutputData->pdata.ReadingWcmd_ack->ReplyFlag =0;   // 正常启动抄表
  }
  
  OutputData->length = sizeof(Server188_ReadingWcmd_ackParam_Stuct);
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //读取某一采集器的所有表计当前数据
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName AllCurrentDataRcmd_Func(Server188_InputData_Stuct* Indata, 
                                             Server188_OututData_Stuct* OutputData)	
{
  static unsigned short   MeterRcmd_ON [6];
  SystemErrName err = NO_ERR;
  unsigned itemp; 
  unsigned char *ReplyFlag;
  unsigned short* ReadAddr;
  TimerData_Struct timer;
  Server188_MeterListSpecData_Struct  List;
  Server188_MeterRecordData_Struct Record; 
  Server188_MeterValueRecordData_Struct Value;
  
  get_SystemTimer(&timer);
  if(timer.Year > 2000)
  {timer.Year -= 2000;}
  else
  {timer.Year = 0;}
  Int8ToBCD (timer.Year, &(OutputData->pdata.AllCurrentDataRcmd_ack->Spec.Time.Year), 2);
  Int8ToBCD (timer.Month, &(OutputData->pdata.AllCurrentDataRcmd_ack->Spec.Time.Month), 2);
  Int8ToBCD (timer.Day, &OutputData->pdata.AllCurrentDataRcmd_ack->Spec.Time.Day, 2);
  Int8ToBCD (timer.Hour, &OutputData->pdata.AllCurrentDataRcmd_ack->Spec.Time.Hour, 2);
  Int8ToBCD (timer.Minute, &OutputData->pdata.AllCurrentDataRcmd_ack->Spec.Time.Minute, 2);
  Int8ToBCD (timer.Second, &OutputData->pdata.AllCurrentDataRcmd_ack->Spec.Time.Second, 2);
 
  if(NO_ERR != Read_MeterListSpecFormRom(&List))
  {
    OutputData->pdata.AllCurrentDataRcmd_ack->Spec.FileSum = Indata->pdata.AllCurrentDataRcmd_ask->PackageNumber;
    OutputData->pdata.AllCurrentDataRcmd_ack->Spec.Filecount = Indata->pdata.AllCurrentDataRcmd_ask->PackageNumber; 
    OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum = 0; 
    
    
    ReplyFlag = (unsigned char*)OutputData->pdata.AllCurrentDataRcmd_ack->Value;
    *ReplyFlag =  0xCB;   //0xCB:无此包
    OutputData->length = sizeof(Server188_AllCurrentDataRcmd_ackParam_Spec_Stuct)+1;
    return NO_ERR; 
  }
  
  //共有分包(HEX)
  switch (Indata->port)
  {
  case GPRS_CH1:
    {ReadAddr = &MeterRcmd_ON[0];break;}
  case GPRS_CH2:
    {ReadAddr = &MeterRcmd_ON[1];break;}
  case GPRS_CH3:
    {ReadAddr = &MeterRcmd_ON[2];break;}
  case GPRS_CH4:
    {ReadAddr = &MeterRcmd_ON[3];break;}
  case Rs232_Port:
    {ReadAddr = &MeterRcmd_ON[4];break;}
  default :{return CmdValid;}
  }
  itemp =  List.MeterNumber[0] + List.MeterNumber[1] + List.MeterNumber[2] + List.MeterNumber[3];
  OutputData->pdata.AllCurrentDataRcmd_ack->Spec.FileSum = itemp / 17;
  if(0 != (itemp % 17))
  {OutputData->pdata.AllCurrentDataRcmd_ack->Spec.FileSum ++;}
  
  if(OutputData->pdata.AllCurrentDataRcmd_ack->Spec.FileSum < Indata->pdata.AllCurrentDataRcmd_ask->PackageNumber)
  {  
    OutputData->pdata.AllCurrentDataRcmd_ack->Spec.Filecount = Indata->pdata.AllCurrentDataRcmd_ask->PackageNumber; 
    OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum = 0; 
    ReplyFlag = (unsigned char*)OutputData->pdata.AllCurrentDataRcmd_ack->Value;
    *ReplyFlag =  0xCB;   //0xCB:无此包
    OutputData->length = sizeof(Server188_AllCurrentDataRcmd_ackParam_Spec_Stuct)+1;
    return NO_ERR;
  }
  
  OutputData->pdata.AllCurrentDataRcmd_ack->Spec.Filecount = Indata->pdata.AllCurrentDataRcmd_ask->PackageNumber;
  if(1 == OutputData->pdata.AllCurrentDataRcmd_ack->Spec.Filecount)
  {* ReadAddr = 0;}
  OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum = 0;
  
  while (OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum < 17)
  {
    err = ReadMeterRecordFormRom(&Record, *ReadAddr);
    if(NO_ERR == err)
    {
      memcpy(&(OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].Meter),
             &( Record.Config), 
             sizeof(Server188_Meter_Stuct));
      //if(0 == Record.Config.Addr.FactoryID)
      //{ 
        
        err = ReadMeterValueFormRom(&Value, *ReadAddr);
        if(NO_ERR == err) 
        {
          OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].Value[0] = Value.Value.Flow_PV[0];
          OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].Value[1] = Value.Value.Flow_PV[1];
          OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].Value[2] = Value.Value.Flow_PV[2];
          OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].Value[3] = Value.Value.Flow_PV[3];
          OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].ST.Word = Value.Value.ST.Word;
          OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].Temp = Value.Value.Temp;
          if(0x00 != Value.CSR.Bit.Lose )
          {OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].ST.Bit.Lose = 1;}
          else
          {OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].ST.Bit.Lose = 0;}
          OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].ST.Bit.RomErr = 0;
        }
        else
        {
          OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].Value[0] = Value.Value.Flow_PV[0];
          OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].Value[1] = Value.Value.Flow_PV[1];
          OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].Value[2] = Value.Value.Flow_PV[2];
          OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].Value[3] = Value.Value.Flow_PV[3];
          OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].ST.Word = Value.Value.ST.Word;
          OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].Temp = Value.Value.Temp;
          OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].ST.Bit.RomErr = 1;
        }
        OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum ++; 
      //}
    }
    (*ReadAddr)++;
    if(*ReadAddr >= TotalNumber_MeterConfig)  
    {
      OutputData->pdata.AllCurrentDataRcmd_ack->Spec.FileSum =  OutputData->pdata.AllCurrentDataRcmd_ack->Spec.Filecount;
      //OutputData->pdata.MeterRcmd_ack->Spec.PackageSum = OutputData->pdata.MeterRcmd_ack->Spec.PackageNumber;
      ReportAllCurrentDataFinish = 1;
      break;
    }
    else
    {
      ReportAllCurrentDataFinish = 0;
    }
  }
  ReplyFlag = (unsigned char*)&(OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum]);
  *ReplyFlag =  0x00;   //0xCB:无此包
  OutputData->length = sizeof(Server188_MeterValue_Stuct) * OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum;  
  OutputData->length += sizeof(Server188_AllCurrentDataRcmd_ackParam_Spec_Stuct)+1;
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //读取某一采集器的所有表计日冻结数据
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName AllFreezingDataRcmd_Func(Server188_InputData_Stuct* Indata, 
                                   Server188_OututData_Stuct* OutputData)
{
  static unsigned short   MeterRcmd_ON [6];
  SystemErrName err = NO_ERR;
  unsigned short itemp;
  unsigned char *ReplyFlag;
  unsigned short* ReadAddr;
  TimerData_Struct timer;
  Server188_MeterListSpecData_Struct  List;
  Server188_MeterRecordData_Struct    Record; 
  
  get_SystemTimer(&timer);
  OutputData->pdata.AllFreezingDataRcmd_ack->Spec.Time.Year = timer.Year - 2000;
  OutputData->pdata.AllFreezingDataRcmd_ack->Spec.Time.Month = timer.Month;
  OutputData->pdata.AllFreezingDataRcmd_ack->Spec.Time.Day = timer.Day;
  OutputData->pdata.AllFreezingDataRcmd_ack->Spec.Time.Hour= timer.Hour;
  OutputData->pdata.AllFreezingDataRcmd_ack->Spec.Time.Minute = timer.Minute;
  OutputData->pdata.AllFreezingDataRcmd_ack->Spec.Time.Second = timer.Second;
  
  if(NO_ERR != Read_MeterListSpecFormRom(&List))
  {
    OutputData->pdata.AllFreezingDataRcmd_ack->Spec.FileSum   = Indata->pdata.AllFreezingDataRcmd_ask->PackageNumber;
    OutputData->pdata.AllFreezingDataRcmd_ack->Spec.Filecount = Indata->pdata.AllFreezingDataRcmd_ask->PackageNumber; 
    OutputData->pdata.AllFreezingDataRcmd_ack->Spec.RecordNum = 0; 
    ReplyFlag = (unsigned char*)&(OutputData->pdata.AllFreezingDataRcmd_ack->Value);
    *ReplyFlag =  0xCB;   //0xCB:无此包
    OutputData->length = sizeof(Server188_AllFreezingDataRcmd_ackParam_Spec_Stuct)+1;
    return NO_ERR; 
  }
  
  //共有分包(HEX)
  switch (Indata->port)
  {
  case GPRS_CH1:
    {ReadAddr = &MeterRcmd_ON[0];break;}
  case GPRS_CH2:
    {ReadAddr = &MeterRcmd_ON[1];break;}
  case GPRS_CH3:
    {ReadAddr = &MeterRcmd_ON[2];break;}
  case GPRS_CH4:
    {ReadAddr = &MeterRcmd_ON[3];break;}
  case Rs232_Port:
    {ReadAddr = &MeterRcmd_ON[4];break;}
  default :{return CmdValid;}
  }
  itemp = List.MeterNumber[0] + List.MeterNumber [1] + List.MeterNumber [2] + List.MeterNumber [3];    
  OutputData->pdata.AllFreezingDataRcmd_ack->Spec.FileSum = itemp / 17;
  if(0 != (itemp% 17))
  {OutputData->pdata.AllCurrentDataRcmd_ack->Spec.FileSum ++;}
  
  OutputData->pdata.AllCurrentDataRcmd_ack->Spec.Filecount = Indata->pdata.AllFreezingDataRcmd_ask->PackageNumber;
  if(1 == OutputData->pdata.AllFreezingDataRcmd_ack->Spec.Filecount);
  {* ReadAddr = 0;}
  OutputData->pdata.AllFreezingDataRcmd_ack->Spec.RecordNum = 0;
  
  while (OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum < 17)
  {
    err = ReadMeterRecordFormRom(&Record, (*ReadAddr)++);
    if(NO_ERR == err)
    {
     // if((Record.Data.Time.Year  ==  Indata->pdata.AllFreezingDataRcmd_ask->Year)
       //  ||(Record.Data.Time.Month  ==  Indata->pdata.AllFreezingDataRcmd_ask->Month)
       //    ||(Record.Data.Time.Day  ==  Indata->pdata.AllFreezingDataRcmd_ask->Day))
      //{
      // memcpy(&(OutputData->pdata.AllFreezingDataRcmd_ack->Value.CJT188[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].Meter),
       //       &(Record.Config), 
       ///      sizeof(Server188_Meter_Stuct));
      // OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].Value[0] = Record.Data.Flow_Accounts[0];
      // OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].Value[1] = Record.Data.Flow_Accounts[1];
      // OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].Value[2] = Record.Data.Flow_Accounts[2];
      // OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].Value[3] = Record.Data.Flow_Accounts[3];
      // OutputData->pdata.AllCurrentDataRcmd_ack->Value[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum].ST.Word = Record.Data.ST.Word;
     // }
   }
   if(*ReadAddr >= TotalNumber_MeterConfig)  
   {
     OutputData->pdata.AllFreezingDataRcmd_ack->Spec.FileSum = OutputData->pdata.AllFreezingDataRcmd_ack->Spec.Filecount;
     break;
   }
  }
  ReplyFlag = (unsigned char*)&(OutputData->pdata.AllFreezingDataRcmd_ack->Value.CJT188[OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum]);
  *ReplyFlag =  0x00;   //0xCB:无此包
  OutputData->length = sizeof(Server188_MeterValue_Stuct) * OutputData->pdata.AllCurrentDataRcmd_ack->Spec.RecordNum;  
  OutputData->length = sizeof(Server188_AllFreezingDataRcmd_ackParam_Spec_Stuct)+1;
 return NO_ERR;
}
/*********************************************************************************
Function:      //
Description:   //读取单表的当前数据
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
static SystemErrName SingCurrentDataRcmd_Func(Server188_InputData_Stuct* Indata, 
                                              Server188_OututData_Stuct* OutputData)	
{
  SystemErrName  err = NO_ERR;
  Server188_MeterRecordData_Struct Record;
  CommPortNumber_Type TargetPort;
  CJT188_MeterDeclarator Meter;
  CJT188_Frame_Struct ack;
  
  switch (Indata->pdata.SingCurrentDataRcmd_ask->Meter.Link.Bit.Channel) 
  {
  case 1:
    {TargetPort = Mbus_Port1; break;}
  case 2:
    {TargetPort = Mbus_Port2; break;}
  case 3:
    {TargetPort = Mbus_Port3; break;}
  case 4:
    {TargetPort = Mbus_Port4; break;}
  default :
    {return IllegalParam;}
  }
  
  OutputData->pdata.SingCurrentDataRcmd_ack->ST.Word = 0;
  OutputData->pdata.SingCurrentDataRcmd_ack->Value[0]= 0;
  OutputData->pdata.SingCurrentDataRcmd_ack->Value[1]= 0;
  OutputData->pdata.SingCurrentDataRcmd_ack->Value[2]= 0;
  OutputData->pdata.SingCurrentDataRcmd_ack->Value[3]= 0;
  
  memcpy(&(OutputData->pdata.SingCurrentDataRcmd_ack->Meter),
         &(Indata->pdata.SingCurrentDataRcmd_ask->Meter), 
         sizeof(Server188_Meter_Stuct));
  OutputData->pdata.SingCurrentDataRcmd_ack->ReplyFlag = 0x00;  //应答标志(HEX)：0x00:正常，0xCF:未找到表
  
  ReadMeterRecordFormAddrAndLink(&Record, 
                                 &(Indata->pdata.SingCurrentDataRcmd_ask->Meter.Addr),
                                 Indata->pdata.SingCurrentDataRcmd_ask->Meter.Link.Bit.Channel,
                                 &err); 
  if(NO_ERR == err)
  {
    Meter.Addr.FactoryID= Record.Config.Addr.FactoryID;
    Meter.Addr.MeterID[0]= Record.Config.Addr.MeterID[0];
    Meter.Addr.MeterID[1]= Record.Config.Addr.MeterID[1];
    Meter.Addr.MeterID[2]= Record.Config.Addr.MeterID[2];
    Meter.Addr.MeterID[3]= Record.Config.Addr.MeterID[3];
    Meter.Addr.MeterID[4]= Record.Config.Addr.MeterID[4];
    Meter.Type= Record.Config.MeterType;  
    
    if(pdTRUE == xSemaphoreTake( Mbus_xMutex, _1s * 6 ))
    {
      config_Mbus_188(TargetPort, Record.Config.Comm);
      if(0x00 == Record.Config.Link.Bit.Protocol)  ///* 规约类型   0000:	阀控188 */
      {
         err = CJT188_ReadDataFormNode (CJT188_ReadData_DI, &Meter, TargetPort, &ack);
        //err = CJT188_ReadDataFormNode (CJT188_ReadData_DI, &Meter, TargetPort, &ack);
        if(NO_ERR == err)
        {
          //读表正确应答 
          OutputData->pdata.SingCurrentDataRcmd_ack->ReplyFlag = 0x00;  //应答标志(HEX)：0x00:正常，0xCF:未找到表
          //if((Meter.Addr.MeterID[3] == 0x17)&&(Meter.Addr.MeterID[2] == 0x06)){
           // OutputData->pdata.SingCurrentDataRcmd_ack->ST.Word = ack.Data.OK.Data.MeterData.ST.Word;
           // OutputData->pdata.SingCurrentDataRcmd_ack->Value[0]= ack.Data.OK.Data.MeterData.Flow_PV[3];
           // OutputData->pdata.SingCurrentDataRcmd_ack->Value[1]= ack.Data.OK.Data.MeterData.Flow_PV[2];
            //OutputData->pdata.SingCurrentDataRcmd_ack->Value[2]= ack.Data.OK.Data.MeterData.Flow_PV[1];
            //OutputData->pdata.SingCurrentDataRcmd_ack->Value[3]= ack.Data.OK.Data.MeterData.Flow_PV[0]; 
          //}
          //else{
            if(Record.Config.Comm.Bit.BaudRate == 0x05) //波特率38400代表波特率2400，且数据取反
            {
              OutputData->pdata.SingCurrentDataRcmd_ack->Value[0]= ack.Data.OK.Data.MeterData.Flow_PV[3];
              OutputData->pdata.SingCurrentDataRcmd_ack->Value[1]= ack.Data.OK.Data.MeterData.Flow_PV[2];
              OutputData->pdata.SingCurrentDataRcmd_ack->Value[2]= ack.Data.OK.Data.MeterData.Flow_PV[1];
              OutputData->pdata.SingCurrentDataRcmd_ack->Value[3]= ack.Data.OK.Data.MeterData.Flow_PV[0];
            }
            else
            {
              OutputData->pdata.SingCurrentDataRcmd_ack->ST.Word = ack.Data.OK.Data.MeterData.ST.Word;
              OutputData->pdata.SingCurrentDataRcmd_ack->Value[0]= ack.Data.OK.Data.MeterData.Flow_PV[0];
              OutputData->pdata.SingCurrentDataRcmd_ack->Value[1]= ack.Data.OK.Data.MeterData.Flow_PV[1];
              OutputData->pdata.SingCurrentDataRcmd_ack->Value[2]= ack.Data.OK.Data.MeterData.Flow_PV[2];
              OutputData->pdata.SingCurrentDataRcmd_ack->Value[3]= ack.Data.OK.Data.MeterData.Flow_PV[3];
            }
            
          //}
          OutputData->pdata.SingCurrentDataRcmd_ack->ST.Bit.Lose = 0; 
        }
        else
        {
          //未 读到表
          OutputData->pdata.SingCurrentDataRcmd_ack->ST.Word = 0;
          //OutputData->pdata.SingCurrentDataRcmd_ack->ST.= 0;
          OutputData->pdata.SingCurrentDataRcmd_ack->Value[0]= 0;
          OutputData->pdata.SingCurrentDataRcmd_ack->Value[1]= 0;
          OutputData->pdata.SingCurrentDataRcmd_ack->Value[2]= 0;
          OutputData->pdata.SingCurrentDataRcmd_ack->Value[3]= 0;
          OutputData->pdata.SingCurrentDataRcmd_ack->ST.Bit.Lose = 1; 
          OutputData->pdata.SingCurrentDataRcmd_ack->ReplyFlag = MeterNoAck;  //应答标志(HEX)：0x00:正常，0xCF:未找到表
        }  
      }
      CloseAllMbus ( );
      xSemaphoreGive(Mbus_xMutex); 
    }
    else
    {
      //下行通道忙 
      OutputData->pdata.SingCurrentDataRcmd_ack->ReplyFlag = MbusBusy;  //应答标志(HEX)：0x00:正常，0xCF:未找到表
    }
  }
  else
  {
    //未找到表 
    OutputData->pdata.SingCurrentDataRcmd_ack->ReplyFlag = NoMeter;  //应答标志(HEX)：0x00:正常，0xCF:未找到表 
  }
  OutputData->length = sizeof(Server188_SingCurrentDataRcmd_ackParam_Stuct);
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //读取单表的日冻结数据
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName SingFreezingDataRcmd_Func(Server188_InputData_Stuct* Indata, 
                                   Server188_OututData_Stuct* OutputData)
{
  SystemErrName  err = NO_ERR;
  //unsigned short No ;
  Server188_MeterRecordData_Struct Record;
  
  if((1 == Indata->pdata.SingFreezingDataRcmd_ask->Meter.Link.Bit.Channel)
     ||(2 == Indata->pdata.SingFreezingDataRcmd_ask->Meter.Link.Bit.Channel)
       ||(3 == Indata->pdata.SingFreezingDataRcmd_ask->Meter.Link.Bit.Channel)
         ||(4 == Indata->pdata.SingFreezingDataRcmd_ask->Meter.Link.Bit.Channel))
  {
     ReadMeterRecordFormAddrAndLink(&Record, 
                                         &(Indata->pdata.SingFreezingDataRcmd_ask->Meter.Addr),
                                         Indata->pdata.SingFreezingDataRcmd_ask->Meter.Link.Bit.Channel,
                                         &err); 
    if(NO_ERR == err)
    {
     // OutputData->pdata.SingFreezingDataRcmd_ack->ST.Word = Record.Data.ST.Word;
     // OutputData->pdata.SingFreezingDataRcmd_ack->Value[0]= Record.Data.Flow_Accounts[0];
     // OutputData->pdata.SingFreezingDataRcmd_ack->Value[1]= Record.Data.Flow_Accounts[1];
      //OutputData->pdata.SingFreezingDataRcmd_ack->Value[2]= Record.Data.Flow_Accounts[2];
      //OutputData->pdata.SingFreezingDataRcmd_ack->Value[3]= Record.Data.Flow_Accounts[3];
      memcpy(&(OutputData->pdata.SingFreezingDataRcmd_ack->Meter),
             &(Record.Config), 
             sizeof(Server188_Meter_Stuct));
      //memcpy(&(OutputData->pdata.SingFreezingDataRcmd_ack->Time),
     ////        &(Record.Data.Time), 
      //       sizeof(Server188_Time_Stuct));
      //if((Record.Data.Time.Day != Indata->pdata.SingFreezingDataRcmd_ask->Day)
      ////   &&(Record.Data.Time.Month != Indata->pdata.SingFreezingDataRcmd_ask->Month)
      //     &&(Record.Data.Time.Year != Indata->pdata.SingFreezingDataRcmd_ask->Year))
      //{OutputData->pdata.SingFreezingDataRcmd_ack->ReplyFlag = 0xCD;} //应答标志(HEX)：0x00:正常; 0xCD:未找到该日期   0xCF:未找到表
     // else
      //{OutputData->pdata.SingFreezingDataRcmd_ack->ReplyFlag = 0x00;} //应答标志(HEX)：0x00:正常; 0xCD:未找到该日期   0xCF:未找到表 
    }
  }
  else
  {err = IllegalParam ;}
  if(NO_ERR != err)
  {
   //OutputData->pdata.SingFreezingDataRcmd_ack->ST.Word = Record.Data.ST.Word;
   //OutputData->pdata.SingFreezingDataRcmd_ack->Value[0]= Record.Data.Flow_Accounts[0];
   //OutputData->pdata.SingFreezingDataRcmd_ack->Value[1]= Record.Data.Flow_Accounts[1];
   //OutputData->pdata.SingFreezingDataRcmd_ack->Value[2]= Record.Data.Flow_Accounts[2];
   //OutputData->pdata.SingFreezingDataRcmd_ack->Value[3]= Record.Data.Flow_Accounts[3];
   memcpy(&(OutputData->pdata.SingFreezingDataRcmd_ack->Meter),
           &(Indata->pdata.SingFreezingDataRcmd_ask->Meter), 
           sizeof(Server188_Meter_Stuct));
   memset(&(OutputData->pdata.SingFreezingDataRcmd_ack->Time), 0, sizeof(Server188_Time_Stuct)); 
   OutputData->pdata.SingCurrentDataRcmd_ack->ReplyFlag = 0xCF;  //应答标志(HEX)：0x00:正常; 0xCD:未找到该日期   0xCF:未找到表
  }
  
 OutputData->length = sizeof(Server188_SingFreezingDataRcmd_ackParam_Stuct);
 return NO_ERR; 
}
/*********************************************************************************
 Function:      //
 Description:   //替换表计
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName AlterMeterWcmd_Func(Server188_InputData_Stuct* Indata, 
                                   Server188_OututData_Stuct* OutputData)
{
  SystemErrName  err = NO_ERR;
  unsigned short NO;
  Server188_MeterRecordData_Struct   Record;
  
  if((1 == Indata->pdata.AlterMeterWcmd_ask->OldMeter.Link.Bit.Channel)
     ||(2 == Indata->pdata.AlterMeterWcmd_ask->OldMeter.Link.Bit.Channel)
       ||(3 == Indata->pdata.AlterMeterWcmd_ask->OldMeter.Link.Bit.Channel)
         ||(4 == Indata->pdata.AlterMeterWcmd_ask->OldMeter.Link.Bit.Channel))
  {
    NO = ReadMeterRecordFormAddrAndLink(&Record,
                                        &(Indata->pdata.AlterMeterWcmd_ask->OldMeter.Addr),
                                        Indata->pdata.AlterMeterWcmd_ask->OldMeter.Link.Bit.Channel,
                                        &err);
  }
  else
  {
    NO = ReadMeterRecordFormAddr(&Record,
                                 &(Indata->pdata.AlterMeterWcmd_ask->OldMeter.Addr),
                                 &err);
  }

 if(NO_ERR == err)
 {
  //memset(void *, int, size_t); 
  memcpy(&(Record.Config),&(Indata->pdata.AlterMeterWcmd_ask->NewMeter), sizeof(Server188_Meter_Stuct)); 
  err = write_MeterRecordToRom(&Record,NO);
  if(NO_ERR == err)
  {OutputData->pdata.AlterMeterWcmd_ack->ReplyFlag = 0x00;} //替换成功
  else
  {OutputData->pdata.AlterMeterWcmd_ack->ReplyFlag = 0xEF;} //存储器写入错误
 }
 else
 {OutputData->pdata.AlterMeterWcmd_ack->ReplyFlag = 0x01;} //0x01: 未查询到表计

 OutputData->length = sizeof(Server188_AlterMeterWcmd_ackParam_Stuct);
 return NO_ERR; 
}
/*********************************************************************************
 Function:      //
 Description:   //删除表计
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName DeleteMeterWcmd_Func(Server188_InputData_Stuct* Indata, 
                                   Server188_OututData_Stuct* OutputData)
{
  SystemErrName  err = NO_ERR;
  unsigned char itemp;
  unsigned short No = 0;
  unsigned char *ReplyFlag;
  Server188_MeterRecordData_Struct Record;
  
  OutputData->pdata.DeleteMeterWcmd_ack->TotalNumber = 0;
  if((Indata->pdata.DeleteMeterWcmd_ask->TotalNumber <= 25)
     &&(Indata->pdata.DeleteMeterWcmd_ask->TotalNumber > 0))
  { 
    itemp = 0;
    while(itemp < Indata->pdata.DeleteMeterWcmd_ask->TotalNumber )  //预备数据
    {
      memcpy(&(OutputData->pdata.DeleteMeterWcmd_ack->Result[itemp].Addr),
             &(Indata->pdata.DeleteMeterWcmd_ask->Meter[itemp].Addr),
             sizeof(Server188_MeterAddr_Stuct));
      OutputData->pdata.DeleteMeterWcmd_ack->Result[itemp++].Flg = 0x01;  //删除失败
    } 
    do{
      if(OutputData->pdata.DeleteMeterWcmd_ack->TotalNumber == Indata->pdata.DeleteMeterWcmd_ask->TotalNumber)
      {break;}
      err =  ReadMeterRecordFormRom(&Record, No);
      if(NO_ERR == err)
      {
        itemp = 0;
        while(itemp < Indata->pdata.DeleteMeterWcmd_ask->TotalNumber)
        {
          if(0 == memcmp(&(Record.Config.Addr), &(Indata->pdata.DeleteMeterWcmd_ask->Meter[itemp].Addr), sizeof(Server188_MeterAddr_Stuct)))
          {
            OutputData->pdata.DeleteMeterWcmd_ack->TotalNumber ++;
            err = delete_MeterRecordFormRom( No );
            if(NO_ERR == err)
            {OutputData->pdata.DeleteMeterWcmd_ack->Result[itemp].Flg = 0x00;} //00删除成功 
            break;
          }
          itemp ++;
        }
      }
    }while(++No < TotalNumber_MeterConfig);
    OutputData->pdata.DeleteMeterWcmd_ack->TotalNumber = Indata->pdata.DeleteMeterWcmd_ask->TotalNumber;
    ReplyFlag = (unsigned char*)&(OutputData->pdata.DeleteMeterWcmd_ack->Result[OutputData->pdata.DeleteMeterWcmd_ack->TotalNumber]);
    *ReplyFlag = 0x00;//删除成功
    itemp = 0;
    while (itemp < OutputData->pdata.DeleteMeterWcmd_ack->TotalNumber)
    {
      if(0x00 != OutputData->pdata.DeleteMeterWcmd_ack->Result[itemp++].Flg)
      {*ReplyFlag = 0x01; }//删除错误
    } 
  }
  else
  {
    OutputData->pdata.DeleteMeterWcmd_ack->TotalNumber = 0; 
    ReplyFlag = (unsigned char*)OutputData->pdata.DeleteMeterWcmd_ack->Result;
    *ReplyFlag = 0x22;//参数不合法 = 0x22, 
  }
  
  OutputData->length = 2 + OutputData->pdata.DeleteMeterWcmd_ack->TotalNumber * sizeof(Server188_DeleteResult_Stuct) ;
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
static SystemErrName CleanMeterWcmd_Func(Server188_InputData_Stuct* Indata, Server188_OututData_Stuct* OutputData)
{
  SystemErrName  err;
  unsigned short No = 0;
  Server188_MeterListSpecData_Struct ListSpec;
  OutputData->pdata.CleanMeterWcmd_ack->ReplyFlag  = 0x00;
  while (No < SingleChannel_MeterNumber * MbusChannelNumber)
  {
    err = delete_MeterRecordFormRom(No++);
    //if(NO_ERR != err)
    //{
      //OutputData->pdata.CleanMeterWcmd_ack->ReplyFlag  = 0x01;
      //break;
    //}
  }
  //if( 0x00 ==OutputData->pdata.CleanMeterWcmd_ack->ReplyFlag)
  {
   ListSpec.MeterNumber[0] = 0; 
   ListSpec.MeterNumber[1] = 0; 
   ListSpec.MeterNumber[2] = 0; 
   ListSpec.MeterNumber[3] = 0; 
   ListSpec.OpenNumber = 0;
   ListSpec.CloseNumber = 0;
   err = write_MeterListSpecToRom(&ListSpec); 
   if(NO_ERR != err)
   {OutputData->pdata.CleanMeterWcmd_ack->ReplyFlag  = 0x01;}
  }

  OutputData->length = sizeof(Server188_CleanMeterWcmd_ackParam_Stuct);
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
SystemErrName Server188_Send(CommPortNumber_Type Port,  Server188_Frame_Struct* frame)
{
  SystemErrName err = NO_ERR;
  unsigned char itemp ;
  unsigned short DataLength;
  
  Frame_Pack(frame);
  
  DataLength = frame->Datas.Head.DataLength.Byte.Byte_H;
  DataLength <<= 8;
  DataLength += frame->Datas.Head.DataLength.Byte.Byte_L;
  DataLength += sizeof(Server188_PreambleType) + sizeof(Server188_FrameHead_Struct) 
                + sizeof(Server188_FrameStern_Struct);
  itemp = 0;
  do{ 
    err =  Comm_send(frame,DataLength, Port,P_NULL, P_NULL);
    if(NO_ERR == err)
    {break;} 
  }while(itemp++ < 3);
  
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
static SystemErrName config_Mbus_188(CommPortNumber_Type Port, Server188_CommDeclarator Comm)
{
  SystemErrName err = NO_ERR;
  //unsigned char DataNumber;
  unsigned long BaudRate;
  SerialPort_Parity  Parity;
  SerialPort_StopBits StopBits;
  
  
  switch(Comm.Bit.BaudRate)      
  {
  case 0x00:{BaudRate = 1200;break;}
  case 0x01:{BaudRate = 2400;break;}
  case 0x02:{BaudRate = 4800;break;}
  case 0x03:{BaudRate = 9600;break;}
  case 0x04:{BaudRate = 19200;break;}
  case 0x05:{BaudRate = 2400;break;}
  case 0x06:{BaudRate = 57600;break;}
  case 0x07:{BaudRate = 115200;break;}
  default: {return IllegalParam;}
  }
  
 /* 
  switch(Comm.Bit.DataNumber)     //数据位  00: 6位数据位  01: 7位数据位 10: 8位数据位   
  {
  case 0x00:{DataNumber = 6;break;}
  case 0x01:{DataNumber = 7;break;}
  case 0x02:{DataNumber = 8;break;}
  default: {return IllegalParam;}
  }*/
  
  switch(Comm.Bit.Parity)     //校验位  00:无校验     01:偶校验       10:奇校验   
  {
  case 0x00:{Parity = Parity_No;break;}
  case 0x01:{Parity = Parity_Even;break;}
  case 0x02:{Parity = Parity_Odd;break;}
  default: {return IllegalParam;}
  }
  
  switch(Comm.Bit.StopBits)     //停止位  0 :1位停止位   1 :2位停止位  
  {
  case 0x00:{StopBits = StopBits_1;break;}  
  case 0x01:{StopBits = StopBits_2;break;}
  default: {return IllegalParam;}
  }
 
  err = config_Mbus(Port, BaudRate, Parity, StopBits,ByDMA);
  
  return err;
}
/*********************************************************************************
 Function:      //
 Description:   //启动抄表序列
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName ReadingSteup_Func( void )
{
  ReadingMeterState = FullReading;
  return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   // 协议入口 
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
SystemErrName Server188_SendErr (CommPortNumber_Type port, Server188_Err_Param_Stuct* Err )
{
  static unsigned char SER = 1;
  SystemErrName err = NO_ERR;
  unsigned short itemp;

  Server188_OutPutBuff_Struct*  OutputData;
  
  OutputData =  getServer188SendBuff(port);
  if(P_NULL == OutputData)
  {return Nothing;}
  //填充 功能码 
  OutputData->Frame.Datas.Head.Code = Server188_ErrUpData;
  OutputData->Frame.Datas.Param.Err_Param.ErrType = Err->ErrType;
  OutputData->Frame.Datas.Param.Err_Param.ErrParam = Err->ErrParam;

  //填充 序列号 SER
  itemp = sizeof(Server188_Err_Param_Stuct);
  OutputData->Frame.Datas.Param.Data[itemp++] = SER ++; 
  //填充 数据域长度 
  OutputData->Frame.Datas.Head.DataLength.Byte.Byte_H = itemp >> 8;
  OutputData->Frame.Datas.Head.DataLength.Byte.Byte_L = itemp & 0x00FF;
  
  err = Server188_Send(port, &(OutputData->Frame));
  freeServer188SendBuff(OutputData);
  return err;
}
/*********************************************************************************
 Function:      //
 Description:   //抄表序列
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void  ReadTask( void )
{
  SystemErrName err = NO_ERR;
  unsigned short No = 0;
  CommPortNumber_Type TargetPort;
  CJT188_MeterDeclarator Meter ;
  CJT188_Frame_Struct ack;
  Server188_MeterRecordData_Struct Record;
  Server188_ValueRecordData_CSR csr; 
  Server188_MeterData_Struct Value;
  TimerData_Struct timer;
  
  if(FullReading == ReadingMeterState)
  {
    No = 0;
    do{
      RLED_OFF ( ); 
      err =  ReadMeterRecordFormRom(&Record, No);
      if(NO_ERR == err)
      {
        Meter.Addr.FactoryID= Record.Config.Addr.FactoryID;
        Meter.Addr.MeterID[0]= Record.Config.Addr.MeterID[0];
        Meter.Addr.MeterID[1]= Record.Config.Addr.MeterID[1];
        Meter.Addr.MeterID[2]= Record.Config.Addr.MeterID[2];
        Meter.Addr.MeterID[3]= Record.Config.Addr.MeterID[3];
        Meter.Addr.MeterID[4]= Record.Config.Addr.MeterID[4];
        Meter.Type= Record.Config.MeterType;
        
        switch (Record.Config.Link.Bit.Channel) 
        {
        case 1:
          {TargetPort = Mbus_Port1; break;}
        case 2:
          {TargetPort = Mbus_Port2; break;}
        case 3:
          {TargetPort = Mbus_Port3; break;}
        case 4:
          {TargetPort = Mbus_Port4; break;}
        default :
          {continue ;}
        }
        
        if(pdTRUE == xSemaphoreTake( Mbus_xMutex, _10s * 3 ))
        {
          err = config_Mbus_188(TargetPort, Record.Config.Comm);
          err = CJT188_ReadDataFormNode (CJT188_ReadData_DI, &Meter, TargetPort, &ack);
          xSemaphoreGive(Mbus_xMutex);
        }
        get_SystemTimer(&timer);
        if(timer.Year > 2000)
        {timer.Year -= 2000;}
        else
        {timer.Year = 0;}
        Int8ToBCD (timer.Year, &(Value.Time.Year), 2);
        Int8ToBCD (timer.Month, &(Value.Time.Month), 2);
        Int8ToBCD (timer.Day, &Value.Time.Day, 2);
        Int8ToBCD (timer.Hour, &Value.Time.Hour, 2);
        Int8ToBCD (timer.Minute, &Value.Time.Minute, 2);
        Int8ToBCD (timer.Second, &Value.Time.Second, 2);
        csr.Byte = 0x00;
        if(NO_ERR == err)
        {
          //读表正确应答   //记录超表数据
          RLED_ON( ); 
          csr.Bit.PV_E = 1;
          
          if(Record.Config.Comm.Bit.BaudRate == 0x05) //波特率38400代表波特率2400，且数据取反
          {
            Value.Flow_PV[0] = ack.Data.OK.Data.MeterData.Flow_PV[3];
            Value.Flow_PV[1] = ack.Data.OK.Data.MeterData.Flow_PV[2];
            Value.Flow_PV[2] = ack.Data.OK.Data.MeterData.Flow_PV[1];
            Value.Flow_PV[3] = ack.Data.OK.Data.MeterData.Flow_PV[0];
          }
          else
          {
            Value.Flow_PV[0] = ack.Data.OK.Data.MeterData.Flow_PV[0];
            Value.Flow_PV[1] = ack.Data.OK.Data.MeterData.Flow_PV[1];
            Value.Flow_PV[2] = ack.Data.OK.Data.MeterData.Flow_PV[2];
            Value.Flow_PV[3] = ack.Data.OK.Data.MeterData.Flow_PV[3];
          }
          
          Value.ST.Word = ack.Data.OK.Data.MeterData.ST.Word;
          
          if(timer.Day == _188_ConcenConfigData.timer.MonthFrozenH)
          {
            csr.Bit.Acc_E = 1; 

            
            if(Record.Config.Comm.Bit.BaudRate == 0x05) //波特率38400代表波特率2400，且数据取反
            {
              Value.Flow_Accounts[0] = ack.Data.OK.Data.MeterData.Flow_PV[3];
              Value.Flow_Accounts[1] = ack.Data.OK.Data.MeterData.Flow_PV[2];
              Value.Flow_Accounts[2] = ack.Data.OK.Data.MeterData.Flow_PV[1];
              Value.Flow_Accounts[3] = ack.Data.OK.Data.MeterData.Flow_PV[0];
            }
            else
            {
              Value.Flow_Accounts[0] = ack.Data.OK.Data.MeterData.Flow_PV[0];
              Value.Flow_Accounts[1] = ack.Data.OK.Data.MeterData.Flow_PV[1];
              Value.Flow_Accounts[2] = ack.Data.OK.Data.MeterData.Flow_PV[2];
              Value.Flow_Accounts[3] = ack.Data.OK.Data.MeterData.Flow_PV[3];
            }
          }
          
          err = CJT188_ReadDataFormNode (CJT188_ReadTemp_DI, &Meter, TargetPort, &ack);
          if(NO_ERR == err)
          {
            //读表温度正确应答 
            Value.Temp = ack.Data.OK.Data.MeterData.Flow_PV[0];
            Value.ST.Bit.Temp_Flag = 0;
          }
          else
          {
            //未读到表温度
            Value.Temp = 0;
            Value.ST.Bit.Temp_Flag = 1;
          }  
        }
        else
        {
          CloseAllMbus ( );
          vTaskDelay(_100ms * 8);
          csr.Bit.Lose = 0x01;
        }
        
        write_MeterValueToRom(csr, &Value, No);
        vTaskDelay(_100ms * 2);
      }
    }while(++No < TotalNumber_MeterConfig);
    CloseAllMbus ( );

    ReadingMeterState = ReadingFinish;
  }
}
/*********************************************************************************
 Function:      //
 Description:   //抄表序列
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void  PatrolTask( void )
{
  static unsigned char flg = 0;
  SystemErrName err = NO_ERR;
  unsigned short No = 0;
  CommPortNumber_Type TargetPort;
  CJT188_MeterDeclarator Meter ;
  CJT188_Frame_Struct ack;
  Server188_MeterRecordData_Struct Record;
  CJT188_ST_Stuct  st;
  CJT8188_ValveAsk_Struct ValveAsk; 
  TimerData_Struct timer;
  
  get_SystemTimer(&timer);
  
  if(1 == timer.Day)
  {
    if(0 == flg)
    {
      flg = 1;
      ReadingMeterState = FullPatroling;
    }
  }
  else
  {flg = 0;}
  
  
  if(FullPatroling == ReadingMeterState)
  {
    No = 0;
    do{
      err =  ReadMeterRecordFormRom(&Record, No);
      RLED_OFF ( ); 
      if(NO_ERR == err)
      {
        Meter.Addr.FactoryID= Record.Config.Addr.FactoryID;
        Meter.Addr.MeterID[0]= Record.Config.Addr.MeterID[0];
        Meter.Addr.MeterID[1]= Record.Config.Addr.MeterID[1];
        Meter.Addr.MeterID[2]= Record.Config.Addr.MeterID[2];
        Meter.Addr.MeterID[3]= Record.Config.Addr.MeterID[3];
        Meter.Addr.MeterID[4]= Record.Config.Addr.MeterID[4];
        Meter.Type= Record.Config.MeterType;
        
        switch (Record.Config.Link.Bit.Channel) 
        {
        case 1:
          {TargetPort = Mbus_Port1; break;}
        case 2:
          {TargetPort = Mbus_Port2; break;}
        case 3:
          {TargetPort = Mbus_Port3; break;}
        case 4:
          {TargetPort = Mbus_Port4; break;}
        default :
          {continue ;}
        }
        
        if(pdTRUE == xSemaphoreTake( Mbus_xMutex, _10s * 3 ))
        {
          err = config_Mbus_188(TargetPort, Record.Config.Comm);
          err = CJT188_ReadDataFormNode (CJT188_ReadData_DI, &Meter, TargetPort, &ack);
          if(NO_ERR == err)
          {
            st.Word = ack.Data.OK.Data.MeterData.ST.Word;
            ValveAsk.ValveCmd = CJT_188ValveHalfOpenCmd;
            err = CJT188_WriteDataToNode(CJT188_WriteValveState_DI,  &Meter, TargetPort, 
                                     &ValveAsk,sizeof (CJT8188_ValveAsk_Struct), 
                                     &ack);
            vTaskDelay(_1s * 10);
           
            if(0x00 == st.Bit.Valve_State)
            {ValveAsk.ValveCmd = CJT_188ValveOpenCmd;}
            else if(0x01 == st.Bit.Valve_State)
            {ValveAsk.ValveCmd = CJT_188ValveCloseCmd;}
            err = CJT188_WriteDataToNode(CJT188_WriteValveState_DI,  &Meter, TargetPort, 
                                         &ValveAsk, sizeof (CJT8188_ValveAsk_Struct), 
                                         &ack);
             vTaskDelay(_1s * 10);
            
//            err = CJT188_ReadDataFormNode (CJT188_ReadData_DI, &Meter, TargetPort, &ack);
//            if((ack.Data.OK.Data.ST.Bit.Valve_State != st.Bit.Valve_State)
//               &&(0x03 != st.Bit.Valve_State))
//            {
//              if(0x01 == st.Bit.Valve_State)
//              {ValveAsk.ValveCmd = CJT_188ValveCloseCmd;}
//              else 
//              {ValveAsk.ValveCmd = CJT_188ValveOpenCmd;}
//              err = CJT188_WriteDataToNode(CJT188_WriteValveState_DI,  &Meter, TargetPort, 
//                                           &ValveAsk, sizeof (CJT8188_ValveAsk_Struct), 
//                                           &ack);
//              vTaskDelay(_1s * 15);
//            }
            
          }
          xSemaphoreGive(Mbus_xMutex);
          RLED_ON ( ); 
        }
      }
    }while(++No < TotalNumber_MeterConfig);    
    CloseAllMbus ( );
    ReadingMeterState = DownIdle;
  }
}
/*********************************************************************************
 Function:      //
 Description:   //引导主动上报采集器的所有表计当前数据
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
void GuideReportAllCurrentData(void)
{
  unsigned char i = 0,sum = 0;
  unsigned char buff[32] = {0xFE,0xFE,0xFE,0xFE,0x68,0xEE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x15,0x02,0x00,0x01,0x00,0x00,0x16};
  
  memcpy(&buff[6],_188_ConcenConfigData.ID,7);
  while(i < 14)
  {
   sum += buff[4+i];
   i++;
  }
  buff[18] = sum;
  
  Server188_InPut (buff, 20, GPRS_CH1);
}
/******************************************END********************************************************/

