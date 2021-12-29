#include "scaletransform.h"

#include <QtGlobal>

ScaleTransform::ScaleTransform(Type type, double sampleRate, int nfft, double minFreq,
                               double maxFreq, int numBins)
    : m_type(type),
      m_sampleRate(sampleRate),
      m_nfft(nfft),
      m_minFreq(minFreq),
      m_maxFreq(maxFreq),
      m_numBins(numBins) {
    assignScaleFunctions();
    createTransformMatrix();
}

ScaleTransform::Type ScaleTransform::type() const { return m_type; }

double ScaleTransform::sampleRate() const { return m_sampleRate; }

int ScaleTransform::nfft() const { return m_nfft; }

double ScaleTransform::minFrequency() const { return m_minFreq; }

double ScaleTransform::maxFrequency() const { return m_maxFreq; }

int ScaleTransform::numBins() const { return m_numBins; }

bool ScaleTransform::matches(const ScaleTransform::Type type, const double sampleRate,
                             const int nfft, const double minFreq, const double maxFreq,
                             const int numBins) const {
    return m_type == type && qFuzzyCompare(m_sampleRate, sampleRate) && m_nfft == nfft &&
           qFuzzyCompare(m_minFreq, minFreq) && qFuzzyCompare(m_maxFreq, maxFreq) &&
           m_numBins == numBins;
}

void ScaleTransform::assignScaleFunctions() {
    switch (m_type) {
        case TypeLinear:
            hzToScale = scaleToHz = t::id;
            break;
        case TypeLogarithmic:
            hzToScale = t::hzToLog;
            scaleToHz = t::logToHz;
            break;
        case TypeMel:
            hzToScale = t::hzToMel;
            scaleToHz = t::melToHz;
            break;
        default:
            break;
    }
}

void ScaleTransform::createTransformMatrix() {
    Eigen::VectorXd centerFreqs(m_numBins);

    const double minScale = hzToScale(m_minFreq);
    const double maxScale = hzToScale(m_maxFreq);

    for (int i = 0; i < m_numBins; ++i) {
        centerFreqs(i) = scaleToHz(minScale + i * (maxScale - minScale) / double(m_numBins - 1));
    }

    m_transform.setZero(m_numBins, m_nfft / 2);

    for (int i = 0; i < m_numBins; ++i) {
        const double startFrequency = centerFreqs(std::max(0, i - 1));
        const double centerFrequency = centerFreqs(i);
        const double endFrequency = centerFreqs(std::min(m_numBins - 1, i + 1));

        const int startCol = (int)std::round(m_nfft * startFrequency / m_sampleRate);
        const int centerCol = (int)std::round(m_nfft * centerFrequency / m_sampleRate);
        const int endCol = (int)std::round(m_nfft * endFrequency / m_sampleRate);

        for (int j = startCol; j < centerCol; ++j) {
            if (j < m_nfft) m_transform(i, j) = double(j - startCol) / double(centerCol - startCol);
        }
        for (int j = centerCol; j <= endCol; ++j) {
            if (j < m_nfft) m_transform(i, j) = double(j - centerCol) / double(endCol - centerCol);
        }
    }
}