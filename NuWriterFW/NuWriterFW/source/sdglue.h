/******************************************************************************
 *
 * Copyright (c) 2013 Windond Electronics Corp.
 * All rights reserved.
 *
 * $Workfile: sdglue.h $
 *
 * $Author: schung $
 ******************************************************************************/
#pragma once

#include <stdint.h>

#include "sd.h"

#define SD_SECTOR 512
#define SD_MUL 8
#define MMC_INFO_SECTOR 1

int  fmiInitSDDevice(void);
int  fmiSD_Read(uint32_t uSector, uint32_t uBufcnt, uint32_t uDAddr);
int  fmiSD_Write(uint32_t uSector, uint32_t uBufcnt, uint32_t uSAddr);


void Burn_MMC_RAW(uint32_t len, uint32_t offset,uint8_t *ptr);
void Read_MMC_RAW(uint32_t len, uint32_t offset,uint8_t *ptr);


//------------------------------------------------------------------
int ChangeMMCImageType(uint32_t imageNo, uint32_t imageType);
int SetMMCImageInfo(struct FW_MMC_IMAGE *mmcImageInfo);
uint32_t GetMMCImageInfo(unsigned int *image);
uint32_t GetMMCReserveSpace(void);
void GetMMCImage(void);
int DelMMCImage(uint32_t imageNo);
