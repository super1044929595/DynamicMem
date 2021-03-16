#ifndef __KEY_HANDLE_FIFO_H
#define __KEY_HANDLE_FIFO_H

#ifdef __cplusplus
	extern "C"{
#endif

#define XTOUCH_KEYVALUE_SENDMESSAGE(xvalue,value,status)    ((uint16_t)xvalue=(uint16_t)(((value&0xff)<<8)|(status&0xff)))

typedef enum{

	TOUCH_PRESS_UNKNOW=0x00U,
	TOUCH_PRESS_UP,
	TOUCH_PRESS_DOWN,
	TOUCH_PRESS_LONGTIME,
	
}Touch_Press_Enum;


//public method
void      Press_KeyValueInit(void);
bool 	  Press_KeyValueWrite(uint8_t prevalue,uint8_t prestatus);
uint16_t  Press_KeyValueRead(void);







#ifdef __cplusplus
	}
#endif

#endif