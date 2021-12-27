#ifndef AUDIOFILEREADER_H
#define AUDIOFILEREADER_H

#include <sndfile.h>

#include <QObject>

class AudioFileReader : public QObject {
    Q_OBJECT
   public:
    AudioFileReader();
    virtual ~AudioFileReader();

    QStringList supportedFileFormats() const;

   public slots:
    void tryRead(const QString& path);

   signals:
    void readSuccess(const std::vector<double> audio, double sampleRate);
    void readFailed(const QString& error);
};

#endif  // AUDIOFILEREADER_H
