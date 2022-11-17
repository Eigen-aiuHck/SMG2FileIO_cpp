#pragma once
#include <string>
#include <vector>

#include "./myInclude/stipulatedTipe.h"
#include "rarc_Format.h"

#define headerSize 0x20
#define dataHeaderSize 0x20
#define dirNodeSectionSize 0x10
#define fileNodeSectionSize 0x10

// default int32 size is 32bit.

class rarc {
    rarc_FormatBlock::header Header;
    rarc_FormatBlock::dataHeader DataHeader;
    std::vector<rarc_FormatBlock::dirNodeSection> DirNodeSection;
    std::vector<rarc_FormatBlock::fileNodeSection> FileNodeSection;
    std::vector<std::vector<u8>> StringData;
    std::vector<std::vector<u8>> FileData;

   protected:
   public:
    bool addFile(std::string const &FileName, std::vector<u8> const &FileData);
    bool addDir(std::string const &DirName);

    bool delFile(std::string const &FileName);
    bool delDir(std::string const &DirName);

    bool editFile(std::string const &FileName, std::vector<u8> const &FileData);
    bool moveFile(std::string const &TergetPath, std::string &ReplacePath);
    bool copyFile(std::string const &TergetPath, std::string &CopyFilePath);
    bool editDir(std::string const &DirName);
};