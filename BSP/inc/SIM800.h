/*********************************************************************************
//������
//���ߣ����������     ʱ�䣺       �ص㣺
//CPU�ͺţ�         ϵͳ��Ƶ��
//�汾�ţ�V0.0        
*********************************************************************************/
#ifndef  __BSP_SIM800_H
#define  __BSP_SIM800_H 

#ifndef  IDLE 
#define  IDLE  0
#endif

#ifndef  INUSE
#define  INUSE  1
#endif
/*********************************************************************************
�ļ�������
*********************************************************************************/
#include "PowerConfig.h"
#include ".\SerialPort.h"
#include "..\inc\DriverPcb.h"
#include "..\..\LIB\Common\inc\SofTimer.h"
/*********************************************************************************
�궨����
*********************************************************************************/
#define  MG2618_PowerFlg              (GPRSPower_state)            
#define  MG2618_PowerON( )            GPRSPower_ON( )         
#define  MG2618_PowerOFF( )           GPRSPower_OFF( )

#define  GET_MG2616_ONOF_PIN         ~(GPRS_ONOFFpin) 
#define  SET_MG2616_ONOF_PIN          clr_GPRS_ONOFFpin
#define  RSET_MG2616_ONOF_PIN         set_GPRS_ONOFFpin      

#define  GET_MG2616_RSET_PIN         ~(GPRS_RESTpin)  
#define  SET_MG2616_RSET_PIN          clr_GPRS_RESTpin  
#define  RSET_MG2616_RSET_PIN         set_GPRS_RESTpin  

#define  ConfigUsart_SIM800(BaudRate,Parity,StopBits,mod)            Launch_USART2(BaudRate, Parity, StopBits, mod)                           
#define  GetDataForUsart_SIM800( )                                   GetDataForBuff_USART( )
#define  FreeUsartInputBuff_SIM800(buff)                             FreeBuff_USART(buff)
#define  SendDataToSIM800_ByIT(buff,length,callBack,callBack_arg)    sendBuffByIT_USART(USART3_Port,buff,length,callBack,callBack_arg )
#define  SendDataToSIM800_ByDMA(buff,length,callBack,callBack_arg)   sendBuffByDMA_USART(USART3_Port,buff,length,callBack,callBack_arg )
#define  SendStringToSIM800_ByIT(String)                               sendStringByIT_USART(USART3_Port, String)  
#define  SendStringToSIM800_ByDMA(String)                              sendStringByDMA_USART(USART3_Port, String) 
#define  SendDataToSIM800(buff,length,callBack)                      SendDataToSIM800_ByDMA(buff,length,callBack,P_NULL)
#define  SendStringToSIM800(String)                                  SendStringToSIM800_ByDMA(String)    


#define  A_                             /* Re-issues the Last Command Given */
#define  ATA                            /* Answer an Incoming Call. */
#define  ATD                            /* Mobile Originated Call to Dial A Number */
#define  ATD_N                          /* Originate Call to Phone Number in Current Memory */
#define  ATD_S                          /* Originate Call to Phone Number in Memory Which 
                                           Corresponds to */
#define  ATDL                           /* Redial Last Telephone Number Used */
#define  ATE                            /* Set Command Echo Mode  */
#define  ATH                            /* Disconnect Existing Connection */
#define  ATI                            /* Display Product Identification Information */ 
#define  ATL                            /* Set Monitor speaker loudnes */
#define  ATM                            /* Set Monitor Speaker Mode */
#define  C+++                            /* Switch from Data Mode or PPP Online Mode to Command Mode  */
#define  ATO                            /* Switch from Command Mode to Data Mode */
#define  ATP                            /* Select Pulse Dialling */
#define  ATQ                            /* Set Result Code Presentation Mode */
#define  ATS0                           /* Set Number of Rings before Automatically Answering the 
                                           Call */
#define  ATS3                           /* Set Command Line Termination Character */
#define  ATS4                           /* Set Response Formatting Character */ 
#define  ATS5                           /* Set Command Line Editing Character */
#define  ATS6                           /* Pause Before Blind Dialling */
#define  ATS7                           /* Set Number of Seconds to Wait for Connection Completion */
#define  ATS8                           /* Set Number of Seconds to Wait for Comma Dial Modifier 
                                           Encountered inDial String of D Command  */
#define  ATS10                          /* Set Disconnect Delay after Indicating the Absence of Data 
                                           Carrier */
#define  ATT                            /* Select Tone Dialing  */
#define  ATV                            /* TA Response Format  */
#define  ATX                            /* Set CONNECT Result Code Format and Monitor Call Progress */
#define  ATZ                            /* Reset Default Configuration */
#define  ATC                           /* Set DCD Function Mode  */
#define  AT&D                           /* Set DTR Function Mode */
#define  AT&F                           /* Factory Defined Configuration */
#define  AT&V                           /* Display Current Configuration */
#define  AT&W                           /* Store Active Profile  */
#define  AT+GCAP                        /* Request Complete TA Capabilities List */ 
#define  AT+GMI                         /* Request Manufacturer Identification */ 
#define  AT+GMM                         /* Request TA Model Identification  */
#define  AT+GMR                         /* Request TA Revision Identification of Software Release. */
#define  AT+GOI                         /* Request Global Object Identification. */
#define  AT+GSN                         /* Request TA Serial Number Identification (IMEI) */
#define  AT+ICF                         /* Set TE-TA Control Character Framing. */
#define  AT+IFC                         /* Set TE-TA Local Data Flow Control */
#define  AT+IPR                         /* Set TE-TA Fixed Local Rate  */
#define  AT+HVOIC                       /* Disconnect Voice Call Only */
                                        
