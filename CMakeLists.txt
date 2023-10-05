cmake_minimum_required(VERSION 3.20)
project(MyAssistC VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
aux_source_directory(./source  PROJECT_SOURCE_DIR)
# FILE(GLOB DIR  ./source/*.cpp)
add_executable(MyAssistC WIN32 ${PROJECT_SOURCE_DIR})   # 创建32位程序

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
