#ifndef VIEW_H
#define VIEW_H

#include <bgfx/bgfx.h>
#include <nanovg.h>

#include <QObject>

class BgfxWidget;

namespace views {
class View : public QObject {
   public:
    virtual void init() = 0;
    virtual void shutdown() = 0;
    virtual void update() = 0;

    virtual QRect viewRect() const = 0;
};
}  // namespace views

#endif  // VIEW_H