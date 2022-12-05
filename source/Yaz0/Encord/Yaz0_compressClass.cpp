#include "Yaz0/encord/Yaz0_compressClass.h"

#include <iomanip>
#include <list>

#include "submodules/myInclude/stipulatedTipe.h"
using namespace nintendoFormat;

Yaz0Encord::Yaz0Encord() {
    uncompressSize = 0;
    dst = new u8[0];
    dstSize = 0;
    dstPlace = 0;
    compressMultiplication = 0;
}

Yaz0Encord::Yaz0Encord(u8 *const pSourceData, u32 const SourceDataSize) {
    newData(pSourceData, SourceDataSize);
}

Yaz0Encord::~Yaz0Encord() {
    if (dst != nullptr) delete[] dst;
}

Yaz0Encord &Yaz0Encord::operator=(std::list<u8> arg_Src) {
    s32 place = 0;
    u8 *pSrc_buf = new u8[arg_Src.size()];
    for (std::list<u8>::iterator itr = arg_Src.begin(); itr != arg_Src.end();) {
        pSrc_buf[place++] = *(itr++);
    }
    newData(pSrc_buf, (unsigned int)arg_Src.size());
    delete[] pSrc_buf;

    return *this;
}

void Yaz0Encord::newData(u8 *const pSourceData, u32 const SourceDataSize) {
    if (SourceDataSize <= 8) {
        return;
    }
    // init data
    reset();
    arg_pack src;
    src.changeDataPointer(pSourceData);
    src.Size = SourceDataSize;

    compressMultiplication = 0.0f;

    // main branch
    arg_pack dst_buf;
    encode(src, dst_buf);

    src.detachDataPointer();  // Becouse argment

    // dst write state
    dst_buf.moveDataPointer(dst);
    dstSize = dst_buf.Size;
    dstPlace = 0;
    uncompressSize = SourceDataSize;

    compressMultiplication = (float)dstSize / (float)src.Size;
}

// private

void Yaz0Encord::reset() {
    delete[] dst;
    dst = new u8[0];
    uncompressSize = 0;
    dstSize = 0;
    dstPlace = 0;
    compressMultiplication = 0;
}

Yaz0Encord::arg_pack &Yaz0Encord::encode(arg_pack const &arg_rSrc,
                                         arg_pack &return_rDst) const {
    return_rDst.changeDataPointer(new u8[arg_rSrc.Size]);

    // null chack
    if (arg_rSrc.Size < 8) {
        return_rDst.Size = 0;

        return return_rDst;
    }

    u32 srcPlace = 0, dstPlace = 0;

    // init decided format area
    return_rDst.Data[dstPlace++] = 0xff;  // uncompress flag write

    while (srcPlace < Yaz0_flontNonCompressByteSize) {
        // not conpress dictionaly
        return_rDst.Data[dstPlace++] = arg_rSrc.Data[srcPlace++];
    }

    // main
    u32 uncompressBitCount = 0, uncompressFlagWritePlace = 0;
    u32 compSize;
    u32 compOffset;
    u32 compThroughCount = 0x01;

    while (srcPlace < arg_rSrc.Size) {
        if (uncompressBitCount == 0) {
            uncompressFlagWritePlace = dstPlace++;
            return_rDst.Data[uncompressFlagWritePlace] = 0x00;
            uncompressBitCount = 8;
        }
        uncompressBitCount--;

        // serch(function)
        serchDictionaly(arg_rSrc, srcPlace, compSize, compOffset,
                        compThroughCount);

        if (compSize < 3) {
            // don't compress
            return_rDst.Data[uncompressFlagWritePlace] |= 0x01
                                                          << uncompressBitCount;
            return_rDst.Data[dstPlace++] = arg_rSrc.Data[srcPlace++];
        } else {
            // compress
            if (compSize <= Yaz0_compress2byteFormatMaxLength) {
                // 2 byte format
                compSize -= 0x02;
                return_rDst.Data[dstPlace++] =
                    ((compSize & 0x0f) << 4) |                       // length
                    (((compOffset)&0x0f00) >> 8);                    // offset
                return_rDst.Data[dstPlace++] = compOffset & 0x00ff;  // offset

                srcPlace += (compSize & 0x0f) + 0x02;  // seek compressed length
            } else {
                // 3 byte format
                compSize -= 0x12;
                return_rDst.Data[dstPlace++] =
                    (compOffset & 0x0f00) >> 8;                      // offset
                return_rDst.Data[dstPlace++] = compOffset & 0x00ff;  // offset
                return_rDst.Data[dstPlace++] = compSize & 0x00ff;    // length

                srcPlace +=
                    (compSize & 0x00ff) + 0x12;  // seek compressed length
            }
        }
    }

    // Byte Padding
    while (uncompressBitCount != 0) {
        --uncompressBitCount;
        return_rDst.Data[uncompressFlagWritePlace] |= 0x01
                                                      << uncompressBitCount;
        return_rDst.Data[dstPlace++] = 0x00;
    }

    for (u32 i = 0; i < dstPlace % 16; i++) {
        return_rDst.Data[dstPlace++] = 0x00;
    }
    return_rDst.Size = dstPlace;

    new_optimisation(return_rDst);

    return return_rDst;
}

