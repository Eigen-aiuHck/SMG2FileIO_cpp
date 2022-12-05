cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME "SMG2 FileIO")
set(BUILD_OUTPUT_NAME main_app)
set(SOURCE_FILE_NAME main.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)

project(${PROJECT_NAME}
	VERSION 0.0.1
	DESCRIPTION "Configure template project"
	LANGUAGES CXX)

add_executable(${BUILD_OUTPUT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/source/${SOURCE_FILE_NAME})
target_include_directories(${BUILD_OUTPUT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_compile_features(${BUILD_OUTPUT_NAME} PUBLIC cxx_std_17)
target_compile_options(${BUILD_OUTPUT_NAME} PUBLIC
	$<$<CXX_COMPILER_ID:Clang>:
		$<$<CONFIG:Debug>:-g -O0 -Wall>
		$<$<CONFIG:Release>:-O3 -Wall>
		$<$<CONFIG:RelWithDebgInfo>:-g -O3 -Wall>
	>
)

add_subdirectory(source/bcsv)
add_subdirectory(source/rarc)
add_subdirectory(source/Yaz0)