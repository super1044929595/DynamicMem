#include "User_Debug_Info.h"




typedef struct {
	
	uint8_t hour;
	uint8_t min;
	uint8_t sec;

	uint16_t year;
	uint8_t  month;
	uint8_t  date;

}UerDebugSystemTimeInfo;

static UerDebugSystemTimeInfo gUserDebugSystem_Time;

static UerDebugSystemTimeInfo user_GetSystemTime(void);

static void user_SetSystemTime(size_t t);

static void user_Debug_Init(void);

static void user_Debug_Printf(uint8_t level, uint8_t top);

///The Node Information begin//////////////////////////////////////////////////////

typedef struct _UD_ListInfo {

	uint8_t					 device_type;
	uint8_t*				 pdata;
	uint16_t				 datalength;
	uint32_t				 timedate;
	UerDebugSystemTimeInfo	 systime;
	struct  _UD_ListInfo*	 nextlistinfo;

}UD_ListInfo;

static UD_ListInfo gUD_ListInfoStart;
static uint8_t     gUD_ListInitEnable = 0;
static uint16_t    gUD_NodeCount = 0;

static const uint32_t xUDListInfoStructSize = (uint32_t)((sizeof(UD_ListInfo) + ((uint32_t)(portBYTE_ALIGNMENT - 1))) & ~((uint32_t)portBYTE_ALIGNMENT_MASK));

void user_Debug_Add(uint8_t* pdata, uint16_t len)
{
	//osPoolCAlloc (userdebug_handle_mempool);
	uint16_t _len = 0;
	UD_ListInfo* _p_list= (UD_ListInfo*)NULL;
	UD_ListInfo* _p_listIdle = (UD_ListInfo*)&gUD_ListInfoStart;
	uint8_t* _p_temp = (uint8_t*)JW_MemAlloc((uint16_t)(len + xUDListInfoStructSize));
	if (_p_temp == (uint8_t*)NULL)	return;

	//APP_UI_TRACE(0,"HJW----->user_Debug_Add begin");

	user_Debug_Init();

	gUD_NodeCount++;

#if 1
	do {
			_p_list = _p_listIdle;
			_p_listIdle = (UD_ListInfo*)_p_listIdle->nextlistinfo;
			
	} while (_p_listIdle != (UD_ListInfo*)NULL);
#else
	while (_p_listIdle != (UD_ListInfo*)NULL) {
		_p_listIdle = _p_listIdle->nextlistinfo;
 }
#endif
	//APP_UI_TRACE(0,"HJW----->while begin");
	_len = xUDListInfoStructSize;
	_p_listIdle = (UD_ListInfo*)_p_temp;

	_p_listIdle->pdata = (uint8_t*)(_p_listIdle + xUDListInfoStructSize);
	_p_listIdle->datalength = (uint16_t)len;
	//update byself
	//_p_list->timedate=osGetTickCount();
	_p_listIdle->device_type =1;//NO-type current 
	_p_listIdle->nextlistinfo = (UD_ListInfo*)NULL;
	_p_listIdle->systime = user_GetSystemTime();
	_p_list->nextlistinfo = _p_listIdle;
	memcpy((uint8_t*)(_p_listIdle + _len), (uint8_t*)pdata, len);

}

/************************************************
*      user_Debug_ContextRead
*
*
************************************************/

void user_Debug_ContextRead(void)
{
	UD_ListInfo* _p_head = (UD_ListInfo*)(gUD_ListInfoStart.nextlistinfo);
	UD_ListInfo* _p_list = (UD_ListInfo*)(gUD_ListInfoStart.nextlistinfo);
	char* _str[100];

	if ((UD_ListInfo*)NULL == _p_list)	return;
	
#ifdef USER_PRINTF_ENABLE
	printf("\r\n User_SPP_Debug Context-------------------|---Read Time :  %d:%d:%d;\n", _p_list->systime.hour, _p_list->systime.min, _p_list->systime.sec);
#endif
	do {
	
		_p_head = (UD_ListInfo*)_p_list;

		_p_list = (UD_ListInfo*)(_p_list->nextlistinfo);

#ifdef USER_PRINTF_ENABLE
		printf("\r\n                                          |\n						");
#endif

		for(uint8_t i = 0; i < (_p_head->datalength-1); i++)
		{
#ifdef USER_PRINTF_ENABLE

			printf("%c",(char*) _p_head->pdata[i]);

#endif
		}
		//sprintf(_str,"%s", _p_head->pdata);
		_p_head->device_type = 0;
		_p_head->datalength = 0;
		_p_head->pdata = (void*)NULL;

		JW_MemFree((UD_ListInfo*)_p_head);

		gUD_NodeCount--;

		gUD_ListInfoStart.nextlistinfo = _p_list;

	} while (_p_list != NULL);

#ifdef USER_PRINTF_ENABLE
	printf("\r\n\n User_SPP_Debug Context end---------------|\n");
#endif
}

