#ifndef VIEW_H
#define VIEW_H

#include <bgfx/bgfx.h>

class BgfxWidget;

namespace views {
class View {
   protected:
    virtual void init() = 0;
    virtual void update() = 0;

    friend class BgfxWidget;
};
}  // namespace views

#endif  // VIEW_H