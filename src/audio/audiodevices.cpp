#include "audiodevices.h"

#include <stdexcept>

AudioDevices* AudioDevices::g_instance = nullptr;

AudioDevices::AudioDevices() {
    if (g_instance != nullptr) {
        throw std::logic_error("Only one instance of AudioDevices can exist.");
    }
    g_instance = this;
}

AudioDevices::~AudioDevices() { g_instance = nullptr; }

AudioDevices* AudioDevices::instance() { return g_instance; }

void AudioDevices::refreshHostInfo() {
    const int count = Pa_GetHostApiCount();

    for (int index = 0; index < count; ++index) {
        const auto paInfo = Pa_GetHostApiInfo(index);

        AudioHostInfo info;
        info.index = index;
        info.name = QString::fromUtf8(paInfo->name);
        info.deviceCount = paInfo->deviceCount;
        info.defaultInputDevice = paInfo->defaultInputDevice;
        info.defaultOutputDevice = paInfo->defaultOutputDevice;

        m_hostInfos.append(info);
    }

    emit hostInfoRefreshed(m_hostInfos);
}

void AudioDevices::refreshDeviceInfo(int hostApiIndex) {
    const auto& hostInfo = m_hostInfos.at(hostApiIndex);

    QVector<AudioDeviceInfo> inputDevices;
    QVector<AudioDeviceInfo> outputDevices;

    for (int i = 0; i < hostInfo.deviceCount; ++i) {
        const int index = Pa_HostApiDeviceIndexToDeviceIndex(hostInfo.index, i);
        const auto paInfo = Pa_GetDeviceInfo(index);

        AudioDeviceInfo info;
        info.index = index;
        info.name = QString::fromUtf8(paInfo->name);
        info.defaultInputLatency = paInfo->defaultLowInputLatency;
        info.defaultOutputLatency = paInfo->defaultLowOutputLatency;
        info.defaultSampleRate = paInfo->defaultSampleRate;
        info.availableSampleRates = supportedSampleRates(index);

        if (paInfo->maxInputChannels > 0) {
            inputDevices.append(info);
        }

        if (paInfo->maxOutputChannels > 0) {
            outputDevices.append(info);
        }
    }

    emit inputDeviceInfoRefreshed(inputDevices);
    emit outputDeviceInfoRefreshed(outputDevices);
}

QVector<double> AudioDevices::supportedSampleRates(int deviceIndex) const {
    constexpr double standardSampleRates[] = {8000.0,  9600.0,  11025.0,  12000.0, 16000.0,
                                              22050.0, 24000.0, 32000.0,  44100.0, 48000.0,
                                              88200.0, 96000.0, 192000.0, -1};

    PaStreamParameters parameters;
    parameters.device = deviceIndex;
    parameters.channelCount = 1;
    parameters.sampleFormat = paInt16;
    parameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
    parameters.hostApiSpecificStreamInfo = NULL;

    QVector<double> sampleRates;

    for (int i = 0; standardSampleRates[i] > 0; ++i) {
        const double sampleRate = standardSampleRates[i];

        PaError err = Pa_IsFormatSupported(nullptr, &parameters, sampleRate);
        if (err == paFormatIsSupported) {
            sampleRates.append(sampleRate);
        }
    }

    return sampleRates;
}