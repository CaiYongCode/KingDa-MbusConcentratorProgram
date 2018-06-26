/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
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
文件包含区
*********************************************************************************/
#include "PowerConfig.h"
#include ".\SerialPort.h"
#include "..\inc\DriverPcb.h"
#include "..\..\LIB\Common\inc\SofTimer.h"
/*********************************************************************************
宏定义区
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
#define  AT+CNETSCAN                    /* Perform a Net Survey to Show All the Cells’ Information. */
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
#define  AT+CIPSPRT                     /* Set Prompt of ‘>’ When Module Sends Data. */
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



  
#define TCP_LinkType       1   //TCP客户端 
#define UDP_LinkType       2   //UDP 
#define SERVER_LinkType    3   //本地SERVER
/*********************************************************************************
数据类型定义
*********************************************************************************/
typedef enum
{
  Void      = 0,        //无参数指令
  Param     = 1,        //带 参 数 指 令 
  Query     = 2,        //查询指令 查询该指令当前设置的值
  Help      = 3,        //帮助指令： 列出该指令的可能参数，
}AT_Format;


typedef enum{ 
//2.1 普通指令
 AT_Again  = 0,   //该指令用于重复上一条指令
 ATA       = 1,   //应答呼叫
 ATD       = 2,   //发起呼叫
 ATDL      = 3,   //拨打最近呼出的电话号码 
 ATE       = 4,   //使能回馈通道
 ATH       = 5,   //断开当前连接
 ATI       = 6,   //提示模块生产厂家信息 
 ATQ       = 7,   //设置是否在终端回显返回值
 AtData    = 8,   //从数据模式切换到命令模式 
 ATO       = 9,   //从命令模式切换到数据模式 
 ATP       = 10,  //使用脉冲拨号
 ATS0      = 11,  //自动应答设置
 AT_CRC    = 12,  //设置来电类型
 CLVL      = 13,  //受话音量
 CLIP      = 14,  //设置来电号码显示
 ZSETMUTE  = 15,  //静音控制
 CIMI      = 16,  //查询国际 ID 号
 CGMR      = 17,  //获取产品版本号
 ECHO      = 18,  //回音消除控制
 GSN       = 19,  //获取当前 IMEI 号
 ZVERS     = 20,  //获取当前软件版本号
 CLCK      = 21,  //功能锁定
 CCFC      = 22,  //呼叫转移号码和条件设置 
 CCWA      = 23,  //呼叫等待控制
 CHLD      = 24,  //呼叫保持和多方会议
 TSIMINS   = 25,  //查询 SIM 卡状态
 CPWD      = 26,  //修改密码
 CGMI      = 27,  //查询模块厂家
 CSCS      = 28,  //字符集种类的选择
 CLCC      = 29,  //呼叫状态查询
//DTMF指令
 VTS     = 30,  //发送 DTMF 音
//网络服务指令 
 CREG    = 31,  //网络注册和漫游
 COPS    = 32,  //网络选择
//移动设备控制和状态报告
 CPAS    = 33,  //模块状态查询
 CFUN    = 34,  //设置模块功能
 CMEE    = 35,  //移动设备错误报告
 ZPWROFF = 36,  //模块关机
 CPIN    = 37,  //输入 PIN 码
 EPIN1   = 38,  //输入 PIN1 码
 EPIN2   = 39,  //输入 PIN2 码
 EPINC   = 40,  //查询 PIN1， PIN2 剩余输入次数
 CSQ     = 41,  //信号强度查询
 CCLK    = 42,  //时钟管理
// 消息服务指令 
 CSCA    = 43,    //短信中心号码
 CNMA    = 44,    //短信确认
 CMGF    = 45,    //设置短消息模式
 CNMI    = 46,    //设置短信指示格式
 CMGR    = 47,    //读短消息
 CMGW    = 48,    //写短消息
 CSMS    = 49,    //选择短信服务
 CMGS    = 50,    //发送短信
 CPMS    = 51,    //首选短消息存储器
 CMGD    = 52,    //删除短信
 CMGL    = 53,    //短消息列表
 CMSS    = 54,    //发送存贮的短信
//电话本指令 
 CPBS    = 55,    //选择电话本存储
 CPBR    = 56,    //读取电话本信息
 CPBW    = 57,    //向电话本写信息
 CPBF    = 58,    //查找电话本信息
 CNUM    = 59,    //获取本机号码
//数据压缩指令 
 IFC     = 60,    //流控制
 DTR     = 61,    //设置 DTR 模式
 DCD     = 62,    //设置 DCD 模式
 IPR     = 63,    //设定模块波特率
 AT_F      = 64,    //恢复出厂设置
 AT_W       = 65,   //保存设置
//GPRS指令
 CGDCONT = 66,    //设置 PDP 格式
 CGACT   = 67,    //激活/解除 PDP 设置
 CGATT   = 68,    //设置 GPRS 业务 
 CGCLASS = 69,    //GPRS 设备等级
//ZTE 特有指令
 ZSTR          = 70,    //查询模块状态信息
 ZGETICCID     = 71,    //获取 ICCID
 ZCSQ          = 72,    //设置自动显示 CSQ 的条件 
 ZEDT          = 73,    //设置 DTR 的检测模式 
 ZDSLEEP       = 74,    //32KHz 深度睡眠模式
 CUSD          = 75,    //发送 USSD 数据
 ZRINGPINMODE  = 76,    //设置 RING 引脚的信号模式 
 ZVERSWITCH    = 77,    //定制版本切换
// 网络参数指令 
 ZPNUM         = 78,    //设置 APN，用户名，密码 
 ZPPPOPEN      = 79,    //打开 GPRS 数据连接 
 ZPPPCLOSE     = 80,    //关闭 GPRS 数据连接 
 ZIPGETIP      = 81,    //查询当前模块 IP 地址值 
 ZDNSSERV      = 82,    //设置 DNS 服务器的 IP 地址 
 ZDNSGETIP     = 83,    //获取对应 Internet 域名的 IP 地址 
//TCP 连接指令 
 ZIPSETUP      = 84,   //建立 TCP 服务器链接
 ZIPSEND       = 85,   //向目标地址发送 TCP 数据 
 ZPPPSTATUS    = 86,   //查询 GPRS 连接状态 
 ZIPCLOSE      = 87,   //关闭 TCP 链接
 ZIPSTATUS     = 88,   //查询当前 TCP 连接状态 
// UDP 连接指令 
 ZIPSETUPU     = 89,  //建立 UDP 服务器链接 
 ZIPSENDU      = 90,  //向 UDP 服务器发送数据 
 ZIPSTATUSU    = 91,  //查询 UDP 状态
 ZIPCLOSEU     = 92,  //关闭 UDP 链接
// SERVER 指令 
 ZTCPLISTEN    = 93,  //端口监听功能设置 
 ZTCPSENDP     = 94,  //在被动打开的链路上发送数据 
 ZTCPCLOSEP    = 95,  //关闭监听到的连接
 ZTCPSTATUSP   = 96,  //查询被动打开链路
 ZIPTIMEOUT    = 97,  //设置模块连接服务器和发送数据的超时时间
 ZUDPLISTEN    = 98,  //端口监听功能设置
 ZUDPSENDP     = 99,  //在被动打开的链路上发送数据
// FTP 指令
 ZFTPLOGIN     = 100, //登录 FTP 服务器
 ZFTPTYPE      = 101, //设置 FTP 文件类型
 ZFTPUPLOAD    = 102, //上传文件
 ZFTPSIZE      = 103, //获取文件大小
 ZFTPDNLOAD    = 104, //下载文件
 ZFTPDNLOADEX  = 105, //下载 FTP 文件
 ZFTPDEL       = 106, //删除文件
 ZFTPQUIT      = 107, //退出 FTP 
// 透明传输指令 
 ZTRANSFER     = 108, //透明传输
//音频相关指令 
 ZCALLTONE     = 109, //摘机音设置
 ZDTMFTONE     = 110, //ZDTMF 拨号音设置 
 ZKTDSWITCH    = 111, //打开 DTMF 检测功能
 ZKTDIND       = 112, //主动上报检测的 DTMF 按键
 SPEAKER       = 113, //语音通道切换指令 
 ZMICGB        = 114, //设置 MIC 音频参数
//基站相关指令(直放站需求指令) 
 CCED          = 115, //获取小区信息
 ZBCCH         = 116, //锁定 BCCH 信道（暂不支持）
 ZBAND         = 117, //锁定 GSM 频段 
 ZOPT          = 118, //锁定网络运营商
 ZCALIST       = 119, //读取载波列表
// TTS 相关指令
 ZTTS          = 120, //TTS(Text to Speech)语音播报业务 
 ZTTSP         = 121, //设置 TTS(Text to Speech)语音播报参数(暂不支持切换发音人)
// 录音相关指令
 ZAUDREC       = 122, //录音功能
 ZFILEREAD     = 123, //读取文件
// GPS 相关指令 
 ZGPSOPEN      = 124, //开关 GPS 指令
 ZGPSSLEEP     = 125, //GPS 休眠指令 
 ZGPSNMEARPT   = 126, //NMEA 上报控制指令 
 ZGPSLOC       = 127, //获取 GPS 定位经纬度指令 
 ZGPSUTC       = 128, //获取 GPS 时间指令
 ZGPSHS        = 129, //获取角度速度指令
 ZGPSIND       = 130,  //GPS 定位状态上报 
   
 NC_Command_MG2618 = 255 
}Command_MG2618; 

