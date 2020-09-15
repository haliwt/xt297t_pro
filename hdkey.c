#include "hdkey.h"
#include "hdkey.h"




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
	 static  uint8_t power =0;
                    power =power ^ 0x01;
					if(power==1) {//motor up move
					   ref.UpDownRunflg =1; //motor up move 
						keyLed4=1;
						
						//sendflg =1;
					}
					else{
					
					    ref.UpDownRunflg =0; //motor down move
						keyLed4=0; //shut dwon led key
					
						//sendflg =1;

					}
   
	
	
}