#define  AT+CACM                        /* Accumulated Call Meter (ACM) Reset or Query */
#define  AT+CAMM                        /* Accumulated Call Meter Maximum (ACM max) Set or Query */ 
#define  AT+CAOC                        /* Advice of Charge */ 
#define  AT+CBST                        /* Select Bearer Service Type */
#define  AT+CCFC                        /* Call Forwarding Number and Conditions Control */
#define  AT+CCWA                        /* Call Waiting Control */ 
#define  AT+CEER                        /* Extended Error Report   */
#define  AT+CGMI                        /* Request Manufacturer Identification  */
#define  AT+CGMM                        /* Request Model Identification */
#define  AT+CGMR                        /* Request TA Revision Identification of Software Release */
#define  AT+CGSN                        /* Request Product Serial Number Identification 
                                          (Identical with +GSN) */
#define  AT+CSCS                        /* Select TE Character Set */
#define  AT+CSTA                        /* Select Type of Address */
#define  AT+CHLD                        /* Call Hold and Multiparty */
#define  AT+CIMI                        /* Request International Mobile Subscriber Identity */
#define  AT+CLCC                        /* List Current Calls of ME */
#define  AT+CLCK                        /* Facility Lock */
#define  AT+CLIP                        /* Calling Line Identification Presentation */
#define  AT+CLIR                        /* Calling Line Identification Restriction */
#define  AT+CMEE                        /* Report Mobile Equipment Error */
#define  AT+COLP                        /* Connected Line Identification Presentation  */
#define  AT+COPS                        /* Operator Selection */
#define  AT+CPAS                        /* Phone Activity Status */
#define  AT+CPBF                        /* Find Phonebook Entries */
#define  AT+CPBR                        /* Read Current Phonebook Entries */
#define  AT+CPBS                        /* Select Phonebook Memory Storage. */
#define  AT+CPBW                        /* Write Phonebook Entry. */
#define  AT+CPIN                        /* Enter PIN */
#define  AT+CPWD                        /* Change Password */
#define  AT+CR                          /* Service Reporting Control  */
#define  AT+CRC                         /* Set Cellular Result Codes for Incoming Call Indication  */
#define  AT+CREG                        /* Network Registration */
#define  AT+CRLP                        /* Select Radio Link Protocol Parameters  */
#define  AT+CRSM                        /* Restricted SIM Access  */
#define  AT+CSQ                         /* Signal Quality Report */
#define  AT+VTD                         /* Tone Duration */
#define  AT+VTS                         /* DTMF and Tone Generation */
#define  AT+CMUX                        /* Multiplexer Control */
#define  AT+CNUM                        /* Subscriber Number */
#define  AT+CPOL                        /* Preferred Operator List */
#define  AT+COPN                        /* Read Operator Names */
#define  AT+CFUN                        /* Set Phone Functionality */
#define  AT+CCLK                        /* Clock */
#define  AT+CSIM                        /* Generic SIM Access */
#define  AT+CALM                        /* Alert Sound Mode */
#define  AT+CALS                        /* Alert Sound Select */
#define  AT+CRSL                        /* Ringer Sound Level */
#define  AT+CLVL                        /* Loud Speaker Volume Level */
#define  AT+CMUT                        /* Mute Control. */
#define  AT+CPUC                        /* Price Per Unit and Currency Table */
#define  AT+CCWE                        /* Call Meter Maximum Event */
#define  AT+CBC                         /* Battery Charge */
#define  AT+CUSD                        /* Unstructured Supplementary Service Data */
#define  AT+CSSN                        /* Supplementary Services Notification */
                                        
#define  AT+CMGD                        /* Delete SMS Message */
#define  AT+CMGF                        /* Select SMS Message Format */
#define  AT+CMGL                        /* List SMS Messages from Preferred Store */
#define  AT+CMGR                        /* Read SMS Message */
#define  AT+CMGS                        /* Send SMS Message. */
#define  AT+CMGW                        /* Write SMS Message to Memory */
#define  AT+CMSS                        /* Send SMS Message from Storage  */
#define  AT+CNMI                        /* New SMS Message Indications  */
#define  AT+CPMS                        /* Preferred SMS Message Storage */
#define  AT+CRES                        /* Restore SMS Settings */
#define  AT+CSAS                        /* Save SMS Settings */ 
#define  AT+CSCA                        /* SMS Service Center Address */ 
#define  AT+CSCB                        /* Select Cell Broadcast SMS Messages   */
#define  AT+CSDH                        /* Show SMS Text Mode Parameters  */
#define  AT+CSMP                        /* Set SMS Text Mode Parameters  */
#define  AT+CSMS                        /* Select Message Service  */
                                        
#define  AT+STKTRS                      /* STK Terminal Response  */
#define  AT+STKENVS                     /* STK Envelope Command  */
#define  AT+STKCALL                     /* STK call setup  */
#define  AT+STKSMS                      /* STK SMS delivery  */
#define  AT+STKSS                       /* STK SS setup  */
#define  AT+STKUSSD                     /* STK USSD setup  */
#define  AT+STKDTMF                     /* STK sending DTMF  */
#define  +STKPCI                        /* STK Proactive Command Indication  */ 
#define  AT+STKMENU                     /* STK Main menu command   */
#define  AT+STKPCIS                     /* STK URC switch command  */
                                        
#define  AT+SIDET                       /* Change the Side Tone Gain Level  */
#define  AT+CPOWD                       /* Power off  */
#define  AT+SPIC                        /* Times Remained to Input SIM PIN/PUK  */
#define  AT+CMIC                        /* Change the Microphone Gain Level  */
#define  AT+CALA                        /* Set Alarm Time  */
#define  AT+CALD                        /* Delete Alarm  */
#define  AT+CADC                        /* Read ADC   */
#define  AT+CSNS                        /* Single Numbering Scheme  */ 
#define  AT+CDSCB                       /* Reset Cell Broadcast */
#define  AT+CMOD                        /* Configure Alternating Mode Calls  */
                                        
