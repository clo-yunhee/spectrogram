#include "layoutitem.h"

#include <QDebug>

#include "viewlayout.h"

LayoutItem::LayoutItem()
    : m_sizeHint(),
      m_minimumSize(),
      m_maximumSize(),
      m_horizontalSizePolicy(Preferred),
      m_verticalSizePolicy(Preferred),
      m_layout(nullptr) {}

LayoutItem::~LayoutItem() {}

QSize LayoutItem::sizeHint() const { return m_sizeHint; }
QSize LayoutItem::minimumSize() const { return m_minimumSize; }
QSize LayoutItem::maximumSize() const { return m_maximumSize; }
LayoutItem::SizePolicy LayoutItem::horizontalSizePolicy() const { return m_horizontalSizePolicy; }
LayoutItem::SizePolicy LayoutItem::verticalSizePolicy() const { return m_verticalSizePolicy; }
QRect LayoutItem::viewRect() const { return m_viewRect; }

void LayoutItem::setSizeHint(const QSize& size) {
    m_sizeHint = size;
    emit sizeHintChanged(size);
}

void LayoutItem::setMinimumSize(const QSize& size) {
    m_minimumSize = size;
    emit minimumSizeChanged(size);
}

void LayoutItem::setMaximumSize(const QSize& size) {
    m_maximumSize = size;
    emit maximumSizeChanged(size);
}

void LayoutItem::setSizePolicy(const SizePolicy& horizontal, const SizePolicy& vertical) {
    m_horizontalSizePolicy = horizontal;
    m_verticalSizePolicy = vertical;
    emit sizePolicyChanged(horizontal, vertical);
}

void LayoutItem::setHorizontalSizePolicy(const SizePolicy& policy) {
    m_horizontalSizePolicy = policy;
    emit sizePolicyChanged(policy, m_verticalSizePolicy);
}

void LayoutItem::setVerticalSizePolicy(const SizePolicy& policy) {
    m_verticalSizePolicy = policy;
    emit sizePolicyChanged(m_horizontalSizePolicy, policy);
}

void LayoutItem::setViewRect(const QRect& rect) {
    m_viewRect = rect;
    emit viewRectChanged(rect);
}

void LayoutItem::setLayout(ViewLayout* layout) {
    m_layout = layout;
    if (layout != nullptr) {
        layout->calculateLayout();
    } else {
        setViewRect(QRect(QPoint(0, 0), sizeHint()));
    }
}