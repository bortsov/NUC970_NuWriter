#pragma once

#include <stdint.h>

#define outpb(port,value)     (*((uint8_t volatile *) (port))=value)
#define inpb(port)            (*((uint8_t volatile *) (port)))

#define outpw(port,value)     (*((uint32_t volatile *) (port))=value)
#define inpw(port)            (*((uint32_t volatile *) (port)))
