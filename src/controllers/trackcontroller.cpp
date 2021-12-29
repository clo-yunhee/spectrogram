#include "trackcontroller.h"

#include <QFileInfo>

TrackController::TrackController() {}

TrackController::~TrackController() {}

void TrackController::handleFileReadSuccess(const QString& path, const std::vector<double> audio,
                                            const double sampleRate) {
    currentFilePath = path;
    currentFileName = QFileInfo(path).fileName();

    emit currentFileChanged(currentFilePath, currentFileName);

    track.clear();
    track.setSampleRate(sampleRate);
    track.append(audio);

    // refreshAnalysis();
}

/*

void AppWindow::refreshAnalysis() {
    std::vector<double> audio;
    m_audioTrack.retrieveData(audio);
    m_fftAnalyser.analyse(audio);
}

void AppWindow::handleAnalysisDone(const Eigen::MatrixXd& matrix) {
    const double sampleRate = m_audioTrack.sampleRate();
}

*/