//typedef enum{ 
// ZIPRECV       = 0,  //提示从当前数据链路接收数据 
// ZIPRECVU      = 1,  //提示接收 UDP 数据 
// ZTCPRECV      = 2,  //接收数据通告
// ZUDPRECV      = 3,  //接收数据通告
// ZSMGS         = 4,  //短信满指示
// VainIndicator = 5,
//}Indicator_MG2618;

//typedef struct {
 // unsigned char ch;
//} MG2618_IP_ConfigStructure;

//该指令用于进行网络选择。
//操作命令参数
typedef struct {
  enum{
    Auto          = '0',  //自动网络选择，忽略参数<format><oper>
    Manual        = '1',  //人工选择网络，需要参数<format><oper>
    Repeal        = '2',  //从网络解除注册（ MTK此平台不支持此功能）
    SetingFormat  = '3',  //不牵涉网络注册的动作，此命令是用来设置<format>的。此时必须有参数<format>。
    Auto_Manual   = '4',  //4 manual/auto.人工注册网络不成功后，就自动注册网络。
  } mode;    
 enum{ 
   LongChar       = '0',  //长字符格式
   ShortChar      = '1',  //短字符格式
   Digit          = '2'   //数字格式
 }format;   //此值为设定运营商<oper>显示的格式） 
unsigned char* oper;   //运营商名称，根据 format 给出
}COPS_PramStructure_MG2618;


