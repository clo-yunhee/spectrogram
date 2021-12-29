#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include "view.h"

namespace views {
class Spectrogram : protected View {
   public:
    static constexpr bgfx::ViewId id = 1;

    void init() override;
    void shutdown() override;
    void update() override;
    QRect viewRect() const override;

   public slots:

   private:
    NVGcontext* m_nvg;

    double m_timePerPixel;
};
}  // namespace views

#endif  // SPECTROGRAM_H