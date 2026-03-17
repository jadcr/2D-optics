#ifndef MOVABLEITEM_H
#define MOVABLEITEM_H

#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include "opticalelement.h"
#include "sphericalinterface.h"

class MainWindow;

// Для линий (StraightInterface и Detector)
class MovableLineItem : public QGraphicsLineItem
{
public:
    MovableLineItem(OpticalElement* element, MainWindow* mw,
                    const QPointF& p1, const QPointF& p2,
                    QGraphicsItem* parent = nullptr);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    OpticalElement* m_element;
    MainWindow*     m_mainWindow;
    QPointF         m_localP1;  // смещение первого конца относительно позиции элемента
    QPointF         m_localP2;  // смещение второго конца
};

// Для окружностей (SphericalInterface)
class MovableEllipseItem : public QGraphicsEllipseItem
{
public:
    MovableEllipseItem(SphericalInterface* element, MainWindow* mw,
                       const QPointF& center, double radius,
                       QGraphicsItem* parent = nullptr);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    SphericalInterface* m_element;
    MainWindow*         m_mainWindow;
};

class MovableSourceItem : public QGraphicsItem
{
public:
    MovableSourceItem(MainWindow* mw, const QPointF& pos, QGraphicsItem* parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    MainWindow* m_mainWindow;
};

#endif // MOVABLEITEM_H
