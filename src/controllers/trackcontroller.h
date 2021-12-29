#ifndef TRACKCONTROLLER_H
#define TRACKCONTROLLER_H

#include <QObject>

#include "../audio/audiotrack.h"

class TrackController : public QObject {
    Q_OBJECT
   public:
    TrackController();
    virtual ~TrackController();

   public slots:
    void handleFileReadSuccess(const QString& path, const std::vector<double> audio,
                               const double sampleRate);

   signals:
    void currentFileChanged(const QString& path, const QString& fileName);

   private:
    QString currentFilePath;
    QString currentFileName;

    AudioTrack track;
};

#endif  // TRACKCONTROLLER_H