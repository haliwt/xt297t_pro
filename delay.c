#include "delay.h"
#include "cms.h"
/**********************************************************************
**********************************************************************/
/***********************************************************************
�������ܣ���ʱ�Ӻ�����13��ָ������1ѭ��
***********************************************************************/
void Delay(unsigned int dtemp) 
{
	while(dtemp--);
}
/***********************************************************************
�Ӻ������ܣ���ʱtempl  ms�����ж���׼
***********************************************************************/
void Delay_nms (unsigned int inittempl)
{
	unsigned int	i;
	unsigned char	gtemp;

	gtemp=0;
	if(GIE==1)
	{
		gtemp=1;
		GIE=0;
	}
	for(i=0;i<inittempl;i++)
	{
		Delay(154);
		asm("clrwdt");
	}
	if(gtemp==1)	GIE=1;
}
/**********************************************************************
**********************************************************************/


















