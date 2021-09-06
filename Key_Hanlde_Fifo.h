#ifndef __KEY_HANDLE_MIDDLE_H
#define __KEY_HANDLE_MIDDLE_H

#ifdef __cplusplus
	extern "C"{
#endif


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#include "FreeRTOS.h"
//#include "task.h"

//typedef unsigned char  uint8_t;
//typedef unsigned short uint16_t;

#define XTOUCH_KEYVALUE_SENDMESSAGE(xvalue,value,status)    (xvalue=((value&0xff)<<8)|(status&0xff))

typedef enum{

	TOUCH_PRESS_UNKNOW=0x00U,
	TOUCH_PRESS_UP,
	TOUCH_PRESS_DOWN,
	TOUCH_PRESS_DOUBLE,
	TOUCH_PRESS_THREE,
	TOUCH_PRESS_LONGTIEM,

}Touch_Press_Enum;


//public method
void      Press_KeyValueInit(void);
unsigned char 	  Press_KeyValueWrite(unsigned char prevalue,unsigned char  prestatus);
unsigned short  Press_KeyValueRead(void);
unsigned short  Press_KeyLoop(void);



#ifdef __cplusplus
	}
#endif

#endif
