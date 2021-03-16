
#ifndef __USER_MEM_H
#define __USER_MEM_H

#ifdef __cplusplus
	extern "C"{
#endif


//////////////////////////////////////////////////////////////////////////////////////
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"


//////////////////////////////////////////////////////////////////////////////////////

#define JW_MEM_PRINTFENABLEx     

//public macro
#define portBYTE_ALIGNMENT  		 (0x04) 
#define portBYTE_ALIGNMENT_MASK  	 (0x03)
//use the inter space of the sram
#define JWZOOMSIZE            (8*1024)

extern 		size_t  jwMemBytesRemain;

void *JW_MemAlloc( size_t xWantedSize );

void JW_MemFree( void *pv );

void JW_MEM_TEST(void);

size_t JW_MEMLeftSize(void);

#ifdef __cplusplus
		}
#endif

#endif

