#include "coordgrid.h"
#include <QPainter>
#include <cmath>

CoordinateGrid::CoordinateGrid(QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_desiredPixelStep(50.0)          // ~50 пикселей между делениями
    , m_axisPen(Qt::black, 2.0)
    , m_tickPen(Qt::darkGray, 1.5)
{
    setZValue(-1000);
    setFlag(ItemIsSelectable, false);
    setAcceptedMouseButtons(Qt::NoButton);
}

QRectF CoordinateGrid::boundingRect() const
{
    return QRectF(-1000, -1000, 2000, 2000);
}

qreal CoordinateGrid::computeStep(qreal scale) const
{
    if (scale <= 0.0) return 1.0;
    qreal rawStep = m_desiredPixelStep / scale;
    qreal exponent = std::floor(std::log10(rawStep));
    qreal mantissa = rawStep / std::pow(10.0, exponent);

    // Округление до 1, 2, 5 или 10
    if (mantissa < 1.5)
        mantissa = 1.0;
    else if (mantissa < 3.5)
        mantissa = 2.0;
    else if (mantissa < 7.5)
        mantissa = 5.0;
    else
        mantissa = 10.0;

    return mantissa * std::pow(10.0, exponent);
}

void CoordinateGrid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();

    qreal scale = painter->transform().m11();   // текущий масштаб (по X и Y считаем одинаковым)
    qreal step = computeStep(scale);

    // Видимая область в координатах сцены (с запасом)
    QRectF viewRect = painter->transform().inverted().mapRect(
        QRectF(0, 0, painter->device()->width(), painter->device()->height()));
    qreal left = viewRect.left() - step;
    qreal right = viewRect.right() + step;
    qreal bottom = viewRect.bottom() + step;
    qreal top = viewRect.top() - step;

    // Рисуем оси
    painter->setPen(m_axisPen);
    painter->drawLine(QPointF(left, 0), QPointF(right, 0)); // ось X
    painter->drawLine(QPointF(0, bottom), QPointF(0, top)); // ось Y

    // Стрелки на концах осей
    painter->drawLine(QPointF(right - 5, -5), QPointF(right, 0));
    painter->drawLine(QPointF(right - 5, 5), QPointF(right, 0));
    painter->drawLine(QPointF(-5, top - 5), QPointF(0, top));
    painter->drawLine(QPointF(5, top - 5), QPointF(0, top));

    // Рисуем тики (деления) на осях
    painter->setPen(m_tickPen);
    int tickSize = 5;
    qreal tickSceneSize = tickSize / scale;

    // Тики на оси X (вертикальные чёрточки)
    int startCol = static_cast<int>(std::floor(left / step));
    int endCol = static_cast<int>(std::ceil(right / step));
    for (int i = startCol; i <= endCol; ++i) {
        qreal x = i * step;
        if (qAbs(x) < 1e-9) continue;
        painter->drawLine(QPointF(x, -tickSceneSize), QPointF(x, tickSceneSize));
    }

    // Тики на оси Y (используем top как минимум, bottom как максимум)
    int startRow = static_cast<int>(std::floor(top / step));
    int endRow = static_cast<int>(std::ceil(bottom / step));
    for (int i = startRow; i <= endRow; ++i) {
        qreal y = i * step;
        if (qAbs(y) < 1e-9) continue;
        painter->drawLine(QPointF(-tickSceneSize, y), QPointF(tickSceneSize, y));
    }

    // Подписи делений (только на осях)
    qreal labelInterval = step * 5.0; // подписываем каждое 5-е деление
    if (labelInterval < 1.0) labelInterval = 1.0;
    if (labelInterval > 100.0) labelInterval = 100.0;

    painter->setPen(Qt::black);
    painter->setFont(QFont("Arial", 8));

    // Подписи на оси X (под осью)
    int startLabelX = static_cast<int>(std::floor(left / labelInterval));
    int endLabelX = static_cast<int>(std::ceil(right / labelInterval));
    for (int i = startLabelX; i <= endLabelX; ++i) {
        qreal x = i * labelInterval;
        painter->drawText(QPointF(x - 5, 15), QString::number(x, 'f', 2));
    }

    // Подписи на оси Y
    int startLabelY = static_cast<int>(std::floor(top / labelInterval));
    int endLabelY = static_cast<int>(std::ceil(bottom / labelInterval));
    for (int i = startLabelY; i <= endLabelY; ++i) {
        qreal y = i * labelInterval;
        painter->drawText(QPointF(10, y - 5), QString::number(y, 'f', 2));
    }

    // Подписи самих осей
    painter->drawText(QPointF(right - 10, -10), "X");
    painter->drawText(QPointF(5, top - 10), "Y");

    painter->restore();
}
