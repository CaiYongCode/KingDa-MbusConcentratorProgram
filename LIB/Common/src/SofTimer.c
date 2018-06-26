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
//include "..\..\APP\inc\MCPX.h"
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
static  unsigned char SofTimerVerify (SofTimer_Struct *p_tmr) 
{
 unsigned char itemp = 0;
 while (itemp < SofTimer_Number)
 {
   if(p_tmr == &(tmr[itemp]))
   {
     return 1;
   }
   itemp ++;
 }
 return 0;
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
  //else
  //{IWDG -> KR = 0xBBBB;}  //��λ
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
unsigned char SofTimer_ChangePeriod (SofTimer_Struct* p_tmr,       //���ݵĶ�ʱ��
                                     unsigned short NewPeriod,   //�µ�����
                                     unsigned char opt)          //�Ƿ�λ��ʱ�� 1:���¼�ʱ  0:������ʱ
{
  if(SofTimerVerify (p_tmr))
  {
    p_tmr->ARR = NewPeriod;
    if(opt)
   {p_tmr->CNTR = p_tmr->ARR;}
   return 1;
  }
  return 0;  
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
unsigned char SofTimer_Compelling(SofTimer_Struct* p_tmr)
{
  if(SofTimerVerify (p_tmr))
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
    return 1;
  }
  return 0;
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
unsigned char SofTimerStart(SofTimer_Struct* p_tmr) 
{
  if(SofTimerVerify (p_tmr))
  {
    p_tmr->CR.bit.State = SofTimer_RUN;
    return 1;
  }
  return 0;
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
unsigned char SofTimerStop(SofTimer_Struct* p_tmr) 
{
  if(SofTimerVerify (p_tmr))
  {
    p_tmr->CR.bit.State = SofTimer_STOP;
    return 1;
  }
  return 0;
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
void SofTimerDel(SofTimer_Struct** p_tmr) 
{
  if(SofTimerVerify (*p_tmr))
  {
    (*p_tmr)->CR.bit.EN = SofTimer_IDLE;
     *p_tmr = P_NULL;
  }
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
void SofTimerRset(SofTimer_Struct*  p_tmr)          
{
  if(SofTimerVerify (p_tmr))
  {p_tmr->CNTR = p_tmr->ARR;  }
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
unsigned char SofTimerRemainGet(SofTimer_Struct* p_tmr, unsigned long* ms_cnt)
{
  unsigned long tb =0;
  if(SofTimerVerify (p_tmr))
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
   *ms_cnt = p_tmr->CNTR * tb;
   return 1;
  }
  return 0;
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
  if(SofTimerVerify (p_tmr))
  {
    if(p_tmr->CR.bit.State)
    { return 1; }
  }
  return 0;
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
void SofTimerScan(unsigned char tb)
{
  unsigned short itemp =0;
  
  while (itemp < SofTimer_Number)
  {
    if(SofTimer_USE == (tmr[itemp].CR.bit.EN )
       &&(SofTimer_RUN == tmr[itemp].CR.bit.State) 
         && (tb == tmr[itemp].CR.bit.TB))
    {
      if(tmr[itemp].CNTR <=1)
      { //��ʱʱ�䵽�
        if(P_NULL != tmr[itemp].CallbackPtr)
        { tmr[itemp].CallbackPtr(&tmr[itemp],tmr[itemp].CallbackPtrArg);}
        if(SofTimer_ONE_SHOT == tmr[itemp].CR.bit.Opt) //��ʱ��Ϊ���δ���ʽ����
        {tmr[itemp].CR.bit.EN = SofTimer_IDLE;}
        else
        {tmr[itemp].CNTR = tmr[itemp].ARR;}
      }
      else
      {tmr[itemp].CNTR -- ;}
    }
    itemp ++;
  }
}
/**********************************END*****************************************/
