#include "spectrogram.h"

#include <QDebug>
#include <QRect>
#include <cmath>

void views::Spectrogram::init(uint32_t clearRgba) {
    View::init(id, "Spectrogram", clearRgba);

    m_image = -1;
}

void views::Spectrogram::shutdown() { nvgDelete(nvg); }

void views::Spectrogram::update() {
    QRect rect = View::updateInternal();

    nvgBeginFrame(nvg, rect.width(), rect.height(), 1.0f);

    nvgBeginPath(nvg);
    nvgFillColor(nvg, nvgRGBA(0, 0, 0, 0));
    nvgRect(nvg, rect.x(), rect.y(), rect.width(), rect.height());
    nvgFill(nvg);

    nvgBeginPath(nvg);
    if (m_image >= 0) {
        nvgFillPaint(nvg, nvgImagePattern(nvg, rect.x(), rect.y(), rect.width(), rect.height(),
                                          0.0f, m_image, 1.0f));
    } else {
        nvgFillColor(nvg, nvgRGBA(0, 0, 0, 0));
    }
    nvgRect(nvg, rect.x(), rect.y(), rect.width(), rect.height());
    nvgFill(nvg);

    nvgEndFrame(nvg);
}

void views::Spectrogram::updateSpectrogramImage(const Eigen::MatrixXd& matrix) {
    const int width = matrix.rows();
    const int height = matrix.cols();

    bool createImage = false;

    if (m_image >= 0) {
        int w, h;
        nvgImageSize(nvg, m_image, &w, &h);

        if (w != width || h != height) {
            nvgDeleteImage(nvg, m_image);
            createImage = true;
        }
    } else {
        createImage = true;
    }

    struct pixel {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    std::vector<pixel> data(width * height);

    const double referenceAmplitude = 1e-2;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double amp = matrix(x, height - 1 - y);

            if (amp > referenceAmplitude) {
                amp = referenceAmplitude;
            }

            amp /= referenceAmplitude;

            pixel px;
            px.r = 255 * amp;
            px.g = 255 * amp;
            px.b = 255 * amp;
            px.a = 255;

            data[y * width + x] = px;
        }
    }

    if (createImage) {
        m_image = nvgCreateImageRGBA(nvg, width, height, NVG_IMAGE_PREMULTIPLIED,
                                     reinterpret_cast<uint8_t*>(data.data()));
    } else {
        nvgUpdateImage(nvg, m_image, reinterpret_cast<uint8_t*>(data.data()));
    }
}
