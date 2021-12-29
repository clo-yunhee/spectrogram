#ifndef AUDIOCONTROLLER_H
#define AUDIOCONTROLLER_H

#include <QObject>
#include <QVector>

#include "../audio/audiodevices.h"
#include "../audio/audiofilereader.h"
#include "../audio/audioguard.h"
#include "../audio/audiotrack.h"

class AudioController : public QObject {
    Q_OBJECT
   public:
    AudioController();
    virtual ~AudioController();

   public slots:
    void refreshHostInfo();
    void refreshDeviceInfo(int hostApiIndex);

    void tryReadFile(const QString& path);

   signals:
    void hostInfoRefreshed(const QVector<AudioHostInfo>& info);
    void inputDeviceInfoRefreshed(const QVector<AudioDeviceInfo> info);
    void outputDeviceInfoRefreshed(const QVector<AudioDeviceInfo> info);

    void fileReadSuccess(const QString& path, const std::vector<double> audio,
                         const double sampleRate);
    void fileReadFailed(const QString& path, const QString& error);

   private:
    AudioGuard guard;
    AudioDevices devices;

    AudioFileReader fileReader;
};

#endif  // AUDIOCONTROLLER_H