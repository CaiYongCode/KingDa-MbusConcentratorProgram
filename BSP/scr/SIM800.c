/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\SIM800.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
//AT 指令列表
const char*  const CmdNameList_MG2618[] = { 
"A/",                             /* Re-issues the Last Command Given */
"ATA",                            /* Answer an Incoming Call. */
"ATD",                            /* Mobile Originated Call to Dial A Number */
"ATD_N",                          /* Originate Call to Phone Number in Current Memory */
"ATD_S",                          /* Originate Call to Phone Number in Memory Which                                   Corresponds to */
"ATDL",                           /* Redial Last Telephone Number Used */
"ATE",                            /* Set Command Echo Mode  */
"ATH",                            /* Disconnect Existing Connection */
"ATI",                            /* Display Product Identification Information */ 
"ATL",                            /* Set Monitor speaker loudnes */
"ATM",                            /* Set Monitor Speaker Mode */
"+++",                            /* Switch from Data Mode or PPP Online Mode to Command Mode  */
"ATO",                            /* Switch from Command Mode to Data Mode */
"ATP",                            /* Select Pulse Dialling */
"ATQ",                            /* Set Result Code Presentation Mode */
"ATS0",                           /* Set Number of Rings before Automatically Answering the 
                                     Call */
"ATS3",                           /* Set Command Line Termination Character */
"ATS4",                           /* Set Response Formatting Character */ 
"ATS5",                           /* Set Command Line Editing Character */
"ATS6",                           /* Pause Before Blind Dialling */
"ATS7",                           /* Set Number of Seconds to Wait for Connection Completion */
"ATS8",                           /* Set Number of Seconds to Wait for Comma Dial Modifier 
"                                  Encountered inDial String of D Command  */
"ATS10                          /* Set Disconnect Delay after Indicating the Absence of Data 
"                                  Carrier */
"ATT                            /* Select Tone Dialing  */
"ATV                            /* TA Response Format  */
"ATX                            /* Set CONNECT Result Code Format and Monitor Call Progress */
"ATZ                            /* Reset Default Configuration */
"ATC                           /* Set DCD Function Mode  */
"AT&D                           /* Set DTR Function Mode */
"AT&F                           /* Factory Defined Configuration */
"AT&V                           /* Display Current Configuration */
"AT&W                           /* Store Active Profile  */
"AT+GCAP                        /* Request Complete TA Capabilities List */ 
"AT+GMI                         /* Request Manufacturer Identification */ 
"AT+GMM                         /* Request TA Model Identification  */
"AT+GMR                         /* Request TA Revision Identification of Software Release. */
"AT+GOI                         /* Request Global Object Identification. */
"AT+GSN                         /* Request TA Serial Number Identification (IMEI) */
"AT+ICF                         /* Set TE-TA Control Character Framing. */
"AT+IFC                         /* Set TE-TA Local Data Flow Control */
"AT+IPR                         /* Set TE-TA Fixed Local Rate  */
"AT+HVOIC                       /* Disconnect Voice Call Only */
"                               
"AT+CACM                        /* Accumulated Call Meter (ACM) Reset or Query */
"AT+CAMM                        /* Accumulated Call Meter Maximum (ACM max) Set or Query */ 
"AT+CAOC                        /* Advice of Charge */ 
"AT+CBST                        /* Select Bearer Service Type */
"AT+CCFC                        /* Call Forwarding Number and Conditions Control */
"AT+CCWA                        /* Call Waiting Control */ 
"AT+CEER                        /* Extended Error Report   */
"AT+CGMI                        /* Request Manufacturer Identification  */
"AT+CGMM                        /* Request Model Identification */
"AT+CGMR                        /* Request TA Revision Identification of Software Release */
"AT+CGSN                        /* Request Product Serial Number Identification 
"                                 (Identical with +GSN) */
"AT+CSCS                        /* Select TE Character Set */
"AT+CSTA                        /* Select Type of Address */
"AT+CHLD                        /* Call Hold and Multiparty */
"AT+CIMI                        /* Request International Mobile Subscriber Identity */
"AT+CLCC                        /* List Current Calls of ME */
"AT+CLCK                        /* Facility Lock */
"AT+CLIP                        /* Calling Line Identification Presentation */
"AT+CLIR                        /* Calling Line Identification Restriction */
"AT+CMEE                        /* Report Mobile Equipment Error */
"AT+COLP                        /* Connected Line Identification Presentation  */
"AT+COPS                        /* Operator Selection */
"AT+CPAS                        /* Phone Activity Status */
"AT+CPBF                        /* Find Phonebook Entries */
"AT+CPBR                        /* Read Current Phonebook Entries */
"AT+CPBS                        /* Select Phonebook Memory Storage. */
"AT+CPBW                        /* Write Phonebook Entry. */
"AT+CPIN                        /* Enter PIN */
"AT+CPWD                        /* Change Password */
"AT+CR                          /* Service Reporting Control  */
"AT+CRC                         /* Set Cellular Result Codes for Incoming Call Indication  */
"AT+CREG                        /* Network Registration */
"AT+CRLP                        /* Select Radio Link Protocol Parameters  */
"AT+CRSM                        /* Restricted SIM Access  */
"AT+CSQ                         /* Signal Quality Report */
"AT+VTD                         /* Tone Duration */
"AT+VTS                         /* DTMF and Tone Generation */
"AT+CMUX                        /* Multiplexer Control */
"AT+CNUM                        /* Subscriber Number */
"AT+CPOL                        /* Preferred Operator List */
"AT+COPN                        /* Read Operator Names */
"AT+CFUN                        /* Set Phone Functionality */
"AT+CCLK                        /* Clock */
"AT+CSIM                        /* Generic SIM Access */
"AT+CALM                        /* Alert Sound Mode */
"AT+CALS                        /* Alert Sound Select */
"AT+CRSL                        /* Ringer Sound Level */
"AT+CLVL                        /* Loud Speaker Volume Level */
"AT+CMUT                        /* Mute Control. */
"AT+CPUC                        /* Price Per Unit and Currency Table */
"AT+CCWE                        /* Call Meter Maximum Event */
"AT+CBC                         /* Battery Charge */
"AT+CUSD                        /* Unstructured Supplementary Service Data */
"AT+CSSN                        /* Supplementary Services Notification */
"                               
"AT+CMGD                        /* Delete SMS Message */
"AT+CMGF                        /* Select SMS Message Format */
"AT+CMGL                        /* List SMS Messages from Preferred Store */
"AT+CMGR                        /* Read SMS Message */
"AT+CMGS                        /* Send SMS Message. */
"AT+CMGW                        /* Write SMS Message to Memory */
"AT+CMSS                        /* Send SMS Message from Storage  */
"AT+CNMI                        /* New SMS Message Indications  */
"AT+CPMS                        /* Preferred SMS Message Storage */
"AT+CRES                        /* Restore SMS Settings */
"AT+CSAS                        /* Save SMS Settings */ 
"AT+CSCA                        /* SMS Service Center Address */ 
"AT+CSCB                        /* Select Cell Broadcast SMS Messages   */
"AT+CSDH                        /* Show SMS Text Mode Parameters  */
"AT+CSMP                        /* Set SMS Text Mode Parameters  */
"AT+CSMS                        /* Select Message Service  */
"                               
"AT+STKTRS                      /* STK Terminal Response  */
"AT+STKENVS                     /* STK Envelope Command  */
"AT+STKCALL                     /* STK call setup  */
"AT+STKSMS                      /* STK SMS delivery  */
"AT+STKSS                       /* STK SS setup  */
"AT+STKUSSD                     /* STK USSD setup  */
"AT+STKDTMF                     /* STK sending DTMF  */
"+STKPCI                        /* STK Proactive Command Indication  */ 
"AT+STKMENU                     /* STK Main menu command   */
"AT+STKPCIS                     /* STK URC switch command  */
"                               
"AT+SIDET                       /* Change the Side Tone Gain Level  */
"AT+CPOWD                       /* Power off  */
"AT+SPIC                        /* Times Remained to Input SIM PIN/PUK  */
"AT+CMIC                        /* Change the Microphone Gain Level  */
"AT+CALA                        /* Set Alarm Time  */
"AT+CALD                        /* Delete Alarm  */
"AT+CADC                        /* Read ADC   */
"AT+CSNS                        /* Single Numbering Scheme  */ 
"AT+CDSCB                       /* Reset Cell Broadcast */
"AT+CMOD                        /* Configure Alternating Mode Calls  */
"                               
"AT+CFGRI                       /* Indicate RI When Using URC  */
"AT+CLTS                        /* Get Local Timestamp. */
"AT+CLDTMF                      /* Local DTMF Tone Generation. */
"AT+CDRIND                      /* CS Voice/Data Call Termination Indication  */
"AT+CSPN                        /* Get Service Provider Name from SIM */
"AT+CCVM                        /* Get and Set the Voice Mail Number on the SIM */
"AT+CBAND                       /* Get and Set Mobile Operation Band  */
"AT+CHF                         /* Configure Hands Free Operation. */
"AT+CHFA                        /* Swap the Audio Channels */
"AT+CSCLK                       /* Configure Slow Clock  */
"AT+CENG                        /* Switch on or off Engineering Mode */
"AT+SCLASS0                     /* Store Class 0 SMS to SIM When Received Class 0 SMS . */
"AT+CCID                        /* Show ICCID  */
"AT+CMTE                        /* Set Critical Temperature Operating Mode or Query  */
"                                  Temperature */
"AT+CMGDA                       /* Delete All SMS. */
"AT+STTONE                      /* Play SIM Toolkit Tone */
"AT+SIMTONE                     /* Generate Specifically Tone */
"AT+CCPD                        /* Enable or Disable Alpha String */
"AT+CGID                        /* Get SIM Card Group Identifier */
"AT+MORING                      /* Show State of Mobile Originated Call..... */
"AT+CMGHEX                      /* Enable or Disable Sending Non-ASCII Character SMS */
"AT+CCODE                       /* Configure SMS Code Mode */
"AT+CIURC                       /* Enable or Disable Initial URC Presentation . */
"AT+CPSPWD                      /* Change PS Super Password  */
"AT+EXUNSOL                     /* Enable or Disable Proprietary Unsolicited Indications. */
"AT+CGMSCLASS                   /* Change GPRS Multislot Class */
"AT+CDEVICE                     /* View Current Flash Device Type */
"AT+CCALR                       /* Call Ready Query  */
"AT+GSV                         /* Display Product Identification Information */
"AT+SGPIO                       /* Control the GPIO. */
"AT+SPWM                        /* Generate the Pulse-Width-Modulation */
"AT+ECHO                        /* Echo Cancellation Control  */
"AT+CAAS                        /* Control Auto Audio Switch  */
"AT+SVR                         /* Configure Voice Coding Type for Voice Calls */
"AT+GSMBUSY                     /* Reject Incoming Call */
"AT+CEMNL                       /* Set the List of Emergency Number  */
"AT*CELLLOCK                    /* Set the List of ARFCN Which Needs to Be Locked */
"AT+SLEDS                       /* Set the Timer Period of Net Light */
"AT+CBUZZERRING                 /* Use the Buzzer Sound as the Incoming Call Ring */
"AT+CEXTERNTONE                 /* Close or Open the Microphone. */
"AT+CNETLIGHT                   /* Close the Net Light or Open It to Shining  */
"AT+CWHITELIST                  /* Set the White List */
"AT+CSDT                        /* Switch on or off Detecting SIM Card */
"AT+CSMINS                      /* SIM Inserted Status Reporting */
"AT+CSGS                        /* Netlight Indication of GPRS Status */
"AT+CMICBIAS                    /* Close or Open the MICBIAS */
"AT+DTAM                        /* Set TTS and RECORD Play Mode in Call  */
"AT+SJDR                        /* Set Jamming Detection Function */
"AT+CPCMCFG                     /* Set PCM Parameter */
"AT+CPCMSYNC                    /* Set PCM Sync Parameter */
"AT+CANT                        /* Antenna Detecting */
"AT+CAGCSET                     /* Close or Open AGC Function */
"AT+SD2PCM                      /* SD and PCM Switch Function */
"AT+SKPD                        /* Keypad Detecting Function */
"AT+SIMTONEX                    /* Custom Tones  */
"AT+CROAMING                    /* Roaming State */
"AT+CNETSCAN                    /* Perform a Net Survey to Show All the Cells’ Information. */
"AT+CMNRP                       /* Dual Serial Port Feature */
"AT+CEGPRS                      /* Switch on or off EDGE  */
"AT+CGPIO                       /* Control the GPIO by PIN Index . */
"AT+CMEDPLAY                    /* Play Audio File */
"AT+CMEDIAVOL                   /* Control the Volume when Playing Audio File */
"AT+SNDLEVEL                    /* Set the Sound Level of Special AT Command  */
"AT+ECHARGE                     /* Charge Control */
"AT+SIMTIMER                    /* Modify the Poll Interval Time Requested by SIM Card . */
"AT+SPE                         /* Speech Enhancement Control */
"AT+CCONCINDEX                  /* Report Concatenated SMS Index */
"AT+SDMODE                      /* SD Mode Switch Function */
"AT+SRSPT                       /* Control SMS Retransmission */
"                                
"AT+CGATT                       /* Attach or Detach from GPRS Service   */
"AT+CGDCONT                     /* Define PDP Context  */
"AT+CGQMIN                      /* Quality of Service Profile (Minimum Acceptable)  */
"AT+CGQREQ                      /* Quality of Service Profile (Requested)  */
"AT+CGACT                       /* PDP Context Activate or Deactivate  */
"AT+CGDATA                      /* Enter Data State  */
"AT+CGPADDR                     /* Show PDP Address   */
"AT+CGCLASS                     /* GPRS Mobile Station Class  */
"AT+CGEREP                      /* Control Unsolicited GPRS Event Reporting AT+CGREG Network 
"                                  Registration Status  */
"AT+CGSMS                       /* Select Service for MO SMS Messages  */
"                               
"T+CIPMUX                       /* Start Up Multi-IP Connection   */
"T+CIPSTART                     /* Start Up TCP or UDP Connection  */
"T+CIPSEND                      /* Send Data Through TCP or UDP Connection.  */
"T+CIPQSEND                     /* Select Data Transmitting Mode  */
"T+CIPACK                       /* Query Previous Connection Data Transmitting State  */
"T+CIPCLOSE                     /* Close TCP or UDP Connection   */
"T+CIPSHUT                      /* Deactivate GPRS PDP Context   */
"T+CLPORT                       /* Set Local Port  */
"T+CSTT                         /* Start Task and Set APN, USER NAME, PASSWORD  */
"AT+CIICR                       /* Bring Up Wireless Connection with GPRS or CSD   */
"AT+CIFSR                       /* Get Local IP Address  */
"AT+CIPSTATUS                   /* Query Current Connection Status   */
"AT+CDNSCFG                     /* Configure Domain Name Server.  */
"AT+CDNSGIP                     /* Query the IP Address of Given Domain Name */
"AT+CIPHEAD                     /* Add an IP Head at the Beginning of a Package Received */
"AT+CIPATS                      /* Set Auto Sending Timer   */
"AT+CIPSPRT                     /* Set Prompt of ‘>’ When Module Sends Data. */
"AT+CIPSERVER                   /* Configure Module as Server  */
"AT+CIPCSGP                     /* Set CSD or GPRS for Connection Mode   */
"AT+CIPSRIP                     /* Show Remote IP Address and Port When Received Data */
"AT+CIPDPDP                     /* Set Whether to Check State of GPRS Network Timing */
"AT+CIPMODE                     /* Select TCPIP Application Mode  */
"AT+CIPCCFG                     /* Configure Transparent Transfer Mode  */
"AT+CIPSHOWTP                   /* Display Transfer Protocol in IP Head When Received Data  */
"AT+CIPUDPMODE                  /* UDP Extended Mode  */
"AT+CIPRXGET                    /* Get Data from Network Manually  */
"AT+CIPSCONT                    /* Save TCPIP Application Context  */ 
"AT+CIPRDTIMER                  /* Set Remote Delay Timer  */
"AT+CIPSGTXT                    /* Select GPRS PDP context   */
"AT+CIPTKA                      /* Set TCP Keepalive Parameters  */
"                               
"AT+SAPBR                       /* Bearer Settings for Applications Based on IP  */
"                                
"AT+CIPPING                     /* PING Request  */
"AT+CIPCTL                      /* Set the Mode When Receiving an IP Packet  */
"AT+CIPFLT                      /* Set the Rules of IP Filter  */
"AT+CIPBEIPING                  /* Set the Module to be PING or Not  */
"                               
"AT+HTTPINIT                    /* Initialize HTTP Service   */
"AT+HTTPTERM                    /* Terminate HTTP Service  */
"AT+HTTPPARA                    /* Set HTTP Parameters Value  */
"AT+HTTPDATA                    /* Input HTTP Data  */
"AT+HTTPACTION                  /* HTTP Method Action   */
"AT+HTTPREAD                    /* Read the HTTP Server Response */
"AT+HTTPSCONT                   /* Save HTTP Application Context */
"AT+HTTPSTATUS                  /* Read HTTP Status .  */
"AT+HTTPHEAD                    /* Read the HTTP Header Information of Server Response  */
"                               
"AT+FTPPORT                     /* Set FTP Control Port  */
"AT+FTPMODE                     /* Set Active or Passive FTP Mode  */
"AT+FTPTYPE                     /* Set the Type of Data to Be Transferred.  */
"AT+FTPPUTOPT                   /* Set FTP Put Type  */
"AT+FTPCID                      /* Set FTP Bearer Profile Identifier  */
"AT+FTPREST                     /* Set Resume Broken Download .  */
"AT+FTPSERV                     /* Set FTP Server Address   */
"AT+FTPUN                       /* Set FTP User Name   */
"AT+FTPPW                       /* Set FTP Password..  */
"AT+FTPGETNAME                  /* Set Download File Name   */
"AT+FTPGETPATH                  /* Set Download File Path  */
"AT+FTPPUTNAME                  /* Set Upload File Name   */
"AT+FTPPUTPATH                  /* Set Upload File Path  */
"AT+FTPGET                      /* Download File  */
"AT+FTPPUT                      /* Set Upload File  */
"AT+FTPSCONT                    /* Save FTP Application Context */
"AT+FTPDELE                     /* Delete Specified File in FTP Server
"AT+FTPSIZE                     /* Get the Size of Specified File in FT P Server   */
"AT+FTPSTATE                    /* Get the FTP State */
"AT+FTPEXTPUT                   /* Extend Upload File */
"AT+FTPMKD                      /* Make Directory on the Remote Machine  */
"AT+FTPRMD                      /* Remove Directory on the Remote Machine */
"AT+FTPLIST                     /* List Contents of Directory on the Remote Machine. */
"AT+FTPGETTOFS                  /* Download File and Save in File System */
"AT+FTPPUTFRMFS                 /* Upload File from File System */
"AT+FTPEXTGET                   /* Extend Download File */                                             
"AT+FTPFILEPUT                  /* Load File in RAM from File System then Upolad with FTPPUT  */
"AT+FTPQUIT Quit                /* Current FTP Session */
"                               
"AT+CIPGSMLOC                   /* GSM Location and Time */
"                               
"AT+EMAILCID                    /* Set Email Bearer Profile Identifier.. */
"AT+EMAILTO                     /* Set Timeout Value of SMTP/POP3 Server Resp onse */
"AT+SMTPSRV                     /* Set SMTP Server Address and Port */
"AT+SMTPAUTH                    /* Set User Name and Password for SMT P Authentication */
"AT+SMTPFROM                    /* Set Sender Address and Name */
"AT+SMTPRCPT                    /* Set the Email Recipient(TO/CC/BCC) Address and Name */
"AT+SMTPSUB                     /* Set the Email Subject */
"AT+SMTPBODY                    /* Set the Email Body */
"AT+SMTPFILE                    /* Set the Email Attachment */
"AT+SMTPSEND                    /* Send the Email */
"AT+SMTPFT                      /* Transfer the Email Attachment */
"AT+SMTPCS                      /* Set the Email Charset */
"AT+POP3SRV                     /* Set POP3 Server and Account */
"AT+POP3IN                      /* Log in POP3 Server */
"AT+POP3NUM                     /* Get Email Number and Total Size */
"AT+POP3LIST                    /* Get the Specific Email Size  */
"AT+POP3UIDL                    /* Get the Specific Email Unique-id */
"AT+POP3CMD                     /* Get Multi-line Response */
"AT+POP3READ                    /* Read Multi-line Response */
"AT+POP3DEL                     /* Mark the Specific Email to Delete */
"AT+POP3RSET                    /* Unmark the Emails that Be Marked as Deleted */
"AT+POP3OUT                     /* Log Out POP3 Server */
"                               
"AT+CMMSCURL                    /* Set the URL of the MMS Center */
"AT+CMMSPROTO                   /* Set the Protocol Parameter and MMS Proxy  */
"AT+CMMSCID                     /* Set the Network Parameters for MMS */
"AT+CMMSSENDCFG                 /* Set the Parameters for Sending MMS */
"AT+CMMSEDIT                    /* Enter or Exit Edit Mode */
"AT+CMMSDOWN                    /* Download the File Data or Title from UART */
"AT+CMMSDELFILE                 /* Delete the File of the Edited MMS by File Index */
"AT+CMMSSEND                    /* Start MMS Sending */
"AT+CMMSRECP                    /* Add Recipients */
"AT+CMMSCC                      /* Add Copy Recipients */
"AT+CMMSBCC                     /* Add Secret Recipients */
"AT+CMMSDELRECP                 /* Delete Recipients */
"AT+CMMSDELCC                   /* Delete Copy Recipients  */
"AT+CMMSDELBCC                  /* Delete Secret Recipients */
"AT+CMMSRECV                    /* Receive MMS */
"AT+CMMSVIEW                    /* Get the MMS into Buffer and Show the Information. */
"AT+CMMSREAD                    /* Read the Given File of the MMS in the Buffer. */
"AT+CMMSRDPUSH                  /* Read the Information of the MMS PUSH Message. */
"AT+CMMSUA                      /* Set User Agent  */
"AT+CMMSPROFILE                 /* Set User Agent Profile  */
"AT+CMMSTIMEOUT                 /* Set MMS Timeout */
"AT+CMMSSTATUS                  /* Get MMS Status  */
"AT+CMMSINIT                    /* Initialize MMS Function */
"AT+CMMSTERM                    /* Exit MMS Function */
"AT+CMMSSCONT                   /* Save MMS Context */
"                               
"AT+DDET DTMF                   /* Detection Control */
"                                
"AT+CREC                        /* Record Operation */
"AT+CRECORD                     /* Record and Send Data to UART */
"                               
"AT+CTTS                        /* TTS Operation */
"AT+CTTSPARAM                   /* Set Parameters of the TTS Playing */
"AT+CTTSRING                    /* Enable/Disable TTS Play During Incoming Call Ring */
};   
/*********************************************************************************
公共变量定义区
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
私有变量定义区
*********************************************************************************/
//static unsigned char  GprsStop_time = 0;
MG2618DescriptorStruct MG2618Infr;
/*********************************************************************************
测试变量定义区
*********************************************************************************/

