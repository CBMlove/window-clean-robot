#ifndef __SCI_H
#define __SCI_H
unsigned char uart_getkey1(void);
unsigned char uart_getkey2(void);
void uart_init(void);
void uart_putchar1(unsigned char ch);
void putstr1(char ch[]);
void uart_putchar2(unsigned char ch);
void putstr2(char ch[]);
void Word2Str5(char zifu[],word val,byte StartPtr);
void Byte2Str3(char zifu[],byte val,byte StartPtr);
//unsigned char TERMIO_GetChar(void);
//void printp( char * , ...);
#endif