#ifndef AUDIOTRACK_H
#define AUDIOTRACK_H

#include <QObject>
#include <shared_mutex>
#include <vector>

class AudioTrack : public QObject {
    Q_OBJECT
   public:
    AudioTrack();
    virtual ~AudioTrack();

    void clear();

    double sampleRate() const;
    void setSampleRate(double sampleRate);

    int length() const;
    double duration() const;

    void retrieveData(std::vector<double>& dest, int start = 0, int length = -1);

   public slots:
    void append(const std::vector<double>& audio);

   signals:
    void sampleRateChanged(double sampleRate);
    void lengthChanged(int length);
    void durationChanged(double duration);
    void audioAdded();

   private:
    double m_sampleRate;

    std::shared_mutex m_mutex;
    std::vector<double> m_audio;
};

#endif  // AUDIOTRACK_H