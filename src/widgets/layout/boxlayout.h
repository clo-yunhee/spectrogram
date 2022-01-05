#ifndef BOXLAYOUT_H
#define BOXLAYOUT_H

#include "viewlayout.h"

class BoxLayout : public ViewLayout {
    Q_OBJECT
   public:
    enum Direction {
        Horizontal = 0,
        Vertical,
    };

    BoxLayout(Direction direction);
    virtual ~BoxLayout();

    void setSpacing(int spacing);
    int spacing() const;

    void addItem(LayoutItem *item) override;
    void removeItem(LayoutItem *item) override;

    void calculateLayout() override;

   private:
    Direction m_direction;

    int m_spacing;
    QList<LayoutItem *> m_items;
};

#endif  // BOXLAYOUT_H