/*********************************************************************************
内部函数定义区
*********************************************************************************/
static SystemErrName CommandSerialization_MG2618(CommandDescriptor_MG2618 *Command);
static SystemErrName ZPNUM_PramFill(CommandDescriptor_MG2618 *Command);
static SystemErrName TcpUdpConnect_PramFill(CommandDescriptor_MG2618 *buff);
static SystemErrName UpdSend_PramFill(CommandDescriptor_MG2618 *buff);
static SystemErrName TcpSend_PramFill(CommandDescriptor_MG2618 *buff);
static SystemErrName TcpUdp_CSR_PramFill(CommandDescriptor_MG2618 *buff);
static void AckTimeOut_MG2618 (void *p_tmr, void *p_arg);
static SystemErrName Default_ACK_Func(unsigned char *data,unsigned short length);
static void TcpClose_ACK(DataBuffSpec_Struct *Data);
//static unsigned short MatchinString (unsigned char **data, const char **StringList, unsigned short ListSize);
//static unsigned char* AddIntToString(unsigned char* tp, unsigned long vlue);
static unsigned char GetData_TCP_UDP(DataBuffSpec_Struct *Data);
//应答呼叫
#if (0 == ATA_Command_EN )
  #define AT_Again_ACK_Func   P_NULL
# else
  #define AT_Again_ACK_Func   P_NULL   
  //SystemErrName AT_Again_ACK_Func_ACK_Func(unsigned char *data, unsigned short length);
#endif
//应答呼叫
#if (0 == ATA_Command_EN )
  #define ATA_ACK_Func   P_NULL
# else
  SystemErrName ATA_ACK_Func(unsigned char *data, unsigned short length);
