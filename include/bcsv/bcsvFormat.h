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
typedef struct fieldSectionCell {
    u32 nameHash;
    u32 bitmask;
    u16 offsetToData;  // Offset to the data this field in an individual entry
    u8 dataShiftAmount;
    u8 dataType;
};

// STRING < FLOAT < LONG < LONG2 < SHORT < CHAR < STRING_PTR
// 0x01 < 0x02 < 0x00 < 0x03 < 0x04 < 0x05 < 0x06
typedef std::vector<u8> dataSection;
namespace dataSectionUtl {
typedef struct dataTypeFormat {
    u8 Numb;
    u16 Size;
};

#define BCSV_LONG \
    { 0x00, 0x0004 }
#define BCSV_STRING \
    { 0x01, 0x0032 }
#define BCSV_FLOAT \
    { 0x02, 0x0004 }
#define BCSV_LONG2 \
    { 0x03, 0x0004 }
#define BCSV_SHORT \
    { 0x04, 0x0002 }
#define BCSV_CHAR \
    { 0x05, 0x0008 }
#define BCSV_STRING_PTR \
    { 0x06, 0x0004 }

std::vector<dataTypeFormat> const DataSortOrderList = {
    BCSV_STRING, BCSV_FLOAT, BCSV_LONG,      BCSV_LONG2,
    BCSV_SHORT,  BCSV_CHAR,  BCSV_STRING_PTR};

s32 dataSortOrder(fieldSectionCell const &arg_fieldSectionData) {
    u32 RoopNumb = 0;
    for (auto buf : DataSortOrderList) {
        if (arg_fieldSectionData.dataType == buf.Numb) return RoopNumb;
        ++RoopNumb;
    }
}

u32 InsertPlaceCalculation(
    std::list<fieldSectionCell> const &arg_rFieldSectionData,
    fieldSectionCell const &arg_rFieldData) {
    u32 DataInsertPlace = 0;
    for (auto buf_dataType : DataSortOrderList) {
        for (auto buf_fieldSection : arg_rFieldSectionData) {
            if (buf_dataType.Numb == buf_fieldSection.dataType) {
                if (buf_fieldSection.nameHash == arg_rFieldData.nameHash) {
                    return DataInsertPlace;
                } else
                    DataInsertPlace += buf_dataType.Size;
            }
        }
    }
}

s32 dataSize(fieldSectionCell const &arg_fieldSectionData) {
    for (auto buf : DataSortOrderList) {
        if (arg_fieldSectionData.dataType == buf.Numb) return buf.Size;
    }
}
}  // namespace dataSectionUtl

typedef std::vector<u8> stringPool;
}  // namespace bcsvFormatBlock
}  // namespace nintendoFormat