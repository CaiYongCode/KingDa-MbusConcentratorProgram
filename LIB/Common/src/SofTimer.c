/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\SofTimer.h"
//#include "..\inc\SystemDefine.h"
//#include "..\..\LIB\CMSIS\Device\STM32F10x\Include\stm32f10x.h"
//include "..\..\APP\inc\MCPX.h"
//#include "..\..\APP\inc\GPRS.h"
//#include "..\..\APP\inc\ReadEquipData.h"
//#include "..\..\APP\inc\Appcon_API.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
/*********************************************************************************
公共变量定义区
*********************************************************************************/
/*********************************************************************************
外部变量声明区
*********************************************************************************/


extern SofTimer_Struct*  RTC_Timer;   //RTC 守护时钟 防止RTC停转
/*********************************************************************************
私有变量定义区
*********************************************************************************/ 
static  SofTimer_Struct tmr[ SofTimer_Number ];
/*********************************************************************************
测试变量定义区
*********************************************************************************/
void WriteCH376Cmd( unsigned char mCmd ) ;
/*********************************************************************************
内部函数定义区
*********************************************************************************/
/*********************************************************************************
功能代码定义区
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
SofTimer_Struct* SofTimerCreate (unsigned short Period,     //周期
                                 SofTimer_TB tb,              //时基
                                 unsigned char opt,          //模式
                                 SofTimer_CALLBACK p_callback, //回调
                                 void *p_callback_arg)        //回调参数
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
  //{IWDG -> KR = 0xBBBB;}  //复位
 return p_tmr;
}
/*********************************************************************************
 Function:      //
 Description:   //改变定时周期
 Input:         //
                //
 Output:        //
 Return:	//
 Others:        //
*********************************************************************************/
unsigned char SofTimer_ChangePeriod (SofTimer_Struct* p_tmr,       //操纵的定时器
                                     unsigned short NewPeriod,   //新的周期
                                     unsigned char opt)          //是否复位定时器 1:重新计时  0:继续计时
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
 Description:   //强制定时立即发生一次定时溢出事件
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
      if(SofTimer_ONE_SHOT == p_tmr->CR.bit.Opt) //定时器为单次触发式工作
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
 Description:   //启动定时器
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
 Description:   //删除定时器
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
 Description:   //定时器计时复位
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
 Description:   //获取定时剩余时间 (单位毫秒)
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
 Description:   //获取定时器状态
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
 Description:   //获取定时器状态
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
      { //定时时间到达；
        if(P_NULL != tmr[itemp].CallbackPtr)
        { tmr[itemp].CallbackPtr(&tmr[itemp],tmr[itemp].CallbackPtrArg);}
        if(SofTimer_ONE_SHOT == tmr[itemp].CR.bit.Opt) //定时器为单次触发式工作
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
