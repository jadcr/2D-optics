#include "scene.h"
#include <limits>

Scene::Scene()
    : m_bounds(-100, -100, 200, 200)
{
}

Scene::~Scene()
{
    qDeleteAll(m_elements);
}

void Scene::addElement(OpticalElement* element)
{
    m_elements.append(element);
}

void Scene::removeElement(OpticalElement* element)
{
    m_elements.removeOne(element);
    delete element;
}

void Scene::clearElements()
{
    qDeleteAll(m_elements);
    m_elements.clear();
}

bool Scene::traceRay(Ray& ray, int maxBounces)
{
    if (!ray.isActive()) return false;

    for (int bounce = 0; bounce < maxBounces && ray.isActive(); ++bounce)
    {
        double closestT = std::numeric_limits<double>::max();
        OpticalElement* hitElement = nullptr;
        QPointF hitPoint;
        QVector2D hitNormal;

        for (OpticalElement* elem : m_elements)
        {
            double t;
            QPointF point;
            QVector2D normal;
            if (elem->intersect(ray, t, point, normal))
            {
                if (t > 1e-9 && t < closestT)
                {
                    closestT = t;
                    hitElement = elem;
                    hitPoint = point;
                    hitNormal = normal;
                }
            }
        }

        if (hitElement == nullptr)
        {
            break;
        }

        ray.propagate(closestT);
        hitElement->process(ray, hitPoint, hitNormal);
    }
    return true;
}
