#ifndef SCENE_H
#define SCENE_H

#include <QList>
#include <QRectF>
#include "opticalelement.h"
#include "ray.h"

class Scene
{
public:
    Scene();
    ~Scene();

    void addElement(OpticalElement* element);
    void removeElement(OpticalElement* element);
    void clearElements();
    const QList<OpticalElement*>& elements() const { return m_elements; }

    bool traceRay(Ray& ray, int maxBounces = 100);
    void setBounds(const QRectF& bounds) { m_bounds = bounds; }
    QRectF bounds() const { return m_bounds; }

private:
    QList<OpticalElement*> m_elements;
    QRectF m_bounds;
};

#endif // SCENE_H
