#ifndef STRAIGHTINTERFACE_H
#define STRAIGHTINTERFACE_H

#include "opticalelement.h"

class StraightInterface : public OpticalElement
{
public:
    StraightInterface(const QPointF& p1, const QPointF& p2,
                      double n1 = 1.0, double n2 = 1.5, bool reflective = false,
                      const QString& name = "Straight");

    QPointF p1() const { return m_p1; }
    QPointF p2() const { return m_p2; }
    void setPoints(const QPointF& p1, const QPointF& p2);

    bool intersect(const Ray& ray, double& t, QPointF& point, QVector2D& normal) const override;
    void process(Ray& ray, const QPointF& point, const QVector2D& normal) const override;
    QString info() const override;

private:
    QPointF m_p1, m_p2;
    QVector2D m_direction;
    QVector2D m_normal;
};

#endif // STRAIGHTINTERFACE_H
