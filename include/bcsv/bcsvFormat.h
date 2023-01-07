#pragma ouce
#include <vector>

#include "submodules/myInclude/stipulatedTipe.h"

namespace nintendoFormat {
namespace bcsvFormatBlock {
/*
日本語解説
BCSVのよくわからないことのメモ

DataSectionはエントリごとのブロックに分けられ、そしてデータタイプによって並べ替えられる。

offsetToDataは各エントリーの現在のフィールドのデータへのオフセット。
*/
typedef struct header {
    u32 entryCount;
    u32 fieldCount;
    u32 offsetEntryDataSectoin;
    u32 eachEntrySize;  // byte
};

// 0x00 long, 4byte
// 0x01 string, 32byte, no shift
// 0x02 float, 4byte, no shift
// 0x03 long, 4byte
// 0x04 short, 2byte
// 0x05 char, 8byte
// 0x06 *string, 4byte
typedef struct fieldSection {
    u32 nameHash;
    u32 bitmask;
    u16 offsetToData;  // Offset to the data this field in an individual entry
    u8 dataShiftAmount;
    u8 dataType;
};

// STRING < FLOAT < LONG < LONG_2 < SHORT < CHAR < STRING_OFFSET
// 0x01 < 0x02 < 0x00 < 0x03 < 0x04 < 0x05 < 0x06
typedef std::vector<u8> dataSection;
namespace dataSectionUtl {
typedef struct LONG {
    u8 Numb = 0x00;
    u16 Size = 0x04;
};

std::vector<DataTypeFormat> const DataSortNumber = STRING;

s32 dataSortOrder(fieldSection const &arg_fieldSectionData) {
    u32 RoopNumb = 0;
    for (u8 buf : DataSortNumber) {
        if (arg_fieldSectionData.dataType == buf) return RoopNumb;
        ++RoopNumb;
    }
}

s32 dataSize(fieldSection const &arg_fieldSectionData) {}
}  // namespace dataSectionUtl

typedef std::vector<u8> stringPool;
}  // namespace bcsvFormatBlock
}  // namespace nintendoFormat