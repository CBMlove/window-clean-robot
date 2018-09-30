#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "SCI.h"
typedef char u8;
typedef unsigned int u16;
/***************************************************
//���ߵ��Ե�ʱ�����bug
//���ҵ��ֹ֮ͣ������������������
**********************************************/
/***************************************************
//���ߵ��Ե�ʱ�����bug
//���ҵ��ֹ֮ͣ������������������
**********************************************/
#define BUZZER PTBD_PTBD6
#define RELAY1 PTCD_PTCD6
#define RELAY2 PTCD_PTCD7
#define RELAY3 PTFD_PTFD4
#define RELAY4 PTFD_PTFD5
#define RELAY5 PTFD_PTFD6
#define RELAY6 PTGD_PTGD3
#define RELAY7 PTDD_PTDD6

u8 time_cnt;
u8 time_1ms_cnt;
u8 time_10ms_cnt;
u8 time_100ms_cnt;

u8 motor_speed=0;
u8 motor_direction=0;

void mcu_init(void);
void io_init(void);
void timer_init(void);							//TPM����
void motor_control(u8 direction,u8 speed);
void main(void) 
{
	mcu_init();
	io_init();
	//uart_init();
	timer_init();
  EnableInterrupts; /* enable interrupts */
  /* include your code here */


  for(;;) {
		if(time_cnt>=10)								//1ms����
		{
			time_cnt=0;
			time_1ms_cnt++;
		}
		if(time_1ms_cnt>=10)						//10ms����
		{
			time_1ms_cnt=0;
			time_10ms_cnt++;
		}
		if(time_10ms_cnt>=200)						//100ms����
		{
			time_10ms_cnt=0;
			BUZZER=~BUZZER;
			motor_control(motor_direction,motor_speed);
		}
    __RESET_WATCHDOG(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}

void mcu_init(void) 
{
 	SOPT1 = 0xD3;                   	//ʹ�ܿ��Ź�
  MCGC2 = 0x36;
  while(!(MCGSC & 0x02));		      	//��ʼ���ⲿ����8MHZ
  MCGC1 = 0x98;
  while((MCGSC & 0x1C ) != 0x08); 	//ѡ���ⲿʼ��8��Ƶ
	MCGC3 = 0x48;
	while ((MCGSC & 0x48) != 0x48);		//����PLL 32��Ƶ
	MCGC1 = 0x18;
  while((MCGSC & 0x6C) != 0x6C);  	//ѡ��PLLʱ����Ϊϵͳʱ�ӣ�����ʱ��Ϊ1/2ϵͳʱ��
}

void io_init(void)
{
	PTCDD=0xC0;												//C67������̵���
	PTFDD=0x70;												//F456������̵���
	PTGDD=0x08;												//G3������̵���
	PTDDD=0x40;												//D6������̵���
	PTBDD_PTBDD6=1;										//B6�����������
	//PTDDD=0XFF;												//D�����LED
}

void timer_init(void)
{
	TPM2SC=0x48;											//TOF�ж�ʹ�ܡ�����ʱ�ӡ�Ԥ��Ƶ1
	TPM2MOD=0x0640;										//ģʽ�Ĵ�����ֵ
}

void motor_control(u8 direction,u8 speed)
{
	if(direction)
		RELAY7=1;												//��ת
	else
		RELAY7=0;												//��ת
	switch(speed)
	{
		case 0:
			RELAY1=1;RELAY2=1;RELAY3=1;
			RELAY4=1;RELAY5=1;RELAY6=1;
			break;
		case 1:
			RELAY1=1;RELAY2=1;RELAY3=1;
			RELAY4=1;RELAY5=1;RELAY6=0;			
			break;
		case 2:
			RELAY1=1;RELAY2=1;RELAY3=1;
			RELAY4=1;RELAY5=0;RELAY6=0;			
			break;
		case 3:
			RELAY1=1;RELAY2=1;RELAY3=1;
			RELAY4=0;RELAY5=0;RELAY6=0;			
			break;
		case 4:
			RELAY1=1;RELAY2=1;RELAY3=0;
			RELAY4=0;RELAY5=0;RELAY6=0;			
			break;
		case 5:
			RELAY1=1;RELAY2=0;RELAY3=0;
			RELAY4=0;RELAY5=0;RELAY6=0;
			break;
		case 6:
			RELAY1=0;RELAY2=0;RELAY3=0;
			RELAY4=0;RELAY5=0;RELAY6=0;
			break;
		default:
			RELAY1=0;RELAY2=0;RELAY3=0;
			RELAY4=0;RELAY5=0;RELAY6=0;			
			break;
	}
}

/**********�жϴ�����*********/
interrupt 14 void timer_irq(void)	//0.1ms��ʱ���ж�
{
	TPM2SC_TOF=0;
	time_cnt++;
}
