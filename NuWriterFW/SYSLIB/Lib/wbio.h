/***************************************************************************
 *                                                                         *
 * Copyright (c) 2008 Nuvoton Technology. All rights reserved.             *
 *                                                                         *
 ***************************************************************************/
 
/****************************************************************************
 * 
 * FILENAME
 *     WBIO.h
 *
 * VERSION
 *     1.0
 *
 * DESCRIPTION
 *     This file contains I/O macros, and basic macros and inline functions.
 *
 * DATA STRUCTURES
 *     None
 *
 * FUNCTIONS
 *     None
 *
 * HISTORY
 *     03/28/02		 Ver 1.0 Created by PC30 YCHuang
 *
 * REMARK
 *     None
 **************************************************************************/

#pragma once

#include <stdint.h>

/* W90N960 */
#define LITTLE_ENDIAN

#define outpb(port,value)     (*((uint8_t volatile *) (port))=value)
#define inpb(port)            (*((uint8_t volatile *) (port)))

#define outpw(port,value)     (*((uint32_t volatile *) (port))=value)
#define inpw(port)            (*((uint32_t volatile *) (port)))
