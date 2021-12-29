#include "audiofilereader.h"

#include <set>

AudioFileReader::AudioFileReader() {}

AudioFileReader::~AudioFileReader() {}

QStringList AudioFileReader::supportedFileFormats() {
    // List supported file formats by this build of sndfile.
    SF_FORMAT_INFO format_info;
    int k, count;

    sf_command(nullptr, SFC_GET_SIMPLE_FORMAT_COUNT, &count, sizeof(int));

    std::set<QString> extSet;

    for (k = 0; k < count; k++) {
        format_info.format = k;
        sf_command(nullptr, SFC_GET_SIMPLE_FORMAT, &format_info, sizeof(format_info));
        extSet.emplace(QString("*.%1").arg(format_info.extension));
    };

    return QStringList(extSet.cbegin(), extSet.cend());
}

void AudioFileReader::tryRead(const QString& path) {
    SF_INFO sfInfo;
    sfInfo.format = 0;

    // Open the file

    const std::string str = path.toStdString();
    SNDFILE* sndfile = sf_open(str.c_str(), SFM_READ, &sfInfo);

    if (sndfile == nullptr) {
        emit readFailed(path, sf_strerror(sndfile));
        return;
    }

    // Read the audio as double precision floats.

    const double sampleRate = sfInfo.samplerate;
    const sf_count_t frames = sfInfo.frames;
    const int channels = sfInfo.channels;

    std::vector<double> interleaved(frames * channels);

    sf_command(sndfile, SFC_SET_SCALE_FLOAT_INT_READ, nullptr, SF_TRUE);
    sf_readf_double(sndfile, interleaved.data(), frames);

    if (sf_close(sndfile) != 0) {
        emit readFailed(path, sf_strerror(sndfile));
    }

    // Rudimentary mix down to mono. (avg over all channels)

    std::vector<double> mono(frames);

    for (int i = 0; i < frames; ++i) {
        mono[i] = 0.0;
        for (int c = 0; c < channels; ++c) {
            mono[i] += interleaved[i * channels + c];
        }
        mono[i] /= channels;
    }

    emit readSuccess(path, mono, sampleRate);
}