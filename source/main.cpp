#include <iostream>
#include <iomanip>

#include "submodules/myInclude/convertEndian.h"

int main() {
    u8 test1 = 0x11;
    u16 test2 = 0x1122;
    u32 test3 = 0x11223344;
    std::cout << std::hex << (u16)test1 << std::endl;
    std::cout << std::hex << test2 << std::endl;
    std::cout << std::hex << test3 << std::endl;
    test1 = convertEndian(test1);
    test2 = convertEndian(test2);
    test3 = convertEndian(test3);
    std::cout << std::hex << (u16)test1 << std::endl;
    std::cout << std::hex << test2 << std::endl;
    std::cout << std::hex << test3 << std::endl;
    return 0;
}