
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
	static uint8_t childflg =0 ,timerflg =0,windflg =0;
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
				 Delay_nms (3000);
				 childflg = childflg ^ 0x01;
				 if((KeyOldFlag & 0x01) && (KeyOldFlag & 0x02)){
					 if(childflg ==1){
								
								ref.childLock = 1;
								Led4=1;
					 }
					 else{
					 	ref.childLock = 0;

					 }
				 
				 }
	       }
         

       #endif 
		if(KeyOldFlag & 0x01)
		{
				if(0 == (KeyREFFlag & 0x01)) //定时按键
				{
					KeyREFFlag |= 0x01;
					timerflg = timerflg ^ 0x01;
					if(timerflg ==1){
							Led8=1;
							ref.timerTim = 1;
					}
					else{
						     Led8=0;
							ref.timerTim = 0;

					}
				}
		}
		else
		{
			KeyREFFlag &= ~0x01;
			
		}
			
			if(KeyOldFlag & 0x02)  //风速调节按键
			{
				if(0 == (KeyREFFlag & 0x02))
				{
					KeyREFFlag |= 0x02;
					windflg ++;
					if(windflg==1){
						Led6=1;
						Led1=0;
					    Led9=0;
						Led7 =0;
						ref.windlevel =1;
						
					}
					else if(windflg ==2){ //2档
						Led1=1;
						Led6=0;
					    Led9=0;
						Led7 =0;
						ref.windlevel =2;
					}
					else if(windflg ==3){ //3档
						  Led9 =1;
						  Led1=0;
					      Led6=0;
						  Led7 =0;
						  ref.windlevel =3;
					}
					else if(windflg ==4){ //Auto 
						  ref.windlevel =4;
						  windflg=0;
						  Led7 =1;
						  Led9 =0;
						  Led1=0;
					      Led6=0;
						
					}

				}
			}
			else
			{
				KeyREFFlag &= ~0x02;
				Led2=0;
			}
			
			if(KeyOldFlag & 0x04) //滤网置换按键
			{

                 Delay_nms (3000);
				 Delay_nms (3000);
				if(KeyOldFlag & 0x04)
				{
					KeyREFFlag |= 0x04;
					ref.filterNet =1;
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
	ref.senddata=(ref.windlevel  | ref.filterNet<< 4 | ref.timerTim <<5 | ref.sleep <<6 |ref.childLock << 7 ) & 0xff;
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
			Set_Usart_Async();
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



