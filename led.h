#ifndef __LED_H__
#define __LED_H__
#include <cms.h>

//led output port config 
#define     LED1	RA3
#define     LED2 	RA4
#define		LED3    RB0
#define		LED4	RB2
#define     LED5    RB1
#define     LED6    RB5
#define     LED7    RC1
#define     LED8    RC2
#define     LED9    RC3
#define     LED_POWER_RED    RD2   //ľçÔ´°´źüÖ¸ĘžľĆ


void TouchKey_1_LEDOn(void);
void TouchKey_1_LEDOff(void);
void TouchKey_2_LEDOn(void);
void TouchKey_2_LEDOff(void);

void TouchKey_3_LEDOn(void);
void TouchKey_3_LEDOff(void);

void PowerKey_LEDOn(void);
void PowerKey_LEDOff(void);



#endif 