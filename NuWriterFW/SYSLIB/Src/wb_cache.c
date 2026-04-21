/***************************************************************************
 *                                                                         *
 * Copyright (c) 2008 Nuvoton Technolog. All rights reserved.              *
 *                                                                         *
 ***************************************************************************/
 
/****************************************************************************
* FILENAME
*   wb_cache.c
*
* VERSION
*   1.0
*
* DESCRIPTION
*   The cache related functions of Nuvoton ARM9 MCU
*
* HISTORY
*   2008-06-25  Ver 1.0 draft by Min-Nan Cheng
*
* REMARK
*   None
 **************************************************************************/
#include "wblib.h"

bool volatile _sys_IsCacheOn = false;
int32_t volatile _sys_CacheMode;
extern void sys_flush_and_clean_dcache(void);
extern int sysInitMMUTable(int);


int32_t sysGetSdramSizebyMB()
{
	unsigned int volatile reg, totalsize=0;

	reg = inpw(REG_SDCONF0) & 0x07;
	switch(reg)
	{
		case 1:
			totalsize += 2;
			break;

		case 2:
			totalsize += 4;
			break;

		case 3:
			totalsize += 8;
			break;

		case 4:
			totalsize += 16;
			break;

		case 5:
			totalsize += 32;
			break;

		case 6:
			totalsize += 64;
			break;
	}

	reg = inpw(REG_SDCONF1) & 0x07;
	switch(reg)
	{
		case 1:
			totalsize += 2;
			break;

		case 2:
			totalsize += 4;
			break;

		case 3:
			totalsize += 8;
			break;

		case 4:
			totalsize += 16;
			break;

		case 5:
			totalsize += 32;
			break;

		case 6:
			totalsize += 64;
			break;
	}

	if (totalsize != 0)
		return totalsize;
	else
		return Fail;	
}


int32_t sysEnableCache(uint32_t uCacheOpMode)
{
	sysInitMMUTable(uCacheOpMode);
	_sys_IsCacheOn = true;
	_sys_CacheMode = uCacheOpMode;
	
	return 0;
}

void sysDisableCache()
{
	int temp;

	sys_flush_and_clean_dcache();
	__asm
	{
		/*----- flush I, D cache & write buffer -----*/
		MOV temp, 0x0
		MCR p15, 0, temp, c7, c5, 0 /* flush I cache */
		MCR p15, 0, temp, c7, c6, 0 /* flush D cache */
		MCR p15, 0, temp, c7, c10,4 /* drain write buffer */		

		/*----- disable Protection Unit -----*/
		MRC p15, 0, temp, c1, c0, 0 	/* read Control register */
		BIC	temp, temp, 0x01
		MCR p15, 0, temp, c1, c0, 0 	/* write Control register */
	}
	_sys_IsCacheOn = false;
	_sys_CacheMode = CACHE_DISABLE;
	
}

void sysFlushCache(int32_t nCacheType)
{
	int temp;

	switch (nCacheType)
	{
		case I_CACHE:
			__asm
			{
				/*----- flush I-cache -----*/
				MOV temp, 0x0
				MCR p15, 0, temp, c7, c5, 0 /* invalidate I cache */
			}
			break;

		case D_CACHE:
			sys_flush_and_clean_dcache();
			__asm
			{
				/*----- flush D-cache & write buffer -----*/
				MOV temp, 0x0			
				MCR p15, 0, temp, c7, c10, 4 /* drain write buffer */
			}
			break;

		case I_D_CACHE:
			sys_flush_and_clean_dcache();
			__asm
			{
				/*----- flush I, D cache & write buffer -----*/
				MOV temp, 0x0
				MCR p15, 0, temp, c7, c5, 0 /* invalidate I cache */
				MCR p15, 0, temp, c7, c10, 4 /* drain write buffer */		
			}
			break;

		default:
			;
	}
}

void sysInvalidCache()
{
	int temp;

	__asm
	{		
		MOV temp, 0x0
		MCR p15, 0, temp, c7, c7, 0 /* invalidate I and D cache */
	}
}

bool sysGetCacheState()
{
	return _sys_IsCacheOn;
}


int32_t sysGetCacheMode()
{
	return _sys_CacheMode;
}


int32_t _sysLockCode(uint32_t addr, int32_t size)
{
	int i, cnt, temp;
		
	__asm
	{
	    /* use way3 to lock instructions */
		MRC p15, 0, temp, c9, c0, 1 ;
		ORR temp, temp, 0x07 ;
		MCR p15, 0, temp, c9, c0, 1 ;
	}
	
	if (size % 16)  cnt = (size/16) + 1;
	else            cnt = size / 16;
	
	for (i=0; i<cnt; i++)
	{
		__asm
		{
			MCR p15, 0, addr, c7, c13, 1;
		}
	
		addr += 16;
	}
	
	
	__asm
	{
	    /* use way3 to lock instructions */
		MRC p15, 0, temp, c9, c0, 1 ;
		BIC temp, temp, 0x07 ;
		ORR temp, temp, 0x08 ;
		MCR p15, 0, temp, c9, c0, 1 ;
	}
	
	return Successful;

}


int32_t _sysUnLockCode()
{
	int temp;
	
	/* unlock I-cache way 3 */
	__asm
	{
		MRC p15, 0, temp, c9, c0, 1;
		BIC temp, temp, 0x08 ;
		MCR p15, 0, temp, c9, c0, 1;
	
	}
 
	return Successful;
}

