#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QObject>
#include <QSettings>
#include <memory>

#include "../audio/audiodevices.h"
#include "../windows/ui_settings.h"

Q_DECLARE_METATYPE(AudioHostInfo);
Q_DECLARE_METATYPE(AudioDeviceInfo);

class AppController;

class SettingsController : public QObject {
    Q_OBJECT
   public:
    SettingsController();
    virtual ~SettingsController();

    void setWindowParent(AppController&);

   public slots:
    void handleHostInfoRefreshed(const QVector<AudioHostInfo>& infos);
    void handleInputDeviceInfoRefreshed(const QVector<AudioDeviceInfo>& infos);
    void handleOutputDeviceInfoRefreshed(const QVector<AudioDeviceInfo>& infos);

    void show();
    void close();

    void load();
    void save();

   signals:
    void hostChanged(int hostApiIndex);
    void outputDeviceChanged(int deviceHostIndex);
    void inputDeviceChanged(int deviceHostIndex);
    void sampleRateChanged(double sampleRate);
    void frequencyResolutionChanged(int nfft);
    void timeResolutionChanged(double updatesPerSec);

   private slots:
    void handleHostSelected(int index);
    void handleInputDeviceSelected(int index);
    void handleOutputDeviceSelected(int index);
    void handleSampleRateSelected(int index);
    void handleFrequencyResolutionChanged(int index);
    void handleTimeResolutionChanged(double value);

   private:
    void updateResolutionLabels();

    QWidget window;
    Ui::Settings ui;

    std::unique_ptr<QSettings> settings;
};

#endif  // SETTINGSCONTROLLER_H