//设置 APN，用户名，密码
//操作命令参数
typedef struct { 
 unsigned char* APN;   //GPRS 运营商提供的 GPRS 访问节点(APN)。
 unsigned char* user;   //用户登陆名
 unsigned char* pwd;   //  用户登陆密码
 enum{
   PS = '0',  //PS域上网
   CS =  '1'  //CS域上网
 }MODE;  //选择上网方式
 enum{
   pap  = '0', 
   chap = '1'
 }AUTH;  //授权方式  
}ZPNUM_PramStructure_MG2618;

//打开 GPRS 数据连接  
//操作命令参数
typedef struct {
unsigned char* cmne;     //GPRS运营商提供的 GPRS 访问节点(APN)。
unsigned char* USER;    //用户登陆名。
unsigned char* PWD;     //用户登陆密码。
}ZPPPOPEN_PramStructure_MG2618;

//设置 DNS 服务器的 IP 地址 
//操作命令参数
typedef struct {
 unsigned char IP1[4];          //主DNS服务器地址
 unsigned char IP2[4];          //次DNS服务器地址
}IZDNSSERV_PramStructure_MG2618; 

//获取对应 Internet 域名的 IP 地址
//操作命令参数
typedef struct {
 unsigned char* domainname;     //Internet域名
}ZDNSGETIP_PramStructure_MG2618; 

//建立 TCP/UDP 服务器链接 
//操作命令参数
typedef struct {
 unsigned char  CH;             //TCP 连接的通道号，取值范围:0-4
 unsigned char  IP[4];          //为一个目标地址 IP 值    
 unsigned short PORT;           //服务器端口
 unsigned short LOCAL_PORT;    //本地绑定端口，建议使用 1024 以上的端口绑定 当没有设置 LOCAL_PORT 时，端口随机分配
}IP_Connect_PramStructure_MG2618; 
//向目标地址发送 TCP / UDP 数据 
//操作命令参数
typedef struct {
 unsigned char  CH;            //TCP 连接通道号  取值范围:0-4
 unsigned short LENGTH;        //数据长度(最大支持 4K 个字符，支持 0x00~0xff 发送)
 unsigned char  *buff;         //指向数据缓冲区
 SerialPort_SendEndCallBack_Type callBack;
 void *callBack_arg;
}IP_Send_PramStructure_MG2618; 
//查询当前 TCP/UDP 连接状态 或 关闭 TCP/UDP 链接
//操作命令参数
typedef struct {
 unsigned char CH;             //TCP 连接通道号  取值范围:0-4 
}IP_CSR_PramStructure_MG2618; 

