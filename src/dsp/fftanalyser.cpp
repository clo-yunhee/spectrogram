#include "fftanalyser.h"

FFTAnalyser::FFTAnalyser()
    : m_sampleRate(44100),
      m_nfft(2048),
      m_gapHz(10),
      m_windowLength(2048),
      m_windowGap(4410),
      m_windowType(WindowFunctions::BlackmanNuttall),
      m_plan(nullptr),
      m_data(nullptr) {
    createTransformPlan();
}

FFTAnalyser::~FFTAnalyser() {
    if (m_plan != nullptr) {
        fftw_destroy_plan(m_plan);
        fftw_free(m_data);
    }
}

void FFTAnalyser::setSampleRate(double sampleRate) {
    m_sampleRate = sampleRate;
    checkWindowLengthAndGap();
}

void FFTAnalyser::setTransformSize(int nfft) {
    m_nfft = nfft;
    checkWindowLengthAndGap();
}

void FFTAnalyser::setWindowGap(double gapHz) {
    m_gapHz = gapHz;
    checkWindowLengthAndGap();
}

void FFTAnalyser::setWindowType(WindowFunctions::WindowType type) { m_windowType = type; }

int FFTAnalyser::windowLength() const { return m_windowLength; }

void FFTAnalyser::analyse(const std::vector<double> &audio) {
    const int length = (int)audio.size();

    std::vector<double> window(m_windowLength);
    for (int i = 0; i < m_windowLength; ++i) {
        window[i] = WindowFunctions::eval(m_windowType, i, m_windowLength);
    }

    int start = 0;             // Start (inclusive)
    int end = m_windowLength;  // End (exclusive)

    std::vector<std::vector<double>> matrix;

    while (end <= length) {
        // The analysis segment must be NFFT. Zero-pad the remaining samples.
        for (int i = 0; i < m_windowLength; ++i) {
            m_data[i] = audio[start + i] * window[i];
        }
        for (int i = m_windowLength; i < m_nfft; ++i) {
            m_data[i] = 0;
        }

        fftw_execute(m_plan);

        std::vector<double> column(m_nfft / 2);
        for (int i = 0; i < m_nfft / 2; ++i) {
            const double real = m_data[i];
            const double imag = i > 0 ? m_data[m_nfft - 1 - i] : 0;

            column[i] = real * real + imag * imag;
        }

        matrix.push_back(column);

        start += m_windowGap;
        end += m_windowGap;
    }

    emit analysisDone(matrix);
}

void FFTAnalyser::createTransformPlan() {
    if (m_plan != nullptr) {
        fftw_destroy_plan(m_plan);
        fftw_free(m_data);
    }

    m_data = fftw_alloc_real(m_nfft);
    m_plan = fftw_plan_r2r_1d(m_nfft, m_data, m_data, FFTW_R2HC, FFTW_MEASURE);
}

void FFTAnalyser::checkWindowLengthAndGap() {
    const int gapSamples = std::floor(m_sampleRate / m_gapHz);

    if (gapSamples >= m_nfft) {
        m_windowLength = m_nfft;
    } else {
        m_windowLength = gapSamples;
    }

    m_windowGap = gapSamples;
}
