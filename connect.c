#include "connect.h"
/***********************************
	*
	*
	*
	*
************************************/
#if 0
void SendData_Signal(uint8_t senddata)
{
    switch(senddat){

	   case 0: //关机

	         CNRB3=0;
	         CNRB4 =0;
	         Delay_nms (5);

	        break;
	   case 1:  //开机
			CNRB3 = 1;
			CNRB4= 1 ;
	        Delay_nms(5);

	   break;

	   case 2 : //睡眠  
				CNRB3 =0;
				CNRB4 =1;
	            Delay_nms(5);

	   break;

	}





}
#endif 


