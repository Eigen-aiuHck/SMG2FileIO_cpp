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
        bcsvFormatBlock::fieldSectionCell field = {};

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

// Rework terget.
s32 bcsv::addColumn(u32 const &arg_rInsertPlace,
                    bcsvFormatBlock::fieldSectionCell const &arg_rFieldData) {
    // Avoid duplicate hashes.
    for (auto buf : FieldSection) {
        if (buf.nameHash == arg_rFieldData.nameHash) {
            return -1;
        }
    }

    // header
    ++Header.fieldCount;

    // FieldSection
    auto itr_FieldSection = FieldSection.begin();
    std::advance(itr_FieldSection, arg_rInsertPlace);
    auto const itr_FieldInsertPlace =
        FieldSection.insert(itr_FieldSection, arg_rFieldData);

    // Insert NULL to DataSection for added FieldSection.
    itr_FieldSection = FieldSection.begin();
    {
        u32 const InsertPlaceForData =
            bcsvFormatBlock::dataSectionUtl::InsertPlaceCalculation(
                FieldSection, arg_rFieldData);
        u32 const InsertSizeForData =
            bcsvFormatBlock::dataSectionUtl::dataSize(arg_rFieldData);

        // shift offsetToData
        (*itr_FieldInsertPlace).offsetToData = 0x00;
        for (auto buf_FieldSectionCell : FieldSection) {
            if (InsertPlaceForData < buf_FieldSectionCell.offsetToData)
                buf_FieldSectionCell.offsetToData += InsertSizeForData;
        }

        // initrize offsetToData and insert init Data
        (*itr_FieldInsertPlace).offsetToData = InsertPlaceForData;
        for (auto buf_DataSection : DataSection) {
            auto Itr_InsertPlace =
                std::next(buf_DataSection.begin(), InsertPlaceForData);
            for (u32 i = 0; i < InsertSizeForData; ++i) {
                buf_DataSection.insert(Itr_InsertPlace, 0x00);
            }
        }
    }
}