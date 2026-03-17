#include "movableitem.h"
#include "detector.h"
#include "mainwindow.h"
#include "straightinterface.h"
#include <QPen>
#include <qpainter.h>

MovableLineItem::MovableLineItem(OpticalElement* element, MainWindow* mw,
                                 const QPointF& p1, const QPointF& p2,
                                 QGraphicsItem* parent)
    : QGraphicsLineItem(parent)
    , m_element(element)
    , m_mainWindow(mw)
{
    // Центр отрезка – позиция элемента в сцене
    QPointF center = (p1 + p2) / 2.0;
    setPos(center);
    m_localP1 = p1 - center;
    m_localP2 = p2 - center;
    setLine(QLineF(m_localP1, m_localP2));

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    if (dynamic_cast<Detector*>(element))
        setPen(QPen(Qt::red, 3));
    else
        setPen(QPen(Qt::blue, 1));
}

QVariant MovableLineItem::itemChange(QGraphicsItem::GraphicsItemChange change,
                                     const QVariant& value)
{
    if (change == ItemPositionHasChanged && m_mainWindow)
    {
        QPointF newPos = pos();
        QPointF newP1 = newPos + m_localP1;
        QPointF newP2 = newPos + m_localP2;

        if (auto* line = dynamic_cast<StraightInterface*>(m_element))
            line->setPoints(newP1, newP2);
        else if (auto* det = dynamic_cast<Detector*>(m_element))
            det->setPoints(newP1, newP2);

        m_mainWindow->elementMoved(m_element);
    }
    return QGraphicsLineItem::itemChange(change, value);
}

MovableEllipseItem::MovableEllipseItem(SphericalInterface* element, MainWindow* mw,
                                       const QPointF& center, double radius,
                                       QGraphicsItem* parent)
    : QGraphicsEllipseItem(parent)
    , m_element(element)
    , m_mainWindow(mw)
{
    setPos(center);
    setRect(QRectF(-radius, -radius, 2*radius, 2*radius));
    setPen(QPen(Qt::blue, 0.5));

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

QVariant MovableEllipseItem::itemChange(QGraphicsItem::GraphicsItemChange change,
                                        const QVariant& value)
{
    if (change == ItemPositionHasChanged && m_mainWindow)
    {
        QPointF newCenter = pos();
        m_element->setCenter(newCenter);
        m_mainWindow->elementMoved(m_element);
    }
    return QGraphicsEllipseItem::itemChange(change, value);
}

MovableSourceItem::MovableSourceItem(MainWindow* mw, const QPointF& pos, QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , m_mainWindow(mw)
{
    setPos(pos);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setZValue(1000); // поверх всех элементов
}

QRectF MovableSourceItem::boundingRect() const
{
    return QRectF(-100, -100, 100, 100);
}

void MovableSourceItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();

    const SourceParams& params = m_mainWindow->sourceParams();

    if (params.type == 0) // параллельный пучок
    {

       // QVector2D dir = params.direction.normalized();

       // QVector2D perp(-dir.y(), dir.x());

        // Длина линии: ширина пучка
       // double spacing = params.spacing;
       // int count = params.rayCount;
       // double halfLength = (spacing * (count - 1)) ;
       // if (halfLength < 1.0) halfLength = 5.0;

      // QPointF p1 = (perp).toPointF();
      //  QPointF p2 = (perp).toPointF();

        // Рисуем линию
      //  painter->setPen(QPen(Qt::yellow, 4));
      //  painter->drawLine(p1, p2);
       qreal size = 3;
       painter->setBrush(Qt::yellow);
       painter->setPen(Qt::black);
       painter->drawRect(QRectF(-size, -size, 2*size, 2*size));

    }
    else // точечный источник
    {
        //квадратик
        qreal size = 3;
        painter->setBrush(Qt::yellow);
        painter->setPen(Qt::black);
        painter->drawRect(QRectF(-size, -size, 2*size, 2*size));

    }

    painter->restore();
}

QVariant MovableSourceItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemPositionHasChanged && m_mainWindow)
    {
        m_mainWindow->sourceMoved(pos());
        if (scene()) scene()->update();
    }
    return QGraphicsItem::itemChange(change, value);
}




