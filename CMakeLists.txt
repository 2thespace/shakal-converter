cmake_minimum_required(VERSION 3.10)

project(shakal-converter)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/out)
# include_directories(json/include)


set(SRC 
)

add_executable(${PROJECT_NAME} main.cpp
            ImageDecoder.hpp ImageDecoder.cpp 
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/third-party)

