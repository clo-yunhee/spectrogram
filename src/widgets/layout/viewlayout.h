#ifndef VIEWLAYOUT_H
#define VIEWLAYOUT_H

#include "layoutitem.h"

class ViewLayout : public LayoutItem {
    Q_OBJECT
   public:
    virtual ~ViewLayout();
    virtual void addItem(LayoutItem *item) = 0;
    virtual void removeItem(LayoutItem *item) = 0;
    virtual void calculateLayout() = 0;
};

#endif  // VIEWLAYOUT_H