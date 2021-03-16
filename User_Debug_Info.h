#ifndef __USER_DEBUG_INFO_H
#define __USER_DEBUG_INFO_H

#ifdef __cplusplus
	extern "C"{
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "User_Mem.h"


void	user_Debug_TestSelf(void);

#if 0
//micro 
#define osSuspend()			do{osKernelSuspend();}while(0)
#define osResume()			do{osKernelResume(1);}while(0)
//#define osGetTickCount()	do{ return osKernelGetTickCount(); }while(0)  
#define osGetTickCount()	

#define __ANDRIOD__DEBUG_ENABLE__ 

#if defined(__ANDRIOD__DEBUG_ENABLE__)
#define WIRELESSDEBUG(x,len) do{ SPP_DEBUG_LOG((uint8_t*)x,(uint8_t*)__FUNCTION__,(uint16_t)len,(uint16_t)(sizeof(__FUNCTION__))); }while(0)
#else
#define WIRELESSDEBUG(x,len) do{}while(0);
#endif

#endif
typedef enum{

USER_DEBUG_LEVEL_ZERO=0,
	
USER_DEBUG_LEVEL_LOW,

USER_DEBUG_LEVEL_MIDDLE,

USER_DEBUG_LEVEL_HIGH

}USER_DEBUG_LEVEL;


#define USER_PRINTF_ENABLE

void user_Debug_TestSelf(void);

void user_Debug_Printf(uint8_t level,uint8_t top);



#ifdef __cplusplus
		}
#endif

#endif

