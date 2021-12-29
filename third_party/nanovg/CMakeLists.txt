add_library(nanovg INTERFACE)

target_sources(nanovg INTERFACE
    nanovg_bgfx.cpp
    nanovg.cpp
)

target_include_directories(nanovg INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})