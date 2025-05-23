set(BUILD_SHARED_LIBS OFF)
option(BUILD_STATIC_LIBS "Build static libraries" ON)


# For zlib
set(SKIP_INSTALL_ALL ON CACHE BOOL "Skip installation" FORCE)
set(ZLIB_BUILD_TESTING OFF)
set(AMD64 ON CACHE BOOL "Enable AMD64 optimizations" FORCE)
set(ZLIB_INSTALL OFF)
set(ZLIB_BUILD_STATIC ON)

# For libpng
set(PNG_TESTS OFF CACHE BOOL "Disable libpng tests" FORCE)
set(PNG_SHARED OFF CACHE BOOL "Build static libpng" FORCE)
set(PNG_STATIC ON CACHE BOOL "Build static libpng" FORCE)
set(PNG_TOOLS OFF)
# set(PNG_EXECUTABLES OFF CACHE BOOL "Disable building executables" FORCE)


add_subdirectory(zlib)
set(ZLIB_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/zlib")
set(ZLIB_LIBRARY ZLIB::ZLIBSTATI CACHE STRING "zlib static library target")
# message(FATAL_ERROR "HERE" ${ZLIB_INCLUDE_DIR})
find_package(ZLIB REQUIRED)
add_subdirectory(libpng)

add_library(third-party INTERFACE)
target_link_libraries(third-party INTERFACE ZLIB::ZLIBSTATIC png_static)
target_include_directories(third-party INTERFACE "${CMAKE_CURRENT_LIST_DIR}/libpng")
add_library(shakal::third-party ALIAS third-party)