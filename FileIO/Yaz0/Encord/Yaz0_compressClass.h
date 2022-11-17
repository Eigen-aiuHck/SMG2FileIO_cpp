#pragma once
#include <list>

#include "./myInclude/stipulatedTipe.h"

#define flontNonCompressByteSize 0x08
#define compress2byteFormatMaxLength \
    0x11  // 0x02 + 0x0f : Don't Compress Size + 2byte element numb
#define referanceLengthLimit 0x0fff  // Always 12byte.
#define maxCompressDataSize \
    0x0111  // 0x02 + 0x0f + 0x100 : Don't Compress Size + 2byte element numb +
            // 3byte element numb

#define compressSize_Byte 0
#define compressOffset 1

class Yaz0Encord {
   public:
    u8 *dst = nullptr;
    u32 dstSize = NULL;
    u32 dstPlace = NULL;
    u32 uncompressSize = NULL;
    float compressMultiplication = NULL;

    Yaz0Encord();
    Yaz0Encord(u8 *const sourceData, u32 const sourceDataSize);
    ~Yaz0Encord();

    Yaz0Encord &operator=(std::list<u8> arg_src);

    void newData(u8 *const sourceData, u32 const sourceDataSize);

   private:
    class data_st3 {
       public:
        u8 *Data = nullptr;
        u32 Size = NULL;

        data_st3() { Data = new u8[0]; }
        data_st3(u8 *arg_pOperatorNew) { Data = arg_pOperatorNew; }

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
        virtual ~data_st3() {
            if (Data != nullptr) delete[] Data;
        }
    };

    data_st3 &encode(data_st3 const &arg_rSrc, data_st3 &return_rDst) const;
    u32 *serchDictionaly(data_st3 const &arg_rSrc, u32 const &arg_rSrcPlace,
                         u32 *const return_pCompIndex) const;
    data_st3 &new_optimisation(data_st3 &return_rOptimizeData) const;
    // compressSize_Byte:8or4 offset:12
    // if null, 1 0 output.
};