/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include ".\Includes.h"
//#include ".\KingDaServer.h"
#include ".\Server188.h"
#include<stdio.h>
#include<string.h>
/*********************************************************************************
����������
*********************************************************************************/

/*********************************************************************************
��������������
*********************************************************************************/
GPRS_MODE_struct gprs; 
/*********************************************************************************
�ⲿ����������
*********************************************************************************/
//extern ConcentratorParameterStruct   Concentrator;
extern ServerState_Struct    ServerState[2];

/*********************************************************************************
˽�б���������
*********************************************************************************/
static  unsigned char           Gprs_buff[GPRS_GprsBuffLength];
static  unsigned long           GprsData_length = 0;
static  UsarBuff_Struct*         InputBuff;

#define  GprsSendBuff             Gprs_buff
#define  GprsSendByteNumber       GprsData_length
#define  GprsRecveBuff            Gprs_buff
#define  GprsRecveByteNumber      GprsData_length

/*********************************************************************************
���Ա���������
*********************************************************************************/

/*********************************************************************************
�ڲ�����������
*********************************************************************************/
static unsigned char*  IP32ToString(unsigned long ip, unsigned char* string);
static unsigned char StringIPtoU32(unsigned char* String, unsigned long* ip);
//static void Display_func(void* buff );
static void GPRS_LinkCloseFunc(unsigned char CH);
/*********************************************************************************
���ܴ��붨����
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
/*static void clrRecveBuff( void)
{
  while(pdPASS == xQueueReceive(GprsQueue, &InputBuff, 0))
  {DeleteDataFromUsartBuff(InputBuff);} 
}*/
/*********************************************************************************
Function:      //
Description:   // ѡ��绰������洢
Input:         //
//
Output:        //
Return:        //
Others:        //AT+CPIN?   +CPIN:READY   OK
*********************************************************************************/ 
Cmd_ACK_type _CPBS_func(void)
{
   Cmd_ACK_type   ACK = RecErr;
   unsigned long itemp; 
  unsigned long seat; 
     weitGPRS_COMM( ); 
  sendStringByDMA_USART (GPRS_COMM, "AT+CPBS=\"SM\"\r\n"); //�������� 
  
  do{
    if(pdPASS != xQueueReceive(GprsQueue, &InputBuff, _1s * 2))
    { return ACK;}   //Ӧ��ʱ
    else //��Ӧ�� 
    {
      GprsData_length = GetDataFromUsart (InputBuff,GprsRecveBuff, GPRS_GprsBuffLength);
      GprsRecveBuff[GprsData_length] = '\0';
      {
        seat = 0;
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"AT+CPBS=\"SM\"",&itemp))
        {ACK = RecOK;}
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"OK",&itemp))
        {ACK = ExeOK;}
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"ERROR",&itemp))
        {ACK = ExeErr;}   
        if((ExeOK == ACK)||(ExeErr == ACK))
        {return ACK;}
      }
    }
  }while (1);
}
/*********************************************************************************
Function:      //
Description:   // д�绰����Ŀ 
Input:         //
//
Output:        //
Return:        //
Others:        //AT+CPIN?   +CPIN:READY   OK
*********************************************************************************/ 
Cmd_ACK_type _CPBW_func(unsigned char index, char* number, char type, char* text)
{
  Cmd_ACK_type   ACK = RecErr;
  unsigned long itemp; 
  unsigned long seat; 
  weitGPRS_COMM( ); 
  sprintf((char *)GprsSendBuff, "AT+CPBW=%d,\"%s\",%d,\"%s\"\r\n",index,number,type,text);
  sendStringByDMA_USART (GPRS_COMM, GprsSendBuff); //�������� 
  
  do{
    if(pdPASS != xQueueReceive(GprsQueue, &InputBuff, _1s * 2))
    { return ACK;}   //Ӧ��ʱ
    else //��Ӧ�� 
    {
      GprsData_length = GetDataFromUsart (InputBuff,GprsRecveBuff, GPRS_GprsBuffLength);
      GprsRecveBuff[GprsData_length] = '\0';
      {
        seat = 0;
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"AT+CPBW=",&itemp))
        {ACK = RecOK;}
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"OK",&itemp))
        {ACK = ExeOK;}
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"ERROR",&itemp))
        {ACK = ExeErr;}   
        if((ExeOK == ACK)||(ExeErr == ACK))
        {return ACK;}
      }
    }
  }while (1);
}
/*********************************************************************************
Function:      //
Description:   // ���ҵ绰����Ϣ
Input:         //
//
Output:        //
Return:        //
Others:        //  AT+CPBF="john"  +CPBF: 1,"130********",129,"john"
*********************************************************************************/ 
Cmd_ACK_type _CPBF_FormText(char* text,unsigned char* index, char* type, char* number)
{
  Cmd_ACK_type   ACK = RecErr;
  unsigned char flg = 0;
  char * p;
  weitGPRS_COMM( ); 
  sprintf((char *)GprsSendBuff, "AT+CPBF=\"%s\"\r\n",text);
  sendStringByDMA_USART (GPRS_COMM, GprsSendBuff); //�������� 
  
  do{
    if(pdPASS != xQueueReceive(GprsQueue, &InputBuff, _1s * 2))
    { return ACK;}   //Ӧ��ʱ
    else //��Ӧ�� 
    {
      GprsData_length = GetDataFromUsart (InputBuff,GprsRecveBuff, GPRS_GprsBuffLength);
      GprsRecveBuff[GprsData_length] = '\0';
      {
        p = strstr ((const char *)&GprsRecveBuff,"AT+CPBF=");
        if(p)
        {ACK = RecOK;}
        p = strstr ((const char *)&GprsRecveBuff,text); 
        if(p)
        {
          p = strstr ((const char *)&GprsRecveBuff,"+CPBF:");
          if(p)
          {
            p += strlen("+CPBF:");
            *index = 0;
            p = strpbrk (p,"123456789");   //index
            if(p)
            {
              do{
                *index *= 10;
                *index += *p++ -'0';
              }while(IsDigit(*p));
              p = strpbrk (p,"123456789");  //number
              if(p)
              {
                do{
                  *number ++ = *p++;
                }while(IsDigit(*p));
                *number = '\0';
                *type = 0;
                p = strpbrk (p,"123456789");   //type
                if(p)
                {
                  do{
                    *type *= 10;
                    *type += *p++ -'0';
                  }while(IsDigit(*p));
                  flg = 1;
                }
              }
            }
          }
        }

        if(0 != strstr((const char *)GprsRecveBuff,"OK"))
        {
          if(0 != flg)
          {ACK = ExeOK;}
        }
        if(0 != strstr((const char *)GprsRecveBuff,"ERROR"))
        {ACK = ExeErr;}   
        if((ExeOK == ACK)||(ExeErr == ACK))
        {return ACK;}
      }
    }
  }while (1);
}
/*********************************************************************************
Function:      //
Description:   // ��ȡ�绰����Ϣ 
Input:         //
//
Output:        //
Return:        //
Others:        //AT+CPIN?   +CPIN:READY   OK
*********************************************************************************/ 
/*
Cmd_ACK_type _CPBW_func(unsigned char index, char* number, char* type, char* text)
{
  Cmd_ACK_type   ACK = RecErr;
  unsigned long itemp; 
  unsigned long seat; 
  weitGPRS_COMM( ); 
  sprintf((char *)GprsSendBuff, "AT+CPBW=%d,\"%s\",%d,\"%s\"\r\n",index,number,type,text);
  sendStringByDMA_USART (GPRS_COMM, GprsSendBuff); //�������� 
  
  do{
    if(pdPASS != xQueueReceive(GprsQueue, &InputBuff, _1s * 2))
    { return ACK;}   //Ӧ��ʱ
    else //��Ӧ�� 
    {
      GprsData_length = GetDataFromUsart (InputBuff,GprsRecveBuff, GPRS_GprsBuffLength);
      GprsRecveBuff[GprsData_length] = '\0';
      {
        seat = 0;
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"AT+CPBW=",&itemp))
        {ACK = RecOK;}
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"OK",&itemp))
        {ACK = ExeOK;}
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"ERROR",&itemp))
        {ACK = ExeErr;}   
        if((ExeOK == ACK)||(ExeErr == ACK))
        {return ACK;}
      }
    }
  }while (1);
}*/
/*********************************************************************************
Function:      //
Description:   // ���SIM��״̬ 
Input:         //
//
Output:        //
Return:        //
Others:        //AT+CPIN?   +CPIN:READY   OK
*********************************************************************************/ 
Cmd_ACK_type _CPIN_func(void)
{
  Cmd_ACK_type   ACK = RecErr;
  unsigned long itemp; 
  unsigned long seat; 
  weitGPRS_COMM( ); 
  sendStringByDMA_USART (GPRS_COMM, "AT+CPIN?\r\n"); //�������� 
  do{
    if(pdPASS != xQueueReceive(GprsQueue, &InputBuff, _1s * 2))
    { return ACK;}   //Ӧ��ʱ
    else //��Ӧ�� 
    {
      GprsData_length = GetDataFromUsart (InputBuff,GprsRecveBuff, GPRS_GprsBuffLength);
      GprsRecveBuff[GprsData_length] = '\0';
      //if(0 == Display_func(GprsRecveBuff))
      {
        seat = 0;
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"AT+CPIN?",&itemp))
        {
          seat += 8 + itemp;
          ACK = RecOK;
        }
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"CPIN:",&itemp))
        {
          seat += 5 + itemp;
          ACK = ExeOK;
        }
        else if(0 != SeekMarkString(&GprsRecveBuff[seat],"ERROR",&itemp))
        {
          seat += itemp;
          ACK = ExeErr;
        }   
        if((ExeOK == ACK)||(ExeErr == ACK))
        {return ACK;}
      }
    }
  }while (1);
}
/*********************************************************************************
Function:      //
Description:   // ��������ź�ǿ�� 
Input:         //
//
Output:        //
Return:        //
Others:        //AT+CSQ    CSQ:20��0   OK   +CSQ: <rssi>,<ber>
*********************************************************************************/ 
Cmd_ACK_type _CSQ_func(void)
{
  Cmd_ACK_type   ACK = RecErr;
  unsigned char rssi; 
  unsigned char ber;
  unsigned long itemp;
  unsigned long seat;
  
  weitGPRS_COMM( ); 
  sendStringByDMA_USART (GPRS_COMM, "AT+CSQ\r\n");
  do{
    if(pdPASS != xQueueReceive(GprsQueue, &InputBuff, _1s * 2))
    { return ACK;  }   //Ӧ��ʱ
    else //��Ӧ�� 
    {
      GprsData_length = GetDataFromUsart (InputBuff,GprsRecveBuff, GPRS_GprsBuffLength);
      GprsRecveBuff[GprsData_length] = '\0'; 
      //if(0 == Display_func(GprsRecveBuff))
      {
        seat = 0;    
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"AT+CSQ",&itemp))
        {
          seat += itemp + 6;
          ACK = RecOK;
        }
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"+CSQ:",&itemp))
        {
          seat += itemp +4; 
          rssi = 0;
          ber = 0;
          SeekIsDigit (&GprsRecveBuff[seat],  &itemp);
          seat += itemp;
          while((itemp < GprsData_length)
                &&(IsDigit(GprsRecveBuff[seat])))
          {
            rssi *= 10;
            rssi +=  GprsRecveBuff[seat++] - '0';
          }
          SeekIsDigit (&GprsRecveBuff[seat],  &itemp);
          seat += itemp;
          while((itemp < GprsData_length)
                &&(IsDigit(GprsRecveBuff[seat])))
          {
            ber *= 10;
            ber +=  GprsRecveBuff[seat++] - '0';
          }
          if(0 != SeekMarkString(&GprsRecveBuff[seat],"OK",&itemp))
          {
            ACK = ExeOK;
            gprs.Ber = ber;
            gprs.Rssi = rssi; 
          } 
          else
          {ACK = ExeErr;}
        }
        else if(0 != SeekMarkString(&GprsRecveBuff[seat],"ERROR",&itemp))
        {
          seat += itemp + 5;
          ACK = ExeErr;
        }   
        if((ExeOK == ACK)||(ExeErr == ACK))
        {return ACK;}
      } 
    }
  }while (1);
}
/*********************************************************************************
Function:      //
Description:   // �������ע��״̬   
Input:         //
//
Output:        //
Return:        //
Others:        //AT+CREG?  +CREG:0,1  OK
*********************************************************************************/ 
Cmd_ACK_type  _CREG_func(unsigned char *mode, unsigned char* state)
{
  Cmd_ACK_type   ACK = RecErr;
  unsigned long itemp;
  unsigned long seat;
  
  weitGPRS_COMM( ); 
  sendStringByDMA_USART (GPRS_COMM, "AT+CREG?\r\n");
  do{
    if(pdPASS != xQueueReceive(GprsQueue, &InputBuff, _1s * 2))
    { return ACK;  }   //Ӧ��ʱ
    else //��Ӧ�� 
    {
      GprsData_length = GetDataFromUsart (InputBuff,GprsRecveBuff, GPRS_GprsBuffLength);
      GprsRecveBuff[GprsData_length] = '\0'; 
      //if(0 == Display_func(GprsRecveBuff))
      {
        seat = 0;    
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"AT+CREG?",&itemp))
        {
          seat += 8 + itemp;
          ACK = RecOK;
        }
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"CREG:",&itemp))
        {
          seat += 5 + itemp; 
          *mode = 0;
          *state = 0;
          SeekIsDigit (&GprsRecveBuff[seat],&itemp);
          seat += itemp;
          while((itemp < GprsData_length)
                &&(IsDigit(GprsRecveBuff[seat])))
          {
            *mode *= 10;
            *mode +=  GprsRecveBuff[seat++] - '0';
          }
          SeekIsDigit (&GprsRecveBuff[seat],  &itemp);
          seat += itemp;
          while((itemp < GprsData_length)
                &&(IsDigit(GprsRecveBuff[seat])))
          {
            *state *= 10;
            *state +=  GprsRecveBuff[seat++] - '0';
          }
          if(0 != SeekMarkString(&GprsRecveBuff[seat],"OK",&itemp))
          {ACK = ExeOK;} 
          else
          {ACK = ExeErr;}
        }
        else if(0 != SeekMarkString(&GprsRecveBuff[seat],"ERROR",&itemp))
        {
          seat += 5 + itemp;
          ACK = ExeErr;
        }   
        if((ExeOK == ACK)||(ExeErr == ACK))
        {return ACK;}
      }
    }
  }while (1);
}
/*********************************************************************************
Function:      //
Description:   // ���GPRS����״̬    
Input:         //
//
Output:        //
Return:        //
Others:        // AT+CGATT   +CGATT:1   OK
*********************************************************************************/ 
Cmd_ACK_type  _CGATT_func(unsigned char* state)
{
  Cmd_ACK_type   ACK = RecErr;
  unsigned long itemp;
  unsigned long seat;
  
  weitGPRS_COMM( ); 
  sendStringByDMA_USART (GPRS_COMM, "AT+CGATT\r\n");
  do{
    if(pdPASS != xQueueReceive(GprsQueue, &InputBuff, _1s * 2))
    { return ACK;  }   //Ӧ��ʱ
    else //��Ӧ�� 
    {
      GprsData_length = GetDataFromUsart (InputBuff,GprsRecveBuff, GPRS_GprsBuffLength);
      GprsRecveBuff[GprsData_length] = '\0'; 
      //if(0 == Display_func(GprsRecveBuff))
      {
        seat = 0;    
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"AT+CGATT",&itemp))
        {
          seat += 8 + itemp;
          ACK = RecOK;
        }
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"CGATT:",&itemp))
        {
          seat +=  6 + itemp; 
          *state = 0;
          SeekIsDigit (&GprsRecveBuff[seat],&itemp);
          seat += itemp;
          while((itemp < GprsData_length)
                &&(IsDigit(GprsRecveBuff[seat])))
          {
            *state *= 10;
            *state +=  GprsRecveBuff[seat++] - '0';
          }
          if(0 != SeekMarkString(&GprsRecveBuff[seat],"OK",&itemp))
          {ACK = ExeOK;} 
          else
          {ACK = ExeOK;}
        }
        else if(0 != SeekMarkString(&GprsRecveBuff[seat],"ERROR",&itemp))
        {
          seat += itemp;
          ACK = ExeErr;
        }      
        if((ExeOK == ACK)||(ExeErr == ACK))
        {return ACK;}
      }
    }
  }while (1);
}
/*********************************************************************************
Function:      //
Description:   // ����APN    
Input:         //
               //
Output:        //
Return:        //
Others:        //  AT+CSTT=CMNET    OK
*********************************************************************************/ 
Cmd_ACK_type  _CSTT_func( char *apn)
{
  Cmd_ACK_type   ACK = RecErr;
  unsigned long itemp ;
  unsigned long seat ;
  
  weitGPRS_COMM( );
  
  GprsSendBuff[0] ='\0';
  strcat((char*)GprsSendBuff,"AT+CSTT=");
  strcat((char*)GprsSendBuff,apn);
  strcat((char*)GprsSendBuff, "\r\n");

  sendStringByDMA_USART (GPRS_COMM, GprsSendBuff);

  do{
    if(pdPASS != xQueueReceive(GprsQueue, &InputBuff, _1s * 2))
    { return ACK;  }   //Ӧ��ʱ
    else //��Ӧ�� 
    {
      GprsData_length = GetDataFromUsart (InputBuff,GprsRecveBuff, GPRS_GprsBuffLength);
      GprsRecveBuff[GprsData_length] = '\0';
      //if(0 == Display_func(GprsRecveBuff))
      {
        seat = 0;    
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"AT+CSTT=",&itemp))
        {
          seat +=  8 + itemp;
          ACK = RecOK;
        }
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"OK",&itemp))
        {
          seat += itemp; 
          ACK = ExeOK; 
        }
        else if(0 != SeekMarkString(&GprsRecveBuff[seat],"ERROR",&itemp))
        {
          seat += itemp;
          ACK = ExeErr;
        }   
        if((ExeOK == ACK)||(ExeErr == ACK))
        {return ACK;}
      }
    }
  }while (1);
}
/*********************************************************************************
Function:      //
Description:   // ����������·��GPRS����CSD��   
Input:         //
//
Output:        //
Return:        //
Others:        // "  AT+CIICR     ////OK
*********************************************************************************/ 
Cmd_ACK_type  _CIICR_func(void)
{ 
  Cmd_ACK_type   ACK = RecErr;
  unsigned long itemp ;
  unsigned long seat ;
  
  weitGPRS_COMM( );  
  sendStringByDMA_USART (GPRS_COMM, "AT+CIICR\r\n");
  
  do{
    if(pdPASS != xQueueReceive(GprsQueue, &InputBuff, _1s * 6))
    { return ACK;  }   //Ӧ��ʱ
    else //��Ӧ�� 
    {
      GprsData_length = GetDataFromUsart (InputBuff,GprsRecveBuff, GPRS_GprsBuffLength);
      GprsRecveBuff[GprsData_length] = '\0';
      //if(0 == Display_func(GprsRecveBuff))
      {
        seat = 0;    
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"AT+CIICR",&itemp))
        {
          seat += itemp;
          ACK = RecOK;
        }
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"OK",&itemp))
        {
          seat += itemp; 
          ACK = ExeOK; 
        }
        else if(0 != SeekMarkString(&GprsRecveBuff[seat],"ERROR",&itemp))
        {
          seat += itemp;
          ACK = ExeErr;
        }   
        if((ExeOK == ACK)||(ExeErr == ACK))
        {return ACK;}
      }
    }
  }while (1);
}
/*********************************************************************************
Function:      //
Description:   // ���ö���·ģʽ 
Input:         //
//
Output:        //
Return:        //
Others:        // AT+CIPMUX=1  ����·ģʽ         
                  AT+CIPMUX=0  ����·ģʽ
*********************************************************************************/ 
Cmd_ACK_type  _CIPMUX_func(BaseType_t MUX)
{
 Cmd_ACK_type   ACK = RecErr;
 unsigned long itemp ;
 unsigned long seat ;
  
  weitGPRS_COMM( );  
  if(0 != MUX)
  {sendStringByDMA_USART (GPRS_COMM, "AT+CIPMUX=1\r\n");}
  else
  {sendStringByDMA_USART (GPRS_COMM, "AT+CIPMUX=0\r\n"); }
  
 do{
    if(pdPASS != xQueueReceive(GprsQueue, &InputBuff, _1s * 3))
    { return ACK;  }   //Ӧ��ʱ
    else //��Ӧ�� 
    {
      GprsData_length = GetDataFromUsart (InputBuff,GprsRecveBuff, GPRS_GprsBuffLength);
      GprsRecveBuff[GprsData_length] = '\0';
      //if(0 == Display_func(GprsRecveBuff))
      {
        seat = 0;    
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"AT+CIPMUX=",&itemp))
        {
          seat += itemp;
          ACK = RecOK;
        }
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"OK",&itemp))
        {
          seat += itemp; 
          ACK = ExeOK; 
        }
        else if(0 != SeekMarkString(&GprsRecveBuff[seat],"ERROR",&itemp))
        {
          seat += itemp;
          ACK = ExeErr;
        }   
        if((ExeOK == ACK)||(ExeErr == ACK))
        {return ACK;}
      }
    }
  }while (1);
}
/*********************************************************************************
Function:      //
Description:   // ����͸��ģʽ 
Input:         //
//
Output:        //
Return:        //
Others:        // AT+CIPMODE=1  ͸��ģʽ        
AT+CIPMODE=0  ��͸��ģʽ
*********************************************************************************/ 
Cmd_ACK_type  _CIPMODE_func(BaseType_t mode)
{
  Cmd_ACK_type   ACK = RecErr;
  
  return ACK;
  
  /*if(pdTRUE == mode)
  {
    if(pdPASS != _CIPMUX_func(pdFALSE))
    {return pdPASS;}
  }
  
  weitGPRS_COMM( ); 
  if(pdTRUE == mode)
  {sendStringByDMA_USART (GPRS_COMM, "AT+CIPMODE=1\r\n");}
  else
  {sendStringByDMA_USART (GPRS_COMM, "AT+CIPMODE=0\r\n"); }
  
  if(pdPASS != xQueueReceive(GprsQueue, &InputBuff, _1s))
  { re = pdFAIL;}  //Ӧ��ʱ�ط�
  else //��Ӧ��           
  {
    //��ȡӦ������
    GprsData_length = GetDataFromUsart (InputBuff,Gprs_buff, GPRS_GprsBuffLength);
    if(GprsData_length > 0)
    {
      //������ ����
    }
  }
  return re;*/
}
/*********************************************************************************
Function:      //
Description:   // ��ȡ���� IP ��ַ   
Input:         //
//
Output:        //
Return:        //
Others:        // AT+CIFSR 
*********************************************************************************/ 
Cmd_ACK_type  _CIFSR_func(unsigned long* IP)
{
  Cmd_ACK_type   ACK = RecErr;
  unsigned long itemp ;
  unsigned long seat ;
  
  weitGPRS_COMM( );    
  sendStringByDMA_USART (GPRS_COMM, "AT+CIFSR\r\n");
  
  do{
    if(pdPASS != xQueueReceive(GprsQueue, &InputBuff, _1s * 2))
    { return ACK;  }   //Ӧ��ʱ
    else //��Ӧ�� 
    {
      GprsData_length = GetDataFromUsart (InputBuff,GprsRecveBuff, GPRS_GprsBuffLength);
      GprsRecveBuff[GprsData_length] = '\0';
      //if(0 == Display_func(GprsRecveBuff))
      {
        seat = 0;    
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"AT+CIFSR",&itemp))
        {
          seat += itemp;
          ACK = RecOK;
          if(0 != SeekIsDigit (&GprsRecveBuff[seat],&itemp))
          {
            seat += itemp;
            if(0 != StringIPtoU32(&GprsRecveBuff[seat], IP))
            {ACK = ExeOK; }
            else
            {ACK = ExeErr;}
          }  
        }
        else if(0 != SeekMarkString(&GprsRecveBuff[seat],"ERROR",&itemp))
        {
          seat += itemp;
          ACK = ExeErr;
        }   
        if((ExeOK == ACK)||(ExeErr == ACK))
        {return ACK;}
      }
    }
  }while (1);
}
/*********************************************************************************
Function:      //
Description:   // ����TCP���� 
Input:         //
               //
Output:        //
Return:        //
Others:        // AT+CIPSTART=��TCP������116.228.221.51","8500"     OK CONNECT OK
*********************************************************************************/ 
Cmd_ACK_type _CIPSTART_func(unsigned char ch, unsigned long ip, unsigned short port, CIPSTART_ACK_type* ack)
{
  Cmd_ACK_type   ACK = RecErr; 
  unsigned long itemp ;
  unsigned long seat ;
  unsigned char *p;
  
  if(ch >4){return RecErr;}
  
  GprsSendBuff[0] ='\0';
  p = AddString(GprsSendBuff, "AT+CIPSTART=");
  *p++ ='0' + ch;
  *p = '\0';
  p = AddString(GprsSendBuff, ",\"TCP\", \"");
  p = IP32ToString( ip, p);
  *p++ = '\"';
  *p++ = ',';
  *p++ = '\"';
  GprsSendByteNumber = p -  GprsSendBuff;
  GprsSendByteNumber += IntToString(port, &(GprsSendBuff[GprsSendByteNumber]));
  GprsSendBuff[GprsSendByteNumber++] = '\"';
  GprsSendBuff[GprsSendByteNumber++] = '\r';
  GprsSendBuff[GprsSendByteNumber++] = '\n';

  weitGPRS_COMM( );  
  sendBuffByDMA_USART (GPRS_COMM, GprsSendBuff, GprsSendByteNumber, P_NULL, P_NULL);
  
  do{
    if(pdPASS != xQueueReceive(GprsQueue, &InputBuff, _1s * 120))
    { return ACK;  }   //Ӧ��ʱ
    else //��Ӧ�� 
    {
      GprsData_length = GetDataFromUsart (InputBuff,GprsRecveBuff, GPRS_GprsBuffLength);
      GprsRecveBuff[GprsData_length] = '\0';
      //Display_func(GprsRecveBuff);
      {
        seat = 0;    
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"AT+CIPSTART=\"TCP\"",&itemp))
        {
          seat += itemp;
          ACK = RecOK;  
        }
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"CONNECT OK",&itemp))
        {
          if(0 != SeekIsDigit (&GprsRecveBuff[seat],&itemp))
          {
            if(GprsRecveBuff[itemp]== ch + '0')
            {
              ack->state = CONNECT_OK;
              gprs.cip[ch].ip = ip;
              gprs.cip[ch].prot = port;
              gprs.cip[ch].CSR.mode = 0x01;
              gprs.cip[ch].CSR.state = 0x01;
              ACK = ExeOK;
            }
            else
            {ACK = ExeErr;}
          }
        }
        else if(0 != SeekMarkString(&GprsRecveBuff[seat],"CONNECT FAIL",&itemp))
        {
          if(0 != SeekIsDigit (&GprsRecveBuff[seat],&itemp))
          {
            if(GprsRecveBuff[itemp]== ch + '0')
            {
              ack->state = CONNECT_FAIL;
              ACK = ExeOK;
            }
            else
            {ACK = ExeErr;}
          }
        }
        else if(0 != SeekMarkString(&GprsRecveBuff[seat],"ALREADY CONNECT",&itemp))
        {
          if(0 != SeekIsDigit (&GprsRecveBuff[seat],&itemp))
          {
            if(GprsRecveBuff[itemp]== ch + '0')
            {
              ack->state = ALREADY_CONNECT;
              ACK = ExeOK;
            }
            else
            {ACK = ExeErr;}
          }
        }
        else if(0 != SeekMarkString(&GprsRecveBuff[seat],"+CME ERROR",&itemp))
        {
          ACK = ExeOK;
          ack->state = CME_ERROR;
          seat += itemp;  
          if(0 != SeekIsDigit (&GprsRecveBuff[seat],&itemp))
          {
           seat += itemp;
           ack->ERROR_CODE = StingToBin (&GprsRecveBuff[seat]);
          }
        }
        else if(0 != SeekMarkString(&GprsRecveBuff[seat],"ERROR",&itemp))
        {ACK = ExeErr;}   
        if((ExeOK == ACK)||(ExeErr == ACK))
        {return ACK;}
      }
    }
  }while (1);  
}
/*********************************************************************************
Function:      //
Description:   // �������ݵ�Զ�˷����� 
Input:         //
//
Output:        //
Return:        //
Others:        // AT+CIPSEND            >   data        SEND OK
*********************************************************************************/ 
Cmd_ACK_type  _CIPSEND_func(unsigned char ch, unsigned char* pbuff, unsigned short length)
{
  Cmd_ACK_type   ACK = RecErr; 
  unsigned long itemp ;
  unsigned long seat ;
  unsigned char *p;
  unsigned char flg = 0;
  
  if(ch >6)
  {return RecErr;}
  weitGPRS_COMM( );
  GprsSendBuff[0] ='\0';
  p = AddString(GprsSendBuff, "AT+CIPSEND=");
  *p++ ='0' + ch;
  *p++ =',';
  p = BinToSting (length,p);
  *p++ = '\r';
  *p++ = '\n';
  *p = '\0'; 
  sendStringByDMA_USART (GPRS_COMM, GprsSendBuff);
  
  do{
    if(pdPASS != xQueueReceive(GprsQueue, &InputBuff, _1s * 5))
    { 
      //Ӧ��ʱ
      if(RecErr == ACK)
      { return ACK; }
      else if(0 == flg)
      {
        weitGPRS_COMM( );
        sendBuffByDMA_USART (GPRS_COMM, pbuff, length, P_NULL, P_NULL);
        flg = 1;
      }
      else
      {return ACK;}
    }   
    else //��Ӧ�� 
    {
      GprsData_length = GetDataFromUsart (InputBuff,GprsRecveBuff, GPRS_GprsBuffLength);
      GprsRecveBuff[GprsData_length] = '\0';
      //Display_func(GprsRecveBuff);
      //if(0 == Display_func(GprsRecveBuff))
        seat = 0;    
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"AT+CIPSEND=",&itemp))
        {
          seat += itemp;
          ACK = RecOK;  
        }
        if(0 != SeekMarkString(&GprsRecveBuff[seat],">",&itemp))   //���Է�������
        {
          ACK = RecOK;
          weitGPRS_COMM( );
          sendBuffByDMA_USART (GPRS_COMM, pbuff, length, P_NULL, P_NULL);
          flg = 1;
        }  
        else if(0 != SeekMarkString(&GprsRecveBuff[seat],"SEND OK",&itemp)) //��ͨģʽ���ݷ��ͳɹ�
        {
          if(0 != SeekIsDigit (&GprsRecveBuff[seat],&itemp))
          {
            if(GprsRecveBuff[itemp]== ch + '0')
            {ACK = ExeOK;}
            else
            {ACK = ExeErr;}
          }
        }
        else if(0 != SeekMarkString(&GprsRecveBuff[seat],"SEND FAIL",&itemp))  //���ݷ���ʧ�� 
        {ACK = ExeErr;}
        else if(0 != SeekMarkString(&GprsRecveBuff[seat],"DATA ACCEPT",&itemp)) //���ٷ���ģʽ���Լ�����������
        {
          seat += itemp;
          if(0 != SeekIsDigit (&GprsRecveBuff[seat],&itemp))
          {
            if(GprsRecveBuff[itemp]== ch + '0')
            {ACK = ExeOK;}
            else
            {ACK = ExeErr;}
          }
        }
        else if(0 != SeekMarkString(&GprsRecveBuff[seat],"+CME ERROR",&itemp))  //
        {
          ACK = ExeOK;
          seat += itemp;   
        }
        else if(0 != SeekMarkString(&GprsRecveBuff[seat],"ERROR",&itemp))
        {ACK = ExeErr;}   
        if((ExeOK == ACK)||(ExeErr == ACK))
        {return ACK;}
    }
  }while (1);  
}
/*********************************************************************************
Function:      //
Description:   // �ر�Զ������ 
Input:         //
//
Output:        //
Return:        //
Others:        // 
*********************************************************************************/ 
Cmd_ACK_type  _CIPCLOSE_func(unsigned char ch)
{
  Cmd_ACK_type   ACK = RecErr; 
  unsigned long itemp ;
  unsigned long seat ;
  
  if(ch >6)
  {return RecErr;}
  
  sprintf((char *)GprsSendBuff, "AT+CIPCLOSE=%d\r\n",ch);
  weitGPRS_COMM( );  
  sendStringByDMA_USART (GPRS_COMM, GprsSendBuff);
  
  do{
    if(pdPASS != xQueueReceive(GprsQueue, &InputBuff, _1s * 40))
    { 
      if(RecOK == ACK)
      {
        GPRS_LinkCloseFunc ( ch );
        ACK = ExeOK;
      }
      return ACK;  
    }   //Ӧ��ʱ
    else //��Ӧ�� 
    {
      GprsData_length = GetDataFromUsart (InputBuff,GprsRecveBuff, GPRS_GprsBuffLength);
      GprsRecveBuff[GprsData_length] = '\0';
      //if(0 == Display_func(GprsRecveBuff))
      {
        seat = 0;    
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"AT+CIPCLOSE=",&itemp))
        {
          seat += itemp + strlen("AT+CIPCLOSE=");
          if(GprsRecveBuff[seat] >= '0')
          {
           itemp = GprsRecveBuff[seat] - '0';
           if(itemp == ch)
           {ACK = RecOK;  }
          }
        }
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"CLOSE OK",&itemp))
        {
          if(0 != SeekIsDigit (&GprsRecveBuff[seat],&itemp))
          {
            if(GprsRecveBuff[itemp]== ch + '0')
            {
              GPRS_LinkCloseFunc ( ch );
              ACK = ExeOK;
            }
            else
            {ACK = ExeErr;}
          }
        }
        if(0 != SeekMarkString(&GprsRecveBuff[seat],"ERROR",&itemp))
        {ACK = ExeErr;}   
        if((ExeOK == ACK)||(ExeErr == ACK))
        {return ACK;}
      }
    }
  }while (1);  
}
/*********************************************************************************
Function:      //
Description:   //ͬ��ͨ�Ŵ��ڱ�����
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/ 
Cmd_ACK_type  _SyncRate_func(void)
{
  unsigned long cnt  = 0;
  unsigned long itemp;
  UsarBuff_Struct* data;
  do{
    cnt ++;
    sendStringByDMA_USART (USART3_Port,"AT");   
    if(pdPASS == xQueueReceive(GprsQueue, &data, _100ms))
    {
      GprsData_length = GetDataFromUsart (data, GprsRecveBuff, GprsData_length);
      itemp = 0;
      while (itemp < GprsData_length)
      {
        if('A' == GprsRecveBuff[itemp++])
        {
         if('T' == GprsRecveBuff[itemp]) 
         { return RecOK;}
        }
      }
    }  
  }while(cnt < 100);
  return ExeErr;  
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
static void GPRS_LinkCloseFunc(unsigned char CH)
{
  CommPortNumber_Type port = GPRS_CH1;
  port += CH; 
  
  gprs.cip[CH].ip = 0;
  gprs.cip[CH].prot = 0;
  gprs.cip[CH].CSR.mode = 0;
  gprs.cip[CH].CSR.state = 0;
  if(port == ServerState[0].Port)
  {
    ServerState[0].Port = Null_Port;
    ServerState[0].SR.Bit.CN = 0;
    ServerState[0].Connect_CNT = 0;
  }
  if(port == ServerState[1].Port)
  {
    ServerState[1].Port = Null_Port;
    ServerState[1].SR.Bit.CN = 0;
    ServerState[1].Connect_CNT = 0;
  }
}
/*********************************************************************************
Function:      //
Description:   //ָʾ��䴦��
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/ 
static void GPRS_ReceiveFunc(unsigned char CH, char* pbuff, unsigned long length)
{
  CommPortNumber_Type port = GPRS_CH1;
  port += CH; 
  if(port == ServerState[0].Port)
  {WriteDtatToServerRecBuff(0,pbuff,length);}
   if(port == ServerState[1].Port)
  {WriteDtatToServerRecBuff(1,pbuff,length);}
}
/*********************************************************************************
Function:      //
Description:   //ָʾ��䴦��
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/ 
void GprsDisplay_func(void* buff )
{
  unsigned char itemp1;
  unsigned long itemp2 ;
  char *p1;
  char *p2;
  
  p1 = strstr(buff, "\r\n+RECEIVE,");
  if(NULL != p1)
  {
    p2 = p1;  
    p1 = strchr(p1,',');
    if(NULL != p1)
    {
      p1++;
      itemp1 = atoi(p1);
      p1 = strchr(p1,',');
      if(NULL != p1)
      {
        p1++;
        itemp2 = atoi(p1);
        if(0 != itemp2)
        {
          p1 = strchr(p1,':');
          p1 += 3;
          if(NULL != p1)
          {
            GPRS_ReceiveFunc (itemp1, p1, itemp2);   //�������ݴ��� 
            itemp2 += p1- p2 ;
            StringDel(p2,itemp2);
          } 
        }
      }
    }  
  }
  p1 = strstr(buff, ", CLOSED\r\n");
  if(NULL != p1)   
  {
    while(--p1 >= buff) 
    {
      if(IsDigit(*p1))
      {
        itemp1 = *p1 - '0'; 
        GPRS_LinkCloseFunc ( itemp1 );
        StringDel(p1,strlen(", CLOSED\r\n")+3);
        break; 
      }
    }
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
static void PortInit( void )
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  GPRSPower_OFF();
  clr_GPRS_REST();
  clr_GPRS_ONOFF(); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_7 | GPIO_Pin_6;	     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  
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
unsigned char getGprs_IdleCIP( void )
{
  unsigned char itemp = 0;
  while(itemp < 6)
  {
    if(0 == gprs.cip[itemp].CSR.state)
    {return itemp;}
    else
    {itemp ++;}
  }
  return 255;
}
/*********************************************************************************
Function:      //
Description:   // ���ӷ�����
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/ 
unsigned char connectServer (unsigned char ServerNO)
{
  unsigned char itemp;
  unsigned short Port;
  unsigned long ip;
  CIPSTART_ACK_type ack;
  if((0 != ServerNO)&&(1 != ServerNO))
  {return 0;}
  //����TCP����
  itemp = 0;
  ip = 0;
  while (itemp < 4)
  {
    ip <<= 8;
    ip |= _188_ConcenConfigData.gprs.IP[itemp];
    itemp ++;
  }
  Port = _188_ConcenConfigData.gprs.Port.Byte.Byte_H;
  Port <<= 8;
  Port += _188_ConcenConfigData.gprs.Port.Byte.Byte_L;
  
  if((0 == ip)||(0 == Port))
  {return 0;}

  if(pdTRUE == xSemaphoreTake( Gprs_xMutex, 0 ))
  { 
    itemp = getGprs_IdleCIP( );
    _CIPSTART_func(itemp,ip, Port, &ack);  //����TCP����
   if(CONNECT_OK  == ack.state){
      ServerState[ServerNO].Port = GPRS_CH1;
      ServerState[ServerNO].Port += itemp;
      ServerState[ServerNO].SR.Bit.CN = 1;
      ServerState[ServerNO].SR.Bit.GZ = 0;
      xSemaphoreGive( Gprs_xMutex); 
      return 1;
    }
    else{ 
      _CIPCLOSE_func(itemp);
      xSemaphoreGive( Gprs_xMutex); 
      return 0;
    }
  }
  return 0;
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
void closeServer (unsigned char ServerNO)
{
  if((GPRS_CH1 == ServerState[ServerNO].Port)
     ||(GPRS_CH2 == ServerState[ServerNO].Port)
       ||(GPRS_CH3 == ServerState[ServerNO].Port)
         ||(GPRS_CH4 == ServerState[ServerNO].Port)
           ||(GPRS_CH5 == ServerState[ServerNO].Port))
  {
    if(pdTRUE == xSemaphoreTake( Gprs_xMutex, _1s * 10 ))
    {
      _CIPCLOSE_func(ServerState[ServerNO].Port - GPRS_CH1); 
      ServerState[ServerNO].SR.Bit.CN =  0x00;
      ServerState[ServerNO].Port = Null_Port;
      ServerState[ServerNO].Connect_CNT = 0;
      xSemaphoreGive( Gprs_xMutex);  
    }
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
unsigned char get_SIM_ID (char* Text, char* number)
{
  Cmd_ACK_type ACK;
  unsigned char index;
  char type;
  if(pdTRUE != xSemaphoreTake( Gprs_xMutex, _1s * 2 ))
  {
    ACK = _CPBS_func( );
    if(ExeOK == ACK)
    {ACK = _CPBF_FormText(Text,&index, &type, number);}
    xSemaphoreGive( Gprs_xMutex);  
   if(ExeOK == ACK)
   {return 1;}
  }
  return 0;
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
static unsigned char  ModeInitConfig ( void )
{
  unsigned char  rssi; 
  unsigned char  ber;
  unsigned char itemp = 0;
  if(RecOK != _SyncRate_func( ))
  {return 0;}
  vTaskDelay(_1s *1);
  //��������ź�ǿ��
  itemp = 0;
  while (itemp++ < 30)
  {
    if(ExeOK == _CSQ_func( ))
    {break;}
    else
    {vTaskDelay(_100ms *5);}
    if(itemp >  10)
    {return 0;}
  }
  vTaskDelay(_1s *1);
  //�������ע��״̬ 
  itemp = 0;
  while (itemp++ < 30)
  {
    if(ExeOK == _CREG_func(&rssi,&ber))
    {break;}
    else
    {vTaskDelay(_100ms *5);}
    if(itemp >  10)
    {return 0;}
  }
  vTaskDelay(_1s *1);
   //���ö���·ģʽ 
  itemp = 0;
  while (itemp++ < 30)
  {
    if(ExeOK == _CIPMUX_func(1))
    {break;}
    else
    {vTaskDelay(_100ms *5);}
    if(itemp >  10)
    {return 0;}
  }
  vTaskDelay(_1s *1);
  //����APN
  if( 0 != _188_ConcenConfigData.gprs.ConnectionMode.Bit.D1)
  {
    itemp = 0;
    while (itemp++ < 30)
    {
      if(ExeOK == _CSTT_func(_188_ConcenConfigData.gprs.APN))
      {break;}
      else
      {vTaskDelay(_100ms *5);}
      if(itemp >  10)
      {return 0;}
    }
  }
  vTaskDelay(_1s * 1);
  //����������·��GPRS����CSD��
  itemp = 0;
  while (itemp++ < 30)
  {
    if(ExeOK == _CIICR_func( ))
    {break;}
    else
    {vTaskDelay(_1s * 1);}
    if(itemp >  10)
    {return 0;}
  }
   vTaskDelay(_1s * 1);
  //��ȡ���� IP ��ַ
  itemp = 0;
  while (itemp++ < 30)
  {
    if(ExeOK == _CIFSR_func(&(gprs.localip)))
    {break;}
    else
    {vTaskDelay(_100ms *5);}
    if(itemp >  10)
    {return 0;}
  }
  
  /*itemp = 0;
  while (itemp++ < 3)
  {
    if(0 != get_SIM_ID ("zss", gprs.ID))
    {break;}
    else
    {vTaskDelay(_100ms *5);}
    if(itemp >  10)
    {return 0;}
  }*/
  return 1;
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
void OFF_GPRS ( void )
{
  unsigned char *p;
  unsigned char itemp =0;
  p = (unsigned char *) &gprs;
  //gprs.CSR.Bit.ready = 0;
  while (itemp < 6)
  {
    if(0 != gprs.cip[itemp].CSR.state)
    { _CIPCLOSE_func(itemp);}
    itemp ++;
  }
  itemp = 0;
  while (itemp < sizeof( GPRS_MODE_struct ))
  {
   p[itemp++] = 0x00; 
  }
  ServerState[0].SR.Bit.ACK = 0;
  ServerState[0].SR.Bit.CN = 0;
  ServerState[0].SR.Bit.GZ = 0;
  ServerState[0].Connect_CNT = 0;
  ServerState[0].Port = Null_Port;
  
  ServerState[1].SR.Bit.ACK = 0;
  ServerState[1].SR.Bit.CN = 0;
  ServerState[1].SR.Bit.GZ = 0;
  ServerState[1].Connect_CNT = 0;
  ServerState[1].Port = Null_Port;
  GPRSPower_OFF(); //�ر�GPRS��Դ  
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
unsigned char  Launch_GPRS ( void )
{
  static unsigned char  Launch_CNT = 1;
  GLED_OFF();
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, DISABLE); 
  OFF_GPRS(); //�ر�GPRS��Դ  
  vTaskDelay(_1s * 30 );
  PortInit( );
  clr_GPRS_ONOFF();
  GPRSPower_ON(); //��Gprs��Դ
  vTaskDelay(_1s *1);
  set_GPRS_ONOFF();   
  vTaskDelay(_100ms  * 13);
  clr_GPRS_ONOFF();   //��ɿ���
  gprs.CSR.Bit.power = 1;
  vTaskDelay(_1s * 20); 
  Launch_USART(GPRS_COMM,38400, Parity_No, StopBits_2, ByDMA);  //�򿪿��ƶ˿�;
  GLED_ON();
  if( 0 != ModeInitConfig ( ))
  {
    gprs.CSR.Bit.ready = 1;
    return 1;
  }
  else
  {
    GLED_OFF();
    if(Launch_CNT ++ > 10)
    {GprsTask_MonitorTimer   =  0xffff;}  //ϵͳ��λ
    gprs.CSR.Bit.ready = 0;
    return 0;
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
void GprsTask_func ( void* Param )
{
  while (1)
  {
    if(0 == gprs.CSR.Bit.ready)
    {Launch_GPRS( );} 
    else if((1 == ServerState[0].SR.Bit.EN)&&(1 == ServerState[1].SR.Bit.EN))
    {
      if((1 == ServerState[0].SR.Bit.GZ)&&(1 == ServerState[1].SR.Bit.GZ))
      {Launch_GPRS( );}
    }
    else if((1 == ServerState[0].SR.Bit.EN)&&(0 == ServerState[1].SR.Bit.EN))
    {
      if(1 == ServerState[0].SR.Bit.GZ)
      {Launch_GPRS( );}
    }
    else if((0 == ServerState[0].SR.Bit.EN)&&(1 == ServerState[1].SR.Bit.EN))
    {
      if(1 == ServerState[1].SR.Bit.GZ)
      {Launch_GPRS( );}
    }
    else
    { OFF_GPRS();} 
    
    if(pdPASS == xQueueReceive(GprsQueue, &InputBuff, _1s * 2 ))
    { 
      GprsData_length = GetDataFromUsart (InputBuff,GprsRecveBuff, GPRS_GprsBuffLength);
    }
    GprsTask_MonitorTimer = 0;   //Gprsͨ�� ������ӿ��Ź���ʱ�� ��λ
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
SystemErrName GprsIP_SendFunc(unsigned char CH, void* Data,unsigned short DataLength) 
{
  Cmd_ACK_type   state;
  
  if(0 == gprs.cip[CH].CSR.state)
  {return Port_OFF;}
  if(pdTRUE == xSemaphoreTake(Gprs_xMutex, _1s * 3 ))
  {
    state = _CIPSEND_func(CH, Data, DataLength);
    xSemaphoreGive( Gprs_xMutex);
    if(ExeOK == state)
    {return NO_ERR;}
    else
    {return SendErr;}
  }
  return PortBusy;
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
static unsigned char*  IP32ToString(unsigned long ip, unsigned char* string)
{
  unsigned char itemp;
  unsigned char itemp2;
  unsigned char  bcd[3];
  itemp =0;
  while (itemp++ < 4)
  {
    itemp2 = ip >>24;
    ip <<= 8;  
    
    bcd[0] = itemp2 % 10;
    itemp2 /= 10;
    bcd[1] = itemp2 % 10;
    itemp2 /= 10;
    bcd[2] = itemp2 % 10;
    if(0 != bcd[2])
    {
      *string ++ = bcd[2] + '0';
      *string ++ = bcd[1] + '0';
    }
    else
    {
     if(0 != bcd[1])
     {*string ++ = bcd[1] + '0';}
    }
     *string ++ = bcd[0] + '0';
     *string ++ = '.';
  }
  *(--string) = '\0';
  return string;
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
static unsigned char StringIPtoU32(unsigned char* String, unsigned long* ip)
{
  unsigned char itemp;
  unsigned char itemp2;
  unsigned char cnt = 0;
  unsigned char cnt2;
  
  *ip  = 0;
  itemp = 0;
  itemp2 = 0;
  while (cnt++ < 4)
  {
    cnt2 = 0;
    while (IsDigit(String[itemp]))
    {
      if(++cnt2> 3)
      {break;}
      itemp2 *= 10;
      itemp2 += String[itemp++] - '0';
     
    }
    if('.' == String[itemp++])
    {
     *ip <<= 8; 
     *ip += itemp2;
     itemp2 = 0;
    }
    else
    {break;}
  }
  if(cnt < 4)
  {return 0;}
  else
  {
   *ip <<= 8; 
   *ip += itemp2;
    return 1;
  }
}
/**************************************END****************************************/