#define  AT+CFGRI                       /* Indicate RI When Using URC  */
#define  AT+CLTS                        /* Get Local Timestamp. */
#define  AT+CLDTMF                      /* Local DTMF Tone Generation. */
#define  AT+CDRIND                      /* CS Voice/Data Call Termination Indication  */
#define  AT+CSPN                        /* Get Service Provider Name from SIM */
#define  AT+CCVM                        /* Get and Set the Voice Mail Number on the SIM */
#define  AT+CBAND                       /* Get and Set Mobile Operation Band  */
#define  AT+CHF                         /* Configure Hands Free Operation. */
#define  AT+CHFA                        /* Swap the Audio Channels */
#define  AT+CSCLK                       /* Configure Slow Clock  */
#define  AT+CENG                        /* Switch on or off Engineering Mode */
#define  AT+SCLASS0                     /* Store Class 0 SMS to SIM When Received Class 0 SMS . */
#define  AT+CCID                        /* Show ICCID  */
#define  AT+CMTE                        /* Set Critical Temperature Operating Mode or Query  */
                                           Temperature */
#define  AT+CMGDA                       /* Delete All SMS. */
#define  AT+STTONE                      /* Play SIM Toolkit Tone */
#define  AT+SIMTONE                     /* Generate Specifically Tone */
#define  AT+CCPD                        /* Enable or Disable Alpha String */
#define  AT+CGID                        /* Get SIM Card Group Identifier */
#define  AT+MORING                      /* Show State of Mobile Originated Call..... */
#define  AT+CMGHEX                      /* Enable or Disable Sending Non-ASCII Character SMS */
#define  AT+CCODE                       /* Configure SMS Code Mode */
#define  AT+CIURC                       /* Enable or Disable Initial URC Presentation . */
#define  AT+CPSPWD                      /* Change PS Super Password  */
#define  AT+EXUNSOL                     /* Enable or Disable Proprietary Unsolicited Indications. */
#define  AT+CGMSCLASS                   /* Change GPRS Multislot Class */
#define  AT+CDEVICE                     /* View Current Flash Device Type */
#define  AT+CCALR                       /* Call Ready Query  */
#define  AT+GSV                         /* Display Product Identification Information */
#define  AT+SGPIO                       /* Control the GPIO. */
#define  AT+SPWM                        /* Generate the Pulse-Width-Modulation */
#define  AT+ECHO                        /* Echo Cancellation Control  */
#define  AT+CAAS                        /* Control Auto Audio Switch  */
#define  AT+SVR                         /* Configure Voice Coding Type for Voice Calls */
#define  AT+GSMBUSY                     /* Reject Incoming Call */
#define  AT+CEMNL                       /* Set the List of Emergency Number  */
#define  AT*CELLLOCK                    /* Set the List of ARFCN Which Needs to Be Locked */
#define  AT+SLEDS                       /* Set the Timer Period of Net Light */
#define  AT+CBUZZERRING                 /* Use the Buzzer Sound as the Incoming Call Ring */
#define  AT+CEXTERNTONE                 /* Close or Open the Microphone. */
#define  AT+CNETLIGHT                   /* Close the Net Light or Open It to Shining  */
#define  AT+CWHITELIST                  /* Set the White List */
#define  AT+CSDT                        /* Switch on or off Detecting SIM Card */
#define  AT+CSMINS                      /* SIM Inserted Status Reporting */
#define  AT+CSGS                        /* Netlight Indication of GPRS Status */
#define  AT+CMICBIAS                    /* Close or Open the MICBIAS */
#define  AT+DTAM                        /* Set TTS and RECORD Play Mode in Call  */
#define  AT+SJDR                        /* Set Jamming Detection Function */
#define  AT+CPCMCFG                     /* Set PCM Parameter */
#define  AT+CPCMSYNC                    /* Set PCM Sync Parameter */
#define  AT+CANT                        /* Antenna Detecting */
#define  AT+CAGCSET                     /* Close or Open AGC Function */
#define  AT+SD2PCM                      /* SD and PCM Switch Function */
#define  AT+SKPD                        /* Keypad Detecting Function */
#define  AT+SIMTONEX                    /* Custom Tones  */
#define  AT+CROAMING                    /* Roaming State */
#define  AT+CNETSCAN                    /* Perform a Net Survey to Show All the Cells�� Information. */
#define  AT+CMNRP                       /* Dual Serial Port Feature */
#define  AT+CEGPRS                      /* Switch on or off EDGE  */
#define  AT+CGPIO                       /* Control the GPIO by PIN Index . */
#define  AT+CMEDPLAY                    /* Play Audio File */
#define  AT+CMEDIAVOL                   /* Control the Volume when Playing Audio File */
#define  AT+SNDLEVEL                    /* Set the Sound Level of Special AT Command  */
#define  AT+ECHARGE                     /* Charge Control */
#define  AT+SIMTIMER                    /* Modify the Poll Interval Time Requested by SIM Card . */
#define  AT+SPE                         /* Speech Enhancement Control */
#define  AT+CCONCINDEX                  /* Report Concatenated SMS Index */
#define  AT+SDMODE                      /* SD Mode Switch Function */
#define  AT+SRSPT                       /* Control SMS Retransmission */
                                         
#define  AT+CGATT                       /* Attach or Detach from GPRS Service   */
#define  AT+CGDCONT                     /* Define PDP Context  */
#define  AT+CGQMIN                      /* Quality of Service Profile (Minimum Acceptable)  */
#define  AT+CGQREQ                      /* Quality of Service Profile (Requested)  */
#define  AT+CGACT                       /* PDP Context Activate or Deactivate  */
#define  AT+CGDATA                      /* Enter Data State  */
#define  AT+CGPADDR                     /* Show PDP Address   */
#define  AT+CGCLASS                     /* GPRS Mobile Station Class  */
#define  AT+CGEREP                      /* Control Unsolicited GPRS Event Reporting AT+CGREG Network 
                                           Registration Status  */
#define  AT+CGSMS                       /* Select Service for MO SMS Messages  */
                                        
