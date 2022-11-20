#pragma once
#include <list>
#include <vector>

#include "./myInclude/stipulatedTipe.h"

namespace nintendoFormat {
#define Yaz0_flontNonCompressByteSize 0x08
#define Yaz0_compress2byteFormatMaxLength \
    0x11  // 0x02 + 0x0f : Don't Compress Size + 2byte element numb
#define Yaz0_referanceLengthLimit 0x0fff  // Always 12byte.
#define Yaz0_maxCompressDataSize \
    0x0111  // 0x02 + 0x0f + 0x100 : Don't Compress Size + 2byte element numb +
            // 3byte element numb

#define Yaz0_compressSize_Byte 0
#define Yaz0_compressOffset 1

class Yaz0Encord {
   public:
    Yaz0Encord();
    Yaz0Encord(u8 *const sourceData, u32 const sourceDataSize);
    ~Yaz0Encord();

    Yaz0Encord &operator=(std::list<u8> arg_src);

    void newData(u8 *const sourceData, u32 const sourceDataSize);

    std::vector<u8> getFormatData();
    u32 getUncompressSize();
    float getcompressMultiplication();

   private:
    u8 *dst = nullptr;
    u32 dstSize = NULL;
    u32 dstPlace = NULL;
    u32 uncompressSize = NULL;
    float compressMultiplication = NULL;

    class arg_pack;

    void reset();
    arg_pack &encode(arg_pack const &arg_rSrc, arg_pack &return_rDst) const;
    void serchDictionaly(arg_pack const &arg_rSrc, u32 const &arg_rSrcPlace,
                         u32 &return_rCompSize, u32 &return_rCompOffset,
                         u32 &arg_return_rCompThroughNumb) const;
    arg_pack &new_optimisation(arg_pack &return_rOptimizeData) const;
    // compressSize_Byte:8or4 offset:12
    // if null, 1 0 output.
};

class Yaz0Encord::arg_pack {
   public:
    u8 *Data = nullptr;
    u32 Size = NULL;

    arg_pack() { Data = new u8[0]; }
    arg_pack(u8 *arg_pOperatorNew) { Data = arg_pOperatorNew; }

    virtual void changeDataPointer(u8 *arg_pOperatorNew) {
        delete[] Data;
        Data = arg_pOperatorNew;
        return;
    }
    virtual void moveDataPointer(u8 *&arg_toType) {
        arg_toType = Data;
        Data = nullptr;
    }
    virtual void detachDataPointer() { Data = nullptr; }
    virtual ~arg_pack() {
        if (Data != nullptr) delete[] Data;
    }
};
}  // namespace nintendoFormat