#ifndef FFTANALYSER_H
#define FFTANALYSER_H

#include <fftw3.h>

#include <Eigen/Dense>
#include <QObject>

#include "windowfunctions.h"

class FFTAnalyser : public QObject {
    Q_OBJECT
   public:
    FFTAnalyser();
    virtual ~FFTAnalyser();

    void setSampleRate(double sampleRate);
    void setTransformSize(int nfft);
    void setWindowGap(double gapHz);
    void setWindowType(WindowFunctions::WindowType type);

    int windowLength() const;

    void analyse(const std::vector<double>& audio);

   signals:
    void analysisDone(Eigen::MatrixXd matrix);

   private:
    void createTransformPlan();
    void checkWindowLengthAndGap();

    double m_sampleRate;
    int m_nfft;
    double m_gapHz;

    int m_windowLength;
    int m_windowGap;
    WindowFunctions::WindowType m_windowType;

    fftw_plan m_plan;
    double* m_data;
};

#endif  // FFTANALYSER_H