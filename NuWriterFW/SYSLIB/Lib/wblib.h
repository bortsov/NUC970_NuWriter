/***************************************************************************
 *                                                                         *
 * Copyright (c) 2008 Nuvoton Technology. All rights reserved.             *
 *                                                                         *
 ***************************************************************************/
 
/****************************************************************************
 * 
 * FILENAME
 *     WBLIB.h
 *
 * VERSION
 *     1.0
 *
 * DESCRIPTION
 *     The header file of NUC900 system library.
 *
 * DATA STRUCTURES
 *     None
 *
 * FUNCTIONS
 *     None
 *
 * HISTORY
 *     2008-06-26  Ver 1.0 draft by Min-Nan Cheng
 *
 * REMARK
 *     None
 **************************************************************************/
#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "NUC970_reg.h"
#include "wberrcode.h"
#include "wbio.h"

//-- function return value
#define	   Successful  0
#define	   Fail        1

#define EXTERNAL_CRYSTAL_CLOCK  12000000


/* Define constants for use external IO in service parameters.  */
#define EXT0			0
#define EXT1			1
#define EXT2			2
#define EXT3			3
#define EXT4			4

#define SIZE_256K		4
#define SIZE_512K		5
#define SIZE_1M			6
#define SIZE_2M			7
#define SIZE_4M			8
#define SIZE_8M			9
#define SIZE_16M		10
#define SIZE_32M		11

#define BUS_DISABLE		12
#define BUS_BIT_8		13
#define BUS_BIT_16		14
#define BUS_BIT_32		15


/* Define the vector numbers associated with each interrupt */
typedef enum int_source_e
{
	IRQ_WDT=1,		// Watch Dog Timer
	IRQ_WWDT,		// Windowed-WDT Interrupt
	IRQ_LVD,
	IRQ_EXTI0,		// External Interrupt 0
	IRQ_EXTI1,		// External Interrupt 1
	IRQ_EXTI2,		// External Interrupt 2
	IRQ_EXTI3,		// External Interrupt 3
	IRQ_EXTI4,		// External Interrupt 4
	IRQ_EXTI5,		// External Interrupt 5
	IRQ_EXTI6,		// External Interrupt 6
	IRQ_EXTI7,		// External Interrupt 7
	IRQ_ACTL,		// Audio Controller Interrupt
	IRQ_LCD,		// LCD Controller Interrupt
	IRQ_CAP,		// Sensor Interface Controller Interrupt
	IRQ_RTC,		// RTC interrupt
	IRQ_TIMER0,		// Timer 0 interrupt
	IRQ_TIMER1,		// Timer 1 interrupt
    IRQ_ADC,
	IRQ_EMC0_RX,	// EMC 0 RX Interrupt
	IRQ_EMC1_RX,	// EMC 1 RX Interrupt
	IRQ_EMC0_TX,	// EMC 0 TX Interrupt
	IRQ_EMC1_TX,	// EMC 1 TX Interrupt
	IRQ_EHCI,		// USB 2.0 Host Controller Interrupt
	IRQ_OHCI,		// USB 1.1 Host Controller Interrupt
	IRQ_GDMA0,		// GDMA Channel 0 Interrupt
	IRQ_GDMA1,		// GDMA Channel 1 Interrupt
    IRQ_SDH,
	IRQ_SIC,		// SIC Interrupt
	IRQ_UDC,		// USB Device Controller Interrupt
	IRQ_TIMER2,		// Timer 2 interrupt
	IRQ_TIMER3,		// Timer 3 interrupt
	IRQ_TIMER4,		// Timer 4 interrupt
	IRQ_JPEG,		// JPEG Engine Interrupt
	IRQ_GE2D,		// 2D Graphic Engine Interrupt
	IRQ_CRYPTO,		// CRYPTO Engine Interrupt
	IRQ_UART0,		// UART 0 interrupt
	IRQ_UART1,		// UART 1 interrupt
	IRQ_UART2,		// UART 2 interrupt
	IRQ_UART4,		// UART 4 interrupt
	IRQ_UART6,		// UART 6 interrupt
	IRQ_UART8,		// UART 8 interrupt
	IRQ_UART10,		// ADC interrupt
	IRQ_UART3,		// UART 3 interrupt
	IRQ_UART5,		// UART 5 interrupt
	IRQ_UART7,		// UART 7 interrupt
	IRQ_SMC0,		// SmartCard 0 Interrupt
	IRQ_SMC1,		// SmartCard 1 Interrupt
	IRQ_I2C0,		// I2C 0 Interrupt
	IRQ_I2C1,		// I2C 1 Interrupt
	IRQ_USI0,		// Universal serial interface 0 interrupt
	IRQ_USI1,		// Universal serial interface 1 interrupt
	IRQ_GPIO,		// GPIO interrupt
	IRQ_CAN0,		// CAN 0 interrupt
	IRQ_CAN1,		// CAN 1 interrupt
	IRQ_PWM,		// PWM interrupt
	IRQ_KPI,		// KPI interrupt
	IRQ_PS2_0,		// PS/2 0 Interrupt
	IRQ_PS2_1		// PS/2 1 Interrupt
} INT_SOURCE_E;