void Yaz0Encord::serchDictionaly(arg_pack const &arg_rSrc,
                                 u32 const &arg_rSrcPlace,
                                 u32 &return_rCompSize, u32 &return_rCompOffset,
                                 u32 &arg_return_rCompThroughCount) const {
    s32 const minReferencePlace = arg_rSrcPlace - 1;
    s32 compLength = 0, compLengthBuf = 0, compLengthOffset = 0,
        maxReferencePlace = minReferencePlace - Yaz0_referanceLengthLimit;

    bool overWriteFlag = false;
    bool ThroughFlag = false;

    if (maxReferencePlace < 0) {
        maxReferencePlace = 0;
    }

    // search dictionaly
    for (u32 dictionaryPlace = maxReferencePlace;
         dictionaryPlace < arg_rSrcPlace; dictionaryPlace++) {
        compLengthBuf = 0;

        while (arg_rSrc.Data[dictionaryPlace + compLengthBuf] ==
               arg_rSrc.Data[arg_rSrcPlace + compLengthBuf]) {
            if (arg_rSrcPlace + compLengthBuf ==
                    arg_rSrc.Size - 1 ||  // FileEnd Over.
                compLengthBuf == Yaz0_maxCompressDataSize) {
                // --compLengthBuf;
                overWriteFlag = true;
                break;
            }

            ++compLengthBuf;
        }

        if (compLengthBuf > compLength) {
            compLength = compLengthBuf;
            compLengthOffset =
                minReferencePlace -
                dictionaryPlace;  // (srcPlace - 1) - Reference Dictionary place
            ThroughFlag = false;
        }
    }

    // search skipping
    if (!overWriteFlag) {
        for (u32 dictionaryPlace = maxReferencePlace;
             dictionaryPlace < arg_rSrcPlace; dictionaryPlace++) {
            u32 ifComp3byte = 0;
            compLengthBuf = 1;

            while (arg_rSrc.Data[dictionaryPlace + compLengthBuf] ==
                   arg_rSrc.Data[arg_rSrcPlace + compLengthBuf]) {
                if (arg_rSrcPlace + compLengthBuf ==
                        arg_rSrc.Size - 1 ||  // FileEnd Over.
                    compLengthBuf == Yaz0_maxCompressDataSize + 0x01) {
                    // --compLengthBuf;
                    break;
                }
                ++compLengthBuf;
            }

            if (compLengthBuf >
                Yaz0_compress2byteFormatMaxLength) {  // 0x11 is 2 byte compress
                                                      // max
                ifComp3byte = 0x01;
            }

            if (compLengthBuf >
                (compLength + arg_return_rCompThroughCount +
                 ifComp3byte)) {  // default is "compLength + 0x01"
                compLength = compLengthBuf;
                ThroughFlag = true;
            }
        }
    }

    // if skipping
    if (ThroughFlag) {
        compLength = 0;
        compLengthOffset = 0;
        arg_return_rCompThroughCount += 0x01;
    } else {
        arg_return_rCompThroughCount = 0x01;
    }

    return_rCompSize = compLength;
    return_rCompOffset = compLengthOffset;
}

Yaz0Encord::arg_pack &Yaz0Encord::new_optimisation(
    arg_pack &return_rOptimizeData) const {
    arg_pack BaseData(new u8[return_rOptimizeData.Size]);

    for (u32 Place = 0; Place < return_rOptimizeData.Size; Place++) {
        BaseData.Data[Place] = return_rOptimizeData.Data[Place];
    }

    return_rOptimizeData.changeDataPointer(BaseData.Data);
    BaseData.detachDataPointer();

    return return_rOptimizeData;
}