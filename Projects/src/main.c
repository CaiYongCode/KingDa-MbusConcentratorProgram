/*******************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*******************************************************************************/
/*******************************************************************************
文件包含区
*******************************************************************************/
#include "..\..\User\inc\Includes.h"
#include "..\..\User\inc\Server188.h"
/*******************************************************************************
常量定义区
*******************************************************************************/
const unsigned char VersionInfo[]= "KD_V3.0.6";
/*******************************************************************************
公共变量定义区
*******************************************************************************/
/*******************************************************************************
外部变量声明区
*******************************************************************************/
//extern ConcentratorParameterStruct   Concentrator;
/*******************************************************************************
私有变量定义区
*******************************************************************************/
static TaskHandle_t xLaunch_Systerm = NULL;

/*******************************************************************************
测试变量定义区
*******************************************************************************/

/*******************************************************************************
内部函数定义区
*******************************************************************************/
/*******************************************************************************
功能代码定义区
*******************************************************************************/
void FactorySettings( void )
{
  unsigned char itemp = 0;
  //while(itemp < sizeof(ConcentratorParameterStruct))
  //{
   // ((unsigned char*)&Concentrator)[itemp++] = 0;
 // }
  //EEROM_Write(ConCofigDataAtEEROM,&Concentrator,sizeof(ConcentratorParameterStruct));
  
  while(itemp < sizeof(Server188_ConfigData_Stuct))
  {
    ((unsigned char*)&_188_ConcenConfigData)[itemp++] = 0;
  }
  _188_ConcenConfigData.AreaCode = 0xEE;   //区域码
  //_188_ConcenConfigData.ID[ConcentratorAddr_Length]; //集中器地址 5字节BCD码
  _188_ConcenConfigData.timer.ReadingCycle.Byte.Byte_H = 1440 >>8;
  _188_ConcenConfigData.timer.ReadingCycle.Byte.Byte_L = 1440 & 0x00ff;
  _188_ConcenConfigData.timer.DayFrozen = 23;
  _188_ConcenConfigData.timer.MonthFrozenH = 1;
  _188_ConcenConfigData.timer.MonthFrozenL = 23;  
  _188_ConcenConfigData.timer.FreezingSwitch.Byte = 0xFF;

  strcpy(_188_ConcenConfigData.gprs.APN, "CMNET");
  _188_ConcenConfigData.gprs.BeatCycle = 3;
  _188_ConcenConfigData.gprs.ConnectionMode.Bit.D0 = 0;
  _188_ConcenConfigData.gprs.ConnectionMode.Bit.D1 = 1;  //1使用APN 
  _188_ConcenConfigData.gprs.DNS[0] ='\0';
  _188_ConcenConfigData.gprs.IP[0] = 112;
  _188_ConcenConfigData.gprs.IP[1] = 25;
  _188_ConcenConfigData.gprs.IP[2] = 204;
  _188_ConcenConfigData.gprs.IP[3] = 3; 
  _188_ConcenConfigData.gprs.Port.Byte.Byte_H = 15001 >>8 ;
  _188_ConcenConfigData.gprs.Port.Byte.Byte_L = 15001 & 0x00FF ;
  EEROM_Write(Server188_ConfigData_AddrEeprom,&_188_ConcenConfigData,sizeof(Server188_ConfigData_Stuct));
  
  itemp = 0;
  while ('\0' != VersionInfo[itemp++]);
  EEROM_Write(VersionInfoAtEEROM,(void*)VersionInfo,itemp);
}
/*******************************************************************************
 Function:      //
 Description:   // 
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*******************************************************************************/
void SystemLoad ( void )
{
  unsigned char itemp = 0;
  unsigned char* p = (unsigned char*)VersionInfoAtEEROM;
    
  itemp = *p;
  itemp = 0;
  while ('\0' != VersionInfo[itemp])
  {
    if(VersionInfo[itemp] != ((unsigned char*)VersionInfoAtEEROM)[itemp])
    {break;}
    else
    {itemp ++;}
  }
  if('\0' == VersionInfo[itemp])   //配置数据正确 加载数据集 
  { 
    itemp = 0;
   while(itemp < sizeof(Server188_ConfigData_Stuct))
   {
     ((unsigned char*)&_188_ConcenConfigData)[itemp] = ((unsigned char*)Server188_ConfigData_AddrEeprom)[itemp];
     itemp++;
   }
  }
  else   //恢复默认配置数据
  {
    FactorySettings( );
  }
 // FactorySettings( );
}
/*******************************************************************************
 Function:      //
 Description:   // 
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*******************************************************************************/
int main(void)
{
  __set_PRIMASK(1);
  
  MCU_init ( );
  SystemLoad ( );
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
  SysTick_Config(320000);
  
  DeInit_UsartBuff( );
   
 IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
 IWDG_SetPrescaler(IWDG_Prescaler_32);  //预分频因子 f =1250 
 IWDG_SetReload(3125);    // 2.5 秒 
 IWDG_ReloadCounter( ); //复位看门狗
 IWDG_Enable( );//启动看门狗工作
 //IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);
 
 //建立 系统启动及监视任务 优先级最高
 xTaskCreate(Launch_Systerm,     /* 任务函数 */
             "vTaskUserIF",      /* 任务名 */
              512,               /* stack 大小，单位 word*/
              NULL,              /* 任务参数 */
              configMAX_PRIORITIES -1,      /* 任务优先级 */
              &xLaunch_Systerm);            /* 任务句柄 */
 vTaskStartScheduler( );

 
while(1);
}
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif
/**************************************END*************************************/



