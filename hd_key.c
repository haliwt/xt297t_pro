#include "hd_key.h"



void HDKEY_LED_Init(void)
{


	WPUA = 0B00000001;				//配置上拉情况
	WPUB = 0B00000000;
	WPUC = 0B00000000;
	
	TRISA = 0B00000000;
	TRISB = 0B00000000;

	TRISC = 0B00000001;
	TRISD = 0x00;
	
}