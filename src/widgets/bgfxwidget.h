#ifndef BGFXWIDGET_H
#define BGFXWIDGET_H

#include <bgfx/bgfx.h>

#include <QTimer>
#include <QWidget>
#include <memory>

#include "views/spectrogram.h"

class BgfxWidget : public QWidget {
    Q_OBJECT
   public:
    BgfxWidget(QWidget *parent);
    virtual ~BgfxWidget();

   public slots:
    void init();
    void update();
    void reset();

   private slots:
    void handleTimeout();

   private:
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    bool m_isBgfxInitialised;

    uint32_t m_debugFlags;
    uint32_t m_resetFlags;

   public:
    QTimer *m_updateTimer;

    std::unique_ptr<ViewLayout> m_viewLayout;

    struct {
        views::Spectrogram spectrogram;
    } views;
};

#endif  // BGFXWIDGET_H