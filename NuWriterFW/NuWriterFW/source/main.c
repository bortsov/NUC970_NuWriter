/******************************************************************************
 * @file     main.c
 * @brief    XUB.bin  code
 * @version  1.0.1
 * @date     04, September, 2012
 *
 * @note
 * Copyright (C) 2012-2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include "config.h"
#include "usbd.h"
#include "sdglue.h"
#include "filesystem.h"  //for eMMC format test

extern void ParseFlashType(void);
extern uint32_t eMMCBlockSize;

uint32_t PLL_Get(uint32_t reg, unsigned int srcclk)
{
    uint32_t N,M,P;
    N =((inpw(reg) & 0x007F)>>0)+1;
    M =((inpw(reg) & 0x1F80)>>7)+1;
    P =((inpw(reg) & 0xE000)>>13)+1;
    return (srcclk*N/(M*P));
}
void CPU_Info(void)
{
    uint32_t system=12;
    uint32_t cpu,pclk;
    switch( ((inpw(REG_CLKDIV0) & (0x3<<3))>>3)) {
    case 0:
        system=12;
        break;
    case 1:
        return;
    case 2: /* ACLKout */
        system=PLL_Get(REG_APLLCON,system);
        break;
    case 3: /* UCLKout */
        system=PLL_Get(REG_UPLLCON,system);
        break;
    }
    system= ( system/(((inpw(REG_CLKDIV0) & (0x7<<0))>>0)+1) )/ (((inpw(REG_CLKDIV0) & (0xF<<8))>>8)+1);
    cpu   = (system/(((inpw(REG_CLKDIV0) & (0xf<<16))>>16)+1));
    pclk  = ((system)/(((inpw(REG_CLKDIV0) & (0xf<<24))>>24)+1))/2;
    sysprintf("CPU: %dMHz, DDR: %dMHz, SYS: %dMHz, PCLK: %dMHz\n",cpu,system/2,system,pclk);
}
/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main()
{
    uint32_t StartAddr=0,ExeAddr=EXEADDR;
    UNLOCKREG();

    *((volatile unsigned int *)REG_AIC_MDCR)=0xFFFFFFFF;  // disable all interrupt channel
    *((volatile unsigned int *)REG_AIC_MDCRH)=0xFFFFFFFF;  // disable all interrupt channel
    /* copy arm vctor table to 0x0 */
    memcpy((unsigned char *)StartAddr,(unsigned char *)ExeAddr,0x40);

    outpw(REG_AHBIPRST,1<<19);  //USBD reset
    outpw(REG_AHBIPRST,0<<19);
    outpw(REG_USBD_PHY_CTL, inpw(REG_USBD_PHY_CTL) & ~0x100);
    outpw(REG_HCLKEN, inpw(REG_HCLKEN) & ~0x80000);



    outpw(REG_HCLKEN,  inpw(REG_HCLKEN)  | (1<<22));   //Enable eMMC clock
    outpw(REG_HCLKEN,  inpw(REG_HCLKEN)  | (1<<21));   //Enable NAND clock
    //outpw(REG_HCLKEN,  inpw(REG_HCLKEN)  | (1<<23));   //Enable CRYPTO clock
    outpw(REG_PCLKEN0, inpw(REG_PCLKEN0) | (1<<16));   //Enable UART0 clock
    outpw(REG_PCLKEN1, inpw(REG_PCLKEN1) | (1<<4 ));   //Enable SPI1 clock
    outpw(REG_PCLKEN1, inpw(REG_PCLKEN1) | (1<<26 ));   //Enable MTPC clock

    outpw(REG_PCLKEN0, inpw(REG_PCLKEN0) | 0x8);
    outpw(0xB8003120, 0x8000);
    outpw(0xB8003124, 0x4000);


    sysInitializeUART();
    MSG_DEBUG("0x%x\n", inpw(REG_USBD_PHY_CTL));

    sysprintf("=======================================\n");
    sysprintf("Run firmware code\n");
    CPU_Info();

    /* enable USB engine */
    udcInit();

    /* start Timer 0 */
    sysSetTimerReferenceClock(TIMER0, 12000000);
    sysStartTimer(TIMER0, 100, PERIODIC_MODE);

    fmiHWInit();
    sysDelay(10);//For connection stability

/* SD Init */
    _sd_ReferenceClock = 300000;//12000;    // kHz upll or hxt
    eMMCBlockSize=fmiInitSDDevice();
    sysDelay(10); //For connection stability
    MSG_DEBUG("eMMCBlockSize=%08x\n",eMMCBlockSize);

    sysprintf("Parse NuWriter command line\n");
    sysprintf("=======================================\n");
    while(1) {
        ParseFlashType();
    }

}
