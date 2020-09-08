#include "led.h"

/*****************************************************************
	*
	*Function Name :void TouchKey_1_LED(void)
	*Function:read flash data
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************/
void TouchKey_1_LEDOn(void)
{
    LED1 =1;
	LED2 =1;
	LED4 = 0;

}
/*****************************************************************
	*
	*Function Name :void TouchKey_1_LEDOff(void)
	*Function:read flash data
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************/
void TouchKey_1_LEDOff(void)
{
    LED1 =0;
	LED2 =0;
	LED4 = 1;

}
/*****************************************************************
	*
	*Function Name :void TouchKey_2_LEDOn(void)
	*Function:read flash data
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************/
void TouchKey_2_LEDOn(void)
{
   LED3 =1;
   LED5=1;
   LED8 = 0;


}

void TouchKey_2_LEDOff(void)
{
   LED3 =0;
   LED5=0;
   LED8 = 1;


}
/*****************************************************************
	*
	*Function Name :void TouchKey_3_LEDOn(void)
	*Function:read flash data
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************/
void TouchKey_3_LEDOn(void)
{
   LED6=1;
   LED7=1;
   LED9 = 0;



}
void TouchKey_3_LEDOff(void)
{
   LED6=0;
   LED7=0;
   LED9 = 1;
}
/*****************************************************************
	*
	*Function Name :void PowerKey_LEDOn(void)
	*Function:read flash data
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************/
void PowerKey_LEDOn(void)
{
    LED_POWER_RED =1;

}
/*****************************************************************
	*
	*Function Name :void PowerKey_LEDOff(void)
	*Function:read flash data
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************/
void PowerKey_LEDOff(void)
{
   LED_POWER_RED = 0;

}