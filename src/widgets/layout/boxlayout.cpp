#include "boxlayout.h"

BoxLayout::BoxLayout(Direction direction) : m_direction(direction) {
    connect(this, &LayoutItem::viewRectChanged, this, &BoxLayout::calculateLayout);
}

BoxLayout::~BoxLayout() {}

void BoxLayout::setSpacing(int spacing) {
    m_spacing = spacing;
    calculateLayout();
}

int BoxLayout::spacing() const { return m_spacing; }

void BoxLayout::addItem(LayoutItem* item) {
    m_items.append(item);
    item->setLayout(this);
}
void BoxLayout::removeItem(LayoutItem* item) {
    m_items.removeAll(item);
    item->setLayout(this);
}

void BoxLayout::calculateLayout() {
    const QRect layoutRect = viewRect();

    const int itemCount = m_items.count();

    int marginTop = 20;
    int marginBottom = 20;
    int marginLeft = 20;
    int marginRight = 20;

    int currentX = layoutRect.x() + marginLeft;
    int currentY = layoutRect.y() + marginTop;
    int layoutWidth = layoutRect.width() - marginRight;
    int layoutHeight = layoutRect.height() - marginBottom;

    for (int i = 0; i < itemCount; ++i) {
        auto& item = m_items.at(i);

        const auto horizontalPolicy = item->horizontalSizePolicy();
        const auto verticalPolicy = item->verticalSizePolicy();

        const QSize max = item->maximumSize();
        const QSize min = item->minimumSize();
        const QSize hint = item->sizeHint();

        QRect itemRect;
        itemRect.setX(currentX);
        itemRect.setY(currentY);

        if (hint.isValid()) {
            itemRect.setSize(hint);
        } else {
            itemRect.setWidth(layoutWidth);
            itemRect.setHeight(layoutHeight);
        }

        item->setViewRect(itemRect);

        if (m_direction == Horizontal) {
            currentX += itemRect.width();
            currentX += m_spacing;
        } else if (m_direction == Vertical) {
            currentY += itemRect.height();
            currentY += m_spacing;
        }
    }
}