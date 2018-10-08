/*********************************************************************************
//������: Operation for SI446x

//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "PCF8563.h"  
#include "..\..\User\inc\Includes.h"
/*********************************************************************************
����������
*********************************************************************************/
/*********************************************************************************
��������������
*********************************************************************************/
/*********************************************************************************
�ⲿ����������
*********************************************************************************/
TimerData_Struct  SystemTimer;

/*********************************************************************************
˽�б���������
**********************************************************************************/ 
/*********************************************************************************
���Ա���������
*********************************************************************************/
//unsigned  short  ioiiiii =0;
/*********************************************************************************
�ڲ��������Ͷ�����
*********************************************************************************/
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
 Return:		//
 Others:        //
*********************************************************************************/
static void delay_us(unsigned int us)
{
  //unsigned char i;
  while (us--);
 //   for(i=0;i<10;i++);
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
unsigned char I2C_8563_ByteWrite(unsigned char WriteAddr,unsigned char pBuffer)
{
  unsigned long itemp = 0;
  /* Send STRAT condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  /* Test on EV5 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {
   itemp ++;
   if(itemp > 0xffff)		//��ʱ
   {return 0;}
  }  
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C1, PCF8563ADDR_WRITE, I2C_Direction_Transmitter);
  /* Test on EV6 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
   itemp ++;
   if(itemp > 0xffff)		//��ʱ
   {return 0;}
  }     
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C1, WriteAddr);
  /* Test on EV8 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    itemp ++;
    if(itemp > 0xffff)		//��ʱ
    {return 0;}
  }  
  /* Send the byte to be written */
  I2C_SendData(I2C1, pBuffer); 
  /* Test on EV8 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    itemp ++;
    if(itemp > 0xffff)		//��ʱ
    {return 0;}
  }  
  /* Send STOP condition */
  I2C_GenerateSTOP(I2C1, ENABLE);
  return 1;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:	    	//
 Others:        //
*********************************************************************************/
unsigned char I2C_8563_byteRead(unsigned char ReadAddr)
{ 
  unsigned long itemp = 0; 
  unsigned char readbyte = 0;
   /* While the bus is busy */
 // I2C1_Configuration();
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
  {itemp ++;if(itemp > 0xffff){I2C1_Rset( ); break;}}		//��ʱ
  I2C_GenerateSTART(I2C1, ENABLE);
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {itemp ++;if(itemp > 0xffff){I2C1_Rset( ); break;}} 
  I2C_Send7bitAddress(I2C1, PCF8563ADDR_WRITE, I2C_Direction_Transmitter);
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {itemp ++;if(itemp > 0xffff){I2C1_Rset( );  break;} }
  /* Clear EV6 by setting again the PE bit */
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C1, ReadAddr);  
  /* Test on EV8 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {itemp ++;if(itemp > 0xffff){I2C1_Rset( ); break;}}		
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2C1, ENABLE);
  /* Test on EV5 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {itemp ++;if(itemp > 0xffff){I2C1_Rset( ); break;}}
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(I2C1, PCF8563ADDR_READ, I2C_Direction_Receiver);
  /* Test on EV6 and clear it */
  itemp = 0;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {itemp ++;if(itemp > 0xffff){I2C1_Rset( ); break;}}
  itemp = 0;
  //while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
  //{itemp ++;if(itemp > 0x4ff){I2C1_Rset( ); break;}};
  
  while(I2C_GetFlagStatus(I2C1,I2C_FLAG_RXNE) == RESET)
 {itemp ++;if(itemp > 0xffff){I2C1_Rset( ); break;}};
  readbyte = I2C_ReceiveData(I2C1);
   I2C_AcknowledgeConfig(I2C1, DISABLE);  //�ر�I2C��Ӧ����
   I2C_GenerateSTOP(I2C1, ENABLE);
  return readbyte;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void PCF8563_Set_Time(TimerData_Struct* timer)
{
 unsigned char itemp1;
 unsigned char itemp2;
#if (IIC_Mode==IIC_HARD)
  I2C1_Configuration();//��I2C
#else
  IIC_Software_Init();
#endif
 if(P_NULL == timer){return;}
 if(timer->Year > 1999)
 {
  if(timer->Year < 2099){itemp1 = timer->Year - 2000;}
  else{return;}
 }
 else if(timer->Year > 1900){itemp1 = timer->Year - 1900;}
 else{return;}
  
 if((timer->Month < 1)||(timer->Month > 12)){return;}
 if((timer->Day < 1)||( timer->Day > 31 )){return;}
 if(timer->Hour > 23){return;}
 if(timer->Minute > 59){return;} 
 if(timer->Second > 59){return;}  

 itemp2 = itemp1 / 10;
 itemp2 <<=4 ;
 itemp2 |=itemp1 % 10;
#if (IIC_Mode==IIC_HARD)
  I2C_8563_ByteWrite(YEAR, itemp2);
#else
  PCF8563_Write_Byte(YEAR, itemp2);
#endif
 

 itemp1 = timer->Month;
 itemp2 = itemp1 / 10;
 itemp2 <<=4 ;
 itemp2 |=itemp1 % 10;
 if(timer->Year < 2000){itemp2|= 0x80;}
#if (IIC_Mode==IIC_HARD)
  I2C_8563_ByteWrite(MONTH, itemp2); 
#else
  PCF8563_Write_Byte(MONTH, itemp2); 
#endif
 
 
 itemp1 = timer->Day;
 itemp2 = itemp1 / 10;
 itemp2 <<=4 ;
 itemp2 |=itemp1 % 10; 
#if (IIC_Mode==IIC_HARD)
  I2C_8563_ByteWrite(DAY, itemp2);//
#else
  PCF8563_Write_Byte(DAY, itemp2);//
#endif
 

 timer->WeekDay = CaculateWeekDay(timer->Year,timer->Month,timer->Day);//������������
#if (IIC_Mode==IIC_HARD)
  I2C_8563_ByteWrite(WEEK, timer->WeekDay);//��������
#else
  PCF8563_Write_Byte(WEEK, timer->WeekDay);//��������
#endif
 

 itemp1 = timer->Hour;
 itemp2 = itemp1 / 10;
 itemp2 <<=4 ;
 itemp2 |=itemp1 % 10; 
#if (IIC_Mode==IIC_HARD)
  I2C_8563_ByteWrite(HOUR, itemp2);//
#else
  PCF8563_Write_Byte(HOUR, itemp2);//
#endif
 
 itemp1 = timer->Minute;
 itemp2 = itemp1 / 10;
 itemp2 <<=4 ;
 itemp2 |=itemp1 % 10; 
#if (IIC_Mode==IIC_HARD)
  I2C_8563_ByteWrite(MINUTE, itemp2);//
#else
  PCF8563_Write_Byte(MINUTE, itemp2);//
#endif

 itemp1 = timer->Second;
 itemp2 = itemp1 / 10;
 itemp2 <<=4 ;
 itemp2 |=itemp1 % 10;  
#if (IIC_Mode==IIC_HARD)
  I2C_8563_ByteWrite(SECOND, itemp2);//
#else
  PCF8563_Write_Byte(SECOND, itemp2);//
#endif
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void PCF8563_Read_time(TimerData_Struct* timer)
{
  unsigned char itemp1;
  unsigned char itemp2;
  unsigned char flg;
#if (IIC_Mode==IIC_HARD)
  I2C1_Configuration();//��I2C
#else
  IIC_Software_Init();
#endif
#if (IIC_Mode==IIC_HARD )
  itemp1 = I2C_8563_byteRead(MONTH);//
#else
  itemp1 = PCF8563_Read_Byte(MONTH);
#endif
  flg = itemp1 & 0x80;
  itemp1 &= 0x1F; 
  itemp2 = itemp1 & 0x0F;
  itemp1 >>= 4;
  itemp2 += itemp1 * 10;
  timer->Month = itemp2 ;
  
#if (IIC_Mode==IIC_HARD )
  itemp1 = I2C_8563_byteRead(YEAR);//
#else
  itemp1 = PCF8563_Read_Byte(YEAR);
#endif
  itemp2 = itemp1 & 0x0F;
  itemp1 >>= 4;
  itemp2 += itemp1 * 10;
  if(flg){timer->Year = itemp2 + 1900;}
  else{timer->Year = itemp2 + 2000;}
  
#if (IIC_Mode==IIC_HARD )
  itemp1 = (I2C_8563_byteRead(DAY) & 0x3F);//
#else
  itemp1 = (PCF8563_Read_Byte(DAY) & 0x3F);
#endif
  itemp2 = itemp1 & 0x0F;
  itemp1 >>= 4;
  itemp2 += itemp1 * 10;
  timer->Day = itemp2 ;
  
#if (IIC_Mode==IIC_HARD )
  timer->WeekDay = (I2C_8563_byteRead(WEEK) & 0x07);//
#else
  timer->WeekDay = (PCF8563_Read_Byte(WEEK) & 0x07);//
#endif
  
 
#if (IIC_Mode==IIC_HARD )
  itemp1 = (I2C_8563_byteRead(HOUR) & 0x3F);//
#else
  itemp1 = (PCF8563_Read_Byte(HOUR) & 0x3F);//
#endif  
  itemp2 = itemp1 & 0x0F;
  itemp1 >>= 4;
  itemp2 += itemp1 * 10;
  timer->Hour = itemp2 ;
  
#if (IIC_Mode==IIC_HARD )
  itemp1 = (I2C_8563_byteRead(MINUTE)& 0x7F);//
#else
  itemp1 = (PCF8563_Read_Byte(MINUTE)& 0x7F);//
#endif  
  itemp2 = itemp1 & 0x0F;
  itemp1 >>= 4;
  itemp2 += itemp1 * 10;
  timer->Minute = itemp2 ;
  
//  delay_us(20);
#if (IIC_Mode==IIC_HARD )
  itemp1 = (I2C_8563_byteRead(SECOND)& 0x7F);//
#else
  itemp1 = (PCF8563_Read_Byte(SECOND)& 0x7F);//
#endif  
  itemp2 = itemp1 & 0x0F;
  itemp1 >>= 4;
  itemp2 += itemp1 * 10;
  timer->Second = itemp2 ;
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void get_SystemTimer(TimerData_Struct* Timer)
{
   vTaskSuspendAll();  //����������
   
   Timer->Second = SystemTimer.Second;
   Timer->Year = SystemTimer.Year;
   Timer->Month = SystemTimer.Month;
   Timer->Day = SystemTimer.Day;
   Timer->WeekDay = SystemTimer.WeekDay;
   Timer->Hour = SystemTimer.Hour;
   Timer->Minute = SystemTimer.Minute;
   
   xTaskResumeAll ();  //�������ر�
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:		//
 Others:        //
*********************************************************************************/
void F8563_woring_mode(unsigned char mod)//����ģʽ����
{
 I2C1_Configuration();//��I2C
 mod &= WARING_DEFINE_TIME; 
 mod |= 0x80;   //�����ж�
 I2C_8563_ByteWrite(CONTROL2,mod);  //
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
void F8563_worning_init(unsigned char hour,unsigned char minute)  //��������
{
 I2C_8563_ByteWrite(WAR_MINUTE,minute);
 I2C_8563_ByteWrite(WAR_HOUR,hour);      
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
void F8563_back_init(unsigned char time, unsigned char TD)  //����ʱ����
{
  I2C1_Configuration();//��I2C
  TD &= 0x03;
  TD |= 0x80; //����ʱ��Ч
  I2C_8563_ByteWrite(DTIME_CONTROL,TD);   //0x80  Ƶ������ 1/60Hz  ��λΪ����
  I2C_8563_ByteWrite(TIME_CONT,time);   		// 	
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
unsigned char CaculateWeekDay(unsigned short y,unsigned char m, unsigned char d)//����
{
  if(m==1||m==2)
  {
    m+=12;
    y--;
  }
  unsigned char iWeek=(d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7+1;
  return iWeek;
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
void F8563_Interrupt(void)  //F8563 �жϴ�����
{
//  unsigned char flg;
//  I2C1_Configuration();//��I2C
//  flg = I2C_8563_byteRead( CONTROL2 );
//  I2C_8563_ByteWrite(CONTROL2,flg & 0x03); //����жϱ�־
//  if(flg & 0x04)  //��ʱ�ж�
//  {
//    PCF8563_Adj_Read_time(&Clock);//ȡϵͳʱ�� 
//  }
//  if(flg & 0x08)  //�����ж�
//  { flg = 0;}
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void IIC_Software_Init(void)    //IIC�����ʼ��
{                          
  GPIO_InitTypeDef GPIO_InitStructure;  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
       
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT ;  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;  
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
  GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);  //PB8,PB9 �����  
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void IIC_Soft_Start(void)  
{  
  SDA_OUT();     //sda�����  
  IIC_SDA_SET;          
  IIC_SCL_SET;  
  delay_us(4);  
  IIC_SDA_RESET;//START:when CLK is high,DATA change form high to low   
  delay_us(4);  
  IIC_SCL_RESET;//ǯסI2C���ߣ�׼�����ͻ��������   
} 
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
//����IICֹͣ�ź�  
void IIC_Soft_Stop(void)  
{  
  SDA_OUT();//sda�����  
  IIC_SDA_RESET;  
  IIC_SCL_SET;//STOP:when CLK is high DATA change form low to high  
  delay_us(4);  
  IIC_SDA_SET;  
  delay_us(4);
  IIC_SCL_SET;   
} 
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
unsigned char IIC_Soft_Wait_Ack(void)  
{  
  unsigned short ucErrTime=0;  
  SDA_IN();      //SDA����Ϊ����    
  //IIC_SDA_SET;   
  //delay_us(1);       
  IIC_SCL_SET;
  delay_us(1);     
  while(READ_SDA) 
  {  
    ucErrTime++;  
    if(ucErrTime>250)  
    {  
      IIC_Soft_Stop();  
      return 1;  
    }  
  }  
  IIC_SCL_RESET;//ʱ�����0        
  return 0;    
}  
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void IIC_Soft_Ack(void)  
{  
  IIC_SCL_RESET;  
  SDA_OUT();  
  IIC_SDA_RESET;  
  delay_us(2);  
  IIC_SCL_SET;  
  delay_us(2);  
  IIC_SCL_RESET;  
} 
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void IIC_Soft_NAck(void)  
{  
  IIC_SCL_RESET;  
  SDA_OUT();  
  IIC_SDA_SET;  
  delay_us(2);  
  IIC_SCL_SET;  
  delay_us(2);  
  IIC_SCL_RESET;  
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void IIC_Soft_Send_Byte(unsigned txd)  
{                          
  unsigned char t;     
  SDA_OUT();        
  IIC_SCL_RESET;//����ʱ�ӿ�ʼ���ݴ���  
  for(t=0;t<8;t++)  
  {                
       
    if((txd&0x80)>>7)  
      IIC_SDA_SET;  
    else  
      IIC_SDA_RESET;  
    txd<<=1;      
    delay_us(2);    
    IIC_SCL_SET;  
    delay_us(2);   
    IIC_SCL_RESET;    
    delay_us(2);  
  }      
}    

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
unsigned char IIC_Soft_Read_Byte(unsigned char ack)  
{  
  unsigned char i,receive=0;  
  SDA_IN();//SDA����Ϊ����  
  for(i=0;i<8;i++ )  
  {  
    IIC_SCL_RESET;   
    delay_us(2);  
    IIC_SCL_SET;  
    receive<<=1;  
    if(READ_SDA)receive++;     
    delay_us(1);   
  }                      
  if (!ack)  
    IIC_Soft_NAck();//����nACK  
  else  
    IIC_Soft_Ack(); //����ACK     
  return receive;  
}  
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void PCF8563_Write_Byte(unsigned char REG_ADD, unsigned char dat)  
{  
  unsigned char i;
  IIC_Soft_Start();  
  IIC_Soft_Send_Byte(PCF8563_Write);//����д������Ӧ��λ  
   i = 100;
  while(IIC_Soft_Wait_Ack( )&&i!=0)
   i--;  
  IIC_Soft_Send_Byte(REG_ADD);  
  IIC_Soft_Wait_Ack();  
  IIC_Soft_Send_Byte(dat); //��������    
  IIC_Soft_Wait_Ack();  
  IIC_Soft_Stop();  
}  
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
unsigned char PCF8563_Read_Byte(unsigned char REG_ADD)  
{  
  unsigned char ReData=0;  
  unsigned char i;
  IIC_Soft_Start( );  
  IIC_Soft_Send_Byte(PCF8563_Write);   //����д������Ӧ��λ 
  i = 100;
  while(IIC_Soft_Wait_Ack( )&&i!=0)
    i--;  
  IIC_Soft_Send_Byte(REG_ADD); //ȷ��Ҫ�����ļĴ���  
  IIC_Soft_Wait_Ack();  
  IIC_Soft_Start();    //��������  
  IIC_Soft_Send_Byte(PCF8563_Read);    //���Ͷ�ȡ����  
  IIC_Soft_Wait_Ack();  
  ReData = IIC_Soft_Read_Byte(0);  //��ȡ����,�ӷ��ͷ�Ӧ��  
  IIC_Soft_Stop();  
  return ReData;  
} 
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/