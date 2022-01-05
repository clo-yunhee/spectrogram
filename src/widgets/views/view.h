#ifndef VIEW_H
#define VIEW_H

#include <bgfx/bgfx.h>
#include <nanovg.h>

#include <QObject>
#include <QRect>
#include <QSize>
#include <QWidget>

#include "../layout/layoutitem.h"
#include "../layout/viewlayout.h"

class BgfxWidget;

class View : public LayoutItem {
    Q_OBJECT
   public:
    View();
    virtual ~View() = default;

    virtual void init(uint32_t clearRgba) = 0;
    virtual void shutdown() = 0;
    virtual void update() = 0;

   protected:
    void init(bgfx::ViewId id, const char* name, uint32_t clearRgba);
    QRect updateInternal();

    // NanoVG context
    NVGcontext* nvg;

   private:
    bgfx::ViewId m_id;
    uint32_t m_clearRgba;
};

#endif  // VIEW_H