void user_Debug_Init(void)
{
	if (gUD_ListInitEnable == 0) {
		gUD_ListInitEnable = 1;
		memset((char*)&gUD_ListInfoStart, 0, sizeof(UD_ListInfo));
		gUD_ListInfoStart.pdata = (uint8_t*)NULL;
		gUD_ListInfoStart.timedate = 0;
		gUD_ListInfoStart.nextlistinfo = (UD_ListInfo*)NULL;
	}
}

void user_Debug_Clean(void)
{
	//osPoolFree (userdebug_handle_mempool, key_handle);
}




void user_SetSystemTime(size_t t)
{
	size_t time = t;
	char time_str[10];
	gUserDebugSystem_Time.hour = t / 3600;
	gUserDebugSystem_Time.min = (t%3600) / 60;
	gUserDebugSystem_Time.sec = ((t % 3600) %60) % 60; 
	if ((gUserDebugSystem_Time.hour == 24) && (gUserDebugSystem_Time.hour == 0) && (gUserDebugSystem_Time.hour == 0)) {
		gUserDebugSystem_Time.hour = 0;
		gUserDebugSystem_Time.min  = 0;
		gUserDebugSystem_Time.sec  = 0;
	}
	//sprintf(time_str,"%d:%d:%d", gUserDebugSystem_Time.hour, gUserDebugSystem_Time.min, gUserDebugSystem_Time.sec);
}

UerDebugSystemTimeInfo user_GetSystemTime(void)
{
	UerDebugSystemTimeInfo time;
	time.hour = gUserDebugSystem_Time.hour;
	time.min = gUserDebugSystem_Time.min;
	time.sec = (uint8_t)(gUserDebugSystem_Time.sec);
	return time;
}


static size_t timeout=1000;
void user_Debug_TestSelf(void)
{

	char test[] = "dsshdsdasdasdasdasdaghfh";
	char test1[] = "j大家好工商局几个h sgjasg jag da 结核杆菌jj";
	char test2[] = "沙建行公司  手机号阿克苏\n";
	user_SetSystemTime(timeout);

	while(1){
		    
			
		    user_SetSystemTime(timeout);
			user_Debug_Add(test, sizeof(test));
			user_Debug_Add(test1, sizeof(test1));
			user_Debug_Add(test2, sizeof(test2));
		
			timeout++;
		JW_MEMLeftSize();
		user_Debug_ContextRead();
		JW_MEMLeftSize();


	}

	
}

#if 0
////////////////////////////////////////////////////////////////////
//private method
static void user_Debug_Handle(void);


uint16_t CRC16(const uint8_t * buffer, uint32_t size);


///////////////POOL/////////////////////////////////////////////////
osPoolDef (userdebug_handle_mempool, 20, APP_KEY_HANDLE);
osPoolId   userdebug_handle_mempool = NULL;


//////////////Thread////////////////////////////////////////////////
static void user_Debug_thread(void const *argument);
osThreadDef(user_Debug_thread, osPriorityLow, 1, 1024*10, "user_Debug_thread");
osThreadId userDebug_thread_tid;

///////////////OsMail///////////////////////////////////////////////

typedef struct {
	uint32_t id;
	uint32_t ptr;
	uint32_t param0;
	uint32_t param1;
} User_Debug_MESSAGE;

#define OSMAIXCOUNT (100)
osMailQDef (user_Debug_mailbox, OSMAIXCOUNT, User_Debug_MESSAGE);
static osMailQId user_Debug_mailbox = NULL;
static uint16_t user_DebugMail_Cnt=0;