#endif
//发起呼叫
#if (0 == ATD_Command_EN )
#define ATD_ACK_Func   P_NULL
# else
SystemErrName ATD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//拨打最近呼出的电话号码 
#if (0 == ATDL_Command_EN )
#define ATDL_ACK_Func   P_NULL
# else
SystemErrName ATDL_ACK_Func(unsigned char *data, unsigned short length);
#endif
//使能回馈通道
#if (0 == ATE_Command_EN )
#define ATE_ACK_Func   P_NULL
# else
SystemErrName ATE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//断开当前连接
#if (0 == ATH_Command_EN )
#define ATH_ACK_Func   P_NULL
# else
SystemErrName ATH_ACK_Func(unsigned char *data, unsigned short length);
#endif
//提示模块生产厂家信息
#if (0 == ATI_Command_EN )
#define ATI_ACK_Func   P_NULL
# else
SystemErrName ATI_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置是否在终端回显返回值
#if (0 == ATQ_Command_EN )
#define ATQ_ACK_Func   P_NULL
# else
SystemErrName ATQ_ACK_Func(unsigned char *data, unsigned short length);
#endif
//从数据模式切换到命令模式 
#if (0 == AtData_Command_EN )
#define AtData_ACK_Func   P_NULL
# else
SystemErrName AtData_ACK_Func(unsigned char *data, unsigned short length);
#endif
//从命令模式切换到数据模式
#if (0 == ATO_Command_EN )
#define ATO_ACK_Func   P_NULL
# else
SystemErrName ATO_ACK_Func(unsigned char *data, unsigned short length); 
#endif
//使用脉冲拨号
#if (0 == ATP_Command_EN )
#define ATP_ACK_Func   P_NULL
# else
SystemErrName ATP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//自动应答设置
#if (0 == ATS0_Command_EN )
#define ATS0_ACK_Func   P_NULL
# else
SystemErrName ATS0_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置来电类型
#if (0 == AT_CRC_Command_EN )
#define AT_CRC_ACK_Func   P_NULL
# else
SystemErrName AT_CRC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//受话音量
#if (0 == CLVL_Command_EN )
#define CLVL_ACK_Func   P_NULL
# else
SystemErrName CLVL_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置来电号码显示
#if (0 == CLIP_Command_EN )
#define CLIP_ACK_Func   P_NULL
# else
SystemErrName CLIP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//静音控制
#if (0 == ZSETMUTE_Command_EN )
#define ZSETMUTE_ACK_Func   P_NULL
# else
SystemErrName ZSETMUTE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询国际 ID 号
#if (0 == CIMI_Command_EN )
#define CIMI_ACK_Func   P_NULL
# else
SystemErrName CIMI_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取产品版本号
#if (0 == CGMR_Command_EN )
#define CGMR_ACK_Func   P_NULL
# else
SystemErrName CGMR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//回音消除控制
#if (0 == ECHO_Command_EN)
#define ECHO_ACK_Func   P_NULL
# else
SystemErrName ECHO_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取当前 IMEI 号
#if (0 == GSN_Command_EN)
#define GSN_ACK_Func   P_NULL
# else
SystemErrName GSN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取当前软件版本号
#if (0 == ZVERS_Command_EN)
#define ZVERS_ACK_Func   P_NULL
# else
SystemErrName ZVERS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//功能锁定
#if (0 == CLCK_Command_EN)
#define CLCK_ACK_Func   P_NULL
# else
SystemErrName CLCK_ACK_Func(unsigned char *data, unsigned short length);
#endif
//呼叫转移号码和条件设置

