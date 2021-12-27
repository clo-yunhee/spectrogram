cmake_minimum_required(VERSION 3.21)
project(Spectrogram LANGUAGES C CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/find_qt_root.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    set(CMAKE_CXX_FLAGS "-Wall -Wno-sign-compare -fno-math-errno")
    set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -g0 -flto -ftree-vectorize -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
    set(CMAKE_CXX_FLAGS_DEBUG "-Og -g")

    include(CheckLinkerFlag)
    check_linker_flag(CXX "-fuse-ld=lld" can_use_lld)
    if(can_use_lld)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "/W3 /EHsc")
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od")
    set(CMAKE_CXX_FLAGS_RELEASE "/Ob2 /Oi /Os /Oy /GL /DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Zi /Ob2 /Oi /Os /Oy /DNDEBUG")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    message(AUTHOR_WARNING "This compiler (${CMAKE_CXX_COMPILER_ID}) will use CMake's own default compile flags")
endif()

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE was not set: defaults to RelWithDebInfo")
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

string(TOUPPER ${CMAKE_BUILD_TYPE} _flags_var_suffix)
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS_${_flags_var_suffix}} ${CMAKE_CXX_FLAGS}")

add_subdirectory(third_party)
add_subdirectory(src)