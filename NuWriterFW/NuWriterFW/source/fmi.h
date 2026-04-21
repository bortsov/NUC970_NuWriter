#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <wblib.h>


static const int SPI_HEAD_ADDR = 4; //Blocks

#define STOR_STRING_LEN 32

/* we allocate one of these for every device that we remember */
struct DISK_DATA
{
    struct DISK_DATA  *next;           /* next device */

    /* information about the device -- always good */
    unsigned int  totalSectorN;
    unsigned int  diskSize;         /* disk size in Kbytes */
    int           sectorSize;
    char          vendor[STOR_STRING_LEN];
    char          product[STOR_STRING_LEN];
    char          serial[STOR_STRING_LEN];
};


struct FMI_SD_INFO
{
    uint32_t  uCardType;      // sd2.0, sd1.1, or mmc
    uint32_t  uRCA;           // relative card address
    bool    bIsCardInsert;
};


struct FMI_SM_INFO
{
	uint32_t	uBlockPerFlash;
	uint32_t	uPagePerBlock;
	uint32_t	uSectorPerBlock;
	uint32_t	uPageSize;
	uint32_t	uBadBlockCount;
	uint32_t  uRegionProtect;     // the page number for Region Protect End Address
	uint32_t	uNandECC;
	uint32_t	uSpareSize;
	bool	bIsMulticycle;
	bool	bIsMLCNand;
  bool    bIsInResetState;
  bool    bIsRA224;

};


//MMC---------------------------------
struct FW_MMC_IMAGE
{
    uint32_t  actionFlag;
    uint32_t  fileLength;
    uint32_t  imageNo;
    char    imageName[16];
    uint32_t  imageType;
    uint32_t  executeAddr;
    uint32_t  flashOffset;
    uint32_t  endAddr;
    uint32_t  ReserveSize;  //unit of sector
    uint8_t   macaddr[8];
    uint32_t  initSize;
    uint8_t   FSType;
    uint32_t  PartitionNum;
    uint32_t  Partition1Size;  //unit of MB
    uint32_t  Partition2Size;  //unit of MB
    uint32_t  Partition3Size;  //unit of MB
    uint32_t  Partition4Size;  //unit of MB
    uint32_t  PartitionS1Size; //Sector size unit 512Byte
    uint32_t  PartitionS2Size; //Sector size unit 512Byte
    uint32_t  PartitionS3Size; //Sector size unit 512Byte
    uint32_t  PartitionS4Size; //Sector size unit 512Byte
};

// SD functions
int  fmiSDCommand(struct FMI_SD_INFO *pSD, uint8_t ucCmd, uint32_t uArg);
int  fmiSDCmdAndRsp(struct FMI_SD_INFO *pSD, uint8_t ucCmd, uint32_t uArg, int nCount);
int  fmiSDCmdAndRsp2(struct FMI_SD_INFO *pSD, uint8_t ucCmd, uint32_t uArg, uint32_t *puR2ptr);
int  fmiSD_Init(struct FMI_SD_INFO *pSD);
int  fmiSelectCard(struct FMI_SD_INFO *pSD);
void fmiGet_SD_info(struct FMI_SD_INFO *pSD, struct DISK_DATA *_info);
int  fmiSD_Read_in(struct FMI_SD_INFO *pSD, uint32_t uSector, uint32_t uBufcnt, uint32_t uDAddr);
int  fmiSD_Write_in(struct FMI_SD_INFO *pSD, uint32_t uSector, uint32_t uBufcnt, uint32_t uSAddr);

// SM functions
int fmiSMCheckRB(void);
int fmiSM_Reset(void);
void fmiSM_Initial(struct FMI_SM_INFO *pSM);
int fmiSM_ReadID(struct FMI_SM_INFO *pSM);
int fmiSM2BufferM_large_page(uint32_t uPage, uint32_t ucColAddr);
int fmiSM_Read_RA(uint32_t uPage, uint32_t ucColAddr);
int fmiCheckInvalidBlock(struct FMI_SM_INFO *pSM, uint32_t BlockNo);
int fmiSM_BlockErase(struct FMI_SM_INFO *pSM, uint32_t uBlock);
int fmiSM_BlockEraseBad(struct FMI_SM_INFO *pSM, uint32_t uBlock);
int fmiMarkBadBlock(struct FMI_SM_INFO *pSM, uint32_t BlockNo);
int CheckBadBlockMark(struct FMI_SM_INFO *pSM, uint32_t block);
int fmiSM_ChipErase(uint32_t uChipSel);
int fmiSM_ChipEraseBad(uint32_t uChipSel);
int fmiSM_Erase(uint32_t uChipSel, uint32_t start, uint32_t len);
int fmiSM_EraseBad(uint32_t uChipSel, uint32_t start, uint32_t len);

int fmiHWInit(void);
int fmiNandInit(void);
int fmiSM_Write_large_page(uint32_t uSector, uint32_t ucColAddr, uint32_t uSAddr);
int fmiSM_Read_large_page(struct FMI_SM_INFO *pSM, uint32_t uPage, uint32_t uDAddr);

int fmiSM_Write_large_page_oob(uint32_t uSector, uint32_t ucColAddr, uint32_t uSAddr,uint32_t oobsize);
int fmiSM_Write_large_page_oob2(uint32_t uSector, uint32_t ucColAddr, uint32_t uSAddr);

extern struct FMI_SM_INFO *pSM;

#define MIN(a,b) ((a) < (b) ? (a) : (b))
