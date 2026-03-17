#include "straightinterface.h"
#include <QtMath>
#include <QLineF>
#include "constants.h"

StraightInterface::StraightInterface(const QPointF& p1, const QPointF& p2,
                                     double n1, double n2, bool reflective, const QString& name)
    : OpticalElement(name), m_p1(p1), m_p2(p2)
{
    setN1(n1);
    setN2(n2);
    setReflective(reflective);
    QVector2D vec(p2 - p1);
    m_direction = vec.normalized();
    m_normal = QVector2D(-m_direction.y(), m_direction.x()).normalized();
}

void StraightInterface::setPoints(const QPointF& p1, const QPointF& p2)
{
    m_p1 = p1;
    m_p2 = p2;
    QVector2D vec(p2 - p1);
    m_direction = vec.normalized();
    m_normal = QVector2D(-m_direction.y(), m_direction.x()).normalized();
}

bool StraightInterface::intersect(const Ray& ray, double& t, QPointF& point, QVector2D& normal) const
{
    QVector2D dir = ray.direction();
    QVector2D origin = QVector2D(ray.origin());

    double denominator = QVector2D::dotProduct(dir, m_normal);
    if (qFabs(denominator) < EPSILON)
        return false;

    QVector2D w = origin - QVector2D(m_p1);
    double t_intersect = -QVector2D::dotProduct(w, m_normal) / denominator;
    if (t_intersect <= EPSILON)
        return false;

    QPointF p = ray.pointAt(t_intersect);

    QVector2D v = QVector2D(p - m_p1);
    double dot = QVector2D::dotProduct(v, m_direction);
    double length = QVector2D(m_p2 - m_p1).length(); // или qSqrt(lengthSq)
    if (dot < -EPSILON || dot > length + EPSILON)
        return false;

    t = t_intersect;
    point = p;

    normal = m_normal;

   // if (QVector2D::dotProduct(dir, m_normal) > 0)
    //    normal = -m_normal;
   // else
   //     normal = m_normal;

    return true;
}

void StraightInterface::process(Ray& ray, const QPointF& point, const QVector2D& normal) const
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

QString StraightInterface::info() const
{
    return QString("%1: (%2,%3)-(%4,%5) n1=%6 n2=%7 %8")
    .arg(name())
        .arg(m_p1.x()).arg(m_p1.y())
        .arg(m_p2.x()).arg(m_p2.y())
        .arg(n1()).arg(n2())
        .arg(isReflective() ? "reflective" : "refractive");
}
