#include "appwindow.h"

#include <set>

AppWindow::AppWindow() : m_settings(this) {
    ui.setupUi(this);

    QObject::connect(ui.actionOpen, &QAction::triggered, this, &AppWindow::handleOpenFile);
    QObject::connect(ui.actionSettings, &QAction::triggered, &m_settings, &QWidget::showNormal);

    QObject::connect(&m_audioDevices, &AudioDevices::hostInfoRefreshed, &m_settings,
                     &SettingsWindow::handleHostInfoRefreshed);

    QObject::connect(&m_audioDevices, &AudioDevices::inputDeviceInfoRefreshed, &m_settings,
                     &SettingsWindow::handleInputDeviceInfoRefreshed);

    QObject::connect(&m_audioDevices, &AudioDevices::outputDeviceInfoRefreshed, &m_settings,
                     &SettingsWindow::handleOutputDeviceInfoRefreshed);

    QObject::connect(&m_audioFileReader, &AudioFileReader::readSuccess, this, &AppWindow::handleFileReadSuccess);

    QObject::connect(&m_audioFileReader, &AudioFileReader::readFailed, this, &AppWindow::handleFileReadFailed);

    QObject::connect(&m_fftAnalyser, &FFTAnalyser::analysisDone,
                     [this]() { QMessageBox::information(this, "Analysis done", "Analysis done"); });

    m_audioDevices.refreshHostInfo();
}

AppWindow::~AppWindow() {}

void AppWindow::handleOpenFile() {
    QString extFilter = m_audioFileReader.supportedFileFormats().join(", ");
    QString fileName = QFileDialog::getOpenFileName(this, "Open audio file",
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    QString("Audio files (%1)").arg(extFilter));

    m_audioFileReader.tryRead(fileName);

    setWindowTitle(QString("Spectrogram - %1").arg(QFileInfo(fileName).fileName()));
}

void AppWindow::handleFileReadSuccess(const std::vector<double>& audio, const double sampleRate) {
    m_audioTrack.clear();
    m_audioTrack.setSampleRate(sampleRate);
    m_audioTrack.append(audio);

    refreshAnalysis();
}

void AppWindow::handleFileReadFailed(const QString& error) {
    QMessageBox::warning(this, "Failed to read audio file", QString("Error: %1").arg(error));
    setWindowTitle("Spectrogram");
}

void AppWindow::refreshAnalysis() {
    std::vector<double> audio;
    m_audioTrack.retrieveData(audio);
    m_fftAnalyser.analyse(audio);
}