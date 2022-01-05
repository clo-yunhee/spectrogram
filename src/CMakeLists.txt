set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

add_executable(Spectrogram WIN32
    audio/audiodevices.cpp
    audio/audiodevices.h
    audio/audiofilereader.cpp
    audio/audiofilereader.h
    audio/audioguard.h
    audio/audiotrack.cpp
    audio/audiotrack.cpp
    controllers/appcontroller.cpp
    controllers/appcontroller.h
    controllers/audiocontroller.cpp
    controllers/audiocontroller.h
    controllers/maincontroller.cpp
    controllers/maincontroller.h
    controllers/settingscontroller.cpp
    controllers/settingscontroller.h
    controllers/trackcontroller.cpp
    controllers/trackcontroller.h
    dsp/fftanalyser.cpp
    dsp/fftanalyser.h
    dsp/windowfunctions.cpp
    dsp/windowfunctions_defs.h
    dsp/windowfunctions.h
    spectrogram/scaletransform.cpp
    spectrogram/scaletransform.h
    spectrogram/spectrumdata.cpp
    spectrogram/spectrumdata.h
    spectrogram/transformcache.cpp
    spectrogram/transformcache.h
    widgets/layout/boxlayout.cpp
    widgets/layout/boxlayout.h
    widgets/layout/layoutitem.cpp
    widgets/layout/layoutitem.h
    widgets/layout/viewlayout.cpp
    widgets/layout/viewlayout.h
    widgets/views/spectrogram.cpp
    widgets/views/spectrogram.h
    widgets/views/view.cpp
    widgets/views/view.h
    widgets/bgfxwidget.cpp
    widgets/bgfxwidget.h
    windows/appwindow.cpp
    windows/appwindow.h
    main.cpp
)

target_compile_definitions(Spectrogram PRIVATE
    UNICODE _UNICODE
    _USE_MATH_DEFINES
    EIGEN_USE_BLAS
)

target_link_libraries(Spectrogram PRIVATE
    Qt5::Core Qt5::Widgets
    eigen3 openblas_static fftw3
    portaudio_static sndfile
    bgfx bx nanovg
)

add_custom_command(TARGET Spectrogram POST_BUILD
    COMMAND ${CMAKE_COMMAND}
        -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CROSSCOMPILING=${CMAKE_CROSSCOMPILING}
        -DTARGET_EXECUTABLE=$<TARGET_FILE:Spectrogram>
        -DQT_RUNTIME_DIR=${QT_RUNTIME_DIR}
        -DQT_LIBRARY_DIR=${QT_LIBRARY_DIR}
        -DQT_PLUGIN_DIR=${QT_PLUGIN_DIR}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/copy_runtime_deps.cmake
)
