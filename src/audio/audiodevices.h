#ifndef AUDIODEVICES_H
#define AUDIODEVICES_H

#include <portaudio.h>

#include <QObject>
#include <QString>
#include <QVector>

struct AudioHostInfo {
    int index;
    QString name;
    int deviceCount;
    int defaultInputDevice;
    int defaultOutputDevice;
};

struct AudioDeviceInfo {
    int index;
    QString name;
    int defaultInputLatency;
    int defaultOutputLatency;
    double defaultSampleRate;
    QVector<double> availableSampleRates;
};

class AudioDevices : public QObject {
    Q_OBJECT
   public:
    AudioDevices();
    virtual ~AudioDevices();

    static AudioDevices* instance();

   public slots:
    void refreshHostInfo();
    void refreshDeviceInfo(int hostApiIndex);

   signals:
    void hostInfoRefreshed(const QVector<AudioHostInfo>& infos);

    void inputDeviceInfoRefreshed(const QVector<AudioDeviceInfo> infos);
    void outputDeviceInfoRefreshed(const QVector<AudioDeviceInfo> infos);

   private:
    QVector<double> supportedSampleRates(int deviceIndex) const;

    QVector<AudioHostInfo> m_hostInfos;

    static AudioDevices* g_instance;
};

#endif  // AUDIODEVICES_H