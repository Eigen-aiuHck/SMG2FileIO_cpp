#include <vector>

#include "myinclude/stipulatedTipe.h"

namespace nintendoFormat {
namespace bcsvFormatBlock {
typedef struct header {
    u32 entryCount;
    u32 fieldCount;
    u32 offsetEntryDataSectoin;
    u32 eachEntrySize;  // byte
};

// STRING < FLOAT < LONG < LONG_2 < SHORT < CHAR < STRING_OFFSET
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
    u16 nextEntryOffset;
    u8 dataShiftAmount;
    u8 dataType;
};

typedef std::vector<u32> dataSection;

typedef std::vector<u8> stringPool;
}  // namespace bcsvFormatBlock
}  // namespace nintendoFormat