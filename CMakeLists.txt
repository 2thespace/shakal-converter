cmake_minimum_required(VERSION 3.10)

project(shakal-converter)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Builds test for project" OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/out)
# include_directories(json/include)

add_subdirectory(third-party)

add_library(${PROJECT_NAME} STATIC ImageDecoder.hpp ImageDecoder.cpp )
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC shakal::third-party)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/third-party)
add_library(shakal::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

if(BUILD_TESTS) 
    add_executable(test_runner main.cpp)
    target_link_libraries(test_runner PUBLIC shakal::${PROJECT_NAME})
endif()





