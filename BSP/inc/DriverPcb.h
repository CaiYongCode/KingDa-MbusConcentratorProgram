/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef  DRIVER_PCB_H
#define  DRIVER_PCB_H 
/*********************************************************************************
文件包含区
*********************************************************************************/
#include ".\MCU_Config.h"
#include "..\..\LIB\Common\inc\Common_API.h"
/*********************************************************************************
宏定义区
*********************************************************************************/
#define key_cnt          10

#define VREFINT          1224

#define  GPRSPower_state       ( _4V2_EN_ENpin == 0 ? 0 :1)       
#define  GPRSPower_ON()        set_4V2_ENpin        
#define  GPRSPower_OFF()       clr_4V2_ENpin         


#define  HVPower_state         ( HV_EN_ENpin == 0 ? 0 :1)           
#define  HVPower_ON()          set_HV_ENpin         
#define  HVPower_OFF()         clr_HV_ENpin         

#define  GPRS_ONOFF_state      ( GPRS_ONOFFpin == 0 ? 0 :1) 
#define  set_GPRS_ONOFF()        set_GPRS_ONOFFpin    
#define  clr_GPRS_ONOFF()        clr_GPRS_ONOFFpin

#define  GPRS_REST_state       ( GPRS_RESTpin == 0 ? 0 :1) 
#define  set_GPRS_REST()         set_GPRS_RESTpin     
#define  clr_GPRS_REST()         clr_GPRS_RESTpin      

   

#define  Mbus_CH1_State       ( OUT_CH1_ENpin == 0 ? 0 :1)        
#define  Mbus_CH1_ON( )       clr_Mbus_CH1_ENpin    
#define  Mbus_CH1_OFF( )      set_Mbus_CH1_ENpin  

#define  Mbus_CH2_State       ( OUT_CH2_ENpin == 0 ? 0 :1)       
#define  Mbus_CH2_ON( )       clr_Mbus_CH2_ENpin    
#define  Mbus_CH2_OFF( )      set_Mbus_CH2_ENpin 

#define  Mbus_CH3_State       ( OUT_CH3_ENpin == 0 ? 0 :1)       
#define  Mbus_CH3_ON( )       clr_Mbus_CH3_ENpin   
#define  Mbus_CH3_OFF( )      set_Mbus_CH3_ENpin  

#define  Mbus_CH4_State       ( OUT_CH4_ENpin == 0 ? 0 :1)       
#define  Mbus_CH4_ON( )       clr_Mbus_CH4_ENpin      
#define  Mbus_CH4_OFF( )      set_Mbus_CH4_ENpin
         
#define  LED1_OFF()            set_LED1pin             
#define  LED1_ON()             clr_LED1pin 

#define  LED2_OFF()            set_LED2pin             
#define  LED2_ON()             clr_LED2pin 



#define  RLED_OFF()          LED1_OFF()                
#define  RLED_ON()           LED1_ON()             


#define  GLED_OFF()          LED2_OFF()                
#define  GLED_ON()           LED2_ON()   
/*********************************************************************************
数据类型定义
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/
extern unsigned short  Vin ;    //电源电压 MV
extern unsigned short  Vout ;     //输出电压 MV
extern unsigned short  Iout ;     //输出电流 MA
extern BIT_8W Prot_state;   
extern BIT_8W Prot_UP;
extern BIT_8W Prot_DW;
/*********************************************************************************
函数声明区
*********************************************************************************/
void ADC1_TCIE_IRQ (void);
void AnalogComput (void);
void Port_sacn (void);
unsigned char EEROM_Read (unsigned long Address, void* buff, unsigned short length);
unsigned char EEROM_Write (unsigned long Address, void* buff, unsigned short length);
unsigned short getVin ( void );
/********************************************************************************/
#endif













