#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include "view.h"

namespace views {
class Spectrogram : protected View {
   public:
    static constexpr bgfx::ViewId id = 0;

   protected:
    void init() override;
    void update() override;
};
}  // namespace views

#endif  // SPECTROGRAM_H