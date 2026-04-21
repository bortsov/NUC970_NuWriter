/***************************************************************************
 *                                                                         *
 * Copyright (c) 2008 Nuvoton Technolog. All rights reserved.              *
 *                                                                         *
 ***************************************************************************/
 
/****************************************************************************
* FILENAME
*   wb_uart.c
*
* VERSION
*   1.0
*
* DESCRIPTION
*   The UART related function of Nuvoton ARM9 MCU
*
* HISTORY
*   2008-06-25  Ver 1.0 draft by Min-Nan Cheng
*
* REMARK
*   None
 **************************************************************************/
#include <string.h>
#include <stdio.h>
#include "wblib.h"

#define vaStart(list, param) list = (int8_t*)((int)&param + sizeof(param))
#define vaArg(list, type) ((type *)(list += sizeof(type)))[-1]

typedef struct
{
  	union
  	{
		volatile unsigned int RBR;		    	        /*!< Offset: 0x0000   Receive Buffer Register			                 */
		volatile unsigned int THR;			            /*!< Offset: 0x0000   Transmit Holding Register 		                 */
  	} x;
  	volatile unsigned int IER;				        /*!< Offset: 0x0004   Interrupt Enable Register 		                 */
  	volatile unsigned int FCR;				        /*!< Offset: 0x0008   FIFO Control Register 			                 */
  	volatile unsigned int LCR;				        /*!< Offset: 0x000C   Line Control Register				                 */
  	volatile unsigned int MCR;			        	/*!< Offset: 0x0010   Modem Control Register 			                 */
  	volatile unsigned int MSR;				        /*!< Offset: 0x0014   Modem Status Register 			                 */
  	volatile unsigned int  FSR;				        /*!< Offset: 0x0018   FIFO Status Register 				                 */
  	volatile unsigned int ISR;				        /*!< Offset: 0x001C   Interrupt Status Register 		                 */
  	volatile unsigned int TOR;				        /*!< Offset: 0x0020   Time Out Register 				                 */
  	volatile unsigned int BAUD;				        /*!< Offset: 0x0024   Baud Rate Divisor Register		                 */
  	volatile unsigned int IRCR;				        /*!< Offset: 0x0028   IrDA Control Register 			                 */
  	volatile unsigned int ALTCON;			            /*!< Offset: 0x002C   Alternate Control/Status Register	                 */
  	volatile unsigned int FUNSEL;			            /*!< Offset: 0x0030   Function Select Register			                 */
} UART_TypeDef;

#define UART0	   ((UART_TypeDef *)UART0_BA) 


int32_t sysInitializeUART()
{
	/* enable UART0 clock */
	//outpw(REG_CLKEN, inpw(REG_CLKEN) | 0x10000);
	
	/* GPE0, GPE1 */
	outpw(REG_MFP_GPE_L, (inpw(REG_MFP_GPE_L) & 0xffffff00) | 0x99);	// UART0 multi-function

	/* UART0 line configuration for (115200,n,8,1) */
	UART0->LCR |=0x07;	
	UART0->BAUD = 0x30000066;	/* 12MHz reference clock input, 115200 */
	//UART0->BAUD = 0x30000084;	/* 16MHz reference clock input, 115200 */
	return 0;
}


void _PutChar_f(uint8_t ch)
{
	volatile int loop;	
	while ((UART0->FSR & (1<<23))); //waits for TX_FULL bit is clear
	UART0->x.THR = ch;
	if(ch == '\n')
	{		
		while((UART0->FSR & (1<<23))); //waits for TX_FULL bit is clear
		UART0->x.THR = '\r';
	}	
}


void sysPutString(int8_t *string)
{
	while (*string != '\0')
	{
		_PutChar_f(*string);
		string++;
	}
}


static void sysPutRepChar(int8_t c, int count)
{
	while (count--)
		_PutChar_f(c);
}


static void sysPutStringReverse(int8_t *s, int index)
{
	while ((index--) > 0)
		_PutChar_f(s[index]);
}


