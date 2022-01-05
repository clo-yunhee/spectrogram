#include "view.h"

#include <QDebug>

View::View() : nvg(nullptr) {}

void View::init(bgfx::ViewId id, const char* name, uint32_t clearRgba) {
    m_id = id;
    m_clearRgba = clearRgba;

    bgfx::setViewName(id, name);

    nvg = nvgCreate(true, id);
    bgfx::setViewMode(id, bgfx::ViewMode::Sequential);
}

QRect View::updateInternal() {
    QRect rect = viewRect();
    bgfx::setViewRect(m_id, rect.x(), rect.y(), rect.width(), rect.height());
    bgfx::setViewClear(m_id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH | BGFX_CLEAR_STENCIL, m_clearRgba,
                       1.0f, 0u);
    bgfx::touch(m_id);
    return rect;
}
