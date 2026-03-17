#ifndef SPHERICALINTERFACE_H
#define SPHERICALINTERFACE_H

#include "opticalelement.h"

class SphericalInterface : public OpticalElement
{
public:
    SphericalInterface(const QPointF& center, double radius,
                       double n1 = 1.0, double n2 = 1.5, bool reflective = false,
                       const QString& name = "Spherical");

    QPointF center() const { return m_center; }
    double radius() const { return m_radius; }
    void setCenter(const QPointF& c) { m_center = c; }
    void setRadius(double r) { m_radius = r; }

    bool intersect(const Ray& ray, double& t, QPointF& point, QVector2D& normal) const override;
    void process(Ray& ray, const QPointF& point, const QVector2D& normal) const override;
    QString info() const override;

private:
    QPointF m_center;
    double m_radius;
};

#endif // SPHERICALINTERFACE_H
