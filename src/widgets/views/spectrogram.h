#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <Eigen/Dense>

#include "view.h"

namespace views {
class Spectrogram : public View {
   public:
    static constexpr bgfx::ViewId id = 1;

    void init(uint32_t clearRgba) override;
    void shutdown() override;
    void update() override;

   public slots:
    void updateSpectrogramImage(const Eigen::MatrixXd& matrix);

   private:
    int m_image;

    double m_timePerPixel;
};
}  // namespace views

#endif  // SPECTROGRAM_H