#define  T+CIPMUX                       /* Start Up Multi-IP Connection   */
#define  T+CIPSTART                     /* Start Up TCP or UDP Connection  */
#define  T+CIPSEND                      /* Send Data Through TCP or UDP Connection.  */
#define  T+CIPQSEND                     /* Select Data Transmitting Mode  */
#define  T+CIPACK                       /* Query Previous Connection Data Transmitting State  */
#define  T+CIPCLOSE                     /* Close TCP or UDP Connection   */
#define  T+CIPSHUT                      /* Deactivate GPRS PDP Context   */
#define  T+CLPORT                       /* Set Local Port  */
#define  T+CSTT                         /* Start Task and Set APN, USER NAME, PASSWORD  */
#define  AT+CIICR                       /* Bring Up Wireless Connection with GPRS or CSD   */
#define  AT+CIFSR                       /* Get Local IP Address  */
#define  AT+CIPSTATUS                   /* Query Current Connection Status   */
#define  AT+CDNSCFG                     /* Configure Domain Name Server.  */
#define  AT+CDNSGIP                     /* Query the IP Address of Given Domain Name */
#define  AT+CIPHEAD                     /* Add an IP Head at the Beginning of a Package Received */
#define  AT+CIPATS                      /* Set Auto Sending Timer   */
#define  AT+CIPSPRT                     /* Set Prompt of ��>�� When Module Sends Data. */
#define  AT+CIPSERVER                   /* Configure Module as Server  */
#define  AT+CIPCSGP                     /* Set CSD or GPRS for Connection Mode   */
#define  AT+CIPSRIP                     /* Show Remote IP Address and Port When Received Data */
#define  AT+CIPDPDP                     /* Set Whether to Check State of GPRS Network Timing */
#define  AT+CIPMODE                     /* Select TCPIP Application Mode  */
#define  AT+CIPCCFG                     /* Configure Transparent Transfer Mode  */
#define  AT+CIPSHOWTP                   /* Display Transfer Protocol in IP Head When Received Data  */
#define  AT+CIPUDPMODE                  /* UDP Extended Mode  */
#define  AT+CIPRXGET                    /* Get Data from Network Manually  */
#define  AT+CIPSCONT                    /* Save TCPIP Application Context  */ 
#define  AT+CIPRDTIMER                  /* Set Remote Delay Timer  */
#define  AT+CIPSGTXT                    /* Select GPRS PDP context   */
#define  AT+CIPTKA                      /* Set TCP Keepalive Parameters  */
                                        
#define  AT+SAPBR                       /* Bearer Settings for Applications Based on IP  */
                                         
#define  AT+CIPPING                     /* PING Request  */
#define  AT+CIPCTL                      /* Set the Mode When Receiving an IP Packet  */
#define  AT+CIPFLT                      /* Set the Rules of IP Filter  */
#define  AT+CIPBEIPING                  /* Set the Module to be PING or Not  */
                                        
#define  AT+HTTPINIT                    /* Initialize HTTP Service   */
#define  AT+HTTPTERM                    /* Terminate HTTP Service  */
#define  AT+HTTPPARA                    /* Set HTTP Parameters Value  */
#define  AT+HTTPDATA                    /* Input HTTP Data  */
#define  AT+HTTPACTION                  /* HTTP Method Action   */
#define  AT+HTTPREAD                    /* Read the HTTP Server Response */
#define  AT+HTTPSCONT                   /* Save HTTP Application Context */
#define  AT+HTTPSTATUS                  /* Read HTTP Status .  */
#define  AT+HTTPHEAD                    /* Read the HTTP Header Information of Server Response  */
                                        
#define  AT+FTPPORT                     /* Set FTP Control Port  */
#define  AT+FTPMODE                     /* Set Active or Passive FTP Mode  */
#define  AT+FTPTYPE                     /* Set the Type of Data to Be Transferred.  */
#define  AT+FTPPUTOPT                   /* Set FTP Put Type  */
#define  AT+FTPCID                      /* Set FTP Bearer Profile Identifier  */
#define  AT+FTPREST                     /* Set Resume Broken Download .  */
#define  AT+FTPSERV                     /* Set FTP Server Address   */
#define  AT+FTPUN                       /* Set FTP User Name   */
#define  AT+FTPPW                       /* Set FTP Password..  */
#define  AT+FTPGETNAME                  /* Set Download File Name   */
#define  AT+FTPGETPATH                  /* Set Download File Path  */
#define  AT+FTPPUTNAME                  /* Set Upload File Name   */
#define  AT+FTPPUTPATH                  /* Set Upload File Path  */
#define  AT+FTPGET                      /* Download File  */
#define  AT+FTPPUT                      /* Set Upload File  */
#define  AT+FTPSCONT                    /* Save FTP Application Context */
#define  AT+FTPDELE                     /* Delete Specified File in FTP Server
#define  AT+FTPSIZE                     /* Get the Size of Specified File in FT P Server   */
#define  AT+FTPSTATE                    /* Get the FTP State */
#define  AT+FTPEXTPUT                   /* Extend Upload File */
#define  AT+FTPMKD                      /* Make Directory on the Remote Machine  */
#define  AT+FTPRMD                      /* Remove Directory on the Remote Machine */
#define  AT+FTPLIST                     /* List Contents of Directory on the Remote Machine. */
#define  AT+FTPGETTOFS                  /* Download File and Save in File System */
#define  AT+FTPPUTFRMFS                 /* Upload File from File System */
#define  AT+FTPEXTGET                   /* Extend Download File */                                             
#define  AT+FTPFILEPUT                  /* Load File in RAM from File System then Upolad with FTPPUT  */
#define  AT+FTPQUIT Quit                /* Current FTP Session */
                                        
#define  AT+CIPGSMLOC                   /* GSM Location and Time */
                                        
