#include "spectrogram.h"

#include <QRect>

void views::Spectrogram::init() {
    bgfx::setViewName(id, "Spectrogram");
    bgfx::setViewClear(id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f);

    m_nvg = nvgCreate(true, id);
    bgfx::setViewMode(id, bgfx::ViewMode::Sequential);
}

void views::Spectrogram::shutdown() { nvgDelete(m_nvg); }

void views::Spectrogram::update() {
    QRect rect = viewRect();
    bgfx::setViewRect(id, rect.x(), rect.y(), rect.width(), rect.height());
    bgfx::touch(id);

    nvgBeginFrame(m_nvg, rect.width(), rect.height(), 1.0f);

    nvgFillColor(m_nvg, nvgRGBA(255, 255, 255, 255));
    nvgRect(m_nvg, 0, 0, rect.width(), rect.height());
    nvgFill(m_nvg);

    nvgEndFrame(m_nvg);

    bgfx::frame();
}

QRect views::Spectrogram::viewRect() const { return QRect(0, 0, 640, 480); }