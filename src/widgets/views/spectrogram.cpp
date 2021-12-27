#include "spectrogram.h"

void views::Spectrogram::init() {
    bgfx::setViewName(id, "Spectrogram");
    bgfx::setViewClear(id, BGFX_CLEAR_COLOR, 0x000000ff);
}

void views::Spectrogram::update() {}