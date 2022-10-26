cmake_minimum_required(VERSION 3.0)
project(mlock_test C CXX)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(MIMALLOC_PATH "/Users/serinahu/Documents/Whist/whist/protocol/build/lib/64/mimalloc/Darwin")
include_directories(SYSTEM "/Users/serinahu/Documents/Whist/mimalloc/include")
find_library(LIB_MIMALLOC
    NAMES mimalloc
    PATHS ${MIMALLOC_PATH}
    NO_DEFAULT_PATH
    NO_CMAKE_ENVIRONMENT_PATH
    NO_CMAKE_PATH
    NO_SYSTEM_ENVIRONMENT_PATH
    NO_CMAKE_SYSTEM_PATH
    NO_CMAKE_FIND_ROOT_PATH)
message(STATUS "Found mimalloc lib: ${LIB_MIMALLOC}")

add_executable(mlock_test mlock_test.c)
target_link_libraries(mlock_test PUBLIC ${LIB_MIMALLOC})

