#ifndef BGFXWIDGET_H
#define BGFXWIDGET_H

#include <bgfx/bgfx.h>

#include <QtWidgets>

class BgfxWidget : public QWidget {
   public:
    BgfxWidget(QWidget *parent);
    virtual ~BgfxWidget();

   public slots:
    void init();
    void update();
    void reset();

   private:
    void timerEvent(QTimerEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

   private:
    bool m_isBgfxInitialised;

    uint32_t m_debugFlags;
    uint32_t m_resetFlags;
};

#endif  // BGFXWIDGET_H