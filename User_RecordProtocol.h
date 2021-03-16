#ifndef __USER_RECORDPROTOCOL_H
#define __USER_RECORDPROTOCOL_H

#ifdef __cplusplus
	extern "C"{
#endif
//--------------------------------------------------------------------------------------


#include "User_Mem.h"

enum USER_DEBUG_PROTOCOL_TYPE{

	GET_SIGBOARD_FILEINFO=0x21,
	FILE_EXCHANGE_PARMETERS,
	GET_SIGFILEINFO,
	GET_SIGBOARD_FILEDATA,
	SET_FILECONTEXTUPDATE,
	NOTIFY_FILERESULT
	
};

enum USER_FILEEXCHANGE_TYPE{

	FILE_PROTOCAL_VERSION=0x01,
	TRANSFER_BITMAP_ENABLE,
	
};

typedef struct{
	
	const uint8_t*	version_str;
	uint8_t			transfer_bitmap_enable;
	uint16_t		transfer_unit_size;
	uint16_t		timeout;
	uint8_t			receive_file_type;
	uint32_t		baudrate;
	uint8_t			Receive_delay;
	
}UD_FileProtocal_Paramter;


typedef struct{

	uint32_t time_date;
	
	uint8_t  header;
	uint16_t datalength;
	uint8_t* pdata; 	
	uint16_t crc16;

}User_Debug_Frame;


void Package_FileToEP(UD_FileProtocal_Paramter type,uint8_t*pdata,uint16_t datalength);

//--------------------------------------------------------------------------------------
#ifdef __cplusplus
		}
#endif

#endif
