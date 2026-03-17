#ifndef OPTICALELEMENT_H
#define OPTICALELEMENT_H

#include <QString>
#include <QPointF>
#include <QVector2D>
#include "ray.h"

class OpticalElement
{
public:
    OpticalElement(const QString& name = "Element");
    virtual ~OpticalElement() = default;
    //геттеры/сеттеры
    QString name() const { return m_name; }
    void setName(const QString& name) { m_name = name; }

    double n1() const { return m_n1; }
    double n2() const { return m_n2; }
    void setN1(double n) { m_n1 = n; }
    void setN2(double n) { m_n2 = n; }
    bool isReflective() const { return m_reflective; }
    void setReflective(bool r) { m_reflective = r; }

    virtual bool intersect(const Ray& ray, double& t, QPointF& point, QVector2D& normal) const = 0;
    virtual void process(Ray& ray, const QPointF& point, const QVector2D& normal) const = 0;
    virtual QString info() const = 0;

protected:
    QString m_name;
    double m_n1 = 1.0;
    double m_n2 = 1.5;
    bool m_reflective = false;
};

#endif // OPTICALELEMENT_H
