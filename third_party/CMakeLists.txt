#-- Build FFTW3

set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "" FORCE)
set(BUILD_TESTS OFF CACHE INTERNAL "" FORCE)

if(MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    # Special flag behaviour on MSVC.
    # The SSE/SSE2 flags aren't recognised on 64-bit builds,
    # but we should still build with SSE/SSE2 support.

    # Bypass FFTW3's flag check and directly set the variables.
    set(ENABLE_SSE  OFF CACHE INTERNAL "" FORCE)
    set(HAVE_SSE    TRUE CACHE INTERNAL "" FORCE)
    set(ENABLE_SSE2 OFF CACHE INTERNAL "" FORCE)
    set(HAVE_SSE2   TRUE CACHE INTERNAL "" FORCE)
else()
    # On other compilers we can just have default behaviour.
    set(ENABLE_SSE  ON CACHE INTERNAL "" FORCE)
    set(ENABLE_SSE2 ON CACHE INTERNAL "" FORCE)
endif()

# We always check for AVX/AVX2, even on 32-bit builds.
set(ENABLE_AVX  ON CACHE INTERNAL "" FORCE)
set(ENABLE_AVX2 ON CACHE INTERNAL "" FORCE)

add_subdirectory(fftw-3.3.10)
target_include_directories(fftw3 INTERFACE $<TARGET_PROPERTY:fftw3,SOURCE_DIR>/api)

#-- Build PortAudio

set(PA_BUILD_STATIC ON CACHE INTERNAL "" FORCE)
set(PA_BUILD_SHARED OFF CACHE INTERNAL "" FORCE)

add_subdirectory(portaudio)
target_include_directories(portaudio_static INTERFACE $<TARGET_PROPERTY:portaudio_static,SOURCE_DIR>/include)

#-- Build bgfx

add_subdirectory(bgfx.cmake)

#-- Build libsndfile

set(BUILD_TESTING OFF CACHE INTERNAL "" FORCE)
set(BUILD_PROGRAMS OFF CACHE INTERNAL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE INTERNAL "" FORCE)
set(ENABLE_CPACK OFF CACHE INTERNAL "" FORCE)
set(ENABLE_PACKAGE_CONFIG OFF CACHE INTERNAL "" FORCE)
set(INSTALL_PKGCONFIG_MODULE OFF CACHE INTERNAL "" FORCE)
add_subdirectory(libsndfile)

#-- Build nanovg
add_subdirectory(nanovg)