#include "Yaz0_compressClass.h"

#include <iomanip>
#include <list>

#include "./myInclude/stipulatedTipe.h"

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
    data_st3 src;
    src.changeDataPointer(pSourceData);
    src.Size = SourceDataSize;

    compressMultiplication = 0.0f;

    // main branch
    data_st3 dst_buf;
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

Yaz0Encord::data_st3 &Yaz0Encord::encode(data_st3 const &arg_rSrc,
                                         data_st3 &return_rDst) const {
    return_rDst.changeDataPointer(new u8[arg_rSrc.Size]);

    // null chack
    if (arg_rSrc.Size < 8) {
        return_rDst.Size = 0;

        return return_rDst;
    }

    u32 srcPlace = 0, dstPlace = 0;

    // init decided format area
    return_rDst.Data[dstPlace++] = 0xff;  // uncompress flag write

    while (srcPlace < flontNonCompressByteSize) {
        // not conpress dictionaly
        return_rDst.Data[dstPlace++] = arg_rSrc.Data[srcPlace++];
    }

    // main
    u32 uncompressBitCount = 0, uncompressFlagWritePlace = 0;
    u32 compressVal[3];
    compressVal[2] = 0x00;

    while (srcPlace < arg_rSrc.Size) {
        if (uncompressBitCount == 0) {
            // std::cout << std::hex
            //           << (u32)return_rDst.Data[uncompressFlagWritePlace]
            //           << std::endl;  // dev
            uncompressFlagWritePlace = dstPlace++;
            return_rDst.Data[uncompressFlagWritePlace] = 0x00;
            uncompressBitCount = 8;
        }
        uncompressBitCount--;

        // serch(function)
        serchDictionaly(arg_rSrc, srcPlace, compressVal);
        //_ASSERTE(_CrtCheckMemory());

        // std::cout << compressVal[compressSize_Byte] << std::endl; //dev

        if (compressVal[compressSize_Byte] < 3) {
            // don't compress
            return_rDst.Data[uncompressFlagWritePlace] |= 0x01
                                                          << uncompressBitCount;
            return_rDst.Data[dstPlace++] = arg_rSrc.Data[srcPlace++];
        } else {
            // compress
            if (compressVal[compressSize_Byte] <=
                compress2byteFormatMaxLength) {
                // 2 byte format
                compressVal[compressSize_Byte] -= 0x02;
                return_rDst.Data[dstPlace++] =
                    ((compressVal[compressSize_Byte] & 0x0f) << 4) |  // length
                    (((compressVal[1]) & 0x0f00) >> 8);               // offset
                return_rDst.Data[dstPlace++] =
                    compressVal[1] & 0x00ff;  // offset

                srcPlace += (compressVal[compressSize_Byte] & 0x0f) +
                            0x02;  // seek compressed length
                // _ASSERTE(_CrtCheckMemory());
            } else {
                // 3 byte format
                compressVal[compressSize_Byte] -= 0x12;
                return_rDst.Data[dstPlace++] =
                    (compressVal[1] & 0x0f00) >> 8;  // offset
                return_rDst.Data[dstPlace++] =
                    compressVal[1] & 0x00ff;  // offset
                return_rDst.Data[dstPlace++] =
                    compressVal[compressSize_Byte] & 0x00ff;  // length

                srcPlace += (compressVal[compressSize_Byte] & 0x00ff) +
                            0x12;  // seek compressed length
                // _ASSERTE(_CrtCheckMemory());
            }
            // _ASSERTE(_CrtCheckMemory());
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

u32 *Yaz0Encord::serchDictionaly(data_st3 const &arg_rSrc,
                                 u32 const &arg_rSrcPlace,
                                 u32 *const return_pCompIndex) const {
    int const minReferencePlace = arg_rSrcPlace - 1;
    int compLength = 0, compLengthBuf = 0, compLengthOffset = 0,
        maxReferencePlace = minReferencePlace - referanceLengthLimit;

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
                compLengthBuf == maxCompressDataSize) {
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
                    compLengthBuf == maxCompressDataSize + 0x01) {
                    // --compLengthBuf;
                    break;
                }
                ++compLengthBuf;
            }

            if (compLengthBuf >
                compress2byteFormatMaxLength) {  // 0x11 is 2 byte compress max
                ifComp3byte = 0x01;
            }

            if (compLengthBuf >
                (compLength + return_pCompIndex[2] +
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
        return_pCompIndex[2] += 0x01;
    } else {
        return_pCompIndex[2] = 0x01;
    }

    return_pCompIndex[0] = compLength;
    return_pCompIndex[1] = compLengthOffset;
    return return_pCompIndex;
}

Yaz0Encord::data_st3 &Yaz0Encord::new_optimisation(
    data_st3 &return_rOptimizeData) const {
    data_st3 BaseData(new u8[return_rOptimizeData.Size]);

    for (u32 Place = 0; Place < return_rOptimizeData.Size; Place++) {
        BaseData.Data[Place] = return_rOptimizeData.Data[Place];
    }

    return_rOptimizeData.changeDataPointer(BaseData.Data);
    BaseData.detachDataPointer();

    return return_rOptimizeData;
}