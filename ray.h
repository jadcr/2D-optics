#ifndef RAY_H
#define RAY_H

#include <QPointF> //точки
#include <QVector2D> //векторы
#include <QList>//списки

class Ray
{
public:
    Ray(const QPointF& origin, const QVector2D& direction);
    Ray(const Ray& other);
    // геттеры и сеттеры
    QPointF origin() const { return m_origin; }
    QVector2D direction() const { return m_direction; }
    bool isActive() const { return m_active; }
    void setActive(bool active) { m_active = active; }
    const QList<QPointF>& path() const { return m_path; }

    QPointF pointAt(double t) const;
    void propagate(double t);
    void setDirection(const QVector2D& dir);
    void addPoint(const QPointF& pt);
    void clearPath();

private:
    QPointF m_origin;
    QVector2D m_direction;
    bool m_active;
    QList<QPointF> m_path;
};

#endif // RAY_H
