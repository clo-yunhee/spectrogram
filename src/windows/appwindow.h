#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QtWidgets>

#include "../audio/audiodevices.h"
#include "../audio/audiofilereader.h"
#include "../audio/audioguard.h"
#include "../audio/audiotrack.h"
#include "../dsp/fftanalyser.h"
#include "settingswindow.h"
#include "ui/ui_app.h"

class AppWindow : public QMainWindow {
   public:
    AppWindow();
    virtual ~AppWindow();

   private slots:
    void handleOpenFile();
    void handleFileReadSuccess(const std::vector<double>& audio, double sampleRate);
    void handleFileReadFailed(const QString& error);

    void refreshAnalysis();

   private:
    Ui::App ui;

    AudioGuard m_audioGuard;
    AudioDevices m_audioDevices;

    AudioFileReader m_audioFileReader;
    AudioTrack m_audioTrack;

    FFTAnalyser m_fftAnalyser;

    SettingsWindow m_settings;
};

#endif  // APPWINDOW_H