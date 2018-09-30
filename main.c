#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "SCI.h"
typedef char u8;
typedef unsigned int u16;
/***************************************************
//在线调试的时候存在bug
//并且电机停止之后，重新启动，带不动
**********************************************/
/***************************************************
//在线调试的时候存在bug
//并且电机停止之后，重新启动，带不动
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
void timer_init(void);							//TPM外设
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
		if(time_cnt>=10)								//1ms周期
		{
			time_cnt=0;
			time_1ms_cnt++;
		}
		if(time_1ms_cnt>=10)						//10ms周期
		{
			time_1ms_cnt=0;
			time_10ms_cnt++;
		}
		if(time_10ms_cnt>=200)						//100ms周期
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
 	SOPT1 = 0xD3;                   	//使能看门狗
  MCGC2 = 0x36;
  while(!(MCGSC & 0x02));		      	//初始化外部晶振8MHZ
  MCGC1 = 0x98;
  while((MCGSC & 0x1C ) != 0x08); 	//选择外部始终8分频
	MCGC3 = 0x48;
	while ((MCGSC & 0x48) != 0x48);		//配置PLL 32倍频
	MCGC1 = 0x18;
  while((MCGSC & 0x6C) != 0x6C);  	//选择PLL时钟作为系统时钟，总线时钟为1/2系统时钟
}

void io_init(void)
{
	PTCDD=0xC0;												//C67输出，继电器
	PTFDD=0x70;												//F456输出，继电器
	PTGDD=0x08;												//G3输出，继电器
	PTDDD=0x40;												//D6输出，继电器
	PTBDD_PTBDD6=1;										//B6输出，蜂鸣器
	//PTDDD=0XFF;												//D输出，LED
}

void timer_init(void)
{
	TPM2SC=0x48;											//TOF中断使能、总线时钟、预分频1
	TPM2MOD=0x0640;										//模式寄存器的值
}

void motor_control(u8 direction,u8 speed)
{
	if(direction)
		RELAY7=1;												//正转
	else
		RELAY7=0;												//反转
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

/**********中断处理函数*********/
interrupt 14 void timer_irq(void)	//0.1ms定时器中断
{
	TPM2SC_TOF=0;
	time_cnt++;
}
