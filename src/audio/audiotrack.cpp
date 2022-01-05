#include "audiotrack.h"

#include <iostream>

AudioTrack::AudioTrack() : m_sampleRate(44100) {}

AudioTrack::~AudioTrack() {}

void AudioTrack::clear() {
    m_mutex.lock();
    m_audio.clear();
    m_mutex.unlock();
}

double AudioTrack::sampleRate() const { return m_sampleRate; }

void AudioTrack::setSampleRate(const double sampleRate) {
    m_sampleRate = sampleRate;
    emit sampleRateChanged(sampleRate);
}

int AudioTrack::length() const { return (int)m_audio.size(); }

double AudioTrack::duration() const { return m_audio.size() / m_sampleRate; }

void AudioTrack::retrieveData(std::vector<double>& dest, const int start, int length) {
    m_mutex.lock_shared();

    if (length < 0) {
        length = (int)m_audio.size() - start;
    }

    if (start < 0 || start >= m_audio.size()) {
        m_mutex.unlock_shared();
        std::cerr << "AudioTrack::retrieveData: start index out of range";
        return;
    }

    if (length <= 0 || start + length > m_audio.size()) {
        m_mutex.unlock_shared();
        std::cerr << "AudioTrack::retrieveData: end index out of range";
        return;
    }

    auto begin = std::next(m_audio.cbegin(), start);
    auto end = std::next(begin, length);

    dest.resize(length);
    std::copy(begin, end, dest.begin());

    m_mutex.unlock_shared();
}

void AudioTrack::append(const std::vector<double>& audio) {
    m_mutex.lock();
    m_audio.insert(m_audio.end(), audio.cbegin(), audio.cend());
    m_mutex.unlock();

    emit lengthChanged(length());
    emit durationChanged(duration());
    emit audioAdded();
}