#include <list>

#include "bcsvFormat.h"
#include "myinclude/stipulatedTipe.h"

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
    s32 addRow();
    s32 deleteRow(s32 const &arg_deleteNumb);
    s32 addColumn(u32 const &arg_nameHash, u32 const &arg_DataType);
    s32 deleteColumn(u32 const &arg_DeleteNumb);
    s32 deleteColumn(u32 const &arg_DeleteNameHash);

    bool editEntry(std::vector<u32> const &arg_Value, u32 const &arg_row,
                   u32 const &arg_column);

    std::list<u32> searchData(bcsvFormatBlock::stringPool const &);
    std::list<u32> serchData(bcsvFormatBlock::dataSection const &,
                             u32 const &arg_DataType);

   protected:
   private:
    bcsvFormatBlock::header Header;
    std::list<bcsvFormatBlock::fieldSection> FieldSection;
    std::list<bcsvFormatBlock::dataSection> DataSection;
    std::list<bcsvFormatBlock::stringPool> StringPool;
};
}  // namespace nintendoFormat