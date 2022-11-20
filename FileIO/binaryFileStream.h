#include <filesystem>
#include <fstream>
#include <list>
#include <string>
#include <vector>

#include "myinclude/stipulatedTipe.h"

class binaryFileStream {
   private:
    std::filesystem::path TergetName;

   protected:
   public:
    std::string TergetName();
    void setTergetName(std::string);
    void setTergetName(std::filesystem::path);

    std::list<u8> read();
    std::list<u8> read(std::string arg_FilePath);
    std::list<u8> read(std::filesystem::path arg_FilePath);

    bool write(std::list<u8> arg_writeData);
    bool write(std::string arg_FilePath, std::list<u8> arg_writeData);
    bool write(std::filesystem::path arg_FilePath, std::list<u8> arg_writeData);
};