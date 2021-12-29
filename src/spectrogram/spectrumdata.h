#ifndef SPECTRUMDATA_H
#define SPECTRUMDATA_H

#include <Eigen/Dense>

class SpectrumData {
   public:
    SpectrumData(double sampleRate, int nfft, const Eigen::ArrayXd &data);

   private:
    double m_sampleRate;
    int m_nfft;
    Eigen::ArrayXd m_data;
};

#endif  // SPECTRUMDATA_H