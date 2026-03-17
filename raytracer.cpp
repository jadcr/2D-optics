#include "raytracer.h"
#include "detector.h"
#include "mainwindow.h"

RayTracer::RayTracer(MainWindow* mainWindow)
    : m_scene(nullptr)
    , m_mainWindow(mainWindow)
{
}

void RayTracer::addRay(const Ray& ray)
{
    m_rays.append(ray);
}

void RayTracer::clearRays()
{
    m_rays.clear();
}

void RayTracer::traceAll(int maxBounces)
{
    if (!m_scene) return;

    for (OpticalElement* elem : m_scene->elements())
    {
        if (Detector* det = dynamic_cast<Detector*>(elem))
            det->clearHits();
    }

    for (Ray& ray : m_rays)
    {
        m_scene->traceRay(ray, maxBounces);

        if (ray.isActive())
        {
            if (ray.path().isEmpty())
                ray.addPoint(ray.origin());

            QPointF lastPoint = ray.path().last();
            QPointF end = lastPoint + ray.direction().toPointF() * m_defaultRayLength;
            ray.addPoint(end);
            ray.setActive(false);
        }
    }

    if (m_mainWindow)
        m_mainWindow->drawRays();
}
