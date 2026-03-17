#ifndef COORDGRID_H
#define COORDGRID_H

#include <QGraphicsItem>
#include <qpen.h>

class CoordinateGrid : public QGraphicsItem
{
public:
    explicit CoordinateGrid(QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    qreal computeStep(qreal scale) const;
    qreal m_desiredPixelStep;
    QPen m_axisPen;
    QPen m_tickPen;
};

#endif // COORDGRID_H
