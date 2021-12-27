set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

add_executable(Spectrogram WIN32
    audio/audiodevices.cpp
    audio/audiodevices.h
    dsp/windowfunctions.cpp
    dsp/windowfunctions_defs.h
    dsp/windowfunctions.h
    widgets/bgfxwidget.cpp
    widgets/bgfxwidget.h
    windows/appwindow.cpp
    windows/appwindow.h
    windows/settingswindow.cpp
    windows/settingswindow.h
    main.cpp
)

target_link_libraries(Spectrogram PRIVATE fftw3 portaudio_static bgfx bx)

target_compile_definitions(Spectrogram PRIVATE UNICODE _UNICODE BX_CONFIG_DEBUG)

target_link_libraries(Spectrogram PRIVATE Qt5::Core Qt5::Widgets fftw3)

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