#define  AT+EMAILCID                    /* Set Email Bearer Profile Identifier.. */
#define  AT+EMAILTO                     /* Set Timeout Value of SMTP/POP3 Server Resp onse */
#define  AT+SMTPSRV                     /* Set SMTP Server Address and Port */
#define  AT+SMTPAUTH                    /* Set User Name and Password for SMT P Authentication */
#define  AT+SMTPFROM                    /* Set Sender Address and Name */
#define  AT+SMTPRCPT                    /* Set the Email Recipient(TO/CC/BCC) Address and Name */
#define  AT+SMTPSUB                     /* Set the Email Subject */
#define  AT+SMTPBODY                    /* Set the Email Body */
#define  AT+SMTPFILE                    /* Set the Email Attachment */
#define  AT+SMTPSEND                    /* Send the Email */
#define  AT+SMTPFT                      /* Transfer the Email Attachment */
#define  AT+SMTPCS                      /* Set the Email Charset */
#define  AT+POP3SRV                     /* Set POP3 Server and Account */
#define  AT+POP3IN                      /* Log in POP3 Server */
#define  AT+POP3NUM                     /* Get Email Number and Total Size */
#define  AT+POP3LIST                    /* Get the Specific Email Size  */
#define  AT+POP3UIDL                    /* Get the Specific Email Unique-id */
#define  AT+POP3CMD                     /* Get Multi-line Response */
#define  AT+POP3READ                    /* Read Multi-line Response */
#define  AT+POP3DEL                     /* Mark the Specific Email to Delete */
#define  AT+POP3RSET                    /* Unmark the Emails that Be Marked as Deleted */
#define  AT+POP3OUT                     /* Log Out POP3 Server */
                                        
#define  AT+CMMSCURL                    /* Set the URL of the MMS Center */
#define  AT+CMMSPROTO                   /* Set the Protocol Parameter and MMS Proxy  */
#define  AT+CMMSCID                     /* Set the Network Parameters for MMS */
#define  AT+CMMSSENDCFG                 /* Set the Parameters for Sending MMS */
#define  AT+CMMSEDIT                    /* Enter or Exit Edit Mode */
#define  AT+CMMSDOWN                    /* Download the File Data or Title from UART */
#define  AT+CMMSDELFILE                 /* Delete the File of the Edited MMS by File Index */
#define  AT+CMMSSEND                    /* Start MMS Sending */
#define  AT+CMMSRECP                    /* Add Recipients */
#define  AT+CMMSCC                      /* Add Copy Recipients */
#define  AT+CMMSBCC                     /* Add Secret Recipients */
#define  AT+CMMSDELRECP                 /* Delete Recipients */
#define  AT+CMMSDELCC                   /* Delete Copy Recipients  */
#define  AT+CMMSDELBCC                  /* Delete Secret Recipients */
#define  AT+CMMSRECV                    /* Receive MMS */
#define  AT+CMMSVIEW                    /* Get the MMS into Buffer and Show the Information. */
#define  AT+CMMSREAD                    /* Read the Given File of the MMS in the Buffer. */
#define  AT+CMMSRDPUSH                  /* Read the Information of the MMS PUSH Message. */
#define  AT+CMMSUA                      /* Set User Agent  */
#define  AT+CMMSPROFILE                 /* Set User Agent Profile  */
#define  AT+CMMSTIMEOUT                 /* Set MMS Timeout */
#define  AT+CMMSSTATUS                  /* Get MMS Status  */
#define  AT+CMMSINIT                    /* Initialize MMS Function */
#define  AT+CMMSTERM                    /* Exit MMS Function */
#define  AT+CMMSSCONT                   /* Save MMS Context */
                                        
#define  AT+DDET DTMF                   /* Detection Control */
                                         
#define  AT+CREC                        /* Record Operation */
#define  AT+CRECORD                     /* Record and Send Data to UART */
                                        
#define  AT+CTTS                        /* TTS Operation */
#define  AT+CTTSPARAM                   /* Set Parameters of the TTS Playing */
#define  AT+CTTSRING                    /* Enable/Disable TTS Play During Incoming Call Ring */



  
#define TCP_LinkType       1   //TCP�ͻ��� 
#define UDP_LinkType       2   //UDP 
#define SERVER_LinkType    3   //����SERVER
/*********************************************************************************
�������Ͷ���
*********************************************************************************/
typedef enum
{
  Void      = 0,        //�޲���ָ��
  Param     = 1,        //�� �� �� ָ �� 
  Query     = 2,        //��ѯָ�� ��ѯ��ָ�ǰ���õ�ֵ
  Help      = 3,        //����ָ� �г���ָ��Ŀ��ܲ�����
}AT_Format;


