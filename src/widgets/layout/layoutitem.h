#ifndef LAYOUTITEM_H
#define LAYOUTITEM_H

#include <QObject>
#include <QRect>
#include <QSize>

class ViewLayout;

class LayoutItem : public QObject {
    Q_OBJECT
   public:
    enum SizePolicyFlag {
        GrowFlag = 0b0001,    // can grow if needed
        ExpandFlag = 0b0010,  // will grow as much as possible
        ShrinkFlag = 0b0100,  // can shrink if needed
        IgnoreFlag = 0b1000,  // gets as much space as possible
    };

    enum SizePolicy {
        Fixed = 0,
        Minimum = GrowFlag,
        Maximum = ShrinkFlag,
        Preferred = GrowFlag | ShrinkFlag,
        Expanding = GrowFlag | ShrinkFlag | ExpandFlag,
        MinimumExpanding = GrowFlag | ExpandFlag,
        Ignored = ShrinkFlag | GrowFlag | IgnoreFlag,
    };

    LayoutItem();
    virtual ~LayoutItem();

    QSize sizeHint() const;
    QSize minimumSize() const;
    QSize maximumSize() const;
    SizePolicy horizontalSizePolicy() const;
    SizePolicy verticalSizePolicy() const;
    QRect viewRect() const;

   public slots:
    void setSizeHint(const QSize& size);
    void setMinimumSize(const QSize& size);
    void setMaximumSize(const QSize& size);
    void setSizePolicy(const SizePolicy& horizontalPolicy, const SizePolicy& verticalPolicy);
    void setHorizontalSizePolicy(const SizePolicy& policy);
    void setVerticalSizePolicy(const SizePolicy& policy);
    void setViewRect(const QRect& rect);
    void setLayout(ViewLayout* layout);

   signals:
    void sizeHintChanged(QSize);
    void minimumSizeChanged(QSize);
    void maximumSizeChanged(QSize);
    void sizePolicyChanged(SizePolicy, SizePolicy);
    void viewRectChanged(QRect);
    void layoutChanged(ViewLayout*);

   private:
    QSize m_sizeHint;
    QSize m_minimumSize;
    QSize m_maximumSize;
    SizePolicy m_horizontalSizePolicy;
    SizePolicy m_verticalSizePolicy;
    ViewLayout* m_layout;

    QRect m_viewRect;
};

#endif  // LAYOUTITEM_H