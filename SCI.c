/********************************************************
龙丘MC9S08DZ60多功能USB开发板V2.0
编写:龙丘
Designed by Chiu Sir
E-mail:chiusir@163.com
软件版本:V1.0
最后更新:2009年07月11日
相关信息参考下列地址:
博客:  http://longqiu.21ic.org
淘宝店:http://shop36265907.taobao.com 
*********************************************************/
#include <MC9S08DZ60.h>


         
unsigned char uart_getkey1(void)
{  
   while(!(SCI1S1&0x80)) ; 		 //keep waiting when not empty  
   return SCI1D;
}         
unsigned char uart_getkey2(void)
{  
   while(!(SCI2S1&0x80)) ; 		 //keep waiting when not empty  
   return SCI2D;
}

void uart_init(void) {

  SCI1C2 =0x2c;
  SCI1BDH=0x00;
  SCI1BDL=0x68;//16MHz,115200bps,SCI0BDL=0x09 
  
  SCI2C2 =0x2c;
  SCI2BDH=0x00;
  SCI2BDL=0x68;//16MHz, 9600bps,SCI0BDL=0x68 
}             


void uart_putchar1(unsigned char ch)
{ 
   while(!(SCI1S1&0x80)) ; 		 //keep waiting when not empty  
   SCI1D=ch;
}
void uart_putchar2(unsigned char ch)
{ 
   while(!(SCI2S1&0x80)) ; 		 //keep waiting when not empty  
   SCI2D=ch;
}

void putstr1(char ch[])
{
  unsigned char ptr=0;
  while(ch[ptr])
  {
      uart_putchar1((unsigned char)ch[ptr++]);
  } 
}
void putstr2(char ch[])
{
  unsigned char ptr=0;
  while(ch[ptr])
  {
      uart_putchar2((unsigned char)ch[ptr++]);
  } 
}

/***************************************************
  把0--255的数值转化为3位字符串格式
****************************************************/
void Byte2Str3(char zifu[],byte val,byte StartPtr)
{
  char characters[11]="0123456789";   
  byte tv=0;
  
  tv=val/100;
  zifu[StartPtr++] = characters[tv];
  tv=(val%100)/10;
  zifu[StartPtr++] = characters[tv];
  tv=val%10;
  zifu[StartPtr] = characters[tv];
  //zifu[3] = '\0';
  return;
}

/***************************************************
  把0--65535的数值转化为5位字符串格式
  char zifu[]:  目的字符串数组
  word val:     需要转换的整形数据
  byte StartPtr:数据在字符串中的偏移位置
  例如：输出为Vol:12345V,偏移为4,在其中连续插入12345,其他格式需要自定义.
****************************************************/
void Word2Str5(char zifu[],word val,byte StartPtr)
{
  char characters[11]="0123456789";   
  word tv=0;
  
  tv=val/10000;
  zifu[StartPtr++] = characters[tv];
  tv=(val%10000)/1000;
  zifu[StartPtr++] = characters[tv];
  tv=(val%1000)/100;
  zifu[StartPtr++] = characters[tv];
  tv=(val%100)/10;
  zifu[StartPtr++] = characters[tv];
  tv=val%10;
  zifu[StartPtr] = characters[tv];
  //zifu[5] = '\0';
  return;
}