typedef enum{ 
//2.1 ��ָͨ��
 AT_Again  = 0,   //��ָ�������ظ���һ��ָ��
 ATA       = 1,   //Ӧ�����
 ATD       = 2,   //�������
 ATDL      = 3,   //������������ĵ绰���� 
 ATE       = 4,   //ʹ�ܻ���ͨ��
 ATH       = 5,   //�Ͽ���ǰ����
 ATI       = 6,   //��ʾģ������������Ϣ 
 ATQ       = 7,   //�����Ƿ����ն˻��Է���ֵ
 AtData    = 8,   //������ģʽ�л�������ģʽ 
 ATO       = 9,   //������ģʽ�л�������ģʽ 
 ATP       = 10,  //ʹ�����岦��
 ATS0      = 11,  //�Զ�Ӧ������
 AT_CRC    = 12,  //������������
 CLVL      = 13,  //�ܻ�����
 CLIP      = 14,  //�������������ʾ
 ZSETMUTE  = 15,  //��������
 CIMI      = 16,  //��ѯ���� ID ��
 CGMR      = 17,  //��ȡ��Ʒ�汾��
 ECHO      = 18,  //������������
 GSN       = 19,  //��ȡ��ǰ IMEI ��
 ZVERS     = 20,  //��ȡ��ǰ����汾��
 CLCK      = 21,  //��������
 CCFC      = 22,  //����ת�ƺ������������ 
 CCWA      = 23,  //���еȴ�����
 CHLD      = 24,  //���б��ֺͶ෽����
 TSIMINS   = 25,  //��ѯ SIM ��״̬
 CPWD      = 26,  //�޸�����
 CGMI      = 27,  //��ѯģ�鳧��
 CSCS      = 28,  //�ַ��������ѡ��
 CLCC      = 29,  //����״̬��ѯ
//DTMFָ��
 VTS     = 30,  //���� DTMF ��
//�������ָ�� 
 CREG    = 31,  //����ע�������
 COPS    = 32,  //����ѡ��
//�ƶ��豸���ƺ�״̬����
 CPAS    = 33,  //ģ��״̬��ѯ
 CFUN    = 34,  //����ģ�鹦��
 CMEE    = 35,  //�ƶ��豸���󱨸�
 ZPWROFF = 36,  //ģ��ػ�
 CPIN    = 37,  //���� PIN ��
 EPIN1   = 38,  //���� PIN1 ��
 EPIN2   = 39,  //���� PIN2 ��
 EPINC   = 40,  //��ѯ PIN1�� PIN2 ʣ���������
 CSQ     = 41,  //�ź�ǿ�Ȳ�ѯ
 CCLK    = 42,  //ʱ�ӹ���
// ��Ϣ����ָ�� 
 CSCA    = 43,    //�������ĺ���
 CNMA    = 44,    //����ȷ��
 CMGF    = 45,    //���ö���Ϣģʽ
 CNMI    = 46,    //���ö���ָʾ��ʽ
 CMGR    = 47,    //������Ϣ
 CMGW    = 48,    //д����Ϣ
 CSMS    = 49,    //ѡ����ŷ���
 CMGS    = 50,    //���Ͷ���
 CPMS    = 51,    //��ѡ����Ϣ�洢��
 CMGD    = 52,    //ɾ������
 CMGL    = 53,    //����Ϣ�б�
 CMSS    = 54,    //���ʹ����Ķ���
//�绰��ָ�� 
 CPBS    = 55,    //ѡ��绰���洢
 CPBR    = 56,    //��ȡ�绰����Ϣ
 CPBW    = 57,    //��绰��д��Ϣ
 CPBF    = 58,    //���ҵ绰����Ϣ
 CNUM    = 59,    //��ȡ��������
//����ѹ��ָ�� 
 IFC     = 60,    //������
 DTR     = 61,    //���� DTR ģʽ
 DCD     = 62,    //���� DCD ģʽ
 IPR     = 63,    //�趨ģ�鲨����
 AT_F      = 64,    //�ָ���������
 AT_W       = 65,   //��������
//GPRSָ��
 CGDCONT = 66,    //���� PDP ��ʽ
 CGACT   = 67,    //����/��� PDP ����
 CGATT   = 68,    //���� GPRS ҵ�� 
 CGCLASS = 69,    //GPRS �豸�ȼ�
//ZTE ����ָ��
 ZSTR          = 70,    //��ѯģ��״̬��Ϣ
 ZGETICCID     = 71,    //��ȡ ICCID
 ZCSQ          = 72,    //�����Զ���ʾ CSQ ������ 
 ZEDT          = 73,    //���� DTR �ļ��ģʽ 
 ZDSLEEP       = 74,    //32KHz ���˯��ģʽ
 CUSD          = 75,    //���� USSD ����
 ZRINGPINMODE  = 76,    //���� RING ���ŵ��ź�ģʽ 
 ZVERSWITCH    = 77,    //���ư汾�л�
// �������ָ�� 
 ZPNUM         = 78,    //���� APN���û��������� 
 ZPPPOPEN      = 79,    //�� GPRS �������� 
 ZPPPCLOSE     = 80,    //�ر� GPRS �������� 
 ZIPGETIP      = 81,    //��ѯ��ǰģ�� IP ��ֵַ 
 ZDNSSERV      = 82,    //���� DNS �������� IP ��ַ 
 ZDNSGETIP     = 83,    //��ȡ��Ӧ Internet ������ IP ��ַ 
//TCP ����ָ�� 
 ZIPSETUP      = 84,   //���� TCP ����������
 ZIPSEND       = 85,   //��Ŀ���ַ���� TCP ���� 
 ZPPPSTATUS    = 86,   //��ѯ GPRS ����״̬ 
 ZIPCLOSE      = 87,   //�ر� TCP ����
 ZIPSTATUS     = 88,   //��ѯ��ǰ TCP ����״̬ 
// UDP ����ָ�� 
 ZIPSETUPU     = 89,  //���� UDP ���������� 
 ZIPSENDU      = 90,  //�� UDP �������������� 
 ZIPSTATUSU    = 91,  //��ѯ UDP ״̬
 ZIPCLOSEU     = 92,  //�ر� UDP ����
// SERVER ָ�� 
 ZTCPLISTEN    = 93,  //�˿ڼ����������� 
 ZTCPSENDP     = 94,  //�ڱ����򿪵���·�Ϸ������� 
 ZTCPCLOSEP    = 95,  //�رռ�����������
 ZTCPSTATUSP   = 96,  //��ѯ��������·
 ZIPTIMEOUT    = 97,  //����ģ�����ӷ������ͷ������ݵĳ�ʱʱ��
 ZUDPLISTEN    = 98,  //�˿ڼ�����������
 ZUDPSENDP     = 99,  //�ڱ����򿪵���·�Ϸ�������
// FTP ָ��
 ZFTPLOGIN     = 100, //��¼ FTP ������
 ZFTPTYPE      = 101, //���� FTP �ļ�����
 ZFTPUPLOAD    = 102, //�ϴ��ļ�
 ZFTPSIZE      = 103, //��ȡ�ļ���С
 ZFTPDNLOAD    = 104, //�����ļ�
 ZFTPDNLOADEX  = 105, //���� FTP �ļ�
 ZFTPDEL       = 106, //ɾ���ļ�
 ZFTPQUIT      = 107, //�˳� FTP 
// ͸������ָ�� 
 ZTRANSFER     = 108, //͸������
//��Ƶ���ָ�� 
 ZCALLTONE     = 109, //ժ��������
 ZDTMFTONE     = 110, //ZDTMF ���������� 
 ZKTDSWITCH    = 111, //�� DTMF ��⹦��
 ZKTDIND       = 112, //�����ϱ����� DTMF ����
 SPEAKER       = 113, //����ͨ���л�ָ�� 
 ZMICGB        = 114, //���� MIC ��Ƶ����
//��վ���ָ��(ֱ��վ����ָ��) 
 CCED          = 115, //��ȡС����Ϣ
 ZBCCH         = 116, //���� BCCH �ŵ����ݲ�֧�֣�
 ZBAND         = 117, //���� GSM Ƶ�� 
 ZOPT          = 118, //����������Ӫ��
 ZCALIST       = 119, //��ȡ�ز��б�
// TTS ���ָ��
 ZTTS          = 120, //TTS(Text to Speech)��������ҵ�� 
 ZTTSP         = 121, //���� TTS(Text to Speech)������������(�ݲ�֧���л�������)
// ¼�����ָ��
 ZAUDREC       = 122, //¼������
 ZFILEREAD     = 123, //��ȡ�ļ�
// GPS ���ָ�� 
 ZGPSOPEN      = 124, //���� GPS ָ��
 ZGPSSLEEP     = 125, //GPS ����ָ�� 
 ZGPSNMEARPT   = 126, //NMEA �ϱ�����ָ�� 
 ZGPSLOC       = 127, //��ȡ GPS ��λ��γ��ָ�� 
 ZGPSUTC       = 128, //��ȡ GPS ʱ��ָ��
 ZGPSHS        = 129, //��ȡ�Ƕ��ٶ�ָ��
 ZGPSIND       = 130,  //GPS ��λ״̬�ϱ� 
   
 NC_Command_MG2618 = 255 
}Command_MG2618; 

