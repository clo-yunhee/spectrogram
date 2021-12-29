#ifndef AUDIOFILEREADER_H
#define AUDIOFILEREADER_H

#include <sndfile.h>

#include <QObject>

class AudioFileReader : public QObject {
    Q_OBJECT
   public:
    AudioFileReader();
    virtual ~AudioFileReader();

    static QStringList supportedFileFormats();

   public slots:
    void tryRead(const QString& path);

   signals:
    void readSuccess(const QString& path, const std::vector<double> audio, double sampleRate);
    void readFailed(const QString& path, const QString& error);
};

#endif  // AUDIOFILEREADER_H
