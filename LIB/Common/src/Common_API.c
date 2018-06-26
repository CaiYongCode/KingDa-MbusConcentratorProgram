/*********************************************************************************
//概述：
//作者：大胡子书生     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "..\inc\Common_API.h"
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
/*********************************************************************************
常量定义区
*********************************************************************************/
const unsigned char  Integer[10] ={0,1,2,3,4,5,6,7,8,9};
/* CRC 高位字节值表 */ 
const unsigned char  auchCRCHi[] = { 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 
} ; 
/* CRC低位字节值表*/ 
const unsigned char  auchCRCLo[] = { 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
0x43, 0x83, 0x41, 0x81, 0x80, 0x40 
} ;
/*********************************************************************************
公共变量定义区
*********************************************************************************/

/*********************************************************************************
外部变量声明区
*********************************************************************************/

/*********************************************************************************
私有变量定义区
*********************************************************************************/
/*********************************************************************************
测试变量定义区
*********************************************************************************/

/*********************************************************************************
内部函数定义区
*********************************************************************************/

/*********************************************************************************
功能代码定义区
*********************************************************************************/
/*******************************************************************************
  Function:       // CRC_16
  Description:    // 计算数据序列的16位CRC校验码
  Input:          //crc16:数据序列的首地址 
                  //usDataLen:数据序列的长度
  Output:         //
  Return:         //数据序列的CRC校验码
  Others:         //CRC16=X16+X15+X5+1    
                  //数据序列最长255字节
********************************************************************************/
unsigned short crc16(unsigned char *puchMsg, unsigned char usDataLen) 
{ 
 unsigned char uchCRCHi = 0xFF ;  /* 高CRC字节初始化 */ 
 unsigned char uchCRCLo = 0xFF ;  /* 低CRC 字节初始化 */ 
 unsigned char  uIndex ;          /* CRC循环中的索引 */ 
 while (usDataLen--)              /* 传输消息缓冲区 */ 
  { 
   uIndex = uchCRCHi ^ *puchMsg++ ; /* 计算CRC */ 
   uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
   uchCRCLo = auchCRCLo[uIndex] ; 
  } 
  return (uchCRCHi << 8 | uchCRCLo) ; 
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
void DelayuS(unsigned long us)
{
  while ( us -- );
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
void DelymS (unsigned short ms)
{
  unsigned long itemp ;
  while(ms--)
  {
    itemp = 8000;
    while (itemp--);
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
unsigned char getDaysInMonth (unsigned short Year, unsigned char Month)
{
  unsigned char Days;
  
  if(2 == Month)
  {
    if(0 ==(Year%4))
    {
      if(0 ==(Year%100))
      {
        if(0 ==(Year%400))
        {Days = 29;}
        else
        {Days = 28;}
      }
      else
      {Days = 29;}
    }
    else
    {Days = 28;}
  }
  else if(( 1 == Month)||( 3 == Month)||( 5 == Month)||( 7 == Month)||( 8 == Month)||( 10 == Month)||( 12 == Month))
  {Days = 31;}
  else if(( 4 == Month)||( 6 == Month)||( 9 == Month)||( 11 == Month))
  {Days = 30;}
  else
  {Days = 0;}
  
  return Days;
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
void Int8ToBCD (unsigned char data, unsigned char *pbuff, unsigned char w)
{
  unsigned char itemp;
  if( 0 == w )
  {return;}
  else if(w & 0x01)
  {w ++;}
  w >>= 1;
  itemp = 0;
  while (data > 0)
  {
    pbuff[itemp] =  data %10;
    data /= 10;
    pbuff[itemp] |= (data %10) << 4;
    data /= 10;
    itemp ++;
    if(itemp >= w){break;}
  }
  while (itemp < w)
  {pbuff[itemp] = 0;itemp ++;}
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
void Int16ToBCD (unsigned short data, unsigned char *pbuff, unsigned char w)
{
  unsigned char itemp;
  itemp = 0;
  while (data > 0)
  {
    pbuff[itemp] =  data %10;
    data /= 10;
    pbuff[itemp] |= (data %10) << 4;
    data /= 10;
    itemp ++;
    if(itemp >= w){break;}
  }
  while (itemp < w)
  {pbuff[itemp] = 0;itemp ++;}
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
void Int32ToBCD (unsigned long data, unsigned char *pbuff, unsigned char w)
{
  unsigned char itemp;
  itemp = 0;
  while (data > 0)
  {
    pbuff[itemp] =  data %10;
    data /= 10;
    pbuff[itemp] |= (data %10) << 4;
    data /= 10;
    itemp ++;
    if(itemp >= w){break;}
  }
  while (itemp < w)
  {pbuff[itemp] = 0;itemp ++;}
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
unsigned char BCDToInt8 (unsigned char *pbuff, unsigned char w, unsigned char* data)
{
  unsigned char itemp = 0;
  unsigned char cnt = 0;
  *data = 0;
  while (itemp < 2)
  {
    if(cnt >= w)
    {break;}
    else
    {
      if((pbuff[itemp] >> 4) > 9)
      {return 0;}
      *data *= 10;
      *data += pbuff[itemp] >> 4;
      cnt++;
    }
    if(cnt >= w)
    {break;}
    else
    {
      if((pbuff[itemp]& 0x0f) > 9)
      {return 0;}
      *data *= 10;
      *data += pbuff[itemp] & 0x0f;
      cnt++;
    }
    itemp ++;
  }
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
unsigned char BCDToInt16 (unsigned char *pbuff, unsigned char w, unsigned short* data)
{
  unsigned char itemp = 0;
  unsigned char cnt = 0;
  * data = 0;
  while (itemp < 3)
  {
    if(cnt >= w)
    {break;}
    else
    {
      if((pbuff[itemp] >> 4) > 9)
      {return 0;}
      *data *= 10;  
      *data += pbuff[itemp] >> 4;
      cnt++;
    }
    if(cnt >= w)
    {break;}
    else
    {
      if((pbuff[itemp]& 0x0f) > 9)
      {return 0;}
      *data *= 10;
      *data += pbuff[itemp]& 0x0f;
      cnt++;
    }
    itemp ++;
  }
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
unsigned char BCDToInt32 (unsigned char *pbuff, unsigned char w, unsigned long* data)
{
  unsigned char itemp = 0;
  unsigned char cnt = 0;
  *data = 0;
  while (itemp < 5)
  {
    if(cnt >= w)
    {break;}
    else
    {
      if((pbuff[itemp] >> 4) > 9)
      {return 0;}
      *data *= 10;
      *data += pbuff[itemp] >> 4;  
      cnt++;
    }
    if(cnt >= w)
    {break;}
    else
    {
      if((pbuff[itemp]& 0x0f) > 9)
      {return 0;}
      *data *= 10;
      *data += pbuff[itemp]& 0x0f;
      cnt++;
    }
    itemp ++;
  }
  return 1;
}
/*********************************************************************************
Function:      //
Description:   //ASCII码串表示的16进制变换为实数
Input:         //hex： ASCII码串首地址
//length：ASCII码串长度
width：期望的实数16进制码元数
Output:        //
Return:        //
Others:        //
*********************************************************************************/
void AsciiHexToBCD ( unsigned char* hex,
                    unsigned char length,
                    void* bcd, 
                    unsigned char width )   
{
  unsigned char* pdata;
  pdata = bcd;
  do{
    *pdata = 0;
    if((0 == length)||(0 ==width))
    {break;}
    if(IsDigit(*hex))
    {*pdata = *hex - '0';}
    else if(IsABCDEF(*hex))
    {*pdata = *hex - 'A';}
    else if(Isabcdef(*hex))
    {*pdata = *hex - 'a';}
    length --;
    width --;
    if((0 == length)||(0 ==width))
    {break;}
    *pdata <<= 4;
    hex ++;
    if(IsDigit(*hex))
    {*pdata |= *hex - '0';}
    else if(IsABCDEF(*hex))
    {*pdata |= *hex - 'A';}
    else if(Isabcdef(*hex))
    {*pdata |= *hex - 'a';}
    length --;
    width --;
    hex ++;
    pdata ++;
  }while( 1 );
}
/*********************************************************************************
Function:      //
Description:   //实数 变换为 ASCII码串
Input:         //
//

Output:        //
Return:        //
Others:        //
*********************************************************************************/
unsigned char IntToString( unsigned long data, unsigned char* pbuff) 
{
  unsigned char buff[16];
  unsigned char itemp  =0;
  unsigned char re;
  do{
    buff[itemp++] = data % 10 + '0';
    data /= 10;
  }while(0 != data);
  re = itemp;
  while(itemp-- > 0)
  {
    *pbuff++ = buff[itemp];
  }
  return re;
}
/*********************************************************************************
Function:      //
Description:   //将实数字转换为符串
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
unsigned char* BinToSting (unsigned long Uint32,unsigned char *String)
{
  unsigned char buff[10];
  unsigned char cnt = 0;
  do 
  {
    buff[cnt++] = (Uint32 % 10) + '0';
    Uint32 /= 10;
  }while (0 != Uint32);
  do  
  {
    *String++ = buff[--cnt];
  }while (0 != cnt); 
 
  *String = '\0';
  return String;
}
/*********************************************************************************
Function:      //
Description:   //将字符串转换为实数
Input:         //
//
Output:        //
Return:        //
Others:        //
*********************************************************************************/
unsigned long StingToBin (unsigned char *String)
{
  unsigned long Int32 = 0; 
  while (IsDigit(*String))
  {
    Int32 *= 10;
    Int32 += *String - '0';
    String ++;
  }
  return Int32;
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
void CopyBuff (void* tp, void* sp, unsigned long length)
{
  while(length-- > 0)
  {
    ((unsigned char*)tp)[length] = ((unsigned char*)sp)[length];
  }
}
/*********************************************************************************
Function:      //AddString
Description:   //将一个字符串增加到另一个字符串末尾
Input:         // tp：指向目的字符串
// cp：指向源的字符串
Output:        //
Return:        // 指向最后一个增加的字符的下一个字符地址
Others:        //
*********************************************************************************/
unsigned char*  AddString(unsigned char* tp, unsigned char* cp)
{
  while('\0' != *tp)
  {tp++;}
  while('\0' != * cp )
  {
    *tp = *cp; 
    tp++;
    cp++;
  }
  return tp;
}
/*********************************************************************************
Function:      //AddIntToString
Description:   //将一个二进制数以十进制字符串格式增加到另一个字符串末尾
Input:         // tp：指向目的字符串
// cp：指向源的字符串
Output:        //
Return:        // 指向最后一个增加的字符的下一个字符地址
Others:        //
*********************************************************************************/
unsigned char* AddIntToString(unsigned char* tp, unsigned long vlue)
{
  unsigned char buff[20];
  unsigned char itemp =0;
  do
  {
    buff[itemp++] = (vlue % 10)+'0';
    vlue /= 10;
  }while(0 != vlue);
  
  while('\0' != *tp)
  {tp++;}
  
  while(0 != itemp)
  {
    *tp = buff[--itemp]; 
    tp++;
  }
  return tp;
}
/*********************************************************************************
Function:      //AddIntToString
Description:   //将一个二进制数以ASCII码格式增加到另一个字符串末尾
Input:         // tp：指向目的字符串
// cp：指向源的字符串
Output:        //
Return:        // 指向最后一个增加的字符的下一个字符地址
Others:        //
*********************************************************************************/
unsigned char* AddBinToAsciiString(unsigned char* tp, unsigned long vlue)
{
  unsigned char buff[20];
  unsigned char itemp =0;
  unsigned char itemp1 = 0;
  do
  {
    itemp1 = vlue & 0x0000000f;
    if(itemp1 < 0x0A)
    {itemp1 += '0';}
    else
    {itemp1 += 'A'- 0x0A;}
    buff[itemp++] = itemp1;
    vlue >>= 4;
  }while(0 != vlue);
  
  while('\0' != *tp)
  {tp++;}
  
  while(0 != itemp)
  {
    *tp = buff[--itemp]; 
    tp++;
  }
  return tp;
}
/*********************************************************************************
Function:      //
Description:   //搜寻 指定字符 在字符串中首次出现位置
Input:          // 
//    
// 
Return:        //  
Others:        //

*********************************************************************************/
unsigned char SeekChar (unsigned char*String,
                        unsigned char mark,
                        unsigned long *seat)
{
  unsigned long itemp  = 0;
  while ('\0' != String[itemp])
  {
    if(mark == String[itemp])
    {
      *seat = itemp; 
      return 1; 
    }
    else
    {itemp ++;}
  }
  *seat = itemp; 
  return 0;  
}
/*********************************************************************************
Function:      //
Description:   //搜寻'A'-'Z' 在字符串中首次出现位置
Input:          // 
//    
// 
Return:        //  
Others:        //

*********************************************************************************/
unsigned char SeekIsLETTER (unsigned char*String, 
                            unsigned long *seat)
{
  unsigned long itemp  = 0;
  while ('\0' != String[itemp])
  {
    if(IsLETTER(String[itemp]))
    {
      *seat = itemp; 
      return 1; 
    }
    else
    {itemp ++;}
  }
  return 0;       
}
/*********************************************************************************
Function:      //
Description:   // 搜寻'a'-'z' 在字符串中首次出现位置
Input:          // 
//    
// 
Return:        //  
Others:        //

*********************************************************************************/
unsigned char SeekIsletter (unsigned char*String,
                            unsigned long *seat)
{
  unsigned long itemp  = 0;
  while ('\0' != String[itemp])
  {
    if(Isletter(String[itemp]))
    {
      *seat = itemp; 
      return 1; 
    }
    else
    {itemp ++;}
  }
  return 0;       
}
/*********************************************************************************
Function:      //
Description:   // 搜寻'0'-'9' 在字符串中首次出现位置
Input:          // 
//    
// 
Return:        //  
Others:        //

*********************************************************************************/
unsigned char SeekIsDigit (unsigned char* String,
                           unsigned long *seat)
{
  unsigned long itemp  = 0;
  while ('\0' != String[itemp])
  {
    if(IsDigit(String[itemp]))
    {
      *seat = itemp; 
      return 1; 
    }
    else
    {itemp ++;}
  }
  return 0;       
}
/*********************************************************************************
Function:      //
Description:   // 
Input:         // 
//
// 
Return:        //  
Others:        //

*********************************************************************************/
unsigned char   MyStrMatch(char* str1, char* pattern)
{ 
  int len1 = strlen(str1);
  int len2 = strlen(pattern);
  int mark = 0;//用于分段标记,'*'分隔的字符串
  int p1 = 0, p2 = 0;
  
  while ((p1<len1) && (p2<len2))
  {
    if (pattern[p2] == '?')
    {
      p1++;
      p2++;
      continue;
    }
    if (pattern[p2] == '*')
    {
      //如果当前是*号，则mark前面一部分已经获得匹配，从当前点开始继续下一个块的匹配
      p2++;
      mark = p2;
      continue;
    }
    if (str1[p1] != pattern[p2])
    {
      if (p1 == 0 && p2 == 0)
      {
        //如果是首字符，特殊处理，不相同即匹配失败
        return 0; //false;
      }
      /*
      * pattern: ...*bdef*...
      *       ^
      *       mark
      *        ^
      *        p2
      *       ^
      *       new p2
      * str1:.....bdcf...
      *       ^
      *       p1
      *      ^
      *     new p1
      * 如上示意图所示，在比到e和c处不想等
      * p2返回到mark处，
      * p1需要返回到下一个位置。
      * 因为*前已经获得匹配，所以mark打标之前的不需要再比较
      */
      p1 -= p2 - mark - 1;
      p2 = mark;
      continue;
    }
    /*
    * 此处处理相等的情况
    */
    p1++;
    p2++;
  }
  if (p2 == len2)
  {
    if (p1 == len1)
    {
      /*
      * 两个字符串都结束了，说明模式匹配成功
      */
      return 1; //true;
    }
    if (pattern[p2 - 1] == '*')
    {
      /*
      * str1还没有结束，但pattern的最后一个字符是*，所以匹配成功
      *
      */
      return 1; //true;
    }
  }
  while (p2<len2)
  {
    /*
    * pattern多出的字符只要有一个不是*,匹配失败
    *
    */
    if (pattern[p2] != '*')
      return 0; //false;
    p2++;
  }
  return 1;//true;
} 
/*********************************************************************************
Function:      //
Description:   // 
Input:         // 
//
// 
Return:        //  
Others:        //

*********************************************************************************/
void StringDel(char * targ_str, unsigned long length)
{
  unsigned long itemp = 0;
  while (1)
  {
   targ_str[itemp] = targ_str[length + itemp];
   if('\0' == targ_str[itemp])
   {break;}
   itemp ++;
  }
}  
/*********************************************************************************
Function:      //
Description:   // 比对两个字符串是否相等
Input:          // 
//    
// 
Return:        //  
Others:        //

*********************************************************************************/
unsigned char MarkString (unsigned char*String,unsigned char*Mark)
{
  unsigned char i =0;  
  while((String[i]==Mark[i])
        && (String[i]!='\0')
          && (Mark[i]!='\0'))
  { i++;}
  
  if('\0'==Mark[i])
  {i =1;}
  else
  {i = 0;}
  return i;
}
/*********************************************************************************
Function:      //
Description:   // 搜寻 特定字符串 在字符串中首次出现位置
Input:          // 
//    
// 
Return:        //  
Others:        //

*********************************************************************************/
unsigned char SeekMarkString (unsigned char*String,
                              unsigned char* mark,
                              unsigned long *seat)
{
  unsigned char Result = 0;
  unsigned long cnt = 0;
  unsigned long itemp;
  
  while('\0' != String[cnt])
  {
   if( 0!= SeekChar (&(String[cnt]),mark[0],&itemp))
   {
     cnt += itemp;
     if(0 != MarkString (&(String[cnt]),mark))
     {
       Result = 1;
       break;
     }
     else
     {cnt ++;}
   }
   else
   {cnt += itemp;}
  }
  *seat = cnt;
  return Result;
}
/*********************************************************************************
Function:      //
Description:   // 比对两个字符串是否相等
Input:          // 
//    
// 
Return:        //  
Others:        //

*********************************************************************************/
unsigned char ContrastString (unsigned char*String1,unsigned char*String2)
{
  unsigned char i =0;  
  while((String1[i]==String2[i])
        && (String1[i]!='\0')
          && (String2[i]!='\0'))
  { i++;}
  
  if(String1[i]==String2[i])
  {i =1;}
  else
  {i = 0;}
  return i;
}
/*********************************************************************************
Function:      //
Description:   //寻找并判别指示语句类型指示语句
Input:          // data     输入数据首地址
// length   输入数据长度
// indicator 存储指示语句的类型
Output:        //
Return:        //指向指示消息的本体   
Others:        //

*********************************************************************************/
unsigned short MatchinString (unsigned char **data,const char**StringList, unsigned short ListSize)
{
  const char *sp; 
  unsigned char *tp; 
  unsigned short itemp = 0; 
  do 
  {
    sp = StringList[itemp];
    tp = *data;
    while((*sp != '\0')&&(*sp++ == *tp++));
    if(*sp == '\0')
    { 
      *data = tp;
      break;
    }
  }while(++itemp < ListSize);
  return itemp;
}
