
#include <cms.h>
#include "delay.h"
#include "mytype.h"
#include "Touch_Kscan_Library.h"
#include "REL_Sender.h"
#include "usart1.h"
#include "keyled.h"
#include "slideled.h"
#include "hdkey.h"


/**********************************************************************/
/*??????????*/
/**********************************************************************/
volatile unsigned char tcount;
volatile bit buzf;
volatile unsigned int buzsec;

volatile unsigned char DispData;

uint16_t usartNum;
uint8_t senddata[2];
 
uint8_t Order;
uint8_t number;
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
	Clr(TRISD, 0); //code down load code 
	Clr(TRISD, 1);
	OPTION_REG = 0;
	OSCCON = 0x71;
	PIE1 = 0;
	PIE2 = 0;
	IOCB = 0;
	WPUA = 0;
	WPUB = 0;
	
	
	
//	INTCON = 0xC0;			//ÔÊÐíËùÓÐÎ´±»ÆÁ±ÎµÄÖÐ¶Ï¡¢ÍâÉèÖÐ¶Ï


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
	uint8_t j,keyevent=0;
	static unsigned int KeyOldFlag = 0,KeyREFFlag = 0;
	static uint8_t  power =0;
	static uint8_t lamp ,timerflg ,windflg ,powerflg,sendflg;
	static uint8_t slidekey_1 ,slidekey_2,slidekey_3,slidekey_4,slidekey_5,slidekey_6,slidekey_7,slidekey_8;
	unsigned int i = (unsigned int)((KeyFlag[1]<<8) | KeyFlag[0]);

	if(i)
	{
		if(i != KeyOldFlag)
		{
			KeyOldFlag = i;			//????????

			buzf = 1;
			buzsec = 600;
		    keyevent =0;
		
		if(KeyOldFlag & 0x01)
		{
			 if(0 == (KeyREFFlag & 0x01)) //KEY 2 LAMP_KEY  ²¶»ñ = 1 ÏÂ½µÑØ
				{
					lamp =lamp ^ 0x01;
					if(lamp==1&&keyevent ==0){
						ref.lampflg = 1; // turn on lamp
					    keyevent =1;
						keyLed2=1;
						
						
					     RB4= 0;
						 delay_ms(10);
						 RB4=1;

					}
					else if(keyevent !=1){
						ref.lampflg = 0; //turn off lamp
						keyevent =1;
						keyLed2=0;
					
						 RB4= 0;
						 delay_ms(10);
						 RB4=1;
						
						
					}
				}
			}
		
			if(KeyOldFlag & 0x02)  //KEY1 ---TIMER_KEY capture = 5 
			{
				if(0 == (KeyREFFlag & 0x02))
				{
					timerflg = timerflg ^ 0x01;
					if(timerflg ==1&& keyevent==0) {//motor up move
					 
						ref.timerTim =1;
						keyevent =1;
						keyLed1=1;
						
						
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
 					    delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						
					}
					else if(keyevent!=1){
					
						keyevent =1;
						ref.timerTim =0;
						
						keyLed1=0; //shut dwon key led
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
 					    delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
					}

				}
			}
			
		  if(KeyOldFlag & 0x04) //KEY4 POWER_KEY --²¶»ñ =2
		  {
				if(0 == (KeyREFFlag & 0x04))
				{
					 
                    power =power ^ 0x01;
					if(power==1&& keyevent ==0) {//motor up move
					   ref.UpDownRunflg =1; //motor up move 
						keyLed4=1;
						keyevent =1;
						
					   PRB4=0; //data P25
						delay_ms(4);
						PRB4=1;
 					    delay_ms(4);
						PRB4=0; //data P25
						delay_ms(4);
						PRB4=1;
				   }
					else if(keyevent !=1){
					
					    keyevent=1;
					    ref.UpDownRunflg =0; //motor down move
						keyLed4=0; //shut dwon led key
						PRB4=0; //data P25
						delay_ms(4);
						PRB4=1;
 					    delay_ms(4);
						PRB4=0; //data P25
						delay_ms(4);
						PRB4=1;
						 
						 }
   
					
					
				}

			}
		   if(KeyOldFlag & 0x08) //KEY3  Wind_KEY  cpature =3
			{
				if(0 == (KeyREFFlag & 0x08))
				{
					windflg =windflg ^ 0x01;
					if(windflg ==1 && keyevent ==0){
						windflg =1;
						ref.windMotorRunflg = 1; //turn on windows out motor 
						keyevent =1;
						keyLed3=1;
						
					    PRB4=0; //data P25
						delay_ms(2);
						PRB4=1;
 					    delay_ms(2);
						PRB4=0; //data P25
						delay_ms(2);
						PRB4=1;
						delay_ms(2);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						
						
				    }
					else if(keyevent !=1){
						ref.windMotorRunflg = 0;
						keyevent =1;
					
						keyLed3=0;
						
					    PRB4=0; //data P25
						delay_ms(2);
						PRB4=1;
 					    delay_ms(2);
						PRB4=0; //data P25
						delay_ms(2);
						PRB4=1;
						delay_ms(2);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;


					}
				}

			}
		  /******************* slide_touch_key  *****************************/
		  /******************************************************************/
		   if(KeyOldFlag & 0x10) //slide_touch_key_1 capture =1
			{
				 PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú
				if(0 == (KeyREFFlag & 0x10))
				{
					slidekey_1 = slidekey_1 ^ 0x01;
					if(slidekey_1==1 && keyevent ==0){
						keyevent =1;
						SldLed_1 =1;
						
						PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú

						PRB4=0; //data P25
						delay_ms(20);
						PRB4=1;
						slidekey_1 =1;
		
					}
					else if(keyevent !=1){ 
						keyevent =1;
						SldLed_1 =0; //slide touch key 
						sendflg =1;
						PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú

						PRB4=0; //data P25
						delay_ms(20);
						PRB4=1;
						slidekey_1 =0;
						

					}

				}

			}
			
		   if(KeyOldFlag & 0x20) //slide_touch_key_2   cpature =2
			{
				 PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú
				if(0 == (KeyREFFlag & 0x20))
				{
					slidekey_2 = slidekey_2 ^ 0x01;
					if(slidekey_2==1 && keyevent ==0){
						 keyevent =1;
					keyevent =1;
					SldLed_2 = 1;
					sendflg =1;
					PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú

						PRB4=0; //data P25
						delay_ms(4);
						PRB4=1;
 					    delay_ms(4);
						PRB4=0; //data P25
						delay_ms(4);
						PRB4=1;
						RB3=1;
						slidekey_2 =1;
					
					}
					else if(keyevent !=1){
								keyevent =1;
							SldLed_2 = 0;
							sendflg =1;
							PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú

					PRB4=0; //data P25
						delay_ms(4);
						PRB4=1;
 					    delay_ms(4);
						PRB4=0; //data P25
						delay_ms(4);
						PRB4=1;
						RB3=1;

						slidekey_2 =0;
					}

				}

			}
			
		   if(KeyOldFlag & 0x40) //slide_touch_key_3 capture =3
			{
				 PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú
				if(0 == (KeyREFFlag & 0x40))
				{
					slidekey_3 = slidekey_3 ^ 0x01;
					if(slidekey_3==1&& keyevent ==0){
						keyevent =1;
						SldLed_3 =1;
	
						PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú

						 PRB4=0; //data P25
						delay_ms(2);
						PRB4=1;
 					    delay_ms(2);
						PRB4=0; //data P25
						delay_ms(2);
						PRB4=1;
						delay_ms(2);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						RB3=1;
						slidekey_3 =1;
					
						}
					else if(keyevent !=1){
							keyevent =1;
							SldLed_3 =0;
							sendflg =1;
							PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú

						    PRB4=0; //data P25
						delay_ms(2);
						PRB4=1;
 					    delay_ms(2);
						PRB4=0; //data P25
						delay_ms(2);
						PRB4=1;
						delay_ms(2);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						RB3=1;
						slidekey_3 =0;

					}

				}

			}
			
		   if(KeyOldFlag & 0x80) //slide_touch_key_4 capture =4
			{
				 PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú
				if(0 == (KeyREFFlag & 0x80))
				{
					
					slidekey_4 = slidekey_4 ^ 0x01;
					if(slidekey_4==1&& keyevent ==0){
					 keyevent =1;
					   SldLed_4 =1;
						sendflg =1;
						PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú

					    PRB4=0; //data P25
						delay_ms(2);
						PRB4=1;
 					    delay_ms(2);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1; //data P25
						RB3=1;
						slidekey_4 =1;
					
						}
					else if(keyevent !=1){

					keyevent =1;
						SldLed_4 =0;
						sendflg =1;
						PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú

						 PRB4=0; //data P25
						delay_ms(2);
						PRB4=1;
 					    delay_ms(2);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1; //data P25
						RB3=1;
						slidekey_4 =0;
					}
				}

			}
		
		   if(KeyOldFlag & 0x100) ////slide_touch_key_5  capture =5
			{
				 PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú
				if(0 == (KeyREFFlag & 0x100))
				{
					slidekey_5 = slidekey_5 ^ 0x01;
					if(slidekey_5==1&& keyevent ==0){
							SldLed_5 =1;
							sendflg =1;
							PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú
							keyevent =1;

						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
 					    delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						RB3=1;
						slidekey_5 =1;
						}
					else if(keyevent !=1){
					keyevent =1;	
					SldLed_5 =0;
					sendflg =1;
					    PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú
						
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
 					    delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						RB3=1;
						slidekey_5 =0;

					}
					
				}

			}
	
		   if(KeyOldFlag & 0x200) //slide_touch_key_6 capture =  56 
			{
				 PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú
				if(0 == (KeyREFFlag & 0x200))
				{
					slidekey_6 = slidekey_6 ^ 0x01;
					if(slidekey_6==1&& keyevent ==0){
					 keyevent =1;
							SldLed_6 =1;
							sendflg =1;
							 PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú
							
						/*****6*****/
						PRB4=0; //data P25
						PRB4=0; //data P25
						delay_ms(2);
						PRB4=1;
 					    delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						/********6********/
						RB3=1;
						slidekey_6 =1;
					}
					else if(keyevent !=1){
						keyevent =1;
						SldLed_6 =0;
						sendflg =1;
						 PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú
						

						/*****6*****/
						PRB4=0; //data P25
						PRB4=0; //data P25
						delay_ms(2);
						PRB4=1;
 					    delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						/********6********/
						RB3 =1;
						slidekey_6 =0;
						}
				  }
			}
		
		   if(KeyOldFlag & 0x400) //slide_touch_key_7  capture = 54
			{
				 PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú
				if(0 == (KeyREFFlag & 0x400))
				{
					slidekey_7 = slidekey_7 ^ 0x01;
					if(slidekey_7==1&& keyevent ==0){
						 keyevent =1;
						SldLed_7 =1;
						sendflg =1;
						 PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú
						

						 PRB4=0; //data P25
						delay_ms(2);
						PRB4=1;
 					    delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						RB4=1;
					
						RB3=1;
						slidekey_7 =1;
						
						}
					else if(keyevent !=1){
						keyevent =1;
						SldLed_7 =0;
						sendflg =1;
						 PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú
							
						PRB4=0; //data P25
						delay_ms(2);
						PRB4=1;
 					    delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						RB4=1;

						RB3=1;
						slidekey_7 =0;

					}

				}

			}
	
			if(KeyOldFlag & 0x800) //slide_touch_key_8 capture =64
			{
				 PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú 
				if(0 == (KeyREFFlag & 0x800))
				{
					slidekey_8 = slidekey_8 ^ 0x01;
					if(slidekey_8==1&& keyevent ==0){
				       keyevent =1;
						SldLed_8 =1;
						sendflg =1;
						 PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú

						PRB4=0; //data P25
						PRB4=0; //data P25
						delay_ms(2);
						PRB4=1;
 					    delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;

						RB3=1;
						slidekey_8 =1;


						}
					else if(keyevent !=1){
						keyevent =1;
						SldLed_8 =0;
						sendflg =1;
						 PRB3 =0 ;   //·çËÙ·çÁ¿µ÷½Ú
						PRB4=0; //data P25
						delay_ms(2);
						PRB4=1;
 					    delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						delay_ms(1);
						PRB4=0; //data P25
						delay_ms(1);
						PRB4=1;
						RB3=1;
						slidekey_8 =0;
					}
     
				}

			}

		}
	
	else
	{
		KeyOldFlag = 0;
		KeyREFFlag = 0;
	}

	if(slidekey_1 ==1)SldLed_1 =1;
	else SldLed_1 =0;

	if(slidekey_2 ==1)SldLed_2 =1;
	else SldLed_2 =0;

	if(slidekey_3 ==1)SldLed_3 =1;
	else SldLed_3 =0;

	if(slidekey_4 ==1)SldLed_4 =1;
	else SldLed_4 =0;

	if(slidekey_5 ==1)SldLed_5 =1;
	else SldLed_5 =0;

	if(slidekey_6 ==1)SldLed_6 =1;
	else SldLed_6 =0;

	if(slidekey_7 ==1)SldLed_7 =1;
	else SldLed_7 =0;

	if(slidekey_8 ==1)SldLed_8 =1;
	else SldLed_8 =0;

	
}
}

/***********************************************************************
??????????????????
***********************************************************************/
void interrupt time0(void)
{
	if(TMR2IF)
	{
		
		TMR2IF = 0;
		tcount ++;
		__CMS_GetTouchKeyValue();
		
		usartNum ++ ;//125us  8´Î =1ms
		
		
	
	}
	else
	{
		PIR1 = 0;
		PIR2 = 0;
		PRB4=1;
	    PRB3=1;
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
	//Set_Usart_Async();
	TRISB = 0B00000000;
	
	while(1)
	{
		OSCCON = 0x71;
	
	   PRB4=1;
	   PRB3=1;

		if(tcount >= 32)
		{
			 PRB4=1;
	         PRB3=1;
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



