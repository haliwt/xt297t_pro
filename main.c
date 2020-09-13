
#include <cms.h>
#include "delay.h"
#include "mytype.h"
#include "Touch_Kscan_Library.h"
#include "REL_Sender.h"
#include "usart1.h"
#include "keyled.h"
#include "slideled.h"


/**********************************************************************/
/*??????????*/
/**********************************************************************/
volatile unsigned char tcount;
volatile bit buzf;
volatile unsigned int buzsec;

volatile unsigned char DispData;

uint16_t usartNum;
uint8_t senddata[2];
/**********************************************************************/
/**********************************************************************/
/***********************************************************************
????????????????????????
????????
?????????
?????
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
	TRISC = 0x00;
	TRISD = 0x00;
	OPTION_REG = 0;
	OSCCON = 0x71;
	PIE1 = 0;
	PIE2 = 0;
	IOCB = 0;
	WPUA = 0;
	WPUB = 0;


}
/***********************************************************************
???????????????RAM???
????????
?????????
?????
***********************************************************************/
void Init_ram (void)
{
	asm("clrwdt");
	PIE2 = 0;
	PIE1 = 0B00000010;
	PR2 = 125;				//8M???TMR2?????125us???
	T2CON = 5;				//???????2
	
	INTCON = 0XC0;			//??????
	buzf = 1;
	buzsec = 600;
}
/***********************************************************************
???????????????????
????????
?????????
?????
***********************************************************************/
void Sys_set (void)
{
	asm("clrwdt");
	WDTCON = 0x01;
	TRISA = 0B00000000;
	TRISB = 0B00000000;
	TRISC = 0x00;
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
//??????????
***********************************************************************/
void Kscan()
{
	uint8_t j;
	static unsigned int KeyOldFlag = 0,KeyREFFlag = 0;
	static uint8_t lamp =0 ,timerflg =0,windflg =0,powerflg=0,sendflg=0;
	unsigned int i = (unsigned int)((KeyFlag[1]<<8) | KeyFlag[0]);

	if(i)
	{
		if(i != KeyOldFlag)
		{
			KeyOldFlag = i;			//????????

			buzf = 1;
			buzsec = 600;
		
		if(KeyOldFlag & 0x01)
		{
				if(0 == (KeyREFFlag & 0x01)) //KEY 2 LAMP_KEY
				{
					lamp =lamp ^ 0x01;
					if(lamp==1){
						ref.lampflg = 1; //��lamp
						keyLed4=0;
						keyLed3=0;
						keyLed2=1;
						keyLed1=0; 
						sendflg =1;
					}
					else{
						ref.lampflg = 0; //��lamp
						keyLed4=0;
						keyLed3=0;
						keyLed2=0;//lamp off shut dwon key of led 
						keyLed1=0;
						sendflg =1;
					}
				}
			if(KeyOldFlag & 0x02)  //KEY1 ---TIMER_KEY
				if(0 == (KeyREFFlag & 0x02))
				{
					timerflg = timerflg ^ 0x01;
					if(timerflg ==1) {//motor up move
					 
						keyLed4=0;
						keyLed3=0;
						keyLed2=0;
						keyLed1=1;
						sendflg =1;
					}
					else{
					
						keyLed4=0;
						keyLed3=0;
						keyLed2=0;
						keyLed1=0; //shut dwon key led
						sendflg =1;
					}

				}
			}
			
		  if(KeyOldFlag & 0x04) //KEY4 POWER_KEY --motor up and dow move
			{
				if(0 == (KeyREFFlag & 0x04))
				{
					powerflg = powerflg ^ 0x01;
					if(powerflg ==1) {//motor up move
					   ref.UpDownRunflg =1; //motor up move 
						keyLed4=1;
						keyLed3=0;
						keyLed2=0;
						keyLed1=0;
						sendflg =1;
					}
					else{
					    ref.UpDownRunflg =0; //motor down move
						keyLed4=0; //shut dwon led key
						keyLed3=0;
						keyLed2=0;
						keyLed1=0;
						sendflg =1;

					}
				}

			}
		   if(KeyOldFlag & 0x08) //KEY3  Wind_KEY 
			{
				if(0 == (KeyREFFlag & 0x08))
				{
					windflg =windflg ^ 0x01;
					if(windflg ==1){
						ref.windMotoRunrflg = 1; //turn on windows out motor 
						
						keyLed4=0;
						keyLed3=1;
						keyLed2=0;
						keyLed1=0;
						sendflg =1;
				    }
					else{
						ref.windMotoRunrflg = 0;
						
						keyLed4=0;
						keyLed3=0;
						keyLed2=0;
						keyLed1=0;
						sendflg =1;


					}
				}

			}
		  /** slide_touch_key  **/
		   if(KeyOldFlag & 0x10) //???????????
			{
				if(0 == (KeyREFFlag & 0x10))
				{
					SldLed_1 =1;
					SldLed_2 = 0;
					SldLed_3 =0;
					SldLed_4 =0;
					SldLed_5 =0;
					SldLed_6 =0;
					SldLed_7 =0;
					SldLed_8 =0;

				}

			}
		   if(KeyOldFlag & 0x20) //???????????
			{
				if(0 == (KeyREFFlag & 0x20))
				{
					SldLed_1 =0;
					SldLed_2 = 1;
					SldLed_3 =0;
					SldLed_4 =0;
					SldLed_5 =0;
					SldLed_6 =0;
					SldLed_7 =0;
					SldLed_8 =0;

				}

			}
		   if(KeyOldFlag & 0x40) //
			{
				if(0 == (KeyREFFlag & 0x40))
				{
				   SldLed_1 =0;
					SldLed_2 = 0;
					SldLed_3 =1;
					SldLed_4 =0;
					SldLed_5 =0;
					SldLed_6 =0;
					SldLed_7 =0;
					SldLed_8 =0;

				}

			}
		   if(KeyOldFlag & 0x80) //
			{
				if(0 == (KeyREFFlag & 0x80))
				{
					SldLed_1 =0;
					SldLed_2 = 0;
					SldLed_3 =0;
					SldLed_4 =1;
					SldLed_5 =0;
					SldLed_6 =0;
					SldLed_7 =0;
					SldLed_8 =0;
				}

			}
		   if(KeyOldFlag & 0x100) //
			{
				if(0 == (KeyREFFlag & 0x100))
				{
					SldLed_1 =0;
					SldLed_2 = 0;
					SldLed_3 =0;
					SldLed_4 =0;
					SldLed_5 =1;
					SldLed_6 =0;
					SldLed_7 =0;
					SldLed_8 =0;
				}

			}
		   if(KeyOldFlag & 0x200) //???????????
			{
				if(0 == (KeyREFFlag & 0x200))
				{
					SldLed_1 =0;
					SldLed_2 = 0;
					SldLed_3 =0;
					SldLed_4 =0;
					SldLed_5 =0;
					SldLed_6 =1;
					SldLed_7 =0;
					SldLed_8 =0;

				}

			}
		   if(KeyOldFlag & 0x400) //???????????
			{
				if(0 == (KeyREFFlag & 0x400))
				{
					SldLed_1 =0;
					SldLed_2 = 0;
					SldLed_3 =0;
					SldLed_4 =0;
					SldLed_5 =0;
					SldLed_6 =0;
					SldLed_7 =1;
					SldLed_8 =0;

				}

			}
			if(KeyOldFlag & 0x800) //???????????
			{
				if(0 == (KeyREFFlag & 0x800))
				{
					SldLed_1 =0;
					SldLed_2 = 0;
					SldLed_3 =0;
					SldLed_4 =0;
					SldLed_5 =0;
					SldLed_6 =0;
					SldLed_7 =0;
					SldLed_8 =1;

				}

			}
		}
	}
	
	else
	{
		KeyOldFlag = 0;
		KeyREFFlag = 0;
	}
	if(sendflg ==1){
		//usartNum =0;
		 Sys_set ();
		sendflg =0;
		for(i=0;i<=j;i++){
			senddata[0]=(ref.windMotorRunflg<< 4 | ref.timerTim <<1 |ref.lampflg << 0 |ref.powerflg <<7) & 0xff;
			senddata[1]=();
			USART1_SendData();
		}
	}
}


/***********************************************************************
??????????????????
***********************************************************************/
void interrupt time0(void)
{
	if(TMR2IF)
	{
		usartNum ++ ;
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
main??????
***********************************************************************/
void main(void)
{
    static uint8_t poweron=0;
	asm("clrwdt");
	USART1_Init();
	Init_ic();
	Delay_nms(200);													//??????200ms
	Init_ram();	  //???????
	Set_Usart_Async();
	
	
	while(1)
	{
		OSCCON = 0x71;
	
	

		if(tcount >= 32)
		{
			tcount = 0;												//?????????4ms
			Sys_set();
			//Display();
			#if (REL_SENDER_ENABLE == 1)//???????????1
				REL_SenderLoop();//?????????
			#endif
			__CMS_CheckTouchKey();	//?????
			Kscan();			//????????
		}
	}
}
/**********************************************************************/



