#ifndef SCALETRANSFORM_H
#define SCALETRANSFORM_H

#include <Eigen/Dense>
#include <cmath>

class ScaleTransform {
   public:
    enum Type {
        TypeLinear,
        TypeLogarithmic,
        TypeMel,
        TypeErb,
    };

    ScaleTransform(Type type, double sampleRate, int nfft, double minFreq, double maxFreq,
                   int numBins);

    Type type() const;
    double sampleRate() const;
    int nfft() const;
    double minFrequency() const;
    double maxFrequency() const;
    int numBins() const;

    bool matches(ScaleTransform::Type type, double sampleRate, int nfft, double minFreq,
                 double maxFreq, int numBins) const;

    template <class Derived>
    decltype(auto) apply(Eigen::DenseBase<Derived> data) {
        return m_transform * data;
    }

   private:
    void assignScaleFunctions();
    void createTransformMatrix();

    Type m_type;
    double m_sampleRate;
    int m_nfft;
    double m_minFreq;
    double m_maxFreq;
    int m_numBins;

    double (*hzToScale)(double);
    double (*scaleToHz)(double);

    Eigen::MatrixXd m_transform;
};

namespace t {
inline double id(double f) { return f; }
inline double hzToLog(double f) { return std::log2(f); }
inline double logToHz(double f) { return std::pow(2, f); }
inline double hzToMel(double f) { return 1127 * std::log(1 + f / 700); }
inline double melToHz(double m) { return 700 * (exp(m / 1127) - 1); }
}  // namespace t

#endif  // SCALETRANSFORM_H