#if (0 == CCFC_Command_EN)
#define CCFC_ACK_Func   P_NULL
# else
SystemErrName CCFC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//呼叫等待控制
#if (0 == CCWA_Command_EN)
#define CCWA_ACK_Func   P_NULL
# else
SystemErrName CCWA_ACK_Func(unsigned char *data, unsigned short length);
#endif
//呼叫保持和多方会议
#if (0 == CHLD_Command_EN)
#define CHLD_ACK_Func   P_NULL
# else
SystemErrName CHLD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询 SIM 卡状态
#if (0 == TSIMINS_Command_EN)
#define TSIMINS_ACK_Func   P_NULL
# else
SystemErrName TSIMINS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//修改密码
#if (0 == CPWD_Command_EN)
#define CPWD_ACK_Func   P_NULL
# else
SystemErrName CPWD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询模块厂家
#if (0 == CGMI_Command_EN)
#define CGMI_ACK_Func   P_NULL
# else
SystemErrName CGMI_ACK_Func(unsigned char *data, unsigned short length);
#endif
//字符集种类的选择
#if (0 == CSCS_Command_EN)
#define CSCS_ACK_Func   P_NULL
# else
SystemErrName CSCS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//呼叫状态查询
#if (0 == CLCC_Command_EN)
#define CLCC_ACK_Func   P_NULL
# else
SystemErrName CLCC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//发送 DTMF 音
#if (0 == VTS_Command_EN)
#define VTS_ACK_Func   P_NULL
# else
SystemErrName VTS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//网络注册和漫游
#if (0 == CREG_Command_EN)
#define CREG_ACK_Func   P_NULL
# else
//网络服务指令 
SystemErrName CREG_ACK_Func(unsigned char *data, unsigned short length);
#endif
//网络选择
#if (0 == COPS_Command_EN)
#define COPS_ACK_Func   P_NULL
# else
SystemErrName COPS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//模块状态查询
#if (0 == CPAS_Command_EN)
#define CPAS_ACK_Func   P_NULL
# else
SystemErrName CPAS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置模块功能
#if (0 == CFUN_Command_EN)
#define CFUN_ACK_Func   P_NULL
# else
SystemErrName CFUN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//移动设备错误报告
#if (0 == CMEE_Command_EN)
#define CMEE_ACK_Func   P_NULL
# else
SystemErrName CMEE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//模块关机
#if (0 == ZPWROFF_Command_EN)
#define ZPWROFF_ACK_Func   P_NULL
# else
SystemErrName ZPWROFF_ACK_Func(unsigned char *data, unsigned short length);
#endif
//输入 PIN 码
#if (0 == CPIN_Command_EN)
#define CPIN_ACK_Func   P_NULL
# else
SystemErrName CPIN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//输入 PIN1 码
#if (0 == EPIN1_Command_EN)
#define EPIN1_ACK_Func   P_NULL
# else
SystemErrName EPIN1_ACK_Func(unsigned char *data, unsigned short length);
#endif
//输入 PIN2 码
#if (0 == EPIN2_Command_EN)
#define EPIN2_ACK_Func   P_NULL
# else
SystemErrName EPIN2_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询 PIN1， PIN2 剩余输入次数
#if (0 == EPINC_Command_EN)
#define EPINC_ACK_Func   P_NULL
# else
SystemErrName EPINC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//信号强度查询
#if (0 == CSQ_Command_EN)
#define CSQ_ACK_Func   P_NULL
# else
SystemErrName CSQ_ACK_Func(unsigned char *data, unsigned short length);
#endif
//时钟管理
#if (0 == CCLK_Command_EN)
#define CCLK_ACK_Func   P_NULL
# else
SystemErrName CCLK_ACK_Func(unsigned char *data, unsigned short length);
#endif
//短信中心号码
#if (0 == CSCA_Command_EN)
#define CSCA_ACK_Func   P_NULL
# else
SystemErrName CSCA_ACK_Func(unsigned char *data, unsigned short length);
#endif
//短信确认
#if (0 == CNMA_Command_EN)
#define CNMA_ACK_Func   P_NULL
# else
SystemErrName CNMA_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置短消息模式
#if (0 == CMGF_Command_EN)
#define CMGF_ACK_Func   P_NULL
# else
SystemErrName CMGF_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置短信指示格式
#if (0 == CNMI_Command_EN)
#define CNMI_ACK_Func   P_NULL
# else
SystemErrName CNMI_ACK_Func(unsigned char *data, unsigned short length);
#endif
//读短消息
#if (0 == CMGW_Command_EN)
#define CMGR_ACK_Func   P_NULL
# else
SystemErrName CMGR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//写短消息
#if (0 == CMGW_Command_EN)
#define CMGW_ACK_Func   P_NULL
# else
SystemErrName CMGW_ACK_Func(unsigned char *data, unsigned short length);
#endif
//选择短信服务
#if (0 == CSMS_Command_EN)
#define CSMS_ACK_Func   P_NULL
# else
SystemErrName CSMS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//发送短信
#if (0 == CMGS_Command_EN)
#define CMGS_ACK_Func   P_NULL
# else
SystemErrName CMGS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//首选短消息存储器
#if (0 == CPMS_Command_EN)
#define CPMS_ACK_Func   P_NULL
# else
SystemErrName CPMS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//删除短信
#if (0 == CMGD_Command_EN)
#define CMGD_ACK_Func   P_NULL
# else
SystemErrName CMGD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//短消息列表
#if (0 == CMGL_Command_EN)
#define CMGL_ACK_Func   P_NULL
# else
SystemErrName CMGL_ACK_Func(unsigned char *data, unsigned short length);
#endif
//发送存贮的短信
#if (0 == CMSS_Command_EN)
#define CMSS_ACK_Func   P_NULL
# else
SystemErrName CMSS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//选择电话本存储
#if (0 == CPBS_Command_EN)
#define CPBS_ACK_Func   P_NULL
# else 
SystemErrName CPBS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//读取电话本信息
#if (0 == CPBR_Command_EN)
#define CPBR_ACK_Func   P_NULL
# else
SystemErrName CPBR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//向电话本写信息
#if (0 == CPBW_Command_EN)
#define CPBW_ACK_Func   P_NULL
# else
SystemErrName CPBW_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查找电话本信息
#if (0 == CPBF_Command_EN)
#define CPBF_ACK_Func   P_NULL
# else
SystemErrName CPBF_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取本机号码
#if (0 == CNUM_Command_EN)
#define CNUM_ACK_Func   P_NULL
# else
SystemErrName CNUM_ACK_Func(unsigned char *data, unsigned short length);
#endif
//流控制
#if (0 == IFC_Command_EN)
#define IFC_ACK_Func   P_NULL
# else
SystemErrName IFC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 DTR 模式
#if (0 == DTR_Command_EN)
#define DTR_ACK_Func   P_NULL
# else
SystemErrName DTR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 DCD 模式
#if (0 == DCD_Command_EN)
#define DCD_ACK_Func   P_NULL
# else
SystemErrName DCD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设定模块波特率
#if (0 == IPR_Command_EN)
#define IPR_ACK_Func   P_NULL
# else
SystemErrName IPR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//恢复出厂设置
#if (0 == AT_F_Command_EN)
#define AT_F_ACK_Func   P_NULL
# else
SystemErrName AT_F_ACK_Func(unsigned char *data, unsigned short length);
#endif
//保存设置
#if (0 == AT_W_Command_EN)
#define AT_W_ACK_Func   P_NULL
# else
SystemErrName AT_W_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 PDP 格式
#if (0 == CGDCONT_Command_EN)
#define CGDCONT_ACK_Func   P_NULL
# else
SystemErrName CGDCONT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//激活/解除 PDP 设置
#if (0 == CGACT_Command_EN)
#define CGACT_ACK_Func   P_NULL
# else
SystemErrName CGACT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 GPRS 业务
#if (0 == CGATT_Command_EN)
#define CGATT_ACK_Func   P_NULL
# else
SystemErrName CGATT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//GPRS 设备等级
#if (0 == CGCLASS_Command_EN)
#define CGCLASS_ACK_Func   P_NULL
# else
SystemErrName CGCLASS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询模块状态信息
#if (0 == ZSTR_Command_EN)
#define ZSTR_ACK_Func   P_NULL
# else
SystemErrName ZSTR_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取 ICCID
#if (0 == ZGETICCID_Command_EN)
#define ZGETICCID_ACK_Func   P_NULL
# else
SystemErrName ZGETICCID_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置自动显示 CSQ 的条件
#if (0 == ZCSQ_Command_EN)
#define ZCSQ_ACK_Func   P_NULL
# else
SystemErrName ZCSQ_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 DTR 的检测模式
#if (0 == ZEDT_Command_EN)
#define ZEDT_ACK_Func   P_NULL
# else
SystemErrName ZEDT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//32KHz 深度睡眠模式
#if (0 == ZDSLEEP_Command_EN)
#define ZDSLEEP_ACK_Func   P_NULL
# else
SystemErrName ZDSLEEP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//发送 USSD 数据
#if (0 == CUSD_Command_EN)
#define CUSD_ACK_Func   P_NULL
# else
SystemErrName CUSD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 RING 引脚的信号模式
#if (0 == ZRINGPINMODE_Command_EN)
#define ZRINGPINMODE_ACK_Func   P_NULL
# else
SystemErrName ZRINGPINMODE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//定制版本切换
#if (0 == ZVERSWITCH_Command_EN)
#define ZVERSWITCH_ACK_Func   P_NULL
# else
SystemErrName ZVERSWITCH_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 APN，用户名，密码 
#if (0 == ZPNUM_Command_EN)
#define ZPNUM_ACK_Func   P_NULL
# else
SystemErrName ZPNUM_ACK_Func(unsigned char *data, unsigned short length);
#endif
//打开 GPRS 数据连接 
#if (0 == ZPPPOPEN_Command_EN)
#define ZPPPOPEN_ACK_Func   P_NULL
# else
SystemErrName ZPPPOPEN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//关闭 GPRS 数据连接
#if (0 == ZPPPCLOSE_Command_EN)
#define ZPPPCLOSE_ACK_Func   P_NULL
# else
SystemErrName ZPPPCLOSE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询当前模块 IP 地址值
#if (0 == ZIPGETIP_Command_EN)
#define ZIPGETIP_ACK_Func   P_NULL
# else
SystemErrName ZIPGETIP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 DNS 服务器的 IP 地址 
#if (0 == ZDNSSERV_Command_EN)
#define ZDNSSERV_ACK_Func   P_NULL
# else
SystemErrName ZDNSSERV_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取对应 Internet 域名的 IP 地址
#if (0 == ZDNSGETIP_Command_EN)
#define ZDNSGETIP_ACK_Func   P_NULL
# else
SystemErrName ZDNSGETIP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//建立 TCP 服务器链接
#if (0 == ZIPSETUP_Command_EN)
#define ZIPSETUP_ACK_Func   P_NULL
# else 
SystemErrName ZIPSETUP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//向目标地址发送 TCP 数据 
#if (0 == ZIPSEND_Command_EN)
#define ZIPSEND_ACK_Func   P_NULL
# else
SystemErrName ZIPSEND_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询 GPRS 连接状态 
#if (0 == ZPPPSTATUS_Command_EN)
#define ZPPPSTATUS_ACK_Func   P_NULL
# else
SystemErrName ZPPPSTATUS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//关闭 TCP 链接
#if (0 == ZIPCLOSE_Command_EN)
#define ZIPCLOSE_ACK_Func   P_NULL
# else
SystemErrName ZIPCLOSE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询当前 TCP 连接状态
#if (0 == ZIPSTATUS_Command_EN)
#define ZIPSTATUS_ACK_Func   P_NULL
# else
SystemErrName ZIPSTATUS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//建立 UDP 服务器链接 
#if (0 == ZIPSETUPU_Command_EN)
#define ZIPSETUPU_ACK_Func   P_NULL
# else
SystemErrName ZIPSETUPU_ACK_Func(unsigned char *data, unsigned short length);
#endif
//向 UDP 服务器发送数据
#if (0 == ZIPSENDU_Command_EN)
#define ZIPSENDU_ACK_Func   P_NULL
# else
SystemErrName ZIPSENDU_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询 UDP 状态
#if (0 == ZIPSTATUSU_Command_EN)
#define ZIPSTATUSU_ACK_Func   P_NULL
# else
SystemErrName ZIPSTATUSU_ACK_Func(unsigned char *data, unsigned short length);
#endif
//关闭 UDP 链接
#if (0 == ZIPCLOSEU_Command_EN)
#define ZIPCLOSEU_ACK_Func   P_NULL
# else
SystemErrName ZIPCLOSEU_ACK_Func(unsigned char *data, unsigned short length);
#endif
//端口监听功能设置 
#if (0 == ZTCPLISTEN_Command_EN)
#define ZTCPLISTEN_ACK_Func   P_NULL
# else
SystemErrName ZTCPLISTEN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//在被动打开的链路上发送数据
#if (0 == ZTCPSENDP_Command_EN)
#define ZTCPSENDP_ACK_Func   P_NULL
# else
SystemErrName ZTCPSENDP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//关闭监听到的连接
#if (0 == ZTCPCLOSEP_Command_EN)
#define ZTCPCLOSEP_ACK_Func   P_NULL
# else
SystemErrName ZTCPCLOSEP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//查询被动打开链路
#if (0 == ZTCPSTATUSP_Command_EN)
#define ZTCPSTATUSP_ACK_Func   P_NULL
# else
SystemErrName ZTCPSTATUSP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置模块连接服务器和发送数据的超时时间
#if (0 == ZIPTIMEOUT_Command_EN)
#define ZIPTIMEOUT_ACK_Func   P_NULL
# else
SystemErrName ZIPTIMEOUT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//端口监听功能设置
#if (0 == ZUDPLISTEN_Command_EN)
#define ZUDPLISTEN_ACK_Func   P_NULL
# else
SystemErrName ZUDPLISTEN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//在被动打开的链路上发送数据
#if (0 == ZUDPSENDP_Command_EN)
#define ZUDPSENDP_ACK_Func   P_NULL
# else
SystemErrName ZUDPSENDP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//登录 FTP 服务器
#if (0 == ZFTPLOGIN_Command_EN)
#define ZFTPLOGIN_ACK_Func   P_NULL
# else
SystemErrName ZFTPLOGIN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 FTP 文件类型
#if (0 == ZFTPTYPE_Command_EN)
#define ZFTPTYPE_ACK_Func   P_NULL
# else
SystemErrName ZFTPTYPE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//上传文件
#if (0 == ZFTPUPLOAD_Command_EN)
#define ZFTPUPLOAD_ACK_Func   P_NULL
# else
SystemErrName ZFTPUPLOAD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取文件大小
#if (0 == ZFTPSIZE_Command_EN)
#define ZFTPSIZE_ACK_Func   P_NULL
# else
SystemErrName ZFTPSIZE_ACK_Func(unsigned char *data, unsigned short length); 
#endif
//下载文件
#if (0 == ZFTPDNLOAD_Command_EN)
#define ZFTPDNLOAD_ACK_Func   P_NULL
# else
SystemErrName ZFTPDNLOAD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//下载 FTP 文件
#if (0 == ZFTPDNLOADEX_Command_EN)
#define ZFTPDNLOADEX_ACK_Func   P_NULL
# else
SystemErrName ZFTPDNLOADEX_ACK_Func(unsigned char *data, unsigned short length);
#endif
//删除文件
#if (0 == ZFTPDEL_Command_EN)
#define ZFTPDEL_ACK_Func   P_NULL
# else
SystemErrName ZFTPDEL_ACK_Func(unsigned char *data, unsigned short length);
#endif
//退出 FTP
#if (0 == ZFTPQUIT_Command_EN)
#define ZFTPQUIT_ACK_Func   P_NULL
# else
SystemErrName ZFTPQUIT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//透明传输
#if (0 == ZTRANSFER_Command_EN)
#define ZTRANSFER_ACK_Func   P_NULL
# else
SystemErrName ZTRANSFER_ACK_Func(unsigned char *data, unsigned short length);
#endif
//摘机音设置
#if (0 == ZCALLTONE_Command_EN)
#define ZCALLTONE_ACK_Func   P_NULL
# else
SystemErrName ZCALLTONE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//ZDTMF 拨号音设置 
#if (0 == ZDTMFTONE_Command_EN)
#define ZDTMFTONE_ACK_Func   P_NULL
# else
SystemErrName ZDTMFTONE_ACK_Func(unsigned char *data, unsigned short length);
#endif
//打开 DTMF 检测功能
#if (0 == ZKTDSWITCH_Command_EN)
#define ZKTDSWITCH_ACK_Func   P_NULL
# else
SystemErrName ZKTDSWITCH_ACK_Func(unsigned char *data, unsigned short length);
#endif
//主动上报检测的 DTMF 按键
#if (0 == ZKTDIND_Command_EN)
#define ZKTDIND_ACK_Func   P_NULL
# else
SystemErrName ZKTDIND_ACK_Func(unsigned char *data, unsigned short length);
#endif
//语音通道切换指令 
#if (0 == SPEAKER_Command_EN)
#define SPEAKER_ACK_Func   P_NULL
# else
SystemErrName SPEAKER_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 MIC 音频参数
#if (0 == ZMICGB_Command_EN)
#define ZMICGB_ACK_Func   P_NULL
# else
SystemErrName ZMICGB_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取小区信息
#if (0 == CCED_Command_EN)
#define CCED_ACK_Func   P_NULL
# else
SystemErrName CCED_ACK_Func(unsigned char *data, unsigned short length);
#endif
//锁定 BCCH 信道（暂不支持）
#if (0 == ZBCCH_Command_EN)
#define ZBCCH_ACK_Func   P_NULL
# else
SystemErrName ZBCCH_ACK_Func(unsigned char *data, unsigned short length);
#endif
//锁定 GSM 频段 
#if (0 == ZBAND_Command_EN)
#define ZBAND_ACK_Func   P_NULL
# else
SystemErrName ZBAND_ACK_Func(unsigned char *data, unsigned short length);
#endif
//锁定网络运营商
#if (0 == ZOPT_Command_EN)
#define ZOPT_ACK_Func   P_NULL
# else
SystemErrName ZOPT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//读取载波列表
#if (0 == ZCALIST_Command_EN)
#define ZCALIST_ACK_Func   P_NULL
# else
SystemErrName ZCALIST_ACK_Func(unsigned char *data, unsigned short length);
#endif
//TTS(Text to Speech)语音播报业务
#if (0 == ZTTS_Command_EN)
#define ZTTS_ACK_Func   P_NULL
# else
SystemErrName ZTTS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//设置 TTS(Text to Speech)语音播报参数(暂不支持切换发音人)
#if (0 == ZTTSP_Command_EN)
#define ZTTSP_ACK_Func   P_NULL
# else
SystemErrName ZTTSP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//录音功能
#if (0 == ZAUDREC_Command_EN)
#define ZAUDREC_ACK_Func   P_NULL
# else
SystemErrName ZAUDREC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//读取文件
#if (0 == ZFILEREAD_Command_EN)
#define ZFILEREAD_ACK_Func   P_NULL
# else
SystemErrName ZFILEREAD_ACK_Func(unsigned char *data, unsigned short length);
#endif
//开关 GPS 指令
#if (0 == ZGPSOPEN_Command_EN)
#define ZGPSOPEN_ACK_Func   P_NULL
# else
SystemErrName ZGPSOPEN_ACK_Func(unsigned char *data, unsigned short length);
#endif
//GPS 休眠指令 
#if (0 == ZGPSSLEEP_Command_EN)
#define ZGPSSLEEP_ACK_Func   P_NULL
# else
SystemErrName ZGPSSLEEP_ACK_Func(unsigned char *data, unsigned short length);
#endif
//NMEA 上报控制指令
#if (0 == ZGPSNMEARPT_Command_EN)
#define ZGPSNMEARPT_ACK_Func   P_NULL
# else
SystemErrName ZGPSNMEARPT_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取 GPS 定位经纬度指令
#if (0 == ZGPSLOC_Command_EN)
#define ZGPSLOC_ACK_Func   P_NULL
# else
SystemErrName ZGPSLOC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取 GPS 时间指令
#if (0 == ZGPSUTC_Command_EN)
#define ZGPSUTC_ACK_Func   P_NULL
# else
SystemErrName ZGPSUTC_ACK_Func(unsigned char *data, unsigned short length);
#endif
//获取角度速度指令
#if (0 == ZGPSHS_Command_EN)
#define ZGPSHS_ACK_Func   P_NULL
# else
SystemErrName ZGPSHS_ACK_Func(unsigned char *data, unsigned short length);
#endif
//GPS 定位状态上报
#if (0 == ZGPSIND_Command_EN)
#define ZGPSIND_ACK_Func   P_NULL
# else
SystemErrName ZGPSIND_ACK_Func(unsigned char *data, unsigned short length);
#endif
typedef SystemErrName (*Command_ACK_Func) (unsigned char *data, unsigned short length);
const Command_ACK_Func Command_ACK_FuncList_MG2618[] = { 
//2.1 普通指令
 AT_Again_ACK_Func,  //= 0,   //该指令用于重复上一条指令
 ATA_ACK_Func,       //= 1,   //应答呼叫
 ATD_ACK_Func,       //= 2,   //发起呼叫
 ATDL_ACK_Func,      //= 3,   //拨打最近呼出的电话号码 
 ATE_ACK_Func,       //= 4,   //使能回馈通道
 ATH_ACK_Func,       //= 5,   //断开当前连接
 ATI_ACK_Func,       //= 6,   //提示模块生产厂家信息 
 ATQ_ACK_Func,       //= 7,   //设置是否在终端回显返回值
 AtData_ACK_Func,    //= 8,   //从数据模式切换到命令模式 
 ATO_ACK_Func,       //= 9,   //从命令模式切换到数据模式 
 ATP_ACK_Func,       //= 10,  //使用脉冲拨号
 ATS0_ACK_Func,      //= 11,  //自动应答设置
 AT_CRC_ACK_Func,    //= 12,  //设置来电类型
 CLVL_ACK_Func,      //= 13,  //受话音量
 CLIP_ACK_Func,      //= 14,  //设置来电号码显示
 ZSETMUTE_ACK_Func,  //= 15,  //静音控制
 CIMI_ACK_Func,      //= 16,  //查询国际 ID 号
 CGMR_ACK_Func,      //= 17,  //获取产品版本号
 ECHO_ACK_Func,      //= 18,  //回音消除控制
 GSN_ACK_Func,       //= 19,  //获取当前 IMEI 号
 ZVERS_ACK_Func,     //= 20,  //获取当前软件版本号
 CLCK_ACK_Func,      //= 21,  //功能锁定
 CCFC_ACK_Func,      //= 22,  //呼叫转移号码和条件设置 
 CCWA_ACK_Func,      //= 23,  //呼叫等待控制
 CHLD_ACK_Func,      //= 24,  //呼叫保持和多方会议
 TSIMINS_ACK_Func,   //= 25,  //查询 SIM 卡状态
 CPWD_ACK_Func,      //= 26,  //修改密码
 CGMI_ACK_Func,      //= 27,  //查询模块厂家
 CSCS_ACK_Func,      //= 28,  //字符集种类的选择
 CLCC_ACK_Func,      //= 29,  //呼叫状态查询
//DTMF指令
 VTS_ACK_Func,     //= 30,  //发送 DTMF 音
//网络服务指令 
 CREG_ACK_Func,    //= 31,  //网络注册和漫游
 COPS_ACK_Func,    //= 32,  //网络选择
//移动设备控制和状态报告
 CPAS_ACK_Func,    //= 33,  //模块状态查询
 CFUN_ACK_Func,    //= 34,  //设置模块功能
 CMEE_ACK_Func,    //= 35,  //移动设备错误报告
 ZPWROFF_ACK_Func, //= 36,  //模块关机
 CPIN_ACK_Func,    //= 37,  //输入 PIN 码
 EPIN1_ACK_Func,   //= 38,  //输入 PIN1 码
 EPIN2_ACK_Func,   //= 39,  //输入 PIN2 码
 EPINC_ACK_Func,   //= 40,  //查询 PIN1， PIN2 剩余输入次数
 CSQ_ACK_Func,     //= 41,  //信号强度查询
 CCLK_ACK_Func,    //= 42,  //时钟管理
// 消息服务指令 
 CSCA_ACK_Func,    //= 43,    //短信中心号码
 CNMA_ACK_Func,    //= 44,    //短信确认
 CMGF_ACK_Func,    //= 45,    //设置短消息模式
 CNMI_ACK_Func,    //= 46,    //设置短信指示格式
 CMGR_ACK_Func,    //= 47,    //读短消息
 CMGW_ACK_Func,    //= 48,    //写短消息
 CSMS_ACK_Func,    //= 49,    //选择短信服务
 CMGS_ACK_Func,    //= 50,    //发送短信
 CPMS_ACK_Func,    //= 51,    //首选短消息存储器
 CMGD_ACK_Func,    //= 52,    //删除短信
 CMGL_ACK_Func,    //= 53,    //短消息列表
 CMSS_ACK_Func,    //= 54,    //发送存贮的短信
//电话本指令 
 CPBS_ACK_Func,    //= 55,    //选择电话本存储
 CPBR_ACK_Func,    //= 56,    //读取电话本信息
 CPBW_ACK_Func,    //= 57,    //向电话本写信息
 CPBF_ACK_Func,    //= 58,    //查找电话本信息
 CNUM_ACK_Func,    //= 59,    //获取本机号码
//数据压缩指令 
 IFC_ACK_Func,     //= 60,    //流控制
 DTR_ACK_Func,     //= 61,    //设置 DTR 模式
 DCD_ACK_Func,     //= 62,    //设置 DCD 模式
 IPR_ACK_Func,     //= 63,    //设定模块波特率
 AT_F_ACK_Func,      //= 64,    //恢复出厂设置
 AT_W_ACK_Func,       //= 65,   //保存设置
//GPRS指令
 CGDCONT_ACK_Func, //= 66,    //设置 PDP 格式
 CGACT_ACK_Func,   //= 67,    //激活/解除 PDP 设置
 CGATT_ACK_Func,   //= 68,    //设置 GPRS 业务 
 CGCLASS_ACK_Func, //= 69,    //GPRS 设备等级
//ZTE 特有指令
 ZSTR_ACK_Func,         // = 70,    //查询模块状态信息
 ZGETICCID_ACK_Func,    // = 71,    //获取 ICCID
 ZCSQ_ACK_Func,         // = 72,    //设置自动显示 CSQ 的条件 
 ZEDT_ACK_Func,         // = 73,    //设置 DTR 的检测模式 
 ZDSLEEP_ACK_Func,      // = 74,    //32KHz 深度睡眠模式
 CUSD_ACK_Func,         // = 75,    //发送 USSD 数据
 ZRINGPINMODE_ACK_Func, // = 76,    //设置 RING 引脚的信号模式 
 ZVERSWITCH_ACK_Func,    //= 77,    //定制版本切换
// 网络参数指令 
 ZPNUM_ACK_Func,         //= 78,    //设置 APN，用户名，密码 
 ZPPPOPEN_ACK_Func,      //= 79,    //打开 GPRS 数据连接 
 ZPPPCLOSE_ACK_Func,     //= 80,    //关闭 GPRS 数据连接 
 ZIPGETIP_ACK_Func,      //= 81,    //查询当前模块 IP 地址值 
 ZDNSSERV_ACK_Func,     // = 82,    //设置 DNS 服务器的 IP 地址 
 ZDNSGETIP_ACK_Func,    // = 83,    //获取对应 Internet 域名的 IP 地址 
//TCP 连接指令 
 ZIPSETUP_ACK_Func,     // = 84,   //建立 TCP 服务器链接
 ZIPSEND_ACK_Func,       //= 85,   //向目标地址发送 TCP 数据 
 ZPPPSTATUS_ACK_Func,    //= 86,   //查询 GPRS 连接状态 
 ZIPCLOSE_ACK_Func,      //= 87,   //关闭 TCP 链接
 ZIPSTATUS_ACK_Func,     //= 88,   //查询当前 TCP 连接状态 
// UDP 连接指令 
 ZIPSETUPU_ACK_Func,     //= 89,  //建立 UDP 服务器链接 
 ZIPSENDU_ACK_Func,      //= 90,  //向 UDP 服务器发送数据 
 ZIPSTATUSU_ACK_Func,    //= 91,  //查询 UDP 状态
 ZIPCLOSEU_ACK_Func,     //= 92,  //关闭 UDP 链接
// SERVER 指令 
 ZTCPLISTEN_ACK_Func,   /// = 93,  //端口监听功能设置 
 ZTCPSENDP_ACK_Func,     //= 94,  //在被动打开的链路上发送数据 
 ZTCPCLOSEP_ACK_Func,    //= 95,  //关闭监听到的连接
 ZTCPSTATUSP_ACK_Func,   //= 96,  //查询被动打开链路
 ZIPTIMEOUT_ACK_Func,    //= 97,  //设置模块连接服务器和发送数据的超时时间
 ZUDPLISTEN_ACK_Func,    //= 98,  //端口监听功能设置
 ZUDPSENDP_ACK_Func,     //= 99,  //在被动打开的链路上发送数据
// FTP 指令
 ZFTPLOGIN_ACK_Func,     //= 100, //登录 FTP 服务器
 ZFTPTYPE_ACK_Func,      //= 101, //设置 FTP 文件类型
 ZFTPUPLOAD_ACK_Func,    //= 102, //上传文件
 ZFTPSIZE_ACK_Func,      //= 103, //获取文件大小
 ZFTPDNLOAD_ACK_Func,    //= 104, //下载文件
 ZFTPDNLOADEX_ACK_Func,  //= 105, //下载 FTP 文件
 ZFTPDEL_ACK_Func,       //= 106, //删除文件
 ZFTPQUIT_ACK_Func,      //= 107, //退出 FTP 
// 透明传输指令 
 ZTRANSFER_ACK_Func,     //= 108, //透明传输
//音频相关指令 
 ZCALLTONE_ACK_Func,     //= 109, //摘机音设置
 ZDTMFTONE_ACK_Func,    // = 110, //ZDTMF 拨号音设置 
 ZKTDSWITCH_ACK_Func,    //= 111, //打开 DTMF 检测功能
 ZKTDIND_ACK_Func,       //= 112, //主动上报检测的 DTMF 按键
 SPEAKER_ACK_Func,       //= 113, //语音通道切换指令 
 ZMICGB_ACK_Func,        //= 114, //设置 MIC 音频参数
//基站相关指令(直放站需求指令) 
 CCED_ACK_Func,          //= 115, //获取小区信息
 ZBCCH_ACK_Func,         //= 116, //锁定 BCCH 信道（暂不支持）
 ZBAND_ACK_Func,         //= 117, //锁定 GSM 频段 
 ZOPT_ACK_Func,          //= 118, //锁定网络运营商
 ZCALIST_ACK_Func,       //= 119, //读取载波列表
// TTS 相关指令
 ZTTS_ACK_Func,          //= 120, //TTS(Text to Speech)语音播报业务 
 ZTTSP_ACK_Func,         //= 121, //设置 TTS(Text to Speech)语音播报参数(暂不支持切换发音人)
// 录音相关指令
 ZAUDREC_ACK_Func,       //= 122, //录音功能
 ZFILEREAD_ACK_Func,     //= 123, //读取文件
// GPS 相关指令 
 ZGPSOPEN_ACK_Func,      //= 124, //开关 GPS 指令
 ZGPSSLEEP_ACK_Func,    // = 125, //GPS 休眠指令 
 ZGPSNMEARPT_ACK_Func,  // = 126, //NMEA 上报控制指令 
 ZGPSLOC_ACK_Func,      // = 127, //获取 GPS 定位经纬度指令 
 ZGPSUTC_ACK_Func,      // = 128, //获取 GPS 时间指令
 ZGPSHS_ACK_Func,       // = 129, //获取角度速度指令
 ZGPSIND_ACK_Func      // = 130,  //GPS 定位状态上报 
 //NC_Command_MG2618 = 255 
};
/*********************************************************************************
功能代码定义区
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                // 
 Output:        //
 Return:        //
 Others:        //
                  
*********************************************************************************/
SystemErrName SendCmd_MG2618(CommandDescriptor_MG2618 *Command,
                             GPRS_FinishCallBack CallBack,
                             unsigned char opt,
                             unsigned short timer)
{
  SystemErrName  err;
  SystemErrName  err2;  
  if(P_NULL == Command_ACK_FuncList_MG2618[Command->Cmd])
  {
   if(AT_Again != Command->Cmd)
   {return CmdValid;}
  }
  if(OFF == MG2618Infr.ModInf.Status)  
  {return Port_OFF;}     //端口关闭
  if(WaitAck == MG2618Infr.ModInf.Status)  
  {return PortBusy;}     //端口端繁忙
  if( 0 == opt)
  {MG2618Infr.CSR.RTEN = 0;}
  else
  {MG2618Infr.CSR.RTEN = 1;}
  err = CommandSerialization_MG2618(Command);
  if(err > Warning) 
  {return err;} 
  //获取命令计时器
  MG2618Infr.TimeOut = SofTimerCreate( timer,                 //周期
                                               TimerTB_10MS,       //时基
                                               SofTimer_PERIODIC,  //模式
                                               AckTimeOut_MG2618,  //回调
                                               P_NULL);//回调参数
 if(P_NULL == MG2618Infr.TimeOut)
 {return Nothing;}  //获取命令计时器失败 
 if(AT_Again != Command->Cmd)
 {MG2618Infr.Command.Cmd = Command->Cmd;}
 MG2618Infr.CallBack = CallBack;
 err2 = SendStringToSIM800(MG2618Infr.String); 
 if(NO_ERR == err2)
 {
  MG2618Infr.Cnt = 1; 
  MG2618Infr.ModInf.Status = WaitAck;
  SofTimerStart(MG2618Infr.TimeOut); 
 }
 else
 {SofTimerDel(&(MG2618Infr.TimeOut));}
 if(err2 > err)
 {err = err2;}  
 return err;  
}
/*********************************************************************************
 Function:      //
 Description:   // 模块输入数据解析
Input:          // data     输入数据首地址
                // length   输入数据长度
                //
                //   
 Output:        //
 Return:        //
 Others:        //              
*********************************************************************************/
void Decode_MG2618(unsigned char *gprs_ch, DataBuffSpec_Struct *Data)
{
 SystemErrName err = DataValid; 
 unsigned short itemp = 0; 
 //寻找参数开始字节
 *gprs_ch = 0;
 while (1)
 {
  if(IsLETTER(*(Data->pbuff)) 
     ||Isletter(*(Data->pbuff))
     ||('+'== *(Data->pbuff))
     ||('>'==*(Data->pbuff)))
  {break;}
  if(0 == --(Data->length)){return;}
  Data->pbuff ++;
 }
 if(WaitAck == MG2618Infr.ModInf.Status)
 {
  if((('A'==(Data->pbuff)[0])&&('T'==(Data->pbuff)[1]))
     ||(('A'==(Data->pbuff)[0])&&('/'==(Data->pbuff)[1])))
  { //消除回显
   itemp = 0;
   while (1)
   {
    if(('\r'==(Data->pbuff)[itemp])
       ||('\n'==(Data->pbuff)[itemp]))
    {break;}
    if(0 == --(Data->length))
    { return;}
    itemp ++;
   }
   while (1)
   {//寻找参数开始字节
    if('+'==(Data->pbuff)[itemp]
       ||IsLETTER((Data->pbuff)[itemp])
       ||Isletter((Data->pbuff)[itemp])
       ||IsDigit((Data->pbuff)[itemp])
       ||('>'==(Data->pbuff)[itemp]))
    {break;}
    if(0 == --(Data->length)){ return;}
    itemp ++;
   }
  }
  if(P_NULL != Command_ACK_FuncList_MG2618[MG2618Infr.Command.Cmd])
  {
   err = Command_ACK_FuncList_MG2618[MG2618Infr.Command.Cmd](&((Data->pbuff)[itemp]),Data->length);
   if(NO_ERR == err){return;} 
  }
 }
 while (1)
 {
  if(IsLETTER(*(Data->pbuff))||Isletter(*(Data->pbuff))){break;}
  if(0 == --(Data->length)){return;}
  Data->pbuff ++;
 }
 const char*  const IndicatorList_MG2618[] = { 
   "ZIPRECV:",        //提示从当前数据链路接收数据 
   "ZIPCLOSE:",       //提示TCP通道关闭提示  
   "ZIPRECVU:",       //提示接收 UDP 数据 
   "ZTCPRECV(P):",    //接收数据通告
   "ZUDPRECV(P):",    //接收数据通告
   "ZSMGS:"           //短信满指示 
 };  
 itemp = MatchinString(&(Data->pbuff),(const char**)IndicatorList_MG2618, 6);   //
 switch(itemp)
 {
  case 0 :   //提示接收 TCP 数据 
   {
    Data->length -= 8; 
    *gprs_ch = GetData_TCP_UDP(Data);
    break;
   }
  case 1 :   //提示TCP通道关闭提示 
   {
    Data->length -= 9;
    TcpClose_ACK(Data);
    break;
   }
  case 2 :   //接收数据通告
   {break;}
  case 3 :   //接收数据通告
   {break;}
  case 4 :   //短信满指示
   {break;}
  case 5 :   //短信满指示
   {break;} 
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          // 
                // 
                // 
 Output:        //
 Return:        //
 Others:        //
                  
*********************************************************************************/
unsigned short CompString (unsigned char *data,const char *sp,unsigned char **p)
{
 unsigned short length = 0;
 unsigned char *tp; 
 //比对查找参数名
 tp = data;
 while(sp[length] == tp[length])
 {length ++;}
 if('\0'== sp[length])
 {
   if(P_NULL != p)
   {*p = &tp[length];}
   return length;
 }
 else
 {
   if(P_NULL != p)
   {*p = data;}
   return 0;
 }
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                // 
 Output:        //
 Return:        //
 Others:        //
                  
*********************************************************************************/
void StopListen_MG2618(void)
{
 if(P_NULL != MG2618Infr.TimeOut)
 {SofTimerDel(&(MG2618Infr.TimeOut));}
 MG2618Infr.ModInf.Status = Idle; 
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
FunctionalState Get_MG2168PowerState (void)
{
 if(0 == MG2618_PowerFlg)
 {return DISABLE;}
 else
 {return ENABLE;}
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
void PowerOff_GPRS (void)
{
 unsigned char *p ;
 unsigned short itmp; 
//GPIO_InitTypeDef        GPIO_InitStructure;
 
 //GprsClose_UEV(1);
 //GprsClose_UEV(2);
 //GprsClose_UEV(3);
 //GprsClose_UEV(4);
 
 SofTimerDel(&(MG2618Infr.HeartBeatTime));
 SofTimerDel(&(MG2618Infr.TimeOut)) ;
 
 ///GPIO_SetBits(GPIOA, GPIO_Pin_2);
 //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 //GPIO_Init(GPIOA, &GPIO_InitStructure);
 //USART_Cmd(USART2, DISABLE);
 //RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);
 //MG2618_PowerOFF( );
 //MG2618Infr.ModInf.Status = OFF;
 
 p = (unsigned char *)&MG2618Infr;
 itmp = 0;
 //模块描述数据清零
 while(itmp < sizeof(MG2618DescriptorStruct))
 {p[itmp++] = 0;}
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                //
                //
                //   
 Output:        //
 Return:        //
 Others:        //
                  
*********************************************************************************/
static SystemErrName CommandSerialization_MG2618 (CommandDescriptor_MG2618 *Command)
{
 SystemErrName err = NO_ERR;
 unsigned short  itemp = 0;
 
 while (itemp < MG2616_ATORDER_LEN)
 {MG2618Infr.String[itemp++]  = '\0';} 
 
 switch(Command->Cmd)
 {
  case  AtData:
  case  AT_Again:  
    {break;}
  default:     //查询 UDP 状态
     {AddString(MG2618Infr.String, "AT");break;}
 }
 AddString(MG2618Infr.String,(unsigned char*)CmdNameList_MG2618[Command->Cmd]); 
   
 switch(Command->Format)
 {
   case  Void:
     {break;}
   case  Query:
     {AddString(MG2618Infr.String,"?");break; }
   case  Help:
     {AddString(MG2618Infr.String,"=?");break; }   
   case  Param:
     {
      if(Param == Command->Format)
       {
        itemp = 0; 
        while(itemp < sizeof(CommandParameter_MG2618))
        {
         ((unsigned char*)&MG2618Infr.Command.CmdPram)[itemp] = ((unsigned char*)&(Command->CmdPram))[itemp];
         itemp++ ;
        }
       }
      switch(Command->Cmd)
       {
        case  ZPNUM: 
          {err = ZPNUM_PramFill(Command);break;}
        case  ZIPSETUP:
        case  ZIPSETUPU:
          {err = TcpUdpConnect_PramFill(Command);break;}
        case ZIPSEND:         //向目标地址发送 TCP 数据 
          {err = TcpSend_PramFill(Command);break;}
        case ZIPSENDU:      //向 UDP 服务器发送数据 
          {err = UpdSend_PramFill(Command); break; }
        case ZIPCLOSE:      //关闭 TCP 链接
        case ZIPSTATUS:     //查询当前 TCP 连接状态  
        case ZIPCLOSEU:     //关闭 UDP 链接    
        case ZIPSTATUSU:    //查询 UDP 状态
          {err = TcpUdp_CSR_PramFill(Command);break;} 
        default:     //查询 UDP 状态
          {err = CmdValid;break;}
       }
       break;
     }
   default:     //查询 UDP 状态
     {err = CmdValid; break;}
 }  
 if(NO_ERR == err)
 {AddString(MG2618Infr.String, "\r\n");}
 return err;
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                //
                //
                //
 Output:        //
 Return:        //
 Others:        //AT+ZPNUM=<APN>,<USER>,<PWD>,<MODE>,<AUTH>
                //AT+ZPNUM="cmnet","user","pwd",1,0
*********************************************************************************/
static SystemErrName ZPNUM_PramFill(CommandDescriptor_MG2618 *Command)
{
 unsigned char* p;

 p = MG2618Infr.String;
 while('\0'!= *p)
 { p++;}
 
 *p++ = '=';
 if(P_NULL != Command->CmdPram.ZPNUM.APN)
 { p =  AddString(p, Command->CmdPram.ZPNUM.APN);}
 *p++ = ',';
 if(P_NULL != Command->CmdPram.ZPNUM.user)
 { p =  AddString(p, Command->CmdPram.ZPNUM.user);}
 *p++ = ','; 
 if(P_NULL != Command->CmdPram.ZPNUM.pwd)
 { p =  AddString(p, Command->CmdPram.ZPNUM.pwd);}
 //*p++ = ','; 
 //*p++ = Command->CmdPram.ZPNUM.MODE;
 //*p++ = ','; 
 //*p =   Command->CmdPram.ZPNUM.AUTH;
 return NO_ERR; 

}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                //
                //
                //
 Output:        //
 Return:        //
 Others:        //AT+ZIPSETUP=<N>,<IP>,<SERVER_PORT>[,<LOCAL_PORT>]
                  AT+ZIPSETUP=1,61.144.216.219,2332
*********************************************************************************/
static SystemErrName TcpUdpConnect_PramFill(CommandDescriptor_MG2618 *Command)
{
 unsigned char* p;

 if((Command->CmdPram.TcpUdp_Connect.CH < 1)
     ||(Command->CmdPram.TcpUdp_Connect.CH > 4))
  {return IllegalParam;}
 p = MG2618Infr.String;
 while('\0'!= *p)
 { p++;}
 *p++ = '='; 
 *p++ = Command->CmdPram.TcpUdp_Connect.CH +'0';
 *p++ = ',';
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Connect.IP[0]);
 *p++ = '.'; 
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Connect.IP[1]);
 *p++ = '.'; 
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Connect.IP[2]);
 *p++ = '.'; 
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Connect.IP[3]);
 *p++ = ','; 
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Connect.PORT);
 
 if(0 != Command->CmdPram.TcpUdp_Connect.LOCAL_PORT)
 {
   *p++ = ','; 
   p = AddIntToString(p,Command->CmdPram.TcpUdp_Connect.LOCAL_PORT); 
   if(Command->CmdPram.TcpUdp_Connect.LOCAL_PORT > 1024)
   {return NO_ERR; }
   else
   {return ParamRisky; }
 }
 return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                // 
 Output:        //
 Return:        //
 Others:        // AT+ZIPSEND= port,length<CR>
                   AT+ZIPSEND=1,10
                   >abcdefghij
                   +ZIPSEND:OK
                   OK
                   提示'>'后再发送数据
*********************************************************************************/
static SystemErrName TcpSend_PramFill(CommandDescriptor_MG2618 *Command)
{
 unsigned char* p;
 if((Command->CmdPram.TcpUdp_Connect.CH < 1)
     ||(Command->CmdPram.TcpUdp_Connect.CH > 4))
  {return IllegalParam;}
 p = MG2618Infr.String;
 while('\0'!= *p){p++;}
 *p++ = '='; 
 *p++ = Command->CmdPram.TcpUdp_Send.CH +'0';
 *p++ = ',';
 if(Command->CmdPram.TcpUdp_Send.LENGTH > 4096)
 {return IllegalParam;  }
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Send.LENGTH);
 return NO_ERR;
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                // 
 Output:        //
 Return:        //
 Others:        // AT+ZIPSEND= port,length<CR>
                   AT+ZIPSEND=1,10
                   >abcdefghij
                   +ZIPSEND:OK
                   OK
                   提示'>'后再发送数据
*********************************************************************************/
static SystemErrName UpdSend_PramFill(CommandDescriptor_MG2618 *Command)
{
 unsigned char* p;

 if((Command->CmdPram.TcpUdp_Connect.CH < 1)
     ||(Command->CmdPram.TcpUdp_Connect.CH > 4))
  {return IllegalParam;}
 
 p = MG2618Infr.String;
 while('\0'!= *p){p++;}
  
 *p++ = Command->CmdPram.TcpUdp_Send.CH +'0';
 *p++ = ',';
 if(Command->CmdPram.TcpUdp_Send.LENGTH > 1000)
 {return IllegalParam;  }
 p = AddIntToString(p,Command->CmdPram.TcpUdp_Send.LENGTH);
 return NO_ERR; 
}
/*********************************************************************************
 Function:      //
 Description:   //
Input:          //
                // 
 Output:        //
 Return:        //
 Others:        // AT+ZIPCLOSE=<N>
*********************************************************************************/
static SystemErrName TcpUdp_CSR_PramFill(CommandDescriptor_MG2618 *Command)
{
 unsigned char* p;
 if((Command->CmdPram.TcpUdp_Connect.CH < 1)
     ||(Command->CmdPram.TcpUdp_Connect.CH > 4))
  {return IllegalParam;}
 p = MG2618Infr.String;
 while('\0'!= *p){p++;};
  *p++ = '=';
 *p++ = Command->CmdPram.TcpUdp_CSR.CH +'0';
 return NO_ERR;
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
static void AckTimeOut_MG2618 (void *p_tmr, void *p_arg)
{
  if(ZIPSEND == MG2618Infr.Command.Cmd)
  {
    ZIPSEND_ACK_Func("> ", 2);
  }
  else if((0 != MG2618Infr.CSR.RTEN)&&(MG2618Infr.Cnt++ < 3))
  {
    SendStringToSIM800(MG2618Infr.String);
  }
  else
  {
   StopListen_MG2618( ); 
   if(P_NULL != MG2618Infr.CallBack)
   {MG2618Infr.CallBack(MG2618Infr.Command.Cmd,P_NULL);}
  }
}
/*********************************************************************************
 Function:      //
 Description:   //应答呼叫
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATA_Command_EN)
SystemErrName ATA_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //发起呼叫
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATD_Command_EN)
SystemErrName ATD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //拨打最近呼出的电话号码
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATDL_Command_EN)
SystemErrName ATDL_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //使能回馈通道
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATE_Command_EN)
SystemErrName ATE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //断开当前连接
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATH_Command_EN)
SystemErrName ATH_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //提示模块生产厂家信息 
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATI_Command_EN)
SystemErrName ATI_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置是否在终端回显返回值
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATQ_Command_EN)
SystemErrName ATQ_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //从数据模式切换到命令模式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != AtData_Command_EN)
SystemErrName AtData_ACK_Func(unsigned char *data, unsigned short length) 
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //从命令模式切换到数据模式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATO_Command_EN)
SystemErrName ATO_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //使用脉冲拨号
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATP_Command_EN)
SystemErrName ATP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //自动应答设置
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ATS0_Command_EN)
SystemErrName ATS0_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置来电类型
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != AT_CRC_Command_EN)
SystemErrName AT_CRC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //受话音量
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CLVL_Command_EN)
SystemErrName CLVL_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置来电号码显示
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CLIP_Command_EN)
SystemErrName CLIP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //静音控制
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZSETMUTE_Command_EN)
SystemErrName ZSETMUTE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查询国际 ID 号
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CIMI_Command_EN)
SystemErrName CIMI_ACK_Func(unsigned char *data, unsigned short length)
{ 
 return Default_ACK_Func(data, length);
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取产品版本号
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CGMR_Command_EN)
SystemErrName CGMR_ACK_Func(unsigned char *data, unsigned short length)
{
 unsigned short  itemp; 
 itemp = CompString(data,CmdNameList_MG2618[CGMR],&data);
 if(0 != itemp)
 {
  length -= itemp;  
  return Default_ACK_Func(data, length);
 }
 else
 {return DataValid;}  
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //回音消除控制
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ECHO_Command_EN)
SystemErrName ECHO_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取当前 IMEI 号
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != GSN_Command_EN)
SystemErrName GSN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取当前软件版本号
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZVERS_Command_EN)
SystemErrName ZVERS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //功能锁定
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CLCK_Command_EN)
SystemErrName CLCK_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //呼叫转移号码和条件设置
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CCFC_Command_EN)
SystemErrName CCFC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //呼叫等待控制
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CCWA_Command_EN)
SystemErrName CCWA_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //呼叫保持和多方会议
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CHLD_Command_EN)
SystemErrName CHLD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查询 SIM 卡状态
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != TSIMINS_Command_EN)
SystemErrName TSIMINS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //修改密码
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPWD_Command_EN)
SystemErrName CPWD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查询模块厂家
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CGMI_Command_EN)
SystemErrName CGMI_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //字符集种类的选择
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CSCS_Command_EN)
SystemErrName CSCS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //呼叫状态查询
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CLCC_Command_EN)
SystemErrName CLCC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //发送 DTMF 音
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != VTS_Command_EN)
//DTMF指令
SystemErrName VTS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //网络注册和漫游
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CREG_Command_EN)
//网络服务指令 
SystemErrName CREG_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //网络选择
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != COPS_Command_EN)
SystemErrName COPS_ACK_Func(unsigned char *data, unsigned short length)
{
 //请在这里增加实现代码
 unsigned short  itemp; 
 itemp = CompString(data,CmdNameList_MG2618[COPS],&data);
 if(0 != itemp)
 {
  length -= itemp;  
  return Default_ACK_Func(data, length);
 }
 else
 {return DataValid;} 
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //模块状态查询
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPAS_Command_EN)
//移动设备控制和状态报告
SystemErrName CPAS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置模块功能
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CFUN_Command_EN)
SystemErrName CFUN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //移动设备错误报告
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMEE_Command_EN)
SystemErrName CMEE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //模块关机
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZPWROFF_Command_EN)
SystemErrName ZPWROFF_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //输入 PIN 码
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPIN_Command_EN)
SystemErrName CPIN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //输入 PIN1 码
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != EPIN1_Command_EN)
SystemErrName EPIN1_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //输入 PIN2 码
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != EPIN2_Command_EN)
SystemErrName EPIN2_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查询 PIN1， PIN2 剩余输入次数
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != EPINC_Command_EN)
SystemErrName EPINC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //信号强度查询
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CSQ_Command_EN)
SystemErrName CSQ_ACK_Func(unsigned char *data, unsigned short length)
{
 DataBuffSpec_Struct AckData ;
 unsigned short  itemp; 
 unsigned short  itemp1;
 itemp = CompString(data,CmdNameList_MG2618[CSQ],&data);
 if(0 != itemp)
 {
  length -= itemp;  
  while (1)
  {
   if(IsDigit(*data)){break;}
   if(0 == --length){ return DataValid;}
   data ++;
  }
  AckData.pbuff = data;
  
  itemp = 0;
  while (1)
  {
   if(',' == *data){break;}
   itemp *= 10;
   itemp += (*data) - '0';
   if((0 == --length)||('\r' == *data)||('\n' == *data)){ return DataValid;}  
   data ++;
  }
  while (1)
  {
   if(IsDigit(*data)){break;}
   if(0 == --length){ return DataValid;}
   data ++;
  }
  itemp1 = 0;
  while (1)
  {
   if(('\r' ==*data)||('\n' == *data)){break;}
   itemp1 *= 10;
   itemp1 += (*data) - '0';
   if(0 == --length){ return DataValid;}  
   data ++;
  }
  MG2618Infr.ModInf.Rssi = itemp;
  MG2618Infr.ModInf.Ber = itemp1;
  
  if(P_NULL !=MG2618Infr.CallBack)
  {
    AckData.length = 0;
    while (--length)
    {
    if(('\r'== AckData.pbuff[AckData.length])||('\n' == AckData.pbuff[AckData.length+1]))
    {AckData.length ++;break;}  
    if(('O'== AckData.pbuff[AckData.length])||('K' == AckData.pbuff[AckData.length+1]))
    {AckData.length ++;break;} 
     AckData.length ++;
    }
    MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);
  }
  else
  {StopListen_MG2618( );}
  return NO_ERR;
 }
 else
 {return DataValid;}   
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //时钟管理
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CCLK_Command_EN)
SystemErrName CCLK_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //短信中心号码
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CSCA_Command_EN)
// 消息服务指令 
SystemErrName CSCA_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //短信确认
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CNMA_Command_EN)
SystemErrName CNMA_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置短消息模式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMGF_Command_EN)
SystemErrName CMGF_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置短信指示格式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CNMI_Command_EN)
SystemErrName CNMI_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //读短消息
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMGR_Command_EN)
SystemErrName CMGR_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //写短消息
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMGW_Command_EN)
SystemErrName CMGW_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //选择短信服务
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CSMS_Command_EN)
SystemErrName CSMS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //发送短信
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMGS_Command_EN)
SystemErrName CMGS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //首选短消息存储器
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPMS_Command_EN)
SystemErrName CPMS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //删除短信
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMGD_Command_EN)
SystemErrName CMGD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //短消息列表
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMGL_Command_EN)
SystemErrName CMGL_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //发送存贮的短信
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CMSS_Command_EN)
SystemErrName CMSS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //选择电话本存储
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPBS_Command_EN)
//电话本指令 
SystemErrName CPBS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //读取电话本信息
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPBR_Command_EN)
SystemErrName CPBR_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //向电话本写信息
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPBW_Command_EN)
SystemErrName CPBW_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查找电话本信息
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CPBF_Command_EN)
SystemErrName CPBF_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取本机号码
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CNUM_Command_EN)
SystemErrName CNUM_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //流控制
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != IFC_Command_EN)
SystemErrName IFC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 DTR 模式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != DTR_Command_EN)
SystemErrName DTR_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 DCD 模式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != DCD_Command_EN)
SystemErrName DCD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设定模块波特率
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != IPR_Command_EN)
SystemErrName IPR_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //恢复出厂设置
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != AT_F_Command_EN)
SystemErrName AT_F_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //保存设置
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != AT_W_Command_EN)
SystemErrName AT_W_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 PDP 格式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CGDCONT_Command_EN)
SystemErrName CGDCONT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //激活/解除 PDP 设置
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CGACT_Command_EN)
SystemErrName CGACT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 GPRS 业务
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CGATT_Command_EN)
SystemErrName CGATT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //GPRS 设备等级
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CGCLASS_Command_EN)
SystemErrName CGCLASS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查询模块状态信息
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZSTR_Command_EN)
//ZTE 特有指令
SystemErrName ZSTR_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取 ICCID
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGETICCID_Command_EN)
SystemErrName ZGETICCID_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置自动显示 CSQ 的条件
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZCSQ_Command_EN)
SystemErrName ZCSQ_ACK_Func(unsigned char *data, unsigned short length)//设置自动显示 CSQ 的条件
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 DTR 的检测模式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZEDT_Command_EN)
SystemErrName ZEDT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //32KHz 深度睡眠模式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZDSLEEP_Command_EN)
SystemErrName ZDSLEEP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //发送 USSD 数据
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CUSD_Command_EN)
SystemErrName CUSD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 RING 引脚的信号模式
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZRINGPINMODE_Command_EN)
SystemErrName ZRINGPINMODE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //定制版本切换
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZVERSWITCH_Command_EN)
SystemErrName ZVERSWITCH_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 APN，用户名，密码
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZPNUM_Command_EN)
SystemErrName ZPNUM_ACK_Func(unsigned char *data, unsigned short length)
{
 DataBuffSpec_Struct AckData ;
 
 if(Param == MG2618Infr.Command.Format)
 {
   while (1)
   {
    if(IsLETTER(*data)){break;}
    if(0 == --length){return DataValid;}
    data ++;
   }
  if(CompString(data,"OK",&data))
   {
    if(P_NULL != MG2618Infr.CallBack)
    {MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);}
    return NO_ERR;
   }
  return DataValid;
 }
 
 if(P_NULL !=MG2618Infr.CallBack)
 {
   while (1)
   {
    if(IsLETTER(*data) || Isletter(*data))
    {break;}
    if(0 == --length)
    { return DataValid;}
    data ++;
   }
   AckData.pbuff = data;
   AckData.length = 0;

   while (1)
   {
    if(IsLETTER(*data))
    {
     if(CompString(data,"OK",&data))
     {
      MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);
      return NO_ERR;
     }  
    }
    if(0 == --length){return DataValid;}
    data ++;
    AckData.length ++;
   }
  }
 return DataValid;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //打开 GPRS 数据连接
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZPPPOPEN_Command_EN)
SystemErrName ZPPPOPEN_ACK_Func(unsigned char *data, unsigned short length)
{
  //请在这里增加实现代码
  //DataBuffSpec_Struct AckData ;
 unsigned short  itemp; 
 itemp = CompString(data,CmdNameList_MG2618[ZPPPOPEN],&data);
 if(0 != itemp)
 {
  length -= itemp;  
  return Default_ACK_Func(data, length);
 }
 else
 {return DataValid;} 
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //关闭 GPRS 数据连接
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZPPPCLOSE_Command_EN)
SystemErrName ZPPPCLOSE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查询当前模块 IP 地址值
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZIPGETIP_Command_EN)
SystemErrName ZIPGETIP_ACK_Func(unsigned char *data, unsigned short length)
{
 unsigned short  itemp; 
 unsigned char   IP[4] = {0,0,0,0};      //本地IP地址 
 DataBuffSpec_Struct  AckData; 
 itemp = CompString(data,CmdNameList_MG2618[ZIPGETIP],&data);
 if(0 != itemp)
 {
  length -= itemp;
  while (1)
  {
   if(IsDigit(*data) || IsLETTER(*data) || Isletter(*data)){break;}
   if(0 == --length){return DataValid;}
   data ++;
  }
  AckData.pbuff = data;
  AckData.length = 0;
  itemp = 0;
  while (1)
  {
   if(('\r'== *data)||('\n' == *data)){break;}  
   if(0 == --length){return DataValid;}
   AckData.length ++;
   if('.'== *data){itemp ++; itemp %= 4;}
   if(IsDigit(*data))
   {IP[itemp] *= 10; IP[itemp] += *data - '0';  }
   data ++;
  }
  while (1)
  {
   if(IsLETTER(*data)){break;}
   if(0 == --length){return DataValid;}
   data ++;
  }
  
  if(CompString(data,"OK",&data))
  {
    MG2618Infr.ModInf.LOCAL_IP[0] = IP[0];
    MG2618Infr.ModInf.LOCAL_IP[1] = IP[1];
    MG2618Infr.ModInf.LOCAL_IP[2] = IP[2];
    MG2618Infr.ModInf.LOCAL_IP[3] = IP[3];
    if(P_NULL !=MG2618Infr.CallBack) 
    {MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);}
  }
  else
  {return DataValid;}
 }
 return NO_ERR;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 DNS 服务器的 IP 地址
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZDNSSERV_Command_EN)
SystemErrName ZDNSSERV_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取对应 Internet 域名的 IP 地址
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZDNSGETIP_Command_EN)
SystemErrName ZDNSGETIP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //建立 TCP 服务器链接
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZIPSETUP_Command_EN)
SystemErrName ZIPSETUP_ACK_Func(unsigned char *data, unsigned short length)
{
 unsigned short  itemp; 
 itemp = CompString(data,CmdNameList_MG2618[ZIPSETUP],&data);
 if(0 != itemp)
 {
  length -= itemp;  
  return Default_ACK_Func(data, length);
 }
 return DataValid;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //向目标地址发送 TCP 数据
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static void ZIPSEND_OK_Func(CommPortNumber_Type port, void *pbuff)
{
  StopListen_MG2618( );
   
  if(P_NULL !=MG2618Infr.Command.CmdPram.TcpUdp_Send.callBack)
  {
    port = GPRS_Port;
    port += MG2618Infr.Command.CmdPram.TcpUdp_Send.CH ;
    MG2618Infr.Command.CmdPram.TcpUdp_Send.callBack(port,
                                                           MG2618Infr.Command.CmdPram.TcpUdp_Send.buff);}
  
}
/*********************************************************************************
 Function:      //
 Description:   //向目标地址发送 TCP 数据
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZIPSEND_Command_EN)
SystemErrName ZIPSEND_ACK_Func(unsigned char *data, unsigned short length)
{
 CommPortNumber_Type  Port;
 unsigned short itemp  = 0;

 if(length > MG2618Infr.Command.CmdPram.TcpUdp_Send.LENGTH)
 {
  while (itemp < MG2618Infr.Command.CmdPram.TcpUdp_Send.LENGTH)
  {
   if(data[itemp] != MG2618Infr.Command.CmdPram.TcpUdp_Send.buff[itemp])   
   {break;}  
  }
  if(itemp >= MG2618Infr.Command.CmdPram.TcpUdp_Send.LENGTH)
  {
    length =- itemp;
    data = &(data[itemp]);
  } 
 }
 while (1)
 {
  if('+' == *data||('>' == *data )){break;}
  if(0 == --length){return DataValid;}
  data ++;
 }
 if('>' == *data)
 {
  if(P_NULL != MG2618Infr.TimeOut)
  {SofTimerRset(MG2618Infr.TimeOut);}  
  SendDataToSIM800(MG2618Infr.Command.CmdPram.TcpUdp_Send.buff,
                   MG2618Infr.Command.CmdPram.TcpUdp_Send.LENGTH,
                   ZIPSEND_OK_Func);
  
 }
 else
 { 
  itemp = CompString(data,CmdNameList_MG2618[ZIPSEND],&data); 
  if(0 != itemp)
  {
   length -= itemp;   
   while (1)
   {
    if(IsLETTER(*data)){break;}
    if(0 == --length){return DataValid;}
    data++;
   }
   itemp = CompString(data,"OK",&data);
   if(0 != itemp)
   {
    length -= itemp;   
    while (1)
    {
     if(IsLETTER(*data)){break;}
     if(0 == --length){return DataValid;}
     data++;
    }
    itemp = CompString(data,"OK",&data); 
    if(0 != itemp)
    {
     length -= itemp;  
     StopListen_MG2618( );  
     if(P_NULL !=MG2618Infr.Command.CmdPram.TcpUdp_Send.callBack)
     {
      Port = GPRS_Port;
      Port += MG2618Infr.Command.CmdPram.TcpUdp_Send.CH;
      MG2618Infr.Command.CmdPram.TcpUdp_Send.callBack(Port,MG2618Infr.Command.CmdPram.TcpUdp_Send.callBack_arg);
     }            
    } 
   } 
  }
 }
 return NO_ERR; 
}
#endif


/*********************************************************************************
 Function:      //
 Description:   //查询 GPRS 连接状态
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZPPPSTATUS_Command_EN)
SystemErrName ZPPPSTATUS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //关闭 TCP 链接
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZIPCLOSE_Command_EN)
SystemErrName ZIPCLOSE_ACK_Func(unsigned char *data, unsigned short length)
{
 unsigned short  itemp; 
 DataBuffSpec_Struct  AckData;
 itemp = CompString(data,CmdNameList_MG2618[ZIPCLOSE],&data);
 if(0 != itemp)
 {
  length -= itemp; 
  while (1)
  {
   if(IsDigit(*data) || IsLETTER(*data) || Isletter(*data)){break;}
   if(0 == --length){return DataValid;}
   data ++;
  }
  AckData.pbuff = data;
  AckData.length = 0;
  while (1)
  {
   if(('\r'== AckData.pbuff[AckData.length])||('\n' == AckData.pbuff[AckData.length])){break;}  
   if(0 == --length){return DataValid;}
   AckData.length ++;
  }
  data = &AckData.pbuff[AckData.length];
  while (1)
  {
   if(IsLETTER(*data)){break;}
   if(0 == --length){return DataValid;}
   data ++;
  }
  if(CompString(data,"OK",&data))
  {
    if(P_NULL !=MG2618Infr.CallBack)
    {MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);}
    
    //GprsClose_UEV(MG2618Infr.Command.CmdPram.TcpUdp_CSR.CH);
    StopListen_MG2618( );
  }
 }
 return NO_ERR;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查询当前 TCP 连接状态 
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZIPSTATUS_Command_EN)
SystemErrName ZIPSTATUS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //建立 UDP 服务器链接
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZIPSETUPU_Command_EN)
// UDP 连接指令 
SystemErrName ZIPSETUPU_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //向 UDP 服务器发送数据
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZIPSENDU_Command_EN)
SystemErrName ZIPSENDU_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查询 UDP 状态
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZIPSTATUSU_Command_EN)
SystemErrName ZIPSTATUSU_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //关闭 UDP 链接
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZIPCLOSEU_Command_EN)
SystemErrName ZIPCLOSEU_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //端口监听功能设置
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTCPLISTEN_Command_EN)
SystemErrName ZTCPLISTEN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //在被动打开的链路上发送数据
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTCPSENDP_Command_EN)
SystemErrName ZTCPSENDP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //关闭监听到的连接
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTCPCLOSEP_Command_EN)
SystemErrName ZTCPCLOSEP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //查询被动打开链路
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTCPSTATUSP_Command_EN)
SystemErrName ZTCPSTATUSP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置模块连接服务器和发送数据的超时时间
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZIPTIMEOUT_Command_EN)
SystemErrName ZIPTIMEOUT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //端口监听功能设置
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZUDPLISTEN_Command_EN)
SystemErrName ZUDPLISTEN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //在被动打开的链路上发送数据
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZUDPSENDP_Command_EN)
SystemErrName ZUDPSENDP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //登录 FTP 服务器
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPLOGIN_Command_EN)
// FTP 指令
SystemErrName ZFTPLOGIN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 FTP 文件类型
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPTYPE_Command_EN)
SystemErrName ZFTPTYPE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //上传文件
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPUPLOAD_Command_EN)
SystemErrName ZFTPUPLOAD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取文件大小
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPSIZE_Command_EN)
SystemErrName ZFTPSIZE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //下载文件
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPDNLOAD_Command_EN)
SystemErrName ZFTPDNLOAD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //下载 FTP 文件
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPDNLOADEX_Command_EN)
SystemErrName ZFTPDNLOADEX_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //删除文件
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPDEL_Command_EN)
SystemErrName ZFTPDEL_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //退出 FTP
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFTPQUIT_Command_EN)
SystemErrName ZFTPQUIT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //透明传输
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTRANSFER_Command_EN)
// 透明传输指令 
SystemErrName ZTRANSFER_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //摘机音设置
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZCALLTONE_Command_EN)
//音频相关指令 
SystemErrName ZCALLTONE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //ZDTMF 拨号音设置
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZDTMFTONE_Command_EN)
SystemErrName ZDTMFTONE_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //打开 DTMF 检测功能
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZKTDSWITCH_Command_EN)
SystemErrName ZKTDSWITCH_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //主动上报检测的 DTMF 按键
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZKTDIND_Command_EN)
SystemErrName ZKTDIND_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //语音通道切换指令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != SPEAKER_Command_EN)
SystemErrName SPEAKER_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 MIC 音频参数
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZMICGB_Command_EN)
SystemErrName ZMICGB_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取小区信息
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != CCED_Command_EN)
SystemErrName CCED_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //锁定 BCCH 信道（暂不支持）
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZBCCH_Command_EN)
SystemErrName ZBCCH_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //锁定 GSM 频段
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZBAND_Command_EN)
SystemErrName ZBAND_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //锁定网络运营商
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZOPT_Command_EN)
SystemErrName ZOPT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //读取载波列表
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZCALIST_Command_EN)
SystemErrName ZCALIST_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //TTS(Text to Speech)语音播报业务
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTTS_Command_EN)
SystemErrName ZTTS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //设置 TTS(Text to Speech)语音播报参数(暂不支持切换发音人)
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZTTSP_Command_EN)
SystemErrName ZTTSP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //录音功能
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZAUDREC_Command_EN)
SystemErrName ZAUDREC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //读取文件
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZFILEREAD_Command_EN)
SystemErrName ZFILEREAD_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //开关 GPS 指令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSOPEN_Command_EN)
// GPS 相关指令 
SystemErrName ZGPSOPEN_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //GPS 休眠指令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSSLEEP_Command_EN)
SystemErrName ZGPSSLEEP_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //NMEA 上报控制指令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSNMEARPT_Command_EN)
SystemErrName ZGPSNMEARPT_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;

}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取 GPS 定位经纬度指令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSLOC_Command_EN)
SystemErrName ZGPSLOC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取 GPS 时间指令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSUTC_Command_EN)
SystemErrName ZGPSUTC_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err; 
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //获取角度速度指令
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSHS_Command_EN)
SystemErrName ZGPSHS_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err; 
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //GPS 定位状态上报
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
#if(0 != ZGPSIND_Command_EN)
SystemErrName ZGPSIND_ACK_Func(unsigned char *data, unsigned short length)
{
 SystemErrName err = NO_ERR;
 
 请在这里增加实现代码 
 
 return err;
}
#endif
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:        //
 Others:        //
*********************************************************************************/
static SystemErrName Default_ACK_Func(unsigned char *data, unsigned short length)
{
 DataBuffSpec_Struct  AckData;
 if(P_NULL !=MG2618Infr.CallBack)
 {
  while (1)
  {
   if(IsDigit(*data) || IsLETTER(*data) || Isletter(*data)){break;}
   if(0 == --length){return DataValid;}
   data ++;
  }
  AckData.pbuff = data;
  AckData.length = 0;
  while (1)
  {
   if(('\r'== AckData.pbuff[AckData.length])||('\n' == AckData.pbuff[AckData.length])){break;}  
   if(0 == --length){return DataValid;}
   AckData.length ++;
  }
  data = &AckData.pbuff[AckData.length];
  while (1)
  {
   if(IsLETTER(*data)){break;}
   if(0 == --length){return DataValid;}
   data ++;
  }
  if(CompString(data,"OK",&data)||CompString(data,"ERROR",&data))
  {MG2618Infr.CallBack(MG2618Infr.Command.Cmd,&AckData);}
  else
  {return DataValid;}
 }
 return NO_ERR;
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
static unsigned char GetData_TCP_UDP(DataBuffSpec_Struct *Data)
{
  unsigned char  CH;
  unsigned short length;
  while (1)
  {
   if(IsDigit(*(Data->pbuff))){break;}
   if(0 == --(Data->length)){Data->length = 0; return CH;}
   Data->pbuff ++;
  }
  while (1)
  {
   if((','== *(Data->pbuff))){break;}  
   if(0 == --(Data->length)){Data->length = 0; return CH;}
   if(IsDigit(*(Data->pbuff)))
   {CH = *(Data->pbuff) - '0';  }
   Data->pbuff ++;
  }
  length = 0; 
  Data->pbuff ++;
  while (1)
  {
   if((','== *(Data->pbuff))){break;}  
   if(0 == --(Data->length)){Data->length = 0; return CH;}
   if(IsDigit(*(Data->pbuff)))
   {length *= 10; length += *(Data->pbuff) - '0';  }
   Data->pbuff ++;
  }
  Data->pbuff ++;
 if('\r' == Data->pbuff[length] ||'\n' == Data->pbuff[length])
 {Data->length = length;return CH;}
 else
 {Data = P_NULL;return CH;}
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
static void TcpClose_ACK(DataBuffSpec_Struct *Data)
{
  /*
  //unsigned char  CH;
  while (1)
  {
   if(IsDigit(*(Data->pbuff))){break;}
   if(0 == --(Data->length)){return ;}
   Data->pbuff ++;
  }
  while (1)
  {
   if(!IsDigit(*(Data->pbuff))){break;}  
   if(0 == --(Data->length)){ return;}
   if(IsDigit(*(Data->pbuff)))
   {CH = *(Data->pbuff) - '0'; break; }
   Data->pbuff ++;
  }
 //GprsClose_UEV(CH);*/
}
/***************************************END*****************************************/
