cmake_minimum_required(VERSION 3.16)
add_library(Yaz0Dec STATIC decord/Yaz0_uncompressClass.cpp)
add_library(Yaz0Enc STATIC encord/Yaz0_compressClass.cpp)