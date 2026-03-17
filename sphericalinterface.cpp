#include "sphericalinterface.h"
#include <QtMath>
#include "constants.h"

SphericalInterface::SphericalInterface(const QPointF& center, double radius,
                                       double n1, double n2, bool reflective, const QString& name)
    : OpticalElement(name), m_center(center), m_radius(radius)
{
    setN1(n1);
    setN2(n2);
    setReflective(reflective);
}

bool SphericalInterface::intersect(const Ray& ray, double& t, QPointF& point, QVector2D& normal) const
{
    QVector2D dir = ray.direction();
    QVector2D oc = QVector2D(ray.origin() - m_center);

    double a = 1.0;
    double b = 2.0 * QVector2D::dotProduct(oc, dir);
    double c = QVector2D::dotProduct(oc, oc) - m_radius * m_radius;

    double discriminant = b * b - 4 * a * c;
    if (discriminant < -EPSILON)
        return false;
    if (discriminant < 0) discriminant = 0;

    double sqrtD = qSqrt(discriminant);
    double t1 = (-b - sqrtD) / (2 * a);
    double t2 = (-b + sqrtD) / (2 * a);

    double t_intersect = RAY_INFINITY;
    if (t1 > EPSILON && t1 < t_intersect) t_intersect = t1;
    if (t2 > EPSILON && t2 < t_intersect) t_intersect = t2;

    if (t_intersect >= RAY_INFINITY)
        return false;

    t = t_intersect;
    point = ray.pointAt(t);

    normal = QVector2D(point - m_center).normalized(); // всегда наружу
  //  QVector2D norm = QVector2D(point - m_center).normalized();

  //  if (QVector2D::dotProduct(dir, norm) > 0)
  //      norm = -norm;

  //  normal = norm;
    return true;
}

void SphericalInterface::process(Ray& ray, const QPointF& point, const QVector2D& normal) const
{
    QVector2D dir = ray.direction();
    double n_incident, n_transmit;

    double dot = QVector2D::dotProduct(dir, normal);
    double cosTheta1 = qAbs(dot);
    if (dot < -EPSILON) { // падает со стороны нормали
        n_incident = n1();
        n_transmit = n2();
        cosTheta1 = -dot;
    } else if (dot > EPSILON) { // падает с обратной стороны
        n_incident = n2();
        n_transmit = n1();
        cosTheta1 = dot;
    } else {
        ray.setActive(false);
        return;
    }

  //  double cosTheta1 = -QVector2D::dotProduct(dir, normal);

  //  if (cosTheta1 > 0) {
  //      n_incident = n1();
  //      n_transmit = n2();
  //  } else {
  //      n_incident = n2();
  //      n_transmit = n1();
  //      cosTheta1 = -cosTheta1;
  //  }

    if (isReflective()) {
        QVector2D r = dir - 2 * QVector2D::dotProduct(dir, normal) * normal;
        ray.setDirection(r);
    } else {
        double eta = n_incident / n_transmit;
        double sin2Theta2 = eta * eta * (1 - cosTheta1 * cosTheta1);
        if (sin2Theta2 > 1.0) {
            QVector2D r = dir - 2 * QVector2D::dotProduct(dir, normal) * normal;
            ray.setDirection(r);
        } else {
            double cosTheta2 = qSqrt(1 - sin2Theta2);
            QVector2D t = eta * dir + (eta * cosTheta1 - cosTheta2) * normal;
            ray.setDirection(t);
        }
    }
    ray.setActive(true);
}

QString SphericalInterface::info() const
{
    return QString("%1: center=(%2,%3) R=%4 n1=%5 n2=%6 %7")
    .arg(name())
        .arg(m_center.x()).arg(m_center.y())
        .arg(m_radius)
        .arg(n1()).arg(n2())
        .arg(isReflective() ? "reflective" : "refractive");
}