static void sysPutNumber(int value, int radix, int width, int8_t fill)
{
	int8_t    buffer[40];
	int     bi = 0;
	uint32_t  uvalue;
	uint16_t  digit;
	uint16_t  left = false;
	uint16_t  negative = false;

	if (fill == 0)
		fill = ' ';

	if (width < 0)
	{
		width = -width;
		left = true;
	}

	if (width < 0 || width > 80)
		width = 0;

	if (radix < 0)
	{
		radix = -radix;
		if (value < 0)
		{
			negative = true;
			value = -value;
		}
	}

	uvalue = value;

	do
	{
		if (radix != 16)
		{
			digit = uvalue % radix;
			uvalue = uvalue / radix;
		}
		else
		{
			digit = uvalue & 0xf;
			uvalue = uvalue >> 4;
		}
		buffer[bi] = digit + ((digit <= 9) ? '0' : ('A' - 10));
		bi++;

		if (uvalue != 0)
		{
			if ((radix == 10)
				&& ((bi == 3) || (bi == 7) || (bi == 11) | (bi == 15)))
			{
				buffer[bi++] = ',';
			}
		}
	}
	while (uvalue != 0);

	if (negative)
	{
		buffer[bi] = '-';
		bi += 1;
	}

	if (width <= bi)
		sysPutStringReverse(buffer, bi);
	else
	{
		width -= bi;
		if (!left)
			sysPutRepChar(fill, width);
		sysPutStringReverse(buffer, bi);
		if (left)
			sysPutRepChar(fill, width);
	}
}


static int8_t *FormatItem(int8_t *f, int a)
{
	int8_t   c;
	int    fieldwidth = 0;
	int    leftjust = false;
	int    radix = 0;
	int8_t   fill = ' ';

	if (*f == '0')
		fill = '0';

	while ((c = *f++) != 0)
	{
		if (c >= '0' && c <= '9')
		{
			fieldwidth = (fieldwidth * 10) + (c - '0');
		}
		else
			switch (c)
			{
				case '\000':
					return (--f);
				case '%':
					_PutChar_f('%');
					return (f);
				case '-':
					leftjust = true;
					break;
				case 'c':
					{
						if (leftjust)
							_PutChar_f(a & 0x7f);

						if (fieldwidth > 0)
							sysPutRepChar(fill, fieldwidth - 1);

						if (!leftjust)
							_PutChar_f(a & 0x7f);
						return (f);
					}
				case 's':
					{
						if (leftjust)
							sysPutString((int8_t*)a);

						if (fieldwidth > strlen((int8_t*)a))
							sysPutRepChar(fill, fieldwidth - strlen((int8_t*)a));

						if (!leftjust)
							sysPutString((int8_t*)a);
						return (f);
					}
				case 'd':
				case 'i':
					radix = -10;
					break;
				case 'u':
					radix = 10;
					break;
				case 'x':
					radix = 16;
					break;
				case 'X':
					radix = 16;
					break;
				case 'o':
					radix = 8;
					break;
				default:
					radix = 3;
					break;      /* unknown switch! */
			}
		if (radix)
			break;
	}

	if (leftjust)
		fieldwidth = -fieldwidth;

	sysPutNumber(a, radix, fieldwidth, fill);

	return (f);
}


void sysprintf(int8_t* pcStr,...)
{
	int8_t  *argP;
	
	vaStart(argP, pcStr);       /* point at the end of the format string */
	while (*pcStr)
	{                       /* this works because args are all ints */
		if (*pcStr == '%')
			pcStr = FormatItem(pcStr + 1, vaArg(argP, int));
		else
			_PutChar_f(*pcStr++);
	}
}


int8_t sysGetChar()
{
	int i;
	while (1)
	{
		for(i=0;i<0x1000;i++);
		if (!(UART0->FSR & (1 << 14)))
		{
			return (UART0->x.RBR);
		}
	}
}

void sysPutChar(uint8_t ucCh)
{
	volatile int loop;
	while ((UART0->FSR & (1<<23))); //waits for TX_FULL bit is clear
	UART0->x.THR = ucCh;
	//for (loop = 0; loop < 50; loop++);
}

