
#ifndef __PCF8563_H__
#define __PCF8563_H__



#define PCF8563ADDR_WRITE 	0xA2
#define PCF8563ADDR_READ  	0xA3

/*******RTC 各寄存器  报警寄存器最高位为0 有效**********/
#define  CONTROL1       0X00
#define  CONTROL2  	 		0x01
#define  SECOND				0x02
#define  MINUTE				0x03
#define  HOUR 				0x04
#define  DAY     			0x05
#define  WEEK     			0x06
#define  MONTH				0x07
#define  YEAR  		 		0x08
#define  WAR_MINUTE 		        0x09
#define  WAR_HOUR			0x0A		
#define  WAR_DAY			0x0B
#define	 WAR_WEEK			0x0C
#define  DTIME_CONTROL          	0x0E
#define  TIME_CONT			0x0F
/*  中断方式  定时 或 报警中断*/
#define WARING_TIME 			0x02
#define DEFINE_TIME 			0x01
#define WARING_DEFINE_TIME 	        0x03

/*  定时器周期选择*/
#define PCF8563_TD_1_4096S 	        0x00
#define PCF8563_TD_1_64S 		        0x01
#define PCF8563_TD_1S		            0x02
#define PCF8563_TD_60s 	            0x03

#define IIC_SOFT   1 //软件模式
#define IIC_HARD   2 //硬件模式
#define IIC_Mode    IIC_SOFT

#define SDA_IN()  {GPIOB->MODER&=0XFF3FFFFF;GPIOB->MODER|=0X00000000;}  
#define SDA_OUT() {GPIOB->MODER&=0XFF3FFFFF;GPIOB->MODER|=0X00400000;}

#define PCF8563_Write                            (unsigned char)0xa2  //写命令  
#define PCF8563_Read                             (unsigned char)0xa3  //读命令

#define IIC_SDA_SET     GPIO_SetBits(GPIOB,GPIO_Pin_11)
#define IIC_SDA_RESET   GPIO_ResetBits(GPIOB,GPIO_Pin_11)
#define IIC_SCL_SET     GPIO_SetBits(GPIOB,GPIO_Pin_10)
#define IIC_SCL_RESET   GPIO_ResetBits(GPIOB,GPIO_Pin_10)
#define READ_SDA        GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)


//#define INTTER_MODE 	                WARING_TIME

typedef struct{
 unsigned char Second;
 unsigned char Minute;
 unsigned char Hour;
 unsigned char WeekDay;
 unsigned char Day;
 unsigned char Month;
 unsigned short Year;
}TimerData_Struct;

extern TimerData_Struct  SystemTimer;

static void delay_us(unsigned int us);
unsigned char I2C_8563_ByteWrite(unsigned char WriteAddr,unsigned char pBuffer);
unsigned char I2C_8563_byteRead(unsigned char ReadAddr);
void PCF8563_Set_Time(TimerData_Struct* timer); //设置时间
void PCF8563_Read_time(TimerData_Struct* timer);//读取时间
void F8563_woring_mode(unsigned char mod);//
void F8563_worning_init(unsigned char hour,unsigned char minute);  ///   
void F8563_back_init(unsigned char time, unsigned char TD);  /// 
void F8563_Interrupt(void);  // 
void get_SystemTimer(TimerData_Struct* timer);
unsigned char CaculateWeekDay(unsigned short y,unsigned char m, unsigned char d);//算周

void IIC_Soft_Start(void);
void IIC_Soft_Stop(void);
unsigned char IIC_Soft_Wait_Ack(void);
void IIC_Soft_Ack(void);
void IIC_Soft_NAck(void);  
void IIC_Software_Init(void);    //IIC软件初始化
void IIC_Soft_Send_Byte(unsigned txd);
unsigned char IIC_Soft_Read_Byte(unsigned char ack)  ;
void PCF8563_Write_Byte(unsigned char REG_ADD, unsigned char dat);
unsigned char PCF8563_Read_Byte(unsigned char REG_ADD);
void PCF8563_Adj_Read_time(TimerData_Struct* timer); //调准时间
#endif 



