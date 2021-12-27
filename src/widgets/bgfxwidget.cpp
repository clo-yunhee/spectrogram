#include "bgfxwidget.h"

#include <bgfx/platform.h>

BgfxWidget::BgfxWidget(QWidget *parent)
    : QWidget(parent), m_isBgfxInitialised(false), m_debugFlags(BGFX_DEBUG_TEXT), m_resetFlags(BGFX_RESET_VSYNC) {
    init();
}

BgfxWidget::~BgfxWidget() {
    if (m_isBgfxInitialised) {
        bgfx::shutdown();
    }
}

void BgfxWidget::init() {
    if (m_isBgfxInitialised) {
        bgfx::shutdown();
    }

    bgfx::Init init;
    init.type = bgfx::RendererType::Count;
    init.vendorId = BGFX_PCI_ID_NONE;
    init.deviceId = 0;
    init.platformData.nwh = (void *)winId();
    init.resolution.format = bgfx::TextureFormat::RGBA32U;
    init.resolution.width = width();
    init.resolution.height = height();
    init.resolution.reset = m_resetFlags;

    if (!bgfx::init(init)) {
        QMessageBox::critical(this, "Could not init bgfx renderer", "Error in bgfx::init");
        qApp->quit();
    }

    m_isBgfxInitialised = true;

    bgfx::setDebug(m_debugFlags);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0);

    startTimer(33);
}

void BgfxWidget::update() {
    bgfx::setViewRect(0, 0, 0, width(), height());
    bgfx::touch(0);

    bgfx::dbgTextClear();

    const bgfx::Stats *stats = bgfx::getStats();
    bgfx::dbgTextPrintf(0, 0, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width,
                        stats->height, stats->textWidth, stats->textHeight);

    bgfx::frame();
}

void BgfxWidget::reset() { bgfx::reset(width(), height(), m_resetFlags, bgfx::TextureFormat::RGBA32U); }

void BgfxWidget::timerEvent(QTimerEvent *event) {
    if (m_isBgfxInitialised) {
        update();
    }
}

void BgfxWidget::showEvent(QShowEvent *event) {
    if (m_isBgfxInitialised) {
        reset();
    }
}

void BgfxWidget::resizeEvent(QResizeEvent *event) {
    if (m_isBgfxInitialised) {
        reset();
    }
}