//端口监听功能设置
//操作命令参数
typedef struct {
 unsigned char State;          //'1':start listening '2':stop listening
 unsigned short PortNum;       //被监听的端口   
}ZTCPLISTEN_ZUDPLISTEN_PramStructure_MG2618;
//在被动打开的链路上发送数据
//操作命令参数
typedef struct {
 unsigned char CH;             //连接进来的客户端的标识
 unsigned short Length;        //要发送数据的长度 
 unsigned char *buff;          //指向数据缓冲区
}ZTCPSENDP_PramStructure_MG2618;
//关闭监听到的连接
//操作命令参数
typedef struct {
 unsigned char CH;             //连接进来的客户端的标识
}ZTCPCLOSEP_PramStructure_MG2618;
//查询被动打开链路
//操作命令参数
typedef struct {
 unsigned char CH;             //查询的通道号
}ZTCPSTATUSP_PramStructure_MG2618;
//设置模块连接服务器和发送数据的超时时间
//操作命令参数
typedef struct {
 unsigned short connect_timeout;      //模块作为客户端的连接超时时间，单位 s。
 unsigned short send_data_timeout;    //模块发送数据的超时时间，模块在此时间内数据还没有发送出去，就认为服务器或者网络异常，关闭此连接
}ZIPTIMEOUT_PramStructure_MG2618;
//在被动打开的链路上发送数据 UDP
//操作命令参数
typedef struct {
unsigned char IP[4];     //目的方的 IP 地址（依据接收数据上报） 
unsigned short PORT;     //目的方的端口（依据接收数据上报）
unsigned short LEN;      //要发送数据的长度 
unsigned char *buff;     //指向数据缓冲区
}ZUDPSENDP_PramStructure_MG2618;

//命令参数描述符
typedef  union{
   unsigned char                    CH_No;
   ZPNUM_PramStructure_MG2618        ZPNUM;            //设置 APN，用户名，密码 
   IP_Connect_PramStructure_MG2618   TcpUdp_Connect;
   IP_Send_PramStructure_MG2618      TcpUdp_Send; 
   IP_CSR_PramStructure_MG2618       TcpUdp_CSR; 
   COPS_PramStructure_MG2618         COPS;             //该指令用于进行网络选择。
  }CommandParameter_MG2618; 
//命令描述符
typedef struct{                   
  Command_MG2618              Cmd;       //操作命令码
  AT_Format                   Format;
  CommandParameter_MG2618     CmdPram;   //命令参数
}CommandDescriptor_MG2618;

typedef struct{
  unsigned char  IP[4];          //为一个目标地址 IP 值    
  unsigned short PORT;           //服务器端口
  unsigned short LOCAL_PORT;    //本地绑定端口，建议使用 1024 以上的端口绑定 当没有设置 LOCAL_PORT 时，端口随机分配 
}GprsInfr_Struct;  



typedef void (*GPRS_FinishCallBack)(Command_MG2618 cmd, DataBuffSpec_Struct *AckData);

typedef struct {
  
  struct{
    unsigned char RTEN:1;   //应答超时重发使能 
  }CSR; 
  
  union
  {
    unsigned char Byte;
    struct{
      unsigned char Flg :1;  //通道占用状态 0 空闲 1 占用  
      unsigned char CN :1;   //通道连接状态 0 没有连接 1  以连接 
    }Bit;
  }CH_SR[4];
    
  GprsInfr_Struct GprsInfr[4];
    
  
  CommandDescriptor_MG2618   Command;    //当前命令描述符       
  GPRS_FinishCallBack        CallBack;   //操作完成回掉 
  unsigned char             Cnt;        //发送次数计数器
  SofTimer_Struct            *TimeOut;   //超时定时器
  SofTimer_Struct            *HeartBeatTime;  //GPRS心跳定时器
  //SofTimer_Struct            *CH_Time[4];  //

  struct{
   PortStatus      Status;           //端口状态指示  
   unsigned char   LOCAL_IP[4];      //本地IP地址   
   unsigned char   Rssi;             //接收信号强度
   unsigned char   Ber;              //信道位错误率
  }ModInf;  
  unsigned char  String[MG2616_ATORDER_LEN];    //存储ASCII命令序列
}MG2618DescriptorStruct;

typedef struct {
  struct {
    unsigned char  Valid:1;    //该命令是否有效
    unsigned char  Step :1;    //该密令是否需要多步A操作 
  }SR;                         //制状态寄存器
 unsigned char  PramLen;       //操作命令参数字节数
}CmdInformationStructure_MG2618;       
/*********************************************************************************
外部变量声明区
*********************************************************************************/
extern MG2618DescriptorStruct MG2618Infr;
/*********************************************************************************
函数声明区
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