void user_Debug_PoolCreate(void)
{
	    if (userdebug_handle_mempool == NULL)
        userdebug_handle_mempool = osPoolCreate(osPool(userdebug_handle_mempool));
}




///The Node Information end//////////////////////////////////////////////////////

int User_Debug_Thread_Create(void)
{
	userDebug_thread_tid=osThreadCreate(osThread(user_Debug_thread), NULL);
	if(userDebug_thread_tid==NULL){
			TRACE(0,"\r\n Failed to Create user_Debug_thread\n");
        return 0;
	}
	return 0;
}



int User_DebugMailbox_Create(void)
{
	user_Debug_mailbox=osMailCreate(osMailQ(user_Debug_mailbox), NULL);	
	if(user_Debug_mailbox==NULL){
			TRACE(0,"\r\n Failed to Create user_Debug_thread\n");
		return -1;
	}
	return 0;
}

int User_DebugMailbox_put(User_Debug_MESSAGE* msg_src)
{
	osStatus status;
	User_Debug_MESSAGE *msg_p = NULL;

	msg_p = (User_Debug_MESSAGE*)osMailAlloc(user_Debug_mailbox, 0);
	ASSERT(msg_p, "osMailAlloc error");
	msg_p->id = msg_src->id;
	msg_p->ptr = msg_src->ptr;
	msg_p->param0 = msg_src->param0;
	msg_p->param1 = msg_src->param1;

	status = osMailPut(user_Debug_mailbox, msg_p);
	if (osOK == status)
		user_DebugMail_Cnt++;

    TRACE(1, "%s,%d,usb_mailbox_cnt = %d.",__func__,__LINE__,user_DebugMail_Cnt);
	return (int)status;
}

int User_DebugMailbox_Free(User_Debug_MESSAGE* msg_p)
{
	osStatus status;
	status = osMailFree(user_Debug_mailbox, msg_p);
	if (osOK == status)
		user_DebugMail_Cnt--;
     TRACE(1, "%s,%d,usb_mailbox_cnt = %d.",__func__,__LINE__,user_DebugMail_Cnt);
	return (int)status;
}

int User_DebugMailbox_Get(User_Debug_MESSAGE **msg_p)
{
	osEvent evt;
	evt = osMailGet(user_Debug_mailbox, osWaitForever);
	if (evt.status == osEventMail) {
		*msg_p = (User_Debug_MESSAGE*)evt.value.p;
		return 0;
	}
	return -1;
}

/**********************************************************************
*
*
*
*
**********************************************************************/

typedef struct{

	uint16_t used;
	uint16_t freesize;
	uint8_t  cur_sector;
	uint8_t  cur_storefullenable;

}User_Debug_SysInfo ;

User_Debug_SysInfo gUser_Debug_SysInfo;

void user_Debug_thread(void const *argument)
{
	User_Debug_MESSAGE *msg_p=NULL;
	for(;;)
	{
		//
		if(!User_DebugMailbox_Get(&msg_p)){

			user_Debug_Handle();
			
			User_DebugMailbox_Free(msg_p);
		}


	}

}

void user_Debug_Handle(void)
{
		if(gUser_Debug_SysInfo.cur_storefullenable==1){
	        
	        //write large amount data to this store
           
            //Erease the flash sector

			gUser_Debug_SysInfo.cur_storefullenable=0;
 
		}else{

			//user_Debug_Add();
		}	
}

#endif
/********************************************************************
*Function:The Software CRC16 ,the difference with the hardware by IC
*
*
*
*********************************************************************/
uint16_t CRC16(const uint8_t * buffer, uint32_t size)
{
	uint16_t crc = 0xFFFF;
	if (NULL != buffer && size > 0){
		while (size--) {
		crc = (crc >> 8) | (crc << 8);
		crc ^= *buffer++;
		crc ^= ((unsigned char) crc) >> 4;
		crc ^= crc << 12;
		crc ^= (crc & 0xFF) << 5;
		}
	}
	return crc;
}



void user_Debug_Printf(uint8_t level,uint8_t top)
{
	if(top==1){
	  APP_UI_TRACE(0,"\r\n  ----------|");
	}else{
	  APP_UI_TRACE(0,"                |-");
	}
		level=level;
}