//typedef enum{ 
// ZIPRECV       = 0,  //��ʾ�ӵ�ǰ������·�������� 
// ZIPRECVU      = 1,  //��ʾ���� UDP ���� 
// ZTCPRECV      = 2,  //��������ͨ��
// ZUDPRECV      = 3,  //��������ͨ��
// ZSMGS         = 4,  //������ָʾ
// VainIndicator = 5,
//}Indicator_MG2618;

//typedef struct {
 // unsigned char ch;
//} MG2618_IP_ConfigStructure;

//��ָ�����ڽ�������ѡ��
//�����������
typedef struct {
  enum{
    Auto          = '0',  //�Զ�����ѡ�񣬺��Բ���<format><oper>
    Manual        = '1',  //�˹�ѡ�����磬��Ҫ����<format><oper>
    Repeal        = '2',  //��������ע�ᣨ MTK��ƽ̨��֧�ִ˹��ܣ�
    SetingFormat  = '3',  //��ǣ������ע��Ķ���������������������<format>�ġ���ʱ�����в���<format>��
    Auto_Manual   = '4',  //4 manual/auto.�˹�ע�����粻�ɹ��󣬾��Զ�ע�����硣
  } mode;    
 enum{ 
   LongChar       = '0',  //���ַ���ʽ
   ShortChar      = '1',  //���ַ���ʽ
   Digit          = '2'   //���ָ�ʽ
 }format;   //��ֵΪ�趨��Ӫ��<oper>��ʾ�ĸ�ʽ�� 
unsigned char* oper;   //��Ӫ�����ƣ����� format ����
}COPS_PramStructure_MG2618;


//���� APN���û���������
//�����������
typedef struct { 
 unsigned char* APN;   //GPRS ��Ӫ���ṩ�� GPRS ���ʽڵ�(APN)��
 unsigned char* user;   //�û���½��
 unsigned char* pwd;   //  �û���½����
 enum{
   PS = '0',  //PS������
   CS =  '1'  //CS������
 }MODE;  //ѡ��������ʽ
 enum{
   pap  = '0', 
   chap = '1'
 }AUTH;  //��Ȩ��ʽ  
}ZPNUM_PramStructure_MG2618;

//�� GPRS ��������  
//�����������
typedef struct {
unsigned char* cmne;     //GPRS��Ӫ���ṩ�� GPRS ���ʽڵ�(APN)��
unsigned char* USER;    //�û���½����
unsigned char* PWD;     //�û���½���롣
}ZPPPOPEN_PramStructure_MG2618;

//���� DNS �������� IP ��ַ 
//�����������
typedef struct {
 unsigned char IP1[4];          //��DNS��������ַ
 unsigned char IP2[4];          //��DNS��������ַ
}IZDNSSERV_PramStructure_MG2618; 

//��ȡ��Ӧ Internet ������ IP ��ַ
//�����������
typedef struct {
 unsigned char* domainname;     //Internet����
}ZDNSGETIP_PramStructure_MG2618; 

//���� TCP/UDP ���������� 
//�����������
typedef struct {
 unsigned char  CH;             //TCP ���ӵ�ͨ���ţ�ȡֵ��Χ:0-4
 unsigned char  IP[4];          //Ϊһ��Ŀ���ַ IP ֵ    
 unsigned short PORT;           //�������˿�
 unsigned short LOCAL_PORT;    //���ذ󶨶˿ڣ�����ʹ�� 1024 ���ϵĶ˿ڰ� ��û������ LOCAL_PORT ʱ���˿��������
}IP_Connect_PramStructure_MG2618; 
//��Ŀ���ַ���� TCP / UDP ���� 
//�����������
typedef struct {
 unsigned char  CH;            //TCP ����ͨ����  ȡֵ��Χ:0-4
 unsigned short LENGTH;        //���ݳ���(���֧�� 4K ���ַ���֧�� 0x00~0xff ����)
 unsigned char  *buff;         //ָ�����ݻ�����
 SerialPort_SendEndCallBack_Type callBack;
 void *callBack_arg;
}IP_Send_PramStructure_MG2618; 
//��ѯ��ǰ TCP/UDP ����״̬ �� �ر� TCP/UDP ����
//�����������
typedef struct {
 unsigned char CH;             //TCP ����ͨ����  ȡֵ��Χ:0-4 
}IP_CSR_PramStructure_MG2618; 

