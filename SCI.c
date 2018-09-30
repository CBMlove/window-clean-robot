/********************************************************
����MC9S08DZ60�๦��USB������V2.0
��д:����
Designed by Chiu Sir
E-mail:chiusir@163.com
����汾:V1.0
������:2009��07��11��
�����Ϣ�ο����е�ַ:
����:  http://longqiu.21ic.org
�Ա���:http://shop36265907.taobao.com 
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
  ��0--255����ֵת��Ϊ3λ�ַ�����ʽ
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
  ��0--65535����ֵת��Ϊ5λ�ַ�����ʽ
  char zifu[]:  Ŀ���ַ�������
  word val:     ��Ҫת������������
  byte StartPtr:�������ַ����е�ƫ��λ��
  ���磺���ΪVol:12345V,ƫ��Ϊ4,��������������12345,������ʽ��Ҫ�Զ���.
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

