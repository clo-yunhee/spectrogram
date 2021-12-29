#include "bgfxwidget.h"

inline uint32_t argbToRgba(uint32_t argb) {
    return
        // Source is in format: 0xAARRGGBB
        ((argb & 0x00FF0000) << 8) |  // RR______
        ((argb & 0x0000FF00) << 8) |  // __GG____
        ((argb & 0x000000FF) << 8) |  // ____BB__
        ((argb & 0xFF000000) >> 24);  // ______AA
    // Return value is in format: 0xRRGGBBAA
}

BgfxWidget::BgfxWidget(QWidget *parent)
    : QWidget(parent), m_isBgfxInitialised(false), m_debugFlags(0), m_resetFlags(BGFX_RESET_VSYNC) {
    init();
}

BgfxWidget::~BgfxWidget() {
    if (m_isBgfxInitialised) {
        views.spectrogram.shutdown();
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

    QPalette pal = palette();
    uint32_t clearColor = argbToRgba(pal.color(QPalette::Window).rgba());

    bgfx::setViewName(0, "Base");
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, clearColor, 1.0f);

    views.spectrogram.init();

    startTimer(33);
}

void BgfxWidget::update() {
    bgfx::setViewRect(0, 0, 0, width(), height());
    bgfx::touch(0);

    views.spectrogram.update();
}

void BgfxWidget::reset() {
    bgfx::reset(width(), height(), m_resetFlags, bgfx::TextureFormat::RGBA32U);
}

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
