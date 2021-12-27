#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QtWidgets>

#include "../audio/audiodevices.h"
#include "ui/ui_settings.h"

class SettingsWindow : public QWidget {
   public:
    SettingsWindow(QWidget* parent);
    virtual ~SettingsWindow();

   public slots:
    void handleHostInfoRefreshed(const QVector<AudioHostInfo>& infos);
    void handleInputDeviceInfoRefreshed(const QVector<AudioDeviceInfo>& infos);
    void handleOutputDeviceInfoRefreshed(const QVector<AudioDeviceInfo>& infos);

   private slots:
    void handleInputDeviceSelected(int index);
    void handleOutputDeviceSelected(int index);
    void handleSampleRateSelected(int index);
    void handleFrequencyResolutionChanged(int index);
    void handleTimeResolutionChanged(double value);

   private:
    void loadSettings();
    void saveSettings();

    void updateResolutionLabels();

    Ui::Settings ui;

    QVector<AudioHostInfo> m_hostInfos;
    QVector<AudioDeviceInfo> m_deviceInfos;
    QVector<double> m_sampleRates;

    QSettings* m_settings;
};

#endif  // SETTINGSWINDOW_H