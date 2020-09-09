
#include <cms.h>
#include "led.h"
#include "delay.h"
#include "mytype.h"
#include "Touch_Kscan_Library.h"
#include "REL_Sender.h"
#include "usart1.h"

/**********************************************************************/
/*全局变量声明*/
/**********************************************************************/
volatile unsigned char tcount;
volatile bit buzf;
volatile unsigned int buzsec;

volatile unsigned char DispData;
uint8_t ChildLock;


/**********************************************************************/
/**********************************************************************/
/***********************************************************************
子函数功能：上电初始化系统寄存器
入口参数：
返回数据：
备注：
***********************************************************************/
void Init_ic (void)
{
	asm("clrwdt");
	PORTA = 0;
	PORTB = 0;
	INTCON = 0x00;
	PIR1 = 0;
	PIR2 = 0;
	WDTCON = 0x01;
	TRISA = 0B00000000;
	TRISB = 0B00000000;
	TRISC = 0x01;
	TRISD = 0x03;
	OPTION_REG = 0;
	OSCCON = 0x71;
	PIE1 = 0;
	PIE2 = 0;
	IOCB = 0;
	WPUA = 0;
	WPUB = 0;


}
/***********************************************************************
函数功能：初始上电RAM赋值
入口参数：
返回数据：
备注：
***********************************************************************/
void Init_ram (void)
{
	asm("clrwdt");
	PIE2 = 0;
	PIE1 = 0B00000010;
	PR2 = 125;				//8M下将TMR2设置为125us中断
	T2CON = 5;				//使能定时器2
	
	INTCON = 0XC0;			//使能中断
	buzf = 1;
	buzsec = 600;
}
/***********************************************************************
函数功能：系统寄存器刷新
入口参数：
返回数据：
备注：
***********************************************************************/
void Sys_set (void)
{
	asm("clrwdt");
	WDTCON = 0x01;
	TRISA = 0B00000000;
	TRISB = 0B00000000;
	TRISC = 0x01;
	TRISD = 0x00;
	OPTION_REG = 0;
	PIE1 = 0B00000010;
	PR2 = 125;
	INTCON = 0XC0;
	if(5 != T2CON)
		T2CON = 5;
	//WPUB = 0B01010010;
	//WPUA = 0B10000000;
}

/***********************************************************************
//键处理函数
***********************************************************************/
void Kscan()
{
	static unsigned int KeyOldFlag = 0,KeyREFFlag = 0;
	static uint8_t childflg =0 ;
	unsigned int i = (unsigned int)((KeyFlag[1]<<8) | KeyFlag[0]);

	if(i)
	{
		if(i != KeyOldFlag)
		{
			KeyOldFlag = i;			//有检测到按键

			buzf = 1;
			buzsec = 600;
		#if 1
			if((KeyOldFlag & 0x01) && (KeyOldFlag & 0x02))
			{
                 Delay_nms (3000);
				 childflg = childflg ^ 0x01;
				 if((KeyOldFlag & 0x01) && (KeyOldFlag & 0x02)){
				 if(childflg ==1){
							ChildLock =1;
				 }
				 else ChildLock = 0;
				 
				 }
	       }
          if(ChildLock ==1){
				Led4=1;
			}

       #endif 
		if(KeyOldFlag & 0x01)
			{
				if(0 == (KeyREFFlag & 0x01))
				{
					KeyREFFlag |= 0x01;
				
					Led1=1;
				}
			}
			else
			{
				KeyREFFlag &= ~0x01;
				Led1=0;
			}
			
			if(KeyOldFlag & 0x02)
			{
				if(0 == (KeyREFFlag & 0x02))
				{
					KeyREFFlag |= 0x02;
				
					Led2=1;
				}
			}
			else
			{
				KeyREFFlag &= ~0x02;
				Led2=0;
			}
			
			if(KeyOldFlag & 0x04)
			{
				if(0 == (KeyREFFlag & 0x04))
				{
					KeyREFFlag |= 0x04;
				
					Led3=1;
				}
			}
			else
			{
				KeyREFFlag &= ~0x04;
				Led3=0;
			}
		
			
		}
	}
	else
	{
		KeyOldFlag = 0;
		KeyREFFlag = 0;
	}
}


/***********************************************************************
函数功能：中断入口函数
***********************************************************************/
void interrupt time0(void)
{
	if(TMR2IF)
	{
		TMR2IF = 0;
		tcount ++;
		__CMS_GetTouchKeyValue();
	}
	else
	{
		PIR1 = 0;
		PIR2 = 0;
	}
}


/***********************************************************************
main主函数
***********************************************************************/
void main(void)
{
	uint8_t powerflg =0;
	asm("clrwdt");
	USART1_Init();
	Init_ic();
	Delay_nms(200);													//上电延时200ms
	Init_ram();	  //上电给初值
	
	
	
	while(1)
	{
		OSCCON = 0x71;
	
		powerflg = HDKey_Scan(1);
		if(powerflg==1){
			LED_RED = 1;
			USART1_SendData();
		}
		if(tcount >= 32)
		{
			tcount = 0;												//主程序循环4ms
			Sys_set();
			//Display();
			#if (REL_SENDER_ENABLE == 1)//调试宏定义是否为1
				REL_SenderLoop();//发码子程序
			#endif
			__CMS_CheckTouchKey();	//扫描按键
			Kscan();			//按键处理
		}
	}
}
/**********************************************************************/



