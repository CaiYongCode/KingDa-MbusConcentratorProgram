/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\MeterList.h"
#include ".\CJT-188-2004.h"
#include "..\inc\Server188.h"
#include "..\inc\Includes.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
unsigned char   MeterListSpec_errflg = 0;
unsigned char   MeterList_errflg = 0;
unsigned short  Err_MeterNo = 0; 
/*********************************************************************************
公共变量定义区
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
私有变量定义区
*********************************************************************************/ 

/*********************************************************************************
测试变量定义区
*********************************************************************************/

/*********************************************************************************
内部函数定义区
*********************************************************************************/

/*********************************************************************************
功能代码定义区
*********************************************************************************/
SystemErrName writeToFRAM (void* Buffer, unsigned short NumByteToWrite, unsigned short WriteAddr);
SystemErrName readFormFRAM (void* Buffer, unsigned short NumByteToRead,unsigned short ReadAddr);

/*********************************************************************************
Function:      //
Description:   //
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
SystemErrName Read_MeterListSpecFormRom(Server188_MeterListSpecData_Struct* List)
{
  SystemErrName  err = NO_ERR; 
  unsigned short crc; 
  
  
  err = readFormFRAM(List, sizeof(Server188_MeterListSpecData_Struct),MeterListDecAtFRAM);
  if(NO_ERR == err ) 
  {err = readFormFRAM(&crc, sizeof(unsigned short), sizeof (Server188_MeterListSpecData_Struct)+ MeterListDecAtFRAM);}
  if(NO_ERR == err )
  {
    if(crc != crc16((unsigned char *)List,sizeof (Server188_MeterListSpecData_Struct)))
    { err = DataValid;}
  }
  if(NO_ERR == err)
  {MeterListSpec_errflg = 0;}
  else
  {MeterListSpec_errflg = 1;}
  
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
SystemErrName write_MeterListSpecToRom(Server188_MeterListSpecData_Struct* Spec)
{
  SystemErrName  err = NO_ERR;
  unsigned char itemp;
  unsigned short CRC16;
  Server188_MeterListSpec_Struct  ListSpecBuuff;
  

  
  CRC16 = crc16((unsigned char *)Spec,sizeof (Server188_MeterListSpecData_Struct));
  itemp = 0;
  do{
    err = writeToFRAM(Spec, sizeof(Server188_MeterListSpecData_Struct),MeterListDecAtFRAM);
    if(NO_ERR == err)
    {err = writeToFRAM(&CRC16, sizeof(unsigned short),MeterListDecAtFRAM + sizeof(Server188_MeterListSpecData_Struct));}
    if(NO_ERR == err)
    {
      err = readFormFRAM(&ListSpecBuuff, sizeof(Server188_MeterListSpec_Struct),MeterListDecAtFRAM);
      if(0 == memcmp((const void *)&ListSpecBuuff,(const void *)Spec,sizeof(Server188_MeterListSpecData_Struct)))
      {
        if(CRC16 == ListSpecBuuff.CRC16)
        {break;}
      }
    }
  }while(itemp++ < 3);
  if(itemp >=  3)
  {err = DataRomWrite_err;}

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
SystemErrName ReadMeterRecordFormRom(Server188_MeterRecordData_Struct* Record, unsigned short No)
{
  SystemErrName  err =NO_ERR;
  unsigned short crc; 
  unsigned long ReadAddr = MeterListAtFRAM;
  ReadAddr += sizeof(Server188_MeterRecord_Struct) * No;

  err = readFormFRAM(Record, sizeof(Server188_MeterRecordData_Struct),ReadAddr);
  if(NO_ERR == err ) 
  {err = readFormFRAM(&crc, sizeof(unsigned short), sizeof (Server188_MeterRecordData_Struct)+ReadAddr);}
  
  if(NO_ERR == err )
  {
    if(0 != Record->CSR.Bit.Config_E )
    {
      if(crc !=  crc16((unsigned char *)Record,sizeof (Server188_MeterRecordData_Struct)))
      {
        MeterList_errflg = 1;
        Err_MeterNo = No;
        err = DataValid;
      }
    }
    else
    {err = NoData;} 
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
Others:        //
*********************************************************************************/
unsigned short ReadMeterRecordFormAddr(Server188_MeterRecordData_Struct* Record,  
                                       Server188_MeterAddr_Stuct* Addr,
                                       SystemErrName* err)
{
  unsigned short No = 0;
  while (No < TotalNumber_MeterConfig)
  {
    *err = ReadMeterRecordFormRom(Record, No);
    if(NO_ERR == err)
    {
      if(0 == memcmp((const void *)Addr,(const void *)&(Record->Config.Addr),sizeof(Server188_MeterAddr_Stuct)))
      {break;}
    }
    {No ++;}
  }
  if(No >= TotalNumber_MeterConfig)
  { *err = NoData;} 
  return No;
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
unsigned short ReadMeterRecordFormAddrAndLink(Server188_MeterRecordData_Struct* Record, 
                                              Server188_MeterAddr_Stuct* Addr,
                                              unsigned char CH,
                                              SystemErrName* err)
{
  unsigned short No = 0;
  unsigned short ReadAddr;
  
  if((CH < 1)||(CH > 4))
  {return IllegalParam; }
  
  ReadAddr = (CH -1) * SingleChannel_MeterNumber;
  while (No < SingleChannel_MeterNumber)
  {
    *err = ReadMeterRecordFormRom(Record, ReadAddr);
    if(NO_ERR == *err)
    {
      if(0 == memcmp((const void *)Addr,(const void *)&(Record->Config.Addr),sizeof(Server188_MeterAddr_Stuct)))
      {break;}
    }
    ReadAddr ++;
    No ++;
  }
  if(No >= SingleChannel_MeterNumber)
  { *err = NoData;}  
  return ReadAddr;
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
SystemErrName write_MeterRecordToRom(Server188_MeterRecordData_Struct* Record, unsigned short No)
{
  SystemErrName  err =NO_ERR;
  unsigned char itemp =0;
  unsigned short  CRC16;         //数据完整性验证
  unsigned long WriteAddr = MeterListAtFRAM;
  Server188_MeterRecord_Struct Recordbuff;

  WriteAddr += sizeof(Server188_MeterRecord_Struct) * No;
  CRC16 = crc16((unsigned char *)Record,sizeof (Server188_MeterRecordData_Struct));
  do{
    err = writeToFRAM(Record, sizeof(Server188_MeterRecordData_Struct),WriteAddr);
    if(NO_ERR ==err)
    {err = writeToFRAM(&CRC16, sizeof(unsigned short),WriteAddr + sizeof(Server188_MeterRecordData_Struct));}
    if(NO_ERR ==err)
    {
      err = readFormFRAM(&Recordbuff, sizeof(Server188_MeterRecord_Struct),WriteAddr);
      if(0 == memcmp((const void *)&Recordbuff,(const void *)Record,sizeof(Server188_MeterRecordData_Struct)))
      {
        if(Recordbuff.CRC16 == CRC16)
        { err =NO_ERR;}
        break;
      }
    }
  }while (itemp++ < 3);
  if(itemp >= 3)
  {err = DataRomWrite_err;}
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
SystemErrName Add_MeterRecordToRom(Server188_MeterRecordData_Struct* Record, unsigned short* StartNo)
{
  SystemErrName  err = NO_ERR;
  unsigned short itemp;
  unsigned short endNo;
  static Server188_MeterRecordData_Struct  Recordbuff;
  Server188_MeterListSpecData_Struct        SpecData;
  
  err = Read_MeterListSpecFormRom(&SpecData);
  if(NO_ERR == err )
  {
    switch (Record->Config.Link.Bit.Channel)
    {
    case 0x01:
      {
        itemp = 0; 
        SpecData.MeterNumber[0]++;
        break; }
    case 0x02:
      {
        itemp = 1 * SingleChannel_MeterNumber;
        SpecData.MeterNumber[1]++;
        break; 
      }
    case 0x03:
      {
        itemp = 2 * SingleChannel_MeterNumber;
        SpecData.MeterNumber[2]++;
        break; 
      }
    case 0x04:
      {
        itemp = 3 * SingleChannel_MeterNumber; 
        SpecData.MeterNumber[3]++;
        break;
      }
    default: {return IllegalParam;}
    }
    
    endNo = itemp + SingleChannel_MeterNumber;
    if((*StartNo >= itemp)&&(*StartNo < endNo))
    {itemp =  *StartNo;}
    
    do{
      err =  ReadMeterRecordFormRom(&Recordbuff, itemp);
      if(NO_ERR != err)
      {
        Record->CSR.Bit.Config_E = 0x01;
        err =  write_MeterRecordToRom(Record, itemp);
        if(NO_ERR == err )
        {
          *StartNo = itemp;
          err = write_MeterListSpecToRom(&SpecData); 
          if(NO_ERR == err )
          {break;}
        }
      }
      else
      {
        if(0 == memcmp((const void *)&(Recordbuff.Config.Addr),(const void *)&(Record->Config.Addr),sizeof(Server188_Meter_Stuct)))
        {
          err = MeterRepeat;
          break;
        }
      }
    }while(++itemp < endNo);
    
    if(itemp >= endNo)
    {err = Nothing;}
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
Others:        //
*********************************************************************************/
SystemErrName delete_MeterRecordFormRom(unsigned short No)
{
  SystemErrName  err = NO_ERR;
  unsigned char itemp = 0;
  Server188_RecordData_CSR  CSR;
  unsigned long ReadAddr = MeterListAtFRAM;
  Server188_MeterListSpecData_Struct SpecData;
  Server188_MeterRecordData_Struct   Record;
  
  err = Read_MeterListSpecFormRom(&SpecData);
  if(NO_ERR == err )
  {ReadMeterRecordFormRom(&Record, No);}
  if(NO_ERR == err )
  {
    
    ReadAddr += sizeof(Server188_MeterRecord_Struct) * No;
    switch (Record.Config.Link.Bit.Channel)
    {
    case 0x01:
      { 
        if(SpecData.MeterNumber[0] >0 )
        {SpecData.MeterNumber[0]--;}
        break; 
      }
    case 0x02:
      {
        if(SpecData.MeterNumber[1] >0 )
        {SpecData.MeterNumber[1]--;}
        break; 
      }
    case 0x03:
      {
        if(SpecData.MeterNumber[2] >0 )
        {SpecData.MeterNumber[2]--;}
        break; 
      }
    case 0x04:
      {
        if(SpecData.MeterNumber[3] >0 )
        {SpecData.MeterNumber[3]--;}
        break;
      }
    default: {break;}
    }
    do{
      CSR.Byte = 0;
      err = writeToFRAM(&CSR,sizeof(Server188_RecordData_CSR),ReadAddr);//将缓冲器的数据写入EEPROM
      if(NO_ERR == err)
      {
        err = readFormFRAM(&CSR, sizeof(Server188_RecordData_CSR),ReadAddr);
        if(NO_ERR == err)
        {
          if(0 == CSR.Byte)
          {
            err = write_MeterListSpecToRom(&SpecData);
            if(NO_ERR == err)
            {break;}
          }
        }
      }
    }while(itemp++ < 3);
    if(itemp >=  3)
    {err = Nothing;}
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
Others:        //
*********************************************************************************/
SystemErrName ReadMeterValueFormRom(Server188_MeterValueRecordData_Struct* Value, unsigned short No)
{
  SystemErrName  err =NO_ERR;
  unsigned short crc; 
  unsigned long ReadAddr = MeterValueAtFRAM;
  
  ReadAddr += sizeof(Server188_MeterValueRecord_Struct) * No;

  err = readFormFRAM(Value, sizeof(Server188_MeterValueRecordData_Struct),ReadAddr);
  if(NO_ERR == err ) 
  {err = readFormFRAM(&crc, sizeof(unsigned short), sizeof (Server188_MeterValueRecordData_Struct)+ReadAddr);}
  
  if(NO_ERR == err )
  {
    if(0 != Value->CSR.Bit.Config_E )
    {
      if(crc !=  crc16((unsigned char *)Value,sizeof (Server188_MeterValueRecordData_Struct)))
      {
        MeterList_errflg = 1;
        Err_MeterNo = No;
        err = DataValid;
      }
    }
    else
    {err = NoData;} 
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
Others:        //
*********************************************************************************/
SystemErrName write_MeterValueToRom(Server188_ValueRecordData_CSR csr, Server188_MeterData_Struct* Value, unsigned short No)
{
  SystemErrName  err =NO_ERR;
  unsigned char itemp;
  unsigned long WriteAddr = MeterValueAtFRAM;
  Server188_MeterValueRecord_Struct Buff1;
  Server188_MeterValueRecord_Struct Buff2;
  
  WriteAddr += sizeof(Server188_MeterValueRecord_Struct) * No;
  
  err =  ReadMeterValueFormRom(&(Buff1.data),  No);
  
  if(NO_ERR != err)
  {
    itemp = 0 ;
    while(itemp < sizeof (Server188_MeterValueRecordData_Struct))
    {((unsigned char *)&(Buff1.data))[itemp ++] = 0x00;}
    Buff1.data.CSR.Bit.Config_E = 0x01;
  }
  
  if(0 != csr.Bit.PV_E)
  {
    Buff1.data.Value.Flow_PV[0] = Value->Flow_PV[0];
    Buff1.data.Value.Flow_PV[1] = Value->Flow_PV[1];
    Buff1.data.Value.Flow_PV[2] = Value->Flow_PV[2];
    Buff1.data.Value.Flow_PV[3] = Value->Flow_PV[3];
    Buff1.data.CSR.Bit.PV_E = 0x01;
  }
  
  if(0 != csr.Bit.Acc_E)
  {
    Buff1.data.Value.Flow_Accounts[0] = Value->Flow_Accounts[0];
    Buff1.data.Value.Flow_Accounts[1] = Value->Flow_Accounts[1];
    Buff1.data.Value.Flow_Accounts[2] = Value->Flow_Accounts[2];
    Buff1.data.Value.Flow_Accounts[3] = Value->Flow_Accounts[3];
    Buff1.data.CSR.Bit.Acc_E = 0x01;
  }
  Buff1.data.CSR.Bit.Lose = csr.Bit.Lose;
  Buff1.data.Value.ST.Word = Value->ST.Word;
  Buff1.data.Value.Time.Second = Value->Time.Second;
  Buff1.data.Value.Time.Minute = Value->Time.Minute;
  Buff1.data.Value.Time.Hour = Value->Time.Hour;
  Buff1.data.Value.Time.Day = Value->Time.Day;
  Buff1.data.Value.Time.Month = Value->Time.Month;
  Buff1.data.Value.Time.Year = Value->Time.Year;
  Buff1.data.Value.Temp = Value->Temp;
  
  
  Buff1.CRC16 = crc16((unsigned char *)&Buff1.data,sizeof (Server188_MeterValueRecordData_Struct));
  
  itemp = 0;
  do{
    err = writeToFRAM(&Buff1, sizeof(Server188_MeterValueRecord_Struct),WriteAddr);
    if(NO_ERR ==err)
    {
      err = readFormFRAM(&Buff2, sizeof(Server188_MeterValueRecord_Struct),WriteAddr);
      if(0 == memcmp((const void *)&Buff1,(const void *)&Buff2,sizeof(Server188_MeterValueRecord_Struct)))
      {
        break;
      }
    }
  }while (itemp++ < 3);
  
  if(itemp >= 3)
  {err = DataRomWrite_err;}
  
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
SystemErrName write_MeterOperateToRom(Server188_OperateDeclarator_Stuct* Operate,
                                      TimerData_Struct* Timer, 
                                      unsigned short No)
{
  SystemErrName  err =NO_ERR;
  unsigned char itemp;
  unsigned long WriteAddr = MeterValueAtFRAM;
  Server188_MeterValueRecord_Struct Buff1;
  Server188_MeterValueRecord_Struct Buff2;
  
  WriteAddr += sizeof(Server188_MeterValueRecord_Struct) * No;
  
  err =  ReadMeterValueFormRom(&(Buff1.data),  No);
  
  if(NO_ERR != err)
  {
    itemp = 0 ;
    while(itemp < sizeof (Server188_MeterValueRecordData_Struct))
    {((unsigned char *)&(Buff1.data))[itemp ++] = 0x00;}
  }
  
  if(0x99 == Operate->MeterState)  //阀门状态(HEX) 0X99：关阀；0X55：开阀
  {
    Buff1.data.CloseRecord.OPID = Operate->OPID;
    
    Buff1.data.CloseRecord.Time.Day = Timer->Day;
    Buff1.data.CloseRecord.Time.Hour = Timer->Hour;
    Buff1.data.CloseRecord.Time.Minute = Timer->Minute;
    Buff1.data.CloseRecord.Time.Month = Timer->Month;
    Buff1.data.CloseRecord.Time.Second = Timer->Second;
    Buff1.data.CloseRecord.Time.Year = Timer->Year;  
  }
  else if(0x55 == Operate->MeterState)
  {
    Buff1.data.OpenRecord.OPID = Operate->OPID;
    
    Buff1.data.OpenRecord.Time.Day = Timer->Day;
    Buff1.data.OpenRecord.Time.Hour = Timer->Hour;
    Buff1.data.OpenRecord.Time.Minute = Timer->Minute;
    Buff1.data.OpenRecord.Time.Month = Timer->Month;
    Buff1.data.OpenRecord.Time.Second = Timer->Second;
    Buff1.data.OpenRecord.Time.Year = Timer->Year;  
  }
  
  Buff1.CRC16 = crc16((unsigned char *)&Buff1.data,sizeof (Server188_MeterValueRecordData_Struct));
  
  itemp = 0;
  do{
    err = writeToFRAM(&Buff1, sizeof(Server188_MeterValueRecord_Struct),WriteAddr);
    if(NO_ERR ==err)
    {
      err = readFormFRAM(&Buff2, sizeof(Server188_MeterValueRecord_Struct),WriteAddr);
      if(0 == memcmp((const void *)&Buff1,(const void *)&Buff2,sizeof(Server188_MeterValueRecord_Struct)))
      {
        break;
      }
    }
  }while (itemp++ < 3);
  
  if(itemp >= 3)
  {err = DataRomWrite_err;}
  
  return err;
}