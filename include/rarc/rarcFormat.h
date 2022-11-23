#pragma once
#include "./myInclude/stipulatedTipe.h"

namespace rarcFormatBlock {
// static block
struct header  // 0x20
{
    u32 mazic;                   // 0x00
    u32 fileSize;                // 0x04
    u32 dataHeaderOffset;        // 0x08
    u32 fileDataSectionOffset;   // 0x0c
    u32 fileDataSectionLength;   // 0x10
    u32 fileDataSectionAllMRAM;  // 0x14
    u32 fileDataSectionAllARAM;  // 0x18
    u32 fileDataSectionAllROM;   // 0x1c
};

struct dataHeader  // 0x20
{
    u32 dirNodeNumber;           // 0x00
    u32 dirNodeOffset;           // 0x04
    u32 fileNodeNumber;          // 0x08
    u32 fileNodeOffset;          // 0x0c
    u32 stringNodeNumber;        // 0x10
    u32 stringNodeOffset;        // 0x14
    u16 nextAvailableFileIndex;  // 0x18
    u8 keepIDSync;               // 0x1a
    u8 padding1;                 // 0x1b
    u8 padding2;
    u8 padding3;
    u8 padding4;
    u8 padding5;
};

// dinamic block
struct dirNodeSection  // 0x10
{
    u32 nodeNameFirst4String;           // 0x00
    u32 nodeNameOffsetIntoStringTable;  // 0x04
    u16 nameHash;                       // 0x08
    u16 fileNodeEntryNumb;              // 0x0a
    u32 fileNodeOffset;                 // 0x0c
};

struct fileNodeSection  // 0x10
{
    u16 nodeIndex;                      // 0x00
    u16 nameHash;                       // 0x02
    u8 nodeAttributesFlag;              // 0x04
    u8 padding;                         // 0x05
    u16 nodeNameOffsetIntoStringTable;  // 0x06
    u32 dataIndex;                      // 0x08
    u32 sizeForDataindex;               // 0x0c
};
}  // namespace rarcFormatBlock
