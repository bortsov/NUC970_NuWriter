#pragma once

#include "fmi.h"

//Partition Table Entry
#pragma   pack(push,1)
struct PTE {
    uint8_t pteBootIndicator;
    uint8_t pteStartHead;
    uint8_t pteStartSector;
    uint8_t pteStartCylinder;
    uint8_t pteSystemID;
    uint8_t pteEndHead;
    uint8_t pteEndSector;
    uint8_t pteEndCylinder;
    uint32_t pteFirstSector;     // First Partition Sector
    uint32_t ptePartitionSize;
};
#pragma pack(pop)


//Master Boot Record (Sector)
#pragma   pack(push,1)
struct MBR {
    uint8_t  mbrBootCommand[446];      // Offset
    // First Partition
    struct PTE mbrPartition[4];            //[0]:1BEh, [1]:1CEh, [2]:1DEh, [3]:1EEh
    uint16_t mbrSignature;               // 1FEh ~ 1FFh
};
#pragma pack(pop)


struct MBR* create_mbr(uint32_t TotalSize, struct FW_MMC_IMAGE *myPmmcImage);
int32_t FormatFat32(struct MBR* pmbr,uint32_t nCount);
