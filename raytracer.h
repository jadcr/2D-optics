#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QList>
#include "ray.h"
#include "scene.h"

class MainWindow;

class RayTracer
{
public:
    explicit RayTracer(MainWindow* mainWindow = nullptr);

    void setScene(Scene* scene) { m_scene = scene; }
    void addRay(const Ray& ray);
    void clearRays();
    const QList<Ray>& rays() const { return m_rays; }

    // Метод трассировки
    void traceAll(int maxBounces = 100);

    // Длина луча при отсутствии пересечений (по умолчанию 500)
    double m_defaultRayLength = 700.0;

private:
    Scene* m_scene;
    QList<Ray> m_rays;
    MainWindow* m_mainWindow;
};

#endif // RAYTRACER_H
