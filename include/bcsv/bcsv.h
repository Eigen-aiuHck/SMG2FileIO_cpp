#pragma once
#include <list>

#include "bcsvFormat.h"
#include "submodules/myInclude/stipulatedTipe.h"

using namespace nintendoFormat;

namespace nintendoFormat {
// 0x00 //long, 4byte
// 0x01 //string, 32byte, no shift
// 0x02 //float, 4byte, no shift
// 0x03 //long, 4byte
// 0x04 //short, 2byte
// 0x05 //char, 8byte
// 0x06 //*string, 4byte

class bcsv {
   public:
    bcsv(u8 const *const arg_pSrcData, u32 const &arg_rSrcDataSize);
    void newData(u8 const *const arg_pSrcData, u32 const &arg_rSrcDataSize);
    std::vector<u8> outputBinary();

    s32 addColumn(u32 const &arg_rInsertPlace,
               bcsvFormatBlock::fieldSection &arg_rFieldData);
    s32 deleteColumn(s32 const &arg_rDeleteNumb);
    s32 addRow(u32 const &arg_rNameHash, u32 const &arg_rDataType);
    s32 deleteRow(u32 const &arg_rDeleteNumb);

    bool editEntry(std::vector<u32> const &arg_rValue, u32 const &arg_rRow,
                   u32 const &arg_rColumn);

    std::list<u32> searchData(bcsvFormatBlock::stringPool const &);
    std::list<u32> serchData(bcsvFormatBlock::dataSection const &,
                             u32 const &arg_DataType);


   protected:
   private:
    bcsvFormatBlock::header Header;
    std::list<bcsvFormatBlock::fieldSection> FieldSection;
    // Entry, <Data>
    std::list<bcsvFormatBlock::dataSection> DataSection;
    bcsvFormatBlock::stringPool StringPool;
    // std::list<bcsvFormatBlock::fieldSection>::iterator FieldSectionItr;
    // std::list<bcsvFormatBlock::dataSection>::iterator DataSectionItr;
    // std::list<bcsvFormatBlock::stringPool>::iterator StringPoolItr;
};
}  // namespace nintendoFormat