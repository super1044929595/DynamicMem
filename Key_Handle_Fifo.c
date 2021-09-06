#include "Key_Handle_Middle.h"


//public
//---------------------------------------------------------------------
#define TOUCH_FILETR_USE_LENGTH           5U

#define TOUCH_FILTER_PUSHNPSAME           1U
//private 
//---------------------------------------------------------------------
#define TOUCH_FILETR_LENGTH              (TOUCH_FILETR_USE_LENGTH+1)  // the used is only 20 
static  unsigned char   xTouch_Filter_Value[TOUCH_FILETR_USE_LENGTH];
static  unsigned char   xTouch_Filter_Status[TOUCH_FILETR_USE_LENGTH];
static  unsigned char   xTouch_WCnt = 0;
static  unsigned char   xTouch_RCnt = 0;
static  unsigned char   xTouch_cnt = 0;
static  unsigned short  xTouch_Pre = 0;
static unsigned char	Press_StoreFull(void);
static unsigned char    Press_StoreZero(void);
//---------------------------------------------------------------------


unsigned char	Press_StoreFull(void)
{ 
    if ((( xTouch_WCnt+1) % TOUCH_FILETR_LENGTH) == xTouch_RCnt){
         return 1;
    }
    else if (xTouch_cnt == TOUCH_FILETR_LENGTH) {
         return 1;
    }
    else {
        return 0;
    }
}

unsigned char    Press_StoreZero(void)
{	  
    if (xTouch_RCnt% TOUCH_FILETR_LENGTH == xTouch_WCnt) {
        return 1;
    }
    else {
        return 0;
    }
}
//public method
void   Press_KeyValueInit(void)
{	
    xTouch_WCnt=(unsigned char)0;
    xTouch_RCnt=(unsigned char)0;
    xTouch_cnt =(unsigned char)0;
    memset((unsigned char*)xTouch_Filter_Value,0,sizeof(xTouch_Filter_Value));		
    memset((unsigned char*)xTouch_Filter_Status,0,sizeof(xTouch_Filter_Status));
}


unsigned short last_KeyValue[2] = { 0,0 };
unsigned char 	Press_KeyValueWrite(unsigned char prevalue,unsigned char prestatus)
{	
    if(Press_StoreFull())
    {	
        return 1;
    }   
    #ifdef TOUCH_FILTER_PUSHNPSAME //cancel by user when not need the difference of the same button value 	
    XTOUCH_KEYVALUE_SENDMESSAGE(last_KeyValue[1],prevalue,prestatus);
   
    if(last_KeyValue[1]== last_KeyValue[0])
        return 1;
    #endif	
    
    xTouch_Filter_Value[xTouch_WCnt]=(unsigned char)prevalue;		
    xTouch_Filter_Status[xTouch_WCnt]= (unsigned char)prestatus;
    XTOUCH_KEYVALUE_SENDMESSAGE(xTouch_Pre,prevalue,prestatus);
    xTouch_WCnt = xTouch_WCnt++;
    xTouch_WCnt=(unsigned char)(xTouch_WCnt%TOUCH_FILETR_LENGTH);	
    ++xTouch_cnt ;
    last_KeyValue[0] = last_KeyValue[1];
    return 0;
}

unsigned short  Press_KeyValueRead(void)
{	
    unsigned short _key_pre;  
    if(Press_StoreZero())
    {
        return 1;
    }		   
    XTOUCH_KEYVALUE_SENDMESSAGE(_key_pre,xTouch_Filter_Value[xTouch_RCnt],xTouch_Filter_Status[xTouch_RCnt]);
    xTouch_Filter_Value[xTouch_RCnt] = (unsigned char)0;
    xTouch_Filter_Status[xTouch_RCnt]= (unsigned char)TOUCH_PRESS_UNKNOW;
    ++xTouch_RCnt;
    xTouch_RCnt =(unsigned char)(xTouch_RCnt%TOUCH_FILETR_LENGTH);
    --xTouch_cnt;	
    return _key_pre;
}


unsigned short Press_KeyLoop(void)
{
    unsigned short key = 0;
    unsigned short keyfact = 0;

   
    for (int i = 0; i <6; i++) {
        key=Press_KeyValueRead();
        if ((key&0xff) == (unsigned short)TOUCH_PRESS_DOWN) {
            ++keyfact;
        }
    }
   
    return keyfact;
}
