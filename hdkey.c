#include "hdkey.h"
#include "hdkey.h"


 static  uint8_t power =0;

/*****************************************************************
	*
	*Function Name :void Flash_DisplayNumber(void)
	*Function:read flash data
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************/
void PowerOn_Fun(void)
{
   power =power ^ 0x01;
					if(power==1) {//motor up move
					   ref.UpDownRunflg =1; //motor up move 
						keyLed4=1;
						keyLed3=0;
						keyLed2=0;
						keyLed1=0;
						//sendflg =1;
					}
					else{
					
					    ref.UpDownRunflg =0; //motor down move
						keyLed4=0; //shut dwon led key
						keyLed3=0;
						keyLed2=0;
						keyLed1=0;
						//sendflg =1;

					}
   
	
	
}
