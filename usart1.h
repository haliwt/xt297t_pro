#ifndef __USART1_H__
#define __USART1_H__
#include <cms.h>
#include "mytype.h"


struct senddata{
     uint8_t windlevel ;
	 uint8_t windMotorRunflg : 1;  //室外的风机打开电源
	 uint8_t lampflg : 1;   //lampflg = 1 on ,lampflg =0 off
	 uint8_t timerTim :1;   //timer
	 uint8_t powerflg:1;
	 uint8_t UpDownRunflg :1; //motor =1 up move motor =0 down move
	 

};
extern struct  senddata ref;

void USART1_Init(void);
void USART1_SendData(void);
void delay_ns(unsigned char t);
void Set_Usart_Async(void);

uint8_t BCC(void);







#endif 