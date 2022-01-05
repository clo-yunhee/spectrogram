#ifndef TRACKCONTROLLER_H
#define TRACKCONTROLLER_H

#include <QObject>

#include "../audio/audiotrack.h"
#include "../dsp/fftanalyser.h"
#include "../spectrogram/transformcache.h"

class TrackController : public QObject {
    Q_OBJECT
   public:
    TrackController();
    virtual ~TrackController();

   public slots:
    void handleFileReadSuccess(const QString& path, const std::vector<double> audio,
                               const double sampleRate);

    void handleFrequencyResolutionChanged(int nfft);
    void handleTimeResolutionChanged(double updatesPerSec);
    void handleWindowFunctionChanged(WindowFunctions::Type windowType);
    void handleMinFrequencyChanged(double minFreq);
    void handleMaxFrequencyChanged(double maxFreq);
    void handleFrequencyScaleChanged(ScaleTransform::Type scaleType);

   signals:
    void currentFileChanged(const QString& path, const QString& fileName);

    void spectrogramUpdated(const Eigen::MatrixXd& matrix);

   private slots:
    void analyseSpectrogram();
    void rescaleSpectrogram();

    void handleAnalysisDone(const Eigen::MatrixXd& matrix);

   private:
    QString currentFilePath;
    QString currentFileName;

    AudioTrack track;

    int nfft;
    double updatesPerSec;
    WindowFunctions::Type windowType;
    double minFreq;
    double maxFreq;
    ScaleTransform::Type freqScale;

    FFTAnalyser fftAnalyser;
    TransformCache transformCache;

    Eigen::MatrixXd unscaledSpectrogram;
    Eigen::MatrixXd frequencyScaledSpectrogram;
};

#endif  // TRACKCONTROLLER_H