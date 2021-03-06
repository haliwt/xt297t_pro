#ifndef _MYTYPE_H_
#define _MYTYPE_H_

/*******************************************************
					位定义
********************************************************/
#define	Set(x,y)	((x) |= (1 << (y)))				//set 1
#define	Clr(x,y)	((x) &= ~ (1 << (y)))			//set 0

#define	b0(x,y)		(((x) & (1 << (y))) == 0)		//位0判断 if(b0(x,y)) 判断是否为0
#define	b1(x,y)		((x) & (1 << (y)))				//位1判断 if(b1(x,y)) 判断是否为1
/*******************************************************
					宏定义
********************************************************/
#define uchar	unsigned char
#define uint	unsigned int
#define ulong	unsigned long


typedef uchar uint8_t;
typedef uint  int8_t;
typedef  ulong  uint16_t;

#endif
