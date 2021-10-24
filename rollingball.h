#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "octahedronball.h"
#include "trianglesurface.h"

class RollingBall : public OctahedronBall
{
public:
    RollingBall(int n);
    ~RollingBall() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move(float dt) override;
    void setSurface(VisualObject* surface) { triangle_surface = surface; }
    void barycentricCoordinates();
protected:
    VisualObject* triangle_surface;
private:
    gsml::Vector3d old_normal{0.0, 0.0, 1.0};
    int old_index{0};
    float gravity = -9.8f;
    gsml::Vector3d acceleration = {0, 0, 0};
    gsml::Vector3d velocity = {0, 0, 0};
    gsml::Vector3d gAcceleration = {0, 0, -9.8};
    gsml::Vector3d gForce = {0, 0, 0};
    float massKG = 0.05f;

};
#endif // ROLLINGBALL_H
