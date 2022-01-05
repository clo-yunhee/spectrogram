#include "trackcontroller.h"

#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>

TrackController::TrackController() {
    connect(&fftAnalyser, &FFTAnalyser::analysisDone, this, &TrackController::handleAnalysisDone);

    nfft = 2048;
    minFreq = 1;
    maxFreq = 10000;
    freqScale = ScaleTransform::TypeLinear;
}

TrackController::~TrackController() {}

void TrackController::handleFileReadSuccess(const QString& path, const std::vector<double> audio,
                                            const double sampleRate) {
    currentFilePath = path;
    currentFileName = QFileInfo(path).fileName();

    emit currentFileChanged(currentFilePath, currentFileName);

    track.clear();
    track.setSampleRate(sampleRate);
    track.append(audio);

    fftAnalyser.setSampleRate(track.sampleRate());

    analyseSpectrogram();
}

void TrackController::handleFrequencyResolutionChanged(int nfft) {
    this->nfft = nfft;
    fftAnalyser.setTransformSize(nfft);

    analyseSpectrogram();
}

void TrackController::handleTimeResolutionChanged(double updatesPerSec) {
    this->updatesPerSec = updatesPerSec;
    fftAnalyser.setWindowGap(updatesPerSec);

    analyseSpectrogram();
}

void TrackController::handleWindowFunctionChanged(WindowFunctions::Type windowType) {
    this->windowType = windowType;
    fftAnalyser.setWindowType(windowType);

    analyseSpectrogram();
}

void TrackController::handleMinFrequencyChanged(double minFreq) {
    this->minFreq = minFreq;

    rescaleSpectrogram();
}

void TrackController::handleMaxFrequencyChanged(double maxFreq) {
    this->maxFreq = maxFreq;

    rescaleSpectrogram();
}

void TrackController::handleFrequencyScaleChanged(ScaleTransform::Type scaleType) {
    freqScale = scaleType;

    rescaleSpectrogram();
}

void TrackController::analyseSpectrogram() {
    std::vector<double> audio;
    track.retrieveData(audio);
    if (!audio.empty()) {
        fftAnalyser.analyse(audio);
    }
}

void TrackController::rescaleSpectrogram() {
    const double sampleRate = track.sampleRate();

    const auto& frequencyTransform =
        transformCache.get(freqScale, sampleRate, nfft, minFreq, maxFreq, 2000);

    frequencyScaledSpectrogram = frequencyTransform.apply(unscaledSpectrogram);

    if (frequencyScaledSpectrogram.hasNaN()) {
        QMessageBox::critical(nullptr, "Frequency scaled spectrogram has NaN",
                              "Frequency scaled spectrogram has NaN");
    }

    emit spectrogramUpdated(frequencyScaledSpectrogram);
}

void TrackController::handleAnalysisDone(const Eigen::MatrixXd& matrix) {
    unscaledSpectrogram = matrix;
    rescaleSpectrogram();
}