typedef struct datetime_t
{
	uint32_t	year;
	uint32_t	mon;
	uint32_t	day;
	uint32_t	hour;
	uint32_t	min;
	uint32_t	sec;
} DateTime_T;

/* Define constants for use timer in service parameters.  */
#define TIMER0            0
#define TIMER1            1

#define ONE_SHOT_MODE     0
#define PERIODIC_MODE     1
#define CONTINUOUS_MODE   3

#define ONE_HALF_SECS     0
#define FIVE_SECS         1
#define TEN_SECS          2
#define TWENTY_SECS       3

/* Define constants for use UART in service parameters.  */
#define WB_UART_0		0
#define WB_UART_1		1
#define WB_UART_2		2
#define WB_UART_3		3
#define WB_UART_4		4

#define WB_DATA_BITS_5    0x00
#define WB_DATA_BITS_6    0x01
#define WB_DATA_BITS_7    0x02
#define WB_DATA_BITS_8    0x03

#define WB_STOP_BITS_1    0x00
#define WB_STOP_BITS_2    0x04

#define WB_PARITY_NONE    0x00
#define WB_PARITY_ODD     0x00
#define WB_PARITY_EVEN    0x10

//#define WB_DTR_Low        0x01
//#define WB_RTS_Low        0x02
//#define WB_MODEM_En       0x08

#define LEVEL_1_BYTE      0x00
#define LEVEL_4_BYTES     0x40
#define LEVEL_8_BYTES     0x80
#define LEVEL_14_BYTES    0xC0

/* Define constants for use AIC in service parameters.  */
#define WB_SWI                     0
#define WB_D_ABORT                 1
#define WB_I_ABORT                 2
#define WB_UNDEFINE                3

/* The parameters for sysSetInterruptType() use */
#define LOW_LEVEL_SENSITIVE        0x00
#define HIGH_LEVEL_SENSITIVE       0x40
#define NEGATIVE_EDGE_TRIGGER      0x80
#define POSITIVE_EDGE_TRIGGER      0xC0

/* The parameters for sysSetInterruptPriorityLevel() and 
   sysInstallISR() use */
#define FIQ_LEVEL_0                0
#define IRQ_LEVEL_1                1
#define IRQ_LEVEL_2                2
#define IRQ_LEVEL_3                3
#define IRQ_LEVEL_4                4
#define IRQ_LEVEL_5                5
#define IRQ_LEVEL_6                6
#define IRQ_LEVEL_7                7

/* The parameters for sysSetGlobalInterrupt() use */
#define ENABLE_ALL_INTERRUPTS      0
#define DISABLE_ALL_INTERRUPTS     1

/* The parameters for sysSetLocalInterrupt() use */
#define ENABLE_IRQ                 0x7F
#define ENABLE_FIQ                 0xBF
#define ENABLE_FIQ_IRQ             0x3F
#define DISABLE_IRQ                0x80
#define DISABLE_FIQ                0x40
#define DISABLE_FIQ_IRQ            0xC0

/* Define Cache type  */
#define CACHE_WRITE_BACK		0
#define CACHE_WRITE_THROUGH		1
#define CACHE_DISABLE			-1

#define MMU_DIRECT_MAPPING	0
#define MMU_INVERSE_MAPPING	1


/* Define constants for use Cache in service parameters.  */
#define CACHE_4M		2
#define CACHE_8M		3
#define CACHE_16M		4
#define CACHE_32M		5
#define I_CACHE			6
#define D_CACHE			7
#define I_D_CACHE		8


/* Define UART initialization data structure */
typedef struct UART_INIT_STRUCT
{
	uint32_t		uart_no;
    uint32_t		uiFreq;
    uint32_t		uiBaudrate;
    uint8_t		uiDataBits;
    uint8_t		uiStopBits;
    uint8_t		uiParity;
    uint8_t		uiRxTriggerLevel;
} WB_UART_T;

extern uint32_t UART_BA;

/* UART return value */
#define WB_INVALID_PARITY       -1
#define WB_INVALID_DATA_BITS    -2
#define WB_INVALID_STOP_BITS    -3
#define WB_INVALID_BAUD         -4


/* Define PLL initialization data structure */
typedef struct PLL_INIT_STRUCT
{
	uint32_t		pll0;		/* PLL0 output frequency */
    uint32_t		pll1;		/* PLL1 output frequency */
    uint32_t		cpu_src;	/* Select CPU clock from PLL0 or PLL1 */
    uint32_t		ahb_clk;	/* the ratio of CPU : AHB clock */
    uint32_t		apb_clk;	/* the ratio of AHB : APB clock */
} WB_PLL_T;


/* Define PLL freq. setting */
#define PLL_DISABLE	0x12B63
#define	PLL_66MHZ	0x2B63
#define	PLL_100MHZ	0x4F64
#define PLL_120MHZ	0x4F63
#define PLL_133MHZ	0x22A2
#define	PLL_166MHZ	0x4124
#define	PLL_200MHZ	0x4F24


/* Define CPU clock source */
#define CPU_FROM_PLL0			0
#define CPU_FROM_PLL1			1
#define CPU_FROM_PLL0_DIV_2		2
#define CPU_FROM_EXTAL15M		3


/* Define AHB clock */
#define	AHB_CPUCLK_1_1	0
#define	AHB_CPUCLK_1_2	1
#define	AHB_CPUCLK_1_4	2
#define	AHB_CPUCLK_1_8	3

/* Define APB clock */
#define APB_AHB_1_2		1
#define APB_AHB_1_4		2
#define APB_AHB_1_8		3

/* Define the constant values of PM */
#define WB_PM_IDLE		1
#define WB_PM_PD		2
#define WB_PM_MIDLE	    5

#define WB_PM_PD_IRQ_Fail			-1
#define WB_PM_Type_Fail			    -2
#define WB_PM_INVALID_IRQ_NUM		-3
#define WB_PM_CACHE_OFF				-4


/* Define system library Timer functions */
uint32_t	sysGetTicks (int32_t nTimeNo);
int32_t	sysResetTicks (int32_t nTimeNo);
int32_t	sysUpdateTickCount(int32_t nTimeNo, uint32_t uCount);
int32_t	sysSetTimerReferenceClock (int32_t nTimeNo, uint32_t uClockRate);
int32_t	sysStartTimer (int32_t nTimeNo, uint32_t uTicksPerSecond, int32_t nOpMode);
int32_t	sysStopTimer (int32_t nTimeNo);
void	sysClearWatchDogTimerCount (void);
void	sysClearWatchDogTimerInterruptStatus(void);
void	sysDisableWatchDogTimer (void);
void	sysDisableWatchDogTimerReset(void);
void	sysEnableWatchDogTimer (void);
void	sysEnableWatchDogTimerReset(void);
void*	sysInstallWatchDogTimerISR (int32_t nIntTypeLevel, void* pvNewISR);
int32_t	sysSetWatchDogTimerInterval (int32_t nWdtInterval);
int32_t	sysSetTimerEvent(int32_t nTimeNo, uint32_t uTimeTick, void* pvFun);
void	sysClearTimerEvent(int32_t nTimeNo, uint32_t uTimeEventNo);
void	sysSetLocalTime(DateTime_T ltime);
void	sysGetCurrentTime(DateTime_T *curTime);
void	sysDelay(uint32_t uTicks);

/* Define system library UART functions */
int8_t	sysGetChar (void);
int32_t	sysInitializeUART (void);
void	sysPrintf (int8_t* pcStr,...);
void	sysprintf (int8_t* pcStr,...);
void	sysPutChar (uint8_t ucCh);

/* Define system library AIC functions */
int32_t	sysDisableInterrupt (INT_SOURCE_E eIntNo);
int32_t	sysEnableInterrupt (INT_SOURCE_E eIntNo);
bool	sysGetIBitState(void);
uint32_t	sysGetInterruptEnableStatus(void);
uint32_t	sysGetInterruptEnableStatusH(void);
void*	sysInstallExceptionHandler (int32_t nExceptType, void* pvNewHandler);
void*	sysInstallFiqHandler (void* pvNewISR);
void*	sysInstallIrqHandler (void* pvNewISR);
void*	sysInstallISR (int32_t nIntTypeLevel, INT_SOURCE_E eIntNo, void* pvNewISR);
int32_t	sysSetGlobalInterrupt (int32_t nIntState);
int32_t	sysSetInterruptPriorityLevel (INT_SOURCE_E eIntNo, uint32_t uIntLevel);
int32_t	sysSetInterruptType (INT_SOURCE_E eIntNo, uint32_t uIntSourceType);
int32_t	sysSetLocalInterrupt (int32_t nIntState);
int32_t	sysSetAIC2SWMode(void);


/* Define system library Cache functions */
void	sysDisableCache(void);
int32_t	sysEnableCache(uint32_t uCacheOpMode);
void	sysFlushCache(int32_t nCacheType);
bool	sysGetCacheState(void);
int32_t	sysGetSdramSizebyMB(void);
void	sysInvalidCache(void);
int32_t 	sysSetCachePages(uint32_t addr, int32_t size, int32_t cache_mode);


/* Define system clock functions */
int32_t sysGetPLLConfig(WB_PLL_T *sysClk);
int32_t sysSetPLLConfig(WB_PLL_T *sysClk);


/* Define system library External IO functions */
int32_t	sysSetExternalIO(int extNo, uint32_t extBaseAddr, uint32_t extSize, int extBusWidth);
int32_t	sysSetExternalIOTiming1(int extNo, int tACC, int tACS);
int32_t	sysSetExternalIOTiming2(int extNo, int tCOH, int tCOS);


/* Define system power management functions */
void sysDisableAllPM_IRQ(void);
int sysEnablePM_IRQ(int irq_no);
int sysPMStart(int pd_type);
