#include "audiocontroller.h"

AudioController::AudioController() {
    connect(&devices, &AudioDevices::hostInfoRefreshed, this, &AudioController::hostInfoRefreshed);

    connect(&devices, &AudioDevices::inputDeviceInfoRefreshed, this,
            &AudioController::inputDeviceInfoRefreshed);

    connect(&devices, &AudioDevices::outputDeviceInfoRefreshed, this,
            &AudioController::outputDeviceInfoRefreshed);

    connect(&fileReader, &AudioFileReader::readSuccess, this, &AudioController::fileReadSuccess);

    connect(&fileReader, &AudioFileReader::readFailed, this, &AudioController::fileReadFailed);
}

AudioController::~AudioController() {}

void AudioController::refreshHostInfo() { devices.refreshHostInfo(); }

void AudioController::refreshDeviceInfo(int hostApiIndex) {
    devices.refreshDeviceInfo(hostApiIndex);
}

void AudioController::tryReadFile(const QString& path) { fileReader.tryRead(path); }