//�˿ڼ�����������
//�����������
typedef struct {
 unsigned char State;          //'1':start listening '2':stop listening
 unsigned short PortNum;       //�������Ķ˿�   
}ZTCPLISTEN_ZUDPLISTEN_PramStructure_MG2618;
//�ڱ����򿪵���·�Ϸ�������
//�����������
typedef struct {
 unsigned char CH;             //���ӽ����Ŀͻ��˵ı�ʶ
 unsigned short Length;        //Ҫ�������ݵĳ��� 
 unsigned char *buff;          //ָ�����ݻ�����
}ZTCPSENDP_PramStructure_MG2618;
//�رռ�����������
//�����������
typedef struct {
 unsigned char CH;             //���ӽ����Ŀͻ��˵ı�ʶ
}ZTCPCLOSEP_PramStructure_MG2618;
//��ѯ��������·
//�����������
typedef struct {
 unsigned char CH;             //��ѯ��ͨ����
}ZTCPSTATUSP_PramStructure_MG2618;
//����ģ�����ӷ������ͷ������ݵĳ�ʱʱ��
//�����������
typedef struct {
 unsigned short connect_timeout;      //ģ����Ϊ�ͻ��˵����ӳ�ʱʱ�䣬��λ s��
 unsigned short send_data_timeout;    //ģ�鷢�����ݵĳ�ʱʱ�䣬ģ���ڴ�ʱ�������ݻ�û�з��ͳ�ȥ������Ϊ���������������쳣���رմ�����
}ZIPTIMEOUT_PramStructure_MG2618;
//�ڱ����򿪵���·�Ϸ������� UDP
//�����������
typedef struct {
unsigned char IP[4];     //Ŀ�ķ��� IP ��ַ�����ݽ��������ϱ��� 
unsigned short PORT;     //Ŀ�ķ��Ķ˿ڣ����ݽ��������ϱ���
unsigned short LEN;      //Ҫ�������ݵĳ��� 
unsigned char *buff;     //ָ�����ݻ�����
}ZUDPSENDP_PramStructure_MG2618;

//�������������
typedef  union{
   unsigned char                    CH_No;
   ZPNUM_PramStructure_MG2618        ZPNUM;            //���� APN���û��������� 
   IP_Connect_PramStructure_MG2618   TcpUdp_Connect;
   IP_Send_PramStructure_MG2618      TcpUdp_Send; 
   IP_CSR_PramStructure_MG2618       TcpUdp_CSR; 
   COPS_PramStructure_MG2618         COPS;             //��ָ�����ڽ�������ѡ��
  }CommandParameter_MG2618; 
//����������
typedef struct{                   
  Command_MG2618              Cmd;       //����������
  AT_Format                   Format;
  CommandParameter_MG2618     CmdPram;   //�������
}CommandDescriptor_MG2618;

typedef struct{
  unsigned char  IP[4];          //Ϊһ��Ŀ���ַ IP ֵ    
  unsigned short PORT;           //�������˿�
  unsigned short LOCAL_PORT;    //���ذ󶨶˿ڣ�����ʹ�� 1024 ���ϵĶ˿ڰ� ��û������ LOCAL_PORT ʱ���˿�������� 
}GprsInfr_Struct;  



typedef void (*GPRS_FinishCallBack)(Command_MG2618 cmd, DataBuffSpec_Struct *AckData);

typedef struct {
  
  struct{
    unsigned char RTEN:1;   //Ӧ��ʱ�ط�ʹ�� 
  }CSR; 
  
  union
  {
    unsigned char Byte;
    struct{
      unsigned char Flg :1;  //ͨ��ռ��״̬ 0 ���� 1 ռ��  
      unsigned char CN :1;   //ͨ������״̬ 0 û������ 1  ������ 
    }Bit;
  }CH_SR[4];
    
  GprsInfr_Struct GprsInfr[4];
    
  
  CommandDescriptor_MG2618   Command;    //��ǰ����������       
  GPRS_FinishCallBack        CallBack;   //������ɻص� 
  unsigned char             Cnt;        //���ʹ���������
  SofTimer_Struct            *TimeOut;   //��ʱ��ʱ��
  SofTimer_Struct            *HeartBeatTime;  //GPRS������ʱ��
  //SofTimer_Struct            *CH_Time[4];  //

  struct{
   PortStatus      Status;           //�˿�״ָ̬ʾ  
   unsigned char   LOCAL_IP[4];      //����IP��ַ   
   unsigned char   Rssi;             //�����ź�ǿ��
   unsigned char   Ber;              //�ŵ�λ������
  }ModInf;  
  unsigned char  String[MG2616_ATORDER_LEN];    //�洢ASCII��������
}MG2618DescriptorStruct;

typedef struct {
  struct {
    unsigned char  Valid:1;    //�������Ƿ���Ч
    unsigned char  Step :1;    //�������Ƿ���Ҫ�ಽA���� 
  }SR;                         //��״̬�Ĵ���
 unsigned char  PramLen;       //������������ֽ���
}CmdInformationStructure_MG2618;       
/*********************************************************************************
�ⲿ����������
*********************************************************************************/
extern MG2618DescriptorStruct MG2618Infr;
/*********************************************************************************
����������
*********************************************************************************/
FunctionalState Get_MG2168PowerState (void);

void PowerOff_GPRS (void);

void Decode_MG2618(unsigned char *gprs_ch, DataBuffSpec_Struct *Data);

unsigned short CompString (unsigned char *data,const char *sp,unsigned char **p);

void StopListen_MG2618(void);

SystemErrName SendCmd_MG2618(CommandDescriptor_MG2618 *Command,
                             GPRS_FinishCallBack CallBack,
                             unsigned char opt,
                             unsigned short timer);


/********************************************************************************/
#endif
