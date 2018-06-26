/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
#ifndef  __COMMOM_API_H
#define  __COMMOM_API_H 
/*********************************************************************************
文件包含区
*********************************************************************************/

/*********************************************************************************
宏定义区
*********************************************************************************/
#define IsLETTER(c)	(((c)>='A')&&((c)<='Z'))
#define Isletter(c)	(((c)>='a')&&((c)<='z'))
#define IsDigit(c)	(((c)>='0')&&((c)<='9')) 

#define IsABCDEF(c)	(((c)>='A')&&((c)<='F'))
#define Isabcdef(c)	(((c)>='a')&&((c)<='f')) 


#ifndef  P_NULL 
#define  P_NULL    (void *)(0x00000000)
#endif

/*********************************************************************************
数据类型定义
*********************************************************************************/
//数据缓冲区描述符结构定义
typedef struct {
 unsigned char * pbuff;
 unsigned short length; 
}DataBuffSpec_Struct;

typedef struct {
 unsigned short length; 
 void *pbuff ;
}BuffDeclarator_Struct;

//
typedef union {
  unsigned char Byte;
  struct{
    unsigned short no1: 1;
    unsigned short no2: 1;
    unsigned short no3: 1;
    unsigned short no4: 1;
    unsigned short no5: 1;
    unsigned short no6: 1;
    unsigned short no7: 1;
    unsigned short no8: 1;
  }Bit;
}BIT_8W;

typedef union {
  unsigned short HalfWord;
  struct {
    unsigned char  L;
    unsigned char  H;
  }Byte;
  struct{
    unsigned short no1: 1;
    unsigned short no2: 1;
    unsigned short no3: 1;
    unsigned short no4: 1;
    unsigned short no5: 1;
    unsigned short no6: 1;
    unsigned short no7: 1;
    unsigned short no8: 1;
    unsigned short no9: 1;
    unsigned short no10: 1;
    unsigned short no11: 1;
    unsigned short no12: 1;
    unsigned short no13: 1;
    unsigned short no14: 1;
    unsigned short no15: 1;
    unsigned short no16: 1;
  }Bit;
}BIT_16W;

typedef union {
  unsigned long Word;
  struct {
    unsigned char  L;
    unsigned char  H;
  }HalfWord;
  struct {
    unsigned char  LL;  
    unsigned char  L;
    unsigned char  H;
    unsigned char  HH;
  }Byte;
  struct{
    unsigned long no1: 1;
    unsigned long no2: 1;
    unsigned long no3: 1;
    unsigned long no4: 1;
    unsigned long no5: 1;
    unsigned long no6: 1;
    unsigned long no7: 1;
    unsigned long no8: 1;
    unsigned long no9: 1;
    unsigned long no10: 1;
    unsigned long no11: 1;
    unsigned long no12: 1;
    unsigned long no13: 1;
    unsigned long no14: 1;
    unsigned long no15: 1;
    unsigned long no16: 1;
    unsigned long no17: 1;
    unsigned long no18: 1;
    unsigned long no19: 1;
    unsigned long no20: 1;
    unsigned long no21: 1;
    unsigned long no22: 1;
    unsigned long no23: 1;
    unsigned long no24: 1;
    unsigned long no25: 1;
    unsigned long no26: 1;
    unsigned long no27: 1;
    unsigned long no28: 1;
    unsigned long no29: 1;
    unsigned long no30: 1;
    unsigned long no31: 1;
    unsigned long no32: 1;
  }Bit;
}BIT_32W;

typedef enum{
  OK_Message  = 0x00,
  Warning_Message = 0x01,
  Error_Message = 0x02,
}MessageType;

extern const unsigned char  Integer[10];
/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
函数声明区
*********************************************************************************/
unsigned short crc16(unsigned char *puchMsg, unsigned char usDataLen);
unsigned char getDaysInMonth (unsigned short Year, unsigned char Month);
void DelayuS(unsigned long us);
void DelymS (unsigned short ms);
void Int8ToBCD (unsigned char data, unsigned char *pbuff, unsigned char w);
void Int16ToBCD (unsigned short data, unsigned char *pbuff, unsigned char w);
void Int32ToBCD (unsigned long data, unsigned char *pbuff, unsigned char w);
unsigned char BCDToInt8 (unsigned char *pbuff, unsigned char w, unsigned char* data);
unsigned char BCDToInt16 (unsigned char *pbuff, unsigned char w, unsigned short* data);
unsigned char BCDToInt32 (unsigned char *pbuff, unsigned char w, unsigned long* data);
unsigned char IntToString( unsigned long data, unsigned char* pbuff);
void AsciiHexToBCD ( unsigned char* hex, unsigned char length,
                      void* bcd, unsigned char width );

//将实数字转换为符串
unsigned char* BinToSting (unsigned long Uint32,unsigned char *String);
//将字符串转换为实数
unsigned long StingToBin (unsigned char *String);
void CopyBuff (void* tp, void* sp, unsigned long length);
unsigned char*  AddString(unsigned char* tp, unsigned char* cp);
unsigned char* AddIntToString(unsigned char* tp, unsigned long vlue);
unsigned char* AddBinToAsciiString(unsigned char* tp, unsigned long vlue);
unsigned char MarkString (unsigned char*String,unsigned char*Mark);
//搜寻'A'-'Z' 在字符串中首次出现位置
unsigned char SeekIsLETTER (unsigned char*String, unsigned long *seat);
//搜寻'a'-'z' 在字符串中首次出现位置
unsigned char SeekIsletter (unsigned char*String, unsigned long *seat);
//搜寻'0'-'9' 在字符串中首次出现位置
unsigned char SeekIsDigit (unsigned char*String, unsigned long *seat);
//搜寻 指定字符 在字符串中首次出现位置
unsigned char SeekChar (unsigned char*String, unsigned char mark, unsigned long *seat);
//搜寻 特定字符串 在字符串中首次出现位置
void StringDel(char * targ_str, unsigned long length);

unsigned char SeekMarkString(unsigned char*String,unsigned char* mark,unsigned long *seat);



unsigned char ContrastString (unsigned char*String1,unsigned char*String2);
unsigned short MatchinString (unsigned char **data,
                                const char**StringList, 
                                unsigned short ListSize);
/********************************************************************************/
#endif