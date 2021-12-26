set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

add_executable(Spectrogram WIN32
    main.cpp
)

target_link_libraries(Spectrogram PRIVATE fftw3)

target_compile_definitions(Spectrogram PRIVATE _UNICODE)

target_link_libraries(Spectrogram PRIVATE Qt5::Core Qt5::Widgets fftw3)
