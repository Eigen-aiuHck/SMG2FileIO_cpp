#include "bcsv/bcsv.h"

#include "bcsv/bcsvFormat.h"
#include "submodules/myInclude/convertEndian.h"
#include "submodules/myInclude/stipulatedTipe.h"
using namespace nintendoFormat;

bcsv::bcsv(u8 const *const arg_pSrcData, u32 const &arg_rSrcDataSize) {
    newData(arg_pSrcData, arg_rSrcDataSize);
}

void bcsv::newData(u8 const *const arg_pSrcData, u32 const &arg_rSrcDataSize) {
    u32 SrcDataPlace = 0;

    // Header
    Header.entryCount = convertEndian(*((u32 *)(&arg_pSrcData[SrcDataPlace])));
    SrcDataPlace += sizeof(Header.entryCount);

    Header.fieldCount = convertEndian(*((u32 *)(&arg_pSrcData[SrcDataPlace])));
    SrcDataPlace += sizeof(Header.fieldCount);

    Header.offsetEntryDataSectoin =
        convertEndian(*((u32 *)(&arg_pSrcData[SrcDataPlace])));
    SrcDataPlace += sizeof(Header.offsetEntryDataSectoin);

    Header.eachEntrySize =
        convertEndian(*((u32 *)(&arg_pSrcData[SrcDataPlace])));
    SrcDataPlace += sizeof(Header.eachEntrySize);

    // FieldSection
    for (u32 i = 0; i < Header.fieldCount; ++i) {
        bcsvFormatBlock::fieldSection field = {};

        field.nameHash = convertEndian(*((u32 *)(&arg_pSrcData[SrcDataPlace])));
        SrcDataPlace += sizeof(field.nameHash);

        field.bitmask = convertEndian(*((u32 *)(&arg_pSrcData[SrcDataPlace])));
        SrcDataPlace += sizeof(field.bitmask);

        field.offsetToData =
            convertEndian(*((u32 *)(&arg_pSrcData[SrcDataPlace])));
        SrcDataPlace += sizeof(field.offsetToData);

        field.dataShiftAmount =
            convertEndian(*((u32 *)(&arg_pSrcData[SrcDataPlace])));
        SrcDataPlace += sizeof(field.dataShiftAmount);

        field.dataType = convertEndian(*((u32 *)(&arg_pSrcData[SrcDataPlace])));
        SrcDataPlace += sizeof(field.dataType);

        FieldSection.push_back(field);
    }

    // DataSection
    SrcDataPlace = Header.offsetEntryDataSectoin;
    for (s32 i = 0; i < Header.entryCount; ++i) {
        bcsvFormatBlock::dataSection buf;
        for (s32 i1 = 0; i < Header.eachEntrySize; ++i) {
            buf.push_back(arg_pSrcData[SrcDataPlace]);
        }
        DataSection.push_back(buf);
    }

    // StringPool
    while (SrcDataPlace < arg_rSrcDataSize) {
        StringPool.push_back(arg_pSrcData[SrcDataPlace]);
    }
}

s32 bcsv::addRow(u32 const &arg_rInsertPlace,
                 bcsvFormatBlock::fieldSection &arg_rFieldData) {
    u32 NewSectionDataSize = FieldSection.size() + 1;

    // header
    ++Header.fieldCount;

    // FieldSection
    auto itr_FieldSection = FieldSection.begin();
    for (u32 i = 0; i < arg_rInsertPlace; ++i) {
        ++itr_FieldSection;
    }
    FieldSection.insert(itr_FieldSection, arg_rFieldData);

    // Insert NULL to DataSection for added FieldSection.
    itr_FieldSection = FieldSection.begin();

    

    for (u32 i = 0; i < Header.entryCount; ++i) {
        if (i == arg_rInsertPlace) {
        }
    }
}