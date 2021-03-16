#include "Key_Handle_Fifo.h"

//private 
//---------------------------------------------------------------------
#define TOUCH_FILETR_LENGTH              (64U)  // the used is only 20 
static  uint8_t   xTouch_Filter_Value[TOUCH_FILETR_LENGTH];
static  uint8_t   xTouch_Filter_Status[TOUCH_FILETR_LENGTH];
static  uint8_t   xTouch_WCnt=0;
static  uint8_t   xTouch_RCnt=0;
static  uint8_t   xTouch_cnt=0;
static  uint16_t  xTouch_Pre;

static bool	Press_StoreFull(void);
static bool Press_StoreZero(void);

//---------------------------------------------------------------------


bool	Press_StoreFull(void)
{	
    if((xTouch_WCnt==xTouch_RCnt)&&(xTouch_cnt == TOUCH_FILETR_LENGTH))
    {	
        //xTouch_cnt = 0;
        return true;
    }
    else {
        return false;
    }
}

bool    Press_StoreZero(void)
{	
    if((0== xTouch_cnt)&&(xTouch_WCnt== xTouch_RCnt))
    {	
        return true;	
    }
    else
        return false;
}
//public method
void   Press_KeyValueInit(void)
{	
    xTouch_WCnt=(int8_t)0;
    xTouch_RCnt=(int8_t)0;
    memset((char*)xTouch_Filter_Value,0,sizeof(xTouch_Filter_Value));		
    memset((char*)xTouch_Filter_Status,0,sizeof(xTouch_Filter_Status));
}

bool 	Press_KeyValueWrite(uint8_t prevalue,uint8_t prestatus)
{	
    uint16_t _key_pre;	
    if(Press_StoreFull())
    {	
        return true;
    }	
    XTOUCH_KEYVALUE_SENDMESSAGE(_key_pre,prevalue,prestatus);	
    if(_key_pre==xTouch_Pre)	
        return true;	
    xTouch_Filter_Value[xTouch_WCnt]=prevalue;		
    xTouch_Filter_Status[xTouch_WCnt]= prestatus;
    XTOUCH_KEYVALUE_SENDMESSAGE(xTouch_Pre,prevalue,prestatus);
    xTouch_WCnt=(++xTouch_WCnt)%TOUCH_FILETR_LENGTH;	
    xTouch_cnt=++xTouch_cnt;
}

uint16_t  Press_KeyValueRead(void)
{	
    uint16_t _key_pre;  
    if(Press_StoreZero())
    {
        return true;
    }		

    XTOUCH_KEYVALUE_SENDMESSAGE(_key_pre,xTouch_Filter_Value[xTouch_RCnt],xTouch_Filter_Status[xTouch_RCnt]);
    xTouch_Filter_Value[xTouch_RCnt] = (uint8_t)0;
    xTouch_Filter_Status[xTouch_RCnt]= (Touch_Press_Enum)TOUCH_PRESS_UNKNOW;
    xTouch_RCnt =(++xTouch_RCnt)%TOUCH_FILETR_LENGTH;
    xTouch_cnt=--xTouch_cnt;	
    return _key_pre;
}