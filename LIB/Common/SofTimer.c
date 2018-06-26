/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "..\inc\SofTimer.h"
//#include "..\inc\SystemDefine.h"
//#include "..\..\LIB\CMSIS\Device\STM32F10x\Include\stm32f10x.h"
//#include "..\..\APP\inc\MCPX.h"
//#include "..\..\APP\inc\GPRS.h"
//#include "..\..\APP\inc\ReadEquipData.h"
//#include "..\..\APP\inc\Appcon_API.h"
/*********************************************************************************
����������
*********************************************************************************/
/*********************************************************************************
��������������
*********************************************************************************/
/*********************************************************************************
�ⲿ����������
*********************************************************************************/
unsigned char _1mS_cnt =0;

extern SofTimer_Struct*  RTC_Timer;   //RTC �ػ�ʱ�� ��ֹRTCͣת
/*********************************************************************************
˽�б���������
*********************************************************************************/ 
static  SofTimer_Struct tmr[ SofTimer_Number ];
/*********************************************************************************
���Ա���������
*********************************************************************************/
void WriteCH376Cmd( unsigned char mCmd ) ;
/*********************************************************************************
�ڲ�����������
*********************************************************************************/
/*********************************************************************************
���ܴ��붨����
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
void  SofTimer_DeInit(void)
{
  unsigned char i;
  i = 0;
  while (i < SofTimer_Number)
  {
    tmr[i].CR.bit.EN = SofTimer_IDLE;
    i ++;
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
SofTimer_Struct* getSofTimer(void)
{
 unsigned char itemp = 0;
 while (itemp < SofTimer_Number)
 {
   if(SofTimer_IDLE == tmr[itemp].CR.bit.EN)
   {
     tmr[itemp].CR.bit.EN = SofTimer_USE;
     return &(tmr[itemp]);
   }
   itemp ++;
 }
 return P_NULL;
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
static SystemErrName SofTimerVerify (SofTimer_Struct *p_tmr) 
{
 unsigned char itemp = 0;
 while (itemp < SofTimer_Number)
 {
   if(p_tmr == &(tmr[itemp]))
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
 Return:	//
 Others:        //
*********************************************************************************/
SofTimer_Struct* SofTimerCreate (unsigned short Period,     //����
                                 SofTimer_TB tb,              //ʱ��
                                 unsigned char opt,          //ģʽ
                                 SofTimer_CALLBACK p_callback, //�ص�
                                 void *p_callback_arg)        //�ص�����
{
  SofTimer_Struct*  p_tmr;
  
  p_tmr = getSofTimer( );
  if(P_NULL != p_tmr )
  {
   p_tmr->CR.byte = 0x00; 
   p_tmr->CR.bit.EN = SofTimer_USE;
   p_tmr->CR.bit.Opt = opt;
   p_tmr->CR.bit.TB = tb;
   p_tmr->ARR = Period;
   p_tmr->CNTR = p_tmr->ARR;
   p_tmr->CallbackPtr = p_callback;
   p_tmr->CallbackPtrArg = p_callback_arg; 
  }
  else
  {IWDG -> KR = 0xBBBB;}  //��λ
 return p_tmr;
}
/*********************************************************************************
 Function:      //
 Description:   //�ı䶨ʱ����
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
SystemErrName SofTimer_ChangePeriod (SofTimer_Struct* p_tmr,       //���ݵĶ�ʱ��
                                     unsigned short NewPeriod,   //�µ�����
                                     unsigned char opt)          //�Ƿ�λ��ʱ�� 1:���¼�ʱ  0:������ʱ
{
  SystemErrName err;
  err = SofTimerVerify (p_tmr);
  if(NO_ERR == err)
  {
    p_tmr->ARR = NewPeriod;
    if(opt)
   {p_tmr->CNTR = p_tmr->ARR;}
  }
  return err;  
}
/*********************************************************************************
 Function:      //
 Description:   //ǿ�ƶ�ʱ��������һ�ζ�ʱ����¼�
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
SystemErrName SofTimer_Compelling(SofTimer_Struct* p_tmr)
{
  SystemErrName err;
  err = SofTimerVerify (p_tmr);
  if(NO_ERR == err)
  {
    if((SofTimer_USE == p_tmr->CR.bit.EN ) && (SofTimer_RUN == p_tmr->CR.bit.State))
    {
      if(P_NULL != p_tmr->CallbackPtr)
      { p_tmr->CallbackPtr(p_tmr,p_tmr->CallbackPtrArg);}
      if(SofTimer_ONE_SHOT == p_tmr->CR.bit.Opt) //��ʱ��Ϊ���δ���ʽ����
      {p_tmr->CR.bit.EN = SofTimer_IDLE;}
      else
      {p_tmr->CNTR = p_tmr->ARR;}
    } 
  }
  return err;
}
/*********************************************************************************
 Function:      //
 Description:   //������ʱ��
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
SystemErrName SofTimerStart(SofTimer_Struct* p_tmr) 
{
  SystemErrName err;
  err = SofTimerVerify (p_tmr);
  if(NO_ERR == err)
  {p_tmr->CR.bit.State = SofTimer_RUN;}
  return err;
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
SystemErrName SofTimerStop(SofTimer_Struct* p_tmr) 
{
  SystemErrName err;
  err = SofTimerVerify (p_tmr);
  if(NO_ERR == err)
  {p_tmr->CR.bit.State = SofTimer_STOP;}
  return err;
}
/*********************************************************************************
 Function:      //
 Description:   //ɾ����ʱ��
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
SystemErrName SofTimerDel(SofTimer_Struct** p_tmr) 
{
  SystemErrName err;
  err = SofTimerVerify (*p_tmr);
  if(NO_ERR == err)
  {(*p_tmr)->CR.bit.EN = SofTimer_IDLE;}
  *p_tmr = P_NULL;
  return err;
}
/*********************************************************************************
 Function:      //
 Description:   //��ʱ����ʱ��λ
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
SystemErrName SofTimerRset(SofTimer_Struct*  p_tmr)          
{
  SystemErrName err;
  err = SofTimerVerify (p_tmr);
  if(NO_ERR == err)
  {p_tmr->CNTR = p_tmr->ARR;  }
  return err;
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ��ʱʣ��ʱ�� (��λ����)
 Input:         //
                //
 Output:        //
 Return:	//   
 Others:        //
*********************************************************************************/
unsigned long  SofTimerRemainGet(SofTimer_Struct* p_tmr, SystemErrName *err)
{
  unsigned long timer = 0;
  unsigned long tb =0;
  *err = SofTimerVerify (p_tmr);
  if(NO_ERR == err)
  {
   switch (p_tmr->CR.bit.TB) 
   {
    case TimerTB_10MS:
     {tb = 10;break;}
    case TimerTB_100MS:
     {tb = 100;break;}
    case TimerTB_1S:
     {tb = 1000;break;}
    case TimerTB_1M:
     {tb = 60000;break;}
    case TimerTB_1H:
     {tb = 3600000;break;} 
    default :
     {break;}
   }
   timer = p_tmr->CNTR * tb; 
  }
  return timer;
}
/*********************************************************************************
 Function:      //
 Description:   //��ȡ��ʱ��״̬
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
unsigned char get_SofTimerState(SofTimer_Struct* p_tmr)
{
  SystemErrName err;
  err = SofTimerVerify (p_tmr);
  if(NO_ERR == err)
  {
    if(p_tmr->CR.bit.State)
    { return 1; }
    else
    {return 0;}
  }
  return 0;
}

/**********************************END*****************************************/
/* if(TimerTB_1M == flg.bit.TB )
 {
   get_SystemTimer (&timer);
   if((timer.Hour >= ConcentratorUpdataTimer.hour)
       &&(timer.Minute >= ConcentratorUpdataTimer.minute))
   {
    itemp  = timer.Minute;
    itemp += Concentrator.cycle.minute;
    ConcentratorUpdataTimer.minute =   itemp % 60;
    
    itemp /= 60;
    itemp += timer.Hour;
    itemp += Concentrator.cycle.hour;
    itemp %= 24;
    ConcentratorUpdataTimer.hour = itemp;
    Concentrator.CSR.Bit.Updata_flg = 1;
    LaunchReadAllValue_KindgDa( );
